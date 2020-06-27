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

#include "RDirectionalLineSearchOptimizationAlgorithm.h"
#include "RMatrixOperations.h"
#include "RDirectionalLineSearchOptimizationAlgorithm.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RDecisionVariable.h"
#include "RSearchDirection.h"
#include "RMatrixOperations.h"
#include "RModelManager.h"

#include <QDateTime>
#include <math.h>

RDirectionalLineSearchOptimizationAlgorithm::RDirectionalLineSearchOptimizationAlgorithm(QObject *parent, QString name)
    : ROptimizationAlgorithm(parent, name), RLineSearchCallBackAssistant()
{
    theMaxSteps = 100;
    theConvergenceCheck = nullptr;
    theLineSearchAlgorithm = nullptr;
    theMeritFunction = nullptr;
    customPlot = nullptr;
    theStoredObjective = nullptr;
}


RDirectionalLineSearchOptimizationAlgorithm::~RDirectionalLineSearchOptimizationAlgorithm()
{
    if (customPlot) {
        delete customPlot;
    }
}


QObject * RDirectionalLineSearchOptimizationAlgorithm::getSearchDirection() const
{
    return theSearchDirection;
}


void RDirectionalLineSearchOptimizationAlgorithm::setSearchDirection(QObject *value)
{
    theSearchDirection = qobject_cast<RSearchDirection *>(value);
}


QObject * RDirectionalLineSearchOptimizationAlgorithm::getMeritFunction() const
{
    return theMeritFunction;
}


void RDirectionalLineSearchOptimizationAlgorithm::setMeritFunction(QObject *value)
{
    theMeritFunction = qobject_cast<RMeritFunction *>(value);
}


QObject * RDirectionalLineSearchOptimizationAlgorithm::getLineSearchAlgorithm() const
{
    return theLineSearchAlgorithm;
}


void RDirectionalLineSearchOptimizationAlgorithm::setLineSearchAlgorithm(QObject *value)
{
    theLineSearchAlgorithm = qobject_cast<RLineSearchAlgorithm *>(value);
    if (!theLineSearchAlgorithm) {
        qCritical() << "Invalid search algorithm given to" << this->objectName();
    }
}


int RDirectionalLineSearchOptimizationAlgorithm::getMaxSteps() const
{
    return theMaxSteps;
}


void RDirectionalLineSearchOptimizationAlgorithm::setMaxSteps(int value)
{
    theMaxSteps = value;
    if (theMaxSteps < 1) {
        qDebug() << "The optimization model requires a number of steps greater than zero.";
        theMaxSteps = 1;
    }
}


QObject * RDirectionalLineSearchOptimizationAlgorithm::getConvergenceCheck() const
{
    return theConvergenceCheck;
}


void RDirectionalLineSearchOptimizationAlgorithm::setConvergenceCheck(QObject *value)
{
    theConvergenceCheck = qobject_cast<RConvergenceCheck *>(value);
}


QString RDirectionalLineSearchOptimizationAlgorithm::getGradientMethod()
{
    return translateGradientTypeToString();
}


void RDirectionalLineSearchOptimizationAlgorithm::setGradientMethod(QString value)
{
    setGradientTypeInRObject(value);
}


int RDirectionalLineSearchOptimizationAlgorithm::solveOptimizationProblem(QPointer<RParameter> theObjective)
{
    // Set the objective as a class member, to make it accessible for evaluate_F type methods
    theStoredObjective = theObjective;


    // ***************************************************
    //  GET A LIST OF THE DESIGN VARIABLES
    // ***************************************************
    QList<RDecisionVariable *> theDVs = theDomain->findChildren<RDecisionVariable *>();
    int numDVs = theDVs.size();
    QList<RParameter *> theDVsCastAsParameters;
    for (int i=0; i<numDVs; i++) {
        theDVsCastAsParameters << qobject_cast<RParameter *>(theDVs.at(i));
    }



    // ***************************************************
    // CREATE PLOT
    // ***************************************************
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    QVector<double> tickVector;
    tickVector << 0;
    QVector<QString> labelVector;
    labelVector << QString("%1").arg(0);
    if (customPlot == nullptr) {

        customPlot = new QCustomPlot();
        mainWindow->addSubWindow(customPlot, "Optimization Evolution Plot - Using RDirectionalLineSearchOptimization", true);

        customPlot->addGraph();
        customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));

        customPlot->graph(0)->setPen(QColor("blue"));
        customPlot->xAxis->setLabel("Iteration");
        customPlot->yAxis->setLabel("Objective Function");
        customPlot->yAxis->setLabelColor(QColor("blue"));
        customPlot->yAxis->setRange(0.0, 1.0);
        customPlot->xAxis->setAutoTicks(false);
        customPlot->xAxis->setAutoTickLabels(false);

        // Add graph to show the value of the design variables
        for (int i=0; i<numDVs; i++) {

            customPlot->addGraph(nullptr, customPlot->yAxis2);
            customPlot->graph(i+1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            customPlot->graph(i+1)->setPen(QColor("red"));
        }
        customPlot->yAxis2->setVisible(true);
        customPlot->yAxis2->setLabelColor(QColor("red"));
        customPlot->yAxis2->setLabel("Design Variables");
    }
    else {

        customPlot->graph(0)->clearData();
        for (int i=0; i<numDVs; i++) {
            customPlot->graph(i+1)->clearData();
        }
    }



    // ***************************************************
    // INITIAL DECLARATIONS
    // ***************************************************
    double functionValue;
    double maxFunctionValue = 0.0;
    double minFunctionValue = 0.0;
    double maxDesignVariableValue = 0.0;
    double minDesignVariableValue = 0.0;
    double functionBuffer, variableBuffer;
    double stepSize;
    int convergenceCheckResult;
    QVector<double> functionGradient(numDVs);
    int ii = 0;


    // Set the size of the vectors used in the merit function, i.e., those used in evaluate_F()
    searchDirection.resize(numDVs);
    x.resize(numDVs);


    // ***************************************************
    // SET THE START POINT AT THE CURRENT VALUE OF THE DESIGN VARIABLES (remove the initial value property of the continuous DVs...)
    // ***************************************************
    for (int i = 0; i < numDVs; i++) {
        x[i] = theDVs.value(i)->getCurrentValue();
    }


    // ***************************************************
    // START THE OPTIMIZATION LOOP
    // ***************************************************
    while(ii<theMaxSteps) {


        // ***************************************************
        // ITERATE THE COUNTER AND KEEP THE GUI RESPONSIVE
        // ***************************************************
        QCoreApplication::processEvents();
        ii++;


        // ***************************************************
        // EVALUATE THE OBJECTIVE FUNCTION AND ITS GRADIENT
        // ***************************************************
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theObjective);
        if (!theInputCastAsResponse)
        {
            qCritical() << this->objectName() << "needs the input parameter to be a generic response";
        }
        RModel *theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());
        theModel->runAnalysis(theGradientType);
        functionValue = theObjective->getCurrentValue();
        theInputCastAsResponse->getGradientWrtTheseParameters(theDVsCastAsParameters, &functionGradient, theDomain);
        theDomain->clearAnalyzedFlags();



        // ***************************************************
        // GIVE OUTPUT IF REQUESTED
        // ***************************************************
        if (theOutputLevel == RDomain::Maximum) {

            qDebug() << "At step" << ii << "in" << this->objectName() << "the objective is" << functionValue << "and the variables are:";
            for (int i=0; i<numDVs; i++) {
                qDebug() << theDVs[i]->objectName() << "=" << theDVs[i]->getCurrentValue() << "with gradient" << functionGradient.at(i);
            }
            qDebug() << "";
        }


        // ***************************************************
        // PLOT
        // ***************************************************
        for (int i=0; i<numDVs; i++) {
            if(theDVs.at(i)->getCurrentValue() > maxDesignVariableValue) {
                maxDesignVariableValue = theDVs.at(i)->getCurrentValue();
            }
            if(theDVs.at(i)->getCurrentValue() < minDesignVariableValue) {
                minDesignVariableValue = theDVs.at(i)->getCurrentValue();
            }
        }
        if (functionValue > maxFunctionValue) {
            maxFunctionValue = functionValue;
        }
        if (functionValue < minFunctionValue) {
            minFunctionValue = functionValue;
        }
        functionBuffer = 0.1 * (maxFunctionValue - minFunctionValue);
        variableBuffer = 0.1 * (maxDesignVariableValue - minDesignVariableValue);


        customPlot->graph(0)->addData(ii, functionValue);
        customPlot->xAxis->setRange(0, ii+1);
        customPlot->yAxis->setRange(minFunctionValue - functionBuffer, maxFunctionValue + functionBuffer);
        for (int i = 0; i < numDVs; i++) {
            customPlot->graph(i+1)->addData(ii, theDVs.at(i)->getCurrentValue());
        }
        customPlot->yAxis2->setRange(minDesignVariableValue - variableBuffer, maxDesignVariableValue + variableBuffer);

        tickVector << ii;
        labelVector << QString("%1").arg(ii);

        customPlot->xAxis->setTickVector(tickVector);
        customPlot->xAxis->setTickVectorLabels(labelVector);

        customPlot->replot();


        // ***************************************************
        // CHECK CONVERGENCE
        // ***************************************************
        convergenceCheckResult = theConvergenceCheck->check(nullptr, 0.0, &functionGradient);
        if (convergenceCheckResult >= 0) {


            // TELL THE USER THE GOOD NEWS
            if (theOutputLevel >= RDomain::Maximum) {
                qDebug() << "The optimization analysis converged with decision variable values:";
                for (int i=0; i<numDVs; i++) {
                    qDebug() << theDVs[i]->objectName() << "=" << theDVs[i]->getCurrentValue() << "with gradient" << functionGradient.at(i);
                }
            }
            break;
        }
        else if (ii==theMaxSteps) {


            // SORRY THE ANALYSIS DID NOT CONVERGE
            qCritical() << "The max number of iterations in" << this->objectName() << "was reached without convergence.";
            if (theOutputLevel >= RDomain::Medium) {
                qDebug() << "The decision variable values at the maximum number of iterations are:";
                for (int i=0; i<numDVs; i++) {
                    qDebug() << theDVs[i]->objectName() << "=" << theDVs[i]->getCurrentValue() << "with gradient" << functionGradient.at(i);
                }
            }
            return -1;
        }
        else {

            // TAKE A STEP
            theSearchDirection->getSearchDirection(&x, functionValue, &functionGradient, &searchDirection);
            stepSize = theLineSearchAlgorithm->findPoint(this);
            for(int i=0; i < numDVs; i++) {

                x[i] = x.at(i) + stepSize * searchDirection.at(i);
                theDVs[i]->setCurrentValue(x.at(i));
            }
        }
    }

    return 0;
}


double RDirectionalLineSearchOptimizationAlgorithm::evaluate_F(double parameterValue)
{
    if (theStoredObjective) {

        // Get list of design variables
        QList<RDecisionVariable *> theDVs = theDomain->findChildren<RDecisionVariable *>();
        int numDVs = theDVs.size();

        // Set the value of the DVs at the point implied by parameterValue
        for(int i=0; i < numDVs; i++) {

            theDVs[i]->setCurrentValue(x.at(i) + parameterValue * searchDirection.at(i));
        }


        // Run the analysis
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theStoredObjective);
        RModel *theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());
        theModel->runAnalysis(RGradientType::NONE);
        theDomain->clearAnalyzedFlags();


        // Evaluate the objective function
        double objective = theStoredObjective->getCurrentValue();


        // Evaluate the merit function
        return theMeritFunction->getMeritFunctionValue(objective, nullptr, nullptr);
    }
    else {

        qCritical() << "The evaluate_F method was called in" << this->objectName() << "without an objective.";
        return 0.0;
    }
}
