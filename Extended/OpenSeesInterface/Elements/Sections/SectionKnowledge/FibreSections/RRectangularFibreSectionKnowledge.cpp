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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RRectangularFibreSectionKnowledge.h"
#include "RRectangularGlulamBeamColumnComponent.h"
#include "RMeshGenerator.h"
#include "RFiberSection3d.h"
#include "RGlulamMaterialKnowledge.h"
#include "RQuadSectionDamageRegion.h"
#include "R2DPoint.h"
#include "RFibreSectionMeshGenerator.h"
#include "RElasticMaterial.h"

RRectangularFibreSectionKnowledge::RRectangularFibreSectionKnowledge(RDomain* domain,
                                                                     RRectangularGlulamBeamColumnComponent* component,
                                                                     int& numSec,
                                                                     int& numFibresAlong12,
                                                                     int& numFibresAlong23,
                                                                     QString& materialModel)
    : RFibreSectionKnowledge(domain, component->objectName()),
      theComponent(component),
      numFibresAlong12(numFibresAlong12),
      numFibresAlong23(numFibresAlong23),
      theMaterialModel(materialModel)
{
    numSections = numSec;

    theDepth = component->getCrossSectionDepth();
    theWidth = component->getCrossSectionWidth();

    theMatKnowledge = component->getTheGlulamMaterial();

    theGJParameter = nullptr;
    theAreaParameter = nullptr;
}


RRectangularFibreSectionKnowledge::~RRectangularFibreSectionKnowledge()
{

}


RParameter *RRectangularFibreSectionKnowledge::getGJ()
{
    if(theGJParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "GJLambdaParameter", "RConstant");
        theGJParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto GJValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto a = theDepth->getCurrentValue();
            auto b = theWidth->getCurrentValue();

            auto J = a*b*b*b*(0.333-0.21*b/a*(1.0-b*b*b*b/(12*a*a*a*a)));

            //For now assume that only concrete contributes to torsional stiffness
            auto theEParameter = theMatKnowledge->getYoungsModulus();
            auto theNuParameter = theMatKnowledge->getPoissonsRatio();
            auto G = theEParameter->getCurrentValue()/(2.0*(1.0+theNuParameter->getCurrentValue()));

            return G*J;

        };
        theGJParameter->setCurrentValue(GJValue);
    }

    return theGJParameter;
}


RParameter *RRectangularFibreSectionKnowledge::getArea()
{
    if(theAreaParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "AreaLambdaParameter", "RConstant");
        theAreaParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto AreaLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2) {
            auto h = theDepth->getCurrentValue();
            auto b = theWidth->getCurrentValue();

            return b*h;
        };
        theAreaParameter->setCurrentValue(AreaLambda);
    }

    return theAreaParameter;
}


std::shared_ptr<std::vector<RSectionForceDeformation*>> RRectangularFibreSectionKnowledge::getTheSectionForceDeformation(const QString& sectionID)
{
    // Check if the material knowledge is given
    if(theMatKnowledge == nullptr || theComponent == nullptr)
    {
        qDebug()<<"Could not get a material knowledge or component in "<<__FUNCTION__;
        return nullptr;
    }

    // Get the length of the component
    auto length = theComponent->getTheLengthParameter();

    if(length == nullptr || theWidth == nullptr || theDepth == nullptr)
    {
        qDebug()<<"The length, width, or depth parameter is null in "<<__FUNCTION__;
        return nullptr;
    }

    // Check if the length is valid
    if(length->getCurrentValue() <= 0.0)
    {
        qDebug()<<"The length is zero in "<<__FUNCTION__;
        return nullptr;
    }

    // Make the vector to hold the sctions
    auto sections = std::make_shared<std::vector<RSectionForceDeformation*>>(numSections);

    // Create the mesh generator for meshing the sections
    RFibreSectionMeshGenerator theMeshGenerator(theDomain);

    R2DPoint point1;        R2DPoint point3;
    R2DPoint point2;        R2DPoint point4;

    // Store the corner point coordinates as lambdas, so if the width or depth parameters change, the corner points will be automatically updated
    std::function<double(void)> point1YValue = [&](void){return -theWidth->getCurrentValue()/2.0;};
    std::function<double(void)> point1ZValue = [&](void){return theDepth->getCurrentValue()/2.0;};

    std::function<double(void)> point2YValue = [&](void){return -theWidth->getCurrentValue()/2.0;};
    std::function<double(void)> point2ZValue = [&](void){return -theDepth->getCurrentValue()/2.0;};

    std::function<double(void)> point3YValue = [&](void){return theWidth->getCurrentValue()/2.0;};
    std::function<double(void)> point3ZValue = [&](void){return -theDepth->getCurrentValue()/2.0;};

    std::function<double(void)> point4YValue = [&](void){return theWidth->getCurrentValue()/2.0;};
    std::function<double(void)> point4ZValue = [&](void){return theDepth->getCurrentValue()/2.0;};

    // qDebug()<<"theWidth"<<theWidth->getCurrentValue();
    // qDebug()<<"theDepth"<<theDepth->getCurrentValue();

    // Point 1
    point1.setYCoordinateValue(point1YValue);
    point1.setZCoordinateValue(point1ZValue);
    // Point 2
    point2.setYCoordinateValue(point2YValue);
    point2.setZCoordinateValue(point2ZValue);
    // Point 3
    point3.setYCoordinateValue(point3YValue);
    point3.setZCoordinateValue(point3ZValue);
    // Point 4
    point4.setYCoordinateValue(point4YValue);
    point4.setZCoordinateValue(point4ZValue);

    // Torisonal stiffness for this section
    theDomain->createObject("TorsionalMaterial" + this->objectName() + sectionID, "RElasticMaterial");
    auto torsionalMat = qobject_cast<RElasticMaterial*>(theDomain->getLastAddedObject());
    torsionalMat->setTheEParameter(this->getGJ());

    // Create the sections
    for (int i=0; i<numSections; ++i)
    {
        // Make a fibre container to hold the fibres that will be meshed out below
        auto sectionFiberContainer = std::make_shared<RQuadSectionDamageRegion>();

        auto res = theMeshGenerator.meshHomogeneousQuadrilateralSection(point1,point2,point3,point4,numFibresAlong12,numFibresAlong23,theMaterialModel,sectionID,sectionFiberContainer.get(),theMatKnowledge);

        if(res != 0)
        {
            qDebug()<<"Error in meshing out the concrete fibers in "<<__FUNCTION__;
            return nullptr;
        }

        auto fibers = sectionFiberContainer->getAllFibres();

        auto sectionName = QString("Section%1ofComponent%2").arg(i).arg(this->objectName()) + sectionID;
        theDomain->createObject(this->objectName() + sectionName, "RFiberSection3d");
        auto theRSection = qobject_cast<RFiberSection3d *>(theDomain->getLastAddedObject());

        // Set the fiber array
        theRSection->setFibers(fibers);

        // Set the section number
        theRSection->setSectionNumber(i);

        // Set torisional stiffness
        theRSection->setTorsion(torsionalMat);

        theRSection->setTheFibreContainer(sectionFiberContainer);

        sections->at(i) = theRSection;
    }

    return sections;
}








