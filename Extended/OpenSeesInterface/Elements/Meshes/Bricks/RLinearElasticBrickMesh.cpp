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

#include "RLinearElasticBrickMesh.h"
#include "RMeshGenerator.h"
#include "RComponent.h"
#include "RMaterialKnowledge.h"
#include "RParameter.h"
#include "RLinearElasticSectionKnowledge.h"
#include "RRectangularLinearElasticSectionKnowledge.h"
#include "RBrick.h"
#include "RPoint.h"
#include "RElasticIsotropicMaterial.h"

RLinearElasticBrickMesh::RLinearElasticBrickMesh(RDomain *theDomain,
                                                 QString parentName,
                                                 RMaterialKnowledge *material,
                                                 RSectionKnowledge *crossSection,
                                                 RPoint *point1,
                                                 RPoint *point2,
                                                 int numElementsU,
                                                 int numElementsV,
                                                 int numElementsW,
                                                 QList<RParameter*> &parameterList,
                                                 QList<RElement*> &elementList)
    : RBeamColumnMesh(theDomain,parentName,point1,point2),theCrossSection(crossSection),theMaterial(material),
      theParentName(parentName), numberElementsU(numElementsU),numberElementsV(numElementsV),numberElementsW(numElementsW)
{
    theElementList = &elementList;

    depth = nullptr;
    width = nullptr;
}


RLinearElasticBrickMesh::~RLinearElasticBrickMesh()
{
}


int RLinearElasticBrickMesh::createMesh(void)
{  

    if(thePoint1 == nullptr || thePoint2 == nullptr)
    {
        qDebug()<<"Points are required in "<<this->objectName()<<__FUNCTION__;
        return -1;
    }

    if(theMaterial == nullptr)
    {
        qDebug()<<"Material knowledge is required in "<<this->objectName()<<__FUNCTION__;
        return -1;
    }

    if(theCrossSection == nullptr)
    {
        qDebug()<<"Cross-section knowledge is required in "<<this->objectName()<<__FUNCTION__;
        return -1;
    }

    //Create the 8 corner points of the hexadedron volume
    /*
    *                  Width
    *
    *           4^----------------^3          //Bottom
    *            |                |
    *            |                |
    *            |     Point1     | Depth
    *            |        *       |          //From http://opensees.berkeley.edu/wiki/index.php/Standard_Brick_Element
    *            |                |
    *            |                |           x<-----z*
    *           1^----------------^2                  | y
    *                                                 |
    *                                                 |
    *                  Width                          v
    *
    *           8^----------------^7
    *            |                |
    *            |                |           //Top
    *            |     Point2     | Depth
    *            |        *       |
    *            |                |
    *            |                |
    *           5^----------------^6
    *
    *
    */

    RPoint* point1 = nullptr;
    RPoint* point2 = nullptr;
    RPoint* point3 = nullptr;
    RPoint* point4 = nullptr;
    RPoint* point5 = nullptr;
    RPoint* point6 = nullptr;
    RPoint* point7 = nullptr;
    RPoint* point8 = nullptr;


    theDomain->createObject(this->objectName()+"Point1","RPoint");
    point1 = qobject_cast<RPoint*>(theDomain->getLastAddedObject());

    theDomain->createObject(this->objectName()+"Point2","RPoint");
    point2 = qobject_cast<RPoint*>(theDomain->getLastAddedObject());

    theDomain->createObject(this->objectName()+"Point3","RPoint");
    point3 = qobject_cast<RPoint*>(theDomain->getLastAddedObject());

    theDomain->createObject(this->objectName()+"Point4","RPoint");
    point4 = qobject_cast<RPoint*>(theDomain->getLastAddedObject());

    theDomain->createObject(this->objectName()+"Point5","RPoint");
    point5 = qobject_cast<RPoint*>(theDomain->getLastAddedObject());

    theDomain->createObject(this->objectName()+"Point6","RPoint");
    point6 = qobject_cast<RPoint*>(theDomain->getLastAddedObject());

    theDomain->createObject(this->objectName()+"Point7","RPoint");
    point7 = qobject_cast<RPoint*>(theDomain->getLastAddedObject());

    theDomain->createObject(this->objectName()+"Point8","RPoint");
    point8 = qobject_cast<RPoint*>(theDomain->getLastAddedObject());

    if(!point1 || !point2 || !point3 || !point4 || !point5 || !point6 || !point7 || !point8)
    {
        qDebug()<<"Error creating one or more points in "<<this->objectName();
        return -1;
    }

    auto theSection = qobject_cast<RRectangularLinearElasticSectionKnowledge*>(theCrossSection);

    if(theSection == nullptr)
    {
        qDebug()<<"Error: "<<this->objectName()<<" requires a cross-section that is a RRectangularLinearElasticSectionKnowledge";
        return -1;
    }

    depth = theSection->getTheDepth();
    width = theSection->getTheWidth();

    if(depth == nullptr || width == nullptr)
    {
        qDebug()<<"Error: "<<this->objectName()<<" requires a depth and a width from the cross-section";
        return -1;
    }

    if(depth->getCurrentValue() == 0.0 || width->getCurrentValue() == 0.0)
    {
        qDebug()<<"Error: "<<this->objectName()<<" requires a depth and a width that are not zero!";
        return -1;
    }

    std::function<double(void)> point1x = [&](){return thePoint1->getXCoordinateValue() + width->getCurrentValue()/2.0;};
    std::function<double(void)> point1y = [&](){return thePoint1->getYCoordinateValue() + depth->getCurrentValue()/2.0;};
    std::function<double(void)> point1z = [&](){return thePoint1->getZCoordinateValue();};

    std::function<double(void)> point2x = [&](){return thePoint1->getXCoordinateValue() - width->getCurrentValue()/2.0;};
    std::function<double(void)> point2y = [&](){return thePoint1->getYCoordinateValue() + depth->getCurrentValue()/2.0;};
    std::function<double(void)> point2z = [&](){return thePoint1->getZCoordinateValue();};

    std::function<double(void)> point3x = [&](){return thePoint1->getXCoordinateValue() - width->getCurrentValue()/2.0;};
    std::function<double(void)> point3y = [&](){return thePoint1->getYCoordinateValue() - depth->getCurrentValue()/2.0;};
    std::function<double(void)> point3z = [&](){return thePoint1->getZCoordinateValue();};

    std::function<double(void)> point4x = [&](){return thePoint1->getXCoordinateValue() + width->getCurrentValue()/2.0;};
    std::function<double(void)> point4y = [&](){return thePoint1->getYCoordinateValue() - depth->getCurrentValue()/2.0;};
    std::function<double(void)> point4z = [&](){return thePoint1->getZCoordinateValue();};

    std::function<double(void)> point5x = [&](){return thePoint2->getXCoordinateValue() + width->getCurrentValue()/2.0;};
    std::function<double(void)> point5y = [&](){return thePoint2->getYCoordinateValue() + depth->getCurrentValue()/2.0;};
    std::function<double(void)> point5z = [&](){return thePoint2->getZCoordinateValue();};

    std::function<double(void)> point6x = [&](){return thePoint2->getXCoordinateValue() - width->getCurrentValue()/2.0;};
    std::function<double(void)> point6y = [&](){return thePoint2->getYCoordinateValue() + depth->getCurrentValue()/2.0;};
    std::function<double(void)> point6z = [&](){return thePoint2->getZCoordinateValue();};

    std::function<double(void)> point7x = [&](){return thePoint2->getXCoordinateValue() - width->getCurrentValue()/2.0;};
    std::function<double(void)> point7y = [&](){return thePoint2->getYCoordinateValue() - depth->getCurrentValue()/2.0;};
    std::function<double(void)> point7z = [&](){return thePoint2->getZCoordinateValue();};

    std::function<double(void)> point8x = [&](){return thePoint2->getXCoordinateValue() + width->getCurrentValue()/2.0;};
    std::function<double(void)> point8y = [&](){return thePoint2->getYCoordinateValue() - depth->getCurrentValue()/2.0;};
    std::function<double(void)> point8z = [&](){return thePoint2->getZCoordinateValue();};

    //    qDebug()<<"P1x: "<<point1->getXCoordinateValue()<<"P2x: "<<point2->getXCoordinateValue()<<"P3x: "<<point3->getXCoordinateValue()<<"P4x: "<<point4->getXCoordinateValue();
    //    qDebug()<<"P1Y: "<<point1->getYCoordinateValue()<<"P2Y: "<<point2->getYCoordinateValue()<<"P3Y: "<<point3->getYCoordinateValue()<<"P4Y: "<<point4->getYCoordinateValue();
    //    qDebug()<<"P1Z: "<<point1->getZCoordinateValue()<<"P2Z: "<<point2->getZCoordinateValue()<<"P3Z: "<<point3->getZCoordinateValue()<<"P4Z: "<<point4->getZCoordinateValue();

    point1->setXCoordinate(point1x);
    point1->setYCoordinate(point1y);
    point1->setZCoordinate(point1z);

    point2->setXCoordinate(point2x);
    point2->setYCoordinate(point2y);
    point2->setZCoordinate(point2z);

    point3->setXCoordinate(point3x);
    point3->setYCoordinate(point3y);
    point3->setZCoordinate(point3z);

    point4->setXCoordinate(point4x);
    point4->setYCoordinate(point4y);
    point4->setZCoordinate(point4z);

    point5->setXCoordinate(point5x);
    point5->setYCoordinate(point5y);
    point5->setZCoordinate(point5z);

    point6->setXCoordinate(point6x);
    point6->setYCoordinate(point6y);
    point6->setZCoordinate(point6z);

    point7->setXCoordinate(point7x);
    point7->setYCoordinate(point7y);
    point7->setZCoordinate(point7z);

    point8->setXCoordinate(point8x);
    point8->setYCoordinate(point8y);
    point8->setZCoordinate(point8z);

    //    qDebug()<<"theP1x: "<<thePoint1->getXCoordinateValue()<<"theP2x: "<<thePoint2->getXCoordinateValue();
    //    qDebug()<<"theP1Y: "<<thePoint1->getYCoordinateValue()<<"theP2Y: "<<thePoint2->getYCoordinateValue();
    //    qDebug()<<"theP1Z: "<<thePoint1->getZCoordinateValue()<<"theP2Z: "<<thePoint2->getZCoordinateValue();

    //    qDebug()<<"P1x: "<<point1->getXCoordinateValue()<<"P2x: "<<point2->getXCoordinateValue()<<"P3x: "<<point3->getXCoordinateValue()<<"P4x: "<<point4->getXCoordinateValue();
    //    qDebug()<<"P1Y: "<<point1->getYCoordinateValue()<<"P2Y: "<<point2->getYCoordinateValue()<<"P3Y: "<<point3->getYCoordinateValue()<<"P4Y: "<<point4->getYCoordinateValue();
    //    qDebug()<<"P1Z: "<<point1->getZCoordinateValue()<<"P2Z: "<<point2->getZCoordinateValue()<<"P3Z: "<<point3->getZCoordinateValue()<<"P4Z: "<<point4->getZCoordinateValue();


    //    qDebug()<<"P5x: "<<point5->getXCoordinateValue()<<"P6x: "<<point6->getXCoordinateValue()<<"P7x: "<<point7->getXCoordinateValue()<<"P8x: "<<point8->getXCoordinateValue();
    //    qDebug()<<"P5Y: "<<point5->getYCoordinateValue()<<"P6Y: "<<point6->getYCoordinateValue()<<"P7Y: "<<point7->getYCoordinateValue()<<"P8Y: "<<point8->getYCoordinateValue();
    //    qDebug()<<"P5Z: "<<point5->getZCoordinateValue()<<"P6Z: "<<point6->getZCoordinateValue()<<"P7Z: "<<point7->getZCoordinateValue()<<"P8Z: "<<point8->getZCoordinateValue();

    // Create the mesh
    RMeshGenerator theMeshGenerator(theDomain);

    theMeshGenerator.meshHexhedron(point1,
                                   point2,
                                   point3,
                                   point4,
                                   point5,
                                   point6,
                                   point7,
                                   point8,
                                   "RBrick",
                                   numberElementsU,
                                   numberElementsV,
                                   numberElementsW,
                                   theParentName,
                                   *theElementList);

    if(theElementList->empty())
    {
        qCritical()<<"Error: An empty element list was returned from the mesh generator in "<<this->objectName();
        return -1;
    }

    //    //Spread the load from points 1 and 2 to the four points just created
    //    std::function<double(void)> xForce1 = [=](){return thePoint1->getXForceValue()/4.0;};
    //    std::function<double(void)> yForce1 = [=](){return thePoint1->getYForceValue()/4.0;};
    //    std::function<double(void)> zForce1 = [=](){return thePoint1->getZForceValue()/4.0;};

    //    point1->setXForce(xForce1);
    //    point2->setXForce(xForce1);
    //    point3->setXForce(xForce1);
    //    point4->setXForce(xForce1);

    //    point1->setYForce(yForce1);
    //    point2->setYForce(yForce1);
    //    point3->setYForce(yForce1);
    //    point4->setYForce(yForce1);

    //    point1->setZForce(zForce1);
    //    point2->setZForce(zForce1);
    //    point3->setZForce(zForce1);
    //    point4->setZForce(zForce1);

    //    std::function<double(void)> xForce2 = [=](){return thePoint2->getXForceValue()/4.0;};
    //    std::function<double(void)> yForce2 = [=](){return thePoint2->getYForceValue()/4.0;};
    //    std::function<double(void)> zForce2 = [=](){return thePoint2->getZForceValue()/4.0;};

    //    point1->setXForce(xForce2);
    //    point2->setXForce(xForce2);
    //    point3->setXForce(xForce2);
    //    point4->setXForce(xForce2);

    //    point1->setYForce(yForce2);
    //    point2->setYForce(yForce2);
    //    point3->setYForce(yForce2);
    //    point4->setYForce(yForce2);

    //    point1->setZForce(zForce2);
    //    point2->setZForce(zForce2);
    //    point3->setZForce(zForce2);
    //    point4->setZForce(zForce2);

    //Create an elastic isotropic material
    theDomain->createObject(this->objectName()+"ElasticIsotropicMaterial","RElasticIsotropicMaterial");
    auto mat = qobject_cast<RElasticIsotropicMaterial*>(theDomain->getLastAddedObject());
    if(mat == nullptr)
    {
        qDebug()<<"Could not create a material layer for"<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    //Set the material knowledge to the cover concrete material
    mat->setTheMaterialKnowledge(theMaterial);

    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    for(auto&& it: *theElementList)
    {
        auto theElement = qobject_cast<RBrick*>(it);
        if(theElement == nullptr)
        {
            qDebug()<<"Error, could not cast to brick element";
            return -1;
        }

        theElement->setTheMaterial(mat);

        //Since a brick element only has 3 dof per node, we need to change from the usual 6 to avoid opensees throwing an error
        auto numNodes = theElement->getNumNodes();
        for(int i =0; i<numNodes;++i)
        {
            auto node = theElement->getNode(i);
            node->setNumDof(3);
        }


        //Now get/create the OpenSees element
        auto theOSElement = theElement->getTheOpenSeesElement();

        //Add it to the domain
        if(theOSElement)
            theOSDomain->addElement(theOSElement);
        else
        {
            qDebug()<<"Failed to get the OpenSees element from "<<theElement->objectName()<<" in "<<__FUNCTION__;
        }
    }

    return 0;
}


double RLinearElasticBrickMesh::getMaxStress()
{    
    return 0.0;
}


double RLinearElasticBrickMesh::getAxialLoad(int elementNumber)
{
    auto theAxialLoad = 0.0;

    return theAxialLoad;
}



