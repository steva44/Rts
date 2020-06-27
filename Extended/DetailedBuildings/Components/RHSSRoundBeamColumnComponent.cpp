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

#include "RHSSRoundBeamColumnComponent.h"
#include "RMeshGenerator.h"
#include "RElement.h"
#include "RPoint.h"
#include "RConstant.h"
#include "RLinearElasticBeamColumnMesh.h"
#include "RSteelMaterialKnowledge.h"

RHSSRoundBeamColumnComponent::RHSSRoundBeamColumnComponent(QObject *parent, QString &name)
    : RColumnComponent(parent, name)
{
    applyEarthquake = true;

    theWallThicknessParameter = nullptr;
    theOuterDiameterParameter = nullptr;
    theMaterial = nullptr;

    theDomain->createObject(objectName() + "theSteelMaterial", "RSteelMaterial");
    //theBuildingMaterial = qobject_cast<RSteelMaterial *>(theDomain->getLastAddedObject());
    theOuterDiameterDouble = 0.0;
    theWallThicknessDouble = 0.0;

    //Note: We can set this safely since its a round pipe, whatever orientation it is in does not matter.
    auto theOrientationLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){
        return 0.0;
    };
    theOrientationParameter->setCurrentValue(theOrientationLambda);

    //Create the parameters that are specific to this component
    theDomain->createObject(objectName() + "theOuterDiameterParameter", "RConstant");
    theOuterDiameterParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

    theDomain->createObject(objectName() + "theWallThicknessParameter", "RConstant");
    theWallThicknessParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

}


RHSSRoundBeamColumnComponent::~RHSSRoundBeamColumnComponent()
{

}


void RHSSRoundBeamColumnComponent::setWallThickness(QString value)
{
    setStringParameter(value, &theWallThicknessDouble, &theWallThicknessParameter);
    theWallThicknessParameter->setNodalParameterFlag(true);
    addToParameterList(theWallThicknessParameter);
}


void RHSSRoundBeamColumnComponent::setDiameter(QString value)
{
    setStringParameter(value, &theOuterDiameterDouble, &theOuterDiameterParameter);
    theOuterDiameterParameter->setNodalParameterFlag(true);
    addToParameterList(theOuterDiameterParameter);
}


int RHSSRoundBeamColumnComponent::createMesh()
{
    // Do not create mesh until the end-points and other geometry parameters are set
    if (thePoint1 == nullptr || thePoint2 == nullptr|| theGlobalMeshOption->getCurrentValue()==0.0 || theOrientationParameter->getCurrentValue()==-123.0 || theWallThicknessParameter->getCurrentValue()== 0.0  || theOuterDiameterParameter->getCurrentValue()== 0.0 ) {

        qCritical()<<"Error in creating mesh for RHSSRoundBeamColumnComponent";
        return 0;
    }

    // Re-meshing is too risky... (what if we delete shared nodes?)  Hence, just say:
    if (meshWasCreated == true) {
        qCritical() << "Mesh has already been created for this component. ";
        return -1;
    }

    // Check to make sure section is valid
    if(theWallThicknessParameter->getCurrentValue() <= 0.0 || theOuterDiameterParameter->getCurrentValue() <= 0.0 )
    {
        qCritical()<<"One of the section dimensions is less than or equal to zero in RHSSRoundBeamColumnComponent";
        return 0;
    }

    RMeshGenerator theMeshGenerator(theDomain);

    // Create knowledge about the material that will be used in all meshing schemes
    theMaterial = make_unique<RSteelMaterialKnowledge>(theDomain, this->objectName(), "NULL");

    // ********** ONE LINEAR ELASTIC FRAME ELEMENT **********
    if (theGlobalMeshOption->getCurrentValue() >= 1.0)
    {
        // Create knowledge about the cross-section
        theCrossSection = make_unique<RCircularHSSLinearElasticSectionKnowledge>(theDomain, this->objectName(), theOuterDiameterParameter, theWallThicknessParameter);

        // Create the finite element mesh
        theMesh = make_unique<RLinearElasticBeamColumnMesh>(theDomain, this->objectName(), theMaterial.get(), theCrossSection.get(), thePoint1, thePoint2, theOrientationParameter, theParameterList, theElementList);
    }
    //ToDo: change this code so that it creates an inelastic mesh object
    //    else if (theGlobalMeshOption->getCurrentValue() >= 5 && theGlobalMeshOption->getCurrentValue() <10)
    //    {
    //   //The number of elements along the longitudinal axis
    //   theMeshGenerator.meshLinearElement(thePoint1,thePoint2,"RDisplacementBasedElement",4,this->objectName(),theElementList,theParameterList);

    //   if(theElementList.empty())
    //   {
    //       qCritical()<<"An empty element list was returned from the mesh generator in RHssRoundBeamColumnComponent";
    //       return 0;
    //   }

    //   //Number of sections per element, number of fibres along the circumference, number of fibres along the radius
    //   int numSections = 4; int numFibAlongC = 3; int numFibAlongR = 1;

    //   auto outerRadius = 0.5*theOuterDiameterParameter->getCurrentValue();
    //   auto innerRadius = outerRadius-theWallThicknessParameter->getCurrentValue();

    //   QList<RMaterial*> theMaterialList;

    //   theMeshGenerator.meshCircularAreaSection(outerRadius,innerRadius,"R1DSteelMaterial",numSections,numFibAlongR,numFibAlongC,this->objectName(),theElementList,&theMaterialList);

    //   //now go through list and set material properties
    //   for(auto&& it: theMaterialList)
    //   {
    //       auto theSteel1DRebarMaterial = qobject_cast<R1DSteelMaterial*>(it);
    //       auto theSteelKnowledge = qobject_cast<RSteelMaterialKnowledge*>(theMaterial.get());

    //       theSteel1DRebarMaterial->setE0(theSteelKnowledge->getYoungsModulus());
    //       theSteel1DRebarMaterial->setfy(theSteelKnowledge->getYieldStress());
    //       theSteel1DRebarMaterial->setb("0.2");
    //       theSteel1DRebarMaterial->setDensity(theSteelKnowledge->getDensity());
    //   }

    //    }
    else
    {
        // Issue error message
        qCritical() << "Invalid mesh option given to the component named" << this->objectName()<<" resorting to default linear elastic mesh option";
    }


    // Approve structural analysis if a mesh is created (this is a data member of the base class RComponent)
    meshWasCreated = true;

    return 0;
}


double RHSSRoundBeamColumnComponent::getConstructionCost()
{
    // Declare the construction cost to be returned from this method
    double constructionCost = 0.0;

    // Declare model parameters
    double direct_cost_concrete_theta1 = 3.01;
    double direct_cost_concrete_theta2 = 2.67;
    double direct_cost_concrete_theta3 = 0.76;
    double direct_cost_concrete_theta4 = 33.06;
    double direct_cost_concrete_epsilon = 0.42;

    // Temporarily set a few things that the component should know
    double concrete_strength = 35.0e6;
    double reinforcement_ratio = 0.02;

    double height = sqrt( pow((thePoint1->getAssociatedNode()->getXCoordinateValue()-thePoint2->getAssociatedNode()->getXCoordinateValue()),2) + pow((thePoint1->getAssociatedNode()->getYCoordinateValue()-thePoint2->getAssociatedNode()->getYCoordinateValue()),2) + pow((thePoint1->getAssociatedNode()->getZCoordinateValue()-thePoint2->getAssociatedNode()->getZCoordinateValue()),2));

    auto diameterValue = theOuterDiameterParameter->getCurrentValue();

    double model = direct_cost_concrete_theta1 * height/3.048 +
            direct_cost_concrete_theta2 * diameterValue/(0.41) +
            direct_cost_concrete_theta3 * concrete_strength/27.58e6 +
            direct_cost_concrete_theta4 * reinforcement_ratio * diameterValue/(0.41) +
            direct_cost_concrete_epsilon;

    constructionCost = pow(model, 3.0);

    return constructionCost;
}


double RHSSRoundBeamColumnComponent::getNumberOfWorkerHoursForConstruction()
{
    return 7.0;
}


double RHSSRoundBeamColumnComponent::getVolumeOfSteel()
{
    auto height = theLengthParameter->getCurrentValue();

    auto diameterValue = theOuterDiameterParameter->getCurrentValue();

    double crossSectionArea = 3.1415 * (0.5*diameterValue*0.5*diameterValue);

    return 0.05 * height * crossSectionArea;
}

