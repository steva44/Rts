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

#include "RLognormalOccurrenceModel.h"
#include "RParameter.h"
#include "RTimeSeries.h"
#include "RResponse.h"
#include "RDomain.h"

#include <math.h>
#include <random>

RLognormalOccurrenceModel::RLognormalOccurrenceModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    seed = 0;
    format = "yyyy-MM-dd hh:mm:ss.zzz";

    Mean = 0.0;
    StdDeviation = 0.0;

    theStartTime = nullptr;
    theEndTime = nullptr;

    // Instantiating the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RLognormalOccurrenceModel::~RLognormalOccurrenceModel()
{

}


QObject * RLognormalOccurrenceModel::getStartTime() const
{
    return theStartTime;
}


void RLognormalOccurrenceModel::setStartTime(QObject *value)
{
    theStartTime = qobject_cast<RParameter *>(value);
    resetTime();
}


void RLognormalOccurrenceModel::setMean(QString value)
{
    bool OK = true;
    Mean = value.toDouble(&OK);

    if(!OK)
    {
        qCritical()<<"Error setting the log-mean in "<<this->objectName();
    }
}


double RLognormalOccurrenceModel::getMean() const
{
    return Mean;
}


void RLognormalOccurrenceModel::setStandardDeviation(QString value)
{
    bool OK = true;
    StdDeviation = value.toDouble(&OK);

    if(!OK)
    {
        qCritical()<<"Error setting the log-deviation in "<<this->objectName();
    }
}


double RLognormalOccurrenceModel::getStandardDeviation() const
{
    return StdDeviation;
}


QObject * RLognormalOccurrenceModel::getEndTime() const
{
    return theEndTime;
}


void RLognormalOccurrenceModel::setEndTime(QObject *value)
{
    theEndTime = qobject_cast<RParameter *>(value);
    resetTime();
}


QString RLognormalOccurrenceModel::getPrevOccTime() const
{
    return thePrevOccTime.toString(format);
}


void RLognormalOccurrenceModel::setPrevOccTime(QString value)
{

    thePrevOccTime = QDateTime().fromString(value, format);

    resetTime();
}


QList<QPair<double, QString> > RLognormalOccurrenceModel::getTriggerTimes(double startTime, double endTime)
{
    return occurrenceTimes;
}


int RLognormalOccurrenceModel::evaluateModel(RGradientType theGradientType)
{	
    theResponse->setCurrentValue(0.0);

    // If any of the occurrence times matches the current time, then say the hazard is "on"
    for (int i=0; i<occurrenceTimes.size(); ++i)
    {
        if (fabs(occurrenceTimes.at(i).first - theCurrentTime->getCurrentValue()) < 1.0/(24.0*60.0))
        {
            theResponse->setCurrentValue(1.0);

            return 0;
        }
    }

    return 0;
}


int RLognormalOccurrenceModel::resetTime()
{
    // Clear whatever was set before
    occurrenceTimes.clear();

    // Only proceed if we have start time, end time
    if (theStartTime == nullptr || theEndTime == nullptr || thePrevOccTime.isNull() )
        return -1;

    // Get start time and end time as doubles rather than time objects
    double startTime = theStartTime->getCurrentValue();
    double endTime = theEndTime->getCurrentValue();

    auto constTime = QDateTime(QDate(1900, 1, 1), QTime(0, 0, 0, 0));
    auto theReferenceTime = QDateTime(constTime);

    //Get the double value of the reference time in days
    double prevOccTime = theReferenceTime.daysTo(thePrevOccTime);
    prevOccTime += theReferenceTime.time().msecsTo(thePrevOccTime.time()) / 86400000.0;

    if (endTime == 0.0 && startTime == 0.0)
        return -1;

    // Only proceed if end time is greater than start time
    if (endTime <= startTime)
    {
        qCritical()<<"The end time needs to be greater than the start time in "<<this->objectName();
        return -1;
    }

    if (prevOccTime >= startTime)
    {
        qCritical()<<"The previous occurence time has to occur before the start time "<<this->objectName();
        return -1;
    }

    // Initial declarations
    QString thisEventName = this->objectName();

    // Set seed for random number generator
    seed = static_cast<unsigned int>(time(nullptr));
    srand(seed);
    rand();

    std::mt19937 gen(static_cast<unsigned int>(rand()));

    double logMean = log(Mean)-0.5*log(1.0+(StdDeviation/Mean)*(StdDeviation/Mean));
    double logDeviation = sqrt(log((StdDeviation / Mean)*(StdDeviation / Mean) + 1.0));

    std::lognormal_distribution<> logNormDist(logMean, logDeviation);

    auto aGeneratedTime = prevOccTime;

    while (aGeneratedTime <= endTime) {

        //Calculate the new occurence time, will give an occurence in years, convert to days
        auto newOcc = logNormDist(gen)*365.0;

        // Set the newly generated time
        aGeneratedTime += newOcc;

        // Append it to the list of trigger times, unless it is greater than the end time or happened before the building was built!
        if (aGeneratedTime > startTime && aGeneratedTime < endTime) {
            occurrenceTimes.append(qMakePair(aGeneratedTime, thisEventName));
        }
    }


    return 0;
}
