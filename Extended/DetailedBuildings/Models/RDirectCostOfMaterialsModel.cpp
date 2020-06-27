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

#include "RDirectCostOfMaterialsModel.h"
#include "RContinuousRandomVariable.h"
#include "RComponent.h"
#include "RParameter.h"
#include <QDateTime>

RDirectCostOfMaterialsModel::RDirectCostOfMaterialsModel(QObject *parent, QString name)
    : RModel(parent, name)
{

    theResponse = nullptr;
    theBIM = nullptr;
    theRandomness = nullptr;

    priceSteel = 8452;
    priceTimber = 370;
    priceConcreteM = 0.8338;
    priceConcreteB = 142.9;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);


    theDomain->createObject(objectName() + "RandomVariable", "RContinuousRandomVariable");
    theRandomness = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRandomness->setDistributionType(QString("Normal (mean, stdv)"));
    theRandomness->setMean(QString("1"));
    theRandomness->setCoefficientOfVariation("0.2");
}


RDirectCostOfMaterialsModel::~RDirectCostOfMaterialsModel()
{

}


QObject * RDirectCostOfMaterialsModel::getBIM() const
{
    return theBIM;
}

void RDirectCostOfMaterialsModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


int RDirectCostOfMaterialsModel::evaluateModel(RGradientType theGradientType)
{
    double cost = 0;

    // Just get volumes from BIM and calculate costs
    cost += theBIM->getVolumeOfWood()*priceTimber;
    cost += theBIM->getVolumeOfSteel()*priceSteel;
    cost += theBIM->getVolumeOfConcrete()*getPriceOfConcrete("C20");

    // Add 10% waste factor and general randomness variable
    theResponse->setCurrentValue(1.1*cost*theRandomness->getCurrentValue());
    return 0;
}


double RDirectCostOfMaterialsModel::getPriceOfConcrete(QString theConcreteStrength)
{
    double fc = 20.0;
    if (theConcreteStrength == "C20")
        fc = 20.0;
    else if (theConcreteStrength == "C25")
        fc = 25.0;
    else if (theConcreteStrength == "C30")
        fc = 30.0;

    return priceConcreteM * fc + priceConcreteB;
}
