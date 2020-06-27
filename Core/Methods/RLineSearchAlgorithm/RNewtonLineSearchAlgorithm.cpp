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

#include "RNewtonLineSearchAlgorithm.h"
#include "RLineSearchCallBackAssistant.h"
#include "RDomain.h"
#include "RMainWindow.h"

#include <QCoreApplication>
#include <math.h>

RNewtonLineSearchAlgorithm::RNewtonLineSearchAlgorithm(QObject *parent, QString name)
    : RLineSearchAlgorithm(parent, name)
{
    theStartPoint = 100.0;
    theTolerance =  0.01;
    theMaxIterations = 100;
    theDelay =  0.0;
    customPlot = 0;
}


RNewtonLineSearchAlgorithm::~RNewtonLineSearchAlgorithm()
{

}


double RNewtonLineSearchAlgorithm::getStartPoint() const
{
    return theStartPoint;
}


void RNewtonLineSearchAlgorithm::setStartPoint(double value)
{
    theStartPoint = value;
}


double RNewtonLineSearchAlgorithm::getTolerance() const
{
    return theTolerance;
}


void RNewtonLineSearchAlgorithm::setTolerance(double value)
{
    theTolerance = value;
}


int RNewtonLineSearchAlgorithm::getMaxIterations() const
{
    return theMaxIterations;
}


void RNewtonLineSearchAlgorithm::setMaxIterations(int value)
{
    theMaxIterations = value;
}


double RNewtonLineSearchAlgorithm::getDelay() const
{
    return theDelay;
}


void RNewtonLineSearchAlgorithm::setDelay(double value)
{
    theDelay = value;
}


double RNewtonLineSearchAlgorithm::findPoint(RLineSearchCallBackAssistant *theCallBackAssistant)
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
    double x = theStartPoint;
    double xPrevious;
    double f, df;
    int counter = 0;


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


        // Initialize the search
        xPrevious = x;


        // Evaluate the ingredients of the Newton fraction
        f = theCallBackAssistant->evaluate_f(x);
        df = theCallBackAssistant->evaluate_h(x);


        // Add point to the plot
        if (theOutputLevel == RObject::Maximum) {

            customPlot->graph(0)->addData(x, f);
            customPlot->rescaleAxes();
            customPlot->replot();
        }


        // Evaluate the Newton formula
        x = xPrevious - f / df;



        // Output
        if (theOutputLevel == RObject::Maximum) {
            qDebug() << "At step" << counter << this->objectName() << "has parameter value" << x;
        }


        // Check convergence
        if (fabs(x - xPrevious) < theTolerance) {

            convergence = true;
        }
    }

    return x;
}

