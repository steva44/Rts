#ifndef RTimeHistoryScalingPlotModel_H
#define RTimeHistoryScalingPlotModel_H
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

// This class plots responses at various ground motion scaling factors

#include "RResponse.h"
#include "RLoadPattern.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>
#include "qcustomplot.h"

class RParameter;
class RResponse;
class RBIM;

class RTimeHistoryScalingPlotModel : public RModel
{
    Q_OBJECT

    Q_PROPERTY(QString LoadPattern         READ getTheLoadPattern       WRITE setTheLoadPattern)
    Q_PROPERTY(QString InputParameterList  READ getInputParameterList  WRITE setInputParameterList)

public:
    RTimeHistoryScalingPlotModel(QObject *parent, QString name);
    ~RTimeHistoryScalingPlotModel();

    QString getTheLoadPattern();

    void setTheLoadPattern(QString value);

    QString getInputParameterList();

    void setInputParameterList(QString value);

    int evaluateModel(RGradientType theGradientType);

private:

    QPointer<RBIM> theBIM;

    QList<QPointer<RParameter> > theInputParameterList;
    QList<QPointer<RLoadPattern> > theLoadPatterns;

    RResponse *theResponse;
    QPointer<QCustomPlot> customPlot;

};

#endif
