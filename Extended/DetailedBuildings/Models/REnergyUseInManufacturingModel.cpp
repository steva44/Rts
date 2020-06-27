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

#include "REnergyUseInManufacturingModel.h"
#include "RComponent.h"
#include "RLocation.h"

REnergyUseInManufacturingModel::REnergyUseInManufacturingModel(QObject *parent, QString name)
    : RModel(parent, name)
{

    theBIM = nullptr;
    theJouleCoalResponse = nullptr;
    theJouleDieselResponse = nullptr;
    theJouleElectricityResponse = nullptr;
    theJouleFuelOilResponse = nullptr;
    theJouleGasolineResponse = nullptr;
    theJouleLPGResponse = nullptr;
    theJouleNaturalGasResponse = nullptr;
    theManufacturingIntensityConcrete = nullptr;
    theManufacturingIntensitySteel = nullptr;
    theManufacturingIntensityWood = nullptr;

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


    theDomain->createObject(objectName() + "ManufacturingIntensityConcrete", "RContinuousRandomVariable");
    theManufacturingIntensityConcrete = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theManufacturingIntensityConcrete->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theManufacturingIntensityConcrete->setMean(QString("950000"));
    theManufacturingIntensityConcrete->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "ManufacturingIntensitySteel", "RContinuousRandomVariable");
    theManufacturingIntensitySteel = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theManufacturingIntensitySteel->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theManufacturingIntensitySteel->setMean(QString("36400000"));
    theManufacturingIntensitySteel->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "ManufacturingIntensityWood", "RContinuousRandomVariable");
    theManufacturingIntensityWood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theManufacturingIntensityWood->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theManufacturingIntensityWood->setMean(QString("8500000"));
    theManufacturingIntensityWood->setCoefficientOfVariation("0.1");
}


REnergyUseInManufacturingModel::~REnergyUseInManufacturingModel()
{

}


QObject * REnergyUseInManufacturingModel::getBIM() const
{
    return theBIM;
}


void REnergyUseInManufacturingModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


int REnergyUseInManufacturingModel::evaluateModel(RGradientType theGradientType)
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

    // Transform from volume to weight
    double densityConcrete = 2400.0;
    double densitySteel = 8050.0;
    double densityWood = 500.0;

    double weightConcrete = volumeConcrete * densityConcrete;
    double weightSteel = volumeSteel * densitySteel;
    double weightWood = volumeWood * densityWood;


    // Joule per by material
    double concreteJoule = weightConcrete * theManufacturingIntensityConcrete->getCurrentValue();
    double steelJoule = weightSteel * theManufacturingIntensitySteel->getCurrentValue();
    double woodJoule = weightWood * theManufacturingIntensityWood->getCurrentValue();


    // Fraction of different energy sources used (location dependent)
    RLocation *theLocation = qobject_cast<RLocation *>(theBIM->getLocation());

    // Concrete fractions
    double coalFractionForConcrete = 0.471;
    double electricityFractionForConcrete = 0.344;
    double fuelOilFractionForConcrete = 0.154;
    double naturalGasFractionForConcrete = 0.031;

    // Steel fractions
    double coalFractionForSteel = 0.17;
    double electricityFractionForSteel = 0.50;
    double naturalGasFractionForSteel = 0.33;

    // Wood fractions
    double electricityFractionForWood = 0.242;
    double fuelOilFractionForWood = 0.728;
    double naturalGasFractionForWood = 0.03;


    // Joule per energy source
    theJouleCoalResponse->setCurrentValue(concreteJoule * coalFractionForConcrete + steelJoule * coalFractionForSteel);
    theJouleDieselResponse->setCurrentValue(0.0);
    theJouleElectricityResponse->setCurrentValue(concreteJoule * electricityFractionForConcrete + steelJoule * electricityFractionForSteel + woodJoule * electricityFractionForWood);
    theJouleFuelOilResponse->setCurrentValue(concreteJoule * fuelOilFractionForConcrete + woodJoule * fuelOilFractionForWood);
    theJouleGasolineResponse->setCurrentValue(0.0);
    theJouleLPGResponse->setCurrentValue(0.0);
    theJouleNaturalGasResponse->setCurrentValue(concreteJoule * naturalGasFractionForConcrete + steelJoule * naturalGasFractionForSteel + woodJoule * naturalGasFractionForWood);


    return 0;
}
