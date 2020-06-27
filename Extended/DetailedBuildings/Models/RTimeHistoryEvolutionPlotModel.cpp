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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RTimeHistoryEvolutionPlotModel.h"
#include "RStructuralAnalysis.h"
#include "RStructuralAnalysisModel.h"
#include "RParameter.h"
#include "RLoadBaseClass.h"
#include "RResponse.h"
#include "RModel.h"

#include "UniformExcitation.h"
#include "GroundMotion.h"

#include <functional>

RTimeHistoryEvolutionPlotModel::RTimeHistoryEvolutionPlotModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    isOrchestratingModel = true;
    
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    customPlot = nullptr;
    theStructAnalysisMethod = nullptr;
    theStructAnalysisModel = nullptr;
}


RTimeHistoryEvolutionPlotModel::~RTimeHistoryEvolutionPlotModel()
{

}


QString RTimeHistoryEvolutionPlotModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}


void RTimeHistoryEvolutionPlotModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


QString RTimeHistoryEvolutionPlotModel::getTheLoadPattern()
{
    QString string;
    for (int i = 0; i < theLoadPatterns.count(); i++)
    {
        if (theLoadPatterns[i]) {
            string += theLoadPatterns[i]->objectName();
            string += "; ";
        } else {
            theLoadPatterns.removeAt(i);
            i--;
        }
    }

    return string;
}


QObject* RTimeHistoryEvolutionPlotModel::getSAMethod() const
{
    return qobject_cast<QObject*>(theStructAnalysisMethod);
}


void RTimeHistoryEvolutionPlotModel::setSAMethod(QObject *value)
{
    theStructAnalysisMethod = qobject_cast<RStructuralAnalysis*>(value);

    if(theStructAnalysisMethod == nullptr)
    {
        qCritical()<< this->objectName()<<" needs a structural analysis method";
        return;
    }
}

QObject* RTimeHistoryEvolutionPlotModel::getSAModel() const
{
    return qobject_cast<QObject*>(theStructAnalysisModel);
}


void RTimeHistoryEvolutionPlotModel::setSAModel(QObject *value)
{
    theStructAnalysisModel = qobject_cast<RStructuralAnalysisModel*>(value);

    if(theStructAnalysisModel == nullptr)
    {
        qCritical()<< this->objectName()<<" needs a structural analysis model";
        return;
    }
}


void RTimeHistoryEvolutionPlotModel::setTheLoadPattern(QString value)
{
    QRegExp regExp = QRegExp(QString("[ ,;:\t\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator)));
    QStringList stringList = value.split(regExp, QString::SkipEmptyParts);

    for (int i = 0; i < stringList.count(); i++)
    {
        RLoadPattern *object = theDomain->findChild<RLoadPattern *>(stringList[i]);
        if (object)
        {
            theLoadPatterns.append(object);
        } else {
            qCritical() << "Error in" << objectName() << ": Could not find a load pattern named" << stringList[i] << "in the domain";
        }
    }
}


int RTimeHistoryEvolutionPlotModel::evaluateModel(RGradientType theGradientType)
{
    theResponse->setCurrentValue(0.0);

    // Create plot
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    if(mainWindow == nullptr || theStructAnalysisModel == nullptr || theStructAnalysisMethod == nullptr)
    {
        qCritical()<<"Could not evaluate "<<this->objectName()<<" due to missing input parameters";
        return -1;
    }

    // Get the models that serve input to this model
    QList<RModel *> inputModelList;
    for (int i=0; i<theInputParameterList.size(); i++) {
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameterList.at(i));
        inputModelList << qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    }

    // Check to ensure that each response is from a model
    if(inputModelList.size() != theInputParameterList.size())
    {
        qCritical()<<"Error, the input parameters must be model responses";
        return -1;
    }

    // Create the plot
    if (customPlot == nullptr) {

        customPlot = new QCustomPlot();
        mainWindow->addSubWindow(customPlot, "Plot", true);

        for(int i=0; i<theInputParameterList.size(); ++i)
        {
            customPlot->addGraph();
            customPlot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
            int r = rand() % 255;
            int g = rand() % 255;
            int b = rand() % 255;
            customPlot->graph(i)->setPen(QColor(r, g, b, 255));
        }

        customPlot->xAxis->setLabel("Time");
        customPlot->yAxis->setLabel("Response Values");
    }

    QVector<double> timeStepVec;
    QVector< QVector<double> > responseVec;

    // Create the callback function that will be evaluated within the time-history analysis model
    std::function<void(double&)> callback = [&](double& time)
    {
        // Save the current time step
        timeStepVec.push_back(time);

        // Reset the models
        for (int i=0; i<inputModelList.size(); i++)
        {
            inputModelList.at(i)->resetTime();
        }

        // Run analysis in all models
        for (int i=0; i<inputModelList.size(); i++)
        {
            inputModelList.at(i)->runAnalysis(RGradientType::NONE);
        }

        // Get the new response values
        QVector<double> results(theInputParameterList.size());
        for (int i=0; i<theInputParameterList.size(); i++)
        {
            auto responseValue = theInputParameterList[i]->getCurrentValue();
            results[i] = responseValue;

            // Update the plots
            customPlot->graph(i)->addData(time, responseValue);
        }

        // Rescale the axes and refresh the plot
        customPlot->rescaleAxes();
        customPlot->replot();

        responseVec.push_back(results);

        // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
        theDomain->clearAnalyzedFlags();
    };

    // Set the callback to the structural analysis method
    std::shared_ptr<std::function<void(double&)>> ptrToCallBack = std::make_shared<std::function<void(double&)>>(callback);

    theStructAnalysisMethod->setCallback(ptrToCallBack);

    // Run the structural analysis model
    theStructAnalysisModel->evaluateModel(RGradientType::NONE);

    // Create the header text for output
    QString headerStr = "Time Step";

    for(int j = 0; j<theInputParameterList.size(); ++j)
    {
        headerStr.append(", "+ theInputParameterList[j]->objectName());
    }

    qDebug()<< headerStr;

    for(int i =0; i<timeStepVec.size(); ++i)
    {
        QString resultStr = QString::number(timeStepVec[i]);


        for(int j = 0; j<theInputParameterList.size(); ++j)
        {
            resultStr.append(","+QString::number(responseVec[i][j]));
        }

        qDebug()<<resultStr;
    }


    // Set dummy response
    theResponse->setCurrentValue(0.0);

    return 1;
}





