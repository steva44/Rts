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
*   - Seadon Chui                                                    *
*   - Gurvinder Gill                                                 *
*                                                                    *
*********************************************************************/

#include "RShindellEnvironmentalCostOfEnergyModel.h"
#include "RContinuousRandomVariable.h"
#include "RLocation.h"
#include "RDomain.h"

#include <math.h>

RShindellEnvironmentalCostOfEnergyModel::RShindellEnvironmentalCostOfEnergyModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = nullptr;
    theResponse = nullptr;
    theFuelVolumeToCostConversionCoal = nullptr;
    theFuelVolumeToCostConversionDiesel = nullptr;
    theFuelVolumeToCostConversionElectricity = nullptr;
    theFuelVolumeToCostConversionFuelOil = nullptr;
    theFuelVolumeToCostConversionGasoline = nullptr;
    theFuelVolumeToCostConversionLPG = nullptr;
    theFuelVolumeToCostConversionNaturalGas = nullptr;

    // Create the response object
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    // Create a few random variables
    theDomain->createObject(objectName() + "FuelVolumeToCostConversionGasoline", "RContinuousRandomVariable");
    theFuelVolumeToCostConversionGasoline = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theFuelVolumeToCostConversionGasoline->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theFuelVolumeToCostConversionGasoline->setMean(QString("1.0039"));
    theFuelVolumeToCostConversionGasoline->setCoefficientOfVariation("0.2");

    theDomain->createObject(objectName() + "FuelVolumeToCostConversionDiesel", "RContinuousRandomVariable");
    theFuelVolumeToCostConversionDiesel = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theFuelVolumeToCostConversionDiesel->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theFuelVolumeToCostConversionDiesel->setMean(QString("1.268"));
    theFuelVolumeToCostConversionDiesel->setCoefficientOfVariation("0.2");

    theDomain->createObject(objectName() + "FuelVolumeToCostConversionCoal", "RContinuousRandomVariable");
    theFuelVolumeToCostConversionCoal = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theFuelVolumeToCostConversionCoal->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theFuelVolumeToCostConversionCoal->setMean(QString("0.000000066665"));
    theFuelVolumeToCostConversionCoal->setCoefficientOfVariation("0.2");

    theDomain->createObject(objectName() + "FuelVolumeToCostConversionNaturalGas", "RContinuousRandomVariable");
    theFuelVolumeToCostConversionNaturalGas = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theFuelVolumeToCostConversionNaturalGas->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theFuelVolumeToCostConversionNaturalGas->setMean(QString("0.000000030555"));
    theFuelVolumeToCostConversionNaturalGas->setCoefficientOfVariation("0.2");

    theDomain->createObject(objectName() + "FuelVolumeToCostConversionFuelOil", "RContinuousRandomVariable");
    theFuelVolumeToCostConversionFuelOil = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theFuelVolumeToCostConversionFuelOil->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theFuelVolumeToCostConversionFuelOil->setMean(QString("1.268"));
    theFuelVolumeToCostConversionFuelOil->setCoefficientOfVariation("0.2");

    theDomain->createObject(objectName() + "FuelVolumeToCostConversionElectricity", "RContinuousRandomVariable");
    theFuelVolumeToCostConversionElectricity = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theFuelVolumeToCostConversionElectricity->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theFuelVolumeToCostConversionElectricity->setMean(QString("40.05"));
    theFuelVolumeToCostConversionElectricity->setCoefficientOfVariation("0.2");

    theDomain->createObject(objectName() + "FuelVolumeToCostConversionLPG", "RContinuousRandomVariable");
    theFuelVolumeToCostConversionLPG = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theFuelVolumeToCostConversionLPG->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theFuelVolumeToCostConversionLPG->setMean(QString("0.90351"));
    theFuelVolumeToCostConversionLPG->setCoefficientOfVariation("0.2");
}


RShindellEnvironmentalCostOfEnergyModel::~RShindellEnvironmentalCostOfEnergyModel()
{

}


QObject * RShindellEnvironmentalCostOfEnergyModel::getBIM() const
{
    return theBIM;
}


void RShindellEnvironmentalCostOfEnergyModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


QString RShindellEnvironmentalCostOfEnergyModel::getJouleCoalList()
{
    return parameterListToString(theJouleCoalList);
}


void RShindellEnvironmentalCostOfEnergyModel::setJouleCoalList(QString value)
{
    theJouleCoalList = stringToParameterList(value);
}


QString RShindellEnvironmentalCostOfEnergyModel::getJouleDieselList()
{
    return parameterListToString(theJouleDieselList);
}


void RShindellEnvironmentalCostOfEnergyModel::setJouleDieselList(QString value)
{
    theJouleDieselList = stringToParameterList(value);
}


QString RShindellEnvironmentalCostOfEnergyModel::getJouleElectricityList()
{
    return parameterListToString(theJouleElectricityList);
}


void RShindellEnvironmentalCostOfEnergyModel::setJouleElectricityList(QString value)
{
    theJouleElectricityList = stringToParameterList(value);
}


QString RShindellEnvironmentalCostOfEnergyModel::getJouleFuelOilList()
{
    return parameterListToString(theJouleFuelOilList);
}


void RShindellEnvironmentalCostOfEnergyModel::setJouleFuelOilList(QString value)
{
    theJouleFuelOilList = stringToParameterList(value);
}


QString RShindellEnvironmentalCostOfEnergyModel::getJouleGasolineList()
{
    return parameterListToString(theJouleGasolineList);
}


void RShindellEnvironmentalCostOfEnergyModel::setJouleGasolineList(QString value)
{
    theJouleGasolineList = stringToParameterList(value);
}


QString RShindellEnvironmentalCostOfEnergyModel::getJouleLPGList()
{
    return parameterListToString(theJouleLPGList);
}


void RShindellEnvironmentalCostOfEnergyModel::setJouleLPGList(QString value)
{
    theJouleLPGList = stringToParameterList(value);
}


QString RShindellEnvironmentalCostOfEnergyModel::getJouleNaturalGasList()
{
    return parameterListToString(theJouleNaturalGasList);
}


void RShindellEnvironmentalCostOfEnergyModel::setJouleNaturalGasList(QString value)
{
    theJouleNaturalGasList = stringToParameterList(value);
}


int RShindellEnvironmentalCostOfEnergyModel::evaluateModel(RGradientType theGradientType)
{
    // Make the numbers location dependent (will affect conversions in constructor)
    RLocation *theLocation = qobject_cast<RLocation *>(theBIM->getLocation());

    // Energy in Joule per Litre or Joule per kWh for different energy sources
    double energyInCoal        = 1.0;
    double energyInDiesel      = 35800000;
    double energyInElectricity = 1000000000;
    double energyInFuelOil     = 34800000;
    double energyInGasoline    = 34200000;
    double energyInLPG         = 26000000;
    double energyInNaturalGas  = 1.0;

    double result = 0.0;
    for (int i=0; i<theJouleCoalList.size(); i++)
    {
        result += theJouleCoalList.at(i)->getCurrentValue()/ energyInCoal  * theFuelVolumeToCostConversionCoal->getCurrentValue();
    }
    for (int i=0; i<theJouleDieselList.size(); i++)
    {
        result += theJouleDieselList.at(i)->getCurrentValue()/ energyInDiesel  * theFuelVolumeToCostConversionDiesel->getCurrentValue();
    }

    for (int i=0; i<theJouleElectricityList.size(); i++)
    {
        result += theJouleElectricityList.at(i)->getCurrentValue()/ energyInElectricity  * theFuelVolumeToCostConversionElectricity->getCurrentValue();
    }
    for (int i=0; i<theJouleFuelOilList.size(); i++)
    {
        result += theJouleFuelOilList.at(i)->getCurrentValue()/ energyInFuelOil  * theFuelVolumeToCostConversionFuelOil->getCurrentValue();
    }
    for (int i=0; i<theJouleGasolineList.size(); i++)
    {
        result += theJouleGasolineList.at(i)->getCurrentValue()/ energyInGasoline  * theFuelVolumeToCostConversionGasoline->getCurrentValue();
    }
    for (int i=0; i<theJouleLPGList.size(); i++)
    {
        result += theJouleLPGList.at(i)->getCurrentValue()/ energyInLPG  * theFuelVolumeToCostConversionLPG->getCurrentValue();
    }
    for (int i=0; i<theJouleNaturalGasList.size(); i++)
    {
        result += theJouleNaturalGasList.at(i)->getCurrentValue()/ energyInNaturalGas  * theFuelVolumeToCostConversionNaturalGas->getCurrentValue();
    }
    theResponse->setCurrentValue(result);

    return 0;
}

