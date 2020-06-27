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

#include "RSineLoadModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RDomain.h"
#include "RFileResponse.h"
#include "RCommandResponse.h"
#include "RTimeSeries.h"

#include <QDir>
#include <QApplication>
#include <math.h>

RSineLoadModel::RSineLoadModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    // Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RTimeSeriesResponse");
    theTimeSeriesResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theTimeSeriesResponse->setModel(this);

    theStartTimeParameter = nullptr;
    theStartTimeDouble = 0.0;

    theEndTimeParameter = nullptr;
    theEndTimeDouble = 0.0;

    thePeriodParameter = nullptr;
    thePeriodDouble = 0.0;

    theLoadAmplitudeParameter = nullptr;
    theLoadAmplitudeDouble = 0.0;

}


RSineLoadModel::~RSineLoadModel()
{

}


QString RSineLoadModel::getLoadAmplitude()
{
    return getStringParameter(&theLoadAmplitudeDouble, theLoadAmplitudeParameter);
}


void RSineLoadModel::setLoadAmplitude(QString value)
{
    setStringParameter(value, &theLoadAmplitudeDouble, &theLoadAmplitudeParameter);
}


double RSineLoadModel::getLoadAmplitudeValue()
{
    return getDoubleParameter(&theLoadAmplitudeDouble, theLoadAmplitudeParameter);
}


QString RSineLoadModel::getStartTime()
{
    return getStringParameter(&theStartTimeDouble, theStartTimeParameter);
}


void RSineLoadModel::setStartTime(QString value)
{
    setStringParameter(value, &theStartTimeDouble, &theStartTimeParameter);
}


double RSineLoadModel::getStartTimeValue()
{
    return getDoubleParameter(&theStartTimeDouble, theStartTimeParameter);
}


QString RSineLoadModel::getEndTime()
{
    return getStringParameter(&theEndTimeDouble, theEndTimeParameter);
}


void RSineLoadModel::setEndTime(QString value)
{
    setStringParameter(value, &theEndTimeDouble, &theEndTimeParameter);
}


double RSineLoadModel::getEndTimeValue()
{
    return getDoubleParameter(&theEndTimeDouble, theEndTimeParameter);
}


QString RSineLoadModel::getPeriod()
{
    return getStringParameter(&thePeriodDouble, thePeriodParameter);
}


void RSineLoadModel::setPeriod(QString value)
{
    setStringParameter(value, &thePeriodDouble, &thePeriodParameter);
}


double RSineLoadModel::getPeriodValue()
{
    return getDoubleParameter(&thePeriodDouble, thePeriodParameter);
}


QStringList RSineLoadModel::getActionList()
{
    // Remember that the "run" and "help" methods are implemented in base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Help";
    return actionList;
}


int RSineLoadModel::evaluateModel(RGradientType theGradientType)
{	
    // Get a pointer to the response time series, and clean it
    RTimeSeries *theOutputTimeSeries = parameterToTimeSeries(theTimeSeriesResponse);
    if (!theOutputTimeSeries) {
        qCritical() << "Error: The response object" << theTimeSeriesResponse->objectName() << "does not contain a times series object.";
        return -1;
    }
    theOutputTimeSeries->clean();

    double dt = getPeriodValue() / 50.0;

    for (double time = getStartTimeValue(); time<=getEndTimeValue()+dt; time = time + dt) {

        // Set a time series that ramps up from zero to max  (for now apply the load in the x-direction)
        theOutputTimeSeries->setXYZvalues(time, getLoadAmplitudeValue() * sin(2.0*PI/getPeriodValue()*time), 0.0, 0.0);
    }

    return 0;
}


