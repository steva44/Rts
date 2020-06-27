#ifndef R1DOptimizationModel_H
#define R1DOptimizationModel_H
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
*                                                                    *
*********************************************************************/

#include "RModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RSearchDirection.h"
#include "RLineSearchCallBackAssistant.h"
#include "RLineSearchAlgorithm.h"

#include "qcustomplot.h"

class RLineSearch;

class R1DOptimizationModel : public RModel, public RLineSearchCallBackAssistant
{
    Q_OBJECT
    Q_PROPERTY(QObject *Function READ getFunction WRITE setFunction)
    Q_PROPERTY(QObject *Parameter READ getParameter WRITE setParameter)
    Q_PROPERTY(QObject *LineSearchAlgorithm READ getLineSearchAlgorithm WRITE setLineSearchAlgorithm)
    Q_PROPERTY(double Perturbation READ getPerturbation WRITE setPerturbation)

public:
    R1DOptimizationModel(QObject *parent, QString name);
    ~R1DOptimizationModel();

    QObject *getFunction() const;
    void setFunction(QObject *value);

    QObject *getParameter() const;
    void setParameter(QObject *value);

    QObject *getLineSearchAlgorithm() const;
    void setLineSearchAlgorithm(QObject *value);

    double getPerturbation() const;
    void setPerturbation(double value);

    double evaluate_F(double parameterValue);
    double evaluate_f(double parameterValue);
    double evaluate_h(double parameterValue);

    int evaluateModel(RGradientType theGradientType);

private:

    double firstOrderFiniteDifference();
    double secondOrderFiniteDifference();

    QPointer<RParameter> theFunction;
    QPointer<RModel> theModel;
    QPointer<RLineSearchAlgorithm> theLineSearchAlgorithm;
    QPointer<RParameter> theParameter;
    RResponse *theResponse;
    double thePerturbation;
    QPointer<QCustomPlot> customPlot;
};

#endif
