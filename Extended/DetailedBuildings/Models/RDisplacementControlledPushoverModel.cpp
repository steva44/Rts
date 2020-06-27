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

#include "RDisplacementControlledPushoverModel.h"
#include "RDisplacementControlIntegrator.h"
#include "RInelasticStaticStructuralAnalysis.h"
#include "RResponse.h"
#include "RWallComponent.h"
#include "RLoadPattern.h"
#include "RMesh.h"
#include "RMainWindow.h"

#include <OPS_Globals.h>
#include "ID.h"
#include "DisplacementControl.h"

#include "qcustomplot.h"

RDisplacementControlledPushoverModel::RDisplacementControlledPushoverModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    isOrchestratingModel = true;
    customPlot = nullptr;
    theStructAnalysisMethod = nullptr;
    theIntegrator = nullptr;
    theNumStepsParameter = nullptr;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    theNumStepsDouble = 0.0;
    finalDisplacement = 0.0;
}


RDisplacementControlledPushoverModel::~RDisplacementControlledPushoverModel()
{

}


QString RDisplacementControlledPushoverModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}


void RDisplacementControlledPushoverModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


double RDisplacementControlledPushoverModel::getFinalDisp() const
{
    return finalDisplacement;
}


void RDisplacementControlledPushoverModel::setFinalDisp(const double value)
{
    finalDisplacement = value;
}


QObject* RDisplacementControlledPushoverModel::getIntegrator() const
{
    return theIntegrator;
}


void RDisplacementControlledPushoverModel::setIntegrator(QObject *value)
{
    theIntegrator = qobject_cast<RDisplacementControlIntegrator*>(value);

    if(theIntegrator == nullptr)
    {
        qCritical()<< this->objectName()<<" needs a displacement controlled integrator";
        return;
    }

    return;
}


QObject* RDisplacementControlledPushoverModel::getSAMethod() const
{
    return qobject_cast<QObject*>(theStructAnalysisMethod);
}


void RDisplacementControlledPushoverModel::setSAMethod(QObject *value)
{
    theStructAnalysisMethod = qobject_cast<RInelasticStaticStructuralAnalysis*>(value);

    if(theStructAnalysisMethod == nullptr)
    {
        qCritical()<< this->objectName()<<" needs an inelastic static structural analysis method";
        return;
    }
}


QString RDisplacementControlledPushoverModel::getNumSteps()
{
    return getStringParameter(&theNumStepsDouble, theNumStepsParameter);
}


void RDisplacementControlledPushoverModel::setNumSteps(QString value)
{
    setStringParameter(value, &theNumStepsDouble, &theNumStepsParameter);
}


double RDisplacementControlledPushoverModel::getNumStepsValue()
{
    return getDoubleParameter(&theNumStepsDouble, theNumStepsParameter);
}


int RDisplacementControlledPushoverModel::evaluateModel(RGradientType theGradientType)
{
    theResponse->setCurrentValue(0.0);

    if(theIntegrator == nullptr || theStructAnalysisMethod == nullptr)
    {
        qCritical()<<"Error, this model "<<this->objectName()<<"needs a displacement controlled integrator and an inelastic static structural analysis method";
        return -1;
    }

    // Create plot
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    auto theOpenSeesDomain = theDomain->getTheOpenSeesDomain();

    if(theOpenSeesDomain == nullptr)
    {
        qDebug()<<"Could not get the OpenSees domain in "<<__FUNCTION__;
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

        customPlot->xAxis->setLabel("Drift");
        customPlot->yAxis->setLabel("Response Values");
    }


    // Get the number of load steps and the final load
    auto numSteps = static_cast<int>(this->getNumStepsValue());

    // Test to remove
    this->fixTopOfWall();

    // Vectors to store the results
    QVector<double> driftVec(numSteps);
    QVector< QVector<double> > responseVec(numSteps, QVector<double>(theInputParameterList.size()));

    for(int i = 0; i<numSteps; ++i)
    {
        double targetDispAtStep = finalDisplacement*static_cast<double>(i+1)/static_cast<double>(numSteps);

        // Set the target displacement to the integrator
        this->setTargetDisplacement(targetDispAtStep);

        // Reset the structural analysis method to pick up the new target displacement with the integrator
        theStructAnalysisMethod->reset();

        // Run analysis in all models
        for (int i=0; i<inputModelList.size(); i++)
        {
            inputModelList.at(i)->runAnalysis(RGradientType::NONE);
        }

        // Get the drift at this displacement
        auto drift = this->getMaxDrift();

        // Save the values of the parameters
        for (int j=0; j<theInputParameterList.size(); j++)
        {
            auto parameterValue = theInputParameterList.at(j)->getCurrentValue();

            // qDebug()<<"For "<<theInputParameterList.at(j)->objectName()<<" the value is "<<parameterValue;

            responseVec[i][j] = parameterValue;

            // Output and save the analysis results
            qDebug()<<"For Step"<<i+1<<" the target displacement at this step is "<<targetDispAtStep<<" and the value of "<<theInputParameterList[j]->objectName()<<" is "<<parameterValue;

            // Update the plots
            customPlot->graph(j)->addData(drift, parameterValue);

            // Rescale the axes and refresh the plot
            customPlot->rescaleAxes();
            customPlot->replot();
        }

        // Save the drift value
        driftVec[i] = drift;

        // Reset analysis in all models
        for (int i=0; i<inputModelList.size(); i++)
        {
            inputModelList.at(i)->reset();
        }

        // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
        theDomain->clearAnalyzedFlags();
    }

    // Create the header text for output
    QString headerStr = "Drift";

    for(int j = 0; j<theInputParameterList.size(); ++j)
    {
        headerStr.append(", "+ theInputParameterList[j]->objectName());
    }

    headerStr.append(", Step Number");

    qDebug().noquote()<< headerStr;

    for(int i = 0; i<numSteps; ++i)
    {
        QString resultStr = QString::number(driftVec[i]);

        for(int j = 0; j<theInputParameterList.size(); ++j)
        {
            resultStr.append(","+QString::number(responseVec[i][j]));
        }

        resultStr.append(","+QString::number(i+1));

        qDebug().noquote()<<resultStr;
    }

    // Set response
    theResponse->setCurrentValue(1.0);

    return 1;
}


double RDisplacementControlledPushoverModel::getMaxDrift()
{
    double maxDrift = 0.0;

    auto componentList = theDomain->findChildren<RComponent*>();

    for(auto&& it : componentList)
    {
        if(it->getIsStructural() == false)
            continue;

        auto theMesh = it->getMesh();

        if(theMesh == nullptr)
        {
            qDebug()<<"Could not get the mesh for component "<<it->objectName()<<" in "<<__FUNCTION__;
            continue;
        }

        auto drift = theMesh->getMaxDrift();
        if(drift > maxDrift)
            maxDrift = drift;
    }

    return maxDrift;
}


void RDisplacementControlledPushoverModel::fixTopOfWall()
{
    auto componentList = theDomain->findChildren<RWallComponent*>();

    auto theOpenSeesDomain = theDomain->getTheOpenSeesDomain();

    double tol = 1.0e-3;

    ID fixityConditions(6);

    fixityConditions[0]=0; // Only allow movement in the x-direction
    fixityConditions[1]=1;
    fixityConditions[2]=1;
    fixityConditions[3]=1;
    fixityConditions[4]=1;
    fixityConditions[5]=1;

    auto height = componentList.first()->getHeightValue();

    theOpenSeesDomain->addSP_Constraint(2,height,fixityConditions,tol);

}


void RDisplacementControlledPushoverModel::setTargetDisplacement(double target)
{
    theIntegrator->setDispIncr(target);

    theIntegrator->setMinDelta(target);

    theIntegrator->setMaxDelta(target);

    theIntegrator->reset();

    return;
}



