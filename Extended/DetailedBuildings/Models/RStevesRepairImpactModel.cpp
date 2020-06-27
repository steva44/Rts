/*********************************************************************
*                                                                    *
* This file is posted by Dr. Stevan Gavrilovic (steva44@hotmail.com) *
* as a result of work performed in the research group of Professor   *
* Terje Haukaas (terje@civil.ubc.ca) at the University of British    *
* Columbia in Vancouver. The code is part of the computer program    *
* Rts, which is an extension of the computer program Rt developed by *
* Dr. Mojtaba Mahsuli (mahsuli@sharif.edu) in the same group.        *
*                                                                    *
* The content of this file is the product of thesis-related work     *
* performed at the University of British Columbia (UBC). Therefore,  *
* the Rights and Ownership are held by UBC.                          *
*                                                                    *
* Please be cautious when using this code. It is provided “as is”    *
* without warranty of any kind, express or implied.                  *
*                                                                    *
* Contributors to this file:                                         *
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RStevesRepairImpactModel.h"
#include "RContinuousRandomVariable.h"
#include "RStructuralAnalysisModel.h"
#include "RComponent.h"
#include "RCSICostMethod.h"
#include "RCSISchedulingMethod.h"
#include "RGlulamMaterialKnowledge.h"
#include "RResponse.h"
#include "RInfoRichResponse.h"
#include "RShellSectionDamageRegion.h"
#include "RDamageMesh.h"
#include "RBIM.h"
#include "RJob.h"
#include "RMesh.h"
#include "RStatistics.h"
#include "RQuadRCSectionDamageRegion.h"
#include "RWSectionDamageRegion.h"
#include "RQuadSectionDamageRegion.h"
#include "RRectangularRCColumnComponent.h"
#include "RRCSlabComponent.h"
#include "RRCShearWallComponent.h"
#include "RWSteelBeamColumnComponent.h"
#include "RCompositePanelComponent.h"
#include "RRectangularGlulamBeamColumnComponent.h"
#include "RCLTPanelComponent.h"
#include "RShellSectionDamageRegion.h"

#include <QCoreApplication>

// Two types of refined demolition:
// 1) Selective Demolition:
//      Demolition of a selected building element,component, or finish, with some concern for surrounding or adjacent elements,
//      components, or finishes (see the first Subdivision (s) at thebeginning of appropriate Divisions).
//      This type of demolition is accomplishedby hand or pneumatic hand tools, and does not include handling, loading,storing, hauling, or disposal of the debris,
//      scaffolding, or shoring.′′Gutting′′methods may be used in order to save time, but damage that is caused to surrounding or adjacent elements, components, or
//      finishes may have tobe repaired at a later time

// 2) Cutout Demolition:
//      Demolition of a small quantity of floor, wall, roof, orother assembly, with concern for the appearance and structural integrity of the surrounding materials.
//      This type of demolition is accomplished byhand or pneumatic hand tools, and does not include saw cutting, handling,loading, hauling, or disposal of debris,
//      scaffolding, or shoring.

// Assume the use of type 1 for component replacement
// Assume the use of type 2 for cover replacement and spalling repairs

RStevesRepairImpactModel::RStevesRepairImpactModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theSAModelResponse = nullptr;
    theBIM = nullptr;
    theHeavyMachineryUseIntensity = nullptr;
    theWorkerTransportationIntensityAutomobil = nullptr;
    theWorkerTransportationIntensityDieselbus = nullptr;
    theWorkerTransportationIntensitySkytrain = nullptr;
    theMaterialTransportationIntensityConcrete = nullptr;
    theMaterialTransportationIntensitySteel = nullptr;
    theMaterialTransportationIntensityWood = nullptr;

    heavy_machinery_ratio_high_rise_wood = 0.1;
    heavy_machinery_ratio_high_rise_steel = 0.3;
    heavy_machinery_ratio_high_rise_concrete = 0.3;
    heavy_machinery_ratio_low_rise_wood = 0.05;
    heavy_machinery_ratio_low_rise_steel = 0.1;
    heavy_machinery_ratio_low_rise_concrete = 0.1;

    fractionWorkerTransportationAutomobil = 0.5;
    fractionWorkerTransportationDieselbus = 0.2;
    fractionWorkerTransportationSkytrain = 0.3;

    hours_per_shift = 8.0;

    // Create the response objects
    // Cost
    theDomain->createObject(objectName() + "CostResponse", "RGenericResponse");
    theCostResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theCostResponse->setModel(this);

    // Time
    theDomain->createObject(objectName() + "TimeResponse", "RGenericResponse");
    theTimeResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theTimeResponse->setModel(this);

    // Water
    theDomain->createObject(objectName() + "WaterResponse", "RGenericResponse");
    theWaterResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theWaterResponse->setModel(this);

    // Information-rich responses
    // Emissions
    theDomain->createObject(objectName() + "EmissionsResponse", "RInfoRichResponse");
    theEmissionsResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEmissionsResponse->setModel(this);

    // Energy
    theDomain->createObject(objectName() + "EnergyResponse", "RInfoRichResponse");
    theEnergyResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEnergyResponse->setModel(this);

    theScheduler = std::make_unique<RCSISchedulingMethod>(theDomain,"RepairImpactModelScheduler");
    theCSIAssembler = std::make_unique<RCSIAssembler>(theDomain,"RepairImpactCSIAssembler");

    theStatsMethod = theDomain->getStatsMethod();

    createDatabaseOfRandomVariables();
}


RStevesRepairImpactModel::~RStevesRepairImpactModel()
{

}


QObject * RStevesRepairImpactModel::getBIM() const
{
    return theBIM;
}


void RStevesRepairImpactModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


QObject* RStevesRepairImpactModel::getInputParameter() const
{
    return theSAModelResponse;
}


void RStevesRepairImpactModel::setInputParameter(QObject *value)
{

    theSAModelResponse = qobject_cast<RParameter*>(value);

    auto theSAModel = qobject_cast<RStructuralAnalysisModel*>(value);

    if(theSAModelResponse == nullptr || theSAModel)
    {
        qCritical()<< this->objectName()<<" needs a response from a  structural analysis model";
        return;
    }

    this->theAllParametersList.push_back(theSAModelResponse);

}


QList<RParameter *> RStevesRepairImpactModel::getAllParametersList(bool *ok)
{
    // Creating a list of parameters to return at the end
    QList<RParameter *> theParameterList;

    this->theAllParametersList.push_back(theSAModelResponse);

    return theParameterList;
}


int RStevesRepairImpactModel::evaluateModel(RGradientType theGradientType)
{
    // Clear the responses
    theCostResponse->setCurrentValue(0.0);
    theTimeResponse->setCurrentValue(0.0);
    theWaterResponse->setCurrentValue(0.0);
    // Reset the informationRich responses
    theEmissionsResponse->clearResponses();
    theEnergyResponse->clearResponses();
    theScheduler->reset();
    labourQuantityCSICodes.clear();
    materialQuantityCSICodes.clear();

    // Declare cost, energy, and time to be accumulated
    double accumulatedDirectCost = 0.0;

    // Declare the energy sources used in construction
    double JoulesDieselLightMedDutyTransport = 0.0;
    double JoulesDieselHeavyDutyTransport = 0.0;
    double JoulesGasolineLightMedDutyTransport = 0.0;
    double kWhElectricityTransport = 0.0;

    // Quick return if there is no visual damage
    // DO NOT COMMENT THIS OUT BELOW, OTHERWISE WILL EVALUATE EVERY SAMPLE
    if(theSAModelResponse->getCurrentValue() == 0.0)
        return 0;

    // Check that there is a BIM
    if(theBIM == nullptr)
    {
        qCritical() << this->objectName()<<" needs a BIM";
         return 0;
    }

    if (theOutputLevel >= RObject::Medium) {
        qDebug()<<"*********************************";
        qDebug()<<"*******Damage Repair Phase*******";
        qDebug()<<"*********************************";
    }

    // Get the cost method
    auto theCostMethod = theBIM->getTheCSICostMethod();

    // Set the cost-plus multiplier to multiply the bare costs with
    theCostMethod->setDemandSurge(1.2);

    // Get the components from the BIM
    auto theComponentList = theDomain->findChildren<RComponent*>();
    if(theComponentList.isEmpty())
    {
        qDebug()<<"There are no components in "<<__FUNCTION__;
        return -1;
    }

    // Keep the GUI responsive
    QCoreApplication::processEvents();

    // Create a job
    RJob repairPhase;
    repairPhase.setJobCode(theBIM->objectName()+"RepairPhase");

    auto totalTonsSteel = 0.0;
    auto totalVolumeConcreteYrds = 0.0;
    auto totalVolumeWoodYrds = 0.0;
    auto totalCfEpoxy = 0.0;

    // Damage can vary across the surface of the component
    // As a result, we have to account for the various damage types within each area of the component
    // But, if a critical damage type is reached, the component should be replaced
    // i.e., do not worry about fixing a crack in one area of the component if the remainder is severly damaged
    for(auto&& it : theComponentList)
    {
        bool componentReplacement = false;

        it->resetCSI();

        auto theRectConcreteColumn = qobject_cast<RRectangularRCColumnComponent*>(it);
        auto theConcreteShearwall= qobject_cast<RRCShearWallComponent*>(it);
        auto theConcreteSlab = qobject_cast<RRCSlabComponent*>(it);
        auto theSteelWBeamColumn = qobject_cast<RWSteelBeamColumnComponent*>(it);
        auto theCompositeDecking = qobject_cast<RCompositePanelComponent*>(it);
        auto theRectGlulamBeamColumn = qobject_cast<RRectangularGlulamBeamColumnComponent*>(it);
        auto theCLTPanel  = qobject_cast<RCLTPanelComponent*>(it);

        if(theRectConcreteColumn)
        {
            // Create a vector that holds the repair quantities for each damage type
            // For this type of component there are 7 damage types
            // Only worried about calculating the detailed damage quantities for numbers 1-3 - everything above 4 means the component is damaged beyond repair
            // Damage types are
            //      0) undamaged
            //      1) cracking - flexural, shear, torsion
            //      2) cover spalling
            //      3) cover loss
            //      4) core crushing due to axial/flexural
            //      5) reinforcement fracture
            //      6) reinforcement buckling

            auto concreteCoverThickness = theRectConcreteColumn->getConcreteCoverThickness()->getCurrentValue();

            auto longReinfDiameter = theRectConcreteColumn->getLongitudinalReinforcementDiameter();
            auto transReinfDiameter = theRectConcreteColumn->getTransverseReinforcementDiameter();

            auto crackedAreaTotal = 0.0;
            auto spalledAreaTotal = 0.0;
            auto coverLossAreaTotal = 0.0;

            std::vector<double> crackWidthVec;
            std::vector<double> crackLengthVec;
            std::vector<double> crackDepthVec;
            std::vector<double> crackVolumeVec;

            std::vector<double> spallAreaVec;
            std::vector<double> spallDepthVec;

            std::vector<double> coverAreaVec;
            std::vector<double> coverDepthVec;

            // Get the damage mesh from the component
            auto theDamageMesh = it->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region, calculate the damage quantities
            for(auto&& damageRegion : damageRegionVec)
            {
                if(componentReplacement)
                    break;

                auto beamColDamageRegion = dynamic_cast<RQuadRCSectionDamageRegion*>(damageRegion.get());

                if(!beamColDamageRegion)
                {
                    qCritical()<<"Error, requires a frame damage region in "<<__FUNCTION__;
                    return -1;
                }

                auto fibreContainers = beamColDamageRegion->getAllFibreContainers();

                for(auto&& fiberContainer : fibreContainers)
                {
                    // Get the damage type
                    auto damageType = fiberContainer->getGlobalDamageType();

                    // Get the damage area in m2
                    auto damagedArea = fiberContainer->getDamageSurfaceArea();

                    // Get the depth of damage in m
                    auto damagedDepth = fiberContainer->getDamageDepth();

                    if(damageType == 0)
                    {
                        continue;
                    }
                    // Cracking
                    else if(damageType == 1)
                    {
                        crackedAreaTotal += damagedArea;

                        auto crackWidths = fiberContainer->getCrackWidthVec();
                        auto crackLengths = fiberContainer->getCrackLengthVec();
                        auto crackDepths = fiberContainer->getCrackDepthVec();
                        auto crackVolumes = fiberContainer->getCrackVolumeVec();

                        //Save all crack parameters for all cracks in the component
                        crackWidthVec.insert(std::end(crackWidthVec), std::begin(crackWidths), std::end(crackWidths));
                        crackLengthVec.insert(std::end(crackLengthVec), std::begin(crackLengths), std::end(crackLengths));
                        crackDepthVec.insert(std::end(crackDepthVec), std::begin(crackDepths), std::end(crackDepths));
                        crackVolumeVec.insert(std::end(crackVolumeVec), std::begin(crackVolumes), std::end(crackVolumes));
                    }
                    // Spalling
                    else if(damageType == 2)
                    {
                        auto spallArea = damagedArea;
                        auto spallDepth = damagedDepth;

                        spalledAreaTotal += spallArea;

                        spallAreaVec.push_back(spallArea);
                        spallDepthVec.push_back(spallDepth);
                    }
                    // Cover delamination
                    else if(damageType == 3)
                    {
                        auto coverLossArea = damagedArea;
                        auto coverLossDepth = damagedDepth;

                        coverLossAreaTotal += coverLossArea;

                        coverAreaVec.push_back(coverLossArea);
                        coverDepthVec.push_back(coverLossDepth);
                    }
                    // Total component replacement
                    else if(damageType >= 4)
                    {
                        componentReplacement = true;
                        break;
                    }
                }
            }

            // Get the cost of component replacement, if its 0.0 it means that it needs to be calculated
            auto componentReplacementCost = it->getReplacementCost();
            if(componentReplacementCost == 0.0)
            {
                CSIVector& replacementLabourQuantityCSICodes = it->getReplacementLabourQuantityCSICodes();
                CSIVector& replacementMaterialQuantityCSICodes = it->getReplacementMaterialQuantityCSICodes();
                RJob& replacementJob = it->getReplacementJob();

                theCSIAssembler->getRectRCColReplacementCodes(theRectConcreteColumn,&replacementJob,&replacementMaterialQuantityCSICodes,&replacementLabourQuantityCSICodes);

                componentReplacementCost += theCostMethod->getLabourCosts(replacementLabourQuantityCSICodes);
                componentReplacementCost += theCostMethod->getMaterialCosts(replacementMaterialQuantityCSICodes);
                componentReplacementCost += theCostMethod->getEquipmentCosts(replacementLabourQuantityCSICodes);

                it->setReplacementCost(componentReplacementCost);
                it->setReplacementLabourQuantityCSICodes(replacementLabourQuantityCSICodes);
                it->setReplacementMaterialQuantityCSICodes(replacementMaterialQuantityCSICodes);
            }

            // First check to see if the component needs to be replaced, if it does, nothing else below matters
            if(componentReplacement)
            {
                auto replacementLabourQuantityCSICodes = it->getReplacementLabourQuantityCSICodes();
                auto replacementMaterialQuantityCSICodes = it->getReplacementMaterialQuantityCSICodes();
                auto replacementJob = it->getReplacementJob();

                this->commitRepair(&replacementLabourQuantityCSICodes,&replacementMaterialQuantityCSICodes,&replacementJob,&repairPhase, it, componentReplacementCost);

                it->setComponentReplaced(true);
            }
            else
            {
                // Start by calculating the cost of cover replacement
                CSIVector coverReplacementLabourCSICodes;
                CSIVector coverReplacementMaterialCSICodes;
                RJob coverReplacementJob;

                // Get the surface area of the component (m2)
                auto surfaceAreaColumn = 2.0*(theRectConcreteColumn->getSurfaceAreaWidth()+theRectConcreteColumn->getSurfaceAreaDepth());

                // Calculate the repair depth (m)
                auto repairDepth = concreteCoverThickness + longReinfDiameter + transReinfDiameter + 0.0254;

                // Calculate the repair volume (m3)
                auto repairVolume = repairDepth*surfaceAreaColumn;

                // CSI codes for cover replacement from the assembler - the repair volume is in yards and the surface area is in sq ft.
                theCSIAssembler->getRectRCColCoverReplacementCodes(repairVolume*1.30795,surfaceAreaColumn*10.7639,theRectConcreteColumn, &coverReplacementJob, &coverReplacementMaterialCSICodes, &coverReplacementLabourCSICodes);

                // Now calculate the cost of replacing the cover
                auto coverReplacementCost = theCostMethod->getLabourCosts(coverReplacementLabourCSICodes);
                coverReplacementCost += theCostMethod->getMaterialCosts(coverReplacementMaterialCSICodes);
                coverReplacementCost += theCostMethod->getEquipmentCosts(coverReplacementLabourCSICodes);

                // Now see how much it will cost to repair the damage
                CSIVector repairLabourQuantityCSICodes;
                CSIVector repairMaterialQuantityCSICodes;
                RJob repairJob;

                // First we get the repair quantities
                auto crackVol = 0.0;
                auto spallArea = 0.0;
                auto spallVol = 0.0;
                auto coverArea = 0.0;
                auto coverVol = 0.0;

                this->getRCComponentRepairQuantities(crackVolumeVec,crackVol,
                                                     spallAreaVec,spallDepthVec,spallArea,spallVol,
                                                     coverAreaVec,coverDepthVec,coverArea,coverVol);

                //Cracking damage
                if(crackVol != 0.0)
                {
                    // For epoxy injection
                    //    1.	Clean cracks
                    //    2.	Seal crack surfaces to prevent leakage
                    //    3.	Drill injection fitting holes
                    //    4.	Bond cracks with flush fitting that are not grooved
                    //    5.	Inject filler
                    //    6.    Remove surface seals

                    // Volume of crack in cubic feet
                    auto crackVolCF = crackVol*35.3147;

                    // CSI codes for epoxy crack repair
                    theCSIAssembler->getEpoxyCrackRepairCodes(crackVolCF,&repairMaterialQuantityCSICodes,&repairJob,&repairLabourQuantityCSICodes,theRectConcreteColumn);
                }
                // Spalling damage repair
                if(spallVol != 0.0)
                {
                    // For patching of cover
                    //    1.	Removal of old concrete (150mm min.) with light jack hammer or sufficient depth to reach at least 7mm, plus the dimension of the maximum size aggregate behind any reinforcement
                    //    2.	Clean with water blasting or sand blasting
                    //    3.	Placement
                    //    4.	Finishing
                    //    5.    Curing

                    // Get cost of removal of old concrete - unlike the slab and wall that have special cutout items, there is nothing for columns
                    // For the generic selective demo codes, we need the reinforcement ratio
                    auto longReinfRatio = theRectConcreteColumn->getTheLongitudinalReinforcementRatioParameter()->getCurrentValue();
                    auto transReinfRatio = theRectConcreteColumn->getTheTransverseReinforcementRatioParameter()->getCurrentValue();

                    auto maxReinfRatio = longReinfRatio > transReinfRatio ? longReinfRatio : transReinfRatio;

                    // Demolition volume in cubic yards
                    auto demoVol = spallVol*1.30795;

                    theCSIAssembler->getSelectiveConcreteDemolitionCodes(maxReinfRatio,demoVol,&repairJob,&repairLabourQuantityCSICodes,it);

                    // Include the removal and disposal of demolition debris - Needs to be in yards
                    theCSIAssembler->getConcreteRemovalDisposalCodes(demoVol,&repairJob,&repairLabourQuantityCSICodes,it);

                    // Volume patch needs to be in cubic feet
                    auto volumePatch = spallVol*35.3147;

                    // Area patch needs to be in square foot
                    auto areaPatch = spallArea*10.7639;

                    // CSI codes for placement/finishing/curing of repair material
                    theCSIAssembler->getWallPatchCoverCodes(areaPatch,volumePatch,theRectConcreteColumn,&repairJob,&repairMaterialQuantityCSICodes,&repairLabourQuantityCSICodes);
                }

                // Cover loss damage repair
                if(coverVol != 0.0)
                {
                    // Volume in yards
                    // Surface area in sq. ft.
                    // CSI codes for placement/finishing/curing of repair material
                    theCSIAssembler->getRectRCColCoverReplacementCodes(coverVol*1.30795,coverArea*10.7639,theRectConcreteColumn, &repairJob, &repairMaterialQuantityCSICodes, &repairLabourQuantityCSICodes);
                }

                // Calculate the repair cost
                auto repairCost = theCostMethod->getLabourCosts(repairLabourQuantityCSICodes);
                repairCost += theCostMethod->getMaterialCosts(repairMaterialQuantityCSICodes);
                repairCost += theCostMethod->getEquipmentCosts(repairLabourQuantityCSICodes);

                // Check to see if there is anything to repair, avoid unnecessary evaluation of the code below
                if(repairCost < 10.0)
                    continue;

                // First check to see if the repair/replacement cost does not exceed the replacement cost!
                if(componentReplacementCost < repairCost)
                {
                    auto replacementLabourQuantityCSICodes = it->getReplacementLabourQuantityCSICodes();
                    auto replacementMaterialQuantityCSICodes = it->getReplacementMaterialQuantityCSICodes();
                    auto replacementJob = it->getReplacementJob();

                    this->commitRepair(&replacementLabourQuantityCSICodes,&replacementMaterialQuantityCSICodes,&replacementJob,&repairPhase, it, componentReplacementCost);

                    it->setComponentReplaced(true);
                }
                // It is better to repair than to replace
                else
                {
                    //If its cheaper to repair the cover, then do that... otherwise repair the damage.
                    if(coverReplacementCost < repairCost)
                    {
                        this->commitRepair(&coverReplacementLabourCSICodes,&coverReplacementMaterialCSICodes,&coverReplacementJob,&repairPhase, it, coverReplacementCost);
                    }
                    else
                    {
                        this->commitRepair(&repairLabourQuantityCSICodes,&repairMaterialQuantityCSICodes,&repairJob,&repairPhase, it, repairCost);
                    }
                }
            }
        }
        else if(theConcreteSlab)
        {
            // Create a vector that holds the repair quantities for each damage type
            // For this component, there are 8 damage types
            // But, we are only worried about tracking the damage quantities for numbers 1-3
            // Everything above 4 means the component is damaged beyond repair
            // Damage types are:
            //      0) undamaged
            //      1) cracking - flexural, shear, torsion
            //      2) cover spalling
            //      3) cover loss
            //      4) crushing due to axial/flexural
            //      5) reinforcement fracture
            //      6) reinforcement buckling
            //      7) shear failure

            // Get some parameter values from the component
            auto coverThickness = theConcreteSlab->getConcreteCoverThickness()->getCurrentValue();
            auto longReinfRatio = theConcreteSlab->getTheLongitudinalReinforcementRatioParameter()->getCurrentValue();
            auto transReinfRatio = theConcreteSlab->getTheTransverseReinforcementRatioParameter()->getCurrentValue();
            auto reinfRatio = longReinfRatio > transReinfRatio ? longReinfRatio : transReinfRatio;
            auto longReinfDiameter = theConcreteSlab->getLongitudinalReinforcementDiameter();
            auto transReinfDiameter = theConcreteSlab->getTransverseReinforcementDiameter();

            // Create some data members to store the damage
            auto crackedAreaTopTotal = 0.0;               auto crackedAreaBotTotal = 0.0;
            auto spalledAreaTopTotal = 0.0;               auto spalledAreaBotTotal = 0.0;
            auto coverLossAreaTopTotal = 0.0;             auto coverLossAreaBotTotal = 0.0;

            std::vector<double> crackWidthVecTop;         std::vector<double> crackWidthVecBot;
            std::vector<double> crackLengthVecTop;        std::vector<double> crackLengthVecBot;
            std::vector<double> crackDepthVecTop;         std::vector<double> crackDepthVecBot;
            std::vector<double> crackVolumeVecTop;        std::vector<double> crackVolumeVecBot;
            std::vector<double> spallAreaVecTop;          std::vector<double> coverAreaVecTop;
            std::vector<double> spallDepthVecTop;         std::vector<double> coverDepthVecTop;
            std::vector<double> spallAreaVecBot;          std::vector<double> coverAreaVecBot;
            std::vector<double> spallDepthVecBot;         std::vector<double> coverDepthVecBot;

            // Get the damage mesh from the component
            auto theDamageMesh = it->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region, get the visual damage
            for(auto&& damageRegion : damageRegionVec)
            {
                // Get the shell damage region
                auto shellDamageRegion = dynamic_cast<RShellSectionDamageRegion*>(damageRegion.get());

                if(!shellDamageRegion)
                {
                    qCritical()<<"Error, should be a shell damage region in "<<__FUNCTION__;
                    return -1;
                }

                // Get the damage quantities from the damage region
                this->getRCShellDamageQuantities(shellDamageRegion, componentReplacement, crackedAreaTopTotal,spalledAreaTopTotal, coverLossAreaTopTotal,
                                                 crackWidthVecTop, crackLengthVecTop, crackDepthVecTop, crackVolumeVecTop,
                                                 spallAreaVecTop, spallDepthVecTop,coverAreaVecTop, coverDepthVecTop,
                                                 crackedAreaBotTotal, spalledAreaBotTotal, coverLossAreaBotTotal,
                                                 crackWidthVecBot,crackLengthVecBot,  crackDepthVecBot, crackVolumeVecBot,
                                                 spallAreaVecBot, spallDepthVecBot, coverAreaVecBot,coverDepthVecBot,2,5);

                // Break out if any damage region signifies replacement of component
                if(componentReplacement)
                    break;
            }

            // Calculate the cost of component replacement, if we do not have this already.
            auto componentReplacementCost = it->getReplacementCost();
            if(componentReplacementCost == 0.0)
            {
                CSIVector& replacementLabourQuantityCSICodes = it->getReplacementLabourQuantityCSICodes();
                CSIVector& replacementMaterialQuantityCSICodes = it->getReplacementMaterialQuantityCSICodes();
                RJob& replacementJob = it->getReplacementJob();

                theCSIAssembler->getRCSlabReplacementCodes(theConcreteSlab,theBIM,&replacementJob,&replacementMaterialQuantityCSICodes,&replacementLabourQuantityCSICodes);

                componentReplacementCost += theCostMethod->getLabourCosts(replacementLabourQuantityCSICodes);
                componentReplacementCost += theCostMethod->getMaterialCosts(replacementMaterialQuantityCSICodes);
                componentReplacementCost += theCostMethod->getEquipmentCosts(replacementLabourQuantityCSICodes);

                it->setReplacementCost(componentReplacementCost);
                it->setReplacementLabourQuantityCSICodes(replacementLabourQuantityCSICodes);
                it->setReplacementMaterialQuantityCSICodes(replacementMaterialQuantityCSICodes);
            }

            // If the damage is severe, and the component requires replacement, dont check anything else
            if(componentReplacement)
            {
                auto replacementLabourQuantityCSICodes = it->getReplacementLabourQuantityCSICodes();
                auto replacementMaterialQuantityCSICodes = it->getReplacementMaterialQuantityCSICodes();
                auto replacementJob = it->getReplacementJob();

                this->commitRepair(&replacementLabourQuantityCSICodes,&replacementMaterialQuantityCSICodes,&replacementJob,&repairPhase, it, componentReplacementCost);

                it->setComponentReplaced(true);
            }
            // The component requires repair
            else
            {
                //First we need to calculate the cost of replacing the cover on the top and bottom
                // Get the plan area of the slab, i.e., the area of the top and bottom surfaces
                auto areaTop = theConcreteSlab->getPlanArea();
                auto areaBot = theConcreteSlab->getPlanArea();

                // Calculate the repair depth (m)
                auto repairDepth = coverThickness + longReinfDiameter + transReinfDiameter + 0.0254;

                // Calculate the repair volume (m3)
                auto repairVolume = repairDepth*areaTop;

                CSIVector topCoverReplacementLabourCSICodes;
                CSIVector topCoverReplacementMaterialCSICodes;
                RJob topCoverReplacementJob;

                // CSI codes for replacing the top cover
                theCSIAssembler->getRCSlabTopCoverReplacementCodes(repairVolume*35.3147,
                                                                   repairDepth*39.3701,
                                                                   areaTop*35.3147,
                                                                   &topCoverReplacementJob,
                                                                   &topCoverReplacementLabourCSICodes,
                                                                   &topCoverReplacementMaterialCSICodes,
                                                                   theConcreteSlab);

                CSIVector bottomCoverReplacementLabourCSICodes;
                CSIVector bottomCoverReplacementMaterialCSICodes;
                RJob bottomCoverReplacementJob;

                // CSI codes for replacing the bottom cover
                theCSIAssembler->getRCSlabBottomCoverReplacementCodes(repairVolume*35.3147,
                                                                      repairDepth*39.3701,
                                                                      areaBot*35.3147,
                                                                      &bottomCoverReplacementJob,
                                                                      &bottomCoverReplacementLabourCSICodes,
                                                                      &bottomCoverReplacementMaterialCSICodes,
                                                                      theConcreteSlab);

                // Calculate the cost of replacing the top cover
                auto topCoverReplacementCost = theCostMethod->getLabourCosts(topCoverReplacementLabourCSICodes);
                topCoverReplacementCost += theCostMethod->getMaterialCosts(topCoverReplacementMaterialCSICodes);
                topCoverReplacementCost += theCostMethod->getEquipmentCosts(topCoverReplacementLabourCSICodes);

                // Calculate the cost of replacing the bottom cover
                auto bottomCoverReplacementCost = theCostMethod->getLabourCosts(bottomCoverReplacementLabourCSICodes);
                bottomCoverReplacementCost += theCostMethod->getMaterialCosts(bottomCoverReplacementMaterialCSICodes);
                bottomCoverReplacementCost += theCostMethod->getEquipmentCosts(bottomCoverReplacementLabourCSICodes);

                // Get the cost of repairing the top (floor) of the slab
                CSIVector repairTopLabourQuantityCSICodes;
                CSIVector repairTopMaterialQuantityCSICodes;
                RJob repairTopJob;

                // Get the cost of repairing the bottom (ceiling) of the slab
                CSIVector repairBottomLabourQuantityCSICodes;
                CSIVector repairBottomMaterialQuantityCSICodes;
                RJob repairBottomJob;

                auto crackVolTop = 0.0;   auto spallVolTop = 0.0;    auto spallAreaTop = 0.0;
                auto crackVolBot = 0.0;   auto spallVolBot = 0.0;    auto spallAreaBot = 0.0;
                auto coverVolTop = 0.0;   auto coverAreaTop = 0.0;
                auto coverVolBot = 0.0;   auto coverAreaBot = 0.0;

                // Get the repair quantities for the top
                this->getRCComponentRepairQuantities(crackVolumeVecTop,crackVolTop,
                                                     spallAreaVecTop,spallDepthVecTop,spallAreaTop,spallVolTop,
                                                     coverAreaVecTop,coverDepthVecTop,coverAreaTop,coverVolTop);

                // Get the repair quantities for the bottom
                this->getRCComponentRepairQuantities(crackVolumeVecBot,crackVolBot,
                                                     spallAreaVecBot,spallDepthVecBot,spallAreaBot,spallVolBot,
                                                     coverAreaVecBot,coverDepthVecBot,coverAreaBot,coverVolBot);

                if(crackVolTop != 0.0) // Cracking damage
                {
                    // For epoxy injection
                    //    1.	Clean cracks
                    //    2.	Seal crack surfaces to prevent leakage
                    //    3.	Drill injection fitting holes
                    //    4.	Bond cracks with flush fitting that are not grooved
                    //    5.	Inject filler
                    //    6.    Remove surface seals

                    // Volume in cubic square foot
                    auto crackVolCF = crackVolTop*35.3147;

                    // CSI codes for epoxy crack repair
                    theCSIAssembler->getEpoxyCrackRepairCodes(crackVolCF,&repairTopMaterialQuantityCSICodes,&repairTopJob,&repairTopLabourQuantityCSICodes,theConcreteSlab);
                }
                if(crackVolBot != 0.0) // Cracking damage
                {
                    // For epoxy injection
                    //    1.	Clean cracks
                    //    2.	Seal crack surfaces to prevent leakage
                    //    3.	Drill injection fitting holes
                    //    4.	Bond cracks with flush fitting that are not grooved
                    //    5.	Inject filler
                    //    6.    Remove surface seals

                    // Volume in cubic square foot
                    auto crackVolCF = crackVolBot*35.3147;

                    // CSI codes for epoxy crack repair
                    theCSIAssembler->getEpoxyCrackRepairCodes(crackVolCF,&repairBottomMaterialQuantityCSICodes,&repairBottomJob,&repairBottomLabourQuantityCSICodes,theConcreteSlab);
                }
                if(spallVolTop != 0.0) //Patch spalling damage
                {
                    // For patching of cover
                    //    1.	Removal of old concrete
                    //    2.	Clean with water blasting or sand blasting
                    //    3.	Placement
                    //    4.	Finishing
                    //    5.    Curing

                    // Volume of repair in cubic feet
                    auto volumePatch = spallVolTop*35.3147;

                    theCSIAssembler->getConcreteSlabCutOutDemolitionCodes(reinfRatio,volumePatch,&repairTopJob,&repairTopLabourQuantityCSICodes,it);

                    // Area of repair in square foot
                    auto areaPatch = spallAreaTop*10.7639;

                    // CSI codes for placement/finishing/curing of repair material
                    theCSIAssembler->getSlabSpallPatchCodes(areaPatch,volumePatch,theConcreteSlab,&repairTopJob,&repairTopMaterialQuantityCSICodes,&repairTopLabourQuantityCSICodes);

                    // Concrete Disposal - needs to be in cubic yards
                    auto demoVol = spallVolTop*1.30795;

                    // CSI codes for damaged concrete removal and disposal
                    theCSIAssembler->getConcreteRemovalDisposalCodes(demoVol,&repairTopJob,&repairTopLabourQuantityCSICodes,it);
                }
                if(spallVolBot != 0.0) // Patch spalling damage
                {
                    // For patching of cover
                    //    1.	Removal of old concrete
                    //    2.	Clean with water blasting or sand blasting
                    //    3.	Placement
                    //    4.	Finishing
                    //    5.    Curing

                    // Volume of repair in cubic feet
                    auto volumePatch = spallVolBot*35.3147;

                    theCSIAssembler->getConcreteSlabCutOutDemolitionCodes(reinfRatio,volumePatch,&repairBottomJob,&repairBottomLabourQuantityCSICodes,it);

                    // Area of repair in square foot
                    auto areaPatch = spallAreaBot*10.7639;

                    // CSI codes for placement/finishing/curing of repair material
                    theCSIAssembler->getSlabSpallPatchCodes(areaPatch,volumePatch,theConcreteSlab,&repairBottomJob,&repairBottomMaterialQuantityCSICodes,&repairBottomLabourQuantityCSICodes);

                    // Concrete Disposal - needs to be in cubic yards
                    auto demoVol = spallVolBot*1.30795;

                    // CSI codes for damaged concrete removal and disposal
                    theCSIAssembler->getConcreteRemovalDisposalCodes(demoVol,&repairBottomJob,&repairBottomLabourQuantityCSICodes,it);
                }
                // Concrete cover damage on top of slab
                if(coverVolTop != 0.0)
                {
                    // Get the replacement codes for the top cover repairs
                    // Volume in cubic feet, depth in inches, and area in sq. ft.
                    theCSIAssembler->getRCSlabTopCoverReplacementCodes(coverVolTop*35.3147,
                                                                       repairDepth*39.3701,
                                                                       coverAreaTop*10.7639,
                                                                       &repairTopJob,
                                                                       &repairTopLabourQuantityCSICodes,
                                                                       &repairTopMaterialQuantityCSICodes,
                                                                       theConcreteSlab);
                }
                // Concrete cover damage on bottom of slab
                // For fixing the underside of slabs -- can only use spray concrete, no forms
                if(coverVolBot != 0.0)
                {

                    theCSIAssembler->getRCSlabBottomCoverReplacementCodes(coverVolBot*35.3147,
                                                                          repairDepth*39.3701*1.15,
                                                                          coverAreaBot*10.7639,
                                                                          &repairTopJob,
                                                                          &repairTopLabourQuantityCSICodes,
                                                                          &repairTopMaterialQuantityCSICodes,
                                                                          theConcreteSlab);
                }

                // Get the cost of repairing the top (floor) of the slab
                auto repairCostTop = theCostMethod->getLabourCosts(repairTopLabourQuantityCSICodes);
                repairCostTop += theCostMethod->getMaterialCosts(repairTopMaterialQuantityCSICodes);
                repairCostTop += theCostMethod->getEquipmentCosts(repairTopLabourQuantityCSICodes);

                // Get the cost of repairing the top (floor) of the slab
                auto repairCostBottom = theCostMethod->getLabourCosts(repairBottomLabourQuantityCSICodes);
                repairCostBottom += theCostMethod->getMaterialCosts(repairBottomMaterialQuantityCSICodes);
                repairCostBottom += theCostMethod->getEquipmentCosts(repairBottomLabourQuantityCSICodes);

                double totalTopRepairCost = topCoverReplacementCost < repairCostTop ? topCoverReplacementCost : repairCostTop;
                double totalBottomRepairCost = bottomCoverReplacementCost < repairCostBottom ? bottomCoverReplacementCost : repairCostBottom;

                // First check to see if the repair/replacement cost does not exceed the replacement cost!
                if(componentReplacementCost < totalTopRepairCost + totalBottomRepairCost)
                {
                    auto replacementLabourQuantityCSICodes = it->getReplacementLabourQuantityCSICodes();
                    auto replacementMaterialQuantityCSICodes = it->getReplacementMaterialQuantityCSICodes();
                    auto replacementJob = it->getReplacementJob();

                    this->commitRepair(&replacementLabourQuantityCSICodes,&replacementMaterialQuantityCSICodes,&replacementJob,&repairPhase, it, componentReplacementCost);

                    it->setComponentReplaced(true);
                }
                else
                {
                    // Else do the cheaper of the repairs or cover replacements - it could be that cover replacement is cheaper than the repairs
                    if(topCoverReplacementCost < repairCostTop)
                    {
                        this->commitRepair(&topCoverReplacementLabourCSICodes,&topCoverReplacementMaterialCSICodes,&topCoverReplacementJob,&repairPhase, it, topCoverReplacementCost);
                    }
                    else if(repairCostTop > 1.0)
                    {
                        this->commitRepair(&repairTopLabourQuantityCSICodes,&repairTopMaterialQuantityCSICodes,&repairTopJob,&repairPhase, it, repairCostTop);
                    }

                    if(bottomCoverReplacementCost < repairCostBottom)
                    {
                        this->commitRepair(&bottomCoverReplacementLabourCSICodes,&bottomCoverReplacementMaterialCSICodes,&bottomCoverReplacementJob,&repairPhase, it, bottomCoverReplacementCost);
                    }
                    else if(repairCostBottom > 1.0)
                    {
                        this->commitRepair(&repairBottomLabourQuantityCSICodes,&repairBottomMaterialQuantityCSICodes,&repairBottomJob,&repairPhase, it, repairCostBottom);
                    }
                }
            }
        }
        else if(theConcreteShearwall)
        {
            // Create a vector that holds the repair quantities for each damage type
            // For this component, there are 8 damage types
            // But, we are only worried about tracking the damage quantities for numbers 1-3
            // Everything above 4 means the component is damaged beyond repair
            // Visual damage types are
            //      0) undamaged
            //      1) cracking - flexural, shear, torsion
            //      2) cover spalling
            //      3) cover loss
            //      4) crushing due to axial/flexural
            //      5) reinforcement fracture
            //      6) reinforcement buckling
            //      7) shear failure (not implemented)

            // Get the wall dimensions
            auto coverThickness = theConcreteShearwall->getConcreteCoverThickness()->getCurrentValue(); // In m
            auto longReinfDiameter = theConcreteShearwall->getLongitudinalReinforcementDiameter(); // In m
            auto transReinfDiameter = theConcreteShearwall->getTransverseReinforcementDiameter(); // In m
            auto wallLength = theConcreteShearwall->getLengthValue()*3.28084; // In feet

            auto crackedAreaFrontTotal = 0.0;               auto crackedAreaBackTotal = 0.0;
            auto spalledAreaFrontTotal = 0.0;               auto spalledAreaBackTotal = 0.0;
            auto coverLossAreaFrontTotal = 0.0;             auto coverLossAreaBackTotal = 0.0;

            std::vector<double> crackWidthVecFront;         std::vector<double> crackWidthVecBack;
            std::vector<double> crackLengthVecFront;        std::vector<double> crackLengthVecBack;
            std::vector<double> crackDepthVecFront;         std::vector<double> crackDepthVecBack;
            std::vector<double> crackVolumeVecFront;        std::vector<double> crackVolumeVecBack;

            std::vector<double> spallAreaVecFront;          std::vector<double> coverAreaVecFront;
            std::vector<double> spallDepthVecFront;         std::vector<double> coverDepthVecFront;

            std::vector<double> spallAreaVecBack;          std::vector<double> coverAreaVecBack;
            std::vector<double> spallDepthVecBack;         std::vector<double> coverDepthVecBack;

            // Get the damage mesh from the component
            auto theDamageMesh = it->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            for(auto&& damageRegion : damageRegionVec)
            {
                // Get the shell damage region
                auto shellDamageRegion = dynamic_cast<RShellSectionDamageRegion*>(damageRegion.get());

                if(!shellDamageRegion)
                {
                    qCritical()<<"Error, should be a shell damage region in "<<__FUNCTION__;
                    return -1;
                }

                // Get the damage quantities from the damage region
                this->getRCShellDamageQuantities(shellDamageRegion, componentReplacement, crackedAreaFrontTotal,spalledAreaFrontTotal, coverLossAreaFrontTotal,
                                                 crackWidthVecFront, crackLengthVecFront, crackDepthVecFront, crackVolumeVecFront,
                                                 spallAreaVecFront, spallDepthVecFront,coverAreaVecFront, coverDepthVecFront,
                                                 crackedAreaBackTotal, spalledAreaBackTotal, coverLossAreaBackTotal,
                                                 crackWidthVecBack,crackLengthVecBack,  crackDepthVecBack, crackVolumeVecBack,
                                                 spallAreaVecBack, spallDepthVecBack, coverAreaVecBack,coverDepthVecBack,4,1);

                if(componentReplacement)
                    break;
            }

            // Calculate the cost of component replacement cost, if we do not have this already.
            auto componentReplacementCost = it->getReplacementCost();
            if(componentReplacementCost == 0.0)
            {
                CSIVector& replacementLabourQuantityCSICodes = it->getReplacementLabourQuantityCSICodes();
                CSIVector& replacementMaterialQuantityCSICodes = it->getReplacementMaterialQuantityCSICodes();
                RJob& replacementJob = it->getReplacementJob();

                theCSIAssembler->getRCShearWallReplacementCodes(theConcreteShearwall,theBIM,&replacementJob,&replacementMaterialQuantityCSICodes,&replacementLabourQuantityCSICodes);

                componentReplacementCost += theCostMethod->getLabourCosts(replacementLabourQuantityCSICodes);
                componentReplacementCost += theCostMethod->getMaterialCosts(replacementMaterialQuantityCSICodes);
                componentReplacementCost += theCostMethod->getEquipmentCosts(replacementLabourQuantityCSICodes);

                it->setReplacementCost(componentReplacementCost);
            }

            // If the damage type is greater than some limit, then the component will be replaced. It will also be replaced if the repair cost is greater than its replacement cost.
            if(componentReplacement)
            {
                auto replacementLabourQuantityCSICodes = it->getReplacementLabourQuantityCSICodes();
                auto replacementMaterialQuantityCSICodes = it->getReplacementMaterialQuantityCSICodes();
                auto replacementJob = it->getReplacementJob();

                this->commitRepair(&replacementLabourQuantityCSICodes,&replacementMaterialQuantityCSICodes,&replacementJob,&repairPhase, it, componentReplacementCost);

                it->setComponentReplaced(true);
            }
            else
            {
                // Since this is a wall, it should cost the same to replace the cover on both sides
                // Calculate the cost of cover replacement for each side - elevation area in m2
                auto elevationArea = theConcreteShearwall->getElevationArea();

                CSIVector coverReplacementLabourCSICodes;
                CSIVector coverReplacementMaterialCSICodes;
                RJob coverReplacementJob;

                // Calculate the repair depth (m)
                auto repairDepth = coverThickness + longReinfDiameter + transReinfDiameter + 0.0254;

                // Calculate the repair volume (m3)
                auto repairVolume = repairDepth*elevationArea;

                CSIVector frontCoverReplacementLabourCSICodes;
                CSIVector frontCoverReplacementMaterialCSICodes;
                RJob frontCoverReplacementJob;

                // Volume in Cubic Yards
                // Area in square foot
                // Depth of the repair in inches
                // CSI codes for the replacement of the wall cover
                theCSIAssembler->getRCWallCoverReplacementCodes(repairVolume*1.30795, repairDepth*39.3701, elevationArea*10.7639, &coverReplacementJob, &coverReplacementLabourCSICodes,&coverReplacementMaterialCSICodes,theConcreteShearwall);

                auto coverReplacementCost = theCostMethod->getLabourCosts(coverReplacementLabourCSICodes);
                coverReplacementCost += theCostMethod->getMaterialCosts(coverReplacementMaterialCSICodes);
                coverReplacementCost += theCostMethod->getEquipmentCosts(coverReplacementLabourCSICodes);

                // Get the cost of repairing the front of the wall
                CSIVector repairFrontLabourQuantityCSICodes;
                CSIVector repairFrontMaterialQuantityCSICodes;
                RJob repairFrontJob;

                // Get the cost of repairing the Back of the wall
                CSIVector repairBackLabourQuantityCSICodes;
                CSIVector repairBackMaterialQuantityCSICodes;
                RJob repairBackJob;

                auto crackVolFront = 0.0;   auto spallVolFront = 0.0;    auto spallAreaFront = 0.0;
                auto crackVolBack = 0.0;   auto spallVolBack = 0.0;    auto spallAreaBack = 0.0;
                auto coverVolFront = 0.0;   auto coverAreaFront = 0.0;
                auto coverVolBack = 0.0;   auto coverAreaBack = 0.0;

                // Get the repair quantities for the front
                this->getRCComponentRepairQuantities(crackVolumeVecFront,crackVolFront,
                                                     spallAreaVecFront,spallDepthVecFront,spallAreaFront,spallVolFront,
                                                     coverAreaVecFront,coverDepthVecFront,coverAreaFront,coverVolFront);

                // Get the repair quantities for the back
                this->getRCComponentRepairQuantities(crackVolumeVecBack,crackVolBack,
                                                     spallAreaVecBack,spallDepthVecBack,spallAreaBack,spallVolBack,
                                                     coverAreaVecBack,coverDepthVecBack,coverAreaBack,coverVolBack);

                // Cracking damage repair front of wall
                if(crackVolFront != 0.0)
                {
                    // For epoxy injection:
                    //    1.	Clean cracks
                    //    2.	Seal crack surfaces to prevent leakage
                    //    3.	Drill injection fitting holes
                    //    4.	Bond cracks with flush fitting that are not grooved
                    //    5.	Inject filler
                    //    6.    Remove surface seals

                    // Volume in cubic foot
                    auto crackVolCF = crackVolFront*35.3147;

                    // CSI codes for epoxy crack repair
                    theCSIAssembler->getEpoxyCrackRepairCodes(crackVolCF,&repairFrontMaterialQuantityCSICodes,&repairFrontJob,&repairFrontLabourQuantityCSICodes,theConcreteShearwall);
                }

                // Cracking damage repair back of wall
                if(crackVolBack != 0.0)
                {
                    // For epoxy injection:
                    //    1.	Clean cracks
                    //    2.	Seal crack surfaces to prevent leakage
                    //    3.	Drill injection fitting holes
                    //    4.	Bond cracks with flush fitting that are not grooved
                    //    5.	Inject filler
                    //    6.    Remove surface seals

                    // Volume in cubic foot
                    auto crackVolCF = crackVolBack*35.3147;

                    // CSI codes for epoxy crack repair
                    theCSIAssembler->getEpoxyCrackRepairCodes(crackVolCF,&repairBackMaterialQuantityCSICodes,&repairBackJob,&repairBackLabourQuantityCSICodes,theConcreteShearwall);
                }

                // Patch spalling damage back of wall
                if(spallVolFront != 0.0)
                {
                    // For patching of cover:
                    //    1.	Removal of old concrete (150mm min.) with light jack hammer or sufficient depth to reach at least 7mm, plus the dimension of the maximum size aggregate behind any reinforcement
                    //    2.	Clean with water blasting or sand blasting
                    //    3.	Placement
                    //    4.	Finishing
                    //    5.    Curing

                    // Volume of repair in cubic feet
                    auto volumePatch = spallVolFront*35.3147;

                    theCSIAssembler->getConcreteWallCutOutDemolitionCodes(volumePatch,wallLength,&repairFrontJob,&repairFrontLabourQuantityCSICodes,it);

                    // Area of repair in square foot
                    auto areaPatch = spallAreaFront*10.7639;

                    // CSI codes for placement/finishing/curing of repair material
                    theCSIAssembler->getWallPatchCoverCodes(areaPatch,volumePatch,theConcreteShearwall,&repairFrontJob,&repairFrontMaterialQuantityCSICodes,&repairFrontLabourQuantityCSICodes);

                    // Disposal of demolition debris in yards
                    auto demoVol = spallVolBack*1.30795;

                    // CSI codes for damaged concrete removal and disposal
                    theCSIAssembler->getConcreteRemovalDisposalCodes(demoVol,&repairFrontJob,&repairFrontLabourQuantityCSICodes,it);

                    // qDebug()<<"Repairing Spalling on Front of wall";
                }

                // Patch spalling damage back of wall
                if(spallVolBack != 0.0)
                {
                    // For patching of cover:
                    //    1.	Removal of old concrete (150mm min.) with light jack hammer or sufficient depth to reach at least 7mm, plus the dimension of the maximum size aggregate behind any reinforcement
                    //    2.	Clean with water blasting or sand blasting
                    //    3.	Placement
                    //    4.	Finishing
                    //    5.    Curing

                    // Volume of repair in cubic feet
                    auto volumePatch = spallVolBack*35.3147;

                    theCSIAssembler->getConcreteWallCutOutDemolitionCodes(volumePatch,wallLength,&repairBackJob,&repairBackLabourQuantityCSICodes,it);

                    // Area of repair in square foot
                    auto areaPatch = spallAreaBack*10.7639;

                    // CSI codes for placement/finishing/curing of repair material
                    theCSIAssembler->getWallPatchCoverCodes(areaPatch,volumePatch,theConcreteShearwall,&repairBackJob,&repairBackMaterialQuantityCSICodes,&repairBackLabourQuantityCSICodes);

                    // Disposal of demolition debris in yards
                    auto demoVol = spallVolBack*1.30795;

                    // CSI codes for damaged concrete removal and disposal
                    theCSIAssembler->getConcreteRemovalDisposalCodes(demoVol,&repairBackJob,&repairBackLabourQuantityCSICodes,it);

                    // qDebug()<<"Repairing Spalling on Back of wall";
                }
                if(coverVolFront != 0.0) //Partial cover loss
                {

                    // Repair volume in cubic feet
                    auto repairVolume = coverVolFront*35.3147;

                    // Area patch in square foot
                    auto repairArea = coverAreaFront*10.7639;

                    // Depth of the repair in inches
                    auto repairDepth = coverThickness + longReinfDiameter + transReinfDiameter + 0.0254;

                    // Volume in Cubic Yards
                    // Area in square foot
                    // Depth of the repair in inches
                    theCSIAssembler->getRCWallCoverReplacementCodes(repairVolume*1.30795, repairDepth*39.3701, repairArea*10.7639, &repairFrontJob, &repairFrontLabourQuantityCSICodes,&repairFrontMaterialQuantityCSICodes,theConcreteShearwall);
                }
                if(coverVolBack != 0.0) //Partial cover loss
                {

                    // Repair volume in cubic feet
                    auto repairVolume = coverVolBack*35.3147;

                    // Area patch in square foot
                    auto repairArea = coverAreaBack*10.7639;

                    // Repair depth in m
                    auto repairDepth = coverThickness + longReinfDiameter + transReinfDiameter + 0.0254;

                    // Volume in Cubic Yards
                    // Area in square foot
                    // Depth of the repair in inches
                    theCSIAssembler->getRCWallCoverReplacementCodes(repairVolume*1.30795, repairDepth*39.3701, repairArea*10.7639, &repairBackJob, &repairBackLabourQuantityCSICodes,&repairBackMaterialQuantityCSICodes,theConcreteShearwall);
                }

                // Now calculate the repair cost for the Front
                auto repairCostFront = theCostMethod->getLabourCosts(repairFrontLabourQuantityCSICodes);
                repairCostFront += theCostMethod->getMaterialCosts(repairFrontMaterialQuantityCSICodes);
                repairCostFront += theCostMethod->getEquipmentCosts(repairFrontLabourQuantityCSICodes);

                // Now calculate the repair cost for the Back
                auto repairCostBack = theCostMethod->getLabourCosts(repairBackLabourQuantityCSICodes);
                repairCostBack += theCostMethod->getMaterialCosts(repairBackMaterialQuantityCSICodes);
                repairCostBack += theCostMethod->getEquipmentCosts(repairBackLabourQuantityCSICodes);

                double totalFrontRepairCost = coverReplacementCost < repairCostFront ? coverReplacementCost : repairCostFront;
                double totalBackRepairCost = coverReplacementCost < repairCostBack ? coverReplacementCost : repairCostBack;

                // First check to see if the repair/replacement cost does not exceed the replacement cost
                if(componentReplacementCost < totalFrontRepairCost + totalBackRepairCost)
                {
                    auto replacementLabourQuantityCSICodes = it->getReplacementLabourQuantityCSICodes();
                    auto replacementMaterialQuantityCSICodes = it->getReplacementMaterialQuantityCSICodes();
                    auto replacementJob = it->getReplacementJob();

                    this->commitRepair(&replacementLabourQuantityCSICodes,&replacementMaterialQuantityCSICodes,&replacementJob,&repairPhase, it, componentReplacementCost);
                }
                // Else do the cheaper of the repairs or cover replacements - it could be that cover replacement is cheaper than the repairs
                else
                {
                    if(coverReplacementCost < repairCostFront)
                    {
                        this->commitRepair(&coverReplacementLabourCSICodes,&coverReplacementMaterialCSICodes,&coverReplacementJob,&repairPhase, it, coverReplacementCost);
                    }
                    else if(repairCostFront > 1.0)
                    {
                        this->commitRepair(&repairFrontLabourQuantityCSICodes,&repairFrontMaterialQuantityCSICodes,&repairFrontJob,&repairPhase, it, repairCostFront);
                    }

                    if(coverReplacementCost < repairCostBack)
                    {
                        this->commitRepair(&coverReplacementLabourCSICodes,&coverReplacementMaterialCSICodes,&coverReplacementJob,&repairPhase, it, coverReplacementCost);
                    }
                    else if(repairCostBack > 1.0)
                    {
                        this->commitRepair(&repairBackLabourQuantityCSICodes,&repairBackMaterialQuantityCSICodes,&repairBackJob,&repairPhase, it, repairCostBack);
                    }
                }
            }
        }
        else if(theSteelWBeamColumn)
        {
            // For this component, there are 2 damage types:
            //      0) undamaged
            //      1) yielding

            // Get the damage mesh from the component
            auto theDamageMesh = it->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region, get the visual damage
            for(auto&& damageRegion : damageRegionVec)
            {
                if(componentReplacement)
                    break;

                auto WSectionDamageRegion = dynamic_cast<RWSectionDamageRegion*>(damageRegion.get());

                if(!WSectionDamageRegion)
                {
                    qCritical()<<"Error, requires a frame damage region in "<<__FUNCTION__;
                    return -1;
                }

                auto fibreContainers = WSectionDamageRegion->getAllFibreContainers();

                for(auto&& fiberContainer : fibreContainers)
                {
                    auto damageType = fiberContainer->getGlobalDamageType();

                    if(damageType == 0)
                    {
                        continue;
                    }
                    // Yielding
                    else if(damageType == 1)
                    {
                        componentReplacement = true;
                        break;
                    }
                    else
                    {
                        qDebug()<<"Damage mode "<<damageType<<" not recognized for component"<<it->objectName();
                    }
                }
            }

            if(componentReplacement)
            {
                // For replacement of section:
                //    1. Shoring of structure
                //    2. Demolition of old steel
                //    3. Removal/Disposal/Cleaning debris
                //    4. Replacement with a new component

                //*************  1.	Shoring of structure  *************

                // Assume 8 frames
                auto numberOfFrames = 8.0;

                // CSI codes for shoring and bracing
                theCSIAssembler->getShoringCSICodes(numberOfFrames,&materialQuantityCSICodes);

                //*************  2.&3. Demolition of Component / Removal/Disposal/Cleaning debris *************

                auto volSteel = theSteelWBeamColumn->getVolumeOfSteel();

                // Mass needs to be in US ton
                auto demoMass = volSteel*7900.0*0.00110231;

                theCSIAssembler->getSelectiveSteelDemolitionCodes(demoMass,&repairPhase,&labourQuantityCSICodes,it);

                // Removal and demo
                // Volume of steel in yards
                auto volSteelYrd = volSteel*1.30795;

                // Add to running total
                totalTonsSteel += demoMass;

                // CSI codes for steel column removal and disposal
                theCSIAssembler->getSteelRemovalDisposalCodes(volSteelYrd,&repairPhase,&labourQuantityCSICodes,it);

                //*************  4. Replacement with a new component *************

                // CSI codes for the material
                theCSIAssembler->getSteelWColumnMaterialConstructionCodes(theSteelWBeamColumn,&materialQuantityCSICodes);

                // CSI codes for the labour
                theCSIAssembler->getSteelWColumnLabourConstructionCodes(theSteelWBeamColumn,&repairPhase,&labourQuantityCSICodes);

                it->setComponentReplaced(true);
            }
        }
        else if(theCompositeDecking)
        {
            // For this component, there are 2 damage types:
            //      0) undamaged
            //      1) damaged - crushing of topping or yielding

            // Get the damage mesh from the component
            auto theDamageMesh = it->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // Iterate through the damage regions
            for(auto&& damageRegion : damageRegionVec)
            {
                // If any region is damaged beyond repair, than replace the component
                if(componentReplacement)
                    break;

                auto globalDamageType = damageRegion->getGlobalDamageType();

                // Total component replacement
                // Or no damage
                if(globalDamageType == 0)
                {
                    continue;
                }
                else if(globalDamageType >= 1)
                {
                    componentReplacement = true;
                }
            }

            if(componentReplacement)
            {
                // For replacement of section:
                //    1. Shoring of structure
                //    2. Demolition of old component
                //    3. Removal/Disposal/Cleaning debris
                //    4. Replacement with a new component


                //*************  1.	Shoring of structure  *************

                // The structure has to be shored around the damaged area, the slab area in sq. ft.
                // Area of deck requiring repair in sq. ft.
                auto areaSlabSqft = theCompositeDecking->getPlanArea()*10.7639;

                // Assume one jack for every 36 sq. ft., i.e., 6x6 area
                auto numberOfFrames = areaSlabSqft/36.0;

                // CSI codes for shoring and bracing
                theCSIAssembler->getShoringCSICodes(numberOfFrames,&materialQuantityCSICodes);

                //*************  2.&3. Demolition of Component / Removal/Disposal/Cleaning debris *************

                // Volume of concrete in m3
                auto volConc = theCompositeDecking->getVolumeOfConcrete();

                // Volume of steel in m3
                auto volSteel = theCompositeDecking->getVolumeOfSteel();

                // Volume of concrete in yards
                auto volConcYrds = volConc*1.30795;

                totalVolumeConcreteYrds += volConc;

                // Volume of steel in yards
                auto volSteelYrds = volSteel*1.30795;

                totalTonsSteel += volSteel*7900.0*0.00110231;

                // CSI codes for steel decking demolition
                theCSIAssembler->getSteelDeckingDemolitionCodes(areaSlabSqft,&repairPhase,&labourQuantityCSICodes,it);

                // CSI codes for concrete topping demolition
                theCSIAssembler->getConcreteSlabCutOutWireMeshDemolitionCodes(volConcYrds,&repairPhase,&labourQuantityCSICodes,it);

                // Disposal
                // CSI codes for damaged concrete removal and disposal
                theCSIAssembler->getConcreteRemovalDisposalCodes(volConcYrds,&repairPhase,&labourQuantityCSICodes,it);

                // CSI codes for damaged steel removal and disposal
                theCSIAssembler->getSteelRemovalDisposalCodes(volSteelYrds,&repairPhase,&labourQuantityCSICodes,it);

                //*************  4. Replacement with a new component *************

                // CSI codes for the material
                theCSIAssembler->getSteelSkinSlabConstructionCodes(theCompositeDecking,nullptr,&materialQuantityCSICodes);

                // CSI codes for the labour
                theCSIAssembler->getSteelSkinSlabConstructionCodes(theCompositeDecking,&repairPhase,&labourQuantityCSICodes);

                it->setComponentReplaced(true);
            }

        }
        else if(theRectGlulamBeamColumn)
        {
            // For this component, there are 2 damage types:
            //      0) undamaged
            //      1) split or ruptured

            // Get the damage mesh from the component
            auto theDamageMesh = it->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region, get the visual damage
            for(auto&& damageRegion : damageRegionVec)
            {
                if(componentReplacement)
                    break;

                auto rectSectionDamageRegion = dynamic_cast<RQuadSectionDamageRegion*>(damageRegion.get());

                if(!rectSectionDamageRegion)
                {
                    qCritical()<<"Error, requires a rectangular section damage region in "<<__FUNCTION__;
                    return -1;
                }

                auto fibreContainers = rectSectionDamageRegion->getAllFibreContainers();

                for(auto&& fiberContainer : fibreContainers)
                {
                    auto damageType = fiberContainer->getGlobalDamageType();

                    if(damageType == 0)
                    {
                        continue;
                    }
                    // Split or rupture
                    else if(damageType == 1)
                    {
                        componentReplacement = true;
                        break;
                    }
                    else
                    {
                        qDebug()<<"Damage type "<<damageType<<" not recognized for component"<<it->objectName();
                    }
                }
            }

            if(componentReplacement)
            {
                // For replacement of section:
                //    1. Shoring of structure
                //    2. Demolition of old wood
                //    3. Removal/Disposal/Cleaning debris
                //    4. Replacement with a new component

                if (theOutputLevel >= RObject::Maximum)
                {
                    qDebug()<<"************REPLACING GLULAM COLUMN************";
                }

                //*************  1.	Shoring of structure  *************

                // Assume four frames
                auto numberOfFrames = 4.0;

                // CSI codes for shoring and bracing
                theCSIAssembler->getShoringCSICodes(numberOfFrames,&materialQuantityCSICodes);

                //*************  2.&3. Demolition of Component / Removal/Disposal/Cleaning debris *************

                // Volume of wood in m3
                auto volWood = theRectGlulamBeamColumn->getVolumeOfWood();

                // Convert cubic meter to mbf
                auto MBFwood = volWood*0.42377600065786;

                // Volume of wood in yards
                auto volWoodYrds = volWood*1.30795;

                // Add to running total
                totalVolumeWoodYrds += volWoodYrds;

                // Line Number:  060505103202
                // Description: Selective Demolition, wood framing, colums, Dimensional lumber sizes
                // Unit: M.B.F..
                // Crew:  Crew 2Clab
                RCrew* crew2CLAB = repairPhase.addCrew(RCrewCode::CLAB2);

                QString CSINumber = "060505103202";
                QString Description = "Selective Demolition, wood framing, colums, Dimensional lumber sizes";

                labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,MBFwood,Description));

                crew2CLAB->addTask(CSINumber,MBFwood,Description,it);

                // CSI codes for wood column removal and disposal
                theCSIAssembler->getWoodRemovalDisposalCodes(volWoodYrds,&repairPhase,&labourQuantityCSICodes,it);

                //*************  4. Replacement with a new component *************

                // CSI codes for the material
                theCSIAssembler->getGlulamColumnMaterialConstructionCodes(theRectGlulamBeamColumn,&materialQuantityCSICodes);

                // CSI codes for the labour
                theCSIAssembler->getGlulamColumnLabourConstructionCodes(theRectGlulamBeamColumn,&repairPhase,&labourQuantityCSICodes);

                it->setComponentReplaced(true);
            }
        }
        else if(theCLTPanel)
        {
            //For this component, there are 2 damage types:
            //      0) undamaged
            //      1) damaged - crushing, tensile, or shear failure

            // Get the damage mesh from the component
            auto theDamageMesh = it->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // Iterate through the damage regions
            for(auto&& damageRegion : damageRegionVec)
            {
                // If any region is damaged beyond repair, than replace the component
                if(componentReplacement)
                    break;


                auto globalDamageType = damageRegion->getGlobalDamageType();

                // Total component replacement
                // Or no damage
                if(globalDamageType == 0)
                {
                    continue;
                }
                else if(globalDamageType >= 1)
                {
                    componentReplacement = true;
                }
            }

            if(componentReplacement)
            {
                //For replacement of component:
                //    1.  Shoring of structure
                //    2.  Demolition of old component
                //    3.  Removal/Disposal/Cleaning debris
                //    4.  Replacement with a new component

                //*************  1.	Shoring of structure  *************

                // Assume four frames
                auto numberOfFrames = 4.0;

                // CSI codes for shoring and bracing
                theCSIAssembler->getShoringCSICodes(numberOfFrames,&materialQuantityCSICodes);

                //*************  2.&3. Demolition of Component / Removal/Disposal/Cleaning debris *************

                // Volume of wood in m3
                auto volWood = theCLTPanel->getVolumeOfWood();

                // Convert cubic meter to mbf
                auto MBFwood = volWood*0.42377600065786;

                // Volume of wood in yards
                auto volWoodYrds = volWood*1.30795;

                // Add to running total
                totalVolumeWoodYrds += volWoodYrds;

                // Line Number:  060505105660
                // Description: Selective demolition, wood framing, T&G floor planks
                // Unit: M.B.F..
                // Crew:  Crew 2Clab

                RCrew* crew2CLAB = repairPhase.addCrew(RCrewCode::CLAB2);

                QString CSINumber = "060505105660";
                QString Description = "Selective demolition, wood framing, T&G floor planks";

                labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,MBFwood,Description));

                crew2CLAB->addTask(CSINumber,MBFwood,Description,it);

                // CSI codes for damaged wood removal and disposal
                theCSIAssembler->getWoodRemovalDisposalCodes(volWoodYrds,&repairPhase,&labourQuantityCSICodes,it);

                //*************  4. Replacement with a new component *************

                // CSI codes for the material
                theCSIAssembler->getCLTPanelConstructionCodes(theCLTPanel,nullptr,&materialQuantityCSICodes);

                // CSI codes for the labour
                theCSIAssembler->getCLTPanelConstructionCodes(theCLTPanel,&repairPhase,&labourQuantityCSICodes);

                it->setComponentReplaced(true);
            }
        }
    }

    totalVolumeWoodYrds += repairPhase.getYrdWood();
    totalVolumeConcreteYrds += repairPhase.getYrdConcrete();
    totalTonsSteel += repairPhase.getTonsReinfNo3toNo7();
    totalTonsSteel += repairPhase.getTonsReinfNo8toNo18();
    totalCfEpoxy += repairPhase.getCfEpoxy();

    // Get the additional labour costs
    theCSIAssembler->getAdditionalLabourCosts(&repairPhase,theBIM,&labourQuantityCSICodes);

    // Calculate the labour costs
    auto labourDirectCosts = theCostMethod->getLabourCosts(labourQuantityCSICodes);

    if (theOutputLevel >= RObject::Medium) {
        qDebug()<<"***LABOUR DIRECT COSTS***"<<labourDirectCosts;
    }

    theStatsMethod->update(theCostResponse->objectName()+"LabourCosts",labourDirectCosts);

    accumulatedDirectCost += labourDirectCosts;

    auto equipmentDirectCosts = theCostMethod->getEquipmentCosts(labourQuantityCSICodes);

    if (theOutputLevel >= RObject::Medium) {
        qDebug()<<"***EQUIPMENT DIRECT COSTS***"<<equipmentDirectCosts;
    }

    theStatsMethod->update(theCostResponse->objectName()+"EquipmentCosts",equipmentDirectCosts);

    accumulatedDirectCost += equipmentDirectCosts;

    // Calculate the material costs
    auto materialDirectCosts = theCostMethod->getMaterialCosts(materialQuantityCSICodes);

    if (theOutputLevel >= RObject::Medium) {
        qDebug()<<"***MATERIAL DIRECT COSTS***"<<materialDirectCosts;
    }

    theStatsMethod->update(theCostResponse->objectName()+"MaterialCosts",materialDirectCosts);

    accumulatedDirectCost += materialDirectCosts;

    // Set the response from this model, i.e., the cumulative cost, energy, and time
    theCostResponse->setCurrentValue(accumulatedDirectCost);

    theStatsMethod->update(theCostResponse->objectName(),accumulatedDirectCost);


    //**************************************************
    //**************************************************
    //******************Time to Repair******************
    //**************************************************
    //**************************************************

    if (theOutputLevel >= RObject::Medium) {
        qDebug()<<"******** TIME ********";
    }

    theScheduler->addJob(repairPhase);
    theScheduler->setTheCostMethod(theCostMethod);

    auto theCriticalPath = theScheduler->getCriticalPath();

    if (theOutputLevel >= RObject::Medium) {
        qDebug()<<"The critcal path [hrs]"<<theCriticalPath;
        qDebug()<<"The critcal path [day]"<<theCriticalPath/24.0;
        qDebug()<<"The critcal path [month]"<<theCriticalPath/720.0;
    }

    double totalWorkHours = theScheduler->getTotalWorkHours();

    if (theOutputLevel >= RObject::Medium) {
        qDebug()<<"The total work done [hrs]"<<totalWorkHours;
        qDebug()<<"The total work done [day]"<<totalWorkHours/24.0;
        qDebug()<<"The total work done [month]"<<totalWorkHours/720.0;
    }

    int numWorkers = theScheduler->getNumberOfWorkers();

    if (theOutputLevel >= RObject::Medium) {
        qDebug()<<"The number of workers: "<<numWorkers;
    }

    int numCrew = theScheduler->getNumberOfCrews();

    if (theOutputLevel >= RObject::Medium) {
        qDebug()<<"The number of crews: "<<numCrew;
    }

    // Set the time response in days
    theTimeResponse->setCurrentValue(theCriticalPath/24.0);

    theStatsMethod->update(theTimeResponse->objectName(),theCriticalPath/24.0);
    theStatsMethod->update(this->objectName()+"NumberOfWorkers",numWorkers);
    theStatsMethod->update(this->objectName()+"NumberOfCrews",numCrew);

    //****************************************************************************
    //****************************************************************************
    //********************Emissions and Environmental Impacts*********************
    //****************************************************************************
    //****************************************************************************

    auto CarbonDioxideEmissions  =  0.0;
    auto CarbonMonoxideEmissions =  0.0;
    auto OrganicCarbonEmissions  =  0.0;
    auto MethaneEmissions        =  0.0;
    auto NitrogenOxidesEmissions =  0.0;
    auto HFC134aEmissions        =  0.0;
    auto SulfurDioxideEmissions  =  0.0;
    auto AmmoniaEmissions        =  0.0;
    auto NitrousOxideEmissions   =  0.0;
    auto BlackCarbonPMEmissions  =  0.0;

    // Create dummy classes so that can get the values for environmental impacts
    auto concreteMaterial = make_unique<RConcreteMaterialKnowledge>(theDomain, this->objectName(), "C20");
    auto steelMaterial = make_unique<RSteelMaterialKnowledge>(theDomain, this->objectName(), "NULL");
    auto woodMaterial = make_unique<RGlulamMaterialKnowledge>(theDomain, this->objectName());

    // Mass of steel in kg, convert to kg from us ton
    auto totalMassSteel = totalTonsSteel*907.185;

    // Volume of concrete in m^3
    auto totalVolumeConcrete = totalVolumeConcreteYrds*0.764555;

    // Volume of wood in m^3
    auto totalVolumeWood = totalVolumeWoodYrds*0.764555;

    // Mass of Epoxy in kg, 1250 kg/m3 of epoxy, 1 cf = 0.0283168 m3
    auto totalKgEpoxy = totalCfEpoxy*1250.0*0.0283168;

    // All volumes should be in m^3 or kg (steel) while emissions are in kg/m^3
    CarbonDioxideEmissions  += totalMassSteel*steelMaterial->getCarbonDioxideEmissions()  + totalVolumeConcrete*concreteMaterial->getCarbonDioxideEmissions()   + totalVolumeWood*woodMaterial->getCarbonDioxideEmissions();
    CarbonMonoxideEmissions += totalMassSteel*steelMaterial->getCarbonMonoxideEmissions() + totalVolumeConcrete*concreteMaterial->getCarbonMonoxideEmissions()  + totalVolumeWood*woodMaterial->getCarbonMonoxideEmissions();
    OrganicCarbonEmissions  += totalMassSteel*steelMaterial->getOrganicCarbonEmissions()  + totalVolumeConcrete*concreteMaterial->getOrganicCarbonEmissions()   + totalVolumeWood*woodMaterial->getOrganicCarbonEmissions ();
    MethaneEmissions        += totalMassSteel*steelMaterial->getMethaneEmissions()        + totalVolumeConcrete*concreteMaterial->getMethaneEmissions()         + totalVolumeWood*woodMaterial->getMethaneEmissions();
    NitrogenOxidesEmissions += totalMassSteel*steelMaterial->getNitrogenOxidesEmissions() + totalVolumeConcrete*concreteMaterial->getNitrogenOxidesEmissions()  + totalVolumeWood*woodMaterial->getNitrogenOxidesEmissions();
    HFC134aEmissions        += totalMassSteel*steelMaterial->getHFC134aEmissions()        + totalVolumeConcrete*concreteMaterial->getHFC134aEmissions()         + totalVolumeWood*woodMaterial->getHFC134aEmissions();
    SulfurDioxideEmissions  += totalMassSteel*steelMaterial->getSulfurDioxideEmissions () + totalVolumeConcrete*concreteMaterial->getSulfurDioxideEmissions ()  + totalVolumeWood*woodMaterial->getSulfurDioxideEmissions();
    AmmoniaEmissions        += totalMassSteel*steelMaterial->getAmmoniaEmissions()        + totalVolumeConcrete*concreteMaterial->getAmmoniaEmissions()         + totalVolumeWood*woodMaterial->getAmmoniaEmissions();
    NitrousOxideEmissions   += totalMassSteel*steelMaterial->getNitrousOxideEmissions()   + totalVolumeConcrete*concreteMaterial->getNitrousOxideEmissions()    + totalVolumeWood*woodMaterial->getNitrousOxideEmissions();
    BlackCarbonPMEmissions  += totalMassSteel*steelMaterial->getBlackCarbonPMEmissions()  + totalVolumeConcrete*concreteMaterial->getBlackCarbonPMEmissions()   + totalVolumeWood*woodMaterial->getBlackCarbonPMEmissions ();

    // Add emissions from Epoxy. Future work is to add more emissions from epoxy
    // 7.15 kg-CO2/kg-epoxy see:"http://www.dartmouth.edu/~cushman/books/Numbers/Chap1-Materials.pdf" Note:they get it from: Ashby, M. F., 2013: Materials and the Environment – Eco‐Informed Material Choice, 2nd ed., Butterworth‐Heinemann, 616 pages.
    CarbonDioxideEmissions  += totalKgEpoxy*7.15;

    // All emissions are given in kg
    theEmissionsResponse->setCurrentValue("CarbonDioxideEmissions",CarbonDioxideEmissions);
    theEmissionsResponse->setCurrentValue("CarbonMonoxideEmissions",CarbonMonoxideEmissions);
    theEmissionsResponse->setCurrentValue("OrganicCarbonEmissions",OrganicCarbonEmissions);
    theEmissionsResponse->setCurrentValue("MethaneEmissions",MethaneEmissions);
    theEmissionsResponse->setCurrentValue("NitrogenOxidesEmissions",NitrogenOxidesEmissions);
    theEmissionsResponse->setCurrentValue("HFC134aEmissions",HFC134aEmissions);
    theEmissionsResponse->setCurrentValue("SulfurDioxideEmissions",SulfurDioxideEmissions);
    theEmissionsResponse->setCurrentValue("AmmoniaEmissions",AmmoniaEmissions);
    theEmissionsResponse->setCurrentValue("NitrousOxideEmissions",NitrousOxideEmissions);
    theEmissionsResponse->setCurrentValue("BlackCarbonPMEmissions",BlackCarbonPMEmissions);

    theStatsMethod->update(theEmissionsResponse->objectName()+"CarbonDioxideEmissions",CarbonDioxideEmissions);
    theStatsMethod->update(theEmissionsResponse->objectName()+"CarbonMonoxideEmissions",CarbonMonoxideEmissions);
    theStatsMethod->update(theEmissionsResponse->objectName()+"OrganicCarbonEmissions",OrganicCarbonEmissions);
    theStatsMethod->update(theEmissionsResponse->objectName()+"MethaneEmissions",MethaneEmissions);
    theStatsMethod->update(theEmissionsResponse->objectName()+"NitrogenOxidesEmissions",NitrogenOxidesEmissions);
    theStatsMethod->update(theEmissionsResponse->objectName()+"HFC134aEmissions",HFC134aEmissions);
    theStatsMethod->update(theEmissionsResponse->objectName()+"SulfurDioxideEmissions",SulfurDioxideEmissions);
    theStatsMethod->update(theEmissionsResponse->objectName()+"AmmoniaEmissions",AmmoniaEmissions);
    theStatsMethod->update(theEmissionsResponse->objectName()+"NitrousOxideEmissions",NitrousOxideEmissions);
    theStatsMethod->update(theEmissionsResponse->objectName()+"BlackCarbonPMEmissions",BlackCarbonPMEmissions);

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"CarbonDioxideEmissions [kg]"<<CarbonDioxideEmissions;
        qDebug()<<"CarbonMonoxideEmissions [kg]"<<CarbonMonoxideEmissions;
        qDebug()<<"OrganicCarbonEmissions [kg]"<<OrganicCarbonEmissions;
        qDebug()<<"MethaneEmissions [kg]"<<MethaneEmissions;
        qDebug()<<"NitrogenOxidesEmissions [kg]"<<NitrogenOxidesEmissions;
        qDebug()<<"HFC134aEmissions [kg]"<<HFC134aEmissions;
        qDebug()<<"SulfurDioxideEmissions [kg]"<<SulfurDioxideEmissions;
        qDebug()<<"AmmoniaEmissions [kg]"<<AmmoniaEmissions;
        qDebug()<<"NitrousOxideEmissions [kg]"<<NitrousOxideEmissions;
        qDebug()<<"BlackCarbonPMEmissions [kg]"<<BlackCarbonPMEmissions;
    }

    // The ReCiPe2016 impacts are given in their respective units
    auto AgriculturalLandOccupation = totalMassSteel*steelMaterial->getAgriculturalLandOccupationImpact()    + totalVolumeWood*woodMaterial->getAgriculturalLandOccupationImpact()    + totalVolumeConcrete*concreteMaterial->getAgriculturalLandOccupationImpact();
    auto ClimateChange              = totalMassSteel*steelMaterial->getClimateChangeImpact()                 + totalVolumeWood*woodMaterial->getClimateChangeImpact()                 + totalVolumeConcrete*concreteMaterial->getClimateChangeImpact();
    auto FreshwaterEcotoxicity      = totalMassSteel*steelMaterial->getFreshwaterEcotoxicityImpact()         + totalVolumeWood*woodMaterial->getFreshwaterEcotoxicityImpact()         + totalVolumeConcrete*concreteMaterial->getFreshwaterEcotoxicityImpact();
    auto FreshwaterEutrophication   = totalMassSteel*steelMaterial->getFreshwaterEutrophicationImpact()      + totalVolumeWood*woodMaterial->getFreshwaterEutrophicationImpact()      + totalVolumeConcrete*concreteMaterial->getFreshwaterEutrophicationImpact();
    auto HumanToxicity              = totalMassSteel*steelMaterial->getHumanToxicityImpact()                 + totalVolumeWood*woodMaterial->getHumanToxicityImpact()                 + totalVolumeConcrete*concreteMaterial->getHumanToxicityImpact();
    auto IonisingRadiation          = totalMassSteel*steelMaterial->getIonisingRadiationImpact()             + totalVolumeWood*woodMaterial->getIonisingRadiationImpact()             + totalVolumeConcrete*concreteMaterial->getIonisingRadiationImpact();
    auto MarineEcotoxicity          = totalMassSteel*steelMaterial->getMarineEcotoxicityImpact()             + totalVolumeWood*woodMaterial->getMarineEcotoxicityImpact()             + totalVolumeConcrete*concreteMaterial->getMarineEcotoxicityImpact();
    auto MarineEutrophication       = totalMassSteel*steelMaterial->getMarineEutrophicationImpact()          + totalVolumeWood*woodMaterial->getMarineEutrophicationImpact()          + totalVolumeConcrete*concreteMaterial->getMarineEutrophicationImpact();
    auto OzoneDepletion             = totalMassSteel*steelMaterial->getOzoneDepletionImpact()                + totalVolumeWood*woodMaterial->getOzoneDepletionImpact()                + totalVolumeConcrete*concreteMaterial->getOzoneDepletionImpact();
    auto ParticulateMatterFormation = totalMassSteel*steelMaterial->getParticulateMatterFormationImpact()    + totalVolumeWood*woodMaterial->getParticulateMatterFormationImpact()    + totalVolumeConcrete*concreteMaterial->getParticulateMatterFormationImpact();
    auto PhotochemicalOxidant       = totalMassSteel*steelMaterial->getPhotochemicalOxidantFormationImpact() + totalVolumeWood*woodMaterial->getPhotochemicalOxidantFormationImpact() + totalVolumeConcrete*concreteMaterial->getPhotochemicalOxidantFormationImpact();
    auto TerrestrialAcidification   = totalMassSteel*steelMaterial->getTerrestrialAcidificationImpact()      + totalVolumeWood*woodMaterial->getTerrestrialAcidificationImpact()      + totalVolumeConcrete*concreteMaterial->getTerrestrialAcidificationImpact();
    auto TerrestrialEcotoxicity     = totalMassSteel*steelMaterial->getTerrestrialEcotoxicityImpact()        + totalVolumeWood*woodMaterial->getTerrestrialEcotoxicityImpact()        + totalVolumeConcrete*concreteMaterial->getTerrestrialEcotoxicityImpact();
    auto UrbanLandOccupation        = totalMassSteel*steelMaterial->getUrbanLandOccupationImpact()           + totalVolumeWood*woodMaterial->getUrbanLandOccupationImpact()           + totalVolumeConcrete*concreteMaterial->getUrbanLandOccupationImpact();

    theEmissionsResponse->setCurrentValue("AgriculturalLandOccupation",AgriculturalLandOccupation);
    theEmissionsResponse->setCurrentValue("ClimateChange",ClimateChange);
    theEmissionsResponse->setCurrentValue("FreshwaterEcotoxicity",FreshwaterEcotoxicity);
    theEmissionsResponse->setCurrentValue("FreshwaterEutrophication",FreshwaterEutrophication);
    theEmissionsResponse->setCurrentValue("HumanToxicity",HumanToxicity);
    theEmissionsResponse->setCurrentValue("IonisingRadiation",IonisingRadiation);
    theEmissionsResponse->setCurrentValue("MarineEcotoxicity",MarineEcotoxicity);
    theEmissionsResponse->setCurrentValue("MarineEutrophication",MarineEutrophication);
    theEmissionsResponse->setCurrentValue("OzoneDepletion",OzoneDepletion);
    theEmissionsResponse->setCurrentValue("ParticulateMatterFormation",ParticulateMatterFormation);
    theEmissionsResponse->setCurrentValue("PhotochemicalOxidant",PhotochemicalOxidant);
    theEmissionsResponse->setCurrentValue("TerrestrialAcidification",TerrestrialAcidification);
    theEmissionsResponse->setCurrentValue("TerrestrialEcotoxicity",TerrestrialEcotoxicity);
    theEmissionsResponse->setCurrentValue("UrbanLandOccupation",UrbanLandOccupation);

    theStatsMethod->update(theEmissionsResponse->objectName()+"AgriculturalLandOccupation",AgriculturalLandOccupation);
    theStatsMethod->update(theEmissionsResponse->objectName()+"ClimateChange",ClimateChange);
    theStatsMethod->update(theEmissionsResponse->objectName()+"FreshwaterEcotoxicity",FreshwaterEcotoxicity);
    theStatsMethod->update(theEmissionsResponse->objectName()+"FreshwaterEutrophication",FreshwaterEutrophication);
    theStatsMethod->update(theEmissionsResponse->objectName()+"HumanToxicity",HumanToxicity);
    theStatsMethod->update(theEmissionsResponse->objectName()+"IonisingRadiation",IonisingRadiation);
    theStatsMethod->update(theEmissionsResponse->objectName()+"MarineEcotoxicity",MarineEcotoxicity);
    theStatsMethod->update(theEmissionsResponse->objectName()+"MarineEutrophication",MarineEutrophication);
    theStatsMethod->update(theEmissionsResponse->objectName()+"OzoneDepletion",OzoneDepletion);
    theStatsMethod->update(theEmissionsResponse->objectName()+"ParticulateMatterFormation",ParticulateMatterFormation);
    theStatsMethod->update(theEmissionsResponse->objectName()+"PhotochemicalOxidant",PhotochemicalOxidant);
    theStatsMethod->update(theEmissionsResponse->objectName()+"TerrestrialAcidification",TerrestrialAcidification);
    theStatsMethod->update(theEmissionsResponse->objectName()+"TerrestrialEcotoxicity",TerrestrialEcotoxicity);
    theStatsMethod->update(theEmissionsResponse->objectName()+"UrbanLandOccupation",UrbanLandOccupation);

    if (theOutputLevel == RObject::Maximum)
    {
        qDebug()<<"AgriculturalLandOccupation"<<AgriculturalLandOccupation;
        qDebug()<<"ClimateChange"<<ClimateChange;
        qDebug()<<"FreshwaterEcotoxicity"<<FreshwaterEcotoxicity;
        qDebug()<<"FreshwaterEutrophication"<<FreshwaterEutrophication;
        qDebug()<<"HumanToxicity"<<HumanToxicity;
        qDebug()<<"IonisingRadiation"<<IonisingRadiation;
        qDebug()<<"MarineEcotoxicity"<<MarineEcotoxicity;
        qDebug()<<"MarineEutrophication"<<MarineEutrophication;
        qDebug()<<"OzoneDepletion"<<OzoneDepletion;
        qDebug()<<"ParticulateMatterFormation"<<ParticulateMatterFormation;
        qDebug()<<"PhotochemicalOxidant"<<PhotochemicalOxidant;
        qDebug()<<"TerrestrialAcidification"<<TerrestrialAcidification;
        qDebug()<<"TerrestrialEcotoxicity"<<TerrestrialEcotoxicity;
        qDebug()<<"UrbanLandOccupation"<<UrbanLandOccupation;
    }

    //**************************************************
    //*********************Energy***********************
    //**************************************************

    // To do: query a NETWORK MODEL to get the distance that workers have to travel
    // Otherwise, need to estimate the distance that workers have to travel
    // Average distance travelled by each worker in km to get to site and back home
    double workerDistanceTravelled = 25.0;

    // Total distance is the number of work shifts (total work hours/hours per shift) multiplied by the avg. distance travelled to get to site and back home
    auto avgDistancePerWorker = totalWorkHours/hours_per_shift*workerDistanceTravelled;

    // The energy intensity of transporting a worker, according to the transportation method
    auto workerTransportationIntensityAutomobil = theWorkerTransportationIntensityAutomobil->getCurrentValue();
    auto workerTransportationIntensityDieselbus = theWorkerTransportationIntensityDieselbus->getCurrentValue();
    auto workerTransportationIntensitySkytrain  = theWorkerTransportationIntensitySkytrain->getCurrentValue();

    // The joules of energy for worker transport, according to the energy source
    auto joulesGasolineWorkerTransport = avgDistancePerWorker*workerTransportationIntensityAutomobil*fractionWorkerTransportationAutomobil;
    auto joulesDieselWorkerTransport = avgDistancePerWorker*workerTransportationIntensityDieselbus*fractionWorkerTransportationDieselbus;
    auto joulesElectricityWorkerTransport = avgDistancePerWorker*workerTransportationIntensitySkytrain*fractionWorkerTransportationSkytrain;

    JoulesGasolineLightMedDutyTransport += joulesGasolineWorkerTransport;
    JoulesDieselLightMedDutyTransport += joulesDieselWorkerTransport;
    kWhElectricityTransport += joulesElectricityWorkerTransport*2.77778e-7; //Convert joules to kWh

    // To do: query a NETWORK MODEL to get the distance that materials have to travel
    // Otherwise, need to estimate the distance that materials have to travel
    // Material transportation - assume that all materials are delivered using diesel

    // The distance of transporting concrete, steel, and wood in km
    double concreteTravelDistance = 25.0;
    double steelTravelDistance = 25.0;
    double woodTravelDistance = 25.0;

    // The energy intensity for material transpoer (number of J/kg/km) per material
    auto materialTransportationIntensityConcrete = theMaterialTransportationIntensityConcrete->getCurrentValue();
    auto materialTransportationIntensitySteel    = theMaterialTransportationIntensitySteel->getCurrentValue();
    auto materialTransportationIntensityWood     = theMaterialTransportationIntensityWood->getCurrentValue();

    // Mass of each material in Kg
    auto totalMassConcrete = totalVolumeConcrete*2450;
    auto totalMassWood = totalVolumeWood*800;

    // Total joules consumed for the transport of all materials
    auto joulesDieselMaterialTransport = totalMassConcrete*materialTransportationIntensityConcrete*concreteTravelDistance +
            totalMassSteel*materialTransportationIntensitySteel*steelTravelDistance +
            totalMassWood*materialTransportationIntensityWood*woodTravelDistance;

    JoulesDieselHeavyDutyTransport += joulesDieselMaterialTransport;

    theEnergyResponse->setCurrentValue("kWhElectricity",kWhElectricityTransport);
    theEnergyResponse->setCurrentValue("JoulesDieselLightMediumDutyTransport",JoulesDieselLightMedDutyTransport);
    theEnergyResponse->setCurrentValue("JoulesDieselHeavyDutyTransport",JoulesDieselHeavyDutyTransport);
    theEnergyResponse->setCurrentValue("JoulesGasolineLightMediumDutyTransport",JoulesGasolineLightMedDutyTransport);

    theStatsMethod->update(theEnergyResponse->objectName()+"kWhElectricity",kWhElectricityTransport);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesDieselLightMediumDutyTransport",JoulesDieselLightMedDutyTransport);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesDieselHeavyDutyTransport",JoulesDieselHeavyDutyTransport);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesGasolineLightMediumDutyTransport",JoulesGasolineLightMedDutyTransport);

    // Give output if requested
    if (theOutputLevel == RObject::Maximum) {
        qDebug() << "After the repair phase the cost is" << accumulatedDirectCost;
    }


    //Print very detailed repair costs, quantities, etc. of each component
    if (theOutputLevel == RObject::Maximum)
    {
        if(theSAModelResponse)
        {
            if(theSAModelResponse->getCurrentValue() != 0.0)
            {
                std::vector<double> totalRepairCostVec;
                std::vector<double> materialCostVec;
                std::vector<double> labourCostVec;
                std::vector<double> equipmentCostVec;
                std::vector<double> driftVec;
                std::vector<double> labourTimeVec;
                std::vector<double> numWorkersVec;
                std::vector<double> numCrewsVec;
                std::vector<double> elevationVec;
                std::vector<int> floorNumVec;
                std::vector<QString> replaceVec;
                std::vector<QString> typeVec;
                std::vector<QString> nameVec;

                double repairCostToCheck = 0.0;
                double repairTimeToCheck = 0.0;

                //Get the number of tasks for the entire repair
                int numTasksToCheck = theScheduler->getNumTasks();
                int numTasks = 0;

                // auto allTaskVec = theScheduler->getAllTasks();

                // for(auto task : allTaskVec)
                // {
                //     qDebug()<<task.getDescription()<<","<<task.getQuantityOfUnits()<<","<<task.getParentComponent()->objectName();
                // }

                for(auto&& component : theComponentList)
                {
                    if(component->getEvaluateDamage())
                    {
                        auto repairCost = component->getRepairCost();

                        auto CSILabourList = component->getRepairLabourQuantityCSICodes();
                        auto CSIMaterialList = component->getRepairMaterialQuantityCSICodes();

                        auto labourCost = theCostMethod->getLabourCosts(CSILabourList);
                        auto materialCost = theCostMethod->getMaterialCosts(CSIMaterialList);
                        auto equipmentCost = theCostMethod->getEquipmentCosts(CSILabourList);

                        auto sumRepairCosts = labourCost+materialCost+equipmentCost;

                        //Sanity check
                        if(fabs(sumRepairCosts - repairCost) > 0.10)
                        {
                            qDebug()<<"Oops something went wrong the repair cost returned by the component and the one calculated here should be equal!";
                            qDebug()<<"Calculated Here: "<<sumRepairCosts<<", and returned by component: "<<repairCost;
                        }

                        repairCostToCheck += sumRepairCosts;

                        totalRepairCostVec.push_back(sumRepairCosts);

                        materialCostVec.push_back(materialCost);
                        labourCostVec.push_back(labourCost);
                        equipmentCostVec.push_back(equipmentCost);

                        auto theMesh = component->getMesh();

                        if(theMesh == nullptr)
                        {
                            qCritical()<<"Could not get the mesh for component "<<component->objectName()<<" in "<<__FUNCTION__;
                            continue;
                        }

                        auto drift = theMesh->getMaxDrift();

                        driftVec.push_back(drift);

                        //Get the individual component repair time

                        auto componentJob = component->getRepairJob();

                        auto crewsVec = componentJob.getCrewsOnJob();


                        theScheduler->reset();

                        theScheduler->addJob(componentJob);

                        // The critcal path [day]
                        numTasks += theScheduler->getNumTasks();

                        // The total work done [day]
                        double totalWorkDays = theScheduler->getTotalWorkHours()/24.0;

                        repairTimeToCheck += totalWorkDays;

                        // Number of workers
                        int numWorkers = theScheduler->getNumberOfWorkers();

                        // Number of crews
                        int numCrews = theScheduler->getNumberOfCrews();

                        labourTimeVec.push_back(totalWorkDays);
                        numWorkersVec.push_back(numWorkers);
                        numCrewsVec.push_back(numCrews);

                        // Get the floor number and elevation of the component (from ground floor)
                        auto floorNum = component->getBuildingStoreyNumber();

                        floorNumVec.push_back(floorNum);

                        elevationVec.push_back(component->getElevation());

                        // Get the type of component
                        if(dynamic_cast<RRCSlabComponent*>(component))
                        {
                            typeVec.push_back("RC Floor Slab");
                        }
                        else if(dynamic_cast<RRCShearWallComponent*>(component))
                        {
                            typeVec.push_back("RC Shear Wall");
                        }
                        else if(dynamic_cast<RRectangularRCColumnComponent*>(component))
                        {
                            typeVec.push_back("RC Rect. Column");
                        }
                        else if(dynamic_cast<RWSteelBeamColumnComponent*>(component))
                        {
                            typeVec.push_back("W Steel Beam Column");
                        }
                        else if(dynamic_cast<RCompositePanelComponent*>(component))
                        {
                            typeVec.push_back("Composite Panel");
                        }
                        else if(dynamic_cast<RRectangularGlulamBeamColumnComponent*>(component))
                        {
                            typeVec.push_back("Glulam column");
                        }
                        else if(dynamic_cast<RCLTPanelComponent*>(component))
                        {
                            typeVec.push_back("CLT panel");
                        }
                        else
                        {
                            typeVec.push_back("N/A");
                        }

                        // Check to see if it will be replaced or repaired
                        auto shouldReplace = component->getShouldBeReplaced();

                        if(shouldReplace)
                            replaceVec.push_back("replace");
                        else
                            replaceVec.push_back("repair");

                        auto allTaskVec = theScheduler->getAllTasks();

                        nameVec.push_back(component->objectName());

                        qDebug()<<"For component :"<<component->objectName();

                        for(auto task : allTaskVec)
                        {
                            qDebug()<<task.getDescription()<<", Quantity of Units: "<<task.getQuantityOfUnits()<<", Cost: "<<sumRepairCosts
                                   <<", Parent Component: "<<task.getParentComponent()->objectName();
                        }

                    }
                }

                // Checks to ensure that the costs add up at the end
                if(fabs(repairCostToCheck - accumulatedDirectCost) >= 0.10)
                {
                    qCritical()<<"Oops something went wrong repair costs should be equal.";
                    qDebug()<<"Building Level: "<<accumulatedDirectCost<<", and accumulated component: "<<repairCostToCheck;

                }

                if(fabs(repairTimeToCheck - totalWorkHours/24.0) >= 0.10)
                {
                    qCritical()<<"Oops something went wrong repair times should be equal.";
                    qDebug()<<"Building Level: "<<totalWorkHours/24.0<<", and accumulated component: "<<repairTimeToCheck;
                }

                if(numTasks != numTasksToCheck)
                {
                    qCritical()<<"Oops something went wrong as the number of tasks should be equal.";
                    qDebug()<<"Building Level: "<<numTasksToCheck<<", and accumulated component: "<<numTasks;
                }

                // Print out the details
                qDebug()<<"Drift, Total Cost, Materials, Labour, Equipment, Elevation, Labour Time, Number of Workers, Number of Crews, Floor Number, replaceVec, Type, Name";
                for(size_t i = 0; i<totalRepairCostVec.size(); ++i)
                {
                    qDebug().noquote()<<driftVec[i]<<","<<totalRepairCostVec[i]<<","<<materialCostVec[i]<<","<<labourCostVec[i]<<","
                                     <<equipmentCostVec[i]<<","<<elevationVec[i]<<","<<labourTimeVec[i]<<","<<numWorkersVec[i]<<","
                                    <<numCrewsVec[i]<<","<<floorNumVec[i]<<","<<replaceVec[i]<<","<<typeVec[i]<<","<<nameVec[i];
                }
            }
        }
    }

    return 1;
}


int RStevesRepairImpactModel::resetTime()
{
    labourQuantityCSICodes.clear();
    materialQuantityCSICodes.clear();
    theScheduler->reset();

    return 0;
}


void RStevesRepairImpactModel::createDatabaseOfRandomVariables()
{

    theDomain->createObject(objectName() + "HeavyMachineryUseIntensity", "RContinuousRandomVariable");
    theHeavyMachineryUseIntensity = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theHeavyMachineryUseIntensity->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theHeavyMachineryUseIntensity->setMean(QString("2680000"));
    theHeavyMachineryUseIntensity->setCurrentValue(2680000);
    theHeavyMachineryUseIntensity->setCoefficientOfVariation("0.2");

    theDomain->createObject(objectName() + "MaterialTransportationIntensityConcrete", "RContinuousRandomVariable");
    theMaterialTransportationIntensityConcrete = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theMaterialTransportationIntensityConcrete->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theMaterialTransportationIntensityConcrete->setMean(QString("4855"));
    theMaterialTransportationIntensityConcrete->setCurrentValue(4855);
    theMaterialTransportationIntensityConcrete->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "MaterialTransportationIntensitySteel", "RContinuousRandomVariable");
    theMaterialTransportationIntensitySteel = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theMaterialTransportationIntensitySteel->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theMaterialTransportationIntensitySteel->setMean(QString("2200"));
    theMaterialTransportationIntensitySteel->setCurrentValue(2200);
    theMaterialTransportationIntensitySteel->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "MaterialTransportationIntensityWood", "RContinuousRandomVariable");
    theMaterialTransportationIntensityWood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theMaterialTransportationIntensityWood->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theMaterialTransportationIntensityWood->setMean(QString("6110"));
    theMaterialTransportationIntensityWood->setCurrentValue(6110);
    theMaterialTransportationIntensityWood->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "WorkerTransportationIntensityAutomobil", "RContinuousRandomVariable");
    theWorkerTransportationIntensityAutomobil = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theWorkerTransportationIntensityAutomobil->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theWorkerTransportationIntensityAutomobil->setMean(QString("2730000"));
    theWorkerTransportationIntensityAutomobil->setCurrentValue(2730000);
    theWorkerTransportationIntensityAutomobil->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "WorkerTransportationIntensityDieselbus", "RContinuousRandomVariable");
    theWorkerTransportationIntensityDieselbus = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theWorkerTransportationIntensityDieselbus->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theWorkerTransportationIntensityDieselbus->setMean(QString("920000"));
    theWorkerTransportationIntensityDieselbus->setCurrentValue(920000);
    theWorkerTransportationIntensityDieselbus->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "WorkerTransportationIntensitySkytrain", "RContinuousRandomVariable");
    theWorkerTransportationIntensitySkytrain = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theWorkerTransportationIntensitySkytrain->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theWorkerTransportationIntensitySkytrain->setMean(QString("390000"));
    theWorkerTransportationIntensitySkytrain->setCurrentValue(390000);
    theWorkerTransportationIntensitySkytrain->setCoefficientOfVariation("0.1");

}


void RStevesRepairImpactModel::getRCComponentRepairQuantities(const std::vector<double>& crackVolumeVec, double& crackVol,
                                                              const std::vector<double>& spallAreaVec, const std::vector<double>& spallDepthVec, double& spallArea, double& spallVol,
                                                              const std::vector<double>& coverAreaVec, const std::vector<double>& coverDepthVec, double& coverArea, double& coverVol)
{

    for(auto&& it : crackVolumeVec)
        crackVol += it;

    for(auto&& it : spallAreaVec)
        spallArea += it;

    for(size_t i = 0; i<spallAreaVec.size(); ++i)
    {
        auto area = spallAreaVec[i];
        auto depth = spallDepthVec[i];

        spallVol += area*depth;
    }

    for(auto&& it : coverAreaVec)
        coverArea += it;

    for(size_t i = 0; i<coverAreaVec.size(); ++i)
    {
        auto area = coverAreaVec[i];
        auto depth = coverDepthVec[i];

        coverVol += area*depth;
    }
}


void RStevesRepairImpactModel::getRCShellDamageQuantities(RShellSectionDamageRegion* shellDamageRegion, bool&componentReplacement,
                                                          double& crackedAreaTopTotal, double& spalledAreaTopTotal, double& coverLossAreaTopTotal,
                                                          std::vector<double>& crackWidthVecTop,std::vector<double>& crackLengthVecTop, std::vector<double>& crackDepthVecTop, std::vector<double>& crackVolumeVecTop,
                                                          std::vector<double>& spallAreaVecTop,std::vector<double>& spallDepthVecTop,std::vector<double>& coverAreaVecTop,std::vector<double>& coverDepthVecTop,
                                                          double& crackedAreaBotTotal, double& spalledAreaBotTotal, double& coverLossAreaBotTotal,
                                                          std::vector<double>& crackWidthVecBot,std::vector<double>& crackLengthVecBot, std::vector<double>& crackDepthVecBot, std::vector<double>& crackVolumeVecBot,
                                                          std::vector<double>& spallAreaVecBot,std::vector<double>& spallDepthVecBot,std::vector<double>& coverAreaVecBot,std::vector<double>& coverDepthVecBot,
                                                          size_t faceIndex1, size_t faceIndex2)
{

    auto globalDamageType = shellDamageRegion->getGlobalDamageType();

    // No damage
    if(globalDamageType == 0)
    {
        return;
    }

    if(globalDamageType >= 4)
    {
        componentReplacement = true;
        return;
    }

    // Get the quantities for the top/front
    auto damageTypeFace1 = shellDamageRegion->getDamageType(faceIndex1);

    // Cracking quantities
    if(damageTypeFace1 == 1)
    {
        crackedAreaTopTotal += shellDamageRegion->getDamageSurfaceArea(faceIndex1);

        auto crackWidth = shellDamageRegion->getMaxCrackWidth(faceIndex1);
        auto crackLength = shellDamageRegion->getCrackLength(faceIndex1);
        auto crackDepth = shellDamageRegion->getCrackDepth(faceIndex1);
        auto crackVolume = shellDamageRegion->getCrackDepth(faceIndex1);

        crackWidthVecTop.push_back(crackWidth);
        crackLengthVecTop.push_back(crackLength);
        crackDepthVecTop.push_back(crackDepth);
        crackVolumeVecTop.push_back(crackVolume);

    }
    // Spalling quantities
    else if(damageTypeFace1 == 2)
    {
        auto spallArea = shellDamageRegion->getDamageSurfaceArea(faceIndex1);
        auto spallDepth = shellDamageRegion->getDamageDepth(faceIndex1);

        spalledAreaTopTotal += spallArea;

        spallAreaVecTop.push_back(spallArea);
        spallDepthVecTop.push_back(spallDepth);
    }
    // Cover delamination quantities
    else if(damageTypeFace1 == 3)
    {
        auto coverLossArea = shellDamageRegion->getDamageSurfaceArea(faceIndex1);
        auto coverLossDepth = shellDamageRegion->getDamageDepth(faceIndex1);

        coverLossAreaTopTotal += coverLossArea;

        coverAreaVecTop.push_back(coverLossArea);
        coverDepthVecTop.push_back(coverLossDepth);
    }

    //Get the quantities for the bottom/back
    auto damageTypeFace2 = shellDamageRegion->getDamageType(faceIndex2);

    // Cracking quantities
    if(damageTypeFace2 == 1)
    {
        crackedAreaBotTotal += shellDamageRegion->getDamageSurfaceArea(faceIndex2);

        auto crackWidth = shellDamageRegion->getMaxCrackWidth(faceIndex2);
        auto crackLength = shellDamageRegion->getCrackLength(faceIndex2);
        auto crackDepth = shellDamageRegion->getCrackDepth(faceIndex2);
        auto crackVolume = shellDamageRegion->getCrackDepth(faceIndex2);

        crackWidthVecBot.push_back(crackWidth);
        crackLengthVecBot.push_back(crackLength);
        crackDepthVecBot.push_back(crackDepth);
        crackVolumeVecBot.push_back(crackVolume);

    }
    // Spalling quantities
    else if(damageTypeFace2 == 2)
    {
        auto spallArea = shellDamageRegion->getDamageSurfaceArea(faceIndex2);
        auto spallDepth = shellDamageRegion->getDamageDepth(faceIndex2);

        spalledAreaBotTotal += spallArea;

        spallAreaVecBot.push_back(spallArea);
        spallDepthVecBot.push_back(spallDepth);
    }
    // Cover delamination quantities
    else if(damageTypeFace2 == 3)
    {
        auto coverLossArea = shellDamageRegion->getDamageSurfaceArea(faceIndex2);
        auto coverLossDepth = shellDamageRegion->getDamageDepth(faceIndex2);

        coverLossAreaBotTotal += coverLossArea;

        coverAreaVecBot.push_back(coverLossArea);
        coverDepthVecBot.push_back(coverLossDepth);
    }
}


void RStevesRepairImpactModel::commitRepair(CSIVector* labourCSICodes , CSIVector* materialCodes, RJob* job, RJob* repairPhase, RComponent* theComponent, double repairCost)
{

    for(auto&& csi: *labourCSICodes)
    {
        labourQuantityCSICodes.push_back(csi);
    }

    for(auto&& csi: *materialCodes)
    {
        materialQuantityCSICodes.push_back(csi);
    }

    auto crewsVec = job->getCrewsOnJob();

    for(auto&& it : crewsVec)
        repairPhase->addCrew(it);

    repairPhase->getTonsReinfNo3toNo7()   += job->getTonsReinfNo3toNo7();
    repairPhase->getTonsReinfNo8toNo18()  += job->getTonsReinfNo8toNo18();
    repairPhase->getYrdConcrete()         += job->getYrdConcrete();
    repairPhase->getYrdWood()             += job->getYrdWood();
    repairPhase->getCfEpoxy()             += job->getCfEpoxy();

    theComponent->commitRepair(labourCSICodes,materialCodes,job,repairCost);

    return;
}





