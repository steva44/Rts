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

#include "RDownhillSimplexOptimizationAlgorithm.h"
#include "RMatrixOperations.h"
#include "RDecisionVariable.h"
#include "RResponse.h"
#include "RMainWindow.h"

#include <math.h>

RDownhillSimplexOptimizationAlgorithm::RDownhillSimplexOptimizationAlgorithm(QObject *parent, QString name)
    : ROptimizationAlgorithm(parent, name)
{
    theAlpha = 1.0;
    theGamma = 2.0;
    theRho = -0.5;
    theSigma = 0.5;
    theTolerance = 0.00001;
    theMaxSteps = 100;
    theDelay =  0.0;
}


RDownhillSimplexOptimizationAlgorithm::~RDownhillSimplexOptimizationAlgorithm()
{

}


double RDownhillSimplexOptimizationAlgorithm::getAlpha() const
{
    return theAlpha;
}


void RDownhillSimplexOptimizationAlgorithm::setAlpha(double value)
{
    theAlpha = value;
}


double RDownhillSimplexOptimizationAlgorithm::getGamma() const
{
    return theGamma;
}


void RDownhillSimplexOptimizationAlgorithm::setGamma(double value)
{
    theGamma = value;
}


double RDownhillSimplexOptimizationAlgorithm::getRho() const
{
    return theRho;
}


void RDownhillSimplexOptimizationAlgorithm::setRho(double value)
{
    theRho = value;
}


double RDownhillSimplexOptimizationAlgorithm::getSigma() const
{
    return theSigma;
}


void RDownhillSimplexOptimizationAlgorithm::setSigma(double value)
{
    theSigma = value;
}


int RDownhillSimplexOptimizationAlgorithm::getMaxSteps() const
{
    return theMaxSteps;
}


void RDownhillSimplexOptimizationAlgorithm::setMaxSteps(int value)
{
    theMaxSteps = value;
}


double RDownhillSimplexOptimizationAlgorithm::getTolerance() const
{
    return theTolerance;
}


void RDownhillSimplexOptimizationAlgorithm::setTolerance(double value)
{
    theTolerance = value;
}


double RDownhillSimplexOptimizationAlgorithm::getDelay() const
{
    return theDelay;
}


void RDownhillSimplexOptimizationAlgorithm::setDelay(double value)
{
    theDelay = value;
}


int RDownhillSimplexOptimizationAlgorithm::solveOptimizationProblem(QPointer<RParameter> objective)
{
    // Set class variables
    theObjective = objective;
    theDVs = theDomain->findChildren<RDecisionVariable *>();
    int numDVs = theDVs.size();

    if(numDVs == 0)
    {
        qCritical()<<"Error, the number of decision variables has to be greater than zero";
        return -1;
    }


    // Create plot
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    QVector<double> tickVector;
    tickVector << 0;
    QVector<QString> labelVector;
    labelVector << QString("%1").arg(0);
    if (theOutputLevel >= RObject::Medium && numDVs == 2) {

        if (customPlot == nullptr) {

            customPlot = std::make_unique<QCustomPlot>();
            mainWindow->addSubWindow(customPlot.get(), "Simplex Evolution Plot", true);

            customPlot->xAxis->setLabel("Design Variable 1: "+ theDVs.at(0)->objectName());
            customPlot->yAxis->setLabel("Design Variable 2: " + theDVs.at(1)->objectName());

            auto range1 = fabs(theDVs.at(0)->getCurrentValue())*1.20;
            auto range2 = fabs(theDVs.at(1)->getCurrentValue())*1.20;

            customPlot->xAxis->setRange(-range1, range1);
            customPlot->yAxis->setRange(-range2, range2);

            customPlot->addGraph();
            customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::red, 7));
            customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
        }
    }


    // Build a matrix of x-vectors
    QVector< QVector<double> > xVectors(numDVs+1, QVector<double>(numDVs));


    // Let the start point be the first x-vector
    for (int i=0; i<numDVs; i++) {

        xVectors[0][i] = theDVs.at(i)->getCurrentValue();
    }


    // Build a simplex around the start point, creating each vertex by taking a step along each DV axis
    for (int i=1; i<numDVs+1; i++) {

        for (int j=0; j<numDVs; j++) {

            if (j==i-1) {

                xVectors[i][j] = theDVs.at(j)->getCurrentValue() + theAlpha;
            }
            else {

                xVectors[i][j] = theDVs.at(j)->getCurrentValue();
            }
        }
    }


    // Calculate F at each vertex
    QVector<double> F(numDVs+1);
    for (int i=0; i<numDVs+1; i++) {

        F[i] = evaluateObjective(&xVectors[i]);
    }


    // Start the loop
    int loopCounter = 0;
    bool convergence = false;
    bool reflectionBool, expansionBool, contractionBool, shrinkingBool;
    while ( !convergence && loopCounter < theMaxSteps) {


        // Increment the loop counter
        loopCounter++;


        // Implement a delay
        if(theDelay != 0.0)
        {
            QTime dieTime= QTime::currentTime().addSecs(theDelay);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }


        // Update plot
        if (theOutputLevel >= RObject::Medium) {

            qDebug() << this->objectName() << "is now starting step number" << loopCounter << "with points and values:";
            for (int i=0; i<numDVs+1; i++) {

                qDebug() << xVectors[i][0] << xVectors[i][1] << F.at(i);
            }

            if (customPlot) {

                customPlot->graph(0)->clearData();

                if (reflectionBool) {
                    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::red, 7));
                }
                else if (expansionBool) {
                    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::green, Qt::green, 7));
                }
                else if (contractionBool) {
                    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue, 7));
                }
                else if (shrinkingBool) {
                    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::cyan, Qt::cyan, 7));
                }
                else {
                    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::black, Qt::black, 7));
                }
                customPlot->graph(0)->addData(xVectors[0][0], xVectors[0][1]);
                customPlot->graph(0)->addData(xVectors[1][0], xVectors[1][1]);
                customPlot->graph(0)->addData(xVectors[2][0], xVectors[2][1]);

                //reset the range of the plot
                QVector<double> Dvxvec = {xVectors[0][0], xVectors[1][0], xVectors[2][0]};
                QVector<double> Dvyvec = {xVectors[0][1], xVectors[1][1], xVectors[2][1]};

                auto rangeXMax = *std::max_element(Dvxvec.constBegin(), Dvxvec.constEnd());
                auto rangeYMax = *std::max_element(Dvyvec.constBegin(), Dvyvec.constEnd());

                auto rangeXMin = *std::min_element(Dvxvec.constBegin(), Dvxvec.constEnd());
                auto rangeYMin = *std::min_element(Dvyvec.constBegin(), Dvyvec.constEnd());

                customPlot->xAxis->setRange(rangeXMin*0.8, rangeXMax*1.20);
                customPlot->yAxis->setRange(rangeYMin*0.8, rangeYMax*1.20);

                customPlot->replot();
            }
        }


        // Reset recorders
        reflectionBool = false;
        expansionBool = false;
        contractionBool = false;
        shrinkingBool = false;


        // Get the index of the worst and second-worst point
        double worstValue = -1e+100;
        double bestValue = 1e+100;
        int worstIndex, secondWorstIndex, bestIndex;
        for (int i=0; i<numDVs+1; i++) {

            if (F.at(i) > worstValue) {

                worstValue = F.at(i);
                worstIndex = i;
            }

            if (F.at(i) < bestValue) {

                bestValue = F.at(i);
                bestIndex = i;
            }
        }
        double secondWorstValue = bestValue;
        for (int i=0; i<numDVs+1; i++) {

            if (F.at(i) < worstValue && F.at(i) > secondWorstValue) {

                secondWorstValue = F.at(i);
                secondWorstIndex = i;
            }
        }



        // Calculate the centroid, considering all points except the worst
        QVector<double> centroid(numDVs);
        for (int i=0; i<numDVs+1; i++) {

            if (i != worstIndex) {

                for (int j=0; j<numDVs; j++) {

                    centroid[j] += xVectors[i][j];
                }
            }
        }

        for (int j=0; j<numDVs; j++) {

            centroid[j] = centroid.at(j) / ((double)numDVs);
        }


        // REFLECTION *************************************
        QVector<double> reflection(numDVs);
        for (int i=0; i<numDVs; i++) {
            reflection[i] = centroid.at(i) + theAlpha * (centroid.at(i) - xVectors[worstIndex][i]);
        }
        double F_reflection = evaluateObjective(&reflection);
        // *************************************************


        // If the reflection point is better than the second worst, but not better than the best, accept it by replacing the worst point
        if (F_reflection < secondWorstValue && F_reflection > bestValue) {

            for (int i=0; i<numDVs; i++) {

                xVectors[worstIndex][i] = reflection.at(i);
                F[worstIndex] = F_reflection;
                reflectionBool = true;
            }
            if (theOutputLevel >= RObject::Medium) {
                qDebug() << this->objectName() << "accepted a reflection with F=" << F_reflection;
            }
        }


        // If the reflection point is the best of all, then try further expansion
        else if (F_reflection < bestValue) {


            // EXPANSION **************************************
            QVector<double> expansion(numDVs);
            for (int i=0; i<numDVs; i++) {
                expansion[i] = centroid.at(i) + theGamma * (reflection.at(i) - centroid.at(i));
            }
            double F_expansion = evaluateObjective(&expansion);
            // *************************************************


            // If the expansion point is better than the reflection point, accept the expansion by replacing the worst point
            if (F_expansion < F_reflection) {

                for (int i=0; i<numDVs; i++) {

                    xVectors[worstIndex][i] = expansion.at(i);
                    F[worstIndex] = F_expansion;
                    expansionBool = true;
                }
                if (theOutputLevel >= RObject::Medium) {
                    qDebug() << this->objectName() << "accepted an expansion with F=" << F_expansion;
                }
            }


            // If the expansion point isn't better than the reflection point, accept the reflection point by replacing the worst point
            else {

                for (int i=0; i<numDVs; i++) {

                    xVectors[worstIndex][i] = reflection.at(i);
                    F[worstIndex] = F_reflection;
                    reflectionBool = true;
                }
                if (theOutputLevel >= RObject::Medium) {
                    qDebug() << this->objectName() << "accepted a reflection with F=" << F_reflection;
                }
            }
        }


        // If the reflection point is neither best nor better than the second best, do contraction
        else {


            // CONTRACTION **************************************
            QVector<double> contraction(numDVs);
            for (int i=0; i<numDVs; i++) {
                contraction[i] = centroid.at(i) + theRho * (xVectors[worstIndex][i] - centroid.at(i));
            }
            double F_contraction = evaluateObjective(&contraction);
            // *************************************************


            // If the contraction point is better than the worst point, accept it by replacing the worst point
            if (F_contraction < worstValue) {

                for (int i=0; i<numDVs; i++) {

                    xVectors[worstIndex][i] = contraction.at(i);
                    F[worstIndex] = F_contraction;
                    contractionBool = true;
                }
                if (theOutputLevel >= RObject::Medium) {
                    qDebug() << this->objectName() << "accepted a contraction with F=" << F_contraction;
                }
            }


            // If not, then shrink
            else {


                // SHRINKING *************************************
                shrinkingBool = true;
                for (int i=0; i<numDVs+1; i++) {
                    if (i != bestIndex) {
                        for (int j=0; j<numDVs; j++) {
                            xVectors[i][j] = xVectors[bestIndex][j] + theSigma * (xVectors[i][j] - xVectors[bestIndex][j]);
                        }
                        F[i] = evaluateObjective(&xVectors[i]);
                    }
                }
                if (theOutputLevel >= RObject::Medium) {
                    qDebug() << this->objectName() << "did a shrink";
                }
                // *************************************************
            }
        }


        // Use max difference in F-values as convergence criterion for now
        if (qAbs(bestValue-worstValue)<theTolerance) {
            convergence = true;
            qDebug() << this->objectName() << "converged with objective" << F.at(0) << "and design variables:";
            for (int j=0; j<numDVs; j++) {

                qDebug() << xVectors[0][j];
            }
        }
        else
        {
            if (theOutputLevel >= RObject::Medium) {
                qDebug() << "The max difference between the best and worst value is:"<<qAbs(bestValue-worstValue);
            }
        }
    }

    return 0;
}


double RDownhillSimplexOptimizationAlgorithm::evaluateObjective(QVector<double> *point)
{
    // Update the values of the design variables
    for (int i=0; i<point->size(); i++) {
        theDVs.at(i)->setCurrentValue(point->at(i));
    }


    // Get the pointer to the model behind the objective function
    RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theObjective);
    RModel *theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());


    // Run that model using runAnalysis(), which also executes all upstream models
    theModel->runAnalysis(RGradientType::NONE);
    theDomain->clearAnalyzedFlags();


    return theObjective->getCurrentValue();
}

