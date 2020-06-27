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

#include "RBuildingPushoverModel.h"
#include "RParameter.h"
#include "RLoadBaseClass.h"
#include "RResponse.h"
#include "RModel.h"
#include "RComponent.h"
#include "RInfoRichResponse.h"
#include "RPlateComponent.h"
#include "RWallComponent.h"
#include "RNodalLoad.h"

#include <OPS_Globals.h>
#include "ID.h"
#include "TimeSeries.h"
#include "LoadPattern.h"
#include "NodalLoad.h"

RBuildingPushoverModel::RBuildingPushoverModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    isOrchestratingModel = true;
    
    theLoadPattern = nullptr;
    customPlot = nullptr;
    theLoadParameter = nullptr;
    theNumLoadStepsParameter = nullptr;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    theNumLoadStepsDouble = 0.0;
}


RBuildingPushoverModel::~RBuildingPushoverModel()
{

}


QString RBuildingPushoverModel::getCostParameterList()
{
    return parameterListToString(theCostParameterList);
}


void RBuildingPushoverModel::setCostParameterList(QString value)
{
    theCostParameterList = stringToParameterList(value);
}


QString RBuildingPushoverModel::getTimeParameterList()
{
    return parameterListToString(theTimeParameterList);
}


void RBuildingPushoverModel::setTimeParameterList(QString value)
{
    theTimeParameterList = stringToParameterList(value);
}


QString RBuildingPushoverModel::getEmissionsParameterList()
{
    return parameterListToString(theEmissionsParameterList);
}


void RBuildingPushoverModel::setEmissionsParameterList(QString value)
{
    theEmissionsParameterList = stringToParameterList(value);
}


QObject* RBuildingPushoverModel::getLoadParameter() const
{
    return theLoadParameter;
}


void RBuildingPushoverModel::setLoadParameter(QObject *value)
{
    theLoadParameter = qobject_cast<RParameter*>(value);

    if(theLoadParameter == nullptr)
    {
        qCritical()<< this->objectName()<<" needs a load parameter";
        return;
    }

    this->theAllParametersList.push_back(theLoadParameter);

    return;
}


QString RBuildingPushoverModel::getNumLoadSteps()
{
    return getStringParameter(&theNumLoadStepsDouble, theNumLoadStepsParameter);
}


void RBuildingPushoverModel::setNumLoadSteps(QString value)
{
    setStringParameter(value, &theNumLoadStepsDouble, &theNumLoadStepsParameter);
}


double RBuildingPushoverModel::getNumLoadStepsValue()
{
    return getDoubleParameter(&theNumLoadStepsDouble, theNumLoadStepsParameter);
}


QString RBuildingPushoverModel::getTheLoadPattern()
{
    QString string;

    if (theLoadPattern)
    {
        string += theLoadPattern->objectName();
        string += "; ";
    }

    return string;
}


void RBuildingPushoverModel::setTheLoadPattern(QString value)
{
    QRegExp regExp = QRegExp(QString("[ ,;:\t\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator)));
    QStringList stringList = value.split(regExp, QString::SkipEmptyParts);

    for (int i = 0; i < stringList.count(); i++)
    {
        RLoadPattern *object = theDomain->findChild<RLoadPattern *>(stringList[i]);
        if (object)
        {
            theLoadPattern = object;
        } else {
            qCritical() << "Error in" << objectName() << ": Could not find a load pattern named" << stringList[i] << "in the domain";
        }
    }
}


int RBuildingPushoverModel::evaluateModel(RGradientType theGradientType)
{
    theResponse->setCurrentValue(0.0);

    // Create plot
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    if (theOutputLevel >= RObject::Medium) {

        if (customPlot == nullptr) {

            customPlot = new QCustomPlot();
            mainWindow->addSubWindow(customPlot, "Drift vs. Cost", true);
            customPlot->addGraph();
            customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
            customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            customPlot->xAxis->setLabel("Drift");
            customPlot->yAxis->setLabel("Cost");
        }
    }

    // Get the models that serve input to this model
    QList<RModel *> inputModelList;
    for (int i=0; i<theCostParameterList.size(); i++) {
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theCostParameterList.at(i));
        inputModelList << qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    }

    // Re-generate occurrence times
    QList<RModel *> theModels = theDomain->findChildren<RModel *>();
    for (int i=0; i<theModels.size(); i++) {
        theModels.at(i)->resetTime();
    }


    // Get the number of load steps and the final load
    auto numLoadSteps = static_cast<int>(this->getNumLoadStepsValue());

    double finalLoad = theLoadParameter->getCurrentValue();

    double totalCost = 0.0;
    double totalTime = 0.0;

    QVector<double> driftVec(numLoadSteps);
    QVector<double> costVec(numLoadSteps);
    QVector<double> timeVec(numLoadSteps);
    QVector<double> emissionsVec(numLoadSteps);

    // Get the models that serve input to this model
    QList<RInfoRichResponse *> emissionsResponselList;
    for (auto it : theEmissionsParameterList)
    {
        RInfoRichResponse *theInputCastAsResponse = qobject_cast<RInfoRichResponse *>(it);

        if(theInputCastAsResponse)
            emissionsResponselList.push_back(theInputCastAsResponse);
    }

    RInfoRichResponse* emissionsResponse = nullptr;

    if(!emissionsResponselList.empty())
        emissionsResponse = emissionsResponselList.front();


    for(int i = 0; i<numLoadSteps; ++i)
    {
        if(i==1)
            this->applyLoadToFloors();

        totalCost = 0.0;
        totalTime = 0.0;

        auto loadRatio = static_cast<double>(i+1)/static_cast<double>(numLoadSteps);

        double loadAtThisStep = finalLoad*loadRatio;

        theLoadParameter->setCurrentValue(loadAtThisStep);

        // Run analysis in all models
        for (int i=0; i<inputModelList.size(); i++) {
            inputModelList.at(i)->runAnalysis(RGradientType::NONE);
        }

        auto drift = this->getMaxBuildingDrift();


        // Get the cost
        for (int i=0; i<theCostParameterList.size(); i++) {

            auto inputCost = theCostParameterList.at(i)->getCurrentValue();

            // qDebug()<<"For "<<theInputParameterList.at(i)->objectName()<<" the cost is "<<inputCost;

            totalCost += inputCost;
        }

        // Get the time
        for (int i=0; i<theTimeParameterList.size(); i++) {

            auto inputTime = theTimeParameterList.at(i)->getCurrentValue();

            // qDebug()<<"For "<<theInputParameterList.at(i)->objectName()<<" the cost is "<<inputCost;

            totalTime += inputTime;
        }


        qDebug()<<"For Step"<<i+1<<" the load at this step is "<<loadAtThisStep<<" the drift at this step is "<<drift<<" and the repair cost is "<<totalCost;

        driftVec[i] = drift;
        costVec[i] = totalCost;
        timeVec[i] = totalTime;

        if(emissionsResponse)
            emissionsVec[i] = emissionsResponse->getCurrentValue("CarbonDioxideEmissions");

        // Update the plot after the new cost value is obtained
        if (theOutputLevel >= RObject::Medium)
        {
            customPlot->graph(0)->addData(drift, totalCost);
            customPlot->yAxis->rescale();
            customPlot->xAxis->rescale();
            customPlot->replot();
        }

        // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
        theDomain->clearAnalyzedFlags();
    }

    // *****************************************************************
    // *****************************************************************

    qDebug()<<"Drift, Time, Cost, CO2 Emissions, Load Step";
    for(int i = 0; i<numLoadSteps; ++i)
    {
        qDebug()<<driftVec[i]<<","<<timeVec[i]<<","<<costVec[i]<<","<<emissionsVec[i]<<","<<i+1;
    }


    // Set response
    theResponse->setCurrentValue(totalCost);


    return 1;
}


// Function to get the maximum building drift
double RBuildingPushoverModel::getMaxBuildingDrift()
{
    double maxDrift = 0.0;

    auto plateCompList = theDomain->findChildren<RPlateComponent*>();

    std::vector<RNode*> theNodesVec;

    for(auto&& plateComponent : plateCompList)
    {
        auto elementList = plateComponent->getElementList();

        for(auto&& element : elementList)
        {
            auto nodeVec = element->getNodes();

            theNodesVec.insert(theNodesVec.end(), nodeVec.begin(), nodeVec.end());
        }
    }

    std::vector<RNode*> finalNodesVec;
    auto compLambda = [&](RNode* node) {

        for(auto&& it: finalNodesVec)
            if(it->getObjectTag() == node->getObjectTag())
                return true;

        return false;
    };

    for(auto&& it : theNodesVec)
    {
        if(compLambda(it) == false)
            finalNodesVec.push_back(it);
    }

    for(auto&& it : finalNodesVec)
    {
        auto xDisp = it->getXDisplacement();
        auto yDisp = it->getYDisplacement();

        auto dx = xDisp;
        auto dy = yDisp;

        auto heightOfNode = it->getZCoordinateValue();

        auto hDisp = sqrt(dx*dx + dy*dy);

        auto drift = hDisp/heightOfNode;

        if(drift>maxDrift)
            maxDrift = drift;
    }

    return maxDrift;
}


// Function to apply the loads to the floors
void RBuildingPushoverModel::applyLoadToFloors(void)
{
    auto theOpenSeesDomain = theDomain->getTheOpenSeesDomain();

    if(theOpenSeesDomain == nullptr)
    {
        qDebug()<<"Could not get the OpenSees domain in "<<__FUNCTION__;
        return;
    }

    // Collect the floor nodes, the load is applied evenly along each floor
    auto plateCompList = theDomain->findChildren<RPlateComponent*>();

    std::vector<RNode*> theNodesVec;

    for(auto&& plateComponent : plateCompList)
    {
        auto elementList = plateComponent->getElementList();

        for(auto&& element : elementList)
        {
            auto nodeVec = element->getNodes();

            theNodesVec.insert(theNodesVec.end(), nodeVec.begin(), nodeVec.end());
        }
    }

    std::vector<RNode*> finalNodesVec;
    auto compLambda = [&](RNode* node) {

        for(auto&& it: finalNodesVec)
            if(it->getObjectTag() == node->getObjectTag())
                return true;

        return false;
    };

    for(auto&& it : theNodesVec)
    {
        if(compLambda(it) == false)
            finalNodesVec.push_back(it);
    }

    // qDebug()<<"Num floor nodes: "<<numFloorNodes;

    // Need to manually set the load direction here
    qDebug()<<"E-W";
    // qDebug()<<"N-S";
    // qDebug()<<"E-W and N-S";

    // Vector in that gives the direction at which the load will be applied
    std::vector<double> directionVector = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    // Create and set the load to the floor nodes
    for(auto&& theNode : finalNodesVec)
    {
        auto nodeId = theNode->getObjectTag();

        theDomain->createObject("LoadOfNodeNum"+QString::number(nodeId),"RNodalLoad");

        RNodalLoad* theRLoad = qobject_cast<RNodalLoad*>(theDomain->getLastAddedObject());
        
        if(theRLoad == nullptr)
        {
            qCritical()<<"Error creating Nodal Load in the Domain in "<<__FUNCTION__;
            continue;
        }

        theRLoad->setLoadDirectionVector(directionVector);
        theRLoad->setTheLoadPattern(theLoadPattern);
        theRLoad->setParameter(theLoadParameter);
        theRLoad->setNode(theNode);
    }

    return;
}



