#ifndef RPoissonPointProcessOccurrenceModel_H
#define RPoissonPointProcessOccurrenceModel_H
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

#include "RModel.h"

class RParameter;
class RResponse;

class RPoissonPointProcessOccurrenceModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *StartTime READ getStartTime WRITE setStartTime)
    Q_PROPERTY(QObject *EndTime READ getEndTime WRITE setEndTime)
    Q_PROPERTY(QString ReturnPeriodInYears READ getReturnPeriodInYears WRITE setReturnPeriodInYears)

public:
    RPoissonPointProcessOccurrenceModel(QObject *parent, QString name);
    ~RPoissonPointProcessOccurrenceModel();

    QString getReturnPeriodInYears();
    void setReturnPeriodInYears(QString value);
    double getReturnPeriodInYearsValue();

    QObject *getStartTime() const;
    void setStartTime(QObject *value);

    QObject *getEndTime() const;
    void setEndTime(QObject *value);

    QList<QPair<double, QString> > getTriggerTimes(double startTime, double endTime);
    int resetTime();
    int evaluateModel(RGradientType theGradientType);

private:

    unsigned int seed;
    QList<QPair<double, QString> > occurrenceTimes;

    QPointer<RParameter> theReturnPeriodInYearsParameter;
    double theReturnPeriodInYearsDouble;
    RResponse *theResponse;

    QPointer<RParameter> theStartTime;
    QPointer<RParameter> theEndTime;
};

#endif
