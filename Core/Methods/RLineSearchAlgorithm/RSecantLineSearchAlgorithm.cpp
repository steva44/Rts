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

#include "RSecantLineSearchAlgorithm.h"
#include "RLineSearchCallBackAssistant.h"
#include "RDomain.h"
#include "RMainWindow.h"
#include <QCoreApplication>
#include <math.h>

RSecantLineSearchAlgorithm::RSecantLineSearchAlgorithm(QObject *parent, QString name)
    : RLineSearchAlgorithm(parent, name)
{
    theLowerBound = -100.0;
    theUpperBound =  100.0;
    theTolerance =  0.01;
    theMaxIterations = 100;
    theDelay =  0.0;
    customPlot = nullptr;
}


RSecantLineSearchAlgorithm::~RSecantLineSearchAlgorithm()
{

}


double RSecantLineSearchAlgorithm::getLowerBound() const
{
    return theLowerBound;
}


void RSecantLineSearchAlgorithm::setLowerBound(double value)
{
    theLowerBound = value;
}


double RSecantLineSearchAlgorithm::getUpperBound() const
{
    return theUpperBound;
}


void RSecantLineSearchAlgorithm::setUpperBound(double value)
{
    theUpperBound = value;
}


double RSecantLineSearchAlgorithm::getTolerance() const
{
    return theTolerance;
}


void RSecantLineSearchAlgorithm::setTolerance(double value)
{
    theTolerance = value;
}


int RSecantLineSearchAlgorithm::getMaxIterations() const
{
    return theMaxIterations;
}


void RSecantLineSearchAlgorithm::setMaxIterations(int value)
{
    theMaxIterations = value;
}


double RSecantLineSearchAlgorithm::getDelay() const
{
    return theDelay;
}


void RSecantLineSearchAlgorithm::setDelay(double value)
{
    theDelay = value;
}


double RSecantLineSearchAlgorithm::findPoint(RLineSearchCallBackAssistant *theCallBackAssistant)
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
    double f_lowerBound;
    double f_upperBound;
    double f_trialPoint;
    int counter = 0;


    // Check that the upper bound is greater than the lower bound
    if (theUpperBound <= theLowerBound) {
        qCritical() << "The lower bound must be smaller than the upper bound in" << this->objectName();
        return 0;
    }


    // Evaluate the gradient at the two bounds
    f_lowerBound = theCallBackAssistant->evaluate_f(theLowerBound);
    f_upperBound = theCallBackAssistant->evaluate_f(theUpperBound);


    // Add points to the plot
    if (theOutputLevel == RObject::Maximum) {

        customPlot->graph(0)->addData(theLowerBound, f_lowerBound);
        customPlot->graph(0)->addData(theUpperBound, f_upperBound);
        customPlot->rescaleAxes();
        customPlot->replot();
    }


    // Check that the solution is indeed between the lower and upper bounds
    if ( !(f_lowerBound * f_upperBound < 0.0) ) {

        qCritical() << "There does not seem to be a solution between the provided lower and upper bounds in" << this->objectName();
        return 0;
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


        // Calculate trialPoint as the point where the secant crosses the line
        trialPoint = theLowerBound - f_lowerBound * ((theUpperBound - theLowerBound) / (f_upperBound - f_lowerBound));


        // Evaluate f at trialPoint
        f_trialPoint = theCallBackAssistant->evaluate_f(trialPoint);


        // Add point to the plot
        if (theOutputLevel == RObject::Maximum) {

            customPlot->graph(0)->addData(trialPoint, f_trialPoint);
            customPlot->rescaleAxes();
            customPlot->replot();
        }


        // Check convergence
        if (fabs(f_trialPoint) < theTolerance) {

            optimum = trialPoint;
            convergence = true;
        }


        // Check if trialPoint should replace the lower or the upper bound
        if (f_lowerBound * f_trialPoint < 0.0) {
            theUpperBound = trialPoint;
            f_upperBound = f_trialPoint;
        }
        else {
            theLowerBound = trialPoint;
            f_lowerBound = f_trialPoint;
        }
    }

    return optimum;
}

