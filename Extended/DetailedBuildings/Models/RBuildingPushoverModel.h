#ifndef RBuildingPushoverModel_H
#define RBuildingPushoverModel_H
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

// This model performs a pushover analysis on a structure

#include "RResponse.h"
#include "RLoadPattern.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RBuildingPushoverModel : public RModel
{
    Q_OBJECT

    Q_PROPERTY(QString LoadPattern         READ getTheLoadPattern       WRITE setTheLoadPattern)
    Q_PROPERTY(QString CostParameterList READ getCostParameterList WRITE setCostParameterList)
    Q_PROPERTY(QString TimeParameterList READ getTimeParameterList WRITE setTimeParameterList)
    Q_PROPERTY(QString EmissionsParameterList READ getEmissionsParameterList WRITE setEmissionsParameterList)
    Q_PROPERTY(QString NumLoadSteps READ getNumLoadSteps WRITE setNumLoadSteps)
    Q_PROPERTY(QObject *Load READ getLoadParameter WRITE setLoadParameter)

public:
    RBuildingPushoverModel(QObject *parent, QString name);
    ~RBuildingPushoverModel();

    QString getTheLoadPattern();
    void    setTheLoadPattern(QString value);

    QString getCostParameterList();
    void setCostParameterList(QString value);

    QString getTimeParameterList();
    void setTimeParameterList(QString value);

    QString getEmissionsParameterList();
    void setEmissionsParameterList(QString value);

    QObject *getLoadParameter() const;
    void setLoadParameter(QObject *value);

    QString getNumLoadSteps();
    double getNumLoadStepsValue();
    void setNumLoadSteps(QString value);

    int evaluateModel(RGradientType theGradientType);

    double getMaxBuildingDrift();

    void applyLoadToFloors(void);

private:

    QList<QPointer<RParameter> > theCostParameterList;
    QList<QPointer<RParameter> > theTimeParameterList;
    QList<QPointer<RParameter> > theEmissionsParameterList;

    QPointer<RLoadPattern> theLoadPattern;
    QPointer<RParameter> theLoadParameter;

    RResponse *theResponse;

    double theLoadDouble;

    QPointer<RParameter> theNumLoadStepsParameter;
    double theNumLoadStepsDouble;

    QPointer<QCustomPlot> customPlot;

};

#endif
