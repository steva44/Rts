#ifndef RTime_H
#define RTime_H
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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RParameter.h"
#include <QDateTime>

class RTime : public RParameter
{
    Q_OBJECT
    Q_PROPERTY(double CurrentValue READ getCurrentValue WRITE setCurrentValue)  // Current realization of the time in terms of days relative to the reference time
    Q_PROPERTY(QString Time READ getTime WRITE setTime)                         // Current realization of time in the format of "yyyy-dd-MM hh:mm:ss.zzz"

public:
    RTime(QObject *parent, QString name);
    ~RTime();

    void setCurrentValue(double value);

    QString getTime() const;
    void setTime(QString value);

    int getYear() const;
    int getMonth() const;
    int getDay() const;

    // Returns the format of the date and time, Currently, "yyyy-dd-MM hh:mm:ss.zzz" (zzz=milliseconds)
    static QString getTimeFormat();

    // Returns the number of hours (in 24 hr format) that has passed in the day
    double getHoursDay();

private:

    void updateTime();
    void updateValue();

    QDateTime theTime;
    QDateTime theReferenceTime;
    QDateTime constTime;
    QString format;

};

#endif
