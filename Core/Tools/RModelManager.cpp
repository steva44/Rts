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
*   - Rodrigo Costa                                                  *
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RModelManager.h"
#include "RDomain.h"
#include "RModel.h"
#include "RResponse.h"
#ifdef NETWORKS
#include "RLocation.h"
#endif
#ifdef GIS
#include "RGISWidget.h"
#endif

RModelManager::RModelManager(RDomain *domain)
    : RMethod(domain, nullptr)
{
    customPlotForLocations = nullptr;
    theEventOccurrence = false;
}


RModelManager::~RModelManager()
{

}


int RModelManager::createModelMap(QList<QPointer<RParameter> > parameterList, QList<RModel *> functionModelList)
{
    // Find the first level of models after the function
    QList<RModel *> level0ModelList;
    for (int i = 0; i < parameterList.count(); i++) {


        // Check if the parameter that appears in the function is a response
        RResponse *response = qobject_cast<RResponse *>(parameterList[i]);
        if (response) {


            // Get the model that the response is emanating from
            RModel *model = qobject_cast<RModel *>(response->getModel());


            // Insert the model in the list unless it is there already
            if (!level0ModelList.contains(model)) {

                level0ModelList << model;

            }
        }
    }


    // Declare more comprehensive model lists
    QMap<RModel *, RModel *> modelMap;
    QList<QList<RModel *> > modelListList;


    // Start with inserting the most downstream models, those linked directly to the function
    modelListList << level0ModelList;


    // Establish the modelListList and modelMap
    for (int i = 0; i < modelListList.count(); i++) {

        QList<RModel *> leveliModelList = modelListList[i];
        QList<RModel *> nextLevelModelList;

        for (int j = 0; j < leveliModelList.count(); j++) {


            RModel *model = leveliModelList[j];

            QList<RResponse *> responseList = model->getResponseList();


#ifdef NETWORKS
            /*
            // If nothing has happened, the resilience options are not included
            if (!theEventOccurrence){
                RNetworkModel *theNetworkModel = qobject_cast<RNetworkModel *>(model);
                if (theNetworkModel){
                    QList<QPointer <RParameter>>  resilienceOptionList = stringToParameterList(theNetworkModel->getResilienceOptionList());
                    for (int k = 0; k < responseList.count(); k++) {

                        int kaux = responseList.count() - (k+1);
                        if (resilienceOptionList.contains(responseList.at(kaux))){
                            responseList.removeAt(kaux);
                        }
                    }
                }
            }
            */
#endif

            for (int k = 0; k < responseList.count(); k++) {

                RModel *responseModel = qobject_cast<RModel *>(responseList[k]->getModel());

                if (!nextLevelModelList.contains(responseModel)) {

                    nextLevelModelList << responseModel;
                }
                modelMap.insertMulti(responseModel, model);
            }
        }
        //

        if (!nextLevelModelList.isEmpty()) {
            modelListList << nextLevelModelList;
        }
    }


    // Reverse the modelListList
    int count = modelListList.count();
    for (int i = 0; i < count / 2.0; i++) {
        modelListList.swap(i, count - i - 1);
    }


    // Clear the duplicates from modelListList
    for (int i = 0; i < modelListList.count(); i++) {

        QList<RModel *> leveliModelList = modelListList[i];

        for (int j = 0; j < leveliModelList.count(); j++) {

            RModel *model = leveliModelList[j];

            if (functionModelList.contains(model)) {

                functionModelList.removeOne(model);

            }

            else {

                leveliModelList.removeAt(j);

                j--;
            }
        }

        modelListList[i] = leveliModelList;

    }


    // Clear the duplicates from modelMap and store the final map in finalModelMap
    QMap<RModel *, QList<RModel *> > finalModelMap;
    QMapIterator<RModel *, RModel *> iterator(modelMap);
    while (iterator.hasNext()) {
        iterator.next();

        RModel *model = iterator.key();
        QList<RModel *> modelList = modelMap.values(model);
        QSet<RModel *> modelSet = modelList.toSet();
        modelList = modelSet.toList();
        finalModelMap[model] = modelList;
    }


    // Process the position of models in modelListList
    for (int i = 0; i < modelListList.count() - 1; i++) {
        QList<RModel *> leveliModelList = modelListList[i];
        QList<RModel *> nextLevelModelList = modelListList[i + 1];

        for (int j = 0; j < leveliModelList.count(); j++) {
            RModel *model = leveliModelList[j];
            QList<RModel *> dependentModelList = finalModelMap[model];

            for (int k = 0; k < dependentModelList.count(); k++) {
                RModel *dependentModel = dependentModelList[k];
                int lastIndex = nextLevelModelList.count() - 1;
                if (nextLevelModelList.contains(dependentModel)) {
                    nextLevelModelList.move(nextLevelModelList.indexOf(dependentModel), lastIndex);
                }
            }
        }
        modelListList[i + 1] = nextLevelModelList;
    }



    // Convert the modelListList to objectListList and adding the function at the end
    // **terje   QList<QObjectList> objectListList;
    for (int i = 0; i < modelListList.count(); i++) {
        QList<RModel *> leveliModelList = modelListList[i];
        QObjectList leveliObjectList;

        for (int j = 0; j < leveliModelList.count(); j++) {

            leveliObjectList << leveliModelList[j];

        }

        objectListList << leveliObjectList;

    }


    // Convert the finalModelMap to objectMap and adding the function at the end
    // **terje   QMap<QPointer<QObject>, QObjectList> objectMap;
    QMapIterator<RModel *, QList<RModel *> > objectIterator(finalModelMap);
    while (objectIterator.hasNext()) {
        objectIterator.next();

        RModel *model = objectIterator.key();
        QList<RModel *> modelList = finalModelMap[model];

        QObjectList objectList;
        for (int i = 0; i < modelList.count(); i++) {

            objectList << modelList[i];

        }
        objectMap[model] = objectList;
    }

    return 0;
}


int RModelManager::plotFlowchart()
{
    if (objectListList.size() == 0) {
        qCritical() << "Cannot plot flowcharts until the method createModelMap() is called. (Or perhaps there are no models?)";
        return 0;
    }

    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    theMainWindow->raiseDock("Output");

    QPointer<RFlowchartWidget> widget = new RFlowchartWidget(theMainWindow);
    theMainWindow->addSubWindow(widget, QString("Network Flowchart"), false);
    widget->setObjectList(objectListList, objectMap);

    return 0;
}


#ifdef NETWORKS
int RModelManager::createLocationDiagram()
{
    if (theDomain->checkAllProperties() >= 0) {

        RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
        if (customPlotForLocations == 0) {

            customPlotForLocations = new QCustomPlot();
            mainWindow->addSubWindow(customPlotForLocations, "Network Map", true);

            customPlotForLocations->addGraph();

            customPlotForLocations->xAxis->setLabel("Longitude");
            customPlotForLocations->yAxis->setLabel("Latitude");

            customPlotForLocations->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 10));
            customPlotForLocations->graph()->setLineStyle(QCPGraph::lsNone);

            customPlotForLocations->xAxis->setRange(-124.5, -122.5);
            customPlotForLocations->yAxis->setRange(48.0, 50.0);


            QList<RLocation *> theLocations = theDomain->findChildren<RLocation *>();
            for (int i=0; i<theLocations.count(); i++) {

                customPlotForLocations->graph(0)->addData(theLocations[i]->getLongitude(), theLocations[i]->getLatitude());

                QCPItemText *wavePacketText = new QCPItemText(customPlotForLocations);
                customPlotForLocations->addItem(wavePacketText);
                wavePacketText->position->setCoords(theLocations[i]->getLongitude(), theLocations[i]->getLatitude()+0.02);
                wavePacketText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
                wavePacketText->setText(theLocations[i]->objectName());
            }

            customPlotForLocations->replot();
        }
    }
    return 0;
}


QVector<QVector<RModel *> > RModelManager::retrievePathMatrix(){
    return pathMatrix;
}


int RModelManager::establishPaths()
{

    // This method establishes the path matrix for the un-interrupted network


    // Collect model-to-model links in a matrix, based on object map
    QList<QPointer<QObject> > fromModels = objectMap.keys();
    int linkCount = 0;
    for (int i = 0; i < fromModels.count(); i++) {

        QObjectList toModels = objectMap[fromModels[i]];

        for (int j = 0; j < toModels.count(); j++) {

            linkCount++;
        }
    }

    QVector<QVector<RModel *> > linkMatrix;
    QVector<RModel *> linkVector;
    RModel *modelA;
    RModel *modelB;
    for (int i = 0; i < fromModels.count(); i++) {

        QObjectList toModels = objectMap[fromModels[i]];

        for (int j = 0; j < toModels.count(); j++) {

            modelA = qobject_cast<RModel *>(fromModels.at(i));
            modelB = qobject_cast<RModel *>(toModels.at(j));

            if (modelA!=0 && modelB!=0) {

                linkVector.clear();

                linkVector << modelA << modelB;

                linkMatrix << linkVector;

            }
        }
    }


    // Find paths, starting with a copy of the link matrix
    QVector<RModel *> pathVector;
    for (int i=0; i<linkMatrix.count(); i++) {
        pathVector.clear();
        pathVector << linkMatrix[i][0] << linkMatrix[i][1];
        pathMatrix.append(pathVector);
    }


    // Keep adding models front and back, of all vectors
    bool stillWorkToDo = true;
    bool action;
    QVector<RModel *> modelsToAppend;
    QVector<RModel *> modelsToPrepend;
    while (stillWorkToDo) {

        action = false;

        for (int i=0; i<pathMatrix.size(); i++) {

            modelsToAppend.clear();
            modelsToPrepend.clear();

            // Search for match both at the beginning and the end of each path, i.e., each vector
            for (int j=0; j<linkMatrix.count(); j++) {

                if (pathMatrix.at(i).endsWith(linkMatrix[j][0])) {

                    // Found match at the end
                    modelsToAppend << linkMatrix[j][1];
                    action = true;

                }

                if (pathMatrix.at(i).startsWith(linkMatrix[j][1])) {

                    // Found match at the beginning
                    modelsToPrepend << linkMatrix[j][0];
                    action = true;

                }
            }

            // Now append models
            if (modelsToAppend.size() == 0) {

            }

            else if (modelsToAppend.size() == 1) {

                pathMatrix[i].append(modelsToAppend.at(0));

            }

            else {

                for (int m=1; m<modelsToAppend.size(); m++) {

                    pathMatrix.append(pathMatrix[i]);
                    pathMatrix[pathMatrix.size()-1].append(modelsToAppend.at(m));
                }

                pathMatrix[i].append(modelsToAppend.at(0));
            }


            // Now prepend models
            if (modelsToPrepend.size() == 0) {

            }
            else if (modelsToPrepend.size() == 1) {

                pathMatrix[i].prepend(modelsToPrepend.at(0));
            }
            else {

                for (int m=1; m<modelsToPrepend.size(); m++) {

                    pathMatrix.append(pathMatrix[i]);
                    pathMatrix[pathMatrix.size()-1].prepend(modelsToPrepend.at(m));
                }

                pathMatrix[i].prepend(modelsToPrepend.at(0));

            }
        }

        if (!action) {
            stillWorkToDo = false;
        }
    }


    // Check for duplicates, and paths contained in others
    bool duplicate;
    QVector<int> pathsToBeRemoved;
    int minNumColumnsInThisRow;
    for (int i=0; i<pathMatrix.count(); i++) {

        for (int ii=i+1; ii<pathMatrix.count(); ii++) {

            duplicate = true;

            if (pathMatrix[i].size() < pathMatrix[ii].size()) {
                minNumColumnsInThisRow = pathMatrix[i].size();
            }
            else {
                minNumColumnsInThisRow = pathMatrix[ii].size();
            }

            for (int j=0; j<minNumColumnsInThisRow; j++) {

                if (pathMatrix[i][j] != pathMatrix[ii][j]) {

                    duplicate = false;
                    break;
                }
            }

            if (duplicate) {

                // Remove the duplicate from the path matrix
                if (!pathsToBeRemoved.contains(ii)) {
                    pathsToBeRemoved << ii;
                }
            }
        }
    }

    qSort(pathsToBeRemoved);

    for (int i=0; i<pathsToBeRemoved.size(); i++) {

        pathMatrix.remove(pathsToBeRemoved.at(pathsToBeRemoved.size()-i-1));
    }

    return 0;
}


void RModelManager::setPathMatrix(QVector<QVector<RModel *> > originalPathMatrix)
{
    pathMatrix.clear();
    for (int i = 0; i < originalPathMatrix.size(); i++){
        pathMatrix << originalPathMatrix.at(i);
    }
}


int RModelManager::setPaths()
{
    if (pathMatrix.size() == 0) {
        qCritical() << "Cannot print paths until the pathmatrix is established. (Or perhaps there are no models?)";
        return 0;
    }

    // Set the path matrix in all network models, which will direct the traffic
    QList<RNetworkModel *> networkComponents = theDomain->findChildren<RNetworkModel *>();

    for (int i=0; i<networkComponents.count(); i++) {

        networkComponents[i]->setTravelPaths(pathMatrix);
    }

    return 0;
}


int RModelManager::removeClosedPaths()
{
    if (pathMatrix.size() == 0) {
        qCritical() << "Cannot remove paths because no paths exist.";
        return 0;
    }

    // First loop through all network components and get a list of which ones are closed
    QList<RNetworkModel *> networkComponents = theDomain->findChildren<RNetworkModel *>();
    QList<RNetworkModel *> closedComponents;

    for (int i = 0; i < networkComponents.count(); i++) {

        // If this model is not a ship (ships availability works differently) and is not available
        if (networkComponents.at(i) && networkComponents.at(i)->getFunctionality() > 0.8) {

            qCritical() << networkComponents.at(i)->objectName() << "is closed";
            closedComponents << networkComponents.at(i);
        }
    }


    // Then loop through the paths matrix and identify affected paths
    QList<int> pathsToBeClosed;

    for (int i=0; i<pathMatrix.size(); i++) {

        for (int j=0; j<closedComponents.size(); j++) {

            if (pathMatrix.at(i).contains(closedComponents.at(j))) {

                if (!pathsToBeClosed.contains(i)) {

                    pathsToBeClosed << i;
                }
            }
        }
    }

    // Finally removed the paths that were affected
    qSort(pathsToBeClosed);

    for (int i=0; i<pathsToBeClosed.size(); i++) {

        int iaux = pathsToBeClosed.size() - (i+1);
        pathMatrix.removeAt(pathsToBeClosed.at(iaux));
    }

    return 0;
}


//int RModelManager::assignRoutes()
//{

//    // Get a list of all routes
//    QList<RRoute *> routes = theDomain->findChildren<RRoute *>();

//    // Get a list of all trucks
//    QList<RTruckModel *> trucks = theDomain->findChildren<RTruckModel *>();


//    // Start the search
//    for (int i=0; i<trucks.count(); i++) {

//        bool foundRoute = false;

//        for (int j=0; j<pathMatrix.count(); j++) {

//            if (pathMatrix[j].contains(trucks[i]) && !foundRoute) {

//                int truckIndex = pathMatrix[j].indexOf(trucks[i]);

//                // Check that there is something before and after the truck
//                if (truckIndex<1 || pathMatrix[j].count()<truckIndex+2) {
//                    qCritical() << "A truck must have an object both before and after.";
//                    return -1;
//                }

//                // Get location of object before and after this truck
//                RNetworkModel *theModelBefore = qobject_cast<RNetworkModel *>(pathMatrix[j][truckIndex-1]);
//                RNetworkModel *theModelAfter = qobject_cast<RNetworkModel *>(pathMatrix[j][truckIndex+1]);


//                trucks.at(i)->setOrigin(theModelBefore);
//                trucks.at(i)->setDestination(theModelAfter);


//                // See if there are any routes between those two locations
//                for (int k=0; k<routes.count(); k++) {

//                    if (routes[k]->getFromLocation() == theModelBefore->getLocation() && routes[k]->getToLocation() == theModelAfter->getLocation()) {

//                        //qDebug() << "Successfully found a route for" << trucks[i]->objectName() << "between" << routes[k]->getFromLocation()->objectName() << "and" << routes[k]->getToLocation()->objectName();
//                        trucks[i]->assignRoute(routes[k]);
//                        foundRoute = true;
//                    }
//                }
//            }
//        }


//        if (!foundRoute) {

//            qCritical() << "Could not find a route for" << trucks[i]->objectName();
//        }
//    }

//    return 0;
//}


//int RModelManager::assignMaritimeRoutes()
//{

//    // Get a list of all routes
//    QList<RRoute *> routes = theDomain->findChildren<RRoute *>();

//    // Get a list of all ships
//    QList<RShipModel *> ships = theDomain->findChildren<RShipModel *>();


//    // Start the search
//    for (int i=0; i<ships.count(); i++) {

//        bool foundRoute = false;

//        for (int j=0; j<pathMatrix.count(); j++) {

//            if (pathMatrix[j].contains(ships[i]) && !foundRoute) {

//                int shipIndex = pathMatrix[j].indexOf(ships[i]);

//                // Check that there is something before and after the ship
//                if (shipIndex<1 || pathMatrix[j].count()<shipIndex+2) {
//                    qCritical() << "A ship must have an object both before and after.";
//                    return -1;
//                }

//                // Get location of object before and after this ship
//                RNetworkModel *theModelBefore = qobject_cast<RNetworkModel *>(pathMatrix[j][shipIndex-1]);
//                RNetworkModel *theModelAfter = qobject_cast<RNetworkModel *>(pathMatrix[j][shipIndex+1]);

//                // See if there are any routes between those two locations
//                for (int k=0; k<routes.count(); k++) {

//                    if (routes[k]->getFromLocation() == theModelBefore->getLocation() && routes[k]->getToLocation() == theModelAfter->getLocation()) {

//                        //qDebug() << "Successfully found a route for" << ships[i]->objectName() << "between" << routes[k]->getFromLocation()->objectName() << "and" << routes[k]->getToLocation()->objectName();
//                        ships[i]->assignRoute(routes[k]);
//                        foundRoute = true;
//                    }
//                }
//            }
//        }

//        if (!foundRoute) {

//            qCritical() << "Could not find a route for" << ships[i]->objectName();
//        }
//    }

//    return 0;
//}


int RModelManager::printPaths()
{
    if (pathMatrix.size() == 0) {
        qCritical() << "Cannot print paths until the method establishPaths() is called. (Or perhaps there are no models?)";
        return 0;
    }

    for (int i=0; i<pathMatrix.count(); i++) {

        qDebug() << "***";
        qDebug() << "PATH NUMBER" << i+1;

        for (int j=0; j<pathMatrix[i].count(); j++) {

            qDebug() << pathMatrix[i][j]->objectName();
        }
    }
    return 0;
}
#endif


QList<QPair<double, QString> > *RModelManager::getTriggerTimes(double startTime, double endTime, double maxTimeGap)
{
    // Clear the list in case it was already populated
    triggerTimes.clear();


    // Loop through all models to get trigger times
    QList<QPair<double, QString> > allTriggerTimes;
    QList<RModel *> theModels = theDomain->findChildren<RModel *>();
    for (int i=0; i<theModels.count(); i++) {

        allTriggerTimes << theModels[i]->getTriggerTimes(startTime, endTime);
    }


    // Also loop through stand-alone RTime objects
    QList<RTime *> theStandAloneTimes = theDomain->findChildren<RTime *>();
    for (int i=0; i<theStandAloneTimes.count(); i++) {

        double time = theStandAloneTimes.at(i)->getCurrentValue();
        if (theStandAloneTimes.at(i)->objectName() != QString("officialCurrentTime")){
            allTriggerTimes.append(qMakePair(time, theStandAloneTimes.at(i)->objectName()));
        }
    }


    // Should remove all time instants after "end time"...


    // Bring together equal trigger times
    QString compoundAction;
    for (int i=0; i<allTriggerTimes.count(); i++) {

        compoundAction.clear();

        compoundAction.append(allTriggerTimes[i].second);

        for (int j=i+1; j<allTriggerTimes.count(); j++) {

            if (allTriggerTimes[i].first == allTriggerTimes[j].first && allTriggerTimes[i].second != "empty") {

                compoundAction.append(" and ");

                compoundAction.append(allTriggerTimes[j].second);

                allTriggerTimes[j].second = "empty";
                //qDebug() << "Now i=" << i << "and j=" << j << "with compound action:" << compoundAction;
            }
        }

        if (allTriggerTimes[i].second != "empty") {

            triggerTimes.append(qMakePair(allTriggerTimes[i].first, compoundAction));
        }
    }



    // Sort the trigger times
    qSort(triggerTimes.begin(), triggerTimes.end());




    // Make sure there are no time-gaps bigger than "maximum time between evaluations"
    double timeGap, previousTriggerTime, newTriggerTime;
    bool newTimesWereInserted = false;
    QList<QPair<double, QString> > tempListOfTriggerTimes;
    for (int i=0; i<triggerTimes.count()-1; i++) {
        timeGap = triggerTimes.at(i+1).first - triggerTimes.at(i).first;
        previousTriggerTime = triggerTimes.at(i).first;

        while (timeGap > maxTimeGap) {
            newTimesWereInserted = true;
            newTriggerTime = previousTriggerTime + maxTimeGap;
            tempListOfTriggerTimes.append(qMakePair(newTriggerTime, QString("Maximum Time Gap Evaluation")));
            timeGap = triggerTimes.at(i+1).first - newTriggerTime;
            previousTriggerTime = newTriggerTime;
        }
    }



    // Add the new times and sort all trigger times
    triggerTimes.append(tempListOfTriggerTimes);
    qSort(triggerTimes.begin(), triggerTimes.end());


    return &triggerTimes;
}





