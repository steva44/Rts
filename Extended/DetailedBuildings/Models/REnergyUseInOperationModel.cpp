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
*   - Gurvinder Gill                                                 *
*                                                                    *
*********************************************************************/

#include "REnergyUseInOperationModel.h"
#include "RComponent.h"
#include "RLocation.h"
#include "RContinuousRandomVariable.h"

REnergyUseInOperationModel::REnergyUseInOperationModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = nullptr;
    theJouleElectricityResponse = nullptr;
    theJouleNaturalGasResponse = nullptr;
    theResponse = nullptr;
    theDailyOperationElectricityUsage = nullptr;

    theDomain->createObject(objectName() + "DailyOperationElectricityUsage", "RContinuousRandomVariable");
    theDailyOperationElectricityUsage = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theDailyOperationElectricityUsage->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theDailyOperationElectricityUsage->setMean(QString("27200000"));
    theDailyOperationElectricityUsage->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "JouleElectricityResponse", "RGenericResponse");
    theJouleElectricityResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleElectricityResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleNaturalGasResponse", "RGenericResponse");
    theJouleNaturalGasResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleNaturalGasResponse->setModel(this);

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    thePreviousTime = 0.0;

    // Costs are from BC Hydro and FortisBC, per kWh and GJ respectively
    costOfElectricity = 12.43 / 100 * 277.8 / 1000000000;
    costOfNaturalGas = 6.662 / 1000000000;

    // Derived from BCEmissions; electricity and natural gas are the only significant contributors
    fuelFractionResidentialElectricity = 0.383;
    fuelFractionResidentialNaturalGas = 0.617;
    fuelFractionCommercialElectricity = 0.511;
    fuelFractionCommercialNaturalGas = 0.489;
}


REnergyUseInOperationModel::~REnergyUseInOperationModel()
{

}


QObject * REnergyUseInOperationModel::getBIM() const
{
    return theBIM;
}


void REnergyUseInOperationModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


double REnergyUseInOperationModel::getCostOfElectricity()
{
    return costOfElectricity;
}


void REnergyUseInOperationModel::setCostOfElectricity(double value)
{
    costOfElectricity = value;
}


double REnergyUseInOperationModel::getCostOfNaturalGas()
{
    return costOfNaturalGas;
}


void REnergyUseInOperationModel::setCostOfNaturalGas(double value)
{
    costOfNaturalGas = value;
}


int REnergyUseInOperationModel::evaluateModel(RGradientType theGradientType)
{
    // Check that we have a BIM
    if(theBIM == nullptr){
        qCritical() << this->objectName() << "needs a BIM";
        return 0;
    }

    if (!thePreviousTime == 0.0) {

        double timeSinceLastAnalysis = theCurrentTime->getCurrentValue() - thePreviousTime;
        double dailyEnergyUse = theDailyOperationElectricityUsage->getCurrentValue();

        // Use this if we do NOT use eQuest, which provides energy sources:
        RBIM::ROccupancyType occupancyType = theBIM->getOccupancyType();
        if (occupancyType == RBIM::ROccupancyType::Services) {
            theResponse->setCurrentValue((fuelFractionResidentialElectricity * costOfElectricity +
                                          fuelFractionResidentialNaturalGas * costOfNaturalGas) * dailyEnergyUse * timeSinceLastAnalysis);
            theJouleElectricityResponse->setCurrentValue(fuelFractionResidentialElectricity * dailyEnergyUse * timeSinceLastAnalysis);
            theJouleNaturalGasResponse->setCurrentValue(fuelFractionResidentialNaturalGas * dailyEnergyUse * timeSinceLastAnalysis);
        }
        else if (occupancyType == RBIM::ROccupancyType::MultiResidence) {
            theResponse->setCurrentValue((fuelFractionCommercialElectricity * costOfElectricity +
                                          fuelFractionCommercialNaturalGas * costOfNaturalGas) * dailyEnergyUse * timeSinceLastAnalysis);
            theJouleElectricityResponse->setCurrentValue(fuelFractionCommercialElectricity * dailyEnergyUse * timeSinceLastAnalysis);
            theJouleNaturalGasResponse->setCurrentValue(fuelFractionCommercialNaturalGas * dailyEnergyUse * timeSinceLastAnalysis);
        }
        else {
            qDebug() << this->objectName() << " is missing data for this occupancy type";
        }
    }

    else {
        theResponse->setCurrentValue(0.0);
        theJouleElectricityResponse->setCurrentValue(0.0);
        theJouleNaturalGasResponse->setCurrentValue(0.0);

    }
    thePreviousTime = theCurrentTime->getCurrentValue();
    return 0;
}


int REnergyUseInOperationModel::resetTime()
{
    thePreviousTime = 0.0;

    return 0;
}
