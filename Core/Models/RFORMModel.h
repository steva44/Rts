#ifndef RFORMModel_H
#define RFORMModel_H
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
#include "RLineSearchCallBackAssistant.h"

class RParameter;
class RResponse;
class RProbabilityDistributions;
class RRandomVariable;
class RMeritFunction;
class RProbabilityTransformation;
class RConvergenceCheck;
class RSearchDirection;
class RLineSearchAlgorithm;
class QCustomPlot;

class RFORMModel : public RModel, public RLineSearchCallBackAssistant
{
    Q_OBJECT
    Q_PROPERTY(QObject *InputParameter READ getInputParameter WRITE setInputParameter)
    Q_PROPERTY(QString Threshold READ getThreshold WRITE setThreshold)
    Q_PROPERTY(int MaxSteps READ getMaxSteps WRITE setMaxSteps)
    Q_PROPERTY(QObject *ProbabilityTransformation READ getProbabilityTransformation WRITE setProbabilityTransformation)
    Q_PROPERTY(QObject *SearchDirection READ getSearchDirection WRITE setSearchDirection)
    Q_PROPERTY(QObject *LineSearchAlgorithm READ getLineSearchAlgorithm WRITE setLineSearchAlgorithm)
    Q_PROPERTY(QObject *MeritFunction READ getMeritFunction WRITE setMeritFunction)
    Q_PROPERTY(QObject *ConvergenceCheck READ getConvergenceCheck WRITE setConvergenceCheck)
    Q_PROPERTY(QString GradientMethod READ getGradientMethod WRITE setGradientMethod)

public:
    RFORMModel(QObject *parent, QString name);
    ~RFORMModel();

    QObject *getInputParameter() const;
    void setInputParameter(QObject *value);

    QString getThreshold();
    void setThreshold(QString value);
    double getThresholdValue();

    int getMaxSteps() const;
    void setMaxSteps(int value);

    QObject *getProbabilityTransformation() const;
    void setProbabilityTransformation(QObject *value);

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

    int evaluateModel(RGradientType theGradientType);
    double evaluate_F(double parameterValue);

protected slots:

    void help();

private:

    double evaluateLimitStateFunction();
    double firstOrderFiniteDifference();
    double secondOrderFiniteDifference();

    // Input parameters
    QPointer<RParameter> theInputParameter;
    QPointer<RParameter> theThresholdParameter;
    double theThresholdDouble;
    int maxSteps;

    // Input tools
    QPointer<RProbabilityTransformation> theProbabilityTransformation;
    QPointer<RSearchDirection> theSearchDirection;
    QPointer<RConvergenceCheck> theConvergenceCheck;
    QPointer<RLineSearchAlgorithm> theLineSearchAlgorithm;
    QPointer<RMeritFunction> theMeritFunction;

    // Class variables passed to merit function
    QVector<double> searchDirection;
    QVector<double> u;
    double functionValue;
    int numRVs;
    QList<RRandomVariable *> theRVs;

    // Responses
    RResponse *theResponse;

    // Plot
    QPointer<QCustomPlot> customPlot;

};

#endif
