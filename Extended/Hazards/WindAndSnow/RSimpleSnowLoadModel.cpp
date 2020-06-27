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

#include "RSimpleSnowLoadModel.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RTime.h"
#include "RDomain.h"

#include <math.h>

RSimpleSnowLoadModel::RSimpleSnowLoadModel(QObject *parent, QString name)
    : RSnowLoadModel(parent, name)
{
    theSnowStormMagnitude = nullptr;

    // Density in kg/m^3
    theDomain->createObject(objectName() + "theSnowDensity", "RContinuousRandomVariable");
    theSnowDensity = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theSnowDensity->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theSnowDensity->setMean(QString("800.0"));
    theSnowDensity->setCoefficientOfVariation("0.2");
}


RSimpleSnowLoadModel::~RSimpleSnowLoadModel()
{

}


QObject * RSimpleSnowLoadModel::getSnowStormMagnitude() const
{
    return theSnowStormMagnitude;
}


void RSimpleSnowLoadModel::setSnowStormMagnitude(QObject *value)
{
    theSnowStormMagnitude = qobject_cast<RParameter *>(value);
}


bool RSimpleSnowLoadModel::isOn()
{
    double M = theSnowStormMagnitude->getCurrentValue();

    if (M != 0.0) {
        return true;
    }
    else {
        return false;
    }
}


int RSimpleSnowLoadModel::evaluateModel(RGradientType theGradientType)
{
    // Exit quickly if this hazard isn't on
    if (!isOn()) {
        theResponse->setCurrentValue(0.0);
        return 0;
    }

    double depth = theSnowStormMagnitude->getCurrentValue();

    double density = theSnowDensity->getCurrentValue();

    double load = depth * density*9.81;  // in N/m2 or Pa

    theResponse->setCurrentValue(load);

    if (theOutputLevel >= RObject::Medium) {

        qDebug() << "An snowstorm just happened with magnitude" << load << " at time " << theCurrentTime->getTime();
    }

    if (theGradientType >= RGradientType::DDM)
    {
        qDebug() << "Quickly implement DDM in" << this->objectName() << "here... :)";
    }

    return 0;
}

