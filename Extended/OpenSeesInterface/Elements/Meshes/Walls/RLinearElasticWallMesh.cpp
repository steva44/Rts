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

#include "RLinearElasticWallMesh.h"
#include "RMeshGenerator.h"
#include "RPoint.h"
#include "RElasticMembranePlateSection.h"
#include "RShellMITC4.h"

//The class creates a plate mesh from a four point wall geometry
//A wall is defined with 4 corner points and a thickness

RLinearElasticWallMesh::RLinearElasticWallMesh(RDomain *theDomain,
                                               QString theComponentName,
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
                                               QString elementType)
    : RWallMesh(theDomain, theComponentName),theMaterial(material)
{

    //Set the members in the parent class
    theElementList = &elementList;
    theThickness = thickness;
    thePoint1 = point1;
    thePoint2 = point2;
    thePoint3 = point3;
    thePoint4 = point4;
    theElementType = elementType;
    theNumElementsAlongU = numElementsAlongU;
    theNumElementsAlongV = numElementsAlongV;
}


RLinearElasticWallMesh::~RLinearElasticWallMesh()
{
}


int RLinearElasticWallMesh::createMesh(void)
{
    // Do not create mesh until the end-points and other geometry parameters are set
    if (thePoint1 == nullptr || thePoint2 == nullptr ||  thePoint3 == nullptr||  thePoint4 == nullptr || theThickness == nullptr)
    {
        qCritical()<<"Null pointers for points came into "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    //Check to make sure thickness is valid
    if(theThickness->getCurrentValue() <= 0.0  )
    {
        qCritical()<<"The thickness cannot be less than or equal to zero in "<<this->objectName();
        return -1;
    }

    // Create the in the constructor
    RMeshGenerator theMeshGenerator(theDomain);
    QList<RElement*> theMeshedElementList;


    // RShellMITC4
    if (theElementType == "RShellMITC4")
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

        // Generate RShellMITC4 elements
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











