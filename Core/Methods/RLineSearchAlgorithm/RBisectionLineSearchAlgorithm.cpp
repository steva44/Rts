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

#include "RBisectionLineSearchAlgorithm.h"
#include "RLineSearchCallBackAssistant.h"
#include "RDomain.h"
#include "RMainWindow.h"

#include <QCoreApplication>
#include <math.h>

RBisectionLineSearchAlgorithm::RBisectionLineSearchAlgorithm(QObject *parent, QString name)
    : RLineSearchAlgorithm(parent, name)
{
    theLowerBound = -100.0;
    theUpperBound =  100.0;
    theTolerance =  0.01;
    theMaxIterations = 100;
    theDelay =  0.0;
    customPlot = nullptr;
}


RBisectionLineSearchAlgorithm::~RBisectionLineSearchAlgorithm()
{

}


double RBisectionLineSearchAlgorithm::getLowerBound() const
{
    return theLowerBound;
}


void RBisectionLineSearchAlgorithm::setLowerBound(double value)
{
    theLowerBound = value;
}


double RBisectionLineSearchAlgorithm::getUpperBound() const
{
    return theUpperBound;
}


void RBisectionLineSearchAlgorithm::setUpperBound(double value)
{
    theUpperBound = value;
}


double RBisectionLineSearchAlgorithm::getTolerance() const
{
    return theTolerance;
}


void RBisectionLineSearchAlgorithm::setTolerance(double value)
{
    theTolerance = value;
}


int RBisectionLineSearchAlgorithm::getMaxIterations() const
{
    return theMaxIterations;
}


void RBisectionLineSearchAlgorithm::setMaxIterations(int value)
{
    theMaxIterations = value;
}


double RBisectionLineSearchAlgorithm::getDelay() const
{
    return theDelay;
}


void RBisectionLineSearchAlgorithm::setDelay(double value)
{
    theDelay = value;
}


double RBisectionLineSearchAlgorithm::findPoint(RLineSearchCallBackAssistant *theCallBackAssistant)
{
    // Create plot
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    QVector<double> tickVector;
    tickVector << 0;
    QVector<QString> labelVector;
    labelVector << QString("%1").arg(0);
    if (theOutputLevel == RObject::Maximum) {

        if (customPlot == nullptr) {

            customPlot = new QCustomPlot();
            mainWindow->addSubWindow(customPlot, "Function Plot", true);
            customPlot->addGraph();
            customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue, 10));
            customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
            customPlot->xAxis->setLabel("Parameter");
            customPlot->yAxis->setLabel("Function");
        }
        else {

            customPlot->graph(0)->clearData();
        }
    }


    // Initial declarations
    bool convergence = false;
    double trialPoint, optimum;
    double f_lower;
    double f_trial;
    int counter = 0;


    // Check that the upper bound is greater than the lower bound
    if (theUpperBound <= theLowerBound) {
        qCritical() << "The lower bound must be smaller than the upper bound in" << this->objectName();
        return 0;
    }


    // Evaluate f at the lowerbound (the lower bound to trial point is used as reference interval to test where the solution is)
    f_lower = theCallBackAssistant->evaluate_f(theLowerBound);
    bool f_lowerNeedsEvaluation = false;


    // Check that the solution is indeed between the lower and upper bounds
    double f_upper = theCallBackAssistant->evaluate_f(theUpperBound);
    if ( !(f_lower * f_upper < 0.0) ) {

        qCritical() << "There does not seem to be a solution between the provided lower and upper bounds in" << this->objectName();
        return 0;
    }


    // Add points to the plot
    if (theOutputLevel == RObject::Maximum) {

        customPlot->graph(0)->addData(theLowerBound, f_lower);
        customPlot->graph(0)->addData(theUpperBound, f_upper);
        customPlot->rescaleAxes();
        customPlot->replot();
    }


    // Start the loop
    while (!convergence) {


        // Implement a delay
        QTime dieTime= QTime::currentTime().addSecs(theDelay);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


        // Increment counter
        counter++;


        // Exit if we are at max number of iterations
        if (counter > theMaxIterations) {

            qCritical() << this->objectName() << "reached the maximum number of iterations without convergence";
            return 0.0;
        }


        // Output
        if (theOutputLevel == RObject::Maximum) {
            qDebug() << "At step" << counter << this->objectName() << "has lower bound =" << theLowerBound << " upper bound =" << theUpperBound;
        }


        // Calculate trialPoint, the midpoint between the lower and upper bounds
        trialPoint = (theUpperBound + theLowerBound) / 2.0;


        // Evaluate f at trialPoint
        f_trial = theCallBackAssistant->evaluate_f(trialPoint);


        // Add point to the plot
        if (theOutputLevel == RObject::Maximum) {

            customPlot->graph(0)->addData(trialPoint, f_trial);
            customPlot->rescaleAxes();
            customPlot->replot();
        }


        // Evaluate f at the lower bound if needed, and if so, add the point to the plot
        if (f_lowerNeedsEvaluation) {

            f_lower = theCallBackAssistant->evaluate_f(theLowerBound);

            if (theOutputLevel == RObject::Maximum) {

                customPlot->graph(0)->addData(theLowerBound, f_lower);
                customPlot->rescaleAxes();
                customPlot->replot();
            }

        }


        // Check if trialPoint should replace the lower or the upper bound
        if (f_lower * f_trial < 0.0) {
            theUpperBound = trialPoint;
            f_lowerNeedsEvaluation = false;
        }
        else {
            theLowerBound = trialPoint;
            f_lowerNeedsEvaluation = true;
        }


        // Check convergence
        if (fabs(theUpperBound - theLowerBound) < theTolerance) {

            optimum = trialPoint;
            convergence = true;
        }

    }

    return optimum;
}

