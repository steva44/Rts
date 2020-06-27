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

#include "RCostOfEmissionsModel.h"
#include "RContinuousRandomVariable.h"
#include "RInfoRichResponse.h"
#include "RStatistics.h"
#include "RDomain.h"
#include "RBIM.h"

RCostOfEmissionsModel::RCostOfEmissionsModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theResponse = nullptr;
    theCostOfDamageToHumanHealthResponse = nullptr;
    theCostOfEnvironmentalDamageResponse = nullptr;

    // Create the response objects
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    theDomain->createObject(objectName() + "theCostOfDamageToHumanHealthResponse", "RGenericResponse");
    theCostOfDamageToHumanHealthResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theCostOfDamageToHumanHealthResponse->setModel(this);

    theDomain->createObject(objectName() + "theCostOfEnvironmentalDamageResponse", "RGenericResponse");
    theCostOfEnvironmentalDamageResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theCostOfEnvironmentalDamageResponse->setModel(this);

    theDomain->createObject(objectName() + "theCostAtMaxDriftResponse", "RGenericResponse");
    theCostAtMaxDriftResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theCostAtMaxDriftResponse->setModel(this);

    theDomain->createObject(objectName() + "theMaxDriftResponse", "RGenericResponse");
    theMaxDriftResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theMaxDriftResponse->setModel(this);

    theEmissionsToHumanHealthCostConversionCO2       = nullptr;
    theEmissionsToHumanHealthCostConversionCH4       = nullptr;
    theEmissionsToHumanHealthCostConversionN2O       = nullptr;
    theEmissionsToHumanHealthCostConversionHFC_134a  = nullptr;
    theEmissionsToHumanHealthCostConversionBC        = nullptr;
    theEmissionsToHumanHealthCostConversionSO2       = nullptr;
    theEmissionsToHumanHealthCostConversionCO        = nullptr;
    theEmissionsToHumanHealthCostConversionOC        = nullptr;
    theEmissionsToHumanHealthCostConversionNOx       = nullptr;
    theEmissionsToHumanHealthCostConversionNH3       = nullptr;
    theEmissionsToEnvironmentalCostConversionCO2     = nullptr;
    theEmissionsToEnvironmentalCostConversionCH4     = nullptr;
    theEmissionsToEnvironmentalCostConversionN2O     = nullptr;
    theEmissionsToEnvironmentalCostConversionSO2     = nullptr;
    theEmissionsToEnvironmentalCostConversionCO      = nullptr;
    theEmissionsToEnvironmentalCostConversionNOx     = nullptr;
    theEmissionsToEnvironmentalCostConversionNH3     = nullptr;

    AgriculturalLandOccupationToCost      = nullptr;
    ClimateChangeToCost                   = nullptr;
    FreshwaterEcotoxicityToCost           = nullptr;
    FreshwaterEutrophicationToCost        = nullptr;
    HumanToxicityToCost                   = nullptr;
    IonisingRadiationToCost               = nullptr;
    MarineEcotoxicityToCost               = nullptr;
    MarineEutrophicationToCost            = nullptr;
    OzoneDepletionToCost                  = nullptr;
    ParticulateMatterFormationToCost      = nullptr;
    PhotochemicalOxidantToCost            = nullptr;
    TerrestrialAcidificationToCost        = nullptr;
    TerrestrialEcotoxicityToCost          = nullptr;
    UrbanLandOccupationToCost             = nullptr;

    // Do not double discount the cost of emissions in the scenario model as they are already discounted at the rate set below
    theResponse->setDiscount(false);

    discountRate = "3%";

    createHumanHealthCostRandomVariables();

    createEnvironmentalCostRandomVariables();

    createMidpointCostRandomVariables();

    theStatsMethod = theDomain->getStatsMethod();
}


RCostOfEmissionsModel::~RCostOfEmissionsModel()
{

}


QString RCostOfEmissionsModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}


void RCostOfEmissionsModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


int RCostOfEmissionsModel::evaluateModel(RGradientType theGradientType)
{

    if (theOutputLevel >= RObject::Maximum) {

        qDebug()<<"***************************";
        qDebug()<<"*****Cost of Emissions*****";
        qDebug()<<"***************************";
    }

    // Reset the responses
    theResponse->setCurrentValue(0.0);
    theCostOfDamageToHumanHealthResponse->setCurrentValue(0.0);
    theCostOfEnvironmentalDamageResponse->setCurrentValue(0.0);


    // Get the models that serve input to this model
    QList<RInfoRichResponse *> inputResponselList;
    for (auto it : theInputParameterList)
    {
        RInfoRichResponse *theInputCastAsResponse = qobject_cast<RInfoRichResponse *>(it);

        if(theInputCastAsResponse)
            inputResponselList.push_back(theInputCastAsResponse);
    }

    auto totalCostOfEmissions = 0.0;

    auto totalCostCarbonDioxideEmissions  = 0.0;
    auto totalCostMethaneEmissions        = 0.0;
    auto totalCostNitrousOxideEmissions   = 0.0;
    auto totalCostHFC134aEmissions        = 0.0;
    auto totalCostBlackCarbonPMEmissions  = 0.0;
    auto totalCostSulfurDioxideEmissions  = 0.0;
    auto totalCostCarbonMonoxideEmissions = 0.0;
    auto totalCostOrganicCarbonEmissions  = 0.0;
    auto totalCostNitrogenOxidesEmissions = 0.0;
    auto totalCostAmmoniaEmissions        = 0.0;

    auto totalCostOfAgriculturalLandOccupation = 0.0;
    auto totalCostOfClimateChange              = 0.0;
    auto totalCostOfFreshwaterEcotoxicity      = 0.0;
    auto totalCostOfFreshwaterEutrophication   = 0.0;
    auto totalCostOfHumanToxicity              = 0.0;
    auto totalCostOfIonisingRadiation          = 0.0;
    auto totalCostOfMarineEcotoxicity          = 0.0;
    auto totalCostOfMarineEutrophication       = 0.0;
    auto totalCostOfOzoneDepletion             = 0.0;
    auto totalCostOfParticulateMatterFormation = 0.0;
    auto totalCostOfPhotochemicalOxidant       = 0.0;
    auto totalCostOfTerrestrialAcidification   = 0.0;
    auto totalCostOfTerrestrialEcotoxicity     = 0.0;
    auto totalCostOfUrbanLandOccupation        = 0.0;

    auto totalCostCO2EmissionsWorkerTransport    = 0.0;
    auto totalCostCO2EmissionsMaterialTransport  = 0.0;
    auto totalCostCO2EmissionsEquipment          = 0.0;
    auto totalCostCO2EmissionsBuildOpsElectricity= 0.0;
    auto totalCostCO2EmissionsBuildOpsNaturalGas = 0.0;
    auto totalCostCH4EmissionsWorkerTransport    = 0.0;
    auto totalCostCH4EmissionsMaterialTransport  = 0.0;
    auto totalCostCH4EmissionsEquipment          = 0.0;
    auto totalCostCH4EmissionsBuildingOperations = 0.0;
    auto totalCostN2OEmissionsWorkerTransport    = 0.0;
    auto totalCostN2OEmissionsMaterialTransport  = 0.0;
    auto totalCostN2OEmissionsEquipment          = 0.0;
    auto totalCostN2OEmissionsBuildingOperations = 0.0;

    auto totalHumanHealthCostEmissions = 0.0;
    auto totalEnvironmentalCostEmissions = 0.0;

    // Convert from kilogram into metric tonne
    auto massConversion = 1.0e-3;

    // For each response
    for(auto it : inputResponselList)
    {
        if(it->isEmpty())
            continue;

        // The following emissions are from materials manufacturing, to calculate the cost of damage to human health
        auto CarbonDioxideEmissions  = it->getCurrentValue("CarbonDioxideEmissions");
        auto MethaneEmissions        = it->getCurrentValue("MethaneEmissions");
        auto NitrousOxideEmissions   = it->getCurrentValue("NitrousOxideEmissions");
        auto HFC134aEmissions        = it->getCurrentValue("HFC134aEmissions");
        auto BlackCarbonPMEmissions  = it->getCurrentValue("BlackCarbonPMEmissions");
        auto SulfurDioxideEmissions  = it->getCurrentValue("SulfurDioxideEmissions");
        auto CarbonMonoxideEmissions = it->getCurrentValue("CarbonMonoxideEmissions");
        auto OrganicCarbonEmissions  = it->getCurrentValue("OrganicCarbonEmissions");
        auto NitrogenOxidesEmissions = it->getCurrentValue("NitrogenOxidesEmissions");
        auto AmmoniaEmissions        = it->getCurrentValue("AmmoniaEmissions");

        auto CostCarbonDioxideEmissions  = theEmissionsToHumanHealthCostConversionCO2->getCurrentValue()*CarbonDioxideEmissions*massConversion;
        auto CostMethaneEmissions        = theEmissionsToHumanHealthCostConversionCH4->getCurrentValue()*MethaneEmissions*massConversion;
        auto CostNitrousOxideEmissions   = theEmissionsToHumanHealthCostConversionN2O->getCurrentValue()*NitrousOxideEmissions*massConversion;
        auto CostHFC134aEmissions        = theEmissionsToHumanHealthCostConversionHFC_134a->getCurrentValue()*HFC134aEmissions*massConversion;
        auto CostBlackCarbonPMEmissions  = theEmissionsToHumanHealthCostConversionBC->getCurrentValue()*BlackCarbonPMEmissions*massConversion;
        auto CostSulfurDioxideEmissions  = theEmissionsToHumanHealthCostConversionSO2->getCurrentValue()*SulfurDioxideEmissions*massConversion;
        auto CostCarbonMonoxideEmissions = theEmissionsToHumanHealthCostConversionCO->getCurrentValue()*CarbonMonoxideEmissions*massConversion;
        auto CostOrganicCarbonEmissions  = theEmissionsToHumanHealthCostConversionOC->getCurrentValue()*OrganicCarbonEmissions*massConversion;
        auto CostNitrogenOxidesEmissions = theEmissionsToHumanHealthCostConversionNOx->getCurrentValue()*NitrogenOxidesEmissions*massConversion;
        auto CostAmmoniaEmissions        = theEmissionsToHumanHealthCostConversionNH3->getCurrentValue()*AmmoniaEmissions*massConversion;

        // The human health cost from material manufacturing
        auto humanHealthCostEmissions = CostCarbonDioxideEmissions + CostMethaneEmissions + CostNitrousOxideEmissions  + CostHFC134aEmissions + CostBlackCarbonPMEmissions + CostSulfurDioxideEmissions +
                CostCarbonMonoxideEmissions+ CostOrganicCarbonEmissions + CostNitrogenOxidesEmissions+ CostAmmoniaEmissions;

        theStatsMethod->update(it->objectName()+"CostCarbonDioxideEmissions",CostCarbonDioxideEmissions);
        theStatsMethod->update(it->objectName()+"CostMethaneEmissions",CostMethaneEmissions);
        theStatsMethod->update(it->objectName()+"CostNitrousOxideEmissions",CostNitrousOxideEmissions);
        theStatsMethod->update(it->objectName()+"CostHFC134aEmissions",CostHFC134aEmissions);
        theStatsMethod->update(it->objectName()+"CostBlackCarbonPMEmissions",CostBlackCarbonPMEmissions);
        theStatsMethod->update(it->objectName()+"CostSulfurDioxideEmissions",CostSulfurDioxideEmissions);
        theStatsMethod->update(it->objectName()+"CostCarbonMonoxideEmissions",CostCarbonMonoxideEmissions);
        theStatsMethod->update(it->objectName()+"CostOrganicCarbonEmissions",CostOrganicCarbonEmissions);
        theStatsMethod->update(it->objectName()+"CostNitrogenOxidesEmissions",CostNitrogenOxidesEmissions);
        theStatsMethod->update(it->objectName()+"CostAmmoniaEmissions",CostAmmoniaEmissions);

        // Next the cost of environmental damage from material manufacturing is calculated with from the ReCiPe2016 impact categories
        auto AgriculturalLandOccupation  = it->getCurrentValue("AgriculturalLandOccupation");
        auto ClimateChange               = it->getCurrentValue("ClimateChange");
        auto FreshwaterEcotoxicity       = it->getCurrentValue("FreshwaterEcotoxicity");
        auto FreshwaterEutrophication    = it->getCurrentValue("FreshwaterEutrophication");
        auto HumanToxicity               = it->getCurrentValue("HumanToxicity");
        auto IonisingRadiation           = it->getCurrentValue("IonisingRadiation");
        auto MarineEcotoxicity           = it->getCurrentValue("MarineEcotoxicity");
        auto MarineEutrophication        = it->getCurrentValue("MarineEutrophication");
        auto OzoneDepletion              = it->getCurrentValue("OzoneDepletion");
        auto ParticulateMatterFormation  = it->getCurrentValue("ParticulateMatterFormation");
        auto PhotochemicalOxidant        = it->getCurrentValue("PhotochemicalOxidant");
        auto TerrestrialAcidification    = it->getCurrentValue("TerrestrialAcidification");
        auto TerrestrialEcotoxicity      = it->getCurrentValue("TerrestrialEcotoxicity");
        auto UrbanLandOccupation         = it->getCurrentValue("UrbanLandOccupation");

        // auto costOfAgriculturalLandOccupation =  AgriculturalLandOccupation*AgriculturalLandOccupationToCost->getCurrentValue();
        auto costOfAgriculturalLandOccupation =  0.0;
        auto costOfClimateChange              =  ClimateChange*ClimateChangeToCost->getCurrentValue();
        auto costOfFreshwaterEcotoxicity      =  FreshwaterEcotoxicity*FreshwaterEcotoxicityToCost->getCurrentValue();
        auto costOfFreshwaterEutrophication   =  FreshwaterEutrophication*FreshwaterEutrophicationToCost->getCurrentValue();
        auto costOfHumanToxicity              =  HumanToxicity*HumanToxicityToCost->getCurrentValue();
        auto costOfIonisingRadiation          =  IonisingRadiation*IonisingRadiationToCost->getCurrentValue();
        auto costOfMarineEcotoxicity          =  MarineEcotoxicity*MarineEcotoxicityToCost->getCurrentValue();
        auto costOfMarineEutrophication       =  MarineEutrophication*MarineEutrophicationToCost->getCurrentValue();
        auto costOfOzoneDepletion             =  OzoneDepletion*OzoneDepletionToCost->getCurrentValue();
        auto costOfParticulateMatterFormation =  ParticulateMatterFormation*ParticulateMatterFormationToCost->getCurrentValue();
        auto costOfPhotochemicalOxidant       =  PhotochemicalOxidant*PhotochemicalOxidantToCost->getCurrentValue();
        auto costOfTerrestrialAcidification   =  TerrestrialAcidification*TerrestrialAcidificationToCost->getCurrentValue();
        auto costOfTerrestrialEcotoxicity     =  TerrestrialEcotoxicity*TerrestrialEcotoxicityToCost->getCurrentValue();
        auto costOfUrbanLandOccupation        =  UrbanLandOccupation*UrbanLandOccupationToCost->getCurrentValue();

        // The cost of environmental damage from material manufacturing
        auto environmentalCostEmissions = costOfAgriculturalLandOccupation + costOfClimateChange + costOfFreshwaterEcotoxicity + costOfFreshwaterEutrophication +
                costOfHumanToxicity + costOfIonisingRadiation + costOfMarineEcotoxicity + costOfMarineEutrophication + costOfOzoneDepletion + costOfParticulateMatterFormation +
                costOfPhotochemicalOxidant + costOfTerrestrialAcidification + costOfTerrestrialEcotoxicity + costOfUrbanLandOccupation;

        // Sum up the totals for the building
        totalCostOfAgriculturalLandOccupation  += costOfAgriculturalLandOccupation ;
        totalCostOfClimateChange               += costOfClimateChange;
        totalCostOfFreshwaterEcotoxicity       += costOfFreshwaterEcotoxicity;
        totalCostOfFreshwaterEutrophication    += costOfFreshwaterEutrophication;
        totalCostOfHumanToxicity               += costOfHumanToxicity;
        totalCostOfIonisingRadiation           += costOfIonisingRadiation;
        totalCostOfMarineEcotoxicity           += costOfMarineEcotoxicity;
        totalCostOfMarineEutrophication        += costOfMarineEutrophication;
        totalCostOfOzoneDepletion              += costOfOzoneDepletion;
        totalCostOfParticulateMatterFormation  += costOfParticulateMatterFormation ;
        totalCostOfPhotochemicalOxidant        += costOfPhotochemicalOxidant;
        totalCostOfTerrestrialAcidification    += costOfTerrestrialAcidification;
        totalCostOfTerrestrialEcotoxicity      += costOfTerrestrialEcotoxicity;
        totalCostOfUrbanLandOccupation         += costOfUrbanLandOccupation;

        theStatsMethod->update(it->objectName()+"CostOfAgriculturalLandOccupation",costOfAgriculturalLandOccupation);
        theStatsMethod->update(it->objectName()+"CostOfClimateChange",costOfClimateChange);
        theStatsMethod->update(it->objectName()+"CostOfFreshwaterEcotoxicity",costOfFreshwaterEcotoxicity);
        theStatsMethod->update(it->objectName()+"CostOfFreshwaterEutrophication",costOfFreshwaterEutrophication);
        theStatsMethod->update(it->objectName()+"CostOfHumanToxicity",costOfHumanToxicity);
        theStatsMethod->update(it->objectName()+"CostOfIonisingRadiation",costOfIonisingRadiation);
        theStatsMethod->update(it->objectName()+"CostOfMarineEcotoxicity",costOfMarineEcotoxicity);
        theStatsMethod->update(it->objectName()+"CostOfMarineEutrophication",costOfMarineEutrophication);
        theStatsMethod->update(it->objectName()+"CostOfOzoneDepletion",costOfOzoneDepletion);
        theStatsMethod->update(it->objectName()+"CostOfParticulateMatterFormation",costOfParticulateMatterFormation);
        theStatsMethod->update(it->objectName()+"CostOfPhotochemicalOxidant",costOfPhotochemicalOxidant);
        theStatsMethod->update(it->objectName()+"CostOfTerrestrialAcidification",costOfTerrestrialAcidification);
        theStatsMethod->update(it->objectName()+"CostOfTerrestrialEcotoxicity",costOfTerrestrialEcotoxicity);
        theStatsMethod->update(it->objectName()+"CostOfUrbanLandOccupation",costOfUrbanLandOccupation);

        // Next comes the cost of emissions from energy consumption in material transport, worker transport, building operations, etc.
        auto CO2EmissionsWorkerTransport       = it->getCurrentValue("CO2EmissionsWorkerTransport");
        auto CO2EmissionsMaterialTransport     = it->getCurrentValue("CO2EmissionsMaterialTransport");
        auto CO2EmissionsEquipment             = it->getCurrentValue("CO2EmissionsEquipment");
        auto CO2EmissionsBuildOpsElectricity   = it->getCurrentValue("CO2EmissionsBuildOpsElectricity");
        auto CO2EmissionsBuildOpsNaturalGas    = it->getCurrentValue("CO2EmissionsBuildOpsNaturalGas");
        auto CH4EmissionsWorkerTransport       = it->getCurrentValue("CH4EmissionsWorkerTransport");
        auto CH4EmissionsMaterialTransport     = it->getCurrentValue("CH4EmissionsMaterialTransport");
        auto CH4EmissionsEquipment             = it->getCurrentValue("CH4EmissionsEquipment");
        auto CH4EmissionsBuildingOperations    = it->getCurrentValue("CH4EmissionsBuildingOperations");
        auto N2OEmissionsWorkerTransport       = it->getCurrentValue("N2OEmissionsWorkerTransport");
        auto N2OEmissionsMaterialTransport     = it->getCurrentValue("N2OEmissionsMaterialTransport");
        auto N2OEmissionsEquipment             = it->getCurrentValue("N2OEmissionsEquipment");
        auto N2OEmissionsBuildingOperations    = it->getCurrentValue("N2OEmissionsBuildingOperations");

        // Get the cost conversion factors
        // Need to convert the cost from imperial ton to kg
        auto CO2toHumanHealthCost = theEmissionsToHumanHealthCostConversionCO2->getCurrentValue()*massConversion;
        auto CH4toHumanHealthCost = theEmissionsToHumanHealthCostConversionCH4->getCurrentValue()*massConversion;
        auto N2OtoHumanHealthCost = theEmissionsToHumanHealthCostConversionN2O->getCurrentValue()*massConversion;
        auto CO2toEnvironmentalCost = theEmissionsToEnvironmentalCostConversionCO2->getCurrentValue();
        auto CH4toEnvironmentalCost = theEmissionsToEnvironmentalCostConversionCH4->getCurrentValue();
        auto N2OtoEnvironmentalCost = theEmissionsToEnvironmentalCostConversionN2O->getCurrentValue();

        // Human health cost of emissions from energy consumption
        auto humanHealthCostCO2EmissionsWorkerTransport      = CO2EmissionsWorkerTransport*CO2toHumanHealthCost;
        auto humanHealthCostCO2EmissionsMaterialTransport    = CO2EmissionsMaterialTransport*CO2toHumanHealthCost;
        auto humanHealthCostCO2EmissionsEquipment            = CO2EmissionsEquipment*CO2toHumanHealthCost;
        auto humanHealthCostCO2EmissionsBuildOpsElectricity  = CO2EmissionsBuildOpsElectricity*CO2toHumanHealthCost;
        auto humanHealthCostCO2EmissionsBuildOpsNaturalGas   = CO2EmissionsBuildOpsNaturalGas*CO2toHumanHealthCost;
        auto humanHealthCostCH4EmissionsWorkerTransport      = CH4EmissionsWorkerTransport*CH4toHumanHealthCost;
        auto humanHealthCostCH4EmissionsMaterialTransport    = CH4EmissionsMaterialTransport*CH4toHumanHealthCost;
        auto humanHealthCostCH4EmissionsEquipment            = CH4EmissionsEquipment*CH4toHumanHealthCost;
        auto humanHealthCostCH4EmissionsBuildingOperations   = CH4EmissionsBuildingOperations*CH4toHumanHealthCost;
        auto humanHealthCostN2OEmissionsWorkerTransport      = N2OEmissionsWorkerTransport*N2OtoHumanHealthCost;
        auto humanHealthCostN2OEmissionsMaterialTransport    = N2OEmissionsMaterialTransport*N2OtoHumanHealthCost;
        auto humanHealthCostN2OEmissionsEquipment            = N2OEmissionsEquipment*N2OtoHumanHealthCost;
        auto humanHealthCostN2OEmissionsBuildingOperations   = N2OEmissionsBuildingOperations*N2OtoHumanHealthCost;

        humanHealthCostEmissions += humanHealthCostCO2EmissionsWorkerTransport + humanHealthCostCO2EmissionsMaterialTransport + humanHealthCostCO2EmissionsEquipment + humanHealthCostCO2EmissionsBuildOpsElectricity +
                humanHealthCostCO2EmissionsBuildOpsNaturalGas + humanHealthCostCH4EmissionsWorkerTransport + humanHealthCostCH4EmissionsMaterialTransport + humanHealthCostCH4EmissionsEquipment +
                humanHealthCostCH4EmissionsBuildingOperations + humanHealthCostN2OEmissionsWorkerTransport + humanHealthCostN2OEmissionsMaterialTransport + humanHealthCostN2OEmissionsEquipment + humanHealthCostN2OEmissionsBuildingOperations;

        // Environmental cost of emissions from energy consumption
        auto environmentalCostCO2EmissionsWorkerTransport      = CO2EmissionsWorkerTransport*CO2toEnvironmentalCost;
        auto environmentalCostCO2EmissionsMaterialTransport    = CO2EmissionsMaterialTransport*CO2toEnvironmentalCost;
        auto environmentalCostCO2EmissionsEquipment            = CO2EmissionsEquipment*CO2toEnvironmentalCost;
        auto environmentalCostCO2EmissionsBuildOpsElectricity  = CO2EmissionsBuildOpsElectricity*CO2toEnvironmentalCost;
        auto environmentalCostCO2EmissionsBuildOpsNaturalGas   = CO2EmissionsBuildOpsNaturalGas*CO2toEnvironmentalCost;
        auto environmentalCostCH4EmissionsWorkerTransport      = CH4EmissionsWorkerTransport*CH4toEnvironmentalCost;
        auto environmentalCostCH4EmissionsMaterialTransport    = CH4EmissionsMaterialTransport*CH4toEnvironmentalCost;
        auto environmentalCostCH4EmissionsEquipment            = CH4EmissionsEquipment*CH4toEnvironmentalCost;
        auto environmentalCostCH4EmissionsBuildingOperations   = CH4EmissionsBuildingOperations*CH4toEnvironmentalCost;
        auto environmentalCostN2OEmissionsWorkerTransport      = N2OEmissionsWorkerTransport*N2OtoEnvironmentalCost;
        auto environmentalCostN2OEmissionsMaterialTransport    = N2OEmissionsMaterialTransport*N2OtoEnvironmentalCost;
        auto environmentalCostN2OEmissionsEquipment            = N2OEmissionsEquipment*N2OtoEnvironmentalCost;
        auto environmentalCostN2OEmissionsBuildingOperations   = N2OEmissionsBuildingOperations*N2OtoEnvironmentalCost;

        environmentalCostEmissions += environmentalCostCO2EmissionsWorkerTransport + environmentalCostCO2EmissionsMaterialTransport + environmentalCostCO2EmissionsEquipment + environmentalCostCO2EmissionsBuildOpsElectricity +
                environmentalCostCO2EmissionsBuildOpsNaturalGas + environmentalCostCH4EmissionsWorkerTransport + environmentalCostCH4EmissionsMaterialTransport + environmentalCostCH4EmissionsEquipment +
                environmentalCostCH4EmissionsBuildingOperations + environmentalCostN2OEmissionsWorkerTransport + environmentalCostN2OEmissionsMaterialTransport + environmentalCostN2OEmissionsEquipment + environmentalCostN2OEmissionsBuildingOperations;

        // Total emissions cost is the sum of human health and environmental
        auto costCO2EmissionsWorkerTransport      = humanHealthCostCO2EmissionsWorkerTransport    + environmentalCostCO2EmissionsWorkerTransport;
        auto costCO2EmissionsMaterialTransport    = humanHealthCostCO2EmissionsMaterialTransport  + environmentalCostCO2EmissionsMaterialTransport;
        auto costCO2EmissionsEquipment            = humanHealthCostCO2EmissionsEquipment          + environmentalCostCO2EmissionsEquipment;
        auto costCO2EmissionsBuildOpsElectricity  = humanHealthCostCO2EmissionsBuildOpsElectricity+ environmentalCostCO2EmissionsBuildOpsElectricity;
        auto costCO2EmissionsBuildOpsNaturalGas   = humanHealthCostCO2EmissionsBuildOpsNaturalGas + environmentalCostCO2EmissionsBuildOpsNaturalGas;
        auto costCH4EmissionsWorkerTransport      = humanHealthCostCH4EmissionsWorkerTransport    + environmentalCostCH4EmissionsWorkerTransport;
        auto costCH4EmissionsMaterialTransport    = humanHealthCostCH4EmissionsMaterialTransport  + environmentalCostCH4EmissionsMaterialTransport;
        auto costCH4EmissionsEquipment            = humanHealthCostCH4EmissionsEquipment          + environmentalCostCH4EmissionsEquipment;
        auto costCH4EmissionsBuildingOperations   = humanHealthCostCH4EmissionsBuildingOperations + environmentalCostCH4EmissionsBuildingOperations;
        auto costN2OEmissionsWorkerTransport      = humanHealthCostN2OEmissionsWorkerTransport    + environmentalCostN2OEmissionsWorkerTransport;
        auto costN2OEmissionsMaterialTransport    = humanHealthCostN2OEmissionsMaterialTransport  + environmentalCostN2OEmissionsMaterialTransport;
        auto costN2OEmissionsEquipment            = humanHealthCostN2OEmissionsEquipment          + environmentalCostN2OEmissionsEquipment;
        auto costN2OEmissionsBuildingOperations   = humanHealthCostN2OEmissionsBuildingOperations + environmentalCostN2OEmissionsBuildingOperations ;

        totalCostCO2EmissionsWorkerTransport     +=  costCO2EmissionsWorkerTransport;
        totalCostCO2EmissionsMaterialTransport   +=  costCO2EmissionsMaterialTransport;
        totalCostCO2EmissionsEquipment           +=  costCO2EmissionsEquipment;
        totalCostCO2EmissionsBuildOpsElectricity +=  costCO2EmissionsBuildOpsElectricity;
        totalCostCO2EmissionsBuildOpsNaturalGas  +=  costCO2EmissionsBuildOpsNaturalGas;
        totalCostCH4EmissionsWorkerTransport     +=  costCH4EmissionsWorkerTransport;
        totalCostCH4EmissionsMaterialTransport   +=  costCH4EmissionsMaterialTransport;
        totalCostCH4EmissionsEquipment           +=  costCH4EmissionsEquipment;
        totalCostCH4EmissionsBuildingOperations  +=  costCH4EmissionsBuildingOperations;
        totalCostN2OEmissionsWorkerTransport     +=  costN2OEmissionsWorkerTransport;
        totalCostN2OEmissionsMaterialTransport   +=  costN2OEmissionsMaterialTransport;
        totalCostN2OEmissionsEquipment           +=  costN2OEmissionsEquipment;
        totalCostN2OEmissionsBuildingOperations  +=  costN2OEmissionsBuildingOperations;

        theStatsMethod->update(it->objectName()+"CostCO2EmissionsWorkerTransport",costCO2EmissionsWorkerTransport);
        theStatsMethod->update(it->objectName()+"CostCO2EmissionsMaterialTransport",costCO2EmissionsMaterialTransport);
        theStatsMethod->update(it->objectName()+"CostCO2EmissionsEquipment",costCO2EmissionsEquipment);
        theStatsMethod->update(it->objectName()+"CostCO2EmissionsBuildOpsElectricity",costCO2EmissionsBuildOpsElectricity);
        theStatsMethod->update(it->objectName()+"CostCO2EmissionsBuildOpsNaturalGas",costCO2EmissionsBuildOpsNaturalGas);
        theStatsMethod->update(it->objectName()+"CostCH4EmissionsWorkerTransport",costCH4EmissionsWorkerTransport);
        theStatsMethod->update(it->objectName()+"CostCH4EmissionsMaterialTransport",costCH4EmissionsMaterialTransport);
        theStatsMethod->update(it->objectName()+"CostCH4EmissionsEquipment",costCH4EmissionsEquipment);
        theStatsMethod->update(it->objectName()+"CostCH4EmissionsBuildingOperations",costCH4EmissionsBuildingOperations);
        theStatsMethod->update(it->objectName()+"CostN2OEmissionsWorkerTransport",costN2OEmissionsWorkerTransport);
        theStatsMethod->update(it->objectName()+"CostN2OEmissionsMaterialTransport",costN2OEmissionsMaterialTransport);
        theStatsMethod->update(it->objectName()+"CostN2OEmissionsEquipment",costN2OEmissionsEquipment);
        theStatsMethod->update(it->objectName()+"CostN2OEmissionsBuildingOperations",costN2OEmissionsBuildingOperations);

        theStatsMethod->update(it->objectName()+"HumanHealthCostEmissions",humanHealthCostEmissions);
        theStatsMethod->update(it->objectName()+"EnvironmentalCostEmissions",environmentalCostEmissions);

        // Accumulate the cost of the emissions natural gas in building operations
        theStatsMethod->accumulate(it->objectName()+"CostCO2EmissionsBuildOpsNaturalGas",costCO2EmissionsBuildOpsNaturalGas);
        theStatsMethod->accumulate(it->objectName()+"CostCH4EmissionsBuildingOperations",costCH4EmissionsBuildingOperations);
        theStatsMethod->accumulate(it->objectName()+"CostN2OEmissionsBuildingOperations",costN2OEmissionsBuildingOperations);

        // Accumulate the cost of emissions for electricity in building operations
        theStatsMethod->accumulate(it->objectName()+"CostCO2EmissionsBuildOpsElectricity",costCO2EmissionsBuildOpsElectricity);

        // Accumulate the cost of the emissions from energy usage in transport and equipment use
        theStatsMethod->accumulate(it->objectName()+"CostCO2EmissionsWorkerTransport",costCO2EmissionsWorkerTransport);
        theStatsMethod->accumulate(it->objectName()+"CostCO2EmissionsMaterialTransport",costCO2EmissionsMaterialTransport);
        theStatsMethod->accumulate(it->objectName()+"CostCO2EmissionsEquipment",costCO2EmissionsEquipment);
        theStatsMethod->accumulate(it->objectName()+"CostCH4EmissionsWorkerTransport",costCH4EmissionsWorkerTransport);
        theStatsMethod->accumulate(it->objectName()+"CostCH4EmissionsMaterialTransport",costCH4EmissionsMaterialTransport);
        theStatsMethod->accumulate(it->objectName()+"CostCH4EmissionsEquipment",costCH4EmissionsEquipment);
        theStatsMethod->accumulate(it->objectName()+"CostN2OEmissionsWorkerTransport",costN2OEmissionsWorkerTransport);
        theStatsMethod->accumulate(it->objectName()+"CostN2OEmissionsMaterialTransport",costN2OEmissionsMaterialTransport);
        theStatsMethod->accumulate(it->objectName()+"CostN2OEmissionsEquipment",costN2OEmissionsEquipment);

        CostCarbonDioxideEmissions  += costCO2EmissionsWorkerTransport + costCO2EmissionsMaterialTransport + costCO2EmissionsEquipment   + costCO2EmissionsBuildOpsElectricity + costCO2EmissionsBuildOpsNaturalGas;
        CostMethaneEmissions        += costCH4EmissionsWorkerTransport  + costCH4EmissionsMaterialTransport  + costCH4EmissionsEquipment  + costCH4EmissionsBuildingOperations;
        CostNitrousOxideEmissions   += costN2OEmissionsWorkerTransport + costN2OEmissionsMaterialTransport + costN2OEmissionsEquipment  + costN2OEmissionsBuildingOperations;

        totalCostCarbonDioxideEmissions    += CostCarbonDioxideEmissions ;
        totalCostMethaneEmissions          += CostMethaneEmissions;
        totalCostNitrousOxideEmissions     += CostNitrousOxideEmissions;
        totalCostHFC134aEmissions          += CostHFC134aEmissions;
        totalCostBlackCarbonPMEmissions    += CostBlackCarbonPMEmissions;
        totalCostSulfurDioxideEmissions    += CostSulfurDioxideEmissions;
        totalCostCarbonMonoxideEmissions   += CostCarbonMonoxideEmissions;
        totalCostOrganicCarbonEmissions    += CostOrganicCarbonEmissions;
        totalCostNitrogenOxidesEmissions   += CostNitrogenOxidesEmissions;
        totalCostAmmoniaEmissions          += CostAmmoniaEmissions;

        totalHumanHealthCostEmissions += humanHealthCostEmissions;
        totalEnvironmentalCostEmissions += environmentalCostEmissions;

        auto costOfEmissionsAtThisStage = 0.0;

        costOfEmissionsAtThisStage += CostCarbonDioxideEmissions;
        costOfEmissionsAtThisStage += CostMethaneEmissions;
        costOfEmissionsAtThisStage += CostNitrousOxideEmissions;
        costOfEmissionsAtThisStage += CostHFC134aEmissions;
        costOfEmissionsAtThisStage += CostBlackCarbonPMEmissions;
        costOfEmissionsAtThisStage += CostSulfurDioxideEmissions;
        costOfEmissionsAtThisStage += CostCarbonMonoxideEmissions;
        costOfEmissionsAtThisStage += CostOrganicCarbonEmissions;
        costOfEmissionsAtThisStage += CostNitrogenOxidesEmissions;
        costOfEmissionsAtThisStage += CostAmmoniaEmissions;

        costOfEmissionsAtThisStage += costOfAgriculturalLandOccupation;
        costOfEmissionsAtThisStage += costOfClimateChange;
        costOfEmissionsAtThisStage += costOfFreshwaterEcotoxicity;
        costOfEmissionsAtThisStage += costOfFreshwaterEutrophication;
        costOfEmissionsAtThisStage += costOfHumanToxicity;
        costOfEmissionsAtThisStage += costOfIonisingRadiation;
        costOfEmissionsAtThisStage += costOfMarineEcotoxicity;
        costOfEmissionsAtThisStage += costOfMarineEutrophication;
        costOfEmissionsAtThisStage += costOfOzoneDepletion;
        costOfEmissionsAtThisStage += costOfParticulateMatterFormation;
        costOfEmissionsAtThisStage += costOfPhotochemicalOxidant;
        costOfEmissionsAtThisStage += costOfTerrestrialAcidification;
        costOfEmissionsAtThisStage += costOfTerrestrialEcotoxicity;
        costOfEmissionsAtThisStage += costOfUrbanLandOccupation;

        if (theOutputLevel >= RObject::Maximum)
        {
            qDebug()<<"For the response "<<it->objectName();
            qDebug()<<"The cost of Carbon Dioxide Emissions" <<CostCarbonDioxideEmissions;
            qDebug()<<"The cost of Methane Emissions"        <<CostMethaneEmissions;
            qDebug()<<"The cost of Nitrous Oxide Emissions"  <<CostNitrousOxideEmissions;
            qDebug()<<"The cost of HFC134a Emissions"        <<CostHFC134aEmissions;
            qDebug()<<"The cost of Black Carbon PM Emissions"<<CostBlackCarbonPMEmissions;
            qDebug()<<"The cost of Sulfur Dioxide Emissions" <<CostSulfurDioxideEmissions;
            qDebug()<<"The cost of Carbon Monoxide Emissions"<<CostCarbonMonoxideEmissions;
            qDebug()<<"The cost of Organic Carbon Emissions" <<CostOrganicCarbonEmissions;
            qDebug()<<"The cost of Nitrogen Oxides Emissions"<<CostNitrogenOxidesEmissions;
            qDebug()<<"The cost of Ammonia Emissions"        <<CostAmmoniaEmissions;

            qDebug()<<"The cost of Agricultural land occupation" <<costOfAgriculturalLandOccupation;
            qDebug()<<"The cost of Climate change" <<costOfClimateChange;
            qDebug()<<"The cost of Freshwater ecotoxicity" <<costOfFreshwaterEcotoxicity;
            qDebug()<<"The cost of Freshwater eutrophication" <<costOfFreshwaterEutrophication;
            qDebug()<<"The cost of Human toxicity" <<costOfHumanToxicity;
            qDebug()<<"The cost of Ionising radiation" <<costOfIonisingRadiation;
            qDebug()<<"The cost of Marine ecotoxicity" <<costOfMarineEcotoxicity;
            qDebug()<<"The cost of Marine eutrophication" <<costOfMarineEutrophication;
            qDebug()<<"The cost of Ozone depletion" <<costOfOzoneDepletion;
            qDebug()<<"The cost of Particulate matter formation" <<costOfParticulateMatterFormation;
            qDebug()<<"The cost of Photochemical oxidant " <<costOfPhotochemicalOxidant;
            qDebug()<<"The cost of Terrestrial acidification" <<costOfTerrestrialAcidification;
            qDebug()<<"The cost of Terrestrial ecotoxicity" <<costOfTerrestrialEcotoxicity;
            qDebug()<<"The cost of Urban land occupation" <<costOfUrbanLandOccupation;
        }

        totalCostOfEmissions += costOfEmissionsAtThisStage;

        theStatsMethod->update(it->objectName()+"costOfEmissionsAtThisStage",costOfEmissionsAtThisStage);
    }

    // Set the values to the response objects
    theResponse->setCurrentValue(totalCostOfEmissions);
    theCostOfDamageToHumanHealthResponse->setCurrentValue(totalHumanHealthCostEmissions);
    theCostOfEnvironmentalDamageResponse->setCurrentValue(totalEnvironmentalCostEmissions);

    // Update the statistics
    theStatsMethod->update(theResponse->objectName()+"TotalCostCarbonDioxideEmissions",totalCostCarbonDioxideEmissions);
    theStatsMethod->update(theResponse->objectName()+"TotalCostMethaneEmissions",totalCostMethaneEmissions);
    theStatsMethod->update(theResponse->objectName()+"TotalCostNitrousOxideEmissions",totalCostNitrousOxideEmissions);
    theStatsMethod->update(theResponse->objectName()+"TotalCostHFC134aEmissions",totalCostHFC134aEmissions);
    theStatsMethod->update(theResponse->objectName()+"TotalCostBlackCarbonPMEmissions",totalCostBlackCarbonPMEmissions);
    theStatsMethod->update(theResponse->objectName()+"TotalCostSulfurDioxideEmissions",totalCostSulfurDioxideEmissions);
    theStatsMethod->update(theResponse->objectName()+"TotalCostCarbonMonoxideEmissions",totalCostCarbonMonoxideEmissions);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOrganicCarbonEmissions",totalCostOrganicCarbonEmissions);
    theStatsMethod->update(theResponse->objectName()+"TotalCostNitrogenOxidesEmissions",totalCostNitrogenOxidesEmissions);
    theStatsMethod->update(theResponse->objectName()+"TotalCostAmmoniaEmissions",totalCostAmmoniaEmissions);

    theStatsMethod->update(theResponse->objectName()+"TotalCostCO2EmissionsWorkerTransport",totalCostCO2EmissionsWorkerTransport);
    theStatsMethod->update(theResponse->objectName()+"TotalCostCO2EmissionsMaterialTransport",totalCostCO2EmissionsMaterialTransport);
    theStatsMethod->update(theResponse->objectName()+"TotalCostCO2EmissionsEquipment",totalCostCO2EmissionsEquipment);
    theStatsMethod->update(theResponse->objectName()+"TotalCostCO2EmissionsBuildOpsElectricity",totalCostCO2EmissionsBuildOpsElectricity);
    theStatsMethod->update(theResponse->objectName()+"TotalCostCO2EmissionsBuildOpsNaturalGas",totalCostCO2EmissionsBuildOpsNaturalGas);
    theStatsMethod->update(theResponse->objectName()+"TotalCostCH4EmissionsWorkerTransport",totalCostCH4EmissionsWorkerTransport);
    theStatsMethod->update(theResponse->objectName()+"TotalCostCH4EmissionsMaterialTransport",totalCostCH4EmissionsMaterialTransport);
    theStatsMethod->update(theResponse->objectName()+"TotalCostCH4EmissionsEquipment",totalCostCH4EmissionsEquipment);
    theStatsMethod->update(theResponse->objectName()+"TotalCostCH4EmissionsBuildingOperations",totalCostCH4EmissionsBuildingOperations);
    theStatsMethod->update(theResponse->objectName()+"TotalCostN2OEmissionsWorkerTransport",totalCostN2OEmissionsWorkerTransport);
    theStatsMethod->update(theResponse->objectName()+"TotalCostN2OEmissionsMaterialTransport",totalCostN2OEmissionsMaterialTransport);
    theStatsMethod->update(theResponse->objectName()+"TotalCostN2OEmissionsEquipment",totalCostN2OEmissionsEquipment);
    theStatsMethod->update(theResponse->objectName()+"TotalCostN2OEmissionsBuildingOperations",totalCostN2OEmissionsBuildingOperations);

    theStatsMethod->update(theResponse->objectName()+"TotalCostOfAgriculturalLandOccupation",totalCostOfAgriculturalLandOccupation);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfClimateChange",totalCostOfClimateChange);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfFreshwaterEcotoxicity",totalCostOfFreshwaterEcotoxicity);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfFreshwaterEutrophication",totalCostOfFreshwaterEutrophication);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfHumanToxicity",totalCostOfHumanToxicity);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfIonisingRadiation",totalCostOfIonisingRadiation);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfMarineEcotoxicity",totalCostOfMarineEcotoxicity);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfMarineEutrophication",totalCostOfMarineEutrophication);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfOzoneDepletion",totalCostOfOzoneDepletion);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfParticulateMatterFormation",totalCostOfParticulateMatterFormation);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfPhotochemicalOxidant",totalCostOfPhotochemicalOxidant);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfTerrestrialAcidification",totalCostOfTerrestrialAcidification);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfTerrestrialEcotoxicity",totalCostOfTerrestrialEcotoxicity);
    theStatsMethod->update(theResponse->objectName()+"TotalCostOfUrbanLandOccupation",totalCostOfUrbanLandOccupation);

    theStatsMethod->update(theResponse->objectName()+"totalHumanHealthCostEmissions",totalHumanHealthCostEmissions);
    theStatsMethod->update(theResponse->objectName()+"totalEnvironmentalCostEmissions",totalEnvironmentalCostEmissions);

    // Output the emissions quantities
    if (theOutputLevel >= RObject::Maximum)
    {
        qDebug()<<"For the response "<<theResponse->objectName();
        qDebug()<<"The total cost of Carbon Dioxide Emissions" <<totalCostCarbonDioxideEmissions;
        qDebug()<<"The total cost of Methane Emissions"        <<totalCostMethaneEmissions;
        qDebug()<<"The total cost of Nitrous Oxide Emissions"  <<totalCostNitrousOxideEmissions;
        qDebug()<<"The total cost of HFC134a Emissions"        <<totalCostHFC134aEmissions;
        qDebug()<<"The total cost of Black Carbon PM Emissions"<<totalCostBlackCarbonPMEmissions;
        qDebug()<<"The total cost of Sulfur Dioxide Emissions" <<totalCostSulfurDioxideEmissions;
        qDebug()<<"The total cost of Carbon Monoxide Emissions"<<totalCostCarbonMonoxideEmissions;
        qDebug()<<"The total cost of Organic Carbon Emissions" <<totalCostOrganicCarbonEmissions;
        qDebug()<<"The total cost of Nitrogen Oxides Emissions"<<totalCostNitrogenOxidesEmissions;
        qDebug()<<"The total cost of Ammonia Emissions"        <<totalCostAmmoniaEmissions;

        qDebug()<<"The total cost of Agricultural land occupation"<<totalCostOfAgriculturalLandOccupation;
        qDebug()<<"The total cost of Climate change"              <<totalCostOfClimateChange;
        qDebug()<<"The total cost of Freshwater ecotoxicity"      <<totalCostOfFreshwaterEcotoxicity;
        qDebug()<<"The total cost of Freshwater eutrophication"   <<totalCostOfFreshwaterEutrophication;
        qDebug()<<"The total cost of Human toxicity"              <<totalCostOfHumanToxicity;
        qDebug()<<"The total cost of Ionising radiation"          <<totalCostOfIonisingRadiation;
        qDebug()<<"The total cost of Marine ecotoxicity"          <<totalCostOfMarineEcotoxicity;
        qDebug()<<"The total cost of Marine eutrophication"       <<totalCostOfMarineEutrophication;
        qDebug()<<"The total cost of Ozone depletion"             <<totalCostOfOzoneDepletion;
        qDebug()<<"The total cost of Particulate matter formation"<<totalCostOfParticulateMatterFormation;
        qDebug()<<"The total cost of Photochemical oxidant "      <<totalCostOfPhotochemicalOxidant;
        qDebug()<<"The total cost of Terrestrial acidification"   <<totalCostOfTerrestrialAcidification;
        qDebug()<<"The total cost of Terrestrial ecotoxicity"     <<totalCostOfTerrestrialEcotoxicity;
        qDebug()<<"The total cost of Urban land occupation"       <<totalCostOfUrbanLandOccupation;

        qDebug()<<"The total cost of all emissions is:"<<totalCostOfEmissions;
    }

    theStatsMethod->update(theResponse->objectName()+"TotalCost",totalCostOfEmissions);

    return 0;
}


void RCostOfEmissionsModel::createHumanHealthCostRandomVariables(void)
{
    auto EmissionsToCostConversionCO2       =  0.0;
    auto EmissionsToCostConversionCH4       =  0.0;
    auto EmissionsToCostConversionN2O       =  0.0;
    auto EmissionsToCostConversionHFC_134a  =  0.0;
    auto EmissionsToCostConversionBC        =  0.0;
    auto EmissionsToCostConversionSO2       =  0.0;
    auto EmissionsToCostConversionCO        =  0.0;
    auto EmissionsToCostConversionOC        =  0.0;
    auto EmissionsToCostConversionNOx       =  0.0;
    auto EmissionsToCostConversionNH3       =  0.0;

    // Create a few random variables
    theDomain->createObject(objectName() + "EmissionsToCostConversionCO2", "RContinuousRandomVariable");
    theEmissionsToHumanHealthCostConversionCO2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToHumanHealthCostConversionCO2->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToCostConversionCH4", "RContinuousRandomVariable");
    theEmissionsToHumanHealthCostConversionCH4 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToHumanHealthCostConversionCH4->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToCostConversionN2O", "RContinuousRandomVariable");
    theEmissionsToHumanHealthCostConversionN2O = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToHumanHealthCostConversionN2O->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToCostConversionHFC_134a", "RContinuousRandomVariable");
    theEmissionsToHumanHealthCostConversionHFC_134a = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToHumanHealthCostConversionHFC_134a->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToCostConversionBC", "RContinuousRandomVariable");
    theEmissionsToHumanHealthCostConversionBC = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToHumanHealthCostConversionBC->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToCostConversionSO2", "RContinuousRandomVariable");
    theEmissionsToHumanHealthCostConversionSO2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToHumanHealthCostConversionSO2->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToCostConversionCO", "RContinuousRandomVariable");
    theEmissionsToHumanHealthCostConversionCO = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToHumanHealthCostConversionCO->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToCostConversionOC", "RContinuousRandomVariable");
    theEmissionsToHumanHealthCostConversionOC = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToHumanHealthCostConversionOC->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToCostConversionNOx", "RContinuousRandomVariable");
    theEmissionsToHumanHealthCostConversionNOx = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToHumanHealthCostConversionNOx->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToCostConversionNH3", "RContinuousRandomVariable");
    theEmissionsToHumanHealthCostConversionNH3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToHumanHealthCostConversionNH3->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Convert from 2007-USD to 2018 CAD; from us ton to tonne
    // $1 in 2007 → $1.22 in 2018
    // 1 US ton = 0.907184999999146 tonne
    auto conversionFactor = 1.22/0.907184999999146;

    // Using The social cost of atmospheric release (Shindell, 2015) (damages per ton in $2007 US)
    // only considers human health impacts
    // these values take into account:
    // global climate: IAM-based climate-health impacts
    // regional climate: changes in precipitation
    // additional climate-health: based on WHO analyses as described in the section 2.2 Composition-health
    if(discountRate == "1.4%")
    {
        //                         CO2     CH4     N2O     HFC-134a    BC          SO2         CO      OC      NOx     NH3
        // Median total; 1.4 %     150     6000    62,000  210,000     310,000     43,000      820     71,000  67,000  25,000

        EmissionsToCostConversionCO2       += 150.0*conversionFactor;
        EmissionsToCostConversionCH4       += 6000.0*conversionFactor;
        EmissionsToCostConversionN2O       += 62000.0*conversionFactor;
        EmissionsToCostConversionHFC_134a  += 210000.0*conversionFactor;
        EmissionsToCostConversionBC        += 310000.0*conversionFactor;
        EmissionsToCostConversionSO2       += 43000.0*conversionFactor;
        EmissionsToCostConversionCO        += 820.0*conversionFactor;
        EmissionsToCostConversionOC        += 71000.0*conversionFactor;
        EmissionsToCostConversionNOx       += 67000.0*conversionFactor;
        EmissionsToCostConversionNH3       += 25000.0*conversionFactor;

    }
    else if(discountRate == "3%")
    {
        //                         CO2     CH4     N2O     HFC-134a    BC          SO2         CO      OC      NOx     NH3
        // Median total; 3 %       84      4600    37,000  160,000     270,000     42,000      630     68,000  67,000  25,000
        EmissionsToCostConversionCO2       += 84.0*conversionFactor;
        EmissionsToCostConversionCH4       += 4600.0*conversionFactor;
        EmissionsToCostConversionN2O       += 37000.0*conversionFactor;
        EmissionsToCostConversionHFC_134a  += 160000.0*conversionFactor;
        EmissionsToCostConversionBC        += 270000.0*conversionFactor;
        EmissionsToCostConversionSO2       += 42000.0*conversionFactor;
        EmissionsToCostConversionCO        += 630.0*conversionFactor;
        EmissionsToCostConversionOC        += 68000.0*conversionFactor;
        EmissionsToCostConversionNOx       += 67000.0*conversionFactor;
        EmissionsToCostConversionNH3       += 25000.0*conversionFactor;
    }
    else if(discountRate == "5%")
    {
        //                         CO2     CH4     N2O     HFC-134a    BC          SO2         CO      OC      NOx     NH3
        // Median total; 5 %       27      2700    12,000  85,000      210,000     40,000      410     64,000  67,000  24,000
        EmissionsToCostConversionCO2       += 27.0*conversionFactor;
        EmissionsToCostConversionCH4       += 2700.0*conversionFactor;
        EmissionsToCostConversionN2O       += 12000.0*conversionFactor;
        EmissionsToCostConversionHFC_134a  += 85000.0*conversionFactor;
        EmissionsToCostConversionBC        += 210000.0*conversionFactor;
        EmissionsToCostConversionSO2       += 40000.0*conversionFactor;
        EmissionsToCostConversionCO        += 410.0*conversionFactor;
        EmissionsToCostConversionOC        += 64000.0*conversionFactor;
        EmissionsToCostConversionNOx       += 67000.0*conversionFactor;
        EmissionsToCostConversionNH3       += 24000.0*conversionFactor;
    }
    else if(discountRate == "declining rate")
    {
        //                               CO2     CH4     N2O     HFC-134a    BC          SO2         CO      OC      NOx     NH3
        // Median total; declining rate  110     4700    47,000  160,000     280,000     42,000      730     69,000  67,000  25,000
        EmissionsToCostConversionCO2       += 110.0*conversionFactor;
        EmissionsToCostConversionCH4       += 4700.0*conversionFactor;
        EmissionsToCostConversionN2O       += 47000.0*conversionFactor;
        EmissionsToCostConversionHFC_134a  += 160000.0*conversionFactor;
        EmissionsToCostConversionBC        += 280000.0*conversionFactor;
        EmissionsToCostConversionSO2       += 42000.0*conversionFactor;
        EmissionsToCostConversionCO        += 730.0*conversionFactor;
        EmissionsToCostConversionOC        += 69000.0*conversionFactor;
        EmissionsToCostConversionNOx       += 67000.0*conversionFactor;
        EmissionsToCostConversionNH3       += 25000.0*conversionFactor;
    }
    else
    {
        qCritical()<<"Error: the discount rate of "<<discountRate<<" is not supported in "<<__FUNCTION__<<" only 1.4%, 3%, 5% and declining rates are supported";
    }

    // Set the final costs to the RVs
    QString cov = "0.3";

    // Set the mean value
    theEmissionsToHumanHealthCostConversionCO2->setMeanValue(EmissionsToCostConversionCO2);
    theEmissionsToHumanHealthCostConversionCH4->setMeanValue(EmissionsToCostConversionCH4);
    theEmissionsToHumanHealthCostConversionN2O->setMeanValue(EmissionsToCostConversionN2O);
    theEmissionsToHumanHealthCostConversionHFC_134a->setMeanValue(EmissionsToCostConversionHFC_134a);
    theEmissionsToHumanHealthCostConversionBC->setMeanValue(EmissionsToCostConversionBC);
    theEmissionsToHumanHealthCostConversionSO2->setMeanValue(EmissionsToCostConversionSO2);
    theEmissionsToHumanHealthCostConversionCO->setMeanValue(EmissionsToCostConversionCO);
    theEmissionsToHumanHealthCostConversionOC->setMeanValue(EmissionsToCostConversionOC);
    theEmissionsToHumanHealthCostConversionNOx->setMeanValue(EmissionsToCostConversionNOx);
    theEmissionsToHumanHealthCostConversionNH3->setMeanValue(EmissionsToCostConversionNH3);

    // Set the current value
    theEmissionsToHumanHealthCostConversionCO2->setCurrentValue(EmissionsToCostConversionCO2);
    theEmissionsToHumanHealthCostConversionCH4->setCurrentValue(EmissionsToCostConversionCH4);
    theEmissionsToHumanHealthCostConversionN2O->setCurrentValue(EmissionsToCostConversionN2O);
    theEmissionsToHumanHealthCostConversionHFC_134a->setCurrentValue(EmissionsToCostConversionHFC_134a);
    theEmissionsToHumanHealthCostConversionBC->setCurrentValue(EmissionsToCostConversionBC);
    theEmissionsToHumanHealthCostConversionSO2->setCurrentValue(EmissionsToCostConversionSO2);
    theEmissionsToHumanHealthCostConversionCO->setCurrentValue(EmissionsToCostConversionCO);
    theEmissionsToHumanHealthCostConversionOC->setCurrentValue(EmissionsToCostConversionOC);
    theEmissionsToHumanHealthCostConversionNOx->setCurrentValue(EmissionsToCostConversionNOx);
    theEmissionsToHumanHealthCostConversionNH3->setCurrentValue(EmissionsToCostConversionNH3);

    // Set the COV
    theEmissionsToHumanHealthCostConversionCO2->setCoefficientOfVariation(cov);
    theEmissionsToHumanHealthCostConversionCH4->setCoefficientOfVariation(cov);
    theEmissionsToHumanHealthCostConversionN2O->setCoefficientOfVariation(cov);
    theEmissionsToHumanHealthCostConversionHFC_134a->setCoefficientOfVariation(cov);
    theEmissionsToHumanHealthCostConversionBC->setCoefficientOfVariation(cov);
    theEmissionsToHumanHealthCostConversionSO2->setCoefficientOfVariation(cov);
    theEmissionsToHumanHealthCostConversionCO->setCoefficientOfVariation(cov);
    theEmissionsToHumanHealthCostConversionOC->setCoefficientOfVariation(cov);
    theEmissionsToHumanHealthCostConversionNOx->setCoefficientOfVariation(cov);
    theEmissionsToHumanHealthCostConversionNH3->setCoefficientOfVariation(cov);

}


void RCostOfEmissionsModel::createEnvironmentalCostRandomVariables(void)
{

    // Create the random variables
    theDomain->createObject(objectName() + "EmissionsToEnvCostConversionCO2", "RContinuousRandomVariable");
    theEmissionsToEnvironmentalCostConversionCO2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToEnvironmentalCostConversionCO2->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToEnvCostConversionCH4", "RContinuousRandomVariable");
    theEmissionsToEnvironmentalCostConversionCH4 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToEnvironmentalCostConversionCH4->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToEnvCostConversionN2O", "RContinuousRandomVariable");
    theEmissionsToEnvironmentalCostConversionN2O = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToEnvironmentalCostConversionN2O->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToEnvCostConversionSO2", "RContinuousRandomVariable");
    theEmissionsToEnvironmentalCostConversionSO2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToEnvironmentalCostConversionSO2->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToEnvCostConversionCO", "RContinuousRandomVariable");
    theEmissionsToEnvironmentalCostConversionCO = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToEnvironmentalCostConversionCO->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToEnvCostConversionNOx", "RContinuousRandomVariable");
    theEmissionsToEnvironmentalCostConversionNOx = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToEnvironmentalCostConversionNOx->setDistributionType(QString("Lognormal (zeta, sigma)"));

    theDomain->createObject(objectName() + "EmissionsToEnvCostConversionNH3", "RContinuousRandomVariable");
    theEmissionsToEnvironmentalCostConversionNH3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEmissionsToEnvironmentalCostConversionNH3->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Environmental cost values taken from Bijleveld, M., Bruyn, S. de, Graaff, L. de, Schep, E., Schroten, A., & Vergeer, R. (2018b). Environmental prices handbook EU28 version
    // Convert from € to CAD
    // 1 € in 2007 → $1.51 CAD in 2018
    auto conversionFactor = 1.51;

    auto EmissionsToCostConversionCO2       = 0.0566*conversionFactor;
    auto EmissionsToCostConversionCH4       = 1.74*conversionFactor;
    auto EmissionsToCostConversionN2O       = 1.50*conversionFactor;
    auto EmissionsToCostConversionSO2       = 11.5*conversionFactor;
    auto EmissionsToCostConversionCO        = 0.0526*conversionFactor;
    auto EmissionsToCostConversionNOx       = 14.8*conversionFactor;
    auto EmissionsToCostConversionNH3       = 17.5*conversionFactor;

    // Set the final costs to the RVs
    QString cov = "0.3";

    // Set the mean value
    theEmissionsToEnvironmentalCostConversionCO2->setMeanValue(EmissionsToCostConversionCO2);
    theEmissionsToEnvironmentalCostConversionCH4->setMeanValue(EmissionsToCostConversionCH4);
    theEmissionsToEnvironmentalCostConversionN2O->setMeanValue(EmissionsToCostConversionN2O);
    theEmissionsToEnvironmentalCostConversionSO2->setMeanValue(EmissionsToCostConversionSO2);
    theEmissionsToEnvironmentalCostConversionCO->setMeanValue(EmissionsToCostConversionCO);
    theEmissionsToEnvironmentalCostConversionNOx->setMeanValue(EmissionsToCostConversionNOx);
    theEmissionsToEnvironmentalCostConversionNH3->setMeanValue(EmissionsToCostConversionNH3);

    // Set the current value
    theEmissionsToEnvironmentalCostConversionCO2->setCurrentValue(EmissionsToCostConversionCO2);
    theEmissionsToEnvironmentalCostConversionCH4->setCurrentValue(EmissionsToCostConversionCH4);
    theEmissionsToEnvironmentalCostConversionN2O->setCurrentValue(EmissionsToCostConversionN2O);
    theEmissionsToEnvironmentalCostConversionSO2->setCurrentValue(EmissionsToCostConversionSO2);
    theEmissionsToEnvironmentalCostConversionCO->setCurrentValue(EmissionsToCostConversionCO);
    theEmissionsToEnvironmentalCostConversionNOx->setCurrentValue(EmissionsToCostConversionNOx);
    theEmissionsToEnvironmentalCostConversionNH3->setCurrentValue(EmissionsToCostConversionNH3);

    // Set the COV
    theEmissionsToEnvironmentalCostConversionCO2->setCoefficientOfVariation(cov);
    theEmissionsToEnvironmentalCostConversionCH4->setCoefficientOfVariation(cov);
    theEmissionsToEnvironmentalCostConversionN2O->setCoefficientOfVariation(cov);
    theEmissionsToEnvironmentalCostConversionSO2->setCoefficientOfVariation(cov);
    theEmissionsToEnvironmentalCostConversionCO->setCoefficientOfVariation(cov);
    theEmissionsToEnvironmentalCostConversionNOx->setCoefficientOfVariation(cov);
    theEmissionsToEnvironmentalCostConversionNH3->setCoefficientOfVariation(cov);

}


void  RCostOfEmissionsModel::createMidpointCostRandomVariables(void)
{

    // Agricultural land occupation
    theDomain->createObject(objectName() + "AgriculturalLandOccupationToCost", "RContinuousRandomVariable");
    AgriculturalLandOccupationToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    AgriculturalLandOccupationToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Climate change
    theDomain->createObject(objectName() + "ClimateChangeToCost", "RContinuousRandomVariable");
    ClimateChangeToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    ClimateChangeToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Freshwater ecotoxicity
    theDomain->createObject(objectName() + "FreshwaterEcotoxicityToCost", "RContinuousRandomVariable");
    FreshwaterEcotoxicityToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    FreshwaterEcotoxicityToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Freshwater eutrophication
    theDomain->createObject(objectName() + "FreshwaterEutrophicationToCost", "RContinuousRandomVariable");
    FreshwaterEutrophicationToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    FreshwaterEutrophicationToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Human toxicity
    theDomain->createObject(objectName() + "HumanToxicityToCost", "RContinuousRandomVariable");
    HumanToxicityToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    HumanToxicityToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Ionising radiation
    theDomain->createObject(objectName() + "IonisingRadiationToCost", "RContinuousRandomVariable");
    IonisingRadiationToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    IonisingRadiationToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Marine ecotoxicity
    theDomain->createObject(objectName() + "MarineEcotoxicityToCost", "RContinuousRandomVariable");
    MarineEcotoxicityToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    MarineEcotoxicityToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Marine eutrophication
    theDomain->createObject(objectName() + "MarineEutrophicationToCost", "RContinuousRandomVariable");
    MarineEutrophicationToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    MarineEutrophicationToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Ozone depletion
    theDomain->createObject(objectName() + "OzoneDepletionToCost", "RContinuousRandomVariable");
    OzoneDepletionToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    OzoneDepletionToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Particulate matter formation
    theDomain->createObject(objectName() + "ParticulateMatterFormationToCost", "RContinuousRandomVariable");
    ParticulateMatterFormationToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    ParticulateMatterFormationToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Photochemical oxidant
    theDomain->createObject(objectName() + "PhotochemicalOxidantToCost", "RContinuousRandomVariable");
    PhotochemicalOxidantToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    PhotochemicalOxidantToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Terrestrial acidification
    theDomain->createObject(objectName() + "TerrestrialAcidificationToCost", "RContinuousRandomVariable");
    TerrestrialAcidificationToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    TerrestrialAcidificationToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Terrestrial ecotoxicity
    theDomain->createObject(objectName() + "TerrestrialEcotoxicityToCost", "RContinuousRandomVariable");
    TerrestrialEcotoxicityToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    TerrestrialEcotoxicityToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));

    // Urban land occupation
    theDomain->createObject(objectName() + "UrbanLandOccupationToCost", "RContinuousRandomVariable");
    UrbanLandOccupationToCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    UrbanLandOccupationToCost->setDistributionType(QString("Lognormal (zeta, sigma)"));


    // Values for the midpoint categories taken from Bijleveld, M., Bruyn, S. de, Graaff, L. de, Schep, E., Schroten, A., & Vergeer, R. (2018a). Environmental prices handbook 2017.
    // Set the final costs to the RVs
    QString cov = "0.3";

    // Convert from € to CAD
    // 1 € in 2007 → $1.51 in 2018 CAD
    auto conversionFactor = 1.51;

    // Set the mean value
    AgriculturalLandOccupationToCost->setMeanValue(0.0845*conversionFactor);
    ClimateChangeToCost->setMeanValue(0.057*conversionFactor);
    FreshwaterEcotoxicityToCost->setMeanValue(0.0361*conversionFactor);
    FreshwaterEutrophicationToCost->setMeanValue(1.86*conversionFactor);
    HumanToxicityToCost->setMeanValue(0.0991*conversionFactor);
    IonisingRadiationToCost->setMeanValue(0.0461*conversionFactor);
    MarineEcotoxicityToCost->setMeanValue(0.00739*conversionFactor);
    MarineEutrophicationToCost->setMeanValue(3.11*conversionFactor);
    OzoneDepletionToCost->setMeanValue(30.4*conversionFactor);
    ParticulateMatterFormationToCost->setMeanValue(39.2*conversionFactor);
    PhotochemicalOxidantToCost->setMeanValue(1.15*conversionFactor);
    TerrestrialAcidificationToCost->setMeanValue(4.97*conversionFactor);
    TerrestrialEcotoxicityToCost->setMeanValue(8.69*conversionFactor);
    UrbanLandOccupationToCost->setMeanValue(0.0845*conversionFactor);

    // Set the current value
    AgriculturalLandOccupationToCost->setCurrentValue(0.0845*conversionFactor);
    ClimateChangeToCost->setCurrentValue(0.057*conversionFactor);
    FreshwaterEcotoxicityToCost->setCurrentValue(0.0361*conversionFactor);
    FreshwaterEutrophicationToCost->setCurrentValue(1.86*conversionFactor);
    HumanToxicityToCost->setCurrentValue(0.0991*conversionFactor);
    IonisingRadiationToCost->setCurrentValue(0.0461*conversionFactor);
    MarineEcotoxicityToCost->setCurrentValue(0.00739*conversionFactor);
    MarineEutrophicationToCost->setCurrentValue(3.11*conversionFactor);
    OzoneDepletionToCost->setCurrentValue(30.4*conversionFactor);
    ParticulateMatterFormationToCost->setCurrentValue(39.2*conversionFactor);
    PhotochemicalOxidantToCost->setCurrentValue(1.15*conversionFactor);
    TerrestrialAcidificationToCost->setCurrentValue(4.97*conversionFactor);
    TerrestrialEcotoxicityToCost->setCurrentValue(8.69*conversionFactor);
    UrbanLandOccupationToCost->setCurrentValue(0.0845*conversionFactor);

    // Set the COV
    AgriculturalLandOccupationToCost->setCoefficientOfVariation(cov);
    ClimateChangeToCost->setCoefficientOfVariation(cov);
    FreshwaterEcotoxicityToCost->setCoefficientOfVariation(cov);
    FreshwaterEutrophicationToCost->setCoefficientOfVariation(cov);
    HumanToxicityToCost->setCoefficientOfVariation(cov);
    IonisingRadiationToCost->setCoefficientOfVariation(cov);
    MarineEcotoxicityToCost->setCoefficientOfVariation(cov);
    MarineEutrophicationToCost->setCoefficientOfVariation(cov);
    OzoneDepletionToCost->setCoefficientOfVariation(cov);
    ParticulateMatterFormationToCost->setCoefficientOfVariation(cov);
    PhotochemicalOxidantToCost->setCoefficientOfVariation(cov);
    TerrestrialAcidificationToCost->setCoefficientOfVariation(cov);
    TerrestrialEcotoxicityToCost->setCoefficientOfVariation(cov);
    UrbanLandOccupationToCost->setCoefficientOfVariation(cov);

}























