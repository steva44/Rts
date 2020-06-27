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

#include "RFixedTimeOccurrenceModel.h"
#include "RParameter.h"
#include "RTimeSeries.h"
#include "RResponse.h"
#include "RDomain.h"

#include <math.h>

RFixedTimeOccurrenceModel::RFixedTimeOccurrenceModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theOccurrenceTime = nullptr;

    // Instantiating the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RFixedTimeOccurrenceModel::~RFixedTimeOccurrenceModel()
{

}


QObject * RFixedTimeOccurrenceModel::getOccurrenceTime() const
{
    return theOccurrenceTime;
}


void RFixedTimeOccurrenceModel::setOccurrenceTime(QObject *value)
{
    theOccurrenceTime = qobject_cast<RTime *>(value);


    if (!theOccurrenceTime) {
        qCritical() << "These days the occurrence model in" << this->objectName() << "must be an RTime object.";
    }
}


QList<QPair<double, QString> > RFixedTimeOccurrenceModel::getTriggerTimes(double startTime, double endTime)
{
    // Here we don't have to return anything, because there exist a time object that will be picked up instead

    QList<QPair<double, QString> > occurrenceTimes;

    return occurrenceTimes;
}


int RFixedTimeOccurrenceModel::evaluateModel(RGradientType theGradientType)
{	
    double occurrenceTime = theOccurrenceTime->getCurrentValue();
    double currentTime = theCurrentTime->getCurrentValue();

    if (qAbs(occurrenceTime-currentTime) < 1e-6) {

        theResponse->setCurrentValue(1.0);
    }
    else {

        theResponse->setCurrentValue(0.0);
    }

    return 1;
}

