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

#include "RStevesConstructionImpactModel.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RComponent.h"
#include "RCSICostMethod.h"
#include "RCSIAssembler.h"
#include "RCSISchedulingMethod.h"
#include "RGlulamMaterialKnowledge.h"
#include "RResponse.h"
#include "RInfoRichResponse.h"
#include "RBIM.h"
#include "RJob.h"
#include "RRectangularRCColumnComponent.h"
#include "RRCSlabComponent.h"
#include "RRCShearWallComponent.h"
#include "RWSteelBeamColumnComponent.h"
#include "RRectangularGlulamBeamColumnComponent.h"
#include "RCLTPanelComponent.h"
#include "RCompositePanelComponent.h"
#include "RStatistics.h"

#include <QCoreApplication>

RStevesConstructionImpactModel::RStevesConstructionImpactModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = nullptr;
    theHeavyMachineryUseIntensity = nullptr;
    theWorkerTransportationIntensityAutomobil = nullptr;
    theWorkerTransportationIntensityDieselbus = nullptr;
    theWorkerTransportationIntensitySkytrain = nullptr;
    theMaterialTransportationIntensityConcrete = nullptr;
    theMaterialTransportationIntensitySteel = nullptr;
    theMaterialTransportationIntensityWood = nullptr;

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

    // Information Rich Responses
    // Emissions
    theDomain->createObject(objectName() + "EmissionsResponse", "RInfoRichResponse");
    theEmissionsResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEmissionsResponse->setModel(this);

    // Energy
    theDomain->createObject(objectName() + "EnergyResponse", "RInfoRichResponse");
    theEnergyResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEnergyResponse->setModel(this);

    // Hours per worker shift
    hours_per_shift = 8.0;

    // Fraction of total worker hours allocated to the use of heavy machinery as determined by the construction type, f^WH
    heavy_machinery_ratio_high_rise_wood = 0.1;
    heavy_machinery_ratio_high_rise_steel = 0.3;
    heavy_machinery_ratio_high_rise_concrete = 0.3;
    heavy_machinery_ratio_low_rise_wood = 0.05;
    heavy_machinery_ratio_low_rise_steel = 0.1;
    heavy_machinery_ratio_low_rise_concrete = 0.1;

    // Fraction of energy source n that is employed in the transportation of workers, f^TW
    fractionWorkerTransportationAutomobil = 0.5;
    fractionWorkerTransportationDieselbus = 0.2;
    fractionWorkerTransportationSkytrain = 0.3;

    // Create the database of random variables used in this model
    createDatabaseOfRandomVariables();

    // Create the scheduler and CSI assembler
    theScheduler = std::make_unique<RCSISchedulingMethod>(theDomain,"ConstructionImpactModelScheduler");
    theCSIAssembler = std::make_unique<RCSIAssembler>(theDomain,"ConstructionImpactCSIAssembler");
    theStatsMethod = theDomain->getStatsMethod();
}


RStevesConstructionImpactModel::~RStevesConstructionImpactModel()
{

}


QObject * RStevesConstructionImpactModel::getBIM() const
{
    return theBIM;
}


void RStevesConstructionImpactModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


int RStevesConstructionImpactModel::evaluateModel(RGradientType theGradientType)
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

    // Check that there is a BIM (always needed)
    if(theBIM == nullptr)
    {
        qCritical() << this->objectName()<<" needs a BIM";
        return -1;
    }

    // Check to see if it is construction time, if not return quickly
    if (theCurrentTime->getCurrentValue() != theBIM->getTimeOfConstructionValue())
    {
        return 0;
    }

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"*********************************";
        qDebug()<<"*******Construction Phase********";
        qDebug()<<"*********************************";
    }

    // Create the mesh if not already done
    theBIM->createMesh();

    // Check if the components were created
    auto theComponentList = theBIM->getComponentList();
    if(theComponentList.isEmpty())
    {
        qDebug()<<"There are no components in "<<__FUNCTION__;
        return -1;
    }

    // Get the cost method, i.e., the method that translates the CSI codes into costs via values from RSMeans
    auto theCostMethod = theBIM->getTheCSICostMethod();

    // Set the cost-plus multiplier to multiply the bare costs with
    theCostMethod->setDemandSurge(1.2);

    // Keep the GUI responsive
    QCoreApplication::processEvents();

    // Create a job - the job class contains the worker crew codes, it creates and returns the crews required for a job
    RJob constructionPhase;
    constructionPhase.setJobCode(theBIM->objectName());

    // Declare some variables, e.g., the energy sources used in construction
    double accumulatedDirectCost = 0.0;
    double JoulesDieselLightMedDuty = 0.0;
    double JoulesDieselHeavyDuty = 0.0;
    double JoulesDieselEquipment = 0.0;
    double JoulesGasolineLightMedDuty = 0.0;
    double kWhElectricity = 0.0;

    // Get the number of building storeys
    auto numBuildingStoreys = theBIM->getNumberOfStoreys();

    // Vector to store the column splices
    // Splices are where the reinforcement needs to be spliced or welded between floors to maintain continuity
    QVector<QPair<QString,double>> vectorColumnSplices;

    // Iterate through the components
    for(auto&& it : theComponentList)
    {
        //  auto componentStoreyNumber = it->getBuildingStoreyNumber();

        //The ceiling height (i.e., the height of the storey from floor to ceiling, NOT storey elevation)
        //  auto storeyHeight = theBIM->getBuildingStoreyHeight(componentStoreyNumber)*3.28084; //height in feet is required but Rts returns in meters

        //The elevation of the floor slab
        //  auto storeyElevation = theBIM->getBuildingStoreyElevation(componentStoreyNumber)*3.28084; //height in feet is required but Rts returns in meters

        CSIVector& componentLabourQuantityCSICodes = it->getConstructionLabourQuantityCSICodes();
        componentLabourQuantityCSICodes.clear();
        RJob& componentJob = it->getConstructionJob();
        componentJob.reset();

        auto theRectConcreteColumn = qobject_cast<RRectangularRCColumnComponent*>(it);
        auto theConcreteSlab = qobject_cast<RRCSlabComponent*>(it);
        auto theConcreteShearwall= qobject_cast<RRCShearWallComponent*>(it);
        auto theSteelWBeamColumn= qobject_cast<RWSteelBeamColumnComponent*>(it);
        auto theRectGlulamBeamColumn= qobject_cast<RRectangularGlulamBeamColumnComponent*>(it);
        auto theCLTPanel = qobject_cast<RCLTPanelComponent*>(it);
        auto theCompositeDecking = qobject_cast<RCompositePanelComponent*>(it);

        if(theRectConcreteColumn)
        {
            //Get the number of longitudinal rebar that need to be spliced if multistorey building
            if(numBuildingStoreys > 1)
            {
                auto numRebarToSplice = theRectConcreteColumn->getNumberOfLongitudinalRebar();

                auto rebarDesignation = theRectConcreteColumn->getLongitudinalRebarDesignation();

                vectorColumnSplices.push_back(QPair<QString,double> (rebarDesignation,numRebarToSplice));
            }

            theCSIAssembler->getRectRCColLabourConstructionCodes(theRectConcreteColumn,&componentJob,&componentLabourQuantityCSICodes);
        }
        else if(theConcreteSlab)
        {
            theCSIAssembler->getRCSlabLabourConstructionCodes(theConcreteSlab,&componentJob,theBIM,&componentLabourQuantityCSICodes);
        }
        else if(theConcreteShearwall)
        {
            theCSIAssembler->getRCShearWallLabourConstructionCodes(theConcreteShearwall,&componentJob,theBIM,&componentLabourQuantityCSICodes);
        }
        else if(theSteelWBeamColumn)
        {
            theCSIAssembler->getSteelWColumnLabourConstructionCodes(theSteelWBeamColumn,&componentJob,&componentLabourQuantityCSICodes);
        }
        else if(theRectGlulamBeamColumn)
        {
            theCSIAssembler->getGlulamColumnLabourConstructionCodes(theRectGlulamBeamColumn,&componentJob,&componentLabourQuantityCSICodes);
        }
        else if(theCLTPanel)
        {
            theCSIAssembler->getCLTPanelConstructionCodes(theCLTPanel,&componentJob,&componentLabourQuantityCSICodes);
        }
        else if(theCompositeDecking)
        {
            theCSIAssembler->getSteelSkinSlabConstructionCodes(theCompositeDecking,&componentJob,&componentLabourQuantityCSICodes);
        }

        // Get the construction cost of each individual component
        auto constructionCost = theCostMethod->getLabourCosts(componentLabourQuantityCSICodes);
        constructionCost += theCostMethod->getEquipmentCosts(componentLabourQuantityCSICodes);

        // Set the cost to the component
        it->setConstructionCost(constructionCost);

        // Also set the job to the component so that we can associate crews/labourers with a particular component
        it->setConstructionJob(componentJob);

        // The CSI codes from the component to the vector of codes
        for(auto&& csi: componentLabourQuantityCSICodes)
        {
            labourQuantityCSICodes.push_back(csi);
        }

        // Set the crews from the component to the construction job
        auto crewsVec = componentJob.getCrewsOnJob();

        for(auto&& crew : crewsVec)
        {
            constructionPhase.addCrew(crew);
        }
    }

    // To do: algorithm to determine splice type
    int spliceType = 1;

    // Take into account column splices, one for every two floors
    if(numBuildingStoreys>1)
    {
        //Splice rebar, standard, self-aligning type
        if(spliceType == 1)
        {
            RCrew* crewC25 = constructionPhase.addCrew(RCrewCode::C25);

            RCrew* crewC5 = constructionPhase.addCrew(RCrewCode::C5);

            for(auto&& it: vectorColumnSplices)
            {
                auto rebarDesignation = it.first;

                auto numRebar = it.second;

                //Assume a column splice every two floors
                auto numberOfSplices = numRebar/2.0;

                if(rebarDesignation == "10M") //#4 equivalent
                {
                    //Line Number:032105750100
                    //Description: Splice rebar, standard, self-aligning type, taper threaded, #4 bars, includes holding rebar in place while splicing
                    //Unit: Each
                    //Crew: C25
                    QString CSINumber = "032105750100";
                    QString Description = "Splice rebar, standard, self-aligning type, taper threaded, #4 bars, includes holding rebar in place while splicing";

                    labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,numberOfSplices,Description));

                    crewC25->addTask(CSINumber,numberOfSplices,Description,nullptr);
                }
                else if(rebarDesignation == "15M") //#5 equivalent
                {
                    //Line Number:032105750105
                    //Description: Splice rebar, standard, self-aligning type, taper threaded, #5 bars, includes holding rebar in place while splicing
                    //Unit: Each
                    //Crew: C25

                    QString CSINumber = "032105750105";
                    QString Description = "Splice rebar, standard, self-aligning type, taper threaded, #5 bars, includes holding rebar in place while splicing";

                    labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,numberOfSplices,Description));

                    crewC25->addTask(CSINumber,numberOfSplices,Description,nullptr);
                }
                else if(rebarDesignation == "20M") //#7 equivalent
                {
                    //Line Number:032105750120
                    //Description: Splice rebar, standard, self-aligning type, taper threaded, #7 bars, includes holding rebar in place while splicing
                    //Unit: Each
                    //Crew: C25

                    QString CSINumber = "032105750120";
                    QString Description = "Splice rebar, standard, self-aligning type, taper threaded, #7 bars, includes holding rebar in place while splicing";

                    labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,numberOfSplices,Description));

                    crewC25->addTask(CSINumber,numberOfSplices,Description,nullptr);
                }
                else if(rebarDesignation == "25M") //#8 equivalent
                {
                    //Line Number:032105750300
                    //Description:
                    //Unit: Each
                    //Crew: C25

                    QString CSINumber = "032105750300";
                    QString Description = "Splice rebar, standard, self-aligning type, taper threaded, #8 bars, includes holding rebar in place while splicing";

                    labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,numberOfSplices,Description));

                    crewC25->addTask(CSINumber,numberOfSplices,Description,nullptr);
                }
                else if(rebarDesignation == "30M") //#10 equivalent
                {
                    //Line Number:032105750310
                    //Description: Splice rebar, standard, self-aligning type, taper threaded, #10 bars, includes holding rebar in place while splicing
                    //Unit: Each
                    //Crew: C5

                    QString CSINumber = "032105750310";
                    QString Description = "Splice rebar, standard, self-aligning type, taper threaded, #10 bars, includes holding rebar in place while splicing";

                    labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,numberOfSplices,Description));

                    crewC5->addTask(CSINumber,numberOfSplices,Description,nullptr);
                }
                else if(rebarDesignation == "35M") //#11 equivalent
                {
                    //Line Number:032105750320
                    //Description: Splice rebar, standard, self-aligning type, taper threaded, #11 bars, includes holding rebar in place while splicing
                    //Unit: Each
                    //Crew: C5

                    QString CSINumber = "032105750320";
                    QString Description = "Splice rebar, standard, self-aligning type, taper threaded, #11 bars, includes holding rebar in place while splicing";

                    labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,numberOfSplices,Description));

                    crewC5->addTask(CSINumber,numberOfSplices,Description,nullptr);
                }
                else if(rebarDesignation == "45M") //#14 equivalent
                {
                    //Line Number:032105750330
                    //Description: Splice rebar, standard, self-aligning type, taper threaded, #14 bars, includes holding rebar in place while splicing
                    //Unit: Each
                    //Crew: C5

                    QString CSINumber = "032105750330";
                    QString Description = "Splice rebar, standard, self-aligning type, taper threaded, #14 bars, includes holding rebar in place while splicing";

                    labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,numberOfSplices,Description));

                    crewC5->addTask(CSINumber,numberOfSplices,Description,nullptr);
                }
                else if(rebarDesignation == "55M") //#18 equivalent
                {
                    //Line Number:032105750340
                    //Description: Splice rebar, standard, self-aligning type, taper threaded, #18 bars, includes holding rebar in place while splicing
                    //Unit: Each
                    //Crew: C5

                    QString CSINumber = "032105750340";
                    QString Description = "Splice rebar, standard, self-aligning type, taper threaded, #18 bars, includes holding rebar in place while splicing";

                    labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,numberOfSplices,Description));

                    crewC5->addTask(CSINumber,numberOfSplices,Description,nullptr);
                }
                else
                {
                    qDebug()<<"There is no splice costing information for the rebar designation of "<<rebarDesignation<<" in RStevesConstructionImpactModel";
                }

            }
        }
    }

    // Get the additional costs, i.e., economy of scale costs
    theCSIAssembler->getAdditionalLabourCosts(&constructionPhase,theBIM,&labourQuantityCSICodes);

    //**************************************************
    //**************************************************
    //***********Labour and Equipment Costs*************
    //**************************************************
    //**************************************************

    // Calculate the labour costs - note that the material costs are in the manufacturing stage of the building
    auto labourDirectCosts = theCostMethod->getLabourCosts(labourQuantityCSICodes);

    if (theOutputLevel >= RObject::Maximum)
    {
        qDebug()<<"***LABOUR DIRECT COSTS***"<<labourDirectCosts;
    }

    accumulatedDirectCost += labourDirectCosts;

    auto equipmentDirectCosts = theCostMethod->getEquipmentCosts(labourQuantityCSICodes);

    if (theOutputLevel >= RObject::Maximum)
    {
        qDebug()<<"***EQUIPMENT DIRECT COSTS***"<<equipmentDirectCosts;
    }

    accumulatedDirectCost += equipmentDirectCosts;

    // Set the response from this model, i.e., the cumulative cost, energy, and time
    theCostResponse->setCurrentValue(accumulatedDirectCost);
    theStatsMethod->update(theCostResponse->objectName()+"LabourCosts",labourDirectCosts);
    theStatsMethod->update(theCostResponse->objectName()+"EquipmentCosts",equipmentDirectCosts);
    theStatsMethod->update(theCostResponse->objectName(),accumulatedDirectCost);

    //**************************************************
    //**************************************************
    //*****************Time and labour******************
    //**************************************************
    //**************************************************

    theScheduler->setTheCostMethod(theCostMethod);
    theScheduler->addJob(constructionPhase);

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"******** TIME ********";
    }

    // The critical path is estimated below, i.e., the time for construction
    auto theCriticalPath = theScheduler->getCriticalPath();

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"The critcal path [hrs]"<<theCriticalPath;
        qDebug()<<"The critcal path [day]"<<theCriticalPath/24.0;
        qDebug()<<"The critcal path [month]"<<theCriticalPath/720.0;
    }

    // The total worker hours are calculated here (different from the critical path)
    double totalWorkHours = theScheduler->getTotalWorkHours();

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"The total work done [hrs]"<<totalWorkHours;
        qDebug()<<"The total work done [day]"<<totalWorkHours/24.0;
        qDebug()<<"The total work done [month]"<<totalWorkHours/720.0;
    }

    // The number of workers
    int numWorkers = theScheduler->getNumberOfWorkers();

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"The number of workers: "<<numWorkers;
    }

    // The number of crews
    int numCrew = theScheduler->getNumberOfCrews();

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"The number of crews: "<<numCrew;
    }

    // Set the time response
    theTimeResponse->setCurrentValue(theCriticalPath/24.0);
    theStatsMethod->update(theTimeResponse->objectName(),theCriticalPath/24.0);
    theStatsMethod->update(this->objectName()+"NumberOfWorkers",numWorkers);
    theStatsMethod->update(this->objectName()+"NumberOfCrews",numCrew);

    //****************************************************************************
    //****************************************************************************
    //********************Emissions and Environmental Impacts*********************
    //****************************************************************************
    //****************************************************************************

    // The emissions from material manufacturing are accounted for in the manufacturing impact model
    // Here the emissions from the wood forms used in construction of concrete buildings are taken into account
    // Create a dummy material object to provide the values for emissions and environmental impacts
    auto woodMaterial = make_unique<RGlulamMaterialKnowledge>(theDomain, this->objectName());

    // Convert yards to m^3 - emissions and impacts are given m^3 of wood
    auto totalVolumeWood = constructionPhase.getYrdWood()*0.764555;

    // Volume of wood in m^3 - all emissions are given in kg
    auto CarbonDioxideEmissions  =  totalVolumeWood*woodMaterial->getCarbonDioxideEmissions();
    auto CarbonMonoxideEmissions =  totalVolumeWood*woodMaterial->getCarbonMonoxideEmissions();
    auto OrganicCarbonEmissions  =  totalVolumeWood*woodMaterial->getOrganicCarbonEmissions ();
    auto MethaneEmissions        =  totalVolumeWood*woodMaterial->getMethaneEmissions();
    auto NitrogenOxidesEmissions =  totalVolumeWood*woodMaterial->getNitrogenOxidesEmissions();
    auto HFC134aEmissions        =  totalVolumeWood*woodMaterial->getHFC134aEmissions();
    auto SulfurDioxideEmissions  =  totalVolumeWood*woodMaterial->getSulfurDioxideEmissions();
    auto AmmoniaEmissions        =  totalVolumeWood*woodMaterial->getAmmoniaEmissions();
    auto NitrousOxideEmissions   =  totalVolumeWood*woodMaterial->getNitrousOxideEmissions();
    auto BlackCarbonPMEmissions  =  totalVolumeWood*woodMaterial->getBlackCarbonPMEmissions ();

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

    if (theOutputLevel >= RObject::Maximum)
    {
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

    //The ReCiPe impacts are given in their respective units
    auto AgriculturalLandOccupation = totalVolumeWood*woodMaterial->getAgriculturalLandOccupationImpact();
    auto ClimateChange              = totalVolumeWood*woodMaterial->getClimateChangeImpact();
    auto FreshwaterEcotoxicity      = totalVolumeWood*woodMaterial->getFreshwaterEcotoxicityImpact();
    auto FreshwaterEutrophication   = totalVolumeWood*woodMaterial->getFreshwaterEutrophicationImpact();
    auto HumanToxicity              = totalVolumeWood*woodMaterial->getHumanToxicityImpact();
    auto IonisingRadiation          = totalVolumeWood*woodMaterial->getIonisingRadiationImpact();
    auto MarineEcotoxicity          = totalVolumeWood*woodMaterial->getMarineEcotoxicityImpact();
    auto MarineEutrophication       = totalVolumeWood*woodMaterial->getMarineEutrophicationImpact();
    auto OzoneDepletion             = totalVolumeWood*woodMaterial->getOzoneDepletionImpact();
    auto ParticulateMatterFormation = totalVolumeWood*woodMaterial->getParticulateMatterFormationImpact();
    auto PhotochemicalOxidant       = totalVolumeWood*woodMaterial->getPhotochemicalOxidantFormationImpact();
    auto TerrestrialAcidification   = totalVolumeWood*woodMaterial->getTerrestrialAcidificationImpact();
    auto TerrestrialEcotoxicity     = totalVolumeWood*woodMaterial->getTerrestrialEcotoxicityImpact();
    auto UrbanLandOccupation        = totalVolumeWood*woodMaterial->getUrbanLandOccupationImpact();

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

    if (theOutputLevel >= RObject::Maximum)
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

    // Energy intensity demand for work with heavy machinery, i^H
    double heavy_machinery_use_intensity = theHeavyMachineryUseIntensity->getCurrentValue();

    // Get the construction (building) type
    QString constructionType = theBIM->getConstructionType();

    // Use of heavy machinery in construction of building -- assumes heavy machinery uses diesel
    // Assumes that the hours of heavy machinery useage is a fraction of the total worker hours used in constructions
    auto joulesDieselHeavyMachinery = 0.0;

    if (constructionType == QString("high-rise-concrete"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_high_rise_concrete*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }
    else if (constructionType == QString("high-rise-steel"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_high_rise_steel*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }
    else if (constructionType == QString("high-rise-wood"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_high_rise_wood*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }
    else if (constructionType == QString("low-rise-concrete"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_low_rise_concrete*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }
    else if (constructionType == QString("low-rise-steel"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_low_rise_steel*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }
    else if (constructionType == QString("low-rise-wood"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_low_rise_wood*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }

    JoulesDieselEquipment += joulesDieselHeavyMachinery;

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

    JoulesGasolineLightMedDuty += joulesGasolineWorkerTransport;
    JoulesDieselLightMedDuty += joulesDieselWorkerTransport;
    kWhElectricity += joulesElectricityWorkerTransport*2.77778e-7; //Convert joules to kWh

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
    auto totalMassConcrete = theBIM->getVolumeOfConcrete()*2450;
    auto totalMassSteel = theBIM->getVolumeOfSteel()*8050;
    auto totalMassWood = totalVolumeWood*800;

    // Total joules consumed for the transport of all materials
    auto joulesDieselMaterialTransport = totalMassConcrete*materialTransportationIntensityConcrete*concreteTravelDistance +
            totalMassSteel*materialTransportationIntensitySteel*steelTravelDistance +
            totalMassWood*materialTransportationIntensityWood*woodTravelDistance;

    JoulesDieselHeavyDuty += joulesDieselMaterialTransport;

    // Save the energy responses
    theEnergyResponse->setCurrentValue("kWhElectricity",kWhElectricity);
    theEnergyResponse->setCurrentValue("JoulesDieselLightMediumDutyTransport",JoulesDieselLightMedDuty);
    theEnergyResponse->setCurrentValue("JoulesDieselHeavyDutyTransport",JoulesDieselHeavyDuty);
    theEnergyResponse->setCurrentValue("JoulesDieselEquipmentUsage",JoulesDieselEquipment);
    theEnergyResponse->setCurrentValue("JoulesGasolineLightMediumDutyTransport",JoulesGasolineLightMedDuty);
    theStatsMethod->update(theEnergyResponse->objectName()+"kWhElectricity",kWhElectricity);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesDieselLightMediumDutyTransport",JoulesDieselLightMedDuty);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesDieselHeavyDutyTransport",JoulesDieselHeavyDuty);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesDieselEquipmentUsage",JoulesDieselEquipment);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesGasolineLightMediumDutyTransport",JoulesGasolineLightMedDuty);

    // Give output if requested
    if (theOutputLevel == RObject::Medium)
    {
        qDebug() << "After the construction phase the cost is" << accumulatedDirectCost;
    }

    return 1;
}


int RStevesConstructionImpactModel::resetTime()
{
    labourQuantityCSICodes.clear();
    theScheduler->reset();

    return 0;
}


void RStevesConstructionImpactModel::createDatabaseOfRandomVariables()
{
    // Random variable for machinery use intensity
    theDomain->createObject(objectName() + "HeavyMachineryUseIntensity", "RContinuousRandomVariable");
    theHeavyMachineryUseIntensity = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theHeavyMachineryUseIntensity->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theHeavyMachineryUseIntensity->setMean(QString("2680000"));
    theHeavyMachineryUseIntensity->setCurrentValue(2680000);
    theHeavyMachineryUseIntensity->setCoefficientOfVariation("0.2");

    // Average of medium and heavy duty transport
    theDomain->createObject(objectName() + "MaterialTransportationIntensityConcrete", "RContinuousRandomVariable");
    theMaterialTransportationIntensityConcrete = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theMaterialTransportationIntensityConcrete->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theMaterialTransportationIntensityConcrete->setMean(QString("4855"));
    theMaterialTransportationIntensityConcrete->setCurrentValue(4855);
    theMaterialTransportationIntensityConcrete->setCoefficientOfVariation("0.1");

    // Transportation sector energy intensities for worker and material transport
    // British Columbia and Territories
    // https://oee.nrcan.gc.ca/corporate/statistics/neud/dpa/showTable.cfm?type=CP&sector=tran&juris=bct&rn=36&page=0
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


