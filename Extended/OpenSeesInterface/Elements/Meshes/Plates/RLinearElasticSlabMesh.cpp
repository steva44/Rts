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

#include "RLinearElasticSlabMesh.h"
#include "RComponent.h"
#include "RMeshGenerator.h"
#include "RShellMITC4.h"
#include "RShellSectionDamageRegion.h"
#include "RElasticMembranePlateSection.h"
#include "SectionForceDeformation.h"
#include "RPoint.h"
#include "RTopoVertex.h"
#include "RTopoEdge.h"
#include "RTopoFace.h"

//The class creates a 2D FE mesh using plate and/or plane stress/strain elements from a four point wall geometry
//A wall is defined with 4 corner points and a thickness
//
//   P4 ____________ P3      v
//     | __|__|__|__ |   1  ^
//     | __|__|__|__ |   2  |
//     | __|__|__|__ |   3  |
//     | __|__|__|__ |   4  *----->u
//     | __|__|__|__ |   ⋮
//   P1|___|__|__|___P2  n
//        1  2 ... m
//
//

RLinearElasticSlabMesh::RLinearElasticSlabMesh(RDomain *domain,
                                               QString componentName,
                                               RMaterialKnowledge* material,
                                               RPoint *point1,
                                               RPoint *point2,
                                               RPoint *point3,
                                               RPoint *point4,
                                               RParameter* thickness,
                                               QPointer<RParameter> numElementsAlongU,
                                               QPointer<RParameter> numElementsAlongV,
                                               QList<RParameter*> &theParameterList,
                                               QList<RElement*> &elementList,
                                               int meshOption)
    : RSlabMesh(domain, componentName), theNumElementsAlongU(numElementsAlongU), theNumElementsAlongV(numElementsAlongV), theMaterial(material)
{
    //Set the members in the parent class
    theElementList = &elementList;
    theThickness = thickness;
    thePoint1 = point1;
    thePoint2 = point2;
    thePoint3 = point3;
    thePoint4 = point4;
    theMeshOption = meshOption;
}


RLinearElasticSlabMesh::~RLinearElasticSlabMesh()
{
}


int RLinearElasticSlabMesh::createMesh(void)
{
    // Do not create mesh until the end-points and other geometry parameters are set
    if (thePoint1 == nullptr || thePoint2 == nullptr ||  thePoint3 == nullptr||  thePoint4 == nullptr || theThickness == nullptr)
    {
        qCritical()<<"Null pointers for points came into "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    // Check to make sure thickness is valid
    if(theThickness->getCurrentValue() <= 0.0  )
    {
        qCritical()<<"The thickness cannot be less than or equal to zero in "<<this->objectName();
        return -1;
    }

    // Create the in the constructor
    RMeshGenerator theMeshGenerator(theDomain);
    QList<RElement*> theMeshedElementList;

    // Now create the mesh
    if (theMeshOption == 1)
    {
        theDomain->createObject(this->objectName()+"ElasticMembranePlateSection","RElasticMembranePlateSection");

        auto thePlateSection = qobject_cast<RElasticMembranePlateSection*>(theDomain->getLastAddedObject());

        if(thePlateSection == nullptr)
        {
            qCritical()<<"Error, was not able to create a plate section from the domain in"<<__FUNCTION__;
            return -1;
        }

        //Set some properties to the section
        thePlateSection->setTheRhoParameter(theMaterial->getDensity());
        thePlateSection->setTheEParameter(theMaterial->getYoungsModulus());
        thePlateSection->setTheNuParameter(theMaterial->getPoissonsRatio());

        thePlateSection->setTheThicknessParameter(theThickness);

        // Generate Mind4s
        theMeshedElementList.clear();
        theMeshGenerator.meshQuadrilateralArea(thePoint1, thePoint2, thePoint3, thePoint4, "RShellMITC4", theNumElementsAlongU, theNumElementsAlongV, theComponentName+"Mind4s", theMeshedElementList);
        if(theMeshedElementList.empty())
        {
            qCritical()<<"Error: An empty element list was returned from the mesh generator in "<<this->objectName();
            return -1;
        }

        for (auto&& it: theMeshedElementList)
        {
            RShellMITC4 *theElement = qobject_cast<RShellMITC4 *>(it);
            theElement->setTheSectionForceDeformation(thePlateSection);

            auto theOSElement = theElement->getTheOpenSeesElement();

            if(theOSElement)
                theDomain->getTheOpenSeesDomain()->addElement(theOSElement);

            *theElementList << theElement;
        }
    }
    else
    {
        qCritical() << "Invalid mesh option given to the component named" << this->objectName();
    }

    theComponentElementList = *theElementList;

    return 0;
}




double RLinearElasticSlabMesh::getMaxCompressiveStressXX()
{
    return maxCompressiveStressXX;
}


double RLinearElasticSlabMesh::getMaxTensileStressXX()
{
    return maxTensileStressXX;
}


double RLinearElasticSlabMesh::getMaxCompressiveStressYY()
{
    return maxCompressiveStressYY;
}


double RLinearElasticSlabMesh::getMaxTensileStressYY()
{
    return maxTensileStressYY;
}


double RLinearElasticSlabMesh::getMaxCompressiveStressZZ()
{
    return maxCompressiveStressZZ;
}


double RLinearElasticSlabMesh::getMaxTensileStressZZ()
{
    return maxTensileStressZZ;
}


double RLinearElasticSlabMesh::getMaxShearStressXY()
{
    auto fabsMinShear = fabs(minShearStressXY);

    if(fabsMinShear > maxShearStressXY)
        return fabsMinShear;
    else
        return maxShearStressXY;
}


double RLinearElasticSlabMesh::getMaxShearStressYZ()
{
    auto fabsMinShear = fabs(minShearStressYZ);

    if(fabsMinShear > maxShearStressYZ)
        return fabsMinShear;
    else
        return maxShearStressYZ;
}


double RLinearElasticSlabMesh::getMaxShearStressXZ()
{
    auto fabsMinShear = fabs(minShearStressXZ);

    if(fabsMinShear > maxShearStressXZ)
        return fabsMinShear;
    else
        return maxShearStressXZ;
}


int RLinearElasticSlabMesh::commitState()
{
    return 0;
}


double RLinearElasticSlabMesh::getMaxVerticalDeflection()
{
    auto maxDeflection = 0.0;

    for (auto&& it : theComponentElementList)
    {
        auto numNodes = it->getNumNodes();

        for(int i = 0; i<numNodes; ++i)
        {
            auto theNode = it->getNode(i);

            auto displacement = fabs(theNode->getZDisplacement());

            if(displacement > maxDeflection)
                maxDeflection = displacement;
        }
    }

    return maxDeflection;
}


//This function returns a node matrix in the same order that the nodes were created in RMeshGenerator
QVector< QVector<RNode *>> RLinearElasticSlabMesh::getNodeMatrix(void)
{
    auto numElemU = static_cast<int>(theNumElementsAlongU->getCurrentValue());
    auto numElemV = static_cast<int>(theNumElementsAlongV->getCurrentValue());

    QVector< QVector<RNode *> > theNodeMatrix(numElemU+1, QVector<RNode *>(numElemV+1));

    int counter = 0;
    for (int i=0; i<numElemU; ++i)
    {
        for (int j=0; j<numElemV; ++j)
        {
            auto theElement = theElementList->at(counter);

            theNodeMatrix[i][j]     = theElement->getNode(0);
            theNodeMatrix[i][j+1]   = theElement->getNode(1);
            theNodeMatrix[i+1][j+1] = theElement->getNode(2);
            theNodeMatrix[i+1][j]   = theElement->getNode(3);

            ++counter;
        }
    }

    return theNodeMatrix;
}


//This function creates a hexahedron out of 6 NURB faces that will be used to create the visualization topology of the component
int RLinearElasticSlabMesh::createTopologicalMesh(void)
{

    theTopoMesh = make_shared<RTopoMesh>(this);

    //Create the 8 points shared by the plate surfaces
    //    4-------3
    //    |   1   | v
    //    |       | ^
    //    1-------2 |->u
    //  5 |---2---| 6<- plate midplane
    //    5-------6
    //    |   3   |
    //    |       |
    //    8-------7
    //    |---4---|


    QVector< QVector<RNode *> > theNodeMatrix = this->getNodeMatrix();

    auto numRow = theNodeMatrix.size();
    auto numCol = theNodeMatrix[0].size();

    auto numElemU = static_cast<int>(theNumElementsAlongU->getCurrentValue());
    auto numElemV = static_cast<int>(theNumElementsAlongV->getCurrentValue());

    //Check to make sure the number of rows is equal to numberOfElementsAlongV and the number of columns is equal to numberOfElementsAlongU
    if(numRow-1 != numElemU || numCol-1 != numElemV )
    {
        qCritical()<<"Error in topo meshing, matrices are not consistent";
        return -1;
    }

    auto dZ = 0.5*getThickness();

    int counter = 0;

    //OpenGL quad vertices order are defined in a Counter Clockwise fashion
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

            auto vertex1 = theTopoMesh->addVertex(theNode1,0.0,0.0,dZ);
            auto vertex2 = theTopoMesh->addVertex(theNode2,0.0,0.0,dZ);
            auto vertex3 = theTopoMesh->addVertex(theNode3,0.0,0.0,dZ);
            auto vertex4 = theTopoMesh->addVertex(theNode4,0.0,0.0,dZ);

            auto edge1 = theTopoMesh->addEdge(vertex1,vertex2);
            auto edge2 = theTopoMesh->addEdge(vertex2,vertex3);
            auto edge3 = theTopoMesh->addEdge(vertex3,vertex4);
            auto edge4 = theTopoMesh->addEdge(vertex4,vertex1);

            RTopoFace face(edge1,edge2,edge3,edge4);

            auto vertex5 = theTopoMesh->addVertex(theNode1,0.0,0.0,-dZ);
            auto vertex6 = theTopoMesh->addVertex(theNode2,0.0,0.0,-dZ);
            auto vertex7 = theTopoMesh->addVertex(theNode3,0.0,0.0,-dZ);
            auto vertex8 = theTopoMesh->addVertex(theNode4,0.0,0.0,-dZ);

            auto edge5 = theTopoMesh->addEdge(vertex5,vertex6);
            auto edge6 = theTopoMesh->addEdge(vertex6,vertex7);
            auto edge7 = theTopoMesh->addEdge(vertex7,vertex8);
            auto edge8 = theTopoMesh->addEdge(vertex8,vertex5);

            RTopoFace face2(edge5,edge6,edge7,edge8);

            face.setIndex(2); //Top
            face2.setIndex(5); //Bot

            face.setTheDamageRegion(damageMesh);
            face2.setTheDamageRegion(damageMesh);

            theTopoMesh->addFace(face);
            theTopoMesh->addFace(face2);

            ++counter;
        }
    }

    // Mesh out the long sides
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
        RNode* theNode3 = theNodeMatrix[i][numCol-1];
        RNode* theNode4 = theNodeMatrix[i+1][numCol-1];


        auto vertex1 = theTopoMesh->addVertex(theNode1,0.0,0.0,dZ);
        auto vertex2 = theTopoMesh->addVertex(theNode1,0.0,0.0,-dZ);
        auto vertex3 = theTopoMesh->addVertex(theNode2,0.0,0.0,dZ);
        auto vertex4 = theTopoMesh->addVertex(theNode2,0.0,0.0,-dZ);

        auto edge1 = theTopoMesh->addEdge(vertex2,vertex4);
        auto edge2 = theTopoMesh->addEdge(vertex4,vertex3);
        auto edge3 = theTopoMesh->addEdge(vertex3,vertex1);
        auto edge4 = theTopoMesh->addEdge(vertex1,vertex2);

        RTopoFace face1(edge1,edge2,edge3,edge4);

        auto vertex5 = theTopoMesh->addVertex(theNode3,0.0,0.0,dZ);
        auto vertex6 = theTopoMesh->addVertex(theNode3,0.0,0.0,-dZ);
        auto vertex7 = theTopoMesh->addVertex(theNode4,0.0,0.0,dZ);
        auto vertex8 = theTopoMesh->addVertex(theNode4,0.0,0.0,-dZ);

        auto edge5 = theTopoMesh->addEdge(vertex6,vertex8);
        auto edge6 = theTopoMesh->addEdge(vertex8,vertex7);
        auto edge7 = theTopoMesh->addEdge(vertex7,vertex5);
        auto edge8 = theTopoMesh->addEdge(vertex5,vertex6);

        RTopoFace face2(edge5,edge6,edge7,edge8);

        face1.setIndex(4); //Front
        face2.setIndex(1); //Back

        face1.setTheDamageRegion(damageMesh);
        face2.setTheDamageRegion(damageMesh);

        theTopoMesh->addFace(face1);
        theTopoMesh->addFace(face2);

        counter+= numElemV;
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
        RNode* theNode3 = theNodeMatrix[numRow-1][i];
        RNode* theNode4 = theNodeMatrix[numRow-1][i+1];

        auto vertex1 = theTopoMesh->addVertex(theNode1,0.0,0.0,dZ);
        auto vertex2 = theTopoMesh->addVertex(theNode1,0.0,0.0,-dZ);
        auto vertex3 = theTopoMesh->addVertex(theNode2,0.0,0.0,dZ);
        auto vertex4 = theTopoMesh->addVertex(theNode2,0.0,0.0,-dZ);

        auto edge1 = theTopoMesh->addEdge(vertex2,vertex4);
        auto edge2 = theTopoMesh->addEdge(vertex4,vertex3);
        auto edge3 = theTopoMesh->addEdge(vertex3,vertex1);
        auto edge4 = theTopoMesh->addEdge(vertex1,vertex2);

        RTopoFace face1(edge1,edge2,edge3,edge4);

        auto vertex5 = theTopoMesh->addVertex(theNode3,0.0,0.0,dZ);
        auto vertex6 = theTopoMesh->addVertex(theNode3,0.0,0.0,-dZ);
        auto vertex7 = theTopoMesh->addVertex(theNode4,0.0,0.0,dZ);
        auto vertex8 = theTopoMesh->addVertex(theNode4,0.0,0.0,-dZ);

        auto edge5 = theTopoMesh->addEdge(vertex6,vertex8);
        auto edge6 = theTopoMesh->addEdge(vertex8,vertex7);
        auto edge7 = theTopoMesh->addEdge(vertex7,vertex5);
        auto edge8 = theTopoMesh->addEdge(vertex5,vertex6);

        RTopoFace face2(edge5,edge6,edge7,edge8);

        face1.setIndex(3); //Left
        face2.setIndex(0); //Right

        face1.setTheDamageRegion(damageMesh);
        face2.setTheDamageRegion(damageMesh);

        theTopoMesh->addFace(face1);
        theTopoMesh->addFace(face2);

        counter+= numElemU;
    }

    return 0;
}


double RLinearElasticSlabMesh::getLength()
{
    //Length
    auto x2 = thePoint2->getXCoordinateValue();
    auto y2 = thePoint2->getYCoordinateValue();
    auto z2 = thePoint2->getZCoordinateValue();

    auto x1 = thePoint1->getXCoordinateValue();
    auto y1 = thePoint1->getYCoordinateValue();
    auto z1 = thePoint1->getZCoordinateValue();

    auto dx = x2-x1;
    auto dy = y2-y1;
    auto dz = z2-z1;

    auto Length = std::sqrt(dx*dx+dy*dy+dz*dz);

    return Length;
}


double RLinearElasticSlabMesh::getWidth()
{

    //Width
    auto x2 = thePoint2->getXCoordinateValue();
    auto y2 = thePoint2->getYCoordinateValue();
    auto z2 = thePoint2->getZCoordinateValue();

    auto x3 = thePoint3->getXCoordinateValue();
    auto y3 = thePoint3->getYCoordinateValue();
    auto z3 = thePoint3->getZCoordinateValue();

    auto dxw = x3-x2;
    auto dyw = y3-y2;
    auto dzw = z3-z2;

    auto width = std::sqrt(dxw*dxw+dyw*dyw+dzw*dzw);
    return width;
}


double RLinearElasticSlabMesh::getThickness()
{
    return theThickness->getCurrentValue();
}

