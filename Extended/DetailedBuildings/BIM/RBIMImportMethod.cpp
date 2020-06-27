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

#include "RBIMImportMethod.h"
#include "RNode.h"
#include "RPoint.h"
#include "RPlateComponent.h"
#include "RColumnComponent.h"
#include "RWallComponent.h"
#include "RIFCImportData.h"
#include "RSlaveNode.h"
#include "RMathUtils.h"
#include "RNonLoadBearingWallComponent.h"
#include "RRCShearWallComponent.h"
#include "RRCSlabComponent.h"
#include "RWindowComponent.h"
#include "RRectangularRCColumnComponent.h"
#include "RIFCtoComponentConverter.h"
#include "RBIM.h"
#include "RLocation.h"

#include "IfcProduct.h"
#include "IfcBuilding.h"
#include "IfcBuildingStorey.h"
#include "IfcMaterial.h"
#include "IfcPhysicalQuantity.h"
#include "IfcQuantityLength.h"
#include "IfcQuantityArea.h"
#include "IfcQuantityVolume.h"
#include "IfcAreaMeasure.h"
#include "IfcVolumeMeasure.h"
#include "IfcRelAssociatesMaterial.h"
#include "IfcRelDefinesByType.h"
#include "IfcMaterialUsageDefinition.h"
#include "IfcMaterialLayerSetUsage.h"
#include "IfcMaterialProfileSetUsage.h"
#include "IfcMaterialLayerSet.h"
#include "IfcSlab.h"
#include "IfcSlabTypeEnum.h"
#include "IfcElementQuantity.h"
#include "IfcTypeObject.h"

#include "csg.hpp"
#include <QApplication>

RBIMImportMethod::RBIMImportMethod(RBIM* BIM, RDomain* Domain) : theBIM(BIM), theDomain(Domain)
{
    theIFCSystem = nullptr;
    theIFCppModel = nullptr;
    theRepresentationConverter = nullptr;
}


RBIMImportMethod::~RBIMImportMethod()
{

}


void RBIMImportMethod::modifyGeometry()
{
    connectionVector.size();

    for(auto&& it: connectionVector)
    {
        RComponent* component1 = nullptr;
        RComponent* component2 = nullptr;

        std::string entityType1;
        std::string entityType2;

        auto idFirstComponent = it.first;
        auto idSecondComponent = it.second;

        for(auto&& it2: mapofRtsIfcObjects)
        {
            auto entityId = it2.first->m_entity_id;

            if(entityId == idFirstComponent)
            {
                component1 = it2.second->theRComponent;
                entityType1 = it2.second->theEntityType;

                if(component2)
                    break;
                else
                    continue;
            }

            if(entityId == idSecondComponent)
            {
                component2 = it2.second->theRComponent;
                entityType2 = it2.second->theEntityType;

                if(component1)
                    break;
                else
                    continue;
            }
        }

        if(component1 == nullptr || component2==nullptr)
        {
            qDebug()<<"Connection failed for components with ID "<<idFirstComponent<<" and ID "<<idSecondComponent<<" in RBIM";
            continue;
        }
        else
        {
            if(entityType1.compare("IfcWallStandardCase") == 0 && entityType2.compare("IfcSlab") == 0)
            {
                modifyWallPlateGeometry(component1,component2);
            }
            else if(entityType1.compare("IfcSlab") == 0 && entityType2.compare("IfcWallStandardCase") == 0)
            {
                modifyWallPlateGeometry(component2,component1);
            }
            else if(entityType1.compare("IfcColumn") == 0 && entityType2.compare("IfcWallStandardCase") == 0)
            {
                // modifyWallColumnGeometry(component2,component1);
            }
            else if(entityType1.compare("IfcWallStandardCase") == 0 && entityType2.compare("IfcColumn") == 0)
            {
                // modifyWallColumnGeometry(component1,component2);
            }
        }
    }
}


void RBIMImportMethod::connectComponents()
{
    connectionVector.size();

    for(auto&& it: connectionVector)
    {
        RComponent* component1 = nullptr;
        RComponent* component2 = nullptr;

        std::string entityType1;
        std::string entityType2;

        auto idFirstComponent = it.first;
        auto idSecondComponent = it.second;

        for(auto&& it2: mapofRtsIfcObjects)
        {
            auto entityId = it2.first->m_entity_id;

            if(entityId == idFirstComponent)
            {
                component1 = it2.second->theRComponent;
                entityType1 = it2.second->theEntityType;


                if(component2)
                    break;
                else
                    continue;
            }

            if(entityId == idSecondComponent)
            {
                component2 = it2.second->theRComponent;
                entityType2 = it2.second->theEntityType;

                if(component1)
                    break;
                else
                    continue;
            }
        }

        if(component1 == nullptr || component2==nullptr)
        {
            qDebug()<<"Connection failed for components with ID "<<idFirstComponent<<" and ID "<<idSecondComponent<<" in RBIM";
            continue;
        }
        else
        {
            if(entityType1.compare("IfcWallStandardCase") == 0 && entityType2.compare("IfcSlab") == 0)
            {
                connectWallPlate(component1,component2);
            }
            else if(entityType1.compare("IfcSlab") == 0 && entityType2.compare("IfcWallStandardCase") == 0)
            {
                connectWallPlate(component2,component1);
            }
            else if(entityType1.compare("IfcSlab") == 0 && entityType2.compare("IfcColumn") == 0)
            {
                connectColumnPlate(component2,component1);
            }
            else if(entityType1.compare("IfcColumn") == 0 && entityType2.compare("IfcSlab") == 0)
            {
                connectColumnPlate(component1,component2);
            }
        }
    }
}


int RBIMImportMethod::connectColumnPlate(RComponent* column, RComponent* plate)
{
    // Check to see if they are already connected
    if(column->theConnectivityStruct.isConnectedToComponent(plate))
        return 1;

    // Get a vector of nodes from the plate
    std::vector<RNode*>* nodeVector = &plate->theConnectivityStruct.theNodeList;

    //Populate the node vector if it is empty
    if(nodeVector->empty())
    {
        auto plateElementList = plate->getElementList();
        for(auto&& it : plateElementList)
        {
            int numNodes = it->getNumNodes();
            for(int i =0; i<numNodes; ++i)
            {
                auto aNode = it->getNode(i);
                nodeVector->push_back(aNode);
            }
        }
    }

    if(nodeVector->empty())
    {
        qDebug()<<"Error getting the nodes in RBIM from component "<<plate->objectName();
        return -1;
    }

    // Cast the column into a beam column so we can get the points
    RBeamColumnComponent* theColumn = qobject_cast<RBeamColumnComponent*>(column);
    if(theColumn == nullptr)
    {
        qDebug()<<"Error in casting column to beam column in RBIM";
        return -1;
    }

    // Get the two points of the column
    RPoint* point1 = theColumn->thePoint1;
    RPoint* point2 = theColumn->thePoint2;

    RNode* node1 = theColumn->thePoint1->getAssociatedNode();
    RNode* node2 = theColumn->thePoint2->getAssociatedNode();

    auto distance1 = this->sqrdDistanceBtwnPts(nodeVector->front(),node1);
    auto distance2 = this->sqrdDistanceBtwnPts(nodeVector->front(),node2);

    //Find the closest node to the plate, connect that one
    for(auto&& it : *nodeVector)
    {
        auto newDistance1 = this->sqrdDistanceBtwnPts(it,node1);
        auto newDistance2 = this->sqrdDistanceBtwnPts(it,node2);

        if(newDistance1<distance1)
            distance1 = newDistance1;

        if(newDistance2<distance2)
            distance2 = newDistance2;
    }

    RPoint* connectingPoint = nullptr;

    if(distance1 < distance2)
        connectingPoint = point1;
    else
        connectingPoint = point2;

    if(fabs(connectingPoint->getZCoordinateValue()) <= 0.001) //Check to see if the point is on ground level, hence if it will be fixed
    {
        qDebug()<<"Warning, connecting point is on ground level in RBIM";
        return 0;
    }

    RNode* associatedNode = connectingPoint->getAssociatedNode();
    RNode* theClosestNode = nullptr;

    //Loop through the other components and find the closest node
    theClosestNode = this->findClosestNode(plate,associatedNode);

    if(theClosestNode == nullptr){
        qCritical()<<"Error in RBIM connectivity, could not find closest node or its parent component";
        return -1;
    }

    if(theClosestNode != associatedNode)
    {
        pointToSlaveNode(theClosestNode,associatedNode);
    }

    //Add the components to the respective connectivity lists
    plate->theConnectivityStruct.addConnectedComponent(column);
    column->theConnectivityStruct.addConnectedComponent(plate);

    return 0;
}


int RBIMImportMethod::connectWallPlate(RComponent* wall, RComponent* plate)
{
    // Check to see if they are already connected
    if(wall->theConnectivityStruct.isConnectedToComponent(plate))
        return 1;

    // Get a vector of nodes from the plate
    std::vector<RNode*>* nodeVector = &plate->theConnectivityStruct.theNodeList;

    // Populate the node vector if it is empty
    if(nodeVector->empty())
    {
        auto plateElementList = plate->getElementList();
        for(auto&& it : plateElementList)
        {
            int numNodes = it->getNumNodes();
            for(int i =0; i<numNodes; ++i)
            {
                auto aNode = it->getNode(i);
                nodeVector->push_back(aNode);
            }
        }
    }

    if(nodeVector->empty())
    {
        qDebug()<<"Error getting the nodes in RBIM from component "<<plate->objectName();
        return -1;
    }

    // Cast the wall into its parent class so we can get the points
    RWallComponent* theWall = qobject_cast<RWallComponent*>(wall);
    RPlateComponent* thePlate = qobject_cast<RPlateComponent*>(plate);

    if(theWall == nullptr || thePlate == nullptr)
    {
        qDebug()<<"Error in casting wall or plate in RBim connectWallPlate";
        return -1;
    }

    //Get the points of the wall
    RPoint* wallPoint1 = qobject_cast<RPoint*>(theWall->getPoint1());
    RPoint* wallPoint2 = qobject_cast<RPoint*>(theWall->getPoint2());
    RPoint* wallPoint3 = qobject_cast<RPoint*>(theWall->getPoint3());
    RPoint* wallPoint4 = qobject_cast<RPoint*>(theWall->getPoint4());

    std::vector<RPoint*> vecWallPoints{wallPoint1,wallPoint2,wallPoint3,wallPoint4};

    //Get the points of the wall
    RPoint* platePoint1 = qobject_cast<RPoint*>(thePlate->getPoint1());
    RPoint* platePoint2 = qobject_cast<RPoint*>(thePlate->getPoint2());
    RPoint* platePoint3 = qobject_cast<RPoint*>(thePlate->getPoint3());
    RPoint* platePoint4 = qobject_cast<RPoint*>(thePlate->getPoint4());

    std::vector<RPoint*> vecPlatePoints{platePoint1,platePoint2,platePoint3,platePoint4};

    //Find the closest edge to the plate, connect that one
    RMathUtils mathTool;

    auto edge12 = mathTool.getMidpoint(wallPoint1,wallPoint2);
    auto edge23 = mathTool.getMidpoint(wallPoint2,wallPoint3);
    auto edge34 = mathTool.getMidpoint(wallPoint3,wallPoint4);
    auto edge41 = mathTool.getMidpoint(wallPoint4,wallPoint1);

    std::vector<double> vecDistances(4);

    vecDistances.at(0) = fabs(mathTool.distanceFromPointToPlane(vecPlatePoints,edge12));
    vecDistances.at(1) = fabs(mathTool.distanceFromPointToPlane(vecPlatePoints,edge23));
    vecDistances.at(2) = fabs(mathTool.distanceFromPointToPlane(vecPlatePoints,edge34));
    vecDistances.at(3) = fabs(mathTool.distanceFromPointToPlane(vecPlatePoints,edge41));

    auto minDistance = vecDistances.at(0);

    int closestEdge = 0;

    for(int i =1; i<4;++i)
    {
        if(vecDistances.at(i) < minDistance)
        {
            minDistance = vecDistances.at(i);
            closestEdge = i;
        }
    }

    // Get the node matrix from the wall
    auto numberOfElementsAlongU = theWall->getNumElementsAlong12()->getCurrentValue();
    auto numberOfElementsAlongV = theWall->getNumElementsAlong23()->getCurrentValue();

    QVector< QVector<RNode *> > theNodeMatrix(numberOfElementsAlongU+1, QVector<RNode *>(numberOfElementsAlongV+1));

    QList<RElement *> theElementList = theWall->getElementList();

    //Assemble the node matrix, assembled the same way it is meshed,
    int counter = 0;
    for (int i=0; i<numberOfElementsAlongU; ++i)
    {
        for (int j=0; j<numberOfElementsAlongV; ++j)
        {
            auto theElement = theElementList.at(counter);

            theNodeMatrix[i][j]     = theElement->getNode(0);
            theNodeMatrix[i][j+1]   = theElement->getNode(1);
            theNodeMatrix[i+1][j+1] = theElement->getNode(2);
            theNodeMatrix[i+1][j]   = theElement->getNode(3);

            ++counter;
        }
    }

    auto numRow = theNodeMatrix.size();
    auto numCol = theNodeMatrix[0].size();

    // Connect the nodes along the closest edge of the wall to the closest node on the plate
    switch (closestEdge) {

    case 0:
        for(unsigned i = 0; i<numRow-1; ++i){

            RNode* theNode = theNodeMatrix[i][0];
            RNode* closestNode  = this->findClosestNode(plate,theNode);

            if(closestNode != theNode)
            {
                pointToSlaveNode(closestNode,theNode);
            }
        }
        break;

    case 1:
        for(unsigned i = 0; i<numCol-1; ++i){

            RNode* theNode = theNodeMatrix[numRow-1][i];
            RNode* closestNode  = this->findClosestNode(plate,theNode);

            if(closestNode != theNode)
            {
                pointToSlaveNode(closestNode,theNode);
            }
        }
        break;

    case 2:
        for(unsigned i = 0; i<numRow-1; ++i){

            RNode* theNode = theNodeMatrix[i][numCol-1];
            RNode* closestNode  = this->findClosestNode(plate,theNode);

            if(closestNode != theNode)
            {
                pointToSlaveNode(closestNode,theNode);
            }
        }
        break;

    case 3:
        for(unsigned i = 0; i<numCol-1; ++i){

            RNode* theNode = theNodeMatrix[0][i];
            RNode* closestNode  = this->findClosestNode(plate,theNode);

            if(closestNode != theNode)
            {
                pointToSlaveNode(closestNode,theNode);
            }
        }
        break;

    default:
        qDebug()<<"Error: closest edge of wall to plate not found in RBIM";
        return -1;
    }

    // Add the components to the connectivity lists
    plate->theConnectivityStruct.addConnectedComponent(wall);
    wall->theConnectivityStruct.addConnectedComponent(plate);

    return 0;
}


int RBIMImportMethod::modifyWallColumnGeometry(RComponent* wall, RComponent* column)
{
    // Check to see if they are already connected
    if(wall->theConnectivityStruct.isConnectedToComponent(column))
        return 1;


    // Cast the wall into its parent class so we can get the points
    RWallComponent* theWall = qobject_cast<RWallComponent*>(wall);
    RBeamColumnComponent* theColumn = qobject_cast<RBeamColumnComponent*>(column);

    if(theWall == nullptr || theColumn == nullptr)
    {
        qDebug()<<"Error in casting wall or column";
        return -1;
    }

    // Get the points of the wall
    RPoint* wallPoint1 = qobject_cast<RPoint*>(theWall->getPoint1());
    RPoint* wallPoint2 = qobject_cast<RPoint*>(theWall->getPoint2());
    RPoint* wallPoint3 = qobject_cast<RPoint*>(theWall->getPoint3());
    RPoint* wallPoint4 = qobject_cast<RPoint*>(theWall->getPoint4());

    // Get the points of the column
    RPoint* colPoint1 = qobject_cast<RPoint*>(theColumn->getPoint1());
    RPoint* colPoint2 = qobject_cast<RPoint*>(theColumn->getPoint2());

    // Find the closest edge of the wall to the column
    RMathUtils mathTool;

    std::vector<double> vecDistances(4);

    vecDistances.at(0) = fabs(mathTool.distance3DLineToLine(colPoint1,colPoint2,wallPoint1,wallPoint2));
    vecDistances.at(1) = fabs(mathTool.distance3DLineToLine(colPoint1,colPoint2,wallPoint2,wallPoint3));
    vecDistances.at(2) = fabs(mathTool.distance3DLineToLine(colPoint1,colPoint2,wallPoint3,wallPoint4));
    vecDistances.at(3) = fabs(mathTool.distance3DLineToLine(colPoint1,colPoint2,wallPoint4,wallPoint1));

    auto minDistance = vecDistances.at(0);

    int closestEdge = 0;

    for(int i =1; i<4;++i)
    {
        if(vecDistances.at(i) < minDistance)
        {
            minDistance = vecDistances.at(i);
            closestEdge = i;
        }
    }

    // Check to make sure the edges are parallel along their entire lengths
    // Check to make sure the column is meshed with a 2D mesh, i.e., a frame element
    // If either is meshed with 3D i.e., brick elements, then the connection should be made at the surface
    // If both are 2D, extend the wall to meet the column
    // If column is meshed with frame/fibre and wall with plate, then column should go through every node on that edge of the plate
    // To do: implement a connection algorithm for connecting a column that abuts a wall
    switch (closestEdge) {

    case 0:

        break;

    case 1:

        break;

    case 2:

        break;

    case 3:

        break;

    default:
        qDebug()<<"Error: closest edge of wall to column not found in RBIM";
        return -1;
    }

    // Add the components to the respective connectivity lists
    column->theConnectivityStruct.addConnectedComponent(wall);
    wall->theConnectivityStruct.addConnectedComponent(column);

    return 0;
}


int RBIMImportMethod::modifyWallPlateGeometry(RComponent* wall, RComponent* plate)
{

    // Cast the plate component
    auto thePlate = qobject_cast<RPlateComponent*>(plate);

    auto theWall = qobject_cast<RWallComponent*>(wall);

    // Get the wall and plate thickness
    auto theWallThickness = theWall->getThickness()->getCurrentValue();

    if(!thePlate || !theWall)
    {
        qDebug()<<"The connectWallPlate function requires a plate and wall component";
        return -1;
    }

    RMathUtils theMathTool;

    // Get some points from the walls and plates to build planes
    RPoint* wallPoint1 = qobject_cast<RPoint*> (theWall->getPoint1());
    RPoint* wallPoint2 = qobject_cast<RPoint*> (theWall->getPoint2());
    RPoint* wallPoint3 = qobject_cast<RPoint*> (theWall->getPoint3());
    RPoint* wallPoint4 = qobject_cast<RPoint*> (theWall->getPoint4());

    // qDebug()<<"Wall point1 ("<<wallPoint1->getXCoordinateValue()<<","<<wallPoint1->getYCoordinateValue()<<","<<wallPoint1->getZCoordinateValue()<<")";
    // qDebug()<<"Wall point2 ("<<wallPoint2->getXCoordinateValue()<<","<<wallPoint2->getYCoordinateValue()<<","<<wallPoint2->getZCoordinateValue()<<")";
    // qDebug()<<"Wall point3 ("<<wallPoint3->getXCoordinateValue()<<","<<wallPoint3->getYCoordinateValue()<<","<<wallPoint3->getZCoordinateValue()<<")";
    // qDebug()<<"Wall point4 ("<<wallPoint4->getXCoordinateValue()<<","<<wallPoint4->getYCoordinateValue()<<","<<wallPoint4->getZCoordinateValue()<<")";

    std::vector<RPoint*> vectorOfWallPoints{wallPoint1,wallPoint2,wallPoint3,wallPoint4};


    RPoint* platePoint1 =qobject_cast<RPoint*> (thePlate->getPoint1());
    RPoint* platePoint2 =qobject_cast<RPoint*> (thePlate->getPoint2());
    RPoint* platePoint3 =qobject_cast<RPoint*> (thePlate->getPoint3());
    RPoint* platePoint4 =qobject_cast<RPoint*> (thePlate->getPoint4());

    // qDebug()<<"Plate point1 ("<<platePoint1->getXCoordinateValue()<<","<<platePoint1->getYCoordinateValue()<<","<<platePoint1->getZCoordinateValue()<<")";
    // qDebug()<<"Plate point2 ("<<platePoint2->getXCoordinateValue()<<","<<platePoint2->getYCoordinateValue()<<","<<platePoint2->getZCoordinateValue()<<")";
    // qDebug()<<"Plate point3 ("<<platePoint3->getXCoordinateValue()<<","<<platePoint3->getYCoordinateValue()<<","<<platePoint3->getZCoordinateValue()<<")";
    // qDebug()<<"Plate point4 ("<<platePoint4->getXCoordinateValue()<<","<<platePoint4->getYCoordinateValue()<<","<<platePoint4->getZCoordinateValue()<<")";

    std::vector<RPoint*> vectorOfPlatePoints{platePoint1,platePoint2,platePoint3,platePoint4};


    auto getStdVectorFromPnt = [&](RPoint* point){

        std::vector<double> vectorOfPointCoords = {point->getXCoordinateValue(),point->getYCoordinateValue(),point->getZCoordinateValue()};

        return vectorOfPointCoords;
    };

    auto wallNormalVec = theMathTool.computePolygonNormal<RPoint*>(vectorOfWallPoints);
    auto plateNormalVec = theMathTool.computePolygonNormal<RPoint*>(vectorOfPlatePoints);

    // Create the planes
    std::vector<double> wPoint1 = {wallPoint1->getXCoordinateValue(),wallPoint1->getYCoordinateValue(),wallPoint1->getZCoordinateValue()};
    std::vector<double> pPoint1 = {platePoint1->getXCoordinateValue(),platePoint1->getYCoordinateValue(),platePoint1->getZCoordinateValue()};

    std::vector<std::vector<double>> wallPlane{wPoint1,wallNormalVec};
    std::vector<std::vector<double>> platePlane{pPoint1,plateNormalVec};

    //declare a vector to hold the resulting intersection line
    std::vector<std::vector<double>> intersectionLine(2, std::vector<double>(3));

    auto res =  theMathTool.intersect3DTwoPlanes(wallPlane,platePlane,&intersectionLine);

    if(res != 2)
    {
        qCritical()<<"Error, the wall and slab planes do not have an intersection... but they should since they are connected!";
        return -1;
    }

    std::vector<double> p1intersection =  intersectionLine.at(0);
    std::vector<double> p2intersection =  intersectionLine.at(1);

    // thePlotter->plotLineDebug(p1intersection,p2intersection);

    // qDebug()<<"The intersection line P1 ("<<p1intersection.at(0)<<","<<p1intersection.at(1)<<","<<p1intersection.at(2)<<")";
    // qDebug()<<"The intersection line P2 ("<<p2intersection.at(0)<<","<<p2intersection.at(1)<<","<<p2intersection.at(2)<<")";

    // Find the intersection points of the wall and plate

    // Check each edge of the wall to see where the wall intersects the plate
    // IF an intersection is found:
    // Adjust the height of the wall so that it intersects with the plane of the plate
    // Check if any edges of the wall are colinear with the intersection plane, if so the wall is already at the height of the plate midplane
    std::vector<double> wPoint2 = getStdVectorFromPnt(vectorOfWallPoints.at(1));
    std::vector<double> wPoint3 = getStdVectorFromPnt(vectorOfWallPoints.at(2));
    std::vector<double> wPoint4 = getStdVectorFromPnt(vectorOfWallPoints.at(3));

    std::vector<double> intersectionPoint1;
    auto check1 = theMathTool.intersect3DTwoLines(p1intersection,p2intersection,wPoint1,wPoint2,&intersectionPoint1);

    std::vector<double> intersectionPoint3;
    auto check3 = theMathTool.intersect3DTwoLines(p1intersection,p2intersection,wPoint3,wPoint4,&intersectionPoint3);

    // Check if edges 1-2 and 3-4 are by any chance colinear to the slab.. if so wall is already at the correct height
    if(check1 == 2 || check3 == 2)
    {
        return 0;
    }

    // If not, need to find the intersection of edges 2-3 and 1-4 with the intersection line

    std::vector<double> intersectionPoint2;
    auto check2 = theMathTool.intersect3DTwoLines(p1intersection,p2intersection,wPoint2,wPoint3,&intersectionPoint2);

    std::vector<double> intersectionPoint4;
    auto check4 = theMathTool.intersect3DTwoLines(p1intersection,p2intersection,wPoint4,wPoint1,&intersectionPoint4);

    // qDebug()<<"Wall point 1"<<wPoint1[0]<<wPoint1[1]<<wPoint1[2];
    // qDebug()<<"Wall point 2"<<wPoint2[0]<<wPoint2[1]<<wPoint2[2];

    // qDebug()<<"intersectionPoint2"<<intersectionPoint2[0]<<intersectionPoint2[1]<<intersectionPoint2[2];
    // qDebug()<<"intersectionPoint4"<<intersectionPoint4[0]<<intersectionPoint4[1]<<intersectionPoint4[2];

    // Result of 5 = there is no intersection
    if(check2 ==5 || check4 == 5)
    {
        qCritical()<<"Error, there an intersection was not found where there is one";
        return -1;
    }

    // thePlotter->plotLineDebug(intersectionPoint2,intersectionPoint4);

    // Check to see that both the wall and the plate are meshed with a plate mesh
    if(!intersectionPoint2.empty())
    {
        auto xCoord = intersectionPoint2[0];
        auto yCoord = intersectionPoint2[1];
        auto zCoord = intersectionPoint2[2];

        RPoint* closestWallPoint = theMathTool.getClosestPointOrNode<RPoint>(intersectionPoint2,vectorOfWallPoints);
        RPoint* closestPlatePoint = theMathTool.getClosestPointOrNode<RPoint>(intersectionPoint2,vectorOfPlatePoints);

        if(closestWallPoint)
        {
            closestWallPoint->setXCoordinate(xCoord);
            closestWallPoint->setYCoordinate(yCoord);
            closestWallPoint->setZCoordinate(zCoord);

        }

        if(closestPlatePoint)
        {
            // If the plate edge is within the thickness of the wall, trim the plate edge back, otherwise could be an interior wall
            auto sqrdDistanceBtwnPoints = theMathTool.sqrdDistanceBtwnPointsOrNodes<RPoint>(&intersectionPoint2,closestPlatePoint);

            if(sqrdDistanceBtwnPoints <= theWallThickness*theWallThickness)
            {
                closestPlatePoint->setXCoordinate(xCoord);
                closestPlatePoint->setYCoordinate(yCoord);
                closestPlatePoint->setZCoordinate(zCoord);
            }
        }
    }

    if(!intersectionPoint4.empty())
    {
        auto xCoord = intersectionPoint4[0];
        auto yCoord = intersectionPoint4[1];
        auto zCoord = intersectionPoint4[2];

        RPoint* closestWallPoint = theMathTool.getClosestPointOrNode<RPoint>(intersectionPoint4,vectorOfWallPoints);
        RPoint* closestPlatePoint = theMathTool.getClosestPointOrNode<RPoint>(intersectionPoint4,vectorOfPlatePoints);

        if(closestWallPoint)
        {
            closestWallPoint->setXCoordinate(xCoord);
            closestWallPoint->setYCoordinate(yCoord);
            closestWallPoint->setZCoordinate(zCoord);
        }

        if(closestPlatePoint)
        {
            //if the plate edge is within the thickness of the wall, trim the plate edge back, otherwise could be an interior wall
            auto sqrdDistanceBtwnPoints = theMathTool.sqrdDistanceBtwnPointsOrNodes<RPoint>(&intersectionPoint4,closestPlatePoint);

            if(sqrdDistanceBtwnPoints <= theWallThickness*theWallThickness)
            {
                closestPlatePoint->setXCoordinate(xCoord);
                closestPlatePoint->setYCoordinate(yCoord);
                closestPlatePoint->setZCoordinate(zCoord);
            }
        }
    }

    return 0;
}


void RBIMImportMethod::getConnectivityRelationships(void)
{
    // Create a CSG tool
    carve::csg::CSG theCSGTool;

    // Get the geometry converter from the IFC system
    shared_ptr<GeometryConverter> theGeomConverter = theIFCSystem->getGeometryConverter();

    // Get a map of the item shape data
    std::map<int, shared_ptr<ProductShapeData>> mapShapeData = theGeomConverter->getShapeInputData();

    // The int "key" corresponds to the entity id
    std::map<int, std::vector<carve::mesh::MeshSet<3>*>> mapOfMeshSets;

    // Only interested in collisions of the building elements, i.e., those imported above such as IfcBeam, IfcColumn, IfcSlab, etc.
    for (auto&& it : mapofRtsIfcObjects)
    {
        int ifcEntityId = it.first->m_entity_id;

        auto theComponent = it.second->theRComponent;

        // Delete this when want to add the stiffness of the nonloadbearing components or windows
        if(qobject_cast<RNonLoadBearingWallComponent*>(theComponent) || qobject_cast<RWindowComponent*>(theComponent))
            continue;

        shared_ptr<ProductShapeData> theProductShapeData = nullptr;

        auto search = mapShapeData.find(ifcEntityId);
        if (search != mapShapeData.end()) {
            theProductShapeData = search->second;
        } else {
            qDebug()<<"Error, could not get product shape data of imported Rts Ifc Object from map";
            continue;
        }

        // std::stringstream stream;
        // theProductShapeData->m_ifc_object_definition.lock()->getStepLine(stream);
        // qDebug()<<"For ID "<<ifcEntityId<<QString::fromStdString(stream.str());

        std::vector<shared_ptr<RepresentationData> > theItemRepresentationData = theProductShapeData->m_vec_representations;
        for(auto&& it2 :theItemRepresentationData)
        {
            std::vector<shared_ptr<ItemShapeData> > vecItemShapeData = it2->m_vec_item_data;
            for(auto&& it3 :vecItemShapeData)
            {
                // Create a global mesh set
                std::vector<carve::mesh::MeshSet<3>* > vecMeshSetsGlobal;

                // Get the meshset in its local configuration
                std::vector<shared_ptr<carve::mesh::MeshSet<3> > > vecMeshSetsLocal = it3->m_meshsets;

                // For collision detection we need the global location of the meshset
                // Get the transform from the product shape data
                carve::math::Matrix theTransformationMatrix = theProductShapeData->getTransform();

                for(auto&& meshIt: vecMeshSetsLocal)
                {
                    carve::mesh::MeshSet<3>* globalMesh = meshIt.get();

                    for( size_t i = 0; i < globalMesh->vertex_storage.size(); ++i )
                    {
                        vec3& point = globalMesh->vertex_storage[i].v;
                        point = theTransformationMatrix*point;
                    }
                    for( size_t i = 0; i < globalMesh->meshes.size(); ++i )
                    {
                        globalMesh->meshes[i]->recalc();
                    }

                    vecMeshSetsGlobal.push_back(globalMesh);
                }

                mapOfMeshSets.insert( std::make_pair(ifcEntityId, vecMeshSetsGlobal ) );
            }
        }
    }


    for(auto&& it:mapOfMeshSets)
    {
        //To do: Implement a check to capture collisions in composite shapes with more than one mesh set
        carve::mesh::MeshSet<3>* mesh1 = it.second.at(0);

        auto id1 = it.first;

        // Get a list of vertices in mesh 1
        std::vector<carve::mesh::Vertex<3>> theVertexVector = mesh1->vertex_storage;

        bool intersectionFound = false;
        for(auto&& it2:mapOfMeshSets)
        {
            auto id2 = it2.first;

            if(id1 == id2)
                continue;

            intersectionFound = false;

            // Get the mesh for the second object
            carve::mesh::MeshSet<3>* mesh2 = it2.second.at(0);

            for(auto&& it: theVertexVector)
            {
                if(intersectionFound)
                    break;

                // Iterate through the faces in mesh two to see if any vertex from mesh1 intersects those faces
                for (meshset_t::face_iter f = mesh2->faceBegin(); f != mesh2->faceEnd();++f)
                {
                    face_t* fa = *f;

                    intersectionFound = fa->containsPoint(it.v);

                    // If an intersection is found then make the connection
                    if(intersectionFound)
                    {
                        // qDebug()<<"surface intersection between "<<id1<<" and "<<id2;
                        this->addConnection(id1,id2);
                        break;
                    }
                }
            }

            // If no intersection of volumes, then check for meshes that are touching each other
            if(!intersectionFound)
            {
                // Try to get a mesh of the intersection
                auto meshSet = theCSGTool.compute(mesh1,mesh2,carve::csg::CSG::OP::INTERSECTION);

                // If there is a mesh, it means the two volumes intersect and this is the resulting mesh of the intersection
                if(meshSet->meshes.size() > 0)
                {
                    // qDebug()<<"volume intersection between "<<id1<<" and "<<id2;
                    intersectionFound = true;
                    this->addConnection(id1,id2);
                }
                // else
                //      qDebug()<<"no volume intersection between "<<id1<<" and "<<id2;
            }
        }
    }


    // Moved here from LoadIfcFileCommand
    theGeomConverter->clearInputCache();

    return;
}


void RBIMImportMethod::meshComponents(void)
{
    auto thePointsList = theDomain->findChildren<RPoint*>();

    if(thePointsList.empty())
        return;

    for(auto&& it: mapofRtsIfcObjects)
    {
        RComponent* theComponent = it.second->theRComponent;

        if(theComponent)
        {
            int isMeshed = theComponent->createMesh();

            if(isMeshed != 0)
                qDebug()<<"Warning: this component failed to mesh in RBIM: "<<theComponent->objectName();
        }
    }
}


void RBIMImportMethod::optimizeMesh()
{
    auto thePointsList = theDomain->findChildren<RPoint*>();

    if(thePointsList.empty())
        return;

    for(auto it: thePointsList)
    {
        QVector<RPoint*> vecOfPointsToSwap;

        for(auto it2: thePointsList)
        {
            if(it == it2)
                continue;

            if(this->arePointsCoinciding(it,it2))
                vecOfPointsToSwap.push_back(it2);
        }

        if(!vecOfPointsToSwap.empty())
        {
            for(auto it3 : vecOfPointsToSwap)
                this->swapPoint(it3,it);

            //recursively call this function again
            this->optimizeMesh();
            return;
        }
    }

}


void RBIMImportMethod::swapPoint(RPoint* originalPoint, RPoint* replacementPoint)
{
    for(auto&& it: mapofRtsIfcObjects)
    {
        RComponent* theComponent = it.second->theRComponent;

        auto thePointList = theComponent->getPointList();

        if(thePointList.isEmpty())
        {
            qDebug()<<"Error an empty point list was received from "<<theComponent->objectName();
            return;
        }

        auto aPlate = qobject_cast<RPlateComponent*>(theComponent);

        auto aWall = qobject_cast<RWallComponent*>(theComponent);

        auto aBeamColumn = qobject_cast<RBeamColumnComponent*>(theComponent);

        if(aPlate)
        {
            if(aPlate->getPoint1() == originalPoint)
                aPlate->setPoint1(replacementPoint);
            else if(aPlate->getPoint2() == originalPoint)
                aPlate->setPoint2(replacementPoint);
            else if(aPlate->getPoint3() == originalPoint)
                aPlate->setPoint3(replacementPoint);
            else if(aPlate->getPoint4() == originalPoint)
                aPlate->setPoint4(replacementPoint);
        }

        if(aWall)
        {
            if(aWall->getPoint1() == originalPoint)
                aWall->setPoint1(replacementPoint);
            else if(aWall->getPoint2() == originalPoint)
                aWall->setPoint2(replacementPoint);
            else if(aWall->getPoint3() == originalPoint)
                aWall->setPoint3(replacementPoint);
            else if(aWall->getPoint4() == originalPoint)
                aWall->setPoint4(replacementPoint);
        }

        if(aBeamColumn)
        {
            if(aBeamColumn->getPoint1() == originalPoint)
                aBeamColumn->setPoint1(replacementPoint);
            else if(aBeamColumn->getPoint2() == originalPoint)
                aBeamColumn->setPoint2(replacementPoint);
        }
    }

    // Get the associated node from the original point
    auto assocNd = originalPoint->getAssociatedNode();

    theDomain->deleteObject(originalPoint->objectName());

    if(assocNd)
        theDomain->deleteObject(assocNd->objectName());
}


double RBIMImportMethod::sqrdDistanceBtwnPts(RNode* pt1, RNode* pt2)
{
    return (pt2->getXCoordinateValue()-pt1->getXCoordinateValue())*(pt2->getXCoordinateValue()-pt1->getXCoordinateValue())+
            (pt2->getYCoordinateValue()-pt1->getYCoordinateValue())*(pt2->getYCoordinateValue()-pt1->getYCoordinateValue())+
            (pt2->getZCoordinateValue()-pt1->getZCoordinateValue())*(pt2->getZCoordinateValue()-pt1->getZCoordinateValue());
}


void RBIMImportMethod::pointToSlaveNode(RNode* masterNode, RNode* slaveNode)
{

    // Check if it is already a slave node - can only be one slave node but a master can have many slave nodes
    RSlaveNode* theSlaveNode = qobject_cast<RSlaveNode*>(slaveNode);
    if(theSlaveNode)
        return;

    auto swapOutNode = [&](RNode* oldNode, RNode* newNode)
    {
        // QString oldNodename = oldNode->objectName();
        // QString newNodename = newNode->objectName();
        // qDebug()<<"oldNode:"<<oldNodename;
        // qDebug()<<"newNode:"<<newNodename;

        for(auto&& it: mapofRtsIfcObjects)
        {
            RComponent* theComponent = it.second->theRComponent;

            //Get the connectivity struct from the component
            componentConnectivityStruct& theCompConnStruct = theComponent->theConnectivityStruct;

            std::vector<RNode*> theComponentNodeList;

            // Collect all of the nodes and points from the elements within the components
            auto compElementList = theComponent->getElementList();
            for(auto&& it : compElementList)
            {
                int numNodes = it->getNumNodes();
                for(int i = 0; i<numNodes; ++i)
                {
                    auto aNode = it->getNode(i);

                    if(aNode == oldNode)
                    {
                        // Set the new node to the element
                        it->setNode(i,newNode);
                        theComponentNodeList.push_back(newNode);

                        // Update the element so that it picks up the new nodes
                        it->update();
                    }
                    else
                    {
                        theComponentNodeList.push_back(aNode);
                    }
                }
            }
            theCompConnStruct.theNodeList  = theComponentNodeList;

            auto theCompPointsList = theComponent->getPointList();
            for(auto&& it: theCompPointsList)
            {
                if(it->getAssociatedNode() == oldNode)
                {
                    it->createOrSetAssociatedNode(newNode);
                }
            }
        }

    };

    theDomain->createObject(slaveNode->objectName()+"IsNowSlaveNodeOf"+masterNode->objectName(),"RSlaveNode");
    theSlaveNode = qobject_cast<RSlaveNode*>(theDomain->getLastAddedObject());

    theSlaveNode->setMasterNode(masterNode);
    theSlaveNode->intializeNode(slaveNode); //Copy the class members over

    //Now need to replace the old node with the created slave node
    //Since Qt does not allow assignment or copying in its infinite wisdom,
    //we must iterate through all of the components and manually change the RNode pointer to
    //the points and elements rather than simply assigning a new value to the value
    //that the RNode pointer is pointing at.

    RNode* theSlaveBaseNode = qobject_cast<RNode*>(theSlaveNode);

    //swap out the current associated node with the slave node
    swapOutNode(slaveNode,theSlaveBaseNode);

    //Finally delete the current node
    QString nodeToDeleteName = slaveNode->objectName();
    theDomain->deleteObject(nodeToDeleteName);

}


RNode* RBIMImportMethod::findClosestNode(RComponent* theComponent, RNode* theNode)
{
    auto theNodeList = theComponent->theConnectivityStruct.theNodeList;

    if(theNodeList.empty())
    {
        qCritical()<<"The nodelist is empty in "<<__FUNCTION__;
        return nullptr;
    }

    RNode* closestNode = nullptr;
    auto distance = this->sqrdDistanceBtwnPts(theNode,theNodeList.front());

    // Now loop through the rest of the node list
    for(auto&& it : theNodeList)
    {
        auto newDistance = this->sqrdDistanceBtwnPts(theNode,it);
        if(newDistance <= distance)
        {
            distance = newDistance;
            closestNode = it;
        }
    }

    // Check to see if the closest node and the incoming node are the one and the same!
    if(closestNode == theNode)
        return theNode;

    // Something went wrong
    if(closestNode == nullptr || distance == 0.0)
        qCritical()<<"Error in finding the closest node in "<<__FUNCTION__;

    return closestNode;
}


std::unordered_map<IfcProduct *, std::shared_ptr<RIFCImportData> > RBIMImportMethod::getMapofRtsIfcObjects() const
{
    return mapofRtsIfcObjects;
}


void RBIMImportMethod::setTheIFCppModel(const std::shared_ptr<IfcPPModel> &value)
{
    theIFCppModel = value;
}


void RBIMImportMethod::setTheIFCSystem(const QPointer<IfcPlusPlusSystem> &value)
{
    theIFCSystem = value;
}


void RBIMImportMethod::setMapOfIfcEntity(const std::map<int, std::shared_ptr<IfcPPEntity> > &value)
{
    mapOfIfcEntity = value;
}


bool RBIMImportMethod::arePointsCoinciding(RPoint* P1, RPoint* P2)
{
    auto epsilon = 1e-6;

    auto x1 = P1->getXCoordinateValue();
    auto y1 = P1->getYCoordinateValue();
    auto z1 = P1->getZCoordinateValue();

    auto x2 = P2->getXCoordinateValue();
    auto y2 = P2->getYCoordinateValue();
    auto z2 = P2->getZCoordinateValue();

    //If there is a point at these coordinates, store it
    if(fabs(x1-x2) < epsilon && fabs(y1-y2) < epsilon && fabs(z1-z2) < epsilon )
        return true;

    return false;
}


RPoint* RBIMImportMethod::findPoint(double &x, double& y, double& z)
{
    // Check for exisiting points at this location and use this point if it exists
    RPoint* existingPoint = nullptr;

    auto pointList = theDomain->findChildren<RPoint*>();

    auto epsilon = 1e-6;

    for(auto&& it: pointList)
    {
        auto xCoord = it->getXCoordinateValue();
        auto yCoord = it->getYCoordinateValue();
        auto zCoord = it->getZCoordinateValue();

        // If there is a point at these coordinates, save it and break out of the loop
        if(fabs(xCoord-x) < epsilon && fabs(yCoord-y) < epsilon && fabs(zCoord-z) < epsilon )
        {
            existingPoint = it;
            break;
        }
    }

    return existingPoint;
}


void RBIMImportMethod::convertIfcObjecttoComponent(const shared_ptr<IfcProduct>& product, const std::string& theClassType)
{
    if( !product || !product->m_Representation)
    {
        qCritical()<<"Null pointer of IfcProduct or the IfcProduct is missing its representation in "<<__FUNCTION__;
        return;
    }

    const int product_id = product->m_entity_id;

    // Create a container to store the information from BIM that is useful
    shared_ptr<RIFCImportData> productGeomInputData( new RIFCImportData() );
    productGeomInputData->theIfcProduct = product;

    // Set the type of entity i.e., IfcColumn, IfcBeam, etc.
    productGeomInputData->theEntityType = theClassType;

    // Get the representation, the parent class for geometry items
    shared_ptr<IfcProductRepresentation>& productRepresentation = product->m_Representation;
    std::vector<shared_ptr<IfcRepresentation> >& vectorRepresentations = productRepresentation->m_Representations;

    // Get the material from the product
    //      Two ways:
    //      1) either from the objects m_HasAssociations_inverse or,
    //      2) it could be a RelDefines so the material from another object will define this objects material

    // Try this object first
    shared_ptr<IfcObjectDefinition> thisObjectDef =  dynamic_pointer_cast<IfcObjectDefinition>(product);

    // First check if it has the association inverse
    if(thisObjectDef)
    {
        auto vecOfAssociations = thisObjectDef->m_HasAssociations_inverse;
        for(auto&& it: vecOfAssociations)
        {
            shared_ptr<IfcRelAssociatesMaterial> theMaterial =  dynamic_pointer_cast<IfcRelAssociatesMaterial>(it.lock());
            if(theMaterial)
            {
                shared_ptr<IfcMaterialSelect> relMaterial = theMaterial->m_RelatingMaterial;
                productGeomInputData->theIfcMaterial = relMaterial;
                break;
            }
        }
    }

    // Now check if material is defined from a relationship with another object
    if(productGeomInputData->theIfcMaterial.lock() == nullptr)
    {
        shared_ptr<IfcObject> theIfcObject =  dynamic_pointer_cast<IfcObject>( product );
        auto theRelDefinesByTypeVec = theIfcObject->m_IsTypedBy_inverse; //vector of IfcRelDefinesByType
        for(auto&& it:theRelDefinesByTypeVec)
        {
            // See if the relating type exists
            shared_ptr<IfcObjectDefinition> theTypeObjectCastToObjectDef =  dynamic_pointer_cast<IfcObjectDefinition>(it.lock()->m_RelatingType);
            if(theTypeObjectCastToObjectDef)
            {
                // Get the vector of associated inverses
                auto vecOfAssocInv = theTypeObjectCastToObjectDef->m_HasAssociations_inverse;
                for(auto&& it2: vecOfAssocInv)
                {
                    shared_ptr<IfcRelAssociatesMaterial> theMaterial =  dynamic_pointer_cast<IfcRelAssociatesMaterial>(it2.lock());
                    if(theMaterial)
                    {
                        shared_ptr<IfcMaterialSelect> relMaterial = theMaterial->m_RelatingMaterial;
                        productGeomInputData->theIfcMaterial = relMaterial;
                        break;
                    }
                }
            }
        }
    }

    if(productGeomInputData->theIfcMaterial.lock() == nullptr)
    {
        qCritical()<<"Error the material for object "<<QString::fromStdWString(product->m_Name->m_value)<<" could not be deduced";
    }

    // Now extract the material from the IfcMaterialSelect - there are 3 options:
    // 1) IfcMaterialUsageDefinition, 2) IfcMaterialDefinition, and 3) IfcMaterialList
    shared_ptr<IfcMaterialSelect> relMaterial = productGeomInputData->theIfcMaterial.lock();
    // 1) IfcMaterialUsageDefinition
    auto matUsageDef = dynamic_pointer_cast<IfcMaterialUsageDefinition>(relMaterial);

    // 2) IfcMaterialDefinition
    auto matDef = dynamic_pointer_cast<IfcMaterialDefinition>(relMaterial);

    // 3) IfcMaterialList
    auto matList = dynamic_pointer_cast<IfcMaterialList>(relMaterial);

    if(matUsageDef)
    {
        // There are two ways of assigning a set of materials with shape parameters:
        //      1) a layer set to a reference curve - assigning a material layer set with material layers having a sequence and thickness parameter to a reference curve of an element - represented by the 'Axis' shape representation of that element
        //      2) a profile set to a cardinal point - assigning a material profile set with assigned profile geometry and insertion points to a reference curve by an offset, called "cardinal point" - the reference curve is represented by the 'Axis' shape representation of that element
        // Each instantiable subtype of IfcMaterialUsageDefinition has to be assigned to a subtype of IfcElement by using the objectified relationship IfcRelAssociatesMaterial; it is only valid in conjunction with an element occurrence.

        // IfcMaterialUsageDefinition is an abstract supertype of IfcMaterialLayerSetUsage or IfcMaterialProfileSetUsage
        // 1) IfcMaterialLayerSetUsage
        auto matLayerSetUsage = dynamic_pointer_cast<IfcMaterialLayerSetUsage>(matUsageDef);

        // 2) IfcMaterialProfileSetUsage
        auto matProfSetUsage = dynamic_pointer_cast<IfcMaterialProfileSetUsage>(matUsageDef);

        if(matLayerSetUsage)
        {
            // Get the layer set
            std::shared_ptr<IfcMaterialLayerSet> layerSet = matLayerSetUsage->m_ForLayerSet;
            // Get a vector of the material layers
            std::vector<shared_ptr<IfcMaterialLayer> >	 matLayers = layerSet->m_MaterialLayers;

            // Could be a case where wall is made from a material of many layers
            // Set the material layers vector
            productGeomInputData->materialLayers = matLayers;

            if(matLayers.size() != 1)
            {
                qDebug()<<"Warning: Only one material layer is currently supported";
            }
            else
            {
                // Get the material
                auto firstMatLayer = matLayers.at(0);

                auto theMaterial = firstMatLayer->m_Material;

                this->getMaterial(theMaterial.get(),productGeomInputData.get());
            }
        }
        else if(matProfSetUsage)
        {
            qDebug()<<"Need to implement IfcMaterialProfileSetUsage";

        }
        else
        {
            qDebug()<<"Error in IfcMaterialUsageDefinition";
        }
    }
    else if(matDef)
    {
        // IfcMaterialDefinition is a general supertype for all material related information items in IFC that have common material related properties that may include association of material with some shape parameters or assignments to identified parts of a component.

        // Get the material
        auto theMaterial = dynamic_pointer_cast<IfcMaterial>(matDef);

        if(theMaterial)
        {
            this->getMaterial(theMaterial.get(),productGeomInputData.get());
        }
        else
        {
            qDebug()<<"Error in IfcMaterialDefinition";
        }
    }
    else if(matList)
    {
        productGeomInputData->theMaterialType = RIFCImportData::materialType::LAYERED;

        for(auto&& it : matList->m_Materials)
        {
            QString matName = QString::fromStdWString(it->m_Name->toString());

            if(matName.compare("Background") == 0 )
            {
                productGeomInputData->theMatList.push_back(RIFCImportData::materialType::wood);
            }
            else if(matName.compare("Rigid Insulation") == 0 )
            {
                productGeomInputData->theMatList.push_back(RIFCImportData::materialType::rigidInsulation);
            }
            else if(matName.compare("Air Space") == 0 )
            {
                productGeomInputData->theMatList.push_back(RIFCImportData::materialType::airSpace);
            }
            else if(matName.compare("Batt Insulation") == 0 )
            {
                productGeomInputData->theMatList.push_back(RIFCImportData::materialType::battInsulation);
            }
            else
            {
                auto name = QString::fromStdWString(productGeomInputData->theIfcProduct.lock()->m_Name->m_value);
                qDebug()<<"Unable to determine the material type from: "<<matName<<" for object "<<name<<" Defaulting to wood.";
            }
        }
    }
    else
    {
        qDebug()<<"Error getting the type of material from "<<QString::fromStdWString(product->m_Name->m_value)<<" in "<<__FUNCTION__;
    }

    // Try to get the connectivity information, if any, from IFC
    auto theIFCElement = dynamic_pointer_cast<IfcElement>(product);
    std::vector<weak_ptr<IfcRelConnectsElements> >	 vecConnectedTo = theIFCElement->m_ConnectedTo_inverse;
    std::vector<weak_ptr<IfcRelConnectsElements> >	 vecConnectedFrom = theIFCElement->m_ConnectedFrom_inverse;

    // Add it to the connectivity manager
    // for(auto&& it : vecConnectedTo)
    // {
    //     auto idIt = it.lock()->m_entity_id;
    //     theConnectionManager.addConnection(product_id,idIt);
    // }
    // for(auto&& it : vecConnectedFrom)
    // {
    //     auto idIt = it.lock()->m_entity_id;
    //     theConnectionManager.addConnection(product_id,idIt);
    // }

    // Now extract the geometry and create the components
    for(auto&& it : vectorRepresentations)
    {
        if(theClassType == "IfcBeam" || theClassType =="IfcColumn")
            theRepresentationConverter->componentFromIFCBeamColumn(it, productGeomInputData, theClassType);
        else if(theClassType =="IfcSlab")
            theRepresentationConverter->componentFromIFCSlabWall(it, productGeomInputData, theClassType);
        else if(theClassType=="IfcWall" || theClassType == "IfcWallStandardCase")
            theRepresentationConverter->componentFromIFCSlabWall(it, productGeomInputData, theClassType);
        else if(theClassType=="IfcWindow")
            theRepresentationConverter->componentFromIFCWindow(it, productGeomInputData, theClassType);
    }

    // At this point a component should have been created, check!
    RComponent* theComponent = productGeomInputData->theRComponent;
    if(!theComponent)
    {
        qCritical()<<"A component of the type "<<QString::fromStdString(theClassType)<<" was not created in RBIM for "<<QString::fromStdWString(product->m_Name->m_value)<<" with id: "<<product_id;
        return;
    }

    // Now get the properties, if any, from IFC
    // Here is where we determine if the element is an interior or exterior element - from ArchiCAD this is set with the IsExternal property
    // It is also where we determine, in the case of a slab, if the element is a floor or roof element
    auto theObject = dynamic_pointer_cast<IfcObject>(thisObjectDef);

    auto vecProperties = this->getVectorIFCProperties(theObject);
    for(auto theProperty : vecProperties)
    {
        // Cast it as an ifc property single value
        auto theProp = dynamic_pointer_cast<IfcPropertySingleValue>(theProperty);
        if(theProp)
        {
            auto propertyName = QString::fromStdWString(theProperty->m_Name->m_value);
            // qDebug()<<propertyName;

            // ArchiCAD sets this property as "IsExternal"
            if(propertyName.compare("IsExternal")==0)
            {
                // IfcSimpleValue A select type for basic defined types of simple data type.
                // IfcInteger Defined type of simple type INTEGER.
                // IfcReal Defined type of simple type REAL.
                // IfcBoolean Defined type of simple type BOOLEAN.
                // IfcLogical Defined type of simple type LOGICAL.
                // IfcIdentifier Defined type of simple type STRING for identification purposes.
                // IfcLabel Defined type of simple type STRING for naming purposes.
                // IfcText Defined type of simple type STRING for descriptive purposes.
                // IfcMeasureValue A select type for basic measure types of ISO/CD 10303-41.
                // IfcDerivedMeasureValue A select type for derived measure types.

                auto asBoolean = dynamic_pointer_cast<IfcBoolean>(theProp->m_NominalValue);
                if(asBoolean)
                {
                    // Get the boolean value from the iterator
                    bool isExternal = asBoolean->m_value;

                    // Set the value to the component
                    if(isExternal)
                    {
                        theComponent->setInteriorExterior(RComponent::RInteriorOrExteriorComponent::EXTERIOR);
                    }
                    else
                        theComponent->setInteriorExterior(RComponent::RInteriorOrExteriorComponent::INTERIOR);
                }
            }
        }
    }

    // Then get the map of product shape data
    auto mapOfProductShapeData = theIFCSystem->getGeometryConverter()->getShapeInputData();

    // Now deal with the openings, if there are any
    std::vector<weak_ptr<IfcRelVoidsElement> > vectorOfOpenings = theIFCElement->m_HasOpenings_inverse;
    if(!vectorOfOpenings.empty())
    {
        // There is code in the IfcPP method subtract openings that saves the meshsets of the openings so that we do not have to convert them here again

        // From the map, find the product shape data
        auto theShapeData = mapOfProductShapeData.find(product_id)->second;

        theRepresentationConverter->getVoidsFromIFCRepresentation(theShapeData,productGeomInputData);
    }

    // Do some special things depending on the type of component
    // If the component is a slab, check if it is a floor or roof
    if(theClassType.compare("IfcSlab")==0)
    {
        auto theIfcSlab = dynamic_pointer_cast<IfcSlab>(product);

        // Check to make sure it is a plate
        auto plateComponent = qobject_cast<RPlateComponent*>(theComponent);

        if(!plateComponent || !theIfcSlab)
        {
            qDebug()<<"Warning, an IfcSlab could not be cast as a plate component, or product is not a slab but labelled as one";
        }
        else
        {
            shared_ptr<IfcSlabTypeEnum>	theSlabType =	theIfcSlab->m_PredefinedType;

            switch (theSlabType->m_enum) {
            case IfcSlabTypeEnum::IfcSlabTypeEnumEnum::ENUM_NOTDEFINED:
                break;
            case IfcSlabTypeEnum::IfcSlabTypeEnumEnum::ENUM_FLOOR:
                plateComponent->setPlateType(RPlateComponent::plateType::FLOOR);
                break;
            case IfcSlabTypeEnum::IfcSlabTypeEnumEnum::ENUM_ROOF:
                qDebug()<<"Roof, make sure this works and delete me!";
                plateComponent->setPlateType(RPlateComponent::plateType::ROOF);
                break;
            case IfcSlabTypeEnum::IfcSlabTypeEnumEnum::ENUM_BASESLAB:
                plateComponent->setPlateType(RPlateComponent::plateType::BASESLAB);
                break;
            case IfcSlabTypeEnum::IfcSlabTypeEnumEnum::ENUM_LANDING:
                plateComponent->setPlateType(RPlateComponent::plateType::LANDING);
                break;
            default:
                break;
            }
        }
    }

    mapofRtsIfcObjects.emplace(std::make_pair(product.get(),productGeomInputData));
}


void RBIMImportMethod::getMaterial(IfcMaterial* theMaterial, RIFCImportData* theRePData)
{

    // The material labels below correspond to the material labels assigned by ArchiCAD
    // These lables may be different with other BIM programs
    QString matName = QString::fromStdWString(theMaterial->m_Name->toString());

    if(matName.compare("Plywood") == 0 )
    {
        theRePData->theMaterialType = RIFCImportData::materialType::wood;
    }
    else if(matName.compare("Concrete - Structural") == 0 )
    {
        theRePData->theMaterialType = RIFCImportData::materialType::concrete;
    }
    else if(matName.compare("Reinforced Concrete - Structural") == 0 )
    {
        theRePData->theMaterialType = RIFCImportData::materialType::reinforcedConcrete;
    }
    else if(matName.compare("GENERIC - STRUCTURAL") == 0 )
    {
        auto name = QString::fromStdWString(theRePData->theIfcProduct.lock()->m_Name->m_value);
        qDebug()<<"The object "<<name<<" has a generic material assigned. Defaulting to wood.";
        theRePData->theMaterialType = RIFCImportData::materialType::wood;
    }
    else
    {
        auto name = QString::fromStdWString(theRePData->theIfcProduct.lock()->m_Name->m_value);
        qDebug()<<"Unable to determine the material type from: "<<matName<<" for object "<<name<<" Defaulting to wood.";
        theRePData->theMaterialType = RIFCImportData::materialType::wood;
    }

    return;
}


void RBIMImportMethod::addConnection(const int id1,const int id2)
{
    if(checkConnectionExists(id1,id2))
        return;
    else
        connectionVector.push_back(std::make_pair(id1,id2));
}


bool RBIMImportMethod::checkConnectionExists(const int id1,const int id2)
{
    for(auto&& it: connectionVector)
    {
        if((it.first == id1 && it.second == id2) || (it.first == id2 && it.second == id1))
            return true;
    }
    return false;
}


int RBIMImportMethod::convertBIMtoRBIM()
{
    // Get the geometry settings and unit converter first
    shared_ptr<GeometrySettings>	geomSettings = theIFCSystem->getGeomSettings();
    shared_ptr<UnitConverter>	    unitConverter = theIFCppModel->getUnitConverter();

    if(geomSettings == nullptr || unitConverter == nullptr)
    {
        qCritical()<<"Empty pointer(s); no geometry settings or unit converter given in "<<__FUNCTION__;
        return -1;
    }

    // Create new representation converter tool, this tool converts an IFC object into an Rts component
    theRepresentationConverter = make_shared<RIFCtoComponentConverter>(geomSettings,unitConverter, theDomain);

    // Get the unit conversion factor, always using meters
    auto unitConversionFactor = unitConverter->getLengthInMeterFactor();

    auto numberColumns = 0;
    auto numberSlabs = 0;
    auto numberWalls = 0;
    auto numberWindows = 0;

    std::vector<shared_ptr<IfcBuildingStorey>> buildingStoreyVector;
    std::vector<shared_ptr<IfcSpace>> buildingSpaceVector;
    shared_ptr<IfcBuilding> theBuilding = nullptr;

    clock_t millisecs = clock();

    // When looping through first check to see if it is an IfcProduct as only these can have geometry, ignore everything else.
    for( auto&& it : mapOfIfcEntity)
    {
        shared_ptr<IfcPPEntity> obj = it.second;

        //Do a check for IfcBuildingElement here and continue if its not, dont waste time
        shared_ptr<IfcBuildingElement> buildingElement = dynamic_pointer_cast<IfcBuildingElement>(obj);
        shared_ptr<IfcBuildingStorey> buildingStorey = dynamic_pointer_cast<IfcBuildingStorey>(obj);
        shared_ptr<IfcSpace> buildingSpace = dynamic_pointer_cast<IfcSpace>(obj);
        shared_ptr<IfcBuilding> building = dynamic_pointer_cast<IfcBuilding>(obj);
        if(buildingElement)
        {
            shared_ptr<IfcProduct> product = dynamic_pointer_cast<IfcProduct>(obj);

            // Check to see if an IfcProductRepresentation, only IfcProductRepresentations can be structural objects
            if(product->m_Representation)
            {

                // Get the entity type
                auto theEntityType = obj->className();

                // Now do a 'case' operation to find supported IFC objects
                if(strcmp(theEntityType,"IfcColumn") == 0)
                {
                    convertIfcObjecttoComponent(product, theEntityType);
                    ++numberColumns;
                }
                else if (strcmp(theEntityType,"IfcBeam") == 0)
                {
                    convertIfcObjecttoComponent(product, theEntityType);
                }
                else if (strcmp(theEntityType,"IfcSlab") == 0)
                {
                    convertIfcObjecttoComponent(product, theEntityType);
                    ++numberSlabs;
                }
                else if (strcmp(theEntityType,"IfcWall") == 0)
                {
                    convertIfcObjecttoComponent(product, theEntityType);
                    numberWalls++;
                }
                else if (strcmp(theEntityType,"IfcWallStandardCase") == 0)
                {
                    convertIfcObjecttoComponent(product, theEntityType);
                    numberWalls++;
                }
                else if (strcmp(theEntityType,"IfcWindow") == 0)
                {
                    convertIfcObjecttoComponent(product, theEntityType);
                    numberWindows++;
                }
            }
        }
        else if(buildingStorey)
        {
            buildingStoreyVector.push_back(buildingStorey);
        }
        else if(buildingSpace)
        {
            buildingSpaceVector.push_back(buildingSpace);
        }
        else if(building)
        {
            theBuilding = building;
        }
        else
            continue;
    }

    auto sizeMap = mapofRtsIfcObjects.size();

    clock_t timeDiff1 = clock() - millisecs;
    qDebug()<<"The number of IFC objects loaded:"<<sizeMap<<" in "<<QString::number( round(timeDiff1*0.1)*1e-5 )<<" sec.";
    qDebug()<<"Number slabs: "<<numberSlabs<<", Number columns: "<<numberColumns<<", Number walls: "<<numberWalls<<", Number windows: "<<numberWindows;

    // Next perfrom the mesh optimization, connectivity, and mesh refinement
    qDebug()<<"Establishing component connectivity";
    QCoreApplication::processEvents();
    clock_t millisecs2 = clock();

    // Get the connectivity relationship of components where connectivity is not specified through IfcRelConnects
    this->getConnectivityRelationships();

    clock_t timeDiff2 = clock() - millisecs2;
    qDebug()<<"Connectivity established in "<<QString::number(round(timeDiff2*0.1)*1e-5)<<" seconds";

    // Modify the geometery of component, depending on the type of components that are connected, e.g., beam-column, beam-beam, slab-wall connections
    qDebug()<<"Modifying geometry of connected components";
    QCoreApplication::processEvents();

    clock_t millisecs3 = clock();

    this->modifyGeometry();

    clock_t timeDiff3 = clock() - millisecs3;
    qDebug()<<"Done modifying geometry "<<QString::number( round(timeDiff3*0.1)*1e-5 )<<" sec.";

    // Optimize the mesh -- check for coinciding nodes, etc.
    qDebug()<<"Optimizing mesh";
    QCoreApplication::processEvents();

    clock_t millisecs4 = clock();

    this->optimizeMesh();

    clock_t timeDiff4 = clock() - millisecs4;
    qDebug()<<"Done mesh optimization"<<QString::number( round(timeDiff4*0.1)*1e-5 )<<" sec.";

    // Mesh the components with a finite element mesh
    qDebug()<<"Creating finite element mesh";
    QCoreApplication::processEvents();
    clock_t millisecs5 = clock();

    this->meshComponents();

    clock_t timeDiff5 = clock() - millisecs5;
    qDebug()<<"Done finite element mesh creation"<<QString::number( round(timeDiff5*0.1)*1e-5 )<<" sec.";


    qDebug()<<"Connect finite element mesh";
    QCoreApplication::processEvents();
    clock_t millisecs6 = clock();

    // Connect the mesh
    this->connectComponents();

    clock_t timeDiff6 = clock() - millisecs6;
    qDebug()<<"Done connecting finite element mesh"<<QString::number( round(timeDiff6*0.1)*1e-5 )<<" sec.";

    // Get the component list from the BIM
    auto compList = theBIM->getComponentList();

    auto numShearWalls = 0;
    auto areaWindows = 0.0;
    auto numExteriorComponents = 0;
    auto numExteriorWalls = 0;
    auto numExteriorWindows = 0;
    auto m3Concrete = 0.0;
    auto m3ReinfSteel = 0.0;
    auto m3Wood = 0.0;

    for(auto&& it : compList)
    {
        if(qobject_cast<RRCShearWallComponent*>(it))
            ++numShearWalls;

        auto window = qobject_cast<RWindowComponent*>(it);

        if(window)
            areaWindows += window->getSurfaceArea();

        if(it->isExterior())
        {
            if( qobject_cast<RNonLoadBearingWallComponent*>(it))
                ++numExteriorWalls;

            if(window)
                ++numExteriorWindows;

            ++numExteriorComponents;
        }

        m3Concrete += it->getVolumeOfConcrete();
        m3ReinfSteel += it->getVolumeOfSteel();
        m3Wood += it->getVolumeOfWood();
    }

    qDebug()<<"numShearWalls: "<<numShearWalls;
    qDebug()<<"areaWindows: "<<areaWindows;
    qDebug()<<"numExteriorComponents: "<<numExteriorComponents;
    qDebug()<<"numExteriorWalls: "<<numExteriorWalls;
    qDebug()<<"numExteriorWindows: "<<numExteriorWindows;
    qDebug()<<"m3 of concrete: "<<m3Concrete;
    qDebug()<<"kg of reinforcing steel: "<<m3ReinfSteel*7900.0;
    qDebug()<<"m3 of wood: "<<m3Wood;


    // Set the building height and storey/height of each component
    theBIM->theBuildingHeight = 0.0;

    auto numStoreys = buildingStoreyVector.size();

    theBIM->theNumberOfStoreys = numStoreys;
    theBIM->buildingStoreyElevation.resize(numStoreys);
    theBIM->buildingStoreyHeight.resize(numStoreys);

    for(auto i = 0; i<numStoreys; ++i)
    {
        auto buildingStorey = buildingStoreyVector.at(i);
        auto storeyElevation = buildingStorey->m_Elevation->m_value*unitConversionFactor;

        // Set the elevation of the storey
        theBIM->buildingStoreyElevation[i] = storeyElevation;

        // Calculate the ceiling height of the storey
        if(i != 0)
        {
            auto prevElev = theBIM->buildingStoreyElevation[i-1];

            auto ceilingHeight = storeyElevation - prevElev;

            theBIM->buildingStoreyHeight[i] = ceilingHeight;
        }

        // Now get the building quantities such as volume, area
        auto grossStoreyFloorArea = 0.0;
        auto grossStoreyHeight = 0.0;

        auto storeyObject = dynamic_pointer_cast<IfcObject>(buildingStorey);

        // Get the vector of quantities (IfcPhysicalQuantities)
        auto vectorQuantities = this->getVectorPhysicalQuantities(storeyObject);

        for(auto physicalQuantity : vectorQuantities)
        {
            auto name = QString::fromStdWString(physicalQuantity->m_Name->m_value);
            if(name.compare("GrossFloorArea") == 0)
            {
                auto theArea = dynamic_pointer_cast<IfcQuantityArea>(physicalQuantity);

                grossStoreyFloorArea = theArea->m_AreaValue->m_value;
            }
            else if(name.compare("GrossHeight") == 0)
            {
                auto theLength = dynamic_pointer_cast<IfcQuantityLength>(physicalQuantity);

                grossStoreyHeight = theLength->m_LengthValue->m_value*unitConversionFactor;
            }
        }

        // Rough calculation of the building volume, if the BIM output program provides another value, this will replace the value calculated below
        theBIM->theBuildingVolume += grossStoreyFloorArea*grossStoreyHeight;

        //Get the m_ContainsElements_inverse from the spatial element class (two parent classes above this)
        std::vector<weak_ptr<IfcRelContainedInSpatialStructure> > vectorOfSpatialStructures = buildingStorey->m_ContainsElements_inverse;

        for(auto it : vectorOfSpatialStructures)
        {
            auto theSpatialStructure = it.lock();
            if(theSpatialStructure)
            {
                std::vector<shared_ptr<IfcProduct> > vecIfcProduct = theSpatialStructure->m_RelatedElements;
                for(auto theIfcproduct : vecIfcProduct)
                {
                    shared_ptr<IfcBuildingElement> buildingElement = dynamic_pointer_cast<IfcBuildingElement>( theIfcproduct );
                    if(!buildingElement)
                        continue;

                    // Find the Rts component corresponding to that ifcproduct, and set the information to the component
                    auto search = mapofRtsIfcObjects.find(theIfcproduct.get());
                    if (search != mapofRtsIfcObjects.end())
                    {
                        shared_ptr<RIFCImportData> IfcObjectData = search->second;

                        RComponent* theComponent = IfcObjectData->theRComponent;

                        theComponent->setElevation(storeyElevation);
                        theComponent->setStoreyNumber(i);
                    }
                    // else {
                    //     qDebug()<<"Could not find object"<<QString::fromStdWString(theIfcproduct->toString()) <<"contained in storey "<<i;
                    // }
                }
            }
        }
    }

    // Calculate the building height
    theBIM->theBuildingHeight = theBIM->buildingStoreyElevation.back() + theBIM->buildingStoreyHeight.back();

    // Calculate the square footage and the occupancy types from the custom IFC spaces or zones
    for(auto it: buildingSpaceVector)
    {
        QString OccupancyType = "null";

        auto spaceObject = dynamic_pointer_cast<IfcObject>(it);

        // Get the vector of properties (IfcProperty)
        auto propertyVec = this->getVectorIFCProperties(spaceObject);

        for(auto theProperty : propertyVec)
        {
            // Cast as an ifc property single value
            auto theProp = dynamic_pointer_cast<IfcPropertySingleValue>(theProperty);
            if(theProp)
            {
                // Try to cast as an Ifc label
                auto ifcLabel = dynamic_pointer_cast<IfcLabel>(theProp->m_NominalValue);

                if(ifcLabel)
                {
                    auto labelValue = QString::fromStdWString(ifcLabel->m_value);

                    if(labelValue.compare("Office")==0)
                    {
                        OccupancyType = "Commercial";
                    }

                    if(labelValue.compare("Residential and Recreation")==0)
                    {
                        OccupancyType = "Residential";
                    }
                }
            }
        }

        // Get the vector of IfcPhysicalQuantities quantities
        auto vectorQuantities = this->getVectorPhysicalQuantities(spaceObject);

        for(auto physicalQuantity : vectorQuantities)
        {
            // Get the gross floor areas
            auto name = QString::fromStdWString(physicalQuantity->m_Name->m_value);
            if(name.compare("GrossFloorArea") == 0)
            {
                auto theArea = dynamic_pointer_cast<IfcQuantityArea>(physicalQuantity);

                auto floorArea = theArea->m_AreaValue->m_value;

                // Check the occupancy type
                if(OccupancyType.compare("Commercial") == 0)
                {
                    theBIM->commercialFloorArea += floorArea;
                }
                else if(OccupancyType.compare("Residential") == 0)
                {
                    theBIM->residentialFloorArea += floorArea;
                }
            }
        }


        // Calculate the number of residential housing units
        if(theBIM->numberOfHousingUnits == 1)
        {
            auto spatialEl = dynamic_pointer_cast<IfcSpatialElement>(it);
            if(spatialEl)
            {
                auto labelValue = QString::fromStdWString(spatialEl->m_LongName->m_value);

                // This is a custom created zone in ArchiCAD named "unit"; this convention has to be followed
                if(labelValue.compare("Unit")==0)
                {
                    ++theBIM->numberOfHousingUnits;
                }
            }
        }
    }


    // Now calculate the building height, volume, gross area, etc. - whatever the BIM can give us
    if(theBuilding)
    {
        //cast to ifc object
        auto buildingObject = dynamic_pointer_cast<IfcObject>(theBuilding);

        //get the vector of quantities (IfcPhysicalQuantities)
        auto vectorQuantities = this->getVectorPhysicalQuantities(buildingObject);

        for(auto physicalQuantity : vectorQuantities)
        {
            auto name = QString::fromStdWString(physicalQuantity->m_Name->m_value);
            if(name.compare("GrossFloorArea") == 0)
            {
                auto theArea = dynamic_pointer_cast<IfcQuantityArea>(physicalQuantity);

                auto floorArea = theArea->m_AreaValue->m_value;

                theBIM->theTotalFloorlArea = floorArea;
            }
            else if(name.compare("GrossVolume") == 0)
            {
                auto theVolume = dynamic_pointer_cast<IfcQuantityVolume>(physicalQuantity);

                auto grossVolume = theVolume->m_VolumeValue->m_value;

                theBIM->theBuildingVolume = grossVolume;
            }
        }
    }
    else
    {
        qDebug()<< "Error, an IfcBuilding was not found in "<<__FUNCTION__;
    }


    qDebug()<<"Number of storeys: "<<theBIM->getNumberOfStoreys();
    qDebug()<<"Residential floor area"<< theBIM->residentialFloorArea<<" sq. m";
    qDebug()<<"Commercial floor area"<< theBIM->commercialFloorArea<<" sq. m";
    qDebug()<<"Building height"<< theBIM->theBuildingHeight<<" m";
    qDebug()<<"Total Floor Area"<< theBIM->theTotalFloorlArea<<" sq. m";
    qDebug()<<"Building Volume"<<theBIM->theBuildingVolume<<" cubic m";
    qDebug()<<"Number Of Housing Units"<<theBIM->numberOfHousingUnits;

    // Finally, set the BIM location
    auto theBIMLocation = qobject_cast<RLocation*>(theBIM->getLocation());

    if(theBIMLocation)
        qDebug()<<"Latitude: "<<theBIMLocation->getLatitude()<<", Longitude: "<<theBIMLocation->getLongitude();


    qDebug()<<"BIM import completed successfully";

    return 0;
}


std::vector<shared_ptr<IfcProperty> > RBIMImportMethod::getVectorIFCProperties(shared_ptr<IfcObject>& ifcObject)
{

    std::vector<shared_ptr<IfcProperty> > result;

    //Get the vector of properties
    std::vector<weak_ptr<IfcRelDefinesByProperties> > DefinedBy_inverse = ifcObject->m_IsDefinedBy_inverse;
    for(auto it : DefinedBy_inverse)
    {
        auto ifcRel = it.lock();
        if(ifcRel)
        {
            //get the property set
            shared_ptr<IfcPropertySetDefinitionSelect> relatingPropertyDefinition = ifcRel->m_RelatingPropertyDefinition;

            //down-cast to a property set
            auto propertySet = dynamic_pointer_cast<IfcPropertySet>(relatingPropertyDefinition);
            if(propertySet)
            {
                std::vector<shared_ptr<IfcProperty> > properties = propertySet->m_HasProperties;
                for(auto it2 : properties)
                    result.push_back(it2);
            }
        }
    }

    return result;
}


std::vector<shared_ptr<IfcPhysicalQuantity> > RBIMImportMethod::getVectorPhysicalQuantities(shared_ptr<IfcObject>& ifcObject)
{

    std::vector<shared_ptr<IfcPhysicalQuantity> > result;

    //Get the vector of properties
    std::vector<weak_ptr<IfcRelDefinesByProperties> > DefinedBy_inverse = ifcObject->m_IsDefinedBy_inverse;
    for(auto it : DefinedBy_inverse)
    {
        auto ifcRel = it.lock();
        if(ifcRel)
        {
            // Get the property set denition
            shared_ptr<IfcPropertySetDefinitionSelect> relatingPropertyDefinition = ifcRel->m_RelatingPropertyDefinition;

            // Cast to a quantity set
            auto quantitySet = dynamic_pointer_cast<IfcElementQuantity>(relatingPropertyDefinition);
            if(quantitySet)
            {
                // Store the quantities
                std::vector<shared_ptr<IfcPhysicalQuantity> > properties = quantitySet->m_Quantities;
                for(auto it2 : properties)
                    result.push_back(it2);
            }
        }
    }

    return result;
}



