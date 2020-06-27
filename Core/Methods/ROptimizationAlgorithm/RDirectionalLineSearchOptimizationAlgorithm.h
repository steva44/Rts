#ifndef RDirectionalLineSearchOptimizationAlgorithm_H
#define RDirectionalLineSearchOptimizationAlgorithm_H
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

#include "ROptimizationAlgorithm.h"
#include "RModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RSearchDirection.h"
#include "RLineSearchCallBackAssistant.h"
#include "RLineSearchAlgorithm.h"
#include "RConvergenceCheck.h"
#include "RMeritFunction.h"

#include "qcustomplot.h"

class RDirectionalLineSearchOptimizationAlgorithm : public ROptimizationAlgorithm, public RLineSearchCallBackAssistant
{
    Q_OBJECT
    Q_PROPERTY(int MaxSteps READ getMaxSteps WRITE setMaxSteps)
    Q_PROPERTY(QObject *SearchDirection READ getSearchDirection WRITE setSearchDirection)
    Q_PROPERTY(QObject *LineSearchAlgorithm READ getLineSearchAlgorithm WRITE setLineSearchAlgorithm)
    Q_PROPERTY(QObject *MeritFunction READ getMeritFunction WRITE setMeritFunction)
    Q_PROPERTY(QObject *ConvergenceCheck READ getConvergenceCheck WRITE setConvergenceCheck)
    Q_PROPERTY(QString GradientMethod READ getGradientMethod WRITE setGradientMethod)

public:
    RDirectionalLineSearchOptimizationAlgorithm(QObject *parent, QString name);
    ~RDirectionalLineSearchOptimizationAlgorithm();

    int getMaxSteps() const;
    void setMaxSteps(int value);

    QObject *getSearchDirection() const;
    void setSearchDirection(QObject *value);

    QObject *getLineSearchAlgorithm() const;
    void setLineSearchAlgorithm(QObject *value);

    QObject *getMeritFunction() const;
    void setMeritFunction(QObject *value);

    QObject *getConvergenceCheck() const;
    void setConvergenceCheck(QObject *value);

    QString getGradientMethod();
    void setGradientMethod(QString value);

    int solveOptimizationProblem(QPointer<RParameter> theObjective);

    // Here we add a public method needed by 1D line-search algorithms that
    // need to evaluate the objective function along a search direction
    double evaluate_F(double parameterValue);

private:

    int theMaxSteps;
    QPointer<RSearchDirection> theSearchDirection;
    QPointer<RConvergenceCheck> theConvergenceCheck;
    QPointer<RLineSearchAlgorithm> theLineSearchAlgorithm;
    QPointer<RMeritFunction> theMeritFunction;
    QPointer<QCustomPlot> customPlot;
    QVector<double> x;
    QVector<double> searchDirection;
    QPointer<RParameter> theStoredObjective;
};

#endif
