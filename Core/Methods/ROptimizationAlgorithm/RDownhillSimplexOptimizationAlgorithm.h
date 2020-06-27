#ifndef RDownhillSimplexOptimizationAlgorithm_H
#define RDownhillSimplexOptimizationAlgorithm_H
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
#include "qpair.h"
#include "qcustomplot.h"

class RDownhillSimplexOptimizationAlgorithm : public ROptimizationAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double Alpha READ getAlpha WRITE setAlpha)
    Q_PROPERTY(double Gamma READ getGamma WRITE setGamma)
    Q_PROPERTY(double Rho READ getRho WRITE setRho)
    Q_PROPERTY(double Sigma READ getSigma WRITE setSigma)
    Q_PROPERTY(int MaxSteps READ getMaxSteps WRITE setMaxSteps)
    Q_PROPERTY(double Tolerance READ getTolerance WRITE setTolerance)
    Q_PROPERTY(double Delay READ getDelay WRITE setDelay)

public:

    RDownhillSimplexOptimizationAlgorithm(QObject *parent, QString name);
    ~RDownhillSimplexOptimizationAlgorithm();

    double getAlpha() const;
    void setAlpha(double value);

    double getGamma() const;
    void setGamma(double value);

    double getRho() const;
    void setRho(double value);

    double getSigma() const;
    void setSigma(double value);

    int getMaxSteps() const;
    void setMaxSteps(int value);

    double getTolerance() const;
    void setTolerance(double value);

    double getDelay() const;
    void setDelay(double value);

    int solveOptimizationProblem(QPointer<RParameter> objective);

private:

    double evaluateObjective(QVector<double> *point);

    double theAlpha;
    double theGamma;
    double theRho;
    double theSigma;
    double theTolerance;
    int theMaxSteps;
    double theDelay;
    QList<RDecisionVariable *> theDVs;
    QPointer<RParameter> theObjective;
    std::unique_ptr<QCustomPlot> customPlot = nullptr;
};

#endif

