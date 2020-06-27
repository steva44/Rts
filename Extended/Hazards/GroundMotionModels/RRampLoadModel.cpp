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

#include "RRampLoadModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RFileResponse.h"
#include "RCommandResponse.h"
#include "RTimeSeries.h"
#include "RDomain.h"

#include <QDir>
#include <QApplication>

RRampLoadModel::RRampLoadModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    // Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RTimeSeriesResponse");
    theTimeSeriesResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theTimeSeriesResponse->setModel(this);

    theRampStartTimeParameter = 0;
    theRampStartTimeDouble = 0.0;

    theRampEndTimeParameter = 0;
    theRampEndTimeDouble = 0.0;
}


RRampLoadModel::~RRampLoadModel()
{

}


QObject * RRampLoadModel::getLoadIntensity() const
{
    return theLoadIntensity;
}


void RRampLoadModel::setLoadIntensity(QObject *value)
{
    theLoadIntensity = qobject_cast<RParameter *>(value);
}


QString RRampLoadModel::getRampStartTime()
{
    return getStringParameter(&theRampStartTimeDouble, theRampStartTimeParameter);
}


void RRampLoadModel::setRampStartTime(QString value)
{
    setStringParameter(value, &theRampStartTimeDouble, &theRampStartTimeParameter);
}


double RRampLoadModel::getRampStartTimeValue()
{
    return getDoubleParameter(&theRampStartTimeDouble, theRampStartTimeParameter);
}


QString RRampLoadModel::getRampEndTime()
{
    return getStringParameter(&theRampEndTimeDouble, theRampEndTimeParameter);
}


void RRampLoadModel::setRampEndTime(QString value)
{
    setStringParameter(value, &theRampEndTimeDouble, &theRampEndTimeParameter);
}


double RRampLoadModel::getRampEndTimeValue()
{
    return getDoubleParameter(&theRampEndTimeDouble, theRampEndTimeParameter);
}


int RRampLoadModel::evaluateModel(RGradientType theGradientType)
{	
    // Get a pointer to the response time series, and clean it
    RTimeSeries *theOutputTimeSeries = parameterToTimeSeries(theTimeSeriesResponse);
    if (!theOutputTimeSeries) {
        qCritical() << "Error: The response object" << theTimeSeriesResponse->objectName() << "does not contain a times series object.";
        return -1;
    }
    theOutputTimeSeries->clean();


    // Set a time series that ramps up from zero to max  (for now apply the load in the x-direction)
    theOutputTimeSeries->setXYZvalues(getRampStartTimeValue(), 0.0, 0.0, 0.0);
    theOutputTimeSeries->setXYZvalues(getRampEndTimeValue(), theLoadIntensity->getCurrentValue(), 0.0, 0.0);

    return 0;
}




