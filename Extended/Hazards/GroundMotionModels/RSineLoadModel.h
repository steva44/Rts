#ifndef RSineLoadModel_H
#define RSineLoadModel_H
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
#include "RTimeSeriesResponse.h"

class RSineLoadModel : public RModel
{
	Q_OBJECT
    Q_PROPERTY(QString LoadAmplitude READ getLoadAmplitude WRITE setLoadAmplitude)
    Q_PROPERTY(QString StartTime READ getStartTime WRITE setStartTime)
    Q_PROPERTY(QString EndTime READ getEndTime WRITE setEndTime)
    Q_PROPERTY(QString Period READ getPeriod WRITE setPeriod)

public:
    RSineLoadModel(QObject *parent, QString name);
    ~RSineLoadModel();

    QString getLoadAmplitude();
    void setLoadAmplitude(QString value);
    double getLoadAmplitudeValue();

    QString getStartTime();
    void setStartTime(QString value);
    double getStartTimeValue();

    QString getEndTime();
    void setEndTime(QString value);
    double getEndTimeValue();

    QString getPeriod();
    void setPeriod(QString value);
    double getPeriodValue();

    QStringList getActionList();

    int evaluateModel(RGradientType theGradientType);

private:

    QPointer<RParameter> theLoadAmplitudeParameter;
    double theLoadAmplitudeDouble;

    RResponse *theTimeSeriesResponse;

    QPointer<RParameter> theStartTimeParameter;
    double theStartTimeDouble;

    QPointer<RParameter> theEndTimeParameter;
    double theEndTimeDouble;

    QPointer<RParameter> thePeriodParameter;
    double thePeriodDouble;

};

#endif
