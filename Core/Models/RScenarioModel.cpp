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

#include "RScenarioModel.h"
#include "RParameter.h"
#include "RTime.h"
#include "RMainWindow.h"
#include "RModelManager.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "RStatistics.h"

#include <QMap>
#include <QCoreApplication>
#include <QMessageBox>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDateTime>
#include <math.h>
#include "qcustomplot.h"

RScenarioModel::RScenarioModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    isOrchestratingModel = true;
    theStartTime = nullptr;
    theEndTime = nullptr;
    theMaxTimeGapParameter = nullptr;
    customPlot = nullptr;

    theMaxTimeGapDouble = 365.0;
    overallMaxCost = 0.0;
    plotCounter = 0;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RScenarioModel::~RScenarioModel()
{

}


QString RScenarioModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}


void RScenarioModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


QObject * RScenarioModel::getStartTime() const
{
    return theStartTime;
}


void RScenarioModel::setStartTime(QObject *value)
{
    theStartTime = qobject_cast<RParameter *>(value);
}


QObject * RScenarioModel::getEndTime() const
{
    return theEndTime;
}


void RScenarioModel::setEndTime(QObject *value)
{
    theEndTime = qobject_cast<RParameter *>(value);
}


QString RScenarioModel::getRealInterestRate()
{
    return getStringParameter(&theRealInterestRateDouble, theRealInterestRateParameter);
}


void RScenarioModel::setRealInterestRate(QString value)
{
    setStringParameter(value, &theRealInterestRateDouble, &theRealInterestRateParameter);
}


double RScenarioModel::getRealInterestRateValue()
{
    return getDoubleParameter(&theRealInterestRateDouble, theRealInterestRateParameter);
}


QString RScenarioModel::getMaxTimeGap()
{
    return getStringParameter(&theMaxTimeGapDouble, theMaxTimeGapParameter);
}


void RScenarioModel::setMaxTimeGap(QString value)
{
    setStringParameter(value, &theMaxTimeGapDouble, &theMaxTimeGapParameter);
}


double RScenarioModel::getMaxTimeGapValue()
{
    return getDoubleParameter(&theMaxTimeGapDouble, theMaxTimeGapParameter);
}


QStringList RScenarioModel::getActionList()
{
    // Remember that the "run" and "help" methods are implemented in base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Plot Model Flowchart"<< "Separator"<< "&Help";
    return actionList;
}


void RScenarioModel::plotModelFlowchart()
{
    if (theDomain->checkAllProperties() >= 0) {
        QList<RModel *> listOfModels = theDomain->findChildren<RModel *>();
        RModelManager theModelManager(theDomain);
        theModelManager.createModelMap(theInputParameterList, listOfModels);
        theModelManager.plotFlowchart();
    }
}


int RScenarioModel::evaluateModel(RGradientType theGradientType)
{
    if(theStartTime == nullptr || theEndTime == nullptr)
    {
        qCritical()<<"Error: the scenario model "<<this->objectName()<<" requires a start and/or end time";
        return -1;
    }

    // Get start and end time
    double startTime = theStartTime->getCurrentValue();
    double endTime = theEndTime->getCurrentValue();


    // Create plot
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    QVector<double> tickVector;
    tickVector << 0;
    QVector<QString> labelVector;
    labelVector << QString("%1").arg(0);

    if (theOutputLevel >= RObject::Medium) {

        if (customPlot == nullptr) {

            customPlot = new QCustomPlot();
            mainWindow->addSubWindow(customPlot, "Scenarios", true);
            customPlot->addGraph();
            customPlot->graph(plotCounter)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
            customPlot->graph(plotCounter)->setPen(QColor(0, 0, 255, 255));
            customPlot->xAxis->setLabel("Time");
            customPlot->yAxis->setLabel("Cost");
            customPlot->xAxis->setRange(startTime*0.9, endTime*1.1);
            customPlot->xAxis->setAutoTicks(false);
            customPlot->xAxis->setAutoTickLabels(false);

        }
        else {
            int r = rand() % 255;
            int g = rand() % 255;
            int b = rand() % 255;

            customPlot->addGraph();
            plotCounter++;
            customPlot->graph(plotCounter)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
            customPlot->graph(plotCounter)->setPen(QColor(r, g, b, 255));
        }
    }


    // Get the models that serve input to this model
    QList<RModel *> inputModelList;
    for (int i=0; i<theInputParameterList.size(); i++) {
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameterList.at(i));
        inputModelList << qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    }

    // Re-generate occurrence times (we are called repeatedly by sampling, and we want a new scenario every time)
    QList<RModel *> theModels = theDomain->findChildren<RModel *>();
    for (int i=0; i<theModels.size(); i++) {
        theModels.at(i)->resetTime();
    }

    // Get all trigger times
    RModelManager theModelManager(theDomain);
    QList<QPair<double, QString> > *triggerTimes = theModelManager.getTriggerTimes(startTime, endTime, getMaxTimeGapValue());


    /*
    // Make sure there are no time-gaps bigger than "maximum time between evaluations"
    double timeGap, previousTriggerTime, newTriggerTime;
    bool newTimesWereInserted = false;

    for (int i=0; i<triggerTimes->count()-1; i++) {
        timeGap = triggerTimes->at(i+1).first - triggerTimes->at(i).first;
        previousTriggerTime = triggerTimes->at(i).first;

        while (timeGap > getMaxTimeGapValue()) {
            newTimesWereInserted = true;
            newTriggerTime = previousTriggerTime + getMaxTimeGapValue();
            triggerTimes->append(qMakePair(newTriggerTime, QString("Maximum Time Gap Evaluation")));
            timeGap = triggerTimes->at(i+1).first - newTriggerTime;
            previousTriggerTime = newTriggerTime;
        }
    }


    // Sort the trigger times
    if (newTimesWereInserted) {
        qSort(triggerTimes->begin(), triggerTimes->end());
    }
    */


    // Create a dummy time object to assist with output
    RTime dummyTime(theDomain, "dummyTime");

    // Give output if requested
    if (theOutputLevel == RObject::Maximum) {
        qDebug() << this->objectName() << "has these trigger times:";
        for (int i = 0; i < triggerTimes->size(); i++) {
            dummyTime.setCurrentValue(triggerTimes->at(i).first);
            qDebug() << dummyTime.getTime() << "prompted by" << triggerTimes->at(i).second;
        }
    }


    // Loop over trigger times, which are already sorted chronologically
    double totalCost = 0.0;
    double triggerTime;

    for (int i=0; i<triggerTimes->size(); i++) {

        // Keep the GUI responsive
        QCoreApplication::processEvents();


        // Get trigger time
        triggerTime = triggerTimes->at(i).first;


        // Give output if requested
        if (theOutputLevel == RObject::Maximum) {
            dummyTime.setCurrentValue(triggerTimes->at(i).first);
            qDebug() << this->objectName() << "starting analysis at" << triggerTimes->at(i).first << "due to" << triggerTimes->at(i).second;
        }


        // Set the current time in all model to the trigger time
        theCurrentTime->setCurrentValue(triggerTime);


        // Run analysis in all models
        for (int i=0; i<inputModelList.size(); i++) {
            inputModelList.at(i)->runAnalysis(RGradientType::NONE);
        }


        // Draw a horizontal line from the previous time the cost was computed, and then below a vertical line up to the new cost
        // (Notice the awkward 0.9999 factor needed for qCustomPlot to understand this)
        if (theOutputLevel >= RObject::Medium) {
            customPlot->graph(plotCounter)->addData(triggerTime*0.9999, totalCost);
        }

        // Accumulate cost, applying an annual discounting factor that assumes the cost is incurred exactly at this trigger time
        for (int i=0; i<theInputParameterList.size(); i++)
        {
            auto inputParameter = theInputParameterList.at(i);

            auto inputCost = inputParameter->getCurrentValue();

            if(inputParameter->getDiscount() == true)
            {
                // qDebug()<<"For "<<inputParameter->objectName()<<" the DISCOUNTED cost is "<<inputCost<<" and the total cost is: "<<totalCost;

                totalCost += inputCost * exp(-getRealInterestRateValue()*(triggerTime-startTime)/365.0);
            }
            else
            {
                // qDebug()<<"For "<<inputParameter->objectName()<<" the UNDISCOUNTED cost is "<<inputCost<<" and the total cost is: "<<totalCost;
                totalCost += inputCost;
            }
        }


        // Give output if requested
        if (theOutputLevel >= RObject::Medium) {
            dummyTime.setCurrentValue(triggerTimes->at(i).first);
            qDebug() << "Analysis at " << triggerTimes->at(i).second << dummyTime.getTime() << "Cumulative cost=" << totalCost << "Discount factor=" << exp(-getRealInterestRateValue()*(triggerTime-startTime)/365.0);
        }


        // Update the plot AFTER the new cost value is obtained
        if (theOutputLevel >= RObject::Medium) {

            if (totalCost > overallMaxCost) {
                overallMaxCost = totalCost;
            }
            customPlot->graph(plotCounter)->addData(triggerTime, totalCost);
            customPlot->yAxis->setRange(0, overallMaxCost*1.1);
            customPlot->replot();
        }


        // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
        theDomain->clearAnalyzedFlags();

    }


    // Set response
    theResponse->setCurrentValue(totalCost);

    theDomain->getStatsMethod()->update(objectName() + "Response",totalCost);

    return 1;
}


