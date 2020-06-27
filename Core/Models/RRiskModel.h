 #ifndef RRiskModel_H
#define RRiskModel_H
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
class RProbabilityDistributions;
class RRandomVariable;
class RFORMModel;
class RFOSMModel;
class RSOSMModel;
class RGenericResponse;
class QCustomPlot;

class RRiskModel : public RModel
{
	Q_OBJECT
    Q_PROPERTY(QObject *Input READ getInput WRITE setInput)
    Q_PROPERTY(double CostIncrement READ getCostIncrement WRITE setCostIncrement)
    Q_PROPERTY(double ProbabilityTolerance READ getProbabilityTolerance WRITE setProbabilityTolerance)

public:
    RRiskModel(QObject *parent, QString name);
    ~RRiskModel();

    QObject *getInput() const;
    void setInput(QObject *value);

    double getCostIncrement() const;
    void setCostIncrement(double value);

    double getProbabilityTolerance() const;
    void setProbabilityTolerance(double value);

    int evaluateModel(RGradientType theGradientType);

private:

    RFORMModel *theFORMModel;
    RFOSMModel *theFOSMModel;
    RSOSMModel *theSOSMModel;
    RGenericResponse *theInputToTheUpstreamModel;
    RGenericResponse *theOutputFromTheUpstreamModel;

    double theCostIncrement;
    double theProbabilityTolerance;
    RResponse *theResponse;
    QPointer<QCustomPlot> customPlot;

};

#endif
