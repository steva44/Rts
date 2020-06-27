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

#include "RRectangularRCFibreSectionKnowledge.h"
#include "RRectangularRCColumnComponent.h"
#include "RMeshGenerator.h"
#include "RFiberSection3d.h"
#include "RConcreteMaterialKnowledge.h"
#include "RSteelMaterialKnowledge.h"
#include "RQuadRCSectionDamageRegion.h"
#include "R2DPoint.h"
#include "R3DPoint.h"
#include "RFibreSectionMeshGenerator.h"
#include "RElasticMaterial.h"

RRectangularRCFibreSectionKnowledge::RRectangularRCFibreSectionKnowledge(RDomain* domain,
                                                                         RRectangularRCColumnComponent* component,
                                                                         int& numSec,
                                                                         int& numFibresInCoreAlong12,
                                                                         int& numFibresInCoreAlong23,
                                                                         int& numFibresInCoverAlong12,
                                                                         int& numFibresInCoverAlong23,
                                                                         int& numReinforcementFibresAlong12,
                                                                         int& numReinforcementFibresAlong23,
                                                                         int& numReinforcementFibresAlong34,
                                                                         int& numReinforcementFibresAlong41)
    : RFibreSectionKnowledge(domain, component->objectName()),
      theComponent(component),
      numFibresInCoreAlong12(numFibresInCoreAlong12),
      numFibresInCoreAlong23(numFibresInCoreAlong23),
      numFibresInCoverAlong12(numFibresInCoverAlong12),
      numFibresInCoverAlong23(numFibresInCoverAlong23),
      numRebarAlong12(numReinforcementFibresAlong12),
      numRebarAlong23(numReinforcementFibresAlong23),
      numRebarAlong34(numReinforcementFibresAlong34),
      numRebarAlong41(numReinforcementFibresAlong41)
{
    theGJParameter = nullptr;
    numSections = numSec;
    theDepth = component->getCrossSectionDepth();
    theWidth = component->getCrossSectionWidth();
    theCoverThickness = component->getConcreteCoverThickness();
    theUnconfinedConcreteMatKnowledge = component->getTheConcreteMaterialKnowledge();
    theConfinedConcreteMatKnowledge = component->getTheConfinedConcreteMaterialKnowledge();
    theReinforcementMaterialKnowledge = component->getTheReinforcementMaterialKnowledge();
    theLongitudinalReinforcementRatio = component->getTheLongitudinalReinforcementRatioParameter();

    auto rebarDesignation = component->getLongitudinalRebarDesignation();

    if(rebarDesignation == "10M")
    {
        theLongRebarArea = 0.000100;
    }
    else if(rebarDesignation == "15M")
    {
        theLongRebarArea =  0.000200;
    }
    else if(rebarDesignation == "20M")
    {
        theLongRebarArea =  0.000300;
    }
    else if(rebarDesignation == "25M")
    {
        theLongRebarArea =  0.000500;
    }
    else if(rebarDesignation == "30M")
    {
        theLongRebarArea =  0.000700;
    }
    else if(rebarDesignation == "35M")
    {
        theLongRebarArea =  0.0001000;
    }
    else if(rebarDesignation == "45M")
    {
        theLongRebarArea =  0.0001500;
    }
    else if(rebarDesignation == "55M")
    {
        theLongRebarArea =  0.0002500;
    }
    else
    {
        qDebug()<<"Rebar designation is not recognized for "<<this->objectName()<<", reverting to 15M";
        theLongRebarArea = 0.000200;
    }
}


RRectangularRCFibreSectionKnowledge::~RRectangularRCFibreSectionKnowledge()
{

}


RParameter *RRectangularRCFibreSectionKnowledge::getGJ()
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
            auto theEParameter = theUnconfinedConcreteMatKnowledge->getYoungsModulus();
            auto theNuParameter = theUnconfinedConcreteMatKnowledge->getPoissonsRatio();
            auto G = theEParameter->getCurrentValue()/(2.0*(1.0+theNuParameter->getCurrentValue()));

            return G*J;

        };
        theGJParameter->setCurrentValue(GJValue);
    }

    return theGJParameter;
}


std::shared_ptr<std::vector<RSectionForceDeformation*>> RRectangularRCFibreSectionKnowledge::getTheSectionForceDeformation(const QString& sectionID)
{

    /*
    *                  Depth
    *
    *       /  4^--------8-------^3
    * cover |   |        ^ y     |
    *       /   |    ^---|--^    |           // Notice Z axis orientated along the member depth and the x-axis comes out of the page
    *           |    |   |  |    | Width
    *           5 z<-|---*  |    6
    *           |    ^------^    |        y
    *           |                |        ^
    *          1^--------7-------^2       |
    *                                 z<- \
    *
    */

    if(theUnconfinedConcreteMatKnowledge == nullptr || theReinforcementMaterialKnowledge == nullptr)
    {
        qDebug()<<"Could not get a material knowledge class in "<<__FUNCTION__;
        return nullptr;
    }

    // The length of the component
    auto length = theComponent->getTheLengthParameter();

    if(length == nullptr || theWidth == nullptr || theDepth == nullptr)
    {
        qDebug()<<"The length, width, or depth parameter is null in "<<__FUNCTION__;
        return nullptr;
    }

    if(length->getCurrentValue() == 0.0)
    {
        qDebug()<<"The length is zero in "<<__FUNCTION__;
        return nullptr;
    }


    QString theSteelMaterialModel = theReinforcementMaterialKnowledge->getTheSteelMaterialModel();

    auto sections = std::make_shared<std::vector<RSectionForceDeformation*>>(numSections);

    // Create the mesh generator for meshing the fibre sections
    RFibreSectionMeshGenerator theMeshGenerator(theDomain);

    // The four corner points of the rectangular cross-section
    R2DPoint point1_2D;        R2DPoint point3_2D;
    R2DPoint point2_2D;        R2DPoint point4_2D;

    // Store the cross-section corner point coordinates as lambdas, so if the width or depth parameters change, the corner points will be automatically updated
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
    // qDebug()<<"theCoverThickness"<<theCoverThickness->getCurrentValue();

    // Point 1
    point1_2D.setYCoordinateValue(point1YValue);
    point1_2D.setZCoordinateValue(point1ZValue);
    // Point 2
    point2_2D.setYCoordinateValue(point2YValue);
    point2_2D.setZCoordinateValue(point2ZValue);
    // Point 3
    point3_2D.setYCoordinateValue(point3YValue);
    point3_2D.setZCoordinateValue(point3ZValue);
    // Point 4
    point4_2D.setYCoordinateValue(point4YValue);
    point4_2D.setZCoordinateValue(point4ZValue);

    //Set the area of the "visible surface" for each fiber container
    auto lengthParam = theComponent->getTheLengthParameter();

    std::function<double(void)> theSurface12 = [=](){

        return lengthParam->getCurrentValue()*theDepth->getCurrentValue()/static_cast<double>(numSections);
    };

    std::function<double(void)> theSurface34 = [=](){

        return lengthParam->getCurrentValue()*theDepth->getCurrentValue()/static_cast<double>(numSections);
    };

    std::function<double(void)> theSurface23 = [=](){

        return lengthParam->getCurrentValue()*theWidth->getCurrentValue()/static_cast<double>(numSections);

    };

    std::function<double(void)> theSurface41 = [=](){

        return lengthParam->getCurrentValue()*theWidth->getCurrentValue()/static_cast<double>(numSections);

    };


    // Create torisonal stiffness for this section
    theDomain->createObject("TorsionalMaterial" + this->objectName() + sectionID, "RElasticMaterial");
    auto torsionalMat = qobject_cast<RElasticMaterial*>(theDomain->getLastAddedObject());

    torsionalMat->setTheEParameter(this->getGJ());

    // Get the 2D section vertex loop
    std::vector<R2DPoint> sectionVertexLoop(4);

    sectionVertexLoop[0] = point1_2D;
    sectionVertexLoop[1] = point2_2D;
    sectionVertexLoop[2] = point3_2D;
    sectionVertexLoop[3] = point4_2D;

    // Create the sections
    for (int i=0; i<numSections; ++i)
    {
        auto sectionFiberContainer = std::make_shared<RQuadRCSectionDamageRegion>();

        sectionFiberContainer->quadrantI.setTheSurfaceArea(theSurface12);
        sectionFiberContainer->quadrantIII.setTheSurfaceArea(theSurface34);

        sectionFiberContainer->quadrantII.setTheSurfaceArea(theSurface23);
        sectionFiberContainer->quadrantIV.setTheSurfaceArea(theSurface41);

        // First mesh out the concrete into fibres
        auto res = theMeshGenerator.meshRCQuadrilateralSection(point1_2D,point2_2D,point3_2D,point4_2D,sectionID,
                                                               numFibresInCoverAlong12,numFibresInCoverAlong23,numFibresInCoreAlong12,
                                                               numFibresInCoreAlong23,theCoverThickness,sectionFiberContainer.get(),
                                                               theConfinedConcreteMatKnowledge,theUnconfinedConcreteMatKnowledge);
        if(res != 0)
        {
            qDebug()<<"Error in meshing out the concrete fibers in "<<__FUNCTION__;
            return nullptr;
        }

        // Now mesh out the rebar "layers"
        auto res2 = theMeshGenerator.meshQuadrilateralRebarLayer(point1_2D,point2_2D,point3_2D,point4_2D,numRebarAlong12,numRebarAlong23,numRebarAlong34,numRebarAlong41,
                                                                 theLongRebarArea,theSteelMaterialModel,sectionID,theCoverThickness,theCoverThickness,theCoverThickness,
                                                                 theCoverThickness,sectionFiberContainer.get(),theReinforcementMaterialKnowledge);

        if(res2 != 0)
        {
            qDebug()<<"Error in meshing out the reinforcement fibers in "<<__FUNCTION__;
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

        // Used for creating the topo mesh
        theRSection->setSectionVertexLoop(sectionVertexLoop);

        sections->at(i) = theRSection;
    }

    return sections;
}







