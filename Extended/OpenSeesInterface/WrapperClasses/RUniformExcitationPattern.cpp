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

#include "RUniformExcitationPattern.h"
#include "ROpenSeesTimeSeries.h"

#include "TimeSeries.h"
#include "LoadPattern.h"
#include "Domain.h"
#include "UniformExcitation.h"
#include "GroundMotion.h"

RUniformExcitationPattern::RUniformExcitationPattern(QObject *parent, QString name)
    : RLoadPattern(parent, name)
{
    theGroundMotion = nullptr;
    theDispSeries  = nullptr;
    theVelSeries  = nullptr;
    theAccelSeries  = nullptr;
    theIntTimeSeries = nullptr;

    vel0 = 0.0;
    cFactor = 1.0;

    direction = -1;
}


RUniformExcitationPattern::~RUniformExcitationPattern()
{

}


int RUniformExcitationPattern::update(void)
{
    // Let OpenSees delete the members
    // delete theGroundMotion;
    // delete theLoadPattern;
    // theGroundMotion = nullptr;
    // theLoadPattern = nullptr;

    return this->createTheUniformExcitationPattern();
}


LoadPattern* RUniformExcitationPattern::getTheLoadPattern()
{

    if(!theLoadPattern)
    {
        this->createTheUniformExcitationPattern();
    }

    return theLoadPattern;
}


QObject *RUniformExcitationPattern::getDispSeries() const
{
    return theDispSeries;
}


void RUniformExcitationPattern::setDispSeries(QObject *value)
{
    theDispSeries = qobject_cast<ROpenSeesTimeSeries*>(value);
}


QObject *RUniformExcitationPattern::getVelSeries() const
{
    return theVelSeries;
}


void RUniformExcitationPattern::setVelSeries(QObject *value)
{
    theVelSeries = qobject_cast<ROpenSeesTimeSeries*>(value);
}


QObject *RUniformExcitationPattern::getAccelSeries() const
{
    return theAccelSeries;
}


void RUniformExcitationPattern::setAccelSeries(QObject *value)
{
    auto valCast = qobject_cast<ROpenSeesTimeSeries*>(value);
    theAccelSeries = valCast;
}

QObject *RUniformExcitationPattern::getIntTimeSeries() const
{
    return theIntTimeSeries;
}


void RUniformExcitationPattern::setIntTimeSeries(QObject *value)
{
    theIntTimeSeries = qobject_cast<ROpenSeesTimeSeries*>(value);
}


int RUniformExcitationPattern::getDirection() const
{
    return direction;
}


void RUniformExcitationPattern::setDirection(int value)
{
    direction = value;
}


int RUniformExcitationPattern::createTheUniformExcitationPattern(void)
{

    //check to make sure that at least on time series came in
    if(theDispSeries == nullptr && theVelSeries == nullptr && theAccelSeries == nullptr && theIntTimeSeries == nullptr)
    {
        qDebug()<<"Error: "<<this->objectName()<<"the load pattern requires a time series in "<<__FUNCTION__;
        return -1;
    }

    if(theIntTimeSeries != nullptr)
    {
        qDebug()<<"Error: "<<this->objectName()<<" missing functionality, incorporate time series integration in "<<__FUNCTION__;
        //A time series integrator is required for the ground motion
        //    TimeSeriesIntegrator *seriesIntegrator = nullptr;

        //    if (strcmp(argv[0],"Trapezoidal") == 0)
        //    {
        //        seriesIntegrator = new TrapezoidalTimeSeriesIntegrator();
        //    }
        //    else if (strcmp(argv[0],"Simpson") == 0)
        //    {
        //        seriesIntegrator = new SimpsonTimeSeriesIntegrator();
        //    }
        return -1;
    }

    //Check that the direction was given
    if(direction < 1 || direction > 6)
    {
        qDebug()<<"Error: "<<this->objectName()<<" a direction needs to be given or invalid direction "<<__FUNCTION__;
        return -1;
    }

    TimeSeries *dispSeries = nullptr;
    TimeSeries *velSeries = nullptr;
    TimeSeries *accelSeries = nullptr;

    if(theAccelSeries)
    {
        theTimeSeries = theAccelSeries;
        accelSeries = theAccelSeries->getTheTimeSeries();
        if(accelSeries == nullptr)
        {
            qDebug()<<"Error: "<<theAccelSeries->objectName()<<" failed to initialize a time series in "<<__FUNCTION__;
            return -1;
        }
    }
    else if(theDispSeries)
    {
        theTimeSeries = theDispSeries;
        dispSeries = theDispSeries->getTheTimeSeries();
        if(dispSeries == nullptr)
        {
            qDebug()<<"Error: "<<theDispSeries->objectName()<<" failed to initialize a time series in "<<__FUNCTION__;
            return -1;
        }
    }
    else if(theVelSeries)
    {
        theTimeSeries = theVelSeries;
        velSeries = theVelSeries->getTheTimeSeries();
        if(velSeries == nullptr)
        {
            qDebug()<<"Error: "<<theVelSeries->objectName()<<" failed to initialize a time series in "<<__FUNCTION__;
            return -1;
        }
    }
    else
        return -1; //Something went wrong


    // Create a new ground motion - ignore the integrator series for now
    // GroundMotion(TimeSeries *dispSeries,
    // TimeSeries *velSeries,
    // TimeSeries *accelSeries,
    // TimeSeriesIntegrator *theIntegrator = 0,
    // double dTintegration = 0.01, double fact = 1.0);
    theGroundMotion = new GroundMotion(dispSeries, velSeries, accelSeries,nullptr,0.01,cFactor);

    //check to see that a new groundmotion was created
    if(theGroundMotion == nullptr)
    {
        qDebug()<<"Error: "<<this->objectName()<<" failed to create a ground motion in "<<__FUNCTION__;
        return -1;
    }

    // Now create the load pattern
    // UniformExcitation(GroundMotion &theMotion,
    // int dof, int tag, double vel0 = 0.0, double fact = 1.0);
    theLoadPattern = new UniformExcitation(*theGroundMotion,direction-1, objectTag, vel0);

    return 0;
}


double RUniformExcitationPattern::getCFactor() const
{
    return cFactor;
}


void RUniformExcitationPattern::setCFactor(double value)
{
    cFactor = value;
}


double RUniformExcitationPattern::getVel0() const
{
    return vel0;
}


void RUniformExcitationPattern::setVel0(double value)
{
    vel0 = value;
}


double RUniformExcitationPattern::getDuration(void)
{
    if(theGroundMotion)
        return theGroundMotion->getDuration();

    return 0.0;
}




