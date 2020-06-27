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

#include "RNonlinearBeamColumnMesh.h"
#include "RMeshGenerator.h"
#include "RFibreSectionKnowledge.h"
#include "RCSectionIntegration.h"
#include "RNonlinearBeamColumnElement.h"

#include "SectionForceDeformation.h"
#include "Information.h"
#include "DispBeamColumn3d.h"

RNonlinearBeamColumnMesh::RNonlinearBeamColumnMesh(RDomain *domain,
                                                   QString parentName,
                                                   QString elementType,
                                                   QList<RElement*>& elementList,
                                                   QList<RParameter*>& theParameterList,
                                                   int numEl,
                                                   RSectionKnowledge *crossSection,
                                                   RPoint *point1,
                                                   RPoint *point2,
                                                   RParameter* orientation)
    : RBeamColumnMesh(domain, parentName, point1, point2),theCrossSection(crossSection),
      theParentName(parentName), theOrientationParameter(orientation),numElements(numEl), theElementType(elementType)
{
    theElementList = &elementList;
    theRhoParameter = nullptr;
    crdTransformationType = "LinearCrdTransf3d";
    integrationType = "Lobatto";
}


RNonlinearBeamColumnMesh::~RNonlinearBeamColumnMesh()
{

}


int RNonlinearBeamColumnMesh::createMesh(void)
{

    // Create the mesh generator
    RMeshGenerator theMeshGenerator(theDomain);

    numberOfElementsAlongU = numElements;

    if(numberOfElementsAlongU <= 0)
    {
        qCritical()<<"Error: An the number of elements must be 1 or greater "<<this->objectName();
        return -1;
    }

    // The number of elements along the longitudinal axis
    theMeshGenerator.meshLinearElement(thePoint1,thePoint2,theElementType,numberOfElementsAlongU,theParentName,*theElementList);

    if(theElementList->empty())
    {
        qCritical()<<"Error: An empty element list was returned from the mesh generator in "<<this->objectName();
        return -1;
    }

    auto theFibreSectionKnowledge = qobject_cast<RFibreSectionKnowledge*>(theCrossSection);

    if(theFibreSectionKnowledge == nullptr)
    {
        qCritical()<<"Error: the section knowledge in "<<this->objectName() <<" must be a RFibreSectionKnowledge";
        return -1;
    }

    auto OpenSeesDomain = theDomain->getTheOpenSeesDomain();

    for(auto&& it : *theElementList)
    {
        auto theRElement = qobject_cast<RNonlinearBeamColumnElement*>(it);

        if(theRElement == nullptr)
        {
            qCritical()<<"Error: element in "<<this->objectName() <<" must be a nonlinear element";
            return -1;
        }

        auto elemID = QString::number( theRElement->getObjectTag());

        // Get the sections from the section knowledge
        auto theSections = theFibreSectionKnowledge->getTheSectionForceDeformation(elemID);
        if(theSections == nullptr)
        {
            qCritical()<<"Error getting the sections in "<<this->objectName()<<" function "<<__FUNCTION__;
            return -1;
        }

        // Add the sections to the element
        theRElement->setTheSectionForceDeformation(theSections);

        // Set the orientation
        theRElement->setOrientation(theOrientationParameter);

        // Set the density
        theRElement->setRho(theRhoParameter);

        // Set the coordinate transformation type
        theRElement->setCrdTransformationType(crdTransformationType);

        // Set the integration typw
        theRElement->setIntegrationType(integrationType);

        // Get the OpenSees element
        auto theElement = theRElement->getTheOpenSeesElement();

        if(theElement == nullptr)
        {
            qDebug()<<"Error getting the element from "<<theRElement->objectName()<<" in "<<__FUNCTION__;
            return -1;
        }

        OpenSeesDomain->addElement(theElement);
    }

    return 0;
}


double RNonlinearBeamColumnMesh::getAxialLoad(int elementNumber)
{     
    auto theElement = qobject_cast<RNonlinearBeamColumnElement*>(theElementList->at(elementNumber));

    if(theElement == nullptr)
    {
        qDebug()<<"Error in "<<this->objectName()<<" in "<<__FUNCTION__;
        return 0.0;
    }

    QVector<double> response(12);
    theElement->getLocalResponse(&response,"EndForces",0);

    auto N1 = response[0];
    auto N2 = response[6];

    // Return the larger of the two
    return (fabs(N1) > fabs(N2)) ? N1 : N2;
}


RParameter *RNonlinearBeamColumnMesh::getTheRhoParameter() const
{
    return theRhoParameter;
}


void RNonlinearBeamColumnMesh::setTheRhoParameter(RParameter *value)
{
    theRhoParameter = value;
}


QString RNonlinearBeamColumnMesh::getCrdTransformationType() const
{
    return crdTransformationType;
}


void RNonlinearBeamColumnMesh::setCrdTransformationType(const QString &value)
{
    crdTransformationType = value;
}


QString RNonlinearBeamColumnMesh::getIntegrationType() const
{
    return integrationType;
}

void RNonlinearBeamColumnMesh::setIntegrationType(const QString &value)
{
    integrationType = value;
}


RSectionKnowledge *RNonlinearBeamColumnMesh::getTheCrossSection() const
{
    return theCrossSection;
}

void RNonlinearBeamColumnMesh::setTheCrossSection(RSectionKnowledge *value)
{
    theCrossSection = value;
}


double RNonlinearBeamColumnMesh::getMaxStress()
{
    return 0.0;
}



