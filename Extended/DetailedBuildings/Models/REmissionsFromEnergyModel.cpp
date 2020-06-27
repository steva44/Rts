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

#include "REmissionsFromEnergyModel.h"
#include "RContinuousRandomVariable.h"
#include "RStatistics.h"
#include "RDomain.h"

REmissionsFromEnergyModel::REmissionsFromEnergyModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theResponse = nullptr;
    LightMedDutyGasolineCO2Emissions  = nullptr;
    LightMedDutyDieselCO2Emissions  = nullptr;
    LightMedDutyPropaneCO2Emissions  = nullptr;
    LightMedDutyNatGasCO2Emissions  = nullptr;
    LightMedDutyGasolineCH4Emissions  = nullptr;
    LightMedDutyDieselCH4Emissions  = nullptr;
    LightMedDutyPropaneCH4Emissions  = nullptr;
    LightMedDutyNatGasCH4Emissions  = nullptr;
    LightMedDutyGasolineN2OEmissions  = nullptr;
    LightMedDutyDieselN2OEmissions  = nullptr;
    LightMedDutyPropaneN2OEmissions  = nullptr;
    LightMedDutyNatGasN2OEmissions  = nullptr;
    HeavyDutyGasolineCO2Emissions  = nullptr;
    HeavyDutyDieselCO2Emissions  = nullptr;
    HeavyDutyNatGasCO2Emissions  = nullptr;
    HeavyDutyGasolineCH4Emissions  = nullptr;
    HeavyDutyDieselCH4Emissions  = nullptr;
    HeavyDutyNatGasCH4Emissions  = nullptr;
    HeavyDutyGasolineN2OEmissions  = nullptr;
    HeavyDutyDieselN2OEmissions  = nullptr;
    HeavyDutyNatGasN2OEmissions  = nullptr;
    EquipmentGasolineCO2Emissions  = nullptr;
    EquipmentDieselCO2Emissions  = nullptr;
    EquipmentNatGasCO2Emissions  = nullptr;
    EquipmentGasolineCH4Emissions  = nullptr;
    EquipmentDieselCH4Emissions  = nullptr;
    EquipmentNatGasCH4Emissions  = nullptr;
    EquipmentGasolineN2OEmissions  = nullptr;
    EquipmentDieselN2OEmissions  = nullptr;
    EquipmentNatGasN2OEmissions  = nullptr;
    BuildingOperationsNatGasCO2Emissions  = nullptr;
    ElectricityGenerationCO2Emissions  = nullptr;
    BuildingOperationsNatGasCH4Emissions  = nullptr;
    BuildingOperationsNatGasN2OEmissions  = nullptr;

    // Create the info rich response object
    theDomain->createObject(objectName() + "Response", "RInfoRichResponse");
    theResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    createRandomVariables();

    theStatsMethod = theDomain->getStatsMethod();
}


REmissionsFromEnergyModel::~REmissionsFromEnergyModel()
{

}


QString REmissionsFromEnergyModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}


void REmissionsFromEnergyModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


int REmissionsFromEnergyModel::evaluateModel(RGradientType theGradientType)
{
    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"***********************************";
        qDebug()<<"*******Emissions From Energy*******";
        qDebug()<<"***********************************";
    }

    theResponse->clearResponses();

    // Get the models that serve input to this model
    QList<RInfoRichResponse *> inputResponselList;
    for (auto it : theInputParameterList)
    {
        RInfoRichResponse *theInputCastAsResponse = qobject_cast<RInfoRichResponse *>(it);

        if(theInputCastAsResponse)
            inputResponselList.push_back(theInputCastAsResponse);
    }

    auto totalCO2Workertransport    = 0.0;
    auto totalCO2Materialtransport  = 0.0;
    auto totalCO2equipment          = 0.0;
    auto totalCO2BuildOpsElec       = 0.0;
    auto totalCO2BuildOpsNaturalGas = 0.0;
    auto totalCH4Workertransport    = 0.0;
    auto totalCH4Materialtransport  = 0.0;
    auto totalCH4equipment          = 0.0;
    auto totalCH4BuildOps           = 0.0;
    auto totalN2OWorkertransport    = 0.0;
    auto totalN2OMaterialtransport  = 0.0;
    auto totalN2Oequipment          = 0.0;
    auto totalN2OBuildOps           = 0.0;

    // Energy in Joule per Litre or Joule per kWh for different energy sources
    double energyInDiesel      = 35800000;
    double energyInGasoline    = 34200000;
    double energyInLPG         = 26000000;
    double energyInNaturalGas  = 37300;

    auto CO2Emissions            =  0.0;
    auto MethaneEmissions        =  0.0;
    auto NitrousOxideEmissions   =  0.0;

    // For each response
    for(auto it : inputResponselList)
    {
        if(it->isEmpty())
            continue;

        // First get the joules or kWh of energy consumed
        auto  JoulesGasolineLightMediumDutyTransport    = it->getCurrentValue("JoulesGasolineLightMediumDutyTransport");
        auto  JoulesDieselLightMedDutyTransport         = it->getCurrentValue("JoulesDieselLightMediumDutyTransport");
        auto  JoulesNatGasLightMediumDutyTransport      = it->getCurrentValue("JoulesNaturalGasLightMediumDutyTransport");
        auto  JoulesPropaneLightMediumDutyTransport     = it->getCurrentValue("JoulesPropaneLightMediumDutyTransport");

        auto  JoulesGasolineHeavyDutyTransport          = it->getCurrentValue("JoulesGasolineHeavyDutyTransport");
        auto  JoulesDieselHeavyDutyTransport            = it->getCurrentValue("JoulesDieselHeavyDutyTransport");
        auto  JoulesNatGasHeavyDutyTransport            = it->getCurrentValue("JoulesNaturalGasHeavyDutyTransport");

        auto  JoulesGasolineEquipment                   = it->getCurrentValue("JoulesGasolineEquipmentUsage");
        auto  JoulesDieselEquipment                     = it->getCurrentValue("JoulesDieselEquipmentUsage");

        auto  kWhElectricity    = it->getCurrentValue("kWhElectricity");
        auto  JoulesNaturalGasBuildingOperations  = it->getCurrentValue("JoulesNaturalGasBuildingOperations");
        auto  kWhElectricityBuildingOps    = it->getCurrentValue("kWhElectricityBuildingOperations");


        // Next, translate the energy consumed into emissions, in accordance with the energy consumption mode
        // All emissions are given in kg
        // Convert kWh electricity into GJ
        auto GJElectricity = kWhElectricity*0.0036;

        // Convert kWh into GJ
        auto GJElectricityBuildOps = kWhElectricityBuildingOps*0.0036;

        // CO2 emissions transport and equipment usage
        auto CO2Workertransport = JoulesGasolineLightMediumDutyTransport/energyInGasoline*LightMedDutyGasolineCO2Emissions->getCurrentValue() +
                JoulesDieselLightMedDutyTransport/energyInDiesel*LightMedDutyGasolineCO2Emissions->getCurrentValue() +
                JoulesNatGasLightMediumDutyTransport/energyInNaturalGas*LightMedDutyNatGasCO2Emissions->getCurrentValue() +
                JoulesPropaneLightMediumDutyTransport/energyInLPG*LightMedDutyPropaneCO2Emissions->getCurrentValue();

        theStatsMethod->update(it->objectName()+"CO2EmissionsWorkerTransport",CO2Workertransport);

        auto CO2Materialtransport =  JoulesGasolineHeavyDutyTransport/energyInGasoline*HeavyDutyGasolineCO2Emissions->getCurrentValue() +
                JoulesDieselHeavyDutyTransport/energyInDiesel*HeavyDutyDieselCO2Emissions->getCurrentValue() +
                JoulesNatGasHeavyDutyTransport/energyInNaturalGas*HeavyDutyNatGasCO2Emissions->getCurrentValue();

        theStatsMethod->update(it->objectName()+"CO2EmissionsMaterialTransport",CO2Materialtransport);

        auto CO2equipment = JoulesDieselEquipment/energyInDiesel*EquipmentDieselCO2Emissions->getCurrentValue() +
                JoulesGasolineEquipment/energyInGasoline*EquipmentGasolineCO2Emissions->getCurrentValue();

        theStatsMethod->update(it->objectName()+"CO2EmissionsEquipment",CO2equipment);

        // CO2 emissions from building operations = GJ*kg/GJ
        auto CO2BuildOpsNaturalGas = JoulesNaturalGasBuildingOperations*1.0e-9*BuildingOperationsNatGasCO2Emissions->getCurrentValue(); // In Giga Joules
        auto CO2BuildOpsElec = GJElectricityBuildOps*ElectricityGenerationCO2Emissions->getCurrentValue(); // In Giga Joules

        theStatsMethod->update(it->objectName()+"CO2EmissionsElectricityBuildingOps",CO2BuildOpsElec);
        theStatsMethod->update(it->objectName()+"CO2EmissionsNaturalGasBuildingOps",CO2BuildOpsNaturalGas);

        auto CO2TransportElec = GJElectricity*ElectricityGenerationCO2Emissions->getCurrentValue(); // In Giga Joules
        theStatsMethod->update(it->objectName()+"CO2EmissionsElectricityTransport",CO2TransportElec);

        CO2Workertransport += CO2TransportElec;

        CO2Emissions +=  CO2Workertransport + CO2Materialtransport + CO2equipment +  CO2BuildOpsElec + CO2BuildOpsNaturalGas;

        totalCO2Workertransport    += CO2Workertransport;
        totalCO2Materialtransport  += CO2Materialtransport;
        totalCO2equipment          += CO2equipment;
        totalCO2BuildOpsElec       += CO2BuildOpsElec;
        totalCO2BuildOpsNaturalGas += CO2BuildOpsNaturalGas;

        // CH4 emissions transport and equipment usage
        auto CH4Workertransport = JoulesGasolineLightMediumDutyTransport/energyInGasoline*LightMedDutyGasolineCH4Emissions->getCurrentValue() +
                JoulesDieselLightMedDutyTransport/energyInDiesel*LightMedDutyGasolineCH4Emissions->getCurrentValue() +
                JoulesNatGasLightMediumDutyTransport/energyInNaturalGas*LightMedDutyNatGasCH4Emissions->getCurrentValue() +
                JoulesPropaneLightMediumDutyTransport/energyInLPG*LightMedDutyPropaneCH4Emissions->getCurrentValue();

        theStatsMethod->update(it->objectName()+"CH4EmissionsWorkerTransport",CH4Workertransport);

        auto CH4Materialtransport =  JoulesGasolineHeavyDutyTransport/energyInGasoline*HeavyDutyGasolineCH4Emissions->getCurrentValue() +
                JoulesDieselHeavyDutyTransport/energyInDiesel*HeavyDutyDieselCH4Emissions->getCurrentValue() +
                JoulesNatGasHeavyDutyTransport/energyInNaturalGas*HeavyDutyNatGasCH4Emissions->getCurrentValue();

        theStatsMethod->update(it->objectName()+"CH4EmissionsMaterialTransport",CH4Materialtransport);

        auto CH4equipment = JoulesDieselEquipment/energyInDiesel*EquipmentDieselCH4Emissions->getCurrentValue() +
                JoulesGasolineEquipment/energyInGasoline*EquipmentGasolineCH4Emissions->getCurrentValue();

        theStatsMethod->update(it->objectName()+"CH4EmissionsEquipment",CH4equipment);

        // CH4 emissions from building operations = GJ*kg/GJ
        auto CH4BuildOps = JoulesNaturalGasBuildingOperations*1.0e-9*BuildingOperationsNatGasCH4Emissions->getCurrentValue(); //Giga joules natural gas

        theStatsMethod->update(it->objectName()+"CH4EmissionsBuildingOperations",CH4BuildOps);

        MethaneEmissions += CH4Workertransport + CH4Materialtransport + CH4equipment + CH4BuildOps;

        totalCH4Workertransport   += CH4Workertransport;
        totalCH4Materialtransport += CH4Materialtransport;
        totalCH4equipment         += CH4equipment;
        totalCH4BuildOps          += CH4BuildOps;

        // N2O  emissions transport and equipment usage
        auto N2OWorkertransport = JoulesGasolineLightMediumDutyTransport/energyInGasoline*LightMedDutyGasolineN2OEmissions->getCurrentValue() +
                JoulesDieselLightMedDutyTransport/energyInDiesel*LightMedDutyGasolineN2OEmissions->getCurrentValue() +
                JoulesNatGasLightMediumDutyTransport/energyInNaturalGas*LightMedDutyNatGasN2OEmissions->getCurrentValue() +
                JoulesPropaneLightMediumDutyTransport/energyInLPG*LightMedDutyPropaneN2OEmissions->getCurrentValue();

        theStatsMethod->update(it->objectName()+"N2OEmissionsWorkerTransport",N2OWorkertransport);

        auto N2OMaterialtransport =  JoulesGasolineHeavyDutyTransport/energyInGasoline*HeavyDutyGasolineN2OEmissions->getCurrentValue() +
                JoulesDieselHeavyDutyTransport/energyInDiesel*HeavyDutyDieselN2OEmissions->getCurrentValue() +
                JoulesNatGasHeavyDutyTransport/energyInNaturalGas*HeavyDutyNatGasN2OEmissions->getCurrentValue();

        theStatsMethod->update(it->objectName()+"N2OEmissionsMaterialTransport",N2OMaterialtransport);

        auto N2Oequipment = JoulesDieselEquipment/energyInDiesel*EquipmentDieselN2OEmissions->getCurrentValue() +
                JoulesGasolineEquipment/energyInGasoline*EquipmentGasolineN2OEmissions->getCurrentValue();

        theStatsMethod->update(it->objectName()+"N2OEmissionsEquipment",N2OMaterialtransport);

        // N2O emissions from building operations = GJ*kg/GJ
        auto N2OBuildOps = JoulesNaturalGasBuildingOperations*1.0e-9*BuildingOperationsNatGasN2OEmissions->getCurrentValue(); //Giga joules natural gas

        theStatsMethod->update(it->objectName()+"N2OEmissionsBuildingOperations",N2OBuildOps);

        NitrousOxideEmissions += N2OWorkertransport + N2OMaterialtransport + N2Oequipment + N2OBuildOps;

        totalN2OWorkertransport   += N2OWorkertransport;
        totalN2OMaterialtransport += N2OMaterialtransport;
        totalN2Oequipment         += N2Oequipment;
        totalN2OBuildOps          += N2OBuildOps;

    }

    theStatsMethod->update(theResponse->objectName()+"TotalEmissionCO2Workertransport",totalCO2Workertransport);
    theStatsMethod->update(theResponse->objectName()+"TotalEmissionCO2Materialtransport",totalCO2Materialtransport);
    theStatsMethod->update(theResponse->objectName()+"TotalEmissionCO2equipment",totalCO2equipment);
    theStatsMethod->update(theResponse->objectName()+"TotalCO2BuildOpsElec",totalCO2BuildOpsElec);
    theStatsMethod->update(theResponse->objectName()+"TotalCO2BuildOpsNaturalGas",totalCO2BuildOpsNaturalGas);
    theStatsMethod->update(theResponse->objectName()+"TotalEmissionCH4Workertransport",totalCH4Workertransport);
    theStatsMethod->update(theResponse->objectName()+"TotalEmissionCH4Materialtransport",totalCH4Materialtransport);
    theStatsMethod->update(theResponse->objectName()+"TotalEmissionCH4equipment",totalCH4equipment);
    theStatsMethod->update(theResponse->objectName()+"TotalEmissionCH4BuildOps",totalCH4BuildOps);
    theStatsMethod->update(theResponse->objectName()+"TotalEmissionN2OWorkertransport",totalN2OWorkertransport);
    theStatsMethod->update(theResponse->objectName()+"TotalEmissionN2OMaterialtransport",totalN2OMaterialtransport);
    theStatsMethod->update(theResponse->objectName()+"TotalEmissionN2Oequipment",totalN2Oequipment);
    theStatsMethod->update(theResponse->objectName()+"TotalEmissionN2OBuildOps",totalN2OBuildOps);

    theResponse->setCurrentValue("CO2EmissionsWorkerTransport",totalCO2Workertransport);
    theResponse->setCurrentValue("CO2EmissionsMaterialTransport",totalCO2Materialtransport);
    theResponse->setCurrentValue("CO2EmissionsEquipment",totalCO2equipment);
    theResponse->setCurrentValue("CO2EmissionsBuildOpsElectricity",totalCO2BuildOpsElec);
    theResponse->setCurrentValue("CO2EmissionsBuildOpsNaturalGas",totalCO2BuildOpsNaturalGas);
    theResponse->setCurrentValue("CH4EmissionsWorkerTransport",totalCH4Workertransport);
    theResponse->setCurrentValue("CH4EmissionsMaterialTransport",totalCH4Materialtransport);
    theResponse->setCurrentValue("CH4EmissionsEquipment",totalCH4equipment);
    theResponse->setCurrentValue("CH4EmissionsBuildingOperations",totalCH4BuildOps);
    theResponse->setCurrentValue("N2OEmissionsWorkerTransport",totalN2OWorkertransport);
    theResponse->setCurrentValue("N2OEmissionsMaterialTransport",totalN2OMaterialtransport);
    theResponse->setCurrentValue("N2OEmissionsEquipment",totalN2Oequipment);
    theResponse->setCurrentValue("N2OEmissionsBuildingOperations",totalN2OBuildOps);

    theStatsMethod->accumulate(theResponse->objectName()+"CO2EmissionsBuildOpsNaturalGas",totalCO2BuildOpsNaturalGas);
    theStatsMethod->accumulate(theResponse->objectName()+"CO2EmissionsBuildOpsElectricity",totalCO2BuildOpsElec);
    theStatsMethod->update(theResponse->objectName()+"CarbonDioxideEmissions",CO2Emissions);
    theStatsMethod->update(theResponse->objectName()+"MethaneEmissions",MethaneEmissions);
    theStatsMethod->update(theResponse->objectName()+"NitrousOxideEmissions",NitrousOxideEmissions);

    if (theOutputLevel >= RObject::Maximum)
    {
        qDebug()<<"CarbonDioxideEmissions from energy use [kg]"<<CO2Emissions;
        qDebug()<<"MethaneEmissions from energy use [kg]"<<MethaneEmissions;
        qDebug()<<"NitrousOxideEmissions from energy use [kg]"<<NitrousOxideEmissions;
    }

    return 0;
}


void REmissionsFromEnergyModel::createRandomVariables()
{

    QString cov = "0.2";

    // Light medium duty transport
    // CO2 [kg/L]
    theDomain->createObject(objectName() + "LightMedDutyGasolineCO2Emissions", "RContinuousRandomVariable");
    LightMedDutyGasolineCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyGasolineCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyGasolineCO2Emissions->setMeanValue(2.175);
    LightMedDutyGasolineCO2Emissions->setCurrentValue(2.175);
    LightMedDutyGasolineCO2Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "LightMedDutyDieselCO2Emissions", "RContinuousRandomVariable");
    LightMedDutyDieselCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyDieselCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyDieselCO2Emissions->setMeanValue(2.556);
    LightMedDutyDieselCO2Emissions->setCurrentValue(2.556);
    LightMedDutyDieselCO2Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "LightMedDutyPropaneCO2Emissions", "RContinuousRandomVariable");
    LightMedDutyPropaneCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyPropaneCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyPropaneCO2Emissions->setMeanValue(1.507);
    LightMedDutyPropaneCO2Emissions->setCurrentValue(1.507);
    LightMedDutyPropaneCO2Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "LightMedDutyNatGasCO2Emissions", "RContinuousRandomVariable");
    LightMedDutyNatGasCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyNatGasCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyNatGasCO2Emissions->setMeanValue(2.723);
    LightMedDutyNatGasCO2Emissions->setCurrentValue(2.723);
    LightMedDutyNatGasCO2Emissions->setCoefficientOfVariation(cov);

    // CH4 [kg/L]
    theDomain->createObject(objectName() + "LightMedDutyGasolineCH4Emissions", "RContinuousRandomVariable");
    LightMedDutyGasolineCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyGasolineCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyGasolineCH4Emissions->setMeanValue(0.00024);
    LightMedDutyGasolineCH4Emissions->setCurrentValue(0.00024);
    LightMedDutyGasolineCH4Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "LightMedDutyDieselCH4Emissions", "RContinuousRandomVariable");
    LightMedDutyDieselCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyDieselCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyDieselCH4Emissions->setMeanValue(0.000060);
    LightMedDutyDieselCH4Emissions->setCurrentValue(0.000060);
    LightMedDutyDieselCH4Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "LightMedDutyPropaneCH4Emissions", "RContinuousRandomVariable");
    LightMedDutyPropaneCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyPropaneCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyPropaneCH4Emissions->setMeanValue(0.00064);
    LightMedDutyPropaneCH4Emissions->setCurrentValue(0.00064);
    LightMedDutyPropaneCH4Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "LightMedDutyNatGasCH4Emissions", "RContinuousRandomVariable");
    LightMedDutyNatGasCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyNatGasCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyNatGasCH4Emissions->setMeanValue(0.013);
    LightMedDutyNatGasCH4Emissions->setCurrentValue(0.013);
    LightMedDutyNatGasCH4Emissions->setCoefficientOfVariation(cov);

    // N2O [kg/L]
    theDomain->createObject(objectName() + "LightMedDutyGasolineN2OEmissions", "RContinuousRandomVariable");
    LightMedDutyGasolineN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyGasolineN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyGasolineN2OEmissions->setMeanValue(0.00053);
    LightMedDutyGasolineN2OEmissions->setCurrentValue(0.00053);
    LightMedDutyGasolineN2OEmissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "LightMedDutyDieselN2OEmissions", "RContinuousRandomVariable");
    LightMedDutyDieselN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyDieselN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyDieselN2OEmissions->setMeanValue(0.00022);
    LightMedDutyDieselN2OEmissions->setCurrentValue(0.00022);
    LightMedDutyDieselN2OEmissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "LightMedDutyPropaneN2OEmissions", "RContinuousRandomVariable");
    LightMedDutyPropaneN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyPropaneN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyPropaneN2OEmissions->setMeanValue(0.000028);
    LightMedDutyPropaneN2OEmissions->setCurrentValue(0.000028);
    LightMedDutyPropaneN2OEmissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "LightMedDutyNatGasN2OEmissions", "RContinuousRandomVariable");
    LightMedDutyNatGasN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    LightMedDutyNatGasN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    LightMedDutyNatGasN2OEmissions->setMeanValue(0.000086);
    LightMedDutyNatGasN2OEmissions->setCurrentValue(0.000086);
    LightMedDutyNatGasN2OEmissions->setCoefficientOfVariation(cov);

    //Heavy duty transport
    // CO2 [kg/L]
    theDomain->createObject(objectName() + "HeavyDutyGasolineCO2Emissions", "RContinuousRandomVariable");
    HeavyDutyGasolineCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    HeavyDutyGasolineCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    HeavyDutyGasolineCO2Emissions->setMeanValue(2.175);
    HeavyDutyGasolineCO2Emissions->setCurrentValue(2.175);
    HeavyDutyGasolineCO2Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "HeavyDutyDieselCO2Emissions", "RContinuousRandomVariable");
    HeavyDutyDieselCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    HeavyDutyDieselCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    HeavyDutyDieselCO2Emissions->setMeanValue(2.556);
    HeavyDutyDieselCO2Emissions->setCurrentValue(2.556);
    HeavyDutyDieselCO2Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "HeavyDutyNatGasCO2Emissions", "RContinuousRandomVariable");
    HeavyDutyNatGasCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    HeavyDutyNatGasCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    HeavyDutyNatGasCO2Emissions->setMeanValue(2.723);
    HeavyDutyNatGasCO2Emissions->setCurrentValue(2.723);
    HeavyDutyNatGasCO2Emissions->setCoefficientOfVariation(cov);

    // CH4 [kg/L]
    theDomain->createObject(objectName() + "HeavyDutyGasolineCH4Emissions", "RContinuousRandomVariable");
    HeavyDutyGasolineCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    HeavyDutyGasolineCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    HeavyDutyGasolineCH4Emissions->setMeanValue(0.000068);
    HeavyDutyGasolineCH4Emissions->setCurrentValue(0.000068);
    HeavyDutyGasolineCH4Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "HeavyDutyDieselCH4Emissions", "RContinuousRandomVariable");
    HeavyDutyDieselCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    HeavyDutyDieselCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    HeavyDutyDieselCH4Emissions->setMeanValue(0.00011);
    HeavyDutyDieselCH4Emissions->setCurrentValue(0.00011);
    HeavyDutyDieselCH4Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "HeavyDutyNatGasCH4Emissions", "RContinuousRandomVariable");
    HeavyDutyNatGasCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    HeavyDutyNatGasCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    HeavyDutyNatGasCH4Emissions->setMeanValue(0.013);
    HeavyDutyNatGasCH4Emissions->setCurrentValue(0.013);
    HeavyDutyNatGasCH4Emissions->setCoefficientOfVariation(cov);

    // N2O [kg/L]
    theDomain->createObject(objectName() + "HeavyDutyGasolineN2OEmissions", "RContinuousRandomVariable");
    HeavyDutyGasolineN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    HeavyDutyGasolineN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    HeavyDutyGasolineN2OEmissions->setMeanValue(0.00020);
    HeavyDutyGasolineN2OEmissions->setCurrentValue(0.00020);
    HeavyDutyGasolineN2OEmissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "HeavyDutyDieselN2OEmissions", "RContinuousRandomVariable");
    HeavyDutyDieselN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    HeavyDutyDieselN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    HeavyDutyDieselN2OEmissions->setMeanValue(0.000151);
    HeavyDutyDieselN2OEmissions->setCurrentValue(0.000151);
    HeavyDutyDieselN2OEmissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "HeavyDutyNatGasN2OEmissions", "RContinuousRandomVariable");
    HeavyDutyNatGasN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    HeavyDutyNatGasN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    HeavyDutyNatGasN2OEmissions->setMeanValue(0.000086);
    HeavyDutyNatGasN2OEmissions->setCurrentValue(0.000086);
    HeavyDutyNatGasN2OEmissions->setCoefficientOfVariation(cov);

    // Equipment and offroad usage
    // CO2 [kg/L]
    theDomain->createObject(objectName() + "EquipmentGasolineCO2Emissions", "RContinuousRandomVariable");
    EquipmentGasolineCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    EquipmentGasolineCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    EquipmentGasolineCO2Emissions->setMeanValue(2.175);
    EquipmentGasolineCO2Emissions->setCurrentValue(2.175);
    EquipmentGasolineCO2Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "EquipmentDieselCO2Emissions", "RContinuousRandomVariable");
    EquipmentDieselCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    EquipmentDieselCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    EquipmentDieselCO2Emissions->setMeanValue(2.556);
    EquipmentDieselCO2Emissions->setCurrentValue(2.556);
    EquipmentDieselCO2Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "EquipmentNatGasCO2Emissions", "RContinuousRandomVariable");
    EquipmentNatGasCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    EquipmentNatGasCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    EquipmentNatGasCO2Emissions->setMeanValue(2.723);
    EquipmentNatGasCO2Emissions->setCurrentValue(2.723);
    EquipmentNatGasCO2Emissions->setCoefficientOfVariation(cov);

    // CH4 [kg/L]
    theDomain->createObject(objectName() + "EquipmentGasolineCH4Emissions", "RContinuousRandomVariable");
    EquipmentGasolineCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    EquipmentGasolineCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    EquipmentGasolineCH4Emissions->setMeanValue(0.0027);
    EquipmentGasolineCH4Emissions->setCurrentValue(0.0027);
    EquipmentGasolineCH4Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "EquipmentDieselCH4Emissions", "RContinuousRandomVariable");
    EquipmentDieselCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    EquipmentDieselCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    EquipmentDieselCH4Emissions->setMeanValue(0.00015);
    EquipmentDieselCH4Emissions->setMeanValue(0.00015);
    EquipmentDieselCH4Emissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "EquipmentNatGasCH4Emissions", "RContinuousRandomVariable");
    EquipmentNatGasCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    EquipmentNatGasCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    EquipmentNatGasCH4Emissions->setMeanValue(0.013);
    EquipmentNatGasCH4Emissions->setCurrentValue(0.013);
    EquipmentNatGasCH4Emissions->setCoefficientOfVariation(cov);

    // N2O [kg/L]
    theDomain->createObject(objectName() + "EquipmentGasolineN2OEmissions", "RContinuousRandomVariable");
    EquipmentGasolineN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    EquipmentGasolineN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    EquipmentGasolineN2OEmissions->setMeanValue(0.00005);
    EquipmentGasolineN2OEmissions->setCurrentValue(0.00005);
    EquipmentGasolineN2OEmissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "EquipmentDieselN2OEmissions", "RContinuousRandomVariable");
    EquipmentDieselN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    EquipmentDieselN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    EquipmentDieselN2OEmissions->setMeanValue(0.0011);
    EquipmentDieselN2OEmissions->setCurrentValue(0.0011);
    EquipmentDieselN2OEmissions->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "EquipmentNatGasN2OEmissions", "RContinuousRandomVariable");
    EquipmentNatGasN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    EquipmentNatGasN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    EquipmentNatGasN2OEmissions->setMeanValue(0.000086);
    EquipmentNatGasN2OEmissions->setCurrentValue(0.000086);
    EquipmentNatGasN2OEmissions->setCoefficientOfVariation(cov);

    // Building Operations
    // CO2 from natural gas [kg/GJ]
    theDomain->createObject(objectName() + "BuildingOperationsNatGasCO2Emissions", "RContinuousRandomVariable");
    BuildingOperationsNatGasCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    BuildingOperationsNatGasCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    BuildingOperationsNatGasCO2Emissions->setMeanValue(49.46);
    BuildingOperationsNatGasCO2Emissions->setCurrentValue(49.46);
    BuildingOperationsNatGasCO2Emissions->setCoefficientOfVariation(cov);

    // CO2 from electricity [kg/GJ]
    theDomain->createObject(objectName() + "BuildingOperationsElecCO2Emissions", "RContinuousRandomVariable");
    ElectricityGenerationCO2Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    ElectricityGenerationCO2Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    ElectricityGenerationCO2Emissions->setMeanValue(2.8);
    ElectricityGenerationCO2Emissions->setCurrentValue(2.8);
    ElectricityGenerationCO2Emissions->setCoefficientOfVariation(cov);

    // CH4 from natural gas [kg/GJ]
    theDomain->createObject(objectName() + "BuildingOperationsNatGasCH4Emissions", "RContinuousRandomVariable");
    BuildingOperationsNatGasCH4Emissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    BuildingOperationsNatGasCH4Emissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    BuildingOperationsNatGasCH4Emissions->setMeanValue(0.0010);
    BuildingOperationsNatGasCH4Emissions->setCurrentValue(0.0010);
    BuildingOperationsNatGasCH4Emissions->setCoefficientOfVariation(cov);

    // N2O from natural gas [kg/GJ]
    theDomain->createObject(objectName() + "BuildingOperationsNatGasN2OEmissions", "RContinuousRandomVariable");
    BuildingOperationsNatGasN2OEmissions = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    BuildingOperationsNatGasN2OEmissions->setDistributionType(QString("Lognormal (zeta, sigma)"));
    BuildingOperationsNatGasN2OEmissions->setMeanValue(0.0009);
    BuildingOperationsNatGasN2OEmissions->setCurrentValue(0.0009);
    BuildingOperationsNatGasN2OEmissions->setCoefficientOfVariation(cov);

}




