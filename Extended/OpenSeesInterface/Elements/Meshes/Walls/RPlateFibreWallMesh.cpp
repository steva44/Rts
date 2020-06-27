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

#include "RPlateFibreWallMesh.h"
#include "RMeshGenerator.h"
#include "RPoint.h"
#include "RMembranePlateFiberSection.h"
#include "RShellMITC4.h"
#include "RRCShearWallFibreSectionKnowledge.h"


RPlateFibreWallMesh::RPlateFibreWallMesh(RDomain *domain,
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
                                         QList<RElement*> &elementList)
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


RPlateFibreWallMesh::~RPlateFibreWallMesh()
{
}


int RPlateFibreWallMesh::createMesh(void)
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

    auto fibreSectionKnowledge = qobject_cast<RRCShearWallFibreSectionKnowledge*>(theCrossSection);

    if(fibreSectionKnowledge == nullptr)
    {
        qCritical()<<"Error: the section knowledge in "<<this->objectName() <<" must be of the type RRCWallLayeredShellSectionKnowledge";
        return -1;
    }

    // Create the in the constructor
    RMeshGenerator theMeshGenerator(theDomain);
    QList<RElement*> theMeshedElementList;


    // Option 1: RShellMITC4
    if (theElementType == "RShellMITC4")
    {

        auto FDsection = fibreSectionKnowledge->getTheSectionForceDeformation();


        if(FDsection == nullptr)
        {
            qCritical()<<"Error, was not able to get the Section Force Deformation from "<<fibreSectionKnowledge->objectName()<<" in"<<__FUNCTION__;
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




