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

#include "RStevesManufacturingImpactModel.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RComponent.h"
#include "RCSICostMethod.h"
#include "RCSIAssembler.h"
#include "RResponse.h"
#include "RInfoRichResponse.h"
#include "RBIM.h"
#include "RRectangularRCColumnComponent.h"
#include "RRCSlabComponent.h"
#include "RRCShearWallComponent.h"
#include "RWSteelBeamColumnComponent.h"
#include "RRectangularGlulamBeamColumnComponent.h"
#include "RCLTPanelComponent.h"
#include "RCompositePanelComponent.h"
#include "RConcreteComponent.h"
#include "RSteelMaterialKnowledge.h"
#include "RConcreteMaterialKnowledge.h"
#include "RGlulamMaterialKnowledge.h"
#include "RStatistics.h"

#include <QCoreApplication>

RStevesManufacturingImpactModel::RStevesManufacturingImpactModel(QObject *parent, QString name)
    : RModel(parent, name)
{
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

    // InformationRichResponses
    // Emissions
    theDomain->createObject(objectName() + "EmissionsResponse", "RInfoRichResponse");
    theEmissionsResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEmissionsResponse->setModel(this);

    // Energy
    theDomain->createObject(objectName() + "EnergyResponse", "RInfoRichResponse");
    theEnergyResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEnergyResponse->setModel(this);

    // Create the CSI assembler
    theCSIAssembler = std::make_unique<RCSIAssembler>(theDomain,"ManufacturingImpactCSIAssembler");
    theStatsMethod = theDomain->getStatsMethod();
}


RStevesManufacturingImpactModel::~RStevesManufacturingImpactModel()
{

}


QObject * RStevesManufacturingImpactModel::getBIM() const
{
    return theBIM;
}


void RStevesManufacturingImpactModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


int RStevesManufacturingImpactModel::evaluateModel(RGradientType theGradientType)
{ 
    // Clear the responses
    theCostResponse->setCurrentValue(0.0);
    theTimeResponse->setCurrentValue(0.0);
    theWaterResponse->setCurrentValue(0.0);
    // Reset the informationRich responses
    theEmissionsResponse->clearResponses();
    theEnergyResponse->clearResponses();
    materialQuantityCSICodes.clear();

    // Check that there is a BIM (always needed)
    if(theBIM == nullptr)
    {
        qCritical() << this->objectName()<<" needs a BIM";
        return 0;
    }

    if (theCurrentTime->getCurrentValue() != theBIM->getTimeOfConstructionValue())
    {
        return 0;
    }

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"*********************************";
        qDebug()<<"*******Manufacturing Phase*******";
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

    // Declare cost, energy, and time to be accumulated
    double accumulatedDirectCost = 0.0;

    // Iterate through the components
    for(auto&& it : theComponentList)
    {
        if(!it->meshWasCreated)
            it->createMesh();

        CSIVector& componentMaterialQuantityCSICodes = it->getManufacturingMaterialQuantityCSICodes();
        componentMaterialQuantityCSICodes.clear();

        auto theRectConcreteColumn = qobject_cast<RRectangularRCColumnComponent*>(it);
        auto theConcreteSlab = qobject_cast<RRCSlabComponent*>(it);
        auto theConcreteShearwall= qobject_cast<RRCShearWallComponent*>(it);
        auto theSteelBeamColumn= qobject_cast<RWSteelBeamColumnComponent*>(it);
        auto theGlulamBeamColumn= qobject_cast<RRectangularGlulamBeamColumnComponent*>(it);
        auto theCLTPanel = qobject_cast<RCLTPanelComponent*>(it);
        auto theCompositeSteelPanel = qobject_cast<RCompositePanelComponent*>(it);

        if(theRectConcreteColumn)
        {
            theCSIAssembler->getRectRCColMaterialConstructionCodes(theRectConcreteColumn,nullptr,&componentMaterialQuantityCSICodes);
        }
        else if(theConcreteSlab)
        {
            theCSIAssembler->getRCSlabMaterialConstructionCodes(theConcreteSlab,nullptr,theBIM,&componentMaterialQuantityCSICodes);
        }
        else if(theConcreteShearwall)
        {
            theCSIAssembler->getRCShearWallMaterialConstructionCodes(theConcreteShearwall,nullptr,theBIM,&componentMaterialQuantityCSICodes);
        }
        else if(theSteelBeamColumn)
        {
            theCSIAssembler->getSteelWColumnMaterialConstructionCodes(theSteelBeamColumn,&componentMaterialQuantityCSICodes);
        }
        else if(theGlulamBeamColumn)
        {
            theCSIAssembler->getGlulamColumnMaterialConstructionCodes(theGlulamBeamColumn,&componentMaterialQuantityCSICodes);
        }
        else if(theCLTPanel)
        {
            theCSIAssembler->getCLTPanelConstructionCodes(theCLTPanel,nullptr,&componentMaterialQuantityCSICodes);
        }
        else if(theCompositeSteelPanel)
        {
            theCSIAssembler->getSteelSkinSlabConstructionCodes(theCompositeSteelPanel,nullptr,&componentMaterialQuantityCSICodes);
        }

        // Get the manufacturing cost of each individual component
        auto manufacturingCost = theCostMethod->getMaterialCosts(componentMaterialQuantityCSICodes);
        manufacturingCost += theCostMethod->getEquipmentCosts(componentMaterialQuantityCSICodes);

        // Set the cost to the component
        it->setManufacturingCost(manufacturingCost);

        // The CSI codes from the component to the vector of codes
        for(auto&& csi: componentMaterialQuantityCSICodes)
        {
            materialQuantityCSICodes.push_back(csi);
        }
    }


    //**************************************************
    //**************************************************
    //***************Cost of Materials******************
    //**************************************************
    //**************************************************

    // Calculate the material costs
    auto materialDirectCosts = theCostMethod->getMaterialCosts(materialQuantityCSICodes);

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"***MATERIAL DIRECT COSTS***"<<materialDirectCosts;
    }

    accumulatedDirectCost += materialDirectCosts;

    // Set the response for direct cost
    theCostResponse->setCurrentValue(accumulatedDirectCost);
    theStatsMethod->update(theCostResponse->objectName()+"MaterialCosts",materialDirectCosts);
    theStatsMethod->update(theCostResponse->objectName(),accumulatedDirectCost);

    //**************************************************
    //**************************************************
    //**************Time to Manufacture*****************
    //**************************************************
    //**************************************************

    // To do: query a NETWORK MODEL to get the lead-time for materials
    //    for (auto it : materialQuantityCSICodes)
    //    {
    //        // lead time For each material
    //    }

    //****************************************************************************
    //****************************************************************************
    //********************Emissions and Environmental Impacts*********************
    //****************************************************************************
    //****************************************************************************

    // Mass in kg and volume in m^3
    auto totalMassSteel = 0.0;
    auto totalVolumeConcrete = 0.0;
    auto totalVolumeWood = 0.0;

    for(auto&& it : theComponentList)
    {
        // Mass of steel in kg
        auto massSteel = it->getVolumeOfSteel()*7900.0;

        // Volume of concrete in m^3
        auto volConc = it->getVolumeOfConcrete();

        // Volume of wood in m^3
        auto volWood =  it->getVolumeOfWood();

        totalMassSteel += massSteel;
        totalVolumeConcrete += volConc;
        totalVolumeWood += volWood;
    }

    // Create dummy material objects to provide the values for emissions and environmental impacts
    auto concreteMaterial = make_unique<RConcreteMaterialKnowledge>(theDomain, this->objectName(), "C20");
    auto steelMaterial = make_unique<RSteelMaterialKnowledge>(theDomain, this->objectName(), "NULL");
    auto woodMaterial = make_unique<RGlulamMaterialKnowledge>(theDomain, this->objectName());

    // Concrete/wood material volumes should be in m^3 and steel in kg
    // All emissions are given in kg
    auto CarbonDioxideEmissions  = totalMassSteel*steelMaterial->getCarbonDioxideEmissions()  + totalVolumeConcrete*concreteMaterial->getCarbonDioxideEmissions()   + totalVolumeWood*woodMaterial->getCarbonDioxideEmissions();
    auto CarbonMonoxideEmissions = totalMassSteel*steelMaterial->getCarbonMonoxideEmissions() + totalVolumeConcrete*concreteMaterial->getCarbonMonoxideEmissions()  + totalVolumeWood*woodMaterial->getCarbonMonoxideEmissions();
    auto OrganicCarbonEmissions  = totalMassSteel*steelMaterial->getOrganicCarbonEmissions()  + totalVolumeConcrete*concreteMaterial->getOrganicCarbonEmissions()   + totalVolumeWood*woodMaterial->getOrganicCarbonEmissions ();
    auto MethaneEmissions        = totalMassSteel*steelMaterial->getMethaneEmissions()        + totalVolumeConcrete*concreteMaterial->getMethaneEmissions()         + totalVolumeWood*woodMaterial->getMethaneEmissions();
    auto NitrogenOxidesEmissions = totalMassSteel*steelMaterial->getNitrogenOxidesEmissions() + totalVolumeConcrete*concreteMaterial->getNitrogenOxidesEmissions()  + totalVolumeWood*woodMaterial->getNitrogenOxidesEmissions();
    auto HFC134aEmissions        = totalMassSteel*steelMaterial->getHFC134aEmissions()        + totalVolumeConcrete*concreteMaterial->getHFC134aEmissions()         + totalVolumeWood*woodMaterial->getHFC134aEmissions();
    auto SulfurDioxideEmissions  = totalMassSteel*steelMaterial->getSulfurDioxideEmissions () + totalVolumeConcrete*concreteMaterial->getSulfurDioxideEmissions ()  + totalVolumeWood*woodMaterial->getSulfurDioxideEmissions();
    auto AmmoniaEmissions        = totalMassSteel*steelMaterial->getAmmoniaEmissions()        + totalVolumeConcrete*concreteMaterial->getAmmoniaEmissions()         + totalVolumeWood*woodMaterial->getAmmoniaEmissions();
    auto NitrousOxideEmissions   = totalMassSteel*steelMaterial->getNitrousOxideEmissions()   + totalVolumeConcrete*concreteMaterial->getNitrousOxideEmissions()    + totalVolumeWood*woodMaterial->getNitrousOxideEmissions();
    auto BlackCarbonPMEmissions  = totalMassSteel*steelMaterial->getBlackCarbonPMEmissions()  + totalVolumeConcrete*concreteMaterial->getBlackCarbonPMEmissions()   + totalVolumeWood*woodMaterial->getBlackCarbonPMEmissions ();

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

    if (theOutputLevel >= RObject::Maximum) {
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
    //**************************************************
    //*********************Energy***********************
    //**************************************************
    //**************************************************

    // Energy from material manufacturing (and the emissions from its usage) is accounted for in the ReCiPe2016 impacts calculated above
    // Energy for delivery of material is calculated in the construction impact model

    if (theOutputLevel == RObject::Medium) {
        qDebug()<<"\n";
        qDebug()<<"After the manufacturing phase the cost is: "<< accumulatedDirectCost;
    }

    return 1;
}


int RStevesManufacturingImpactModel::resetTime()
{
    materialQuantityCSICodes.clear();

    return 0;
}




