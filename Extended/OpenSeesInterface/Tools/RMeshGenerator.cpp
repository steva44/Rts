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

#include "RMeshGenerator.h"
#include "RNode.h"
#include "RPoint.h"
#include "RElement.h"
#include "RAlgebraicExpressionModel.h"
#include "RParameter.h"
#include "RConstant.h"
#include "RDecisionVariable.h"
#include "RContinuousDecisionVariable.h"
#include "RContinuousRandomVariable.h"
#include "RMathUtils.h"
#include "RMaterialKnowledge.h"
#include "R2DPoint.h"
#include "R3DPoint.h"

#include <math.h>

RMeshGenerator::RMeshGenerator(RDomain *domain)
    : RMethod(domain, nullptr)
{
    nodalDebug = false;

    tol = 1.0e-6;

}


RMeshGenerator::~RMeshGenerator()
{

}


int RMeshGenerator::meshQuadrilateralArea(RPoint *point1,
                                          RPoint *point2,
                                          RPoint *point3,
                                          RPoint *point4,
                                          QString elementType,
                                          RParameter* numElementsAlong12,
                                          RParameter* numElementsAlong23,
                                          QString creatorName,
                                          QList<RElement *> &elementList)
{
    // Create some tools
    RMathUtils mathTools;

    int ni = static_cast<int>(numElementsAlong12->getCurrentValue());
    int nj = static_cast<int>(numElementsAlong23->getCurrentValue());

    QVector< QVector<RNode *> > nodeMatrix(ni+1, QVector<RNode *>(nj+1));

    // Check the number of elements coming in
    if(ni <= 0 || nj <= 0)
    {
        qCritical()<<"The number of elements along each side of the Quadrilateral in RMeshGenerator must be greater than or equal to zero";
        return -1;
    }

    auto distance12 = mathTools.computeLengthWithPoints(point1,point2);
    auto distance23 = mathTools.computeLengthWithPoints(point2,point3);
    auto distance14 = mathTools.computeLengthWithPoints(point1,point4);
    auto distance34 = mathTools.computeLengthWithPoints(point3,point4);

    if (distance12<=0.0 || distance23<=0.0 || distance14<=0.0 || distance34<=0.0 ) {
        qCritical() << "Coinciding points were given to the mesh generator in the component called" << creatorName;
        return -1;
    }

    QString nodeName = "";
    QString elementName = "";
    RNode *theNode = nullptr;
    RElement *theElement = nullptr;
    bool foundNode = false;

    // qDebug()<<"The number of elements along 12"<<numElementsAlong12;
    // qDebug()<<"The number of elements along 23"<<numElementsAlong23;
    // qDebug()<<"Distance 12"<<distance12;
    // qDebug()<<"Distance 23"<<distance23;

    //***** LAY OUT THE NODES AND ASSIGN COORDINATES *****
    /*
     *
     * FOR DDM HAVE THE DERIVATIVE OF THESE PARAMETRIC FUNCTIONS
     * WRT TO EACH POINT; WITHIN EACH POINT THEN HAVE DERIVATIVE
     * OF THAT POINT WRT A PARAMETER
     * CHAIN RULE IS THEN USED TO FIND DERIVATIVE OF COORDINATE
     * WRT PARAMETER
     *
     */

    for (int i=0; i<=ni; ++i) {
        for (int j=0;j<=nj; ++j) {

            // Node name
            nodeName = QString("Node_%1_%2").arg(i).arg(j);

            // X COORDINATE
            std::function<double(void)> xCoordinatelambda = [ni, nj, i, j, point1, point2, point3, point4]
            {
                auto divisor = static_cast<double>(ni*nj);

                auto n1X = point1->getXCoordinateValue();
                auto n2X = point2->getXCoordinateValue();
                auto n3X = point3->getXCoordinateValue();
                auto n4X = point4->getXCoordinateValue();

                return static_cast<double>(i*j*n1X-i*j*n2X+i*j*n3X-i*j*n4X-i*n1X*nj+i*n2X*nj-j*n1X*ni+j*n4X*ni+n1X*ni*nj)/(divisor);
            };

            // Y COORDINATE
            std::function<double(void)> yCoordinatelambda = [ni, nj, i, j, point1, point2, point3, point4]
            {
                auto divisor = static_cast<double>(ni*nj);

                auto n1Y = point1->getYCoordinateValue();
                auto n2Y = point2->getYCoordinateValue();
                auto n3Y = point3->getYCoordinateValue();
                auto n4Y = point4->getYCoordinateValue();

                return static_cast<double>(i*j*n1Y-i*j*n2Y+i*j*n3Y-i*j*n4Y-i*n1Y*nj+i*n2Y*nj-j*n1Y*ni+j*n4Y*ni+n1Y*ni*nj)/(divisor);
            };

            // Z COORDINATE
            std::function<double(void)> zCoordinatelambda = [ni, nj, i, j, point1, point2, point3, point4]
            {
                auto divisor = static_cast<double>(ni*nj);

                auto n1Z = point1->getZCoordinateValue();
                auto n2Z = point2->getZCoordinateValue();
                auto n3Z = point3->getZCoordinateValue();
                auto n4Z = point4->getZCoordinateValue();

                return static_cast<double>(i*j*n1Z-i*j*n2Z+i*j*n3Z-i*j*n4Z-i*n1Z*nj+i*n2Z*nj-j*n1Z*ni+j*n4Z*ni+n1Z*ni*nj)/(divisor);
            };

            auto x = xCoordinatelambda();
            auto y = yCoordinatelambda();
            auto z = zCoordinatelambda();

            // Now the lambdas for the gradients

            // X COORDINATE GRADIENT
            std::function<double(RParameter*)> nodalXGradientLambda = [i,j,point1, point2, point3, point4,numElementsAlong12,numElementsAlong23](RParameter *theParameter)
            {
                auto gradient = 0.0;

                auto ni = numElementsAlong12->getCurrentValue();
                auto nj = numElementsAlong23->getCurrentValue();

                if(theParameter == numElementsAlong12) //ni
                {
                    auto n1X = point1->getXCoordinateValue();
                    auto n2X = point2->getXCoordinateValue();
                    auto n3X = point3->getXCoordinateValue();
                    auto n4X = point4->getXCoordinateValue();

                    gradient = -static_cast<double>((n1X - n2X + n3X - n4X) * j - nj * (n1X - n2X)) * i / nj * (1.0/(ni*ni));
                }
                else if(theParameter == numElementsAlong23) //nj
                {
                    auto n1X = point1->getXCoordinateValue();
                    auto n2X = point2->getXCoordinateValue();
                    auto n3X = point3->getXCoordinateValue();
                    auto n4X = point4->getXCoordinateValue();

                    gradient = -static_cast<double>(j * (i * (n1X - n2X + n3X - n4X) - ni * (-n4X + n1X)) * (1.0/(nj*nj)) / ni);
                }
                else{

                    // Calculate Gradients
                    auto dN1dh = static_cast<double>((i*j-i*nj-j*ni+ni*nj)/(ni*nj));
                    auto dN2dh = static_cast<double>((-i*j+i*nj)/(ni*nj));
                    auto dN3dh = static_cast<double>((i*j)/(ni*nj));
                    auto dN4dh = static_cast<double>((-i*j+j*ni)/(ni*nj));

                    gradient = dN1dh*point1->getXCoordinateFOGradient(theParameter)+
                            dN2dh*point2->getXCoordinateFOGradient(theParameter)+
                            dN3dh*point3->getXCoordinateFOGradient(theParameter)+
                            dN4dh*point4->getXCoordinateFOGradient(theParameter);
                }

                // qDebug()<<"For parameter"<<theParameter->objectName()<<"the X-Coordinate gradient :"<<gradient<<"\n";  //More gradient debug info uncomment in RNode

                return gradient;
            };

            // Y COORDINATE GRADIENT
            std::function<double(RParameter*)> nodalYGradientLambda = [i,j,point1, point2, point3, point4,numElementsAlong12,numElementsAlong23](RParameter *theParameter)
            {
                auto gradient = 0.0;
                auto ni = numElementsAlong12->getCurrentValue();
                auto nj = numElementsAlong23->getCurrentValue();

                if(theParameter == numElementsAlong12) //ni
                {
                    auto n1Y = point1->getYCoordinateValue();
                    auto n2Y = point2->getYCoordinateValue();
                    auto n3Y = point3->getYCoordinateValue();
                    auto n4Y = point4->getYCoordinateValue();

                    gradient = -static_cast<double>((n1Y - n2Y + n3Y - n4Y) * j - nj * (n1Y - n2Y)) * i / nj * (1/(ni*ni));
                }
                else if(theParameter == numElementsAlong23) //nj
                {
                    auto n1Y = point1->getYCoordinateValue();
                    auto n2Y = point2->getYCoordinateValue();
                    auto n3Y = point3->getYCoordinateValue();
                    auto n4Y = point4->getYCoordinateValue();

                    gradient = -static_cast<double>(j * (i * (n1Y - n2Y + n3Y - n4Y) - ni * (-n4Y + n1Y)) * (1/(nj*nj)) / ni);
                }
                else{

                    // Calculate Gradients
                    auto dN1dh = static_cast<double>((i*j-i*nj-j*ni+ni*nj)/(ni*nj));
                    auto dN2dh = static_cast<double>((-i*j+i*nj)/(ni*nj));
                    auto dN3dh = static_cast<double>((i*j)/(ni*nj));
                    auto dN4dh = static_cast<double>((-i*j+j*ni)/(ni*nj));

                    gradient = dN1dh*point1->getYCoordinateFOGradient(theParameter)+
                            dN2dh*point2->getYCoordinateFOGradient(theParameter)+
                            dN3dh*point3->getYCoordinateFOGradient(theParameter)+
                            dN4dh*point4->getYCoordinateFOGradient(theParameter);
                }

                // qDebug()<<"For parameter"<<theParameter->objectName()<<"the Y-Coordinate gradient :"<<gradient<<"\n";   //More gradient debug info uncomment in RNode

                return gradient;
            };

            // Z COORDINATE GRADIENT
            std::function<double(RParameter*)> nodalZGradientLambda = [i,j,point1, point2, point3, point4,numElementsAlong12,numElementsAlong23](RParameter *theParameter)
            {

                auto gradient = 0.0;
                auto ni = numElementsAlong12->getCurrentValue();
                auto nj = numElementsAlong23->getCurrentValue();

                if(theParameter == numElementsAlong12) // ni
                {
                    auto n1Z = point1->getZCoordinateValue();
                    auto n2Z = point2->getZCoordinateValue();
                    auto n3Z = point3->getZCoordinateValue();
                    auto n4Z = point4->getZCoordinateValue();

                    gradient = -static_cast<double>((n1Z - n2Z + n3Z - n4Z) * j - nj * (n1Z - n2Z)) * i / nj * (1/(ni*ni));
                }
                else if(theParameter == numElementsAlong23) // nj
                {
                    auto n1Z = point1->getZCoordinateValue();
                    auto n2Z = point2->getZCoordinateValue();
                    auto n3Z = point3->getZCoordinateValue();
                    auto n4Z = point4->getZCoordinateValue();

                    gradient = -static_cast<double>(j * (i * (n1Z - n2Z + n3Z - n4Z) - ni * (-n4Z + n1Z)) * (1/(nj*nj)) / ni);
                }
                else
                {
                    // Calculate Gradients
                    auto dN1dh = static_cast<double>((i*j-i*nj-j*ni+ni*nj)/(ni*nj));
                    auto dN2dh = static_cast<double>((-i*j+i*nj)/(ni*nj));
                    auto dN3dh = static_cast<double>((i*j)/(ni*nj));
                    auto dN4dh = static_cast<double>((-i*j+j*ni)/(ni*nj));

                    gradient = dN1dh*point1->getZCoordinateFOGradient(theParameter)+
                            dN2dh*point2->getZCoordinateFOGradient(theParameter)+
                            dN3dh*point3->getZCoordinateFOGradient(theParameter)+
                            dN4dh*point4->getZCoordinateFOGradient(theParameter);
                }

                // qDebug()<<"For parameter"<<theParameter->objectName()<<"the Z-Coordinate gradient :"<<gradient<<"\n";  //More gradient debug info uncomment in RNode

                return gradient;
            };

            // When meshing multiple components, need to check for existing nodes
            QList<RNode *> existingNodes = theDomain->findChildren<RNode *>();

            // Check if a node already exists at this location; then use it
            foundNode = false;
            for (auto&& it : existingNodes) {

                if (fabs(it->getXCoordinateValue() - x)<tol && fabs(it->getYCoordinateValue() - y)<tol && fabs(it->getZCoordinateValue() - z)<tol)
                {
                    it->setFOGradientXLambda(nodalXGradientLambda);
                    it->setFOGradientYLambda(nodalYGradientLambda);
                    it->setFOGradientZLambda(nodalZGradientLambda);
                    it->setObjectName(it->objectName()+creatorName);

                    nodeMatrix[i][j] = it;

                    foundNode = true;

                    break;
                }
            }

            // Create a node if none was found at this location
            if (!foundNode) {

                // Create the node and set its coordinates
                theDomain->createObject(creatorName + nodeName, "RNode");
                theNode = qobject_cast<RNode *>(theDomain->getLastAddedObject());

                if(theNode == nullptr)
                {
                    qCritical()<<"Error in the mesh generator a null ptr came out of RDomain when creating a node";
                    return -1;
                }

                // Set the functions to update
                theNode->setXCoordinate(xCoordinatelambda);
                theNode->setYCoordinate(yCoordinatelambda);
                theNode->setZCoordinate(zCoordinatelambda);

                theNode->setFOGradientXLambda(nodalXGradientLambda);
                theNode->setFOGradientYLambda(nodalYGradientLambda);
                theNode->setFOGradientZLambda(nodalZGradientLambda);

                nodeMatrix[i][j] = theNode;
            }
        }

    }


    // **** LAY OUT THE ELEMENTS AND ASSIGN NODES FROM ABOVE *****
    for (int i=0; i<ni; ++i)
    {
        for (int j=0; j<nj; ++j)
        {
            // Element name
            elementName = QString("Element_%1_%2").arg(i).arg(j);

            // Create elements and stack them in a list, so the component can set properties
            theDomain->createObject(creatorName + elementName, elementType);
            theElement = qobject_cast<RElement *>(theDomain->getLastAddedObject());

            theElement->setNode(0, nodeMatrix[i][j]);
            theElement->setNode(1, nodeMatrix[i][j+1]);
            theElement->setNode(2, nodeMatrix[i+1][j+1]);
            theElement->setNode(3, nodeMatrix[i+1][j]);
            elementList << theElement;
        }
    }


    return 0;
}


int RMeshGenerator::meshHexhedron(RPoint *point1,
                                  RPoint *point2,
                                  RPoint *point3,
                                  RPoint *point4,
                                  RPoint *point5,
                                  RPoint *point6,
                                  RPoint *point7,
                                  RPoint *point8,
                                  QString elementType,
                                  int numElementsAlong12,
                                  int numElementsAlong23,
                                  int numElementsAlong81,
                                  QString creatorName,
                                  QList<RElement *> &elementList)
{
    //Get some tools
    RMathUtils mathTools;

    // EXTRACT DISTANCES BETWEEN POINTS
    double distance12 = mathTools.computeLengthWithPoints(point1,point2);
    double distance23 = mathTools.computeLengthWithPoints(point2,point3);
    double distance14 = mathTools.computeLengthWithPoints(point1,point4);
    double distance34 = mathTools.computeLengthWithPoints(point3,point4);
    double distance56 = mathTools.computeLengthWithPoints(point5,point6);
    double distance67 = mathTools.computeLengthWithPoints(point6,point7);
    double distance58 = mathTools.computeLengthWithPoints(point5,point8);
    double distance78 = mathTools.computeLengthWithPoints(point7,point8);
    double distance15 = mathTools.computeLengthWithPoints(point1,point5);
    double distance26 = mathTools.computeLengthWithPoints(point2,point6);
    double distance37 = mathTools.computeLengthWithPoints(point3,point7);
    double distance48 = mathTools.computeLengthWithPoints(point4,point8);

    auto minLength = 0.001;

    if (distance12==minLength || distance23==minLength || distance14==minLength || distance34==minLength  ||
            distance56< minLength || distance67< minLength || distance58< minLength || distance78< minLength
            || distance15 < minLength || distance26 < minLength || distance37 < minLength || distance48 < minLength) {
        qCritical() << "Coinciding points were given to the mesh generator in the component called" << creatorName;
        return -1;
    }

    // INITIAL DECLARATIONS BEFORE LOOP
    double x =0.0, y=0.0, z=0.0;

    QString nodeName = "";
    QString elementName = "";
    RNode *theNode = nullptr;
    RElement *theElement = nullptr;
    bool foundNode = false;
    QList<QVector< QVector<RNode *> >> thekMatrixList;

    QMap<RParameter *, double> theXCoordinateGradientMap;
    QMap<RParameter *, double> theYCoordinateGradientMap;
    QMap<RParameter *, double> theZCoordinateGradientMap;


    int nj = numElementsAlong23;
    int ni = numElementsAlong12;
    int nk = numElementsAlong81;

    // qDebug()<<"The number of elements along 12"<<numElementsAlong12;
    // qDebug()<<"The number of elements along 23"<<numElementsAlong23;
    // qDebug()<<"Distance 12"<<distance12;
    // qDebug()<<"Distance 23"<<distance23;

    double dNode1dh = 0.0;
    double dNode2dh = 0.0;
    double dNode3dh = 0.0;
    double dNode4dh = 0.0;
    double dNode5dh = 0.0;
    double dNode6dh = 0.0;
    double dNode7dh = 0.0;
    double dNode8dh = 0.0;

    // **** LAY OUT THE NODES AND ASSIGN COORDINATES *****

    /*
     *
     * FOR DDM NEED TO FIND THE DERIVATIVE OF THESE FUNCTIONS
     * WRT TO THE INPUT PARAMETERS
     *
     *
     */

    for (int k=0; k<=nk; ++k){

        // Create the temp node list for k at this layer
        QVector< QVector<RNode *> > tempNodeList(numElementsAlong12+1, QVector<RNode *>(numElementsAlong23+1));

        for (int i=0; i<=ni; ++i)
        {
            for (int j=0;j<=nj; ++j)
            {

                // Node name
                nodeName = QString("Node_%1_%2_%3").arg(i).arg(j).arg(k);

                theXCoordinateGradientMap.clear();
                theYCoordinateGradientMap.clear();
                theZCoordinateGradientMap.clear();

                // Calculate the needed quantities
                double ti = static_cast<double>(i/ni);
                double tj = static_cast<double>(j/nj);
                double tk = static_cast<double>(k/nk);

                // Calculate gradients
                dNode1dh = static_cast<double>(-ti * tj * tk + ti * tj + ti * tk + tj * tk - ti - tj - tk + 1);
                dNode2dh = static_cast<double>(ti * tj * tk - ti * tj - tj * tk + tj);
                dNode3dh = static_cast<double>(-ti * tj * tk + ti * tj);
                dNode4dh = static_cast<double>(ti * tj * tk - ti * tj - ti * tk + ti);
                dNode5dh = static_cast<double>(ti * tj * tk - ti * tk - tj * tk + tk);
                dNode6dh = static_cast<double>(-ti * tj * tk + tj * tk);
                dNode7dh = static_cast<double>(ti * tj * tk);
                dNode8dh = static_cast<double>(-ti * tj * tk + ti * tk);


                // X COORDINATE
                std::function<double(void)> xCoordinatelambda = [ti,tj,tk, point1, point2, point3, point4,
                        point5, point6, point7, point8]
                {

                    auto n1X = point1->getXCoordinateValue();
                    auto n2X = point2->getXCoordinateValue();
                    auto n3X = point3->getXCoordinateValue();
                    auto n4X = point4->getXCoordinateValue();
                    auto n5X = point5->getXCoordinateValue();
                    auto n6X = point6->getXCoordinateValue();
                    auto n7X = point7->getXCoordinateValue();
                    auto n8X = point8->getXCoordinateValue();

                    return static_cast<double>(-n1X * ti * tj * tk + n2X * ti * tj * tk - n3X * ti * tj * tk + n4X * ti * tj * tk + n5X * ti * tj * tk - n6X * ti * tj * tk + n7X * ti * tj * tk - n8X * ti * tj * tk + n1X * ti * tj + n1X * ti * tk + n1X * tj * tk - n2X * ti * tj - n2X * tj * tk + n3X * ti * tj - n4X * ti * tj - n4X * ti * tk - n5X * ti * tk - n5X * tj * tk + n6X * tj * tk + n8X * ti * tk - ti * n1X - n1X * tj - n1X * tk + n2X * tj + ti * n4X + n5X * tk + n1X);
                };

                // Y COORDINATE
                std::function<double(void)> yCoordinatelambda = [ti,tj,tk, point1, point2, point3, point4,
                        point5, point6, point7, point8]
                {

                    auto n1Y = point1->getYCoordinateValue();
                    auto n2Y = point2->getYCoordinateValue();
                    auto n3Y = point3->getYCoordinateValue();
                    auto n4Y = point4->getYCoordinateValue();
                    auto n5Y = point5->getYCoordinateValue();
                    auto n6Y = point6->getYCoordinateValue();
                    auto n7Y = point7->getYCoordinateValue();
                    auto n8Y = point8->getYCoordinateValue();

                    return static_cast<double>(-n1Y * ti * tj * tk + n2Y * ti * tj * tk - n3Y * ti * tj * tk + n4Y * ti * tj * tk + n5Y * ti * tj * tk - n6Y * ti * tj * tk + n7Y * ti * tj * tk - n8Y * ti * tj * tk + n1Y * ti * tj + n1Y * ti * tk + n1Y * tj * tk - n2Y * ti * tj - n2Y * tj * tk + n3Y * ti * tj - n4Y * ti * tj - n4Y * ti * tk - n5Y * ti * tk - n5Y * tj * tk + n6Y * tj * tk + n8Y * ti * tk - ti * n1Y - n1Y * tj - n1Y * tk + n2Y * tj + ti * n4Y + n5Y * tk + n1Y);
                };


                // Z COORDINATE
                std::function<double(void)> zCoordinatelambda = [ti,tj,tk, point1, point2, point3, point4,
                        point5, point6, point7, point8]
                {

                    auto n1Z = point1->getZCoordinateValue();
                    auto n2Z = point2->getZCoordinateValue();
                    auto n3Z = point3->getZCoordinateValue();
                    auto n4Z = point4->getZCoordinateValue();
                    auto n5Z = point5->getZCoordinateValue();
                    auto n6Z = point6->getZCoordinateValue();
                    auto n7Z = point7->getZCoordinateValue();
                    auto n8Z = point8->getZCoordinateValue();

                    return static_cast<double>(-n1Z * ti * tj * tk + n2Z * ti * tj * tk - n3Z * ti * tj * tk + n4Z * ti * tj * tk + n5Z * ti * tj * tk - n6Z * ti * tj * tk + n7Z * ti * tj * tk - n8Z * ti * tj * tk + n1Z * ti * tj + n1Z * ti * tk + n1Z * tj * tk - n2Z * ti * tj - n2Z * tj * tk + n3Z * ti * tj - n4Z * ti * tj - n4Z * ti * tk - n5Z * ti * tk - n5Z * tj * tk + n6Z * tj * tk + n8Z * ti * tk - ti * n1Z - n1Z * tj - n1Z * tk + n2Z * tj + ti * n4Z + n5Z * tk + n1Z);
                };

                x = xCoordinatelambda();
                y = yCoordinatelambda();
                z = zCoordinatelambda();

                // When meshing multiple components, need to check for existing nodes
                QList<RNode *> existingNodes = theDomain->findChildren<RNode *>();

                // Check if a node already exists at this location; then use it
                foundNode = false;
                for (int k = 0; k<existingNodes.size(); k++) {

                    auto node = existingNodes[k];

                    if (fabs(node->getXCoordinateValue() - x)<tol && fabs(node->getYCoordinateValue() - y)<tol && fabs(node->getZCoordinateValue() - z)<tol)
                    {
                        // existingNodes[k]->setXGradientMap(theXCoordinateGradientMap);
                        // existingNodes[k]->setYGradientMap(theYCoordinateGradientMap);
                        // existingNodes[k]->setZGradientMap(theZCoordinateGradientMap);
                        // existingNodes[k]->setObjectName(existingNodes[k]->objectName()+creatorName);

                        tempNodeList[i][j] = node;

                        foundNode = true;

                        break;
                    }
                }


                // Create a node if none was found at this location
                if (!foundNode) {

                    // Create the node and set its coordinates
                    theDomain->createObject(creatorName + nodeName, "RNode");
                    theNode = qobject_cast<RNode *>(theDomain->getLastAddedObject());
                    theNode->setXCoordinate(xCoordinatelambda);
                    theNode->setYCoordinate(yCoordinatelambda);
                    theNode->setZCoordinate(zCoordinatelambda);

                    // theNode->setXGradientMap(theXCoordinateGradientMap);
                    // theNode->setYGradientMap(theYCoordinateGradientMap);
                    // theNode->setZGradientMap(theZCoordinateGradientMap);

                    tempNodeList[i][j] = theNode;
                }
            }
        }

        thekMatrixList.append(tempNodeList);
    }



    // **** LAY OUT THE ELEMENTS AND ASSIGN NODES FROM ABOVE *****
    for (int k=0; k<nk; ++k)
    {
        for (int i=0; i<ni; ++i)
        {
            for (int j=0; j<nj; ++j)
            {
                QVector< QVector<RNode *> > tempNodeList = thekMatrixList.at(k);
                QVector< QVector<RNode *> > tempNodeList2 = thekMatrixList.at(k+1);

                // Element name
                elementName = QString("Element_%1_%2_%3").arg(i).arg(j).arg(k);

                // Create elements and stack them in a list, so the component can set properties
                theDomain->createObject(creatorName + elementName, elementType);
                theElement = qobject_cast<RElement *>(theDomain->getLastAddedObject());

                theElement->setNode(0, tempNodeList[i][j]);
                theElement->setNode(1, tempNodeList[i][j+1]);
                theElement->setNode(2, tempNodeList[i+1][j+1]);
                theElement->setNode(3, tempNodeList[i+1][j]);
                theElement->setNode(4, tempNodeList2[i][j]);
                theElement->setNode(5, tempNodeList2[i][j+1]);
                theElement->setNode(6, tempNodeList2[i+1][j+1]);
                theElement->setNode(7, tempNodeList2[i+1][j]);

                elementList << theElement;
            }
        }
    }

    return 0;
}


bool RMeshGenerator::meshLinearElement( RPoint *point1,
                                        RPoint *point2,
                                        QString elementType,
                                        int numberOfElements,
                                        QString creatorName,
                                        QList<RElement *> &elementList)
{

    // Create some tools
    RMathUtils mathTools;

    // Calculate distances between points
    double length = mathTools.computeLengthWithPoints(point1,point2);

    if (length<=0.0 ) {
        qCritical()<<"Coinciding points were given to the mesh generator in " <<__FUNCTION__<<" for "<<creatorName;
        return -1;
    }

    if(numberOfElements <= 0)
    {
        qCritical()<<"The number of elements has to be greater than zero in "<<__FUNCTION__<<" for "<<creatorName;
        return -1;
    }

    QVector<RNode *> theNodeVector(numberOfElements+1);

    QString nodeName = "";
    QString elementName = "";
    RElement *theElement = 0;
    RNode *theNode = 0;
    bool foundNode = false;
    int ni = numberOfElements;

    double dN1dh = 0.0;
    double dN2dh = 0.0;

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    for (int i=0; i<=ni; i++) {

        // Node name
        nodeName = QString("Node_%1").arg(i);

        // X COORDINATE
        std::function<double(void)> xCoordinatelambda = [ni, i, point1, point2]
        {
            double n1X = point1->getXCoordinateValue();
            double n2X = point2->getXCoordinateValue();

            double ini = static_cast<double>(i)/static_cast<double>(ni);

            return (n1X+ini*(n2X-n1X));
        };


        // Y COORDINATE
        std::function<double(void)> yCoordinatelambda = [ni, i, point1, point2]
        {
            double n1Y = point1->getYCoordinateValue();
            double n2Y = point2->getYCoordinateValue();

            double ini = static_cast<double>(i)/static_cast<double>(ni);

            return (n1Y+ini*(n2Y-n1Y));
        };


        // Z COORDINATE
        std::function<double(void)> zCoordinatelambda = [ni, i, point1, point2]
        {
            double n1Z = point1->getZCoordinateValue();
            double n2Z = point2->getZCoordinateValue();

            double ini = static_cast<double>(i)/static_cast<double>(ni);

            return (n1Z+ini*(n2Z-n1Z));
        };

        x = xCoordinatelambda();
        y = yCoordinatelambda();
        z = zCoordinatelambda();

        // qDebug()<<"For node "<<nodeName<<" x coord is "<<x;
        // qDebug()<<"For node "<<nodeName<<" y coord is "<<y;
        // qDebug()<<"For node "<<nodeName<<" z coord is "<<z;

        //Now the lambdas for the gradients
        // X COORDINATE GRADIENT

        //Calculate Gradient
        dN1dh = 1.0-i/ni;
        dN2dh = i/ni;

        // X COORDINATE GRADIENT
        std::function<double(RParameter*)> nodalXGradientLambda = [dN1dh, dN2dh, point1, point2](RParameter *theParameter)
        {
            double gradient = dN1dh*point1->getXCoordinateFOGradient(theParameter)+
                    dN2dh*point2->getXCoordinateFOGradient(theParameter);

            // qDebug()<<"For parameter"<<theParameter->objectName()<<"the X-Coordinate gradient :"<<gradient<<"\n";   //More gradient debug info uncomment in RNode


            return gradient;
        };

        // Y COORDINATE GRADIENT
        std::function<double(RParameter*)> nodalYGradientLambda = [dN1dh, dN2dh, point1, point2](RParameter *theParameter)
        {
            double gradient = dN1dh*point1->getYCoordinateFOGradient(theParameter)+
                    dN2dh*point2->getYCoordinateFOGradient(theParameter);

            // qDebug()<<"For parameter"<<theParameter->objectName()<<"the Y-Coordinate gradient :"<<gradient<<"\n";   //More gradient debug info uncomment in RNode

            return gradient;
        };

        // Z COORDINATE GRADIENT
        std::function<double(RParameter*)> nodalZGradientLambda = [dN1dh, dN2dh, point1, point2](RParameter *theParameter)
        {
            double gradient = dN1dh*point1->getZCoordinateFOGradient(theParameter)+
                    dN2dh*point2->getZCoordinateFOGradient(theParameter);

            // qDebug()<<"For parameter"<<theParameter->objectName()<<"the Z-Coordinate gradient :"<<gradient<<"\n";   //More gradient debug info uncomment in RNode

            return gradient;
        };

        // When meshing multiple components, need to check for existing nodes
        QList<RNode *> existingNodes = theDomain->findChildren<RNode *>();

        // Check if a node already exists at this location; then use it
        foundNode = false;
        for (int k = 0; k<existingNodes.size(); ++k) {

            if (fabs(existingNodes[k]->getXCoordinateValue() - x)<tol && fabs(existingNodes[k]->getYCoordinateValue() - y)<tol && fabs(existingNodes[k]->getZCoordinateValue() - z)<tol)
            {
                existingNodes[k]->setFOGradientXLambda(nodalXGradientLambda);
                existingNodes[k]->setFOGradientYLambda(nodalYGradientLambda);
                existingNodes[k]->setFOGradientZLambda(nodalZGradientLambda);
                existingNodes[k]->setObjectName(existingNodes[k]->objectName()+creatorName);

                theNodeVector[i] = existingNodes[k];

                foundNode = true;

                break;
            }
        }

        // Create a node if none was found at this location
        if (!foundNode) {

            // Create the node and set its coordinates
            theDomain->createObject(creatorName + nodeName, "RNode");
            theNode = qobject_cast<RNode *>(theDomain->getLastAddedObject());
            theNode->setXCoordinate(xCoordinatelambda);
            theNode->setYCoordinate(yCoordinatelambda);
            theNode->setZCoordinate(zCoordinatelambda);

            theNode->setFOGradientXLambda(nodalXGradientLambda);
            theNode->setFOGradientYLambda(nodalYGradientLambda);
            theNode->setFOGradientZLambda(nodalZGradientLambda);

            theNodeVector[i] = theNode;
        }
    }


    // **** LAY OUT THE ELEMENTS AND ASSIGN NODES FROM ABOVE *****
    for (int i=0; i<ni; ++i)
    {
        // Element name
        elementName = QString("Element_%1").arg(i);

        // Create elements and stack them in a list, so the component can set properties
        theDomain->createObject(creatorName + elementName, elementType);
        theElement = qobject_cast<RElement *>(theDomain->getLastAddedObject());

        theElement->setNode(0, theNodeVector[i]);
        theElement->setNode(1, theNodeVector[i+1]);

        elementList << theElement;
    }

    return true;
}







