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

#include "RUniformOccurrenceModel.h"
#include "RParameter.h"
#include "RTimeSeries.h"
#include "RResponse.h"
#include "RDomain.h"

#include <math.h>
#include <random>

RUniformOccurrenceModel::RUniformOccurrenceModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    seed = 0;

    numberOfOccurrences = 1;

    theStartTime = nullptr;
    theEndTime = nullptr;

    // Instantiating the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}

RUniformOccurrenceModel::~RUniformOccurrenceModel()
{

}


QObject * RUniformOccurrenceModel::getStartTime() const
{
    return theStartTime;
}


void RUniformOccurrenceModel::setStartTime(QObject *value)
{
    theStartTime = qobject_cast<RParameter *>(value);
    resetTime();
}


void RUniformOccurrenceModel::setNumberOfOccurrences(QString value)
{
    bool OK = true;
    numberOfOccurrences = value.toInt(&OK);

    if(!OK)
    {
        qCritical()<<"Error setting the number of occurrences in "<<this->objectName();
    }
}


int RUniformOccurrenceModel::getNumberOfOccurrences() const
{
    return numberOfOccurrences;
}


QObject * RUniformOccurrenceModel::getEndTime() const
{
    return theEndTime;
}


void RUniformOccurrenceModel::setEndTime(QObject *value)
{
    theEndTime = qobject_cast<RParameter *>(value);
    resetTime();
}


QList<QPair<double, QString> > RUniformOccurrenceModel::getTriggerTimes(double startTime, double endTime)
{
    return occurrenceTimes;
}


int RUniformOccurrenceModel::evaluateModel(RGradientType theGradientType)
{	
    // If any of the occurrence times matches the current time, then say the hazard is "on"
    for (int i=0; i<occurrenceTimes.size(); i++) {

        if (fabs(occurrenceTimes.at(i).first - theCurrentTime->getCurrentValue()) < 1.0/(24.0*60.0)) {

            theResponse->setCurrentValue(1.0);

            return 0;
        }
    }

    // No match, so hazard is "off"
    theResponse->setCurrentValue(0.0);

    return 0;
}


int RUniformOccurrenceModel::resetTime()
{
    // Clear whatever was set before
    occurrenceTimes.clear();

    // Only proceed if we have start time, end time
    if (theStartTime == nullptr || theEndTime == nullptr )
        return -1;


    // Get start time and end time as doubles rather than time objects
    double startTime = theStartTime->getCurrentValue();
    double endTime = theEndTime->getCurrentValue();

    if (endTime == 0.0 && startTime == 0.0)
        return -1;

    // Only proceed if end time is greater than start time
    if (endTime <= startTime)
    {
        qCritical()<<"The end time needs to be greater than the start time in "<<this->objectName();
        return -1;
    }

    // Initial declarations
    QString thisEventName = this->objectName();

    // Set seed for random number generator, incremented by one, in perpetuity, and generate & discard one number
    srand(seed++);
    rand();

    std::mt19937 gen(rand()); //Standard mersenne_twister_engine seeded with rand()
    std::uniform_real_distribution<> distribution(startTime, endTime);

    for(int i = 0; i<numberOfOccurrences; ++i)
    {
        //Generate a time between the start and end time
        auto genTime = distribution(gen);

        // Append it to the list of trigger times
        occurrenceTimes.append(qMakePair(genTime, thisEventName));

    }


    return 0;
}
