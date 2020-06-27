#ifndef RScenarioModel_H
#define RScenarioModel_H
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
*   - Terje Haukaas                                                  *
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RModel.h"

class RParameter;
class RResponse;
class QCustomPlot;

class RScenarioModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)
    Q_PROPERTY(QObject *StartTime READ getStartTime WRITE setStartTime)
    Q_PROPERTY(QObject *EndTime READ getEndTime WRITE setEndTime)
    Q_PROPERTY(QString RealInterestRate READ getRealInterestRate WRITE setRealInterestRate)
    Q_PROPERTY(QString MaxTimeGap READ getMaxTimeGap WRITE setMaxTimeGap)

public:
    RScenarioModel(QObject *parent, QString name);
    ~RScenarioModel();

    QString getInputParameterList();
    void setInputParameterList(QString value);

    QObject *getStartTime() const;
    void setStartTime(QObject *value);

    QObject *getEndTime() const;
    void setEndTime(QObject *value);

    QString getRealInterestRate();
    double getRealInterestRateValue();
    void setRealInterestRate(QString value);

    QString getMaxTimeGap();
    double getMaxTimeGapValue();
    void setMaxTimeGap(QString value);

    QStringList getActionList();

    int evaluateModel(RGradientType theGradientType);

public slots:

    void plotModelFlowchart();

private:

    QList<QPointer<RParameter> > theInputParameterList;
    QPointer<RParameter> theStartTime;
    QPointer<RParameter> theEndTime;

    RResponse *theResponse;

    QPointer<RParameter> theRealInterestRateParameter;
    double theRealInterestRateDouble;

    QPointer<RParameter> theMaxTimeGapParameter;
    double theMaxTimeGapDouble;

    QPointer<QCustomPlot> customPlot;
    double overallMaxCost;
    int plotCounter;
};

#endif
