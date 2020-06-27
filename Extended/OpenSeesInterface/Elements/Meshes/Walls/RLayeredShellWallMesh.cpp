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

#include "RLayeredShellWallMesh.h"
#include "RMeshGenerator.h"
#include "RSectionKnowledge.h"
#include "RRCWallLayeredShellSectionKnowledge.h"
#include "RElement.h"
#include "RPoint.h"
#include "RShellMITC4.h"

//The class creates a layered shell mesh from a four point wall geometry
//A wall is defined with 4 corner points and a thickness

RLayeredShellWallMesh::RLayeredShellWallMesh(RDomain *domain,
                                             QString componentName,
                                             QString elementType,
                                             RSectionKnowledge *crossSection,
                                             QPointer<RParameter> numElementsAlongU,
                                             QPointer<RParameter> numElementsAlongV,
                                             RPoint *point1,
                                             RPoint *point2,
                                             RPoint *point3,
                                             RPoint *point4,
                                             RParameter* thickness,
                                             QList<RElement*>& elementList)
    : RWallMesh(domain, componentName), theCrossSection(crossSection)
{
    //Set the members in the parent class
    thePoint1 = point1;
    thePoint2 = point2;
    thePoint3 = point3;
    thePoint4 = point4;
    theThickness = thickness;
    theNumElementsAlongU = numElementsAlongU;
    theNumElementsAlongV = numElementsAlongV;

    theElementList = &elementList;

    theElementType = elementType;
}


RLayeredShellWallMesh::~RLayeredShellWallMesh()
{
}


int RLayeredShellWallMesh:: createMesh(void)
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

    //Check to make sure the cross-section knowledge is valid
    if(theCrossSection == nullptr )
    {
        qCritical()<<this->objectName()<<" requires a cross-section knowledge in "<<__FUNCTION__;
        return -1;
    }

    auto layeredShellSectionKnowledge = qobject_cast<RRCWallLayeredShellSectionKnowledge*>(theCrossSection);

    if(layeredShellSectionKnowledge == nullptr)
    {
        qCritical()<<"Error: the section knowledge in "<<this->objectName() <<" must be of the type RRCWallLayeredShellSectionKnowledge";
        return -1;
    }


    RMeshGenerator theMeshGenerator(theDomain);
    QList<RElement*> theMeshedElementList;

    // NOW CREATE THE MESH
    if (theElementType == "RShellMITC4")
    {

        auto FDsection = layeredShellSectionKnowledge->getTheSectionForceDeformation();

        if(FDsection == nullptr)
        {
            qCritical()<<"Error, was not able to get the Section Force Deformation from "<<layeredShellSectionKnowledge->objectName()<<" in"<<__FUNCTION__;
            return -1;
        }

        // Generate RShellMITC4
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
            theElement->setTheSectionForceDeformation(FDsection);

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


    return 0;
}

