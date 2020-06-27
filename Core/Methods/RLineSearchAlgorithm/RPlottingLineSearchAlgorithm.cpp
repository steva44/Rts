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

#include "RPlottingLineSearchAlgorithm.h"
#include "RLineSearchCallBackAssistant.h"
#include "RDomain.h"
#include "RMainWindow.h"

#include <math.h>


RPlottingLineSearchAlgorithm::RPlottingLineSearchAlgorithm(QObject *parent, QString name)
    : RLineSearchAlgorithm(parent, name)
{
    theFrom = 1.0;
    theTo = 1.0;
    customPlot = nullptr;
}


RPlottingLineSearchAlgorithm::~RPlottingLineSearchAlgorithm()
{

}


double RPlottingLineSearchAlgorithm::getFrom() const
{
    return theFrom;
}


void RPlottingLineSearchAlgorithm::setFrom(double value)
{
    theFrom = value;
}


double RPlottingLineSearchAlgorithm::getTo() const
{
    return theTo;
}


void RPlottingLineSearchAlgorithm::setTo(double value)
{
    theTo = value;
}


double RPlottingLineSearchAlgorithm::findPoint(RLineSearchCallBackAssistant *theCallBackAssistant)
{
    // Create the plot, unless it was already done
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    QVector<double> tickVector;
    tickVector << 0;
    QVector<QString> labelVector;
    labelVector << QString("%1").arg(0);

    if (customPlot == nullptr) {

        customPlot = new QCustomPlot();
        mainWindow->addSubWindow(customPlot, "Plot Along Search Direction", true);
        customPlot->addGraph();
        customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
        customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
        customPlot->xAxis->setLabel("Step Size");
        customPlot->yAxis->setLabel("Merit Function");
        customPlot->xAxis->setAutoTicks(true);
        customPlot->xAxis->setAutoTickLabels(true);
        customPlot->xAxis->setRange(theFrom-0.1*(theTo-theFrom), theTo+0.1*(theTo-theFrom));
    }
    else {

        customPlot->graph(0)->clearData();
    }


    // Walk along search direction and evaluate merit function
    int numPoints = 20;
    double F, x;
    double minF = 0.0;
    double maxF = 1.0;
    for (int i=0; i<=numPoints; i++) {

        x = theFrom + i * (theTo - theFrom) / (double)numPoints;

        F = theCallBackAssistant->evaluate_F(x);

        if (F > maxF) {
            maxF = F;
        }
        else if (F < minF) {
            minF = F;
        }

        customPlot->graph(0)->addData(x, F);

        customPlot->yAxis->setRange(minF-0.1*(maxF-minF), maxF+0.1*(maxF-minF));

        customPlot->replot();

    }


    // Return value 1.0
    return 1.0;
}

