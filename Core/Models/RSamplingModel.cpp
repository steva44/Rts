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

#include "RSamplingModel.h"
#include "RModel.h"
#include "RRandomNumberGenerator.h"
#include "RResponse.h"
#include "RDomain.h"
#include "RMainWindow.h"
#include "RRandomVariable.h"
#include "RProbabilityTransformation.h"
#include "RMatrixOperations.h"
#include "RNormalDistribution.h"

#include <QCoreApplication>
#include <QPointer>
#include <QDateTime>
#include <QElapsedTimer>
#include <math.h>
#include "qcustomplot.h"

RSamplingModel::RSamplingModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theInputParameter = nullptr;
    theThresholdParameter = nullptr;
    theRandomNumberGenerator = nullptr;
    theProbabilityTransformation = nullptr;

    isOrchestratingModel = true;

    theMatrixOperations = std::make_unique<RMatrixOperations>(theDomain);

    theDomain->createObject(objectName() + "ProbabilityResponse", "RGenericResponse");
    theProbabilityResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theProbabilityResponse->setModel(this);

    theDomain->createObject(objectName() + "BetaResponse", "RGenericResponse");
    theBetaResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theBetaResponse->setModel(this);

    theDomain->createObject(objectName() + "MeanResponse", "RGenericResponse");
    theMeanResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theMeanResponse->setModel(this);

    theDomain->createObject(objectName() + "StdvResponse", "RGenericResponse");
    theStdvResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theStdvResponse->setModel(this);

    sampleNumber = 0;
    theThresholdDouble = 0.0;
    maxSamples = 10000;
    thePlotInterval = 100;
    numBins = 60;
    targetCov = 0.02;

    theSamplingCentre = "Origin";
}


RSamplingModel::~RSamplingModel()
{

}


QObject * RSamplingModel::getInputParameter() const
{
    return theInputParameter;
}


void RSamplingModel::setInputParameter(QObject *value)
{
    theInputParameter = qobject_cast<RParameter *>(value);
}

QString RSamplingModel::getThreshold()
{
    return getStringParameter(&theThresholdDouble, theThresholdParameter);
}


void RSamplingModel::setThreshold(QString value)
{
    setStringParameter(value, &theThresholdDouble, &theThresholdParameter);
}


double RSamplingModel::getThresholdValue()
{
    return getDoubleParameter(&theThresholdDouble, theThresholdParameter);
}


int RSamplingModel::getMaxSamples() const
{
    return maxSamples;
}


void RSamplingModel::setMaxSamples(int value)
{
    if (maxSamples < 1) {
        qDebug() << "The sampling model requires the maximum number of samples to be greater than zero.";
        maxSamples = 1;
        return;
    }

    maxSamples = value;
}


int RSamplingModel::getPlotInterval() const
{
    return thePlotInterval;
}


void RSamplingModel::setPlotInterval(int value)
{
    if (thePlotInterval < 1) {
        qDebug() << "The sampling model requires the plot frequency to be greater than zero.";
        thePlotInterval = 1;
        return;
    }

    thePlotInterval = value;
}


double RSamplingModel::getTargetCov() const
{
    return targetCov;
}


void RSamplingModel::setTargetCov(double value)
{
    if (targetCov < 0.0) {
        qDebug() << "The sampling model requires the target c.o.v. to be greater than zero.";
        targetCov = 0.02;
        return;
    }

    targetCov = value;
}


QObject * RSamplingModel::getRandomNumberGenerator() const
{
    return theRandomNumberGenerator;
}


void RSamplingModel::setRandomNumberGenerator(QObject *value)
{
    theRandomNumberGenerator = qobject_cast<RRandomNumberGenerator *>(value);
}


QObject * RSamplingModel::getProbabilityTransformation() const
{
    return theProbabilityTransformation;
}


void RSamplingModel::setProbabilityTransformation(QObject *value)
{
    theProbabilityTransformation = qobject_cast<RProbabilityTransformation *>(value);
}


QString RSamplingModel::getSamplingCentre()
{
    return theSamplingCentre;
}


void RSamplingModel::setSamplingCentre(QString value)
{
    if (value.toLower().startsWith("origin")) {
        theSamplingCentre = "Origin";
    }
    else if (value.toLower().startsWith("current")) {
        theSamplingCentre = "Current Values";
    }
    else {
        qCritical() << "The sampling centre must be either Origin or Current Values.";
    }
}


QStringList RSamplingModel::getActionList()
{
    // Notice that the "run" and "help" methods are implemented in the base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Help";
    return actionList;
}


int RSamplingModel::evaluateModel(RGradientType theGradientType)
{
    //  Start timer
    if (theOutputLevel >= RObject::Medium) {
        qDebug() << "The analysis in" << this->objectName() << "started...";
        qDebug() << "";
    }
    QDateTime startTime = QDateTime::currentDateTime();


    if(theProbabilityTransformation == nullptr || theRandomNumberGenerator == nullptr)
    {
        qDebug() << "Error, the sampling model " << this->objectName() << "needs both a probability transformation and a random number generator.";
        return -1;
    }


    //  Get a list of random variables
    theRVs.clear();
    theRVs = theDomain->findChildren<RRandomVariable *>();

    int numRVs = theRVs.size();
    QList<RParameter *> theRVsCastAsParameters;
    for (int i=0; i<numRVs; i++) {
        theRVsCastAsParameters << qobject_cast<RParameter *>(theRVs.at(i));

        // This is necessary in analyses with mean of random variables being decision variables:
        theRVs.at(i)->updateParameters();
    }
    theProbabilityTransformation->setRandomVariableList(&theRVs);


    // Reset the random number generator
    theRandomNumberGenerator->reset();


    // Initial declarations
    u.clear();
    u.resize(numRVs);

    uCentre.clear();
    uCentre.resize(numRVs);

    x.clear();
    x.resize(numRVs);

    QVector<double> uMinusSamplingCentre(numRVs);
    double limitStateFunctionValue, parameterFunctionValue;
    double parameterSum = 0.0;
    double parameterSquaredSum = 0.0;
    double qSum = 0.0;
    double qSquaredSum = 0.0;
    double qBar = 0.0;
    double qBarVariance = 0.0;
    double qBarCOV = 1.0e12;
    bool failureHasOccurred = false;
    double beta = 0.0;
    double mean = 0.0;
    double stdv = 0.0;
    double I;
    double phi_h;
    double q;
    double norm;
    double uNorm;
    double covPlotLimit = 0.0;
    double pfPlotLimit = 0.0;


    // Declare a couple of tools internally
    RNormalDistribution theNormalDistribution(theDomain);


    // Keep a copy of the random variables
    QVector<double> randomVariablesBackup;
    for (int i = 0; i < numRVs; i++) {
        randomVariablesBackup.append(theRVs[i]->getCurrentValue());
    }

    // Set the sampling centre
    if (theSamplingCentre == QString("Origin"))
    {
        // Leave the uCentre vector as is, at zero
    }
    else {

        for (int i = 0; i < numRVs; i++) {
            x[i] = theRVs[i]->getCurrentValue();
        }
        theProbabilityTransformation->transformPointToStandardNormalSpace(&x, &uCentre);

    }


    // Create the plots
    double histogramHeight = 0.0;
    double histPlotHeight = 0.0;
    double histogramArea;
    QVector<double> firstData(100);
    QVector<double> theHistogram(numBins);
    QVector<double> theFrequencyDiagram(numBins);
    QVector<double> theHistogramTicks(numBins);
    double histogramMin = 0.0;
    double histogramMax = 0.0;
    double binSize = 1.0;

    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());


    if (thePlotInterval < maxSamples && theOutputLevel >= RObject::Medium) {


        // CREATE COV/PF PLOT
        if (customPlot == nullptr) {

            customPlot = new QCustomPlot();
            mainWindow->addSubWindow(customPlot, "Sampling Evolution Plot", true);

            // Create graph for COV
            customPlot->addGraph();
            customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            customPlot->xAxis->setLabel("Number of Samples");
            customPlot->yAxis->setLabel("C.o.V.");
            customPlot->yAxis->setLabelColor(QColor(0, 0, 255, 255));

            // Create graph for pf
            customPlot->addGraph(nullptr, customPlot->yAxis2);
            customPlot->graph(1)->setPen(QColor(255, 0, 0, 255));
            customPlot->yAxis2->setVisible(true);
            customPlot->yAxis2->setLabelColor(QColor(255, 0, 0, 255));
            customPlot->yAxis2->setLabel("Probability");
        }
        else {

            customPlot->graph(0)->clearData();
            customPlot->graph(1)->clearData();

        }

        // CREATE HISTOGRAM PLOT
        if (customPlot2 == nullptr) {

            customPlot2 = new QCustomPlot();
            mainWindow->addSubWindow(customPlot2, "Frequency Diagram", true);


            customPlot2->addGraph();
            customPlot2->graph(0)->setPen(QColor(0, 0, 0, 255));
            customPlot2->xAxis->setLabel("Parameter Value");
            customPlot2->yAxis->setLabel("Relative frequency");

        }
        else {

            customPlot2->graph(0)->clearData();
        }

    }



    // Start sampling loop
    sampleNumber=0;
    while ( (qBarCOV > targetCov || qBarCOV == 0.0) && sampleNumber < maxSamples) {

        ++sampleNumber;

        // qDebug()<<"Sample number: "<<i;
        QElapsedTimer myTimer;
        myTimer.start();

        // KEEP GUI RESPONSIVE
        QCoreApplication::processEvents();


        // GENERATE STANDARD NORMAL NUMBERS
        theRandomNumberGenerator->generateStandardNormalNumbers(&u);


        // SHIFT RELATIVE TO DESIGN POINT
        if (theSamplingCentre != QString("Origin"))
        {
            theMatrixOperations->addVectortoVector(&uCentre,&u);
        }


        // TRANSFORM INTO ORIGINAL SPACE
        theProbabilityTransformation->transformPointToOriginalSpace(&u, &x);


        // GIVE NUMBERS TO RANDOM VARIABLES
        for (int j=0; j<numRVs; j++) {
            theRVs[j]->setCurrentValue(x.at(j));
            // qDebug()<<"For the RV "<<theRVs[j]->objectName()<<" the value is "<<x.at(j);
        }


        // EVALUATE BOTH FUNCTIONS
        limitStateFunctionValue = evaluateLimitStateFunction();
        parameterFunctionValue = theInputParameter->getCurrentValue(); // Because the analysis was run in the previous line


        // COMPUTE THE CORRECTION FACTOR FOR NON-CENTRED SAMPLING DISTRIBUTION
        uNorm = theMatrixOperations->norm(&u);
        for (int j = 0; j<numRVs; j++) {
            uMinusSamplingCentre[j] = u.at(j) - uCentre.at(j);
        }
        norm = theMatrixOperations->norm(&uMinusSamplingCentre);
        phi_h = exp(0.5*(norm*norm - uNorm*uNorm));

        auto sampleNumAsDouble = static_cast<double>(sampleNumber);

        // COMPUTE STATISTICS FOR THE PARAMETER FUNCTION
        parameterSum += parameterFunctionValue*phi_h;
        parameterSquaredSum += (parameterFunctionValue*phi_h) * (parameterFunctionValue*phi_h);
        mean = parameterSum/sampleNumAsDouble;
        if (sampleNumber>1) {
            stdv = sqrt(1.0/(sampleNumAsDouble-1.0) * (parameterSquaredSum - sampleNumAsDouble * mean * mean));
        }


        if (thePlotInterval < maxSamples && theSamplingCentre == QString("Origin") && theOutputLevel >= RObject::Medium)
        {

            // MAINTAIN HISTOGRAM
            if (sampleNumber<100) {
                firstData[sampleNumber-1] = parameterFunctionValue;
            }
            else if (sampleNumber==100) {
                firstData[sampleNumber-1] = parameterFunctionValue;

                // Create first histogram
                histogramMin = mean - 5.0 * stdv;
                histogramMax = mean + 5.0 * stdv;
                binSize = (histogramMax - histogramMin) / numBins;

                customPlot2->xAxis->setRange(histogramMin-binSize, histogramMax+binSize);

                // Set bin ticks
                for (int k=0; k<numBins; k++) {

                    theHistogramTicks[k] = histogramMin + k * binSize - 0.5 * binSize;
                }

                // Create histogram
                for (int j=0; j<100; j++) {

                    for (int k=0; k<numBins; k++) {

                        if (firstData.at(j) < histogramMin + (k+1) * binSize) {

                            theHistogram[k] += 1.0;

                            if (theHistogram[k] > histogramHeight) {
                                histogramHeight += 1.0;
                            }
                            break;
                        }
                    }
                }
            }
            else {

                // Update histogram
                for (int k=0; k<numBins; k++) {

                    if (parameterFunctionValue < histogramMin + (k+1) * binSize) {

                        theHistogram[k] += 1.0;

                        if (theHistogram[k] > histogramHeight) {
                            histogramHeight += 1.0;
                        }
                        break;
                    }
                }

            }
        }

        histogramArea = sampleNumAsDouble*binSize;

        if (histogramHeight/histogramArea > histPlotHeight) {

            histPlotHeight = histogramHeight/histogramArea*1.1;
        }


        //  COMPUTE FAILURE PROBABILITY FOR THE LIMIT-STATE FUNCTION
        if (limitStateFunctionValue < 0.0) {
            I = 1;
            failureHasOccurred = true;
        } else {
            I = 0;
        }

        if (failureHasOccurred) {

            // Update sums
            q = I * phi_h;
            qSum += q;
            qSquaredSum += q*q;

            if (qSum > 0.0) {

                // Compute coefficient of variation (of pf)
                qBar = qSum/sampleNumAsDouble;
                qBarVariance = (qSquaredSum/sampleNumAsDouble - qBar*qBar)/sampleNumAsDouble;
                if (qBarVariance < 0.0) {
                    qBarVariance = 0.0;
                }
                qBarCOV = sqrt(qBarVariance)/qBar;
            }

        }


        if (thePlotInterval < maxSamples && theOutputLevel >= RObject::Medium)
        {

            // PLOTTING AND OUTPUT
            if ( remainder(sampleNumber, thePlotInterval) == 0.0) {

                if (theOutputLevel == RObject::Maximum) {

                    beta = -theNormalDistribution.getInverseNormalCDF(qBar, 1.0);

                    qDebug() << "Sampling in" << this->objectName() << "at iteration" << sampleNumber << "has Probability:" << qBar << "Beta:" << beta << "C.o.v.:" << qBarCOV << "Mean:" << mean << "Stdv:" << stdv;
                    qDebug() << "";
                }



                // PLOT COV / PF
                if (failureHasOccurred) {

                    customPlot->graph(0)->addData(sampleNumber, qBarCOV);
                    customPlot->graph(1)->addData(sampleNumber, qBar);

                    // Set x-axis range
                    customPlot->xAxis->setRange(0, sampleNumber+100);

                    // Set left y-axis range
                    if (qBarCOV > covPlotLimit) {

                        covPlotLimit = 1.1*qBarCOV;
                        customPlot->yAxis->setRange(0, covPlotLimit);
                    }

                    // Set right y-axis range
                    if (qBar > pfPlotLimit) {

                        pfPlotLimit = 1.1*qBar;
                        customPlot->yAxis2->setRange(0, pfPlotLimit);
                    }

                    customPlot->replot();
                }


                // PLOT HISTOGRAM
                if (theSamplingCentre == QString("Origin")) {
                    customPlot2->yAxis->setRange(0, histPlotHeight);
                    theMatrixOperations->scaleVector(1.0/histogramArea, &theHistogram, &theFrequencyDiagram);
                    customPlot2->graph(0)->setData(theHistogramTicks, theFrequencyDiagram);
                    customPlot2->replot();
                }

            }
        }

        // qDebug() << "Iteration number" << i << "completed in" << myTimer.elapsed()/1000.0 << "seconds.";

    } // End sampling loop


    // Set output & responses from the sampling analysis
    beta = -theNormalDistribution.getInverseNormalCDF(qBar, 1.0);

    theProbabilityResponse->setCurrentValue(qBar);
    theBetaResponse->setCurrentValue(beta);
    theMeanResponse->setCurrentValue(mean);
    theStdvResponse->setCurrentValue(stdv);


    // STOP TIMER
    QDateTime finishTime = QDateTime::currentDateTime();
    double numDays = startTime.daysTo(finishTime);
    double numMilliSecs = startTime.time().msecsTo(finishTime.time());
    double numSecs = numDays * 86400.0 + numMilliSecs / 1000.0;
    if (theOutputLevel >= RObject::Medium) {

        qDebug() << "Sampling in" << this->objectName() << "ended after" << sampleNumber << "iterations with pf=" << qBar << "beta=" << beta << "cov=" << qBarCOV << "mean=" << mean << "stdv=" << stdv;
        qDebug() << "";
        qDebug() << "The analysis in" << this->objectName() << "completed in" << numSecs << "seconds.";
        qDebug() << "";
    }


    // Restore the currentValue of the random variables from the storedValue
    for (int i = 0; i < numRVs; i++) {
        theRVs[i]->setCurrentValue(randomVariablesBackup[i]);
    }


    // Give a message if failure did not occur before the end
    if (!failureHasOccurred) {
        qDebug() << "In" << this->objectName() << "there was no failure event before the sampling ended.";
    }


    // Print normalized relative frequency diagram if requested
    if (theOutputLevel == RObject::Maximum) {
        qDebug() << "Cost-value     PDF-value";
        for (int i=0; i<numBins; i++) {
            QString myString = QString("%1,%2").arg(theHistogramTicks.at(i), 0, 'g', 13).arg(theFrequencyDiagram.at(i), 0, 'g', 13);
            qDebug().noquote() << myString;
        }
    }

    return 0;
}


QString RSamplingModel::getTheResponses()
{
    QString string;
    for (int i = 0; i < theResponses.count(); i++)
    {
        string += theResponses[i];
        string += "; ";
    }

    return string;
}


void RSamplingModel::setTheResponses(QString value)
{
    QRegExp regExp = QRegExp(QString("[ ,;:\t\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator)));
    QStringList stringList = value.split(regExp, QString::SkipEmptyParts);

    for (int i = 0; i < stringList.count(); i++)
    {

        theResponses.append(stringList[i]);
    }
}


double RSamplingModel::evaluateLimitStateFunction()
{
    // Re-set the "is-analyzed" flags in the models, to make sure we start afresh
    theDomain->clearAnalyzedFlags();

    double functionValue = 0.0;

    RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameter);
    if (!theInputCastAsResponse) {
        qCritical() << this->objectName() << "needs the input parameter to be a generic response";
        return -1;
    }
    RModel *theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    theModel->runAnalysis(RGradientType::NONE);
    functionValue = this->getThresholdValue() - theInputParameter->getCurrentValue();

    return functionValue;
}


int RSamplingModel::getSampleNumber() const
{
    return sampleNumber;
}

