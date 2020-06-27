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

#include "RADKZhangMeritFunction.h"
#include "RAlgebraicExpressionModel.h"
#include "RContinuousRandomVariable.h"
#include "RConvergenceCheck.h"
#include "RCorrelation.h"
#include "RDecisionVariable.h"
#include "RDomain.h"
#include "RFORMModel.h"
#include "RGenericResponse.h"
#include "RHLRFSearchDirection.h"
#include "RInHouseProbabilityDistributions.h"
#include "RLineSearchAlgorithm.h"
#include "RMainWindow.h"
#include "RMatrixOperations.h"
#include "RNatafProbabilityTransformation.h"
#include "RNormalDistribution.h"
#include "RObject.h"
#include "RParameter.h"
#include "RProbabilityTransformation.h"
#include "RRandomVariable.h"
#include "RResponse.h"
#include "RSearchDirection.h"
#include "RStandardFORMConvergenceCheck.h"

#include <QCoreApplication>
#include <QPointer>
#include <QDateTime>
#include "qcustomplot.h"

RFORMModel::RFORMModel(QObject *parent, QString name)
    : RModel(parent, name), RLineSearchCallBackAssistant()
{
    isOrchestratingModel = true;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    theInputParameter = nullptr;
    theThresholdParameter = nullptr;
    theThresholdDouble = 0.0;
    maxSteps = 100;

    theProbabilityTransformation = nullptr;
    theSearchDirection = nullptr;
    theConvergenceCheck = nullptr;
    theLineSearchAlgorithm = nullptr;
    customPlot = nullptr;
}


RFORMModel::~RFORMModel()
{

}


void RFORMModel::help()
{
    qDebug() << "*****************************************************************";
    qDebug() << "This model conducts a FORM analysis...";
    qDebug() << "*****************************************************************";
}


QObject * RFORMModel::getInputParameter() const
{
    return theInputParameter;
}


void RFORMModel::setInputParameter(QObject *value)
{
    theInputParameter = qobject_cast<RParameter *>(value);
}


QString RFORMModel::getThreshold()
{
    return getStringParameter(&theThresholdDouble, theThresholdParameter);
}


void RFORMModel::setThreshold(QString value)
{
    setStringParameter(value, &theThresholdDouble, &theThresholdParameter);
}


double RFORMModel::getThresholdValue()
{
    return getDoubleParameter(&theThresholdDouble, theThresholdParameter);
}


int RFORMModel::getMaxSteps() const
{
    return maxSteps;
}


void RFORMModel::setMaxSteps(int value)
{
    maxSteps = value;
    if (maxSteps < 1) {
        qDebug() << "The FORM model requires the maximum number of steps to be greater than zero.";
        maxSteps = 1;
    }
}


QObject * RFORMModel::getProbabilityTransformation() const
{
    return theProbabilityTransformation;
}


void RFORMModel::setProbabilityTransformation(QObject *value)
{
    theProbabilityTransformation = qobject_cast<RProbabilityTransformation *>(value);
}


QObject * RFORMModel::getMeritFunction() const
{
    return theMeritFunction;
}


void RFORMModel::setMeritFunction(QObject *value)
{
    theMeritFunction = qobject_cast<RMeritFunction *>(value);
}


QObject * RFORMModel::getLineSearchAlgorithm() const
{
    return theLineSearchAlgorithm;
}


void RFORMModel::setLineSearchAlgorithm(QObject *value)
{
    theLineSearchAlgorithm = qobject_cast<RLineSearchAlgorithm *>(value);
    if (!theLineSearchAlgorithm) {
        qCritical() << "Invalid search algorithm given to" << this->objectName();
    }
}


QObject * RFORMModel::getConvergenceCheck() const
{
    return theConvergenceCheck;
}


void RFORMModel::setConvergenceCheck(QObject *value)
{
    theConvergenceCheck = qobject_cast<RConvergenceCheck *>(value);
}


QObject * RFORMModel::getSearchDirection() const
{
    return theSearchDirection;
}


void RFORMModel::setSearchDirection(QObject *value)
{
    theSearchDirection = qobject_cast<RSearchDirection *>(value);
}


QString RFORMModel::getGradientMethod()
{
    return translateGradientTypeToString();
}


void RFORMModel::setGradientMethod(QString value)
{
    setGradientTypeInRObject(value);
}


int RFORMModel::evaluateModel(RGradientType theGradientType)
{
    // Get list of random variables
    theRVs = theDomain->findChildren<RRandomVariable *>();
    numRVs = theRVs.size();

    // Loop over random variables to do a few things
    QVector<double> x(numRVs);
    QList<RParameter *> theRVsCastAsParameters;
    for (int i=0; i<numRVs; i++) {

        // Update random variables in case their mean is a design variable
        theRVs.at(i)->updateParameters();

        // Create the vector of random variables and set the start point
        x[i] = theRVs.at(i)->getCurrentValue();

        // Cast random variables as parameters for sensitivity calculations
        theRVsCastAsParameters << qobject_cast<RParameter *>(theRVs.at(i));
    }


    // Set the size of two vectors that are class variables used in the line search
    searchDirection.resize(numRVs);
    u.resize(numRVs);

    // Initialize the probability transformation
    theProbabilityTransformation->setRandomVariableList(&theRVs);
    theProbabilityTransformation->transformPointToStandardNormalSpace(&x, &u);

    // Create a few tools and get pointer to main window, for plotting purposes
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    RMatrixOperations theMatrixOperations(theDomain);
    RNormalDistribution theNormalDistribution(theDomain);

    // Create the plot
    QVector<double> tickVector;
    tickVector << 0;
    QVector<QString> labelVector;
    labelVector << QString("%1").arg(0);

    if (theOutputLevel >= RObject::Medium) {

        if (customPlot == nullptr) {

            customPlot = new QCustomPlot();
            mainWindow->addSubWindow(customPlot, "FORM Evolution Plot", true);
            customPlot->addGraph();
            customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            customPlot->xAxis->setLabel("Iteration");
            customPlot->yAxis->setLabel("Distance from Origin in Standard Normal Space");
            customPlot->xAxis->setAutoTicks(false);
            customPlot->xAxis->setAutoTickLabels(false);
        }
        else {

            customPlot->graph(0)->clearData();
        }
    }


    // Initial declarations before the search
    QVector<double> gradientInStandardNormalSpace(numRVs);
    QVector<double> alpha(numRVs);
    QVector<double> functionGradient(numRVs);
    double gradientInStandardNormalSpaceNorm = 0;
    double stepSize;
    double beta;
    double tempBeta;
    double betaPlotLimit = 0.0;
    double theFailureProbability;
    int iterationIndex = 1;


    // Iterate to find the design point
    while (iterationIndex <= maxSteps)
    {

        // Keep the GUI responsive
        QCoreApplication::processEvents();


        // Transform from standard normal space to original space
        theProbabilityTransformation->transformPointToOriginalSpace(&u, &x);


        // Give output if requested
        if (theOutputLevel == RObject::Maximum) {

            qDebug() << this->objectName() << "is at step number" << iterationIndex << "at this point in the standard normal space:";
            theDomain->print(&u);

            qDebug() << "... corresponding to this point in the original space:";
            theDomain->print(&x);
        }


        // Update random variables
        for (int i = 0; i < numRVs; i++) {

            theRVs[i]->setCurrentValue(x.at(i));
        }


        //  Evaluate the limit-state function
        functionValue = evaluateLimitStateFunction();


        // Evaluate the gradient of the limit-state function
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameter);
        RContinuousRandomVariable *theThresholdCastAsRV;
        if (theInputCastAsResponse) {

            theInputCastAsResponse->getGradientWrtTheseParameters(theRVsCastAsParameters, &functionGradient, theDomain);

            // The sign change here is because the parameter appears with a minus sign in the limit-state function
            theMatrixOperations.scaleVector(-1.0, &functionGradient, &functionGradient);

            // If the threshold is a random variable
            if (theThresholdParameter) {
                theThresholdCastAsRV = theThresholdCastAsRV = qobject_cast<RContinuousRandomVariable *>(theThresholdParameter);
                if (theThresholdCastAsRV) {
                    functionGradient[theRVs.indexOf(theThresholdCastAsRV)] = 1.0;
                }
            }
        }
        else {

            qCritical() << this->objectName() << "needs the input parameter to be a generic response";
            return -1;
        }


        // Set the scale parameter in the first step
        if (iterationIndex == 1) {

            // If the limit-state function value is already near zero, we may already be at the design point
            if (functionValue < 0.1) {

                theConvergenceCheck->setScaleValue(0.1);
            }
            else {

                theConvergenceCheck->setScaleValue(functionValue);
            }
        }


        // Transform the gradient into the standard normal space and compute its norm
        theProbabilityTransformation->transformGradientToStandardNormalSpace(&x, &u, &functionGradient, &gradientInStandardNormalSpace);
        gradientInStandardNormalSpaceNorm = theMatrixOperations.norm(&gradientInStandardNormalSpace);
        if (gradientInStandardNormalSpaceNorm == 0.0) {

            qCritical() << "The norm of the gradient vector in" << this->objectName() << "is zero!";
            return -1;
        }


        // Compute the alpha vector
        for (int i = 0; i < numRVs; i++) {

            alpha[i] = -gradientInStandardNormalSpace.at(i)/gradientInStandardNormalSpaceNorm;
        }


        // Give output if requested
        if (theOutputLevel == RObject::Maximum) {

            qDebug() << "The gradient in the original space in" << this->objectName() << "is:";
            theDomain->print(&functionGradient);

            qDebug() << "The gradient in the standard normal space in" << this->objectName() << "is:";
            theDomain->print(&gradientInStandardNormalSpace);

            qDebug() << "The alpha-vector in" << this->objectName() << "is:";
            theDomain->print(&alpha);
        }


        // Update the plot
        if (theOutputLevel >= RObject::Medium) {

            tempBeta = theMatrixOperations.norm(&u);
            customPlot->graph(0)->addData(iterationIndex, tempBeta);
            customPlot->xAxis->setRange(0, iterationIndex+1);

            if (tempBeta > betaPlotLimit) {

                betaPlotLimit = 1.1*tempBeta;
                customPlot->yAxis->setRange(0, betaPlotLimit);
            }

            tickVector << iterationIndex;
            labelVector << QString("%1").arg(iterationIndex);
            customPlot->xAxis->setTickVector(tickVector);
            customPlot->xAxis->setTickVectorLabels(labelVector);
            customPlot->replot();
        }


        // Check convergence
        int convergenceCheckResult = theConvergenceCheck->check(&u, functionValue, &gradientInStandardNormalSpace);


        // If converged, or at the max number of iterations
        if (convergenceCheckResult > 0 || iterationIndex == maxSteps)  {


            // Compute the reliabity index (beta) and the failure probability
            beta = theMatrixOperations.dotProduct(&alpha, &u);
            theFailureProbability = theNormalDistribution.getNormalCDF(-beta, 1.0);
            theResponse->setCurrentValue(theFailureProbability);


            // Update random variables
            for (int i = 0; i < numRVs; i++) {
                theRVs[i]->setCurrentValue(x.at(i));
            }


            // Calculate DDM gradients
            if (theGradientType == RGradientType::DDM) {

                QMap<RParameter *, double> theLocalGradientMap;
                QMap<RParameter *, double> mapOf_dInput_dPar;
                QMap<RParameter *, double> mapOf_dThreshold_dPar;

                double dProbability_dInput = theNormalDistribution.getNormalPDF(beta,1)/gradientInStandardNormalSpaceNorm;
                mapOf_dInput_dPar = theInputCastAsResponse->getGradientMap();
                QMapIterator<RParameter *, double> i(mapOf_dInput_dPar);
                while (i.hasNext()) {
                    i.next();
                    double dInputdPar = mapOf_dInput_dPar.value(i.key());
                    theLocalGradientMap.insert(i.key(), dProbability_dInput*dInputdPar);
                }


                // Address the possibility of RVs having DVs as mean (must assume they haven't entered the gradient map before; risky?)
                QVector<double> dProbability_dMean(numRVs);
                QVector< QVector<double> > dy_dMean(numRVs,QVector<double>(numRVs));
                QVector< QVector<double> > dy_dStdv(numRVs,QVector<double>(numRVs));


                // Get sensitivities from the probability transformation
                theProbabilityTransformation->computeSensitivityToDistributionParameters(&x, &dy_dMean, &dy_dStdv);


                // Multiply Jacobian_u_Mean * alpha * phi(beta) = dp/dMean
                theMatrixOperations.multiplyMatrixVector(false, 1.0, &dy_dMean, &alpha, 0.0, &dProbability_dMean);


                // Insert dP/dMean * dMean
                for (int k=0; k<numRVs; k++) {
                    RDecisionVariable *theDV = theDomain->findChild<RDecisionVariable *>(qobject_cast<RContinuousRandomVariable *>(theRVs.at(k))->getMean());
                    if (theDV) {
                        theLocalGradientMap.insert(qobject_cast<RParameter *>(theDV), dProbability_dMean.at(k));
                    }
                }


                // Add contribution when threshold is a RV or DV
                RResponse *theThresholdCastAsResponse = qobject_cast<RResponse *>(theThresholdParameter);
                if (theThresholdCastAsResponse) {

                    double dProbability_dThreshold = 1.0/gradientInStandardNormalSpaceNorm;
                    mapOf_dThreshold_dPar = theThresholdCastAsResponse->getGradientMap();
                    QMapIterator<RParameter *, double> ii(mapOf_dThreshold_dPar);
                    while (ii.hasNext()) {
                        ii.next();
                        double dThresholddPar = mapOf_dThreshold_dPar.value(ii.key());
                        theLocalGradientMap.insert(ii.key(), dProbability_dThreshold*dThresholddPar);
                    }
                }
                else if (theThresholdCastAsRV) {

                    double dProbability_dThreshold = 1.0/gradientInStandardNormalSpaceNorm;
                    theLocalGradientMap.insert(theThresholdParameter, dProbability_dThreshold);
                }

                theResponse->setGradientMap(theLocalGradientMap);
            }


            // If we ran to the max number of steps
            if (iterationIndex == maxSteps) {

                qCritical() << this->objectName() << "arrived at iteration" << iterationIndex << "without convergence.";
                return -1;
            }
            else {

                if (iterationIndex != maxSteps) {

                    if (theOutputLevel == RObject::Maximum) {

                        qDebug() << this->objectName() << "converged at beta" << beta << "and probability" << theFailureProbability << "after" << iterationIndex << "iterations";
                    }
                }

                return 0;
            }
        }


        // If no convergence, take a step with a search direction and a step size
        theSearchDirection->getSearchDirection(&u, functionValue, &gradientInStandardNormalSpace, &searchDirection);
        stepSize = theLineSearchAlgorithm->findPoint(this);

        for (int i=0; i<numRVs; i++) {

            u[i] = u.at(i) + stepSize * searchDirection.at(i);
        }

        iterationIndex++;
    }

    return -1;
}


double RFORMModel::evaluateLimitStateFunction()
{
    // Re-set the "is-analyzed" flags in the models, to make sure we start afresh
    theDomain->clearAnalyzedFlags();

    double functionValue;

    RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameter);
    if (!theInputCastAsResponse) {
        qCritical() << this->objectName() << "needs the input parameter to be a generic response";
        return -1;
    }
    RModel *theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    theModel->runAnalysis(theGradientType);
    functionValue = this->getThresholdValue() - theInputParameter->getCurrentValue();

    return functionValue;
}


double RFORMModel::evaluate_F(double parameterValue)
{
    // Initial declarations
    QVector<double> step(numRVs);
    QVector<double> newPoint(numRVs);
    QVector<double> newPointInOriginalSpace(numRVs);


    // New point in the standard normal space, implied by parameterValue
    for (int i=0; i<numRVs; i++) {

        newPoint[i] = u.at(i) + parameterValue * searchDirection.at(i);
    }


    // Transform that point into the original space
    theProbabilityTransformation->transformPointToOriginalSpace(&newPoint, &newPointInOriginalSpace);


    // Evaluate the objective function at that point namely the norm || oldPoint + searchDirection * parameterValue ||
    RMatrixOperations theMatrixOperations(theDomain);
    double objective = theMatrixOperations.norm(&newPoint);


    // Set the value of the random variables to the new trial point
    for (int i = 0; i < numRVs; i++) {

        theRVs[i]->setCurrentValue(newPointInOriginalSpace.at(i));
    }


    // Evaluate the single equality constraint, namely the limit-state function, and put it into a vector
    double g = evaluateLimitStateFunction();
    QVector<double> equalityConstraints(1);
    equalityConstraints[0] = fabs(g);
    QVector<double> inequalityConstraints;


    // Evaluate the merit function
    return theMeritFunction->getMeritFunctionValue(objective, &equalityConstraints, &inequalityConstraints);
}


double RFORMModel::firstOrderFiniteDifference()
{
    qCritical() << "This is blank in FORM for now...";
    return 0.0;
}


double RFORMModel::secondOrderFiniteDifference()
{
    qCritical() << "This is blank in FORM for now...";
    return 0.0;
}




