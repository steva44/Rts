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

#include "REnergyUseInTransportationModel.h"
#include "RComponent.h"
#include "RLocation.h"
#include "RContinuousRandomVariable.h"

REnergyUseInTransportationModel::REnergyUseInTransportationModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = nullptr;
    theTotalWorkerHours = nullptr;
    theHoursPerShift = nullptr;

    theConcreteSource = nullptr;
    theSteelSource = nullptr;
    theTimberSource = nullptr;

    theJoulePerMetreCar = nullptr;
    theJoulePerMetreBus = nullptr;
    theJoulePerMetreSkytrain = nullptr;
    theJoulePerMetreHeavyTruck = nullptr;

    theJouleCoalResponse = nullptr;
    theJouleDieselResponse = nullptr;
    theJouleElectricityResponse = nullptr;
    theJouleFuelOilResponse = nullptr;
    theJouleGasolineResponse = nullptr;
    theJouleLPGResponse = nullptr;
    theJouleNaturalGasResponse = nullptr;

    theDomain->createObject(objectName() + "JouleCoalResponse", "RGenericResponse");
    theJouleCoalResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleCoalResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleDieselResponse", "RGenericResponse");
    theJouleDieselResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleDieselResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleElectricityResponse", "RGenericResponse");
    theJouleElectricityResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleElectricityResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleFuelOilResponse", "RGenericResponse");
    theJouleFuelOilResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleFuelOilResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleGasolineResponse", "RGenericResponse");
    theJouleGasolineResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleGasolineResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleLPGResponse", "RGenericResponse");
    theJouleLPGResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleLPGResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleNaturalGasResponse", "RGenericResponse");
    theJouleNaturalGasResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleNaturalGasResponse->setModel(this);


    theDomain->createObject(objectName() + "JoulePerMetreCar", "RContinuousRandomVariable");
    theJoulePerMetreCar = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theJoulePerMetreCar->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theJoulePerMetreCar->setMean(QString("2730.0"));
    theJoulePerMetreCar->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "JoulePerMetreBus", "RContinuousRandomVariable");
    theJoulePerMetreBus = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theJoulePerMetreBus->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theJoulePerMetreBus->setMean(QString("920.0"));
    theJoulePerMetreBus->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "JoulePerMetreSkytrain", "RContinuousRandomVariable");
    theJoulePerMetreSkytrain = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theJoulePerMetreSkytrain->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theJoulePerMetreSkytrain->setMean(QString("390.0"));
    theJoulePerMetreSkytrain->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "JoulePerMetreHeavyTruck", "RContinuousRandomVariable");
    theJoulePerMetreHeavyTruck = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theJoulePerMetreHeavyTruck->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theJoulePerMetreHeavyTruck->setMean(QString("1000.0"));
    theJoulePerMetreHeavyTruck->setCoefficientOfVariation("0.1");
}


REnergyUseInTransportationModel::~REnergyUseInTransportationModel()
{

}


QObject * REnergyUseInTransportationModel::getBIM() const
{
    return theBIM;
}


void REnergyUseInTransportationModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


RParameter * REnergyUseInTransportationModel::getTotalWorkerHours() const
{
    return theTotalWorkerHours;
}


void REnergyUseInTransportationModel::setTotalWorkerHours(QObject *value)
{
    theTotalWorkerHours = qobject_cast<RParameter *>(value);
}


RParameter * REnergyUseInTransportationModel::getHoursPerShift() const
{
    return theHoursPerShift;
}


void REnergyUseInTransportationModel::setHoursPerShift(QObject *value)
{
    theHoursPerShift = qobject_cast<RParameter *>(value);
}


RLocation * REnergyUseInTransportationModel::getConcreteSource() const
{
    return theConcreteSource;
}


void REnergyUseInTransportationModel::setConcreteSource(QObject *value)
{
    theConcreteSource = qobject_cast<RLocation *>(value);
}


RLocation * REnergyUseInTransportationModel::getSteelSource() const
{
    return theSteelSource;
}


void REnergyUseInTransportationModel::setSteelSource(QObject *value)
{
    theSteelSource = qobject_cast<RLocation *>(value);
}


RLocation * REnergyUseInTransportationModel::getTimberSource() const
{
    return theTimberSource;
}


void REnergyUseInTransportationModel::setTimberSource(QObject *value)
{
    theTimberSource = qobject_cast<RLocation *>(value);
}


int REnergyUseInTransportationModel::evaluateModel(RGradientType theGradientType)
{
    // Check that we have a BIM
    if(theBIM == nullptr){
        qCritical() << this->objectName() << "needs a BIM";
        return 0;
    }


    // Return quick if this isn't the time of manufacturing & construction
    if (theBIM->getTimeOfConstructionValue() != theCurrentTime->getCurrentValue()) {

        theJouleCoalResponse->setCurrentValue(0.0);
        theJouleDieselResponse->setCurrentValue(0.0);
        theJouleElectricityResponse->setCurrentValue(0.0);
        theJouleFuelOilResponse->setCurrentValue(0.0);
        theJouleGasolineResponse->setCurrentValue(0.0);
        theJouleLPGResponse->setCurrentValue(0.0);
        theJouleNaturalGasResponse->setCurrentValue(0.0);

        return 0;
    }

    // Cast BIM location
    RLocation *theBuildingLocation = qobject_cast<RLocation *>(theBIM->getLocation());

    // Here the modelling is about distances travelled (roundtrip) and the means of transportation
    double roundtripWorkerDistanceTravelled = 30000.0;
    double roundtripConcreteDistanceTravelled = 2*theBuildingLocation->computeSurfaceDistance(theConcreteSource) * 1000;
    double roundtripSteelDistanceTravelled = 2*theBuildingLocation->computeSurfaceDistance(theSteelSource) * 1000;
    double roundtripWoodDistanceTravelled = 2*theBuildingLocation->computeSurfaceDistance(theTimberSource) * 1000;


    // Total metres traveled by workers
    double totalWorkerMetresTravelled = theTotalWorkerHours->getCurrentValue() / theHoursPerShift->getCurrentValue() * roundtripWorkerDistanceTravelled;


    // Assume fractions of different transportation modes

    double workerFractionCar = 0.5;
    double workerFractionBus = 0.2;
    double workerFractionSkytrain = 0.3;


    // Loop over components to get material volumes
    double volumeConcrete = 0.0;
    double volumeSteel = 0.0;
    double volumeWood = 0.0;
    QList<RComponent *> theComponentList = theBIM->getComponentList();
    for (int i=0; i<theComponentList.size(); i++) {

        volumeConcrete += theComponentList[i]->getVolumeOfConcrete();
        volumeSteel += theComponentList[i]->getVolumeOfSteel();
        volumeWood += theComponentList[i]->getVolumeOfWood();
    }

    // Total metres traveled by materials (assuming m^3 volumes per truck)
    double totalConcreteMetresTravelled = volumeConcrete / 2.0 * roundtripConcreteDistanceTravelled;
    double totalSteelMetresTravelled = volumeSteel / 1.0 * roundtripSteelDistanceTravelled;
    double totalWoodMetresTravelled = volumeWood / 3.0 * roundtripWoodDistanceTravelled;
    double totalMaterialMetresTravelled = totalConcreteMetresTravelled + totalSteelMetresTravelled + totalWoodMetresTravelled;


    // Assume car uses gasoline, bus and heavy truck use diesel, and Skytrain use electricity
    theJouleCoalResponse->setCurrentValue(0.0);
    theJouleDieselResponse->setCurrentValue(totalWorkerMetresTravelled * workerFractionBus * theJoulePerMetreBus->getCurrentValue()
                                            + totalMaterialMetresTravelled * theJoulePerMetreHeavyTruck->getCurrentValue());
    theJouleElectricityResponse->setCurrentValue(totalWorkerMetresTravelled * workerFractionSkytrain * theJoulePerMetreSkytrain->getCurrentValue());
    theJouleFuelOilResponse->setCurrentValue(0.0);
    theJouleGasolineResponse->setCurrentValue(totalWorkerMetresTravelled * workerFractionCar * theJoulePerMetreCar->getCurrentValue());
    theJouleLPGResponse->setCurrentValue(0.0);
    theJouleNaturalGasResponse->setCurrentValue(0.0);


    return 0;
}
