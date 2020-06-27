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

#include "RWallMesh.h"
#include "RMathUtils.h"
#include "RElement.h"

RWallMesh::RWallMesh(RDomain *theDomain, QString name)
    : RMesh(theDomain, name)
{
    theElementType = "NULL";

    thePoint1 = nullptr;
    thePoint2 = nullptr;
    thePoint3 = nullptr;
    thePoint4 = nullptr;
    theThickness  = nullptr;
}


double RWallMesh::getDrift()
{
    auto node1 = thePoint1->getAssociatedNode();
    auto node2 = thePoint2->getAssociatedNode();

    auto node3 = thePoint3->getAssociatedNode();
    auto node4 = thePoint4->getAssociatedNode();

    const Vector &disp1 = node1->getTrialDisp();
    const Vector &disp2 = node2->getTrialDisp();
    const Vector &disp3 = node3->getTrialDisp();
    const Vector &disp4 = node4->getTrialDisp();

    auto ux41 = disp4(0)-disp1(0);
    auto uy41 = disp4(1)-disp1(1);
    auto dz41 = node4->getZCoordinateValue() - node1->getZCoordinateValue();


    auto ux32 = disp3(0)-disp2(0);
    auto uy32 = disp3(1)-disp2(1);
    auto dz32 = node3->getZCoordinateValue() - node2->getZCoordinateValue();

    //Get the drift value
    auto delta32  = sqrt(ux32*ux32+uy32*uy32);

    auto delta41  = sqrt(ux41*ux41+uy41*uy41);

    auto drift32 = delta32/dz32;

    auto drift41 = delta41/dz41;

    auto drift = (drift41 > drift32 ? drift41 : drift32);

    if(drift > maxDrift)
        maxDrift = drift;

    return drift;
}


RWallMesh::~RWallMesh()
{
}


int  RWallMesh::createTopologicalMesh(void)
{

    RMathUtils theMathTool;

    theTopoMesh = make_shared<RTopoMesh>(this);

    auto numberOfElementsAlongU = static_cast<int>(theNumElementsAlongU->getCurrentValue());
    auto numberOfElementsAlongV = static_cast<int>(theNumElementsAlongV->getCurrentValue());

    QVector< QVector<RNode *> > theNodeMatrix(numberOfElementsAlongU+1, QVector<RNode *>(numberOfElementsAlongV+1));

    int counter = 0;
    for (int i=0; i<numberOfElementsAlongU; ++i)
    {
        for (int j=0; j<numberOfElementsAlongV; ++j)
        {
            auto theElement = theElementList->at(counter);

            theNodeMatrix[i][j]     = theElement->getNode(0);
            theNodeMatrix[i][j+1]   = theElement->getNode(1);
            theNodeMatrix[i+1][j+1] = theElement->getNode(2);
            theNodeMatrix[i+1][j]   = theElement->getNode(3);

            ++counter;
        }
    }

    auto numRow = theNodeMatrix.size();
    auto numCol = theNodeMatrix[0].size();

    //Check to make sure the number of rows is equal to numberOfElementsAlongV and the number of columns is equal to numberOfElementsAlongU
    if(numRow-1 != numberOfElementsAlongU || numCol-1 != numberOfElementsAlongV )
    {
        qCritical()<<"Error in topo meshing, matrices are not consistent";
        return -1;
    }

    auto dZ = 0.5*theThickness->getCurrentValue();

    counter = 0;

    //OpenGL quad vertices order are defined in a Counter Clockwise fashion
    //First create the front and back faces
    for(int i = 0; i<numRow-1; ++i){
        for(int j = 0; j<numCol-1; ++j){

            auto theElement = theElementList->at(counter);
            auto damageMesh = theElement->getTheDamageRegion();

            if(damageMesh == nullptr)
            {
                qDebug()<<"Error getting the damage mesh from element "<<theElement->objectName()<<" in "<<__FUNCTION__;
            }

            RNode* theNode1 = theNodeMatrix[i][j];
            RNode* theNode2 = theNodeMatrix[i+1][j];
            RNode* theNode3 = theNodeMatrix[i+1][j+1];
            RNode* theNode4 = theNodeMatrix[i][j+1];

            std::vector<RNode*> vectorOfNodes{theNode1,theNode2,theNode3,theNode4};

            //Get the normal vector for the face of the wall
            auto normalVec = theMathTool.computePolygonNormal<RNode*>(vectorOfNodes);

            auto vertex1 = theTopoMesh->addVertex(theNode1,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
            auto vertex2 = theTopoMesh->addVertex(theNode2,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
            auto vertex3 = theTopoMesh->addVertex(theNode2,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);
            auto vertex4 = theTopoMesh->addVertex(theNode1,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);
            auto vertex5 = theTopoMesh->addVertex(theNode4,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
            auto vertex6 = theTopoMesh->addVertex(theNode3,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
            auto vertex7 = theTopoMesh->addVertex(theNode3,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);
            auto vertex8 = theTopoMesh->addVertex(theNode4,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);

            //Now mesh out one face
            auto edge1 = theTopoMesh->addEdge(vertex1,vertex2);
            auto edge2 = theTopoMesh->addEdge(vertex2,vertex6);
            auto edge3 = theTopoMesh->addEdge(vertex6,vertex5);
            auto edge4 = theTopoMesh->addEdge(vertex5,vertex1);

            RTopoFace face1(edge1,edge2,edge3,edge4);

            //Now mesh out the other face
            auto edge5 = theTopoMesh->addEdge(vertex3,vertex4);
            auto edge6 = theTopoMesh->addEdge(vertex4,vertex8);
            auto edge7 = theTopoMesh->addEdge(vertex8,vertex7);
            auto edge8 = theTopoMesh->addEdge(vertex7,vertex3);

            RTopoFace face2(edge5,edge6,edge7,edge8);

            face1.setIndex(4);
            face2.setIndex(1);

            face1.setTheDamageRegion(damageMesh);
            face2.setTheDamageRegion(damageMesh);

            theTopoMesh->addFace(face1);
            theTopoMesh->addFace(face2);

            ++counter;
        }
    }

//    Mesh out the long sides
    counter = 0;
    for(int i = 0; i<numRow-1; ++i){

        auto theElement = theElementList->at(counter);
        auto damageMesh = theElement->getTheDamageRegion();

        if(damageMesh == nullptr)
        {
            qDebug()<<"Error getting the damage mesh from element "<<theElement->objectName()<<" in "<<__FUNCTION__;
        }

            RNode* theNode1 = theNodeMatrix[i][0];
            RNode* theNode2 = theNodeMatrix[i+1][0];
            RNode* theNode3 = theNodeMatrix[i+1][numCol-1];
            RNode* theNode4 = theNodeMatrix[i][numCol-1];

            std::vector<RNode*> vectorOfNodes{theNode1,theNode2,theNode3,theNode4};

            //Get the normal vector for the face of the wall
            auto normalVec = theMathTool.computePolygonNormal<RNode*>(vectorOfNodes);

            auto vertex1 = theTopoMesh->addVertex(theNode1,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
            auto vertex2 = theTopoMesh->addVertex(theNode2,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
            auto vertex3 = theTopoMesh->addVertex(theNode2,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);
            auto vertex4 = theTopoMesh->addVertex(theNode1,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);

            auto edge1 = theTopoMesh->addEdge(vertex1,vertex2);
            auto edge2 = theTopoMesh->addEdge(vertex2,vertex3);
            auto edge3 = theTopoMesh->addEdge(vertex3,vertex4);
            auto edge4 = theTopoMesh->addEdge(vertex4,vertex1);

            RTopoFace face1(edge1,edge2,edge3,edge4);

            auto vertex5 = theTopoMesh->addVertex(theNode4,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
            auto vertex6 = theTopoMesh->addVertex(theNode3,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
            auto vertex7 = theTopoMesh->addVertex(theNode3,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);
            auto vertex8 = theTopoMesh->addVertex(theNode4,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);

            auto edge5 = theTopoMesh->addEdge(vertex5,vertex6);
            auto edge6 = theTopoMesh->addEdge(vertex6,vertex7);
            auto edge7 = theTopoMesh->addEdge(vertex7,vertex8);
            auto edge8 = theTopoMesh->addEdge(vertex8,vertex5);

            RTopoFace face2(edge5,edge6,edge7,edge8);

            face1.setIndex(5);
            face2.setIndex(2);

            face1.setTheDamageRegion(damageMesh);
            face2.setTheDamageRegion(damageMesh);

            theTopoMesh->addFace(face1);
            theTopoMesh->addFace(face2);

            counter+= numberOfElementsAlongV;
    }

    //Mesh out the short sides
    counter = 0;
    for(int i = 0; i<numCol-1; ++i){

        auto theElement = theElementList->at(counter);
        auto damageMesh = theElement->getTheDamageRegion();

        if(damageMesh == nullptr)
        {
            qDebug()<<"Error getting the damage mesh from element "<<theElement->objectName()<<" in "<<__FUNCTION__;
        }


        RNode* theNode1 = theNodeMatrix[0][i];
        RNode* theNode2 = theNodeMatrix[0][i+1];
        RNode* theNode3 = theNodeMatrix[numRow-1][i+1];
        RNode* theNode4 = theNodeMatrix[numRow-1][i];

        std::vector<RNode*> vectorOfNodes{theNode1,theNode2,theNode3,theNode4};

        //Get the normal vector for the face of the wall
        auto normalVec = theMathTool.computePolygonNormal<RNode*>(vectorOfNodes);

        auto vertex1 = theTopoMesh->addVertex(theNode1,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
        auto vertex2 = theTopoMesh->addVertex(theNode2,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
        auto vertex3 = theTopoMesh->addVertex(theNode2,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);
        auto vertex4 = theTopoMesh->addVertex(theNode1,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);

        auto edge1 = theTopoMesh->addEdge(vertex1,vertex2);
        auto edge2 = theTopoMesh->addEdge(vertex2,vertex3);
        auto edge3 = theTopoMesh->addEdge(vertex3,vertex4);
        auto edge4 = theTopoMesh->addEdge(vertex4,vertex1);

        RTopoFace face1(edge1,edge2,edge3,edge4);

        auto vertex5 = theTopoMesh->addVertex(theNode4,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
        auto vertex6 = theTopoMesh->addVertex(theNode3,normalVec[0]*dZ,normalVec[1]*dZ,normalVec[2]*dZ);
        auto vertex7 = theTopoMesh->addVertex(theNode3,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);
        auto vertex8 = theTopoMesh->addVertex(theNode4,normalVec[0]*dZ*-1.0,normalVec[1]*dZ*-1.0,normalVec[2]*dZ*-1.0);

        auto edge5 = theTopoMesh->addEdge(vertex5,vertex6);
        auto edge6 = theTopoMesh->addEdge(vertex6,vertex7);
        auto edge7 = theTopoMesh->addEdge(vertex7,vertex8);
        auto edge8 = theTopoMesh->addEdge(vertex8,vertex5);

        RTopoFace face2(edge5,edge6,edge7,edge8);

        face2.setIndex(0);
        face1.setIndex(3);

        face1.setTheDamageRegion(damageMesh);
        face2.setTheDamageRegion(damageMesh);

        theTopoMesh->addFace(face1);
        theTopoMesh->addFace(face2);

        counter+= numberOfElementsAlongU;

    }

    return 0;
}

