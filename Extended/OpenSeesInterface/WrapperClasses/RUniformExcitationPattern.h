#ifndef RUniformExcitationPattern_H
#define RUniformExcitationPattern_H
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

// Wrapper class for OpenSees UniformExcitation
// Stevan Gavrilovic 12/2018

// $patternTag	unique tag among load patterns
// $dir	direction in which ground motion acts
// 1 - corresponds to translation along the global X axis
// 2 - corresponds to translation along the global Y axis
// 3 - corresponds to translation along the global Z axis
// 4 - corresponds to rotation about the global X axis
// 5 - corresponds to rotation about the global Y axis
// 6 - corresponds to rotation about the global Z axis
// $tsTag	tag of the TimeSeries series defining the acceleration history.
// $vel0	the initial velocity (optional, default=0.0)
// $cFactor	constant factor (optional, default=1.0)
// NOTES:
// The responses obtained from the nodes for this type of excitation are RELATIVE values, and not the absolute values obtained from a multi-support case.

// Note for tsTag, it is expected to be given something like this: -accel 2
// where you specify the type -disp {dispSeries} -vel {velSeries} -accel {accelSeries} -int {Series Integrator}
// followed by the tag for that type. Here it is done differently, there will be four QProperties of the time series type such as:
// 1) DispTimeSeries
// 2) VelTimeSeries
// 5) AccelTimeSeries
// 4) IntTimeSeries
// The proper time series should be given by the user and the rest should be null - the program will deduce the rest

#include "RLoadPattern.h"

class ROpenSeesTimeSeries;
class LoadPattern;
class UniformExcitation;
class GroundMotion;

class RUniformExcitationPattern : public RLoadPattern
{
    Q_OBJECT

    Q_PROPERTY(QObject *DispTimeSeries  READ getDispSeries      WRITE setDispSeries)
    Q_PROPERTY(QObject *VelTimeSeries   READ getVelSeries       WRITE setVelSeries)
    Q_PROPERTY(QObject *AccelTimeSeries READ getAccelSeries     WRITE setAccelSeries)
    Q_PROPERTY(QObject *IntTimeSeries   READ getIntTimeSeries   WRITE setIntTimeSeries)
    Q_PROPERTY(int SeriesDirection      READ getDirection       WRITE setDirection)
    Q_PROPERTY(double InitialVelocity   READ getVel0            WRITE setVel0)
    Q_PROPERTY(double ScaleFactor       READ getCFactor         WRITE setCFactor)

public:
    RUniformExcitationPattern(QObject *parent, QString name);
    ~RUniformExcitationPattern();

    LoadPattern* getTheLoadPattern();

    int update(void);

    QObject *getDispSeries() const;
    void setDispSeries(QObject *value);

    QObject *getVelSeries() const;
    void setVelSeries(QObject *value);

    QObject *getAccelSeries() const;
    void setAccelSeries(QObject *value);

    QObject *getIntTimeSeries() const;
    void setIntTimeSeries(QObject *value);

    int getDirection() const;
    void setDirection(int value);

    double getVel0() const;
    void setVel0(double value);

    double getCFactor() const;
    void setCFactor(double value);

    double getDuration(void);

protected:

    int createTheUniformExcitationPattern(void);

    int direction;
    double vel0;
    double cFactor;

    ROpenSeesTimeSeries *theDispSeries;
    ROpenSeesTimeSeries *theVelSeries;
    ROpenSeesTimeSeries *theAccelSeries;
    ROpenSeesTimeSeries *theIntTimeSeries;

    GroundMotion* theGroundMotion;
};

#endif
