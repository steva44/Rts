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

#include "RGoldenSectionLineSearchAlgorithm.h"
#include "RLineSearchCallBackAssistant.h"
#include "RDomain.h"
#include "RMainWindow.h"

#include <math.h>

RGoldenSectionLineSearchAlgorithm::RGoldenSectionLineSearchAlgorithm(QObject *parent, QString name)
    : RLineSearchAlgorithm(parent, name)
{
    theLowerBound = -100.0;
    theUpperBound =  100.0;
    theTolerance =  0.01;
    theMaxIterations = 100;
    theDelay =  0.0;
    customPlot = nullptr;
}


RGoldenSectionLineSearchAlgorithm::~RGoldenSectionLineSearchAlgorithm()
{

}


double RGoldenSectionLineSearchAlgorithm::getLowerBound() const
{
    return theLowerBound;
}


void RGoldenSectionLineSearchAlgorithm::setLowerBound(double value)
{
    theLowerBound = value;
}


double RGoldenSectionLineSearchAlgorithm::getUpperBound() const
{
    return theUpperBound;
}


void RGoldenSectionLineSearchAlgorithm::setUpperBound(double value)
{
    theUpperBound = value;
}


double RGoldenSectionLineSearchAlgorithm::getTolerance() const
{
    return theTolerance;
}


void RGoldenSectionLineSearchAlgorithm::setTolerance(double value)
{
    theTolerance = value;
}


int RGoldenSectionLineSearchAlgorithm::getMaxIterations() const
{
    return theMaxIterations;
}


void RGoldenSectionLineSearchAlgorithm::setMaxIterations(int value)
{
    theMaxIterations = value;
}


double RGoldenSectionLineSearchAlgorithm::getDelay() const
{
    return theDelay;
}


void RGoldenSectionLineSearchAlgorithm::setDelay(double value)
{
    theDelay = value;
}


double RGoldenSectionLineSearchAlgorithm::findPoint(RLineSearchCallBackAssistant *theCallBackAssistant)
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


    // Calculate the golden ratio
    double goldenRatio = (1.0 + sqrt(5.0)) / 2.0;


    // Initial declarations
    bool convergence = false;
    int counter = 0;
    double optimum;


    // Check that the upper bound is greater than the lower bound
    if (theUpperBound <= theLowerBound) {
        qCritical() << "The lower bound must be smaller than the upper bound in" << this->objectName();
        return 0;
    }


    // Initialize the points  x1 ---------- x2 ----- x3 ---------- x4
    double x1 = theLowerBound;
    double x4 = theUpperBound;
    double range = x4 - x1;
    double bigPortionOfRange = range / goldenRatio;
    double x2 = x4 - bigPortionOfRange;
    double x3 = x1 + bigPortionOfRange;


    // Evaluate the function at those points
    double Fx1 = theCallBackAssistant->evaluate_F(x1);
    double Fx2 = theCallBackAssistant->evaluate_F(x2);
    double Fx3 = theCallBackAssistant->evaluate_F(x3);
    double Fx4 = theCallBackAssistant->evaluate_F(x4);


    // Add points to the plot
    if (theOutputLevel == RObject::Maximum) {

        customPlot->graph(0)->addData(x1, Fx1);
        customPlot->graph(0)->addData(x2, Fx2);
        customPlot->graph(0)->addData(x3, Fx3);
        customPlot->graph(0)->addData(x4, Fx4);
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


        // If the function value at x3 is greater than at x2 then the solution is in the range x1 ------------------- x2 ---------- x3
        // hence those points are now mapped to the new standard form                          x1 ---------- x2 ----- x3 ---------- x4
        if (Fx3 > Fx2) {

            // New range
            range = x3 - x1;
            bigPortionOfRange = range / goldenRatio;

            // Renaming previously calculated points
            x4 = x3;
            Fx4 = Fx3;

            x3 = x2;
            Fx3 = Fx2;

            // New point in that range
            x2 = x4 - bigPortionOfRange;
            Fx2 = theCallBackAssistant->evaluate_F(x2);

            // Add point to the plot
            if (theOutputLevel == RObject::Maximum) {

                customPlot->graph(0)->addData(x2, Fx2);
                customPlot->rescaleAxes();
                customPlot->replot();
            }

        }


        // Otherwise the solution is in the range                      x2 ---------- x3 ------------------- x4
        // hence those points are now mapped to the new standard form  x1 ---------- x2 ----- x3 ---------- x4
        else {

            // New range
            range = x4 - x2;
            bigPortionOfRange = range / goldenRatio;

            // Renaming previously calculated points
            x1 = x2;
            Fx1 = Fx2;

            x2 = x3;
            Fx2 = Fx3;

            // New point in that range
            x3 = x1 + bigPortionOfRange;
            Fx3 = theCallBackAssistant->evaluate_F(x3);

            // Add point to the plot
            if (theOutputLevel == RObject::Maximum) {

                customPlot->graph(0)->addData(x3, Fx3);
                customPlot->rescaleAxes();
                customPlot->replot();
            }

        }


        // Output
        if (theOutputLevel == RObject::Maximum) {
            qDebug() << "At step" << counter << this->objectName() << "has interior points" << x2 << "and" << x3;
        }


        // Check convergence
        if (fabs(x3 - x2) < theTolerance) {

            optimum = (x2 + x3) / 2.0;
            convergence = true;
        }

    }

    if (theOutputLevel >= RObject::Medium) {

        qDebug() << this->objectName() << "is returning the solution" << optimum;
    }

    return optimum;
}

