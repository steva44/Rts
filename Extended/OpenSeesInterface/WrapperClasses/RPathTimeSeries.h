#ifndef RPathTimeSeries_H
#define RPathTimeSeries_H
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

// Wrapper class for OpenSees PathTimeSeries
// Stevan Gavrilovic 12/2018

// This command is used to construct a Path TimeSeries object.
// The relationship between load factor and time is input by the user as a series of discrete points in the 2d space (load factor, time). When the time specified does not match any of the input points, linear interpolation is used between points. There are many ways to specify the load path:

// For a load path where the factors are specified in a tcl list with a constant time interval between points:
// timeSeries Path $tag -dt $dt -values {list_of_values} <-factor $cFactor> <-useLast> <-prependZero> <-startTime $tStart>
// For a load path where the factors are specified in a file for a constant time interval between points:
// timeSeries Path $tag -dt $dt -filePath $filePath <-factor $cFactor> <-useLast> <-prependZero> <-startTime $tStart>
// For a load path where the values are specified at non-constant time intervals:
// timeSeries Path $tag -time {list_of_times} -values {list_of_values} <-factor $cFactor> <-useLast>
// For a load path where both time and values are specified in a list included in the command
// timeSeries Path $tag -fileTime $fileTime -filePath $filePath <-factor $cFactor> <-useLast>
// $tag	unique tag among TimeSeries objects.
// $filePath	file containing the load factors values
// $fileTime	file containing the time values for corresponding load factors
// $dT	time interval between specified points.
// { list_of_times}	time values in a tcl list
// { list_of_values}	load factor values in a tcl list
// $cFactor	optional, a factor to multiply load factors by (default = 1.0)
// -useLast	optional, to use last value after the end of the series (default = 0.0)
// -prependZero	optional, to prepend a zero value to the series of load factors (default = false). See NOTES.
// $tStart	optional, to provide a start time for provided load factors (default = 0.0)

// NOTES:
// Linear interpolation between points.
// If the specified time is beyond last point (AND WATCH FOR NUMERICAL ROUNDOFF), 0.0 is returned. Specify -useLast to use the last data point instead of 0.0.
// The transient integration methods in OpenSees assume zero initial conditions. So it is important that any timeSeries that is being used in a transient analysis starts from zero (first data point in the timeSeries = 0.0). To guarantee that this is the case the optional parameter -prependZero can be specified to prepend a zero value to the provided timeSeries.

#include "ROpenSeesTimeSeries.h"
#include "Vector.h"

class TimeSeries;
class PathTimeSeries;
class RParameter;

class RPathTimeSeries : public ROpenSeesTimeSeries
{
    Q_OBJECT

    Q_PROPERTY(QString FileName                 READ getFileName        WRITE setFileName)
    Q_PROPERTY(QString FileTimeName             READ getFileTimeName    WRITE setFileTimeName)
    Q_PROPERTY(QString FilePath                 READ getFilePath        WRITE setFilePath)
    Q_PROPERTY(QString FileTimePath             READ getFileTime        WRITE setFileTime)
    Q_PROPERTY(double ScaleFactor               READ getCFactor         WRITE setCFactor)
    Q_PROPERTY(double TimeInterval              READ getTimeStep        WRITE setTimeStep)
    Q_PROPERTY(double TimeStart                 READ getTStart          WRITE setTStart)
    Q_PROPERTY(bool UseLast                     READ getUseLast         WRITE setUseLast)
    Q_PROPERTY(bool PrependZero                 READ getPrependZero     WRITE setPrependZero)


public:
    RPathTimeSeries(QObject *parent, QString name);
    ~RPathTimeSeries();

    TimeSeries* getTheTimeSeries();

    QString getFileName() const;
    void setFileName(const QString &value);

    QString getFileTimeName() const;
    void setFileTimeName(const QString &value);

    QString getFilePath() const;
    void setFilePath(const QString &value);

    QString getFileTime() const;
    void setFileTime(const QString &value);

    double getTStart() const;
    void setTStart(double value);

    double getCFactor() const;
    void   setCFactor(double value);

    bool getPrependZero() const;
    void setPrependZero(bool value);

    //If a time series is created in another model, can just give the load and time vectos, otherwise will be read in from a file
    Vector getTheLoadPath() const;
    void setTheLoadPath(const Vector &value);

    Vector getTheTimePath() const;
    void setTheTimePath(const Vector &value);

    bool getUseLast() const;
    void setUseLast(bool value);

protected:

    int createThePathTimeSeries(void);
    bool fileExists(QString file);

    // $tag	unique tag among TimeSeries objects.
    // $filePath	file containing the load factors values
    // $fileTime	file containing the time values for corresponding load factors
    // $dT	time interval between specified points.
    // $cFactor	optional, a factor to multiply load factors by (default = 1.0)
    // -useLast	optional, to use last value after the end of the series (default = 0.0)
    // -prependZero	optional, to prepend a zero value to the series of load factors (default = false). See NOTES.
    // $tStart	optional, to provide a start time for provided load factors (default = 0.0)

    QString FileName;
    QString FileTimeName;
    QString filePath;
    QString fileTime;

    double startTime;
    double cFactor;

    bool useLast;
    bool prependZero;

    Vector theLoadPath;
    Vector theTimePath;

};

#endif
