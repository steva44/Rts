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

#include "RWFibreSectionKnowledge.h"
#include "RMeshGenerator.h"
#include "RGenericFibreContainer.h"
#include "RWSectionDamageRegion.h"
#include "RFiberSection3d.h"
#include "R2DPoint.h"
#include "RMaterialKnowledge.h"
#include "RSectionForceDeformation.h"
#include "RFibreSectionMeshGenerator.h"
#include "RElasticMaterial.h"

RWFibreSectionKnowledge::RWFibreSectionKnowledge(RDomain *domain,
                                                 QString componentName,
                                                 int secNum,
                                                 RParameter *flangeWidth,
                                                 RParameter *flangeThickness,
                                                 RParameter *webHeight,
                                                 RParameter *webThickness,
                                                 int numFibresInFlange,
                                                 int numFibresInWeb,
                                                 QString materialModel,
                                                 RMaterialKnowledge *materialKnowledge)

    : RFibreSectionKnowledge(domain, componentName)
{
    numSections = secNum;
    theFlangeWidth = flangeWidth;
    theFlangeThickness = flangeThickness;
    theWebHeight = webHeight;
    theWebThickness = webThickness;
    theNumFibresInFlange = numFibresInFlange;
    theNumFibresInWeb = numFibresInWeb;
    theMaterialModel = materialModel;
    theMaterialKnowledge = materialKnowledge;
}


RWFibreSectionKnowledge::~RWFibreSectionKnowledge()
{

}


RParameter *RWFibreSectionKnowledge::getGJ()
{
    if(theGJParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "GJLambdaParameter", "RConstant");
        theGJParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto GJValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto t = theWebThickness->getCurrentValue();
            auto s = theFlangeThickness->getCurrentValue();
            auto d = theWebHeight->getCurrentValue()+s*2.0;
            auto b = theFlangeWidth->getCurrentValue();

            auto J = (2.0*b*t*t*t+(d-s)*t*t*t)/3.0;

            //For now assume that only concrete contributes to torsional stiffness
            auto theEParameter = theMaterialKnowledge->getYoungsModulus();
            auto theNuParameter = theMaterialKnowledge->getPoissonsRatio();
            auto G = theEParameter->getCurrentValue()/(2.0*(1.0+theNuParameter->getCurrentValue()));

            return G*J;

        };
        theGJParameter->setCurrentValue(GJValue);
    }

    return theGJParameter;
}


RParameter* RWFibreSectionKnowledge::getArea()
{
    if(theAreaParameter == nullptr)
    {
        theDomain->createObject(this->objectName() + "AreaLambdaParameter", "RConstant");
        theAreaParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        //Area - always initialized in component constructor since used across many processes
        auto AreaValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto webHeight = theWebHeight->getCurrentValue();
            auto webThickness = theWebThickness->getCurrentValue();

            auto flangeWidth = theFlangeWidth->getCurrentValue();
            auto flangeThickness = theFlangeThickness->getCurrentValue();

            return webHeight * webThickness + 2.0 * flangeWidth * flangeThickness;
        };
        theAreaParameter->setCurrentValue(AreaValue);
    }

    return theAreaParameter;
}


std::shared_ptr<std::vector<RSectionForceDeformation*>> RWFibreSectionKnowledge::getTheSectionForceDeformation(const QString& sectionID)
{
    //       6
    //  1 * * * * *2
    //       *
    //       *
    //       *
    //  3 * * * * *4
    //       5

    if(theMaterialKnowledge == nullptr)
    {
        qDebug()<<"Could not get a material knowledge class in "<<__FUNCTION__;
        return nullptr;
    }


    if(theFlangeWidth == nullptr || theFlangeThickness == nullptr || theWebHeight == nullptr || theWebThickness == nullptr)
    {
        qDebug()<<"One or more geometry parameters are null in "<<__FUNCTION__;
        return nullptr;
    }

    auto sections = std::make_shared<std::vector<RSectionForceDeformation*>>(numSections);

    // Create the mesh generator for meshing the sections
    RFibreSectionMeshGenerator theMeshGenerator(theDomain);

    // Store the cross-section point coordinates as lambdas, so if the width or height parameters change, the points will be automatically updated
    R2DPoint point1;        R2DPoint point3;        R2DPoint point5;
    R2DPoint point2;        R2DPoint point4;        R2DPoint point6;

    std::function<double(void)> point1YValue = [&](void){return -theFlangeWidth->getCurrentValue()/2.0;};
    std::function<double(void)> point1ZValue = [&](void){return theWebHeight->getCurrentValue()/2.0;};

    std::function<double(void)> point2YValue = [&](void){return theFlangeWidth->getCurrentValue()/2.0;};
    std::function<double(void)> point2ZValue = [&](void){return theWebHeight->getCurrentValue()/2.0;};

    std::function<double(void)> point3YValue = [&](void){return -theFlangeWidth->getCurrentValue()/2.0;};
    std::function<double(void)> point3ZValue = [&](void){return -theWebHeight->getCurrentValue()/2.0;};

    std::function<double(void)> point4YValue = [&](void){return theFlangeWidth->getCurrentValue()/2.0;};
    std::function<double(void)> point4ZValue = [&](void){return -theWebHeight->getCurrentValue()/2.0;};

    std::function<double(void)> point5YValue = [&](void){return 0.0;};
    std::function<double(void)> point5ZValue = [&](void){return -theWebHeight->getCurrentValue()/2.0;};

    std::function<double(void)> point6YValue = [&](void){return 0.0;};
    std::function<double(void)> point6ZValue = [&](void){return theWebHeight->getCurrentValue()/2.0;};

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
    // Point 5
    point5.setYCoordinateValue(point5YValue);
    point5.setZCoordinateValue(point5ZValue);
    // Point 6
    point6.setYCoordinateValue(point6YValue);
    point6.setZCoordinateValue(point6ZValue);


    // Create torisonal stiffness for this section
    theDomain->createObject("TorsionalMaterial" + this->objectName() + sectionID, "RElasticMaterial");
    auto torsionalMat = qobject_cast<RElasticMaterial*>(theDomain->getLastAddedObject());

    torsionalMat->setTheEParameter(this->getGJ());

    // Create the sections
    for (int i=0; i<numSections; ++i)
    {
        auto sectionDamageRegion = std::make_shared<RWSectionDamageRegion>();

        // Mesh out the top flange
        auto res = theMeshGenerator.meshPlateLayer(point1,point2,theMaterialModel,sectionID,theNumFibresInFlange,
                                                   theFlangeThickness,&sectionDamageRegion->flangeTop.getAllFibres(),theMaterialKnowledge);

        if(res != 0)
        {
            qDebug()<<"Error in meshing out the steel fibers in "<<__FUNCTION__;
            return nullptr;
        }

        // Mesh out the bottom flange
        auto res2 = theMeshGenerator.meshPlateLayer(point3,point4,theMaterialModel,sectionID,theNumFibresInFlange,
                                                    theFlangeThickness,&sectionDamageRegion->flangeBottom.getAllFibres(),theMaterialKnowledge);

        if(res2 != 0)
        {
            qDebug()<<"Error in meshing out the steel fibers in "<<__FUNCTION__;
            return nullptr;
        }

        // Mesh out the web
        auto res3 = theMeshGenerator.meshPlateLayer(point5,point6,theMaterialModel,sectionID,theNumFibresInWeb,
                                                    theWebThickness,&sectionDamageRegion->web.getAllFibres(),theMaterialKnowledge);

        if(res3 != 0)
        {
            qDebug()<<"Error in meshing out the costeelncrete fibers in "<<__FUNCTION__;
            return nullptr;
        }


        auto fibers = sectionDamageRegion->getAllFibres();

        auto sectionName = QString("Section%1ofComponent%2").arg(i).arg(this->objectName()) + sectionID;
        theDomain->createObject(this->objectName() + sectionName, "RFiberSection3d");
        auto theRSection = qobject_cast<RFiberSection3d *>(theDomain->getLastAddedObject());

        // Set the fiber array
        theRSection->setFibers(fibers);

        // Set the section number
        theRSection->setSectionNumber(i);

        // Set torisional stiffness
        theRSection->setTorsion(torsionalMat);

        theRSection->setTheFibreContainer(sectionDamageRegion);

        sections->at(i) = theRSection;
    }

    return sections;
}
