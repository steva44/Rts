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
*   - Mojtaba Mahsuli                                                *
*                                                                    *
*********************************************************************/

#include "RGivenMagnitudeModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include <math.h>

RGivenMagnitudeModel::RGivenMagnitudeModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theMagnitudeParameter = nullptr;
    theOccurrence = nullptr;
    theMagnitudeResponse = nullptr;
    theMagnitudeDouble = 0.0;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theMagnitudeResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theMagnitudeResponse->setModel(this);
}


RGivenMagnitudeModel::~RGivenMagnitudeModel()
{

}


QString RGivenMagnitudeModel::getMagnitude()
{
    return getStringParameter(&theMagnitudeDouble, theMagnitudeParameter);
}


void RGivenMagnitudeModel::setMagnitude(QString value)
{
    setStringParameter(value, &theMagnitudeDouble, &theMagnitudeParameter);
}


double RGivenMagnitudeModel::getMagnitudeValue()
{
    return getDoubleParameter(&theMagnitudeDouble, theMagnitudeParameter);
}


QObject * RGivenMagnitudeModel::getOccurrence() const
{
    return theOccurrence;
}


void RGivenMagnitudeModel::setOccurrence(QObject *value)
{
    theOccurrence = qobject_cast<RParameter *>(value);
}


int RGivenMagnitudeModel::evaluateModel(RGradientType theGradientType)
{
    if (theOccurrence->getCurrentValue() == 0.0) {

        theMagnitudeResponse->setCurrentValue(0.0);

        return 0;
    }

    theMagnitudeResponse->setCurrentValue(getMagnitudeValue());

    return 1;
}
