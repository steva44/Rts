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

#include "RCostOfEnergyModel.h"
#include "RContinuousRandomVariable.h"
#include "RInfoRichResponse.h"
#include "RStatistics.h"
#include "RDomain.h"

RCostOfEnergyModel::RCostOfEnergyModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theResponse = nullptr;
    costOFElectricityLowerTier  = nullptr;
    costOFElectricityUpperTier  = nullptr;
    costOFENaturalGasBuildingOperations  = nullptr;
    costOFDieselTransAndEquip  = nullptr;
    costOFGasolineTransAndEquip  = nullptr;
    costOFLPG  = nullptr;

    // Create the response object
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    createRandomVariables();

    theStatsMethod = theDomain->getStatsMethod();
}


RCostOfEnergyModel::~RCostOfEnergyModel()
{

}


QString RCostOfEnergyModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}


void RCostOfEnergyModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


int RCostOfEnergyModel::evaluateModel(RGradientType theGradientType)
{

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"****************************";
        qDebug()<<"*******Cost of Energy*******";
        qDebug()<<"****************************";
    }

    theResponse->setCurrentValue(0.0);

    // Get the models that serve input to this model
    QList<RInfoRichResponse *> inputResponselList;
    for (auto it : theInputParameterList)
    {
        RInfoRichResponse *theInputCastAsResponse = qobject_cast<RInfoRichResponse *>(it);

        if(theInputCastAsResponse)
            inputResponselList.push_back(theInputCastAsResponse);
    }

    auto totalCostOfEnergy = 0.0;

    auto totalCostDieselLightMedDutyTrans = 0.0;
    auto totalCostDieselHeavyDutyTrans    = 0.0;
    auto totalCostDieselEquipment         = 0.0;

    auto  totalCostElectricity = 0.0;
    //    auto  totalCostLPG         = 0.0;
    auto  totalCostNaturalGasBuildingOperations  = 0.0;

    auto  totalCostGasolineLightMedDutyTrans  = 0.0;
    auto  totalCostGasolineHeavyDutyTrans     = 0.0;
    auto  totalCostGasolineEquipment          = 0.0;


    // Energy in Joule per Litre or Joule per kWh for different energy sources
    // double energyInCoal        = 1.0;
    double energyInDiesel      = 35800000;
    // double energyInFuelOil     = 34800000;
    double energyInGasoline    = 34200000;
    double energyInLPG         = 26000000;
    double energyInNaturalGas  = 37300;

    //For each response
    for(auto it : inputResponselList)
    {
        if(it->isEmpty())
            continue;

        auto costOfEnergyAtThisStage = 0.0;

        auto  JoulesCoal                                = it->getCurrentValue("JoulesCoal");

        auto  JoulesGasolineLightMediumDutyTransport    = it->getCurrentValue("JoulesGasolineLightMediumDutyTransport");
        auto  JoulesDieselLightMedDutyTransport         = it->getCurrentValue("JoulesDieselLightMediumDutyTransport");
        auto  JoulesNatGasLightMediumDutyTransport      = it->getCurrentValue("JoulesNaturalGasLightMediumDutyTransport");
        auto  JoulesPropaneLightMediumDutyTransport     = it->getCurrentValue("JoulesPropaneLightMediumDutyTransport");

        auto  JoulesGasolineHeavyDutyTransport          = it->getCurrentValue("JoulesGasolineHeavyDutyTransport");
        auto  JoulesDieselHeavyDutyTransport            = it->getCurrentValue("JoulesDieselHeavyDutyTransport");
        auto  JoulesNatGasHeavyDutyTransport            = it->getCurrentValue("JoulesNaturalGasHeavyDutyTransport");

        auto  JoulesGasolineEquipment                   = it->getCurrentValue("JoulesGasolineEquipmentUsage");
        auto  JoulesDieselEquipment                     = it->getCurrentValue("JoulesDieselEquipmentUsage");

        auto  kWhElectricityTransportation   = it->getCurrentValue("kWhElectricity");
        auto  JoulesFuelOil     = it->getCurrentValue("JoulesFuelOil");
        auto  JoulesLPG         = it->getCurrentValue("JoulesLPG");
        auto  JoulesNaturalGasBuildingOperations  = it->getCurrentValue("JoulesNaturalGasBuildingOperations");
        auto  kWhElectricityBuildingOperations  = it->getCurrentValue("kWhElectricityBuildingOperations");


        //**************************************************
        //**************************************************
        //*******************DIRECT COSTS*******************
        //**************************************************
        //**************************************************

        auto  CostElectricityBuildOps = 0.0;
        auto  CostElectricityTrans = 0.0;
        auto  CostLPG         = 0.0;
        auto  CostNaturalGasBuildingOperations  = 0.0;

        // Direct cost of electricity in operations
        // 0.0829 < 1,350 kWh   $/kWh (lower tier cost)
        // 0.1243 > 1,350 kWh	$/kWh (upper tier cost)	(BC Hydro, 2018)
        if(kWhElectricityBuildingOperations < 1350.0)
        {
            CostElectricityBuildOps += costOFElectricityLowerTier->getCurrentValue()*kWhElectricityBuildingOperations;
        }
        else
        {
            CostElectricityBuildOps += costOFElectricityLowerTier->getCurrentValue()*1350.0;
            CostElectricityBuildOps += costOFElectricityUpperTier->getCurrentValue()*(kWhElectricityBuildingOperations-1350.0);
        }

        // Now add the cost of electricity due to transport - assume upper tier cost
        CostElectricityTrans += costOFElectricityUpperTier->getCurrentValue()*kWhElectricityTransportation;

        theStatsMethod->accumulate(it->objectName()+"CostOfElectricityBuildingOperations",CostElectricityBuildOps);

        theStatsMethod->update(it->objectName()+"CostOfElectricityBuildingOperations",CostElectricityBuildOps);
        theStatsMethod->update(it->objectName()+"CostElectricityWorkerTransport",CostElectricityTrans);

        auto sumElec = CostElectricityBuildOps + CostElectricityTrans;

        totalCostElectricity += sumElec;

        costOfEnergyAtThisStage += sumElec;

        // Direct cost of natural gas in building operations
        // 6.662	$/GJ	(Fortis BC, 2018)
        CostNaturalGasBuildingOperations = costOFENaturalGasBuildingOperations->getCurrentValue()*JoulesNaturalGasBuildingOperations*1.0e-9;

        theStatsMethod->accumulate(it->objectName()+"CostNaturalGasBuildingOperations",CostNaturalGasBuildingOperations);

        totalCostNaturalGasBuildingOperations  += CostNaturalGasBuildingOperations;

        theStatsMethod->update(it->objectName()+"CostNaturalGasBuildingOperations",CostNaturalGasBuildingOperations);

        // For transport and equipment, assume costs are the same as at the pump
        // Diesel: $1.31 /L  (https://www.globalpetrolprices.com/Canada/diesel_prices/)
        auto costDiesel = costOFDieselTransAndEquip->getCurrentValue();

        auto CostDieselLightMedDutyTrans = costDiesel*JoulesDieselLightMedDutyTransport/energyInDiesel;
        auto CostDieselHeavyDutyTrans    = costDiesel*JoulesDieselHeavyDutyTransport/energyInDiesel;
        auto CostDieselEquipment         = costDiesel*JoulesDieselEquipment/energyInDiesel;

        totalCostDieselLightMedDutyTrans += CostDieselLightMedDutyTrans;
        totalCostDieselHeavyDutyTrans    += CostDieselHeavyDutyTrans   ;
        totalCostDieselEquipment         += CostDieselEquipment        ;

        theStatsMethod->update(it->objectName()+"CostDieselLightMedDutyTrans",CostDieselLightMedDutyTrans);
        theStatsMethod->update(it->objectName()+"CostDieselHeavyDutyTrans",CostDieselHeavyDutyTrans);
        theStatsMethod->update(it->objectName()+"CostDieselEquipment",CostDieselEquipment);

        auto totalCostDiesel  = CostDieselLightMedDutyTrans + CostDieselHeavyDutyTrans + CostDieselEquipment;

        costOfEnergyAtThisStage += totalCostDiesel;

        //Gasoline: $1.43 /L (https://www.globalpetrolprices.com/Canada/gasoline_prices/)
        auto costGasoline  = costOFGasolineTransAndEquip->getCurrentValue();
        auto CostGasolineLightMedDutyTrans  = costGasoline*JoulesGasolineLightMediumDutyTransport/energyInGasoline;
        auto CostGasolineHeavyDutyTrans     = costGasoline*JoulesGasolineHeavyDutyTransport/energyInGasoline;
        auto CostGasolineEquipment          = costGasoline*JoulesGasolineEquipment/energyInGasoline;

        theStatsMethod->update(it->objectName()+"CostGasolineLightMedDutyTrans",CostGasolineLightMedDutyTrans);
        theStatsMethod->update(it->objectName()+"CostGasolineHeavyDutyTrans",CostGasolineHeavyDutyTrans);
        theStatsMethod->update(it->objectName()+"CostGasolineEquipment",CostGasolineEquipment);

        totalCostGasolineLightMedDutyTrans  +=  CostGasolineLightMedDutyTrans;
        totalCostGasolineHeavyDutyTrans     +=  CostGasolineHeavyDutyTrans   ;
        totalCostGasolineEquipment          +=  CostGasolineEquipment        ;

        auto totalCostGasoline = CostGasolineLightMedDutyTrans + CostGasolineHeavyDutyTrans + CostGasolineEquipment;

        costOfEnergyAtThisStage += totalCostGasoline;

        //LPG: $0.89 /L (https://www.globalpetrolprices.com/Canada/lpg_prices/)
        CostLPG = costOFLPG->getCurrentValue()*JoulesLPG/energyInLPG;

        //To do: natural gas for transportation
        auto totalCostNaturalGas = CostNaturalGasBuildingOperations;

        costOfEnergyAtThisStage += totalCostNaturalGas;

        if(JoulesCoal != 0.0 || JoulesFuelOil != 0.0)
            qDebug()<<"The prices for coal and fuel oil not implemented yet in "<<__FUNCTION__;

        if (theOutputLevel >= RObject::Maximum) {

            qDebug()<<"For the response "<<it->objectName();
            qDebug()<<"The direct cost of Diesel" <<totalCostDiesel;
            qDebug()<<"The direct cost of Natural Gas" <<totalCostNaturalGas;
            qDebug()<<"The direct cost of Gasoline" <<totalCostGasoline;
            qDebug()<<"The direct cost of Electricity"<<sumElec;

        }

        theStatsMethod->update(it->objectName()+"directCostOfDiesel",totalCostDiesel);
        theStatsMethod->update(it->objectName()+"directCostOfNaturalGas",totalCostNaturalGas);
        theStatsMethod->update(it->objectName()+"directCostOfGasoline",totalCostGasoline);
        theStatsMethod->update(it->objectName()+"directCostOfElectricity",sumElec);

        theStatsMethod->update(it->objectName()+"CostOfEnergy",costOfEnergyAtThisStage);

        totalCostOfEnergy += costOfEnergyAtThisStage;
    }

    if (theOutputLevel >= RObject::Maximum) {

        qDebug()<<"The total cost of energy is:"<<totalCostOfEnergy;
    }


    theResponse->setCurrentValue(totalCostOfEnergy);

    theStatsMethod->update(theResponse->objectName()+"totalCostOfEnergy",totalCostOfEnergy);

    theStatsMethod->update(theResponse->objectName()+"totalCostDieselLightMedDutyTrans",totalCostDieselLightMedDutyTrans);
    theStatsMethod->update(theResponse->objectName()+"totalCostDieselHeavyDutyTrans",totalCostDieselHeavyDutyTrans);
    theStatsMethod->update(theResponse->objectName()+"totalCostDieselEquipment",totalCostDieselEquipment);
    theStatsMethod->update(theResponse->objectName()+"totalCostElectricity",totalCostElectricity);
    //theStatsMethod->update(theResponse->objectName()+"totalCostLPG",ttotalCostLPG);
    theStatsMethod->update(theResponse->objectName()+"totalCostNaturalGasBuildingOperations",totalCostNaturalGasBuildingOperations);
    theStatsMethod->update(theResponse->objectName()+"totalCostGasolineLightMedDutyTrans",totalCostGasolineLightMedDutyTrans);
    theStatsMethod->update(theResponse->objectName()+"totalCostGasolineHeavyDutyTrans",totalCostGasolineHeavyDutyTrans);
    theStatsMethod->update(theResponse->objectName()+"totalCostGasolineEquipment",totalCostGasolineEquipment);

    return 0;
}


void RCostOfEnergyModel::createRandomVariables()
{
    QString cov = "0.05";

    // Direct cost of electricity in operations
    // 0.0829 < 1,350 kWh   $/kWh (lower tier cost)
    // 0.1243 > 1,350 kWh	$/kWh (upper tier cost)	(BC Hydro, 2018)
    theDomain->createObject(objectName() + "costOFElectricityLowerTier", "RContinuousRandomVariable");
    costOFElectricityLowerTier = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    costOFElectricityLowerTier->setDistributionType(QString("Lognormal (zeta, sigma)"));
    costOFElectricityLowerTier->setMeanValue(0.0829);
    costOFElectricityLowerTier->setCurrentValue(0.0829);
    costOFElectricityLowerTier->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "costOFElectricityUpperTier", "RContinuousRandomVariable");
    costOFElectricityUpperTier = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    costOFElectricityUpperTier->setDistributionType(QString("Lognormal (zeta, sigma)"));
    costOFElectricityUpperTier->setMeanValue(0.1243);
    costOFElectricityUpperTier->setCurrentValue(0.1243);
    costOFElectricityUpperTier->setCoefficientOfVariation(cov);

    // Direct cost of natural gas in building operations
    // 6.662	$/GJ	(Fortis BC, 2018)
    theDomain->createObject(objectName() + "costOFENaturalGasBuildingOperations", "RContinuousRandomVariable");
    costOFENaturalGasBuildingOperations = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    costOFENaturalGasBuildingOperations->setDistributionType(QString("Lognormal (zeta, sigma)"));
    costOFENaturalGasBuildingOperations->setMeanValue(6.662);
    costOFENaturalGasBuildingOperations->setCurrentValue(6.662);
    costOFENaturalGasBuildingOperations->setCoefficientOfVariation(cov);


    // For transport and equipment, assume costs are the same at the pump
    // $1.31/L  (https://www.globalpetrolprices.com/Canada/diesel_prices/)
    theDomain->createObject(objectName() + "costOFDieselTransAndEquip", "RContinuousRandomVariable");
    costOFDieselTransAndEquip = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    costOFDieselTransAndEquip->setDistributionType(QString("Lognormal (zeta, sigma)"));
    costOFDieselTransAndEquip->setMeanValue(1.31);
    costOFDieselTransAndEquip->setCurrentValue(1.31);
    costOFDieselTransAndEquip->setCoefficientOfVariation(cov);

    // $1.43/L (https://www.globalpetrolprices.com/Canada/gasoline_prices/)
    theDomain->createObject(objectName() + "costOFGasolineTransAndEquip", "RContinuousRandomVariable");
    costOFGasolineTransAndEquip = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    costOFGasolineTransAndEquip->setDistributionType(QString("Lognormal (zeta, sigma)"));
    costOFGasolineTransAndEquip->setMeanValue(1.43);
    costOFGasolineTransAndEquip->setCurrentValue(1.43);
    costOFGasolineTransAndEquip->setCoefficientOfVariation(cov);

    // $0.89/L (https://www.globalpetrolprices.com/Canada/lpg_prices/)
    theDomain->createObject(objectName() + "costOFLPG", "RContinuousRandomVariable");
    costOFLPG = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    costOFLPG->setDistributionType(QString("Lognormal (zeta, sigma)"));
    costOFLPG->setMeanValue(0.89);
    costOFLPG->setCurrentValue(0.89);
    costOFLPG->setCoefficientOfVariation(cov);

    return;
}



