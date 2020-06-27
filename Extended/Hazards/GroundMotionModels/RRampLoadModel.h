#ifndef RRampLoadModel_H
#define RRampLoadModel_H
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

class RRampLoadModel : public RModel
{
	Q_OBJECT
    Q_PROPERTY(QObject *LoadIntensity READ getLoadIntensity WRITE setLoadIntensity)
    Q_PROPERTY(QString RampStartTime READ getRampStartTime WRITE setRampStartTime)
    Q_PROPERTY(QString RampEndTime READ getRampEndTime WRITE setRampEndTime)

public:
	RRampLoadModel(QObject *parent, QString name);
	~RRampLoadModel();

    QObject *getLoadIntensity() const;
    void setLoadIntensity(QObject *value);

    QString getRampStartTime();
    void setRampStartTime(QString value);
    double getRampStartTimeValue();

    QString getRampEndTime();
    void setRampEndTime(QString value);
    double getRampEndTimeValue();

    int evaluateModel(RGradientType theGradientType);

private:

    QPointer<RParameter> theLoadIntensity;

    RResponse *theTimeSeriesResponse;

    QPointer<RParameter> theRampStartTimeParameter;
    double theRampStartTimeDouble;

    QPointer<RParameter> theRampEndTimeParameter;
    double theRampEndTimeDouble;

};

#endif
