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
*   - Gurvinder Gill                                                 *
*   - Seadon Chui                                                    *
*                                                                    *
*********************************************************************/

#include "RSteelBarTrussComponent.h"
#include "RMeshGenerator.h"
#include "RElement.h"
#include "RPoint.h"
#include "RLinearElasticTrussMesh.h"
#include "RNonlinearTrussMesh.h"
#include "RSteelMaterialKnowledge.h"
#include "RUniaxialFibreSectionKnowledge.h"

RSteelBarTrussComponent::RSteelBarTrussComponent(QObject *parent, QString &name)
    : RTrussComponent(parent, name)
{
    applyEarthquake = true;

    theCrossSectionArea = nullptr;
    theCrossSectionKnowledge = nullptr;
    theMesh = nullptr;
    theMaterial = nullptr;

    auto theOrientationLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){
        return 0.0;
    };
    theOrientationParameter->setCurrentValue(theOrientationLambda);

    // This overrides the global mesh option
    // Set local mesh option to zero to use global mesh option
    // Local mesh options:
    //      1) Linear-elastic mesh
    //      2) Fibre discretized displacement based elements
    //      3) Fibre discretized force based elements
    localMeshOption = 1;

}


RSteelBarTrussComponent::~RSteelBarTrussComponent()
{

}


// The cross-section area in m^2
QObject* RSteelBarTrussComponent::getCrossSectionArea()
{
    return theCrossSectionArea;
}


// The cross-section area in m^2
void RSteelBarTrussComponent::setCrossSectionArea(QObject* value)
{
    auto parameter = qobject_cast<RParameter*>(value);

    theCrossSectionArea = parameter;
}


// Create the finite element mesh
int RSteelBarTrussComponent::createMesh()
{

    // Do not create mesh until the end-points and other parameters are set
    if (thePoint1 == nullptr || thePoint2 == nullptr|| theGlobalMeshOption->getCurrentValue()==0.0 || theOrientationParameter->getCurrentValue()==-123.0 || theCrossSectionArea == nullptr ) {

        qCritical()<<"Error in creating mesh for RSteelBarTrussComponent";
        return 0;
    }

    // Re-meshing is too risky... (what if we delete shared nodes?)  Hence, just say:
    if (meshWasCreated == true) {
        qCritical() << "Mesh has already been created for this component. ";
        return -1;
    }

    // Create knowledge about the material that will be used in all meshing schemes
    theMaterial = make_unique<RSteelMaterialKnowledge>(theDomain, this->objectName(), "NULL");

    // Create cross section knowledge
    theCrossSectionKnowledge  = make_unique<RGenericSectionKnowledge>(theDomain,this->objectName());
    auto genericSectionKnowledge = qobject_cast<RGenericSectionKnowledge*>(theCrossSectionKnowledge.get());
    genericSectionKnowledge->setA(theCrossSectionArea);

    // 1) Linear-elastic mesh
    if (localMeshOption == 1)
    {
        theMesh = make_unique<RLinearElasticTrussMesh>(theDomain, this->objectName(), theMaterial.get(), theCrossSectionKnowledge.get(), thePoint1, thePoint2, theOrientationParameter, theParameterList, theElementList);
    }
    // 2) Fibre discretized displacement based elements
    else if (localMeshOption == 2)
    {
        QString elementType = "RDisplacementBasedElement";

        int numElements = 1;
        int numSections = 1;

        auto uniaxialFibreSection = make_unique<RUniaxialFibreSectionKnowledge>(theDomain,this->objectName(),numSections,theMaterial.get(),theCrossSectionKnowledge.get());

        auto uniaxialSection = qobject_cast<RSectionKnowledge*>(uniaxialFibreSection.get());

        theMesh = make_unique<RNonlinearTrussMesh>(theDomain, this->objectName(), elementType, theElementList,theParameterList,numElements,uniaxialSection,
                                                   thePoint1,thePoint2,theOrientationParameter);
    }
    else
    {
        qCritical() << "Invalid mesh option given to the component named" << this->objectName();
    }

    // Approve structural analysis if a mesh is created (this is a data member of the base class RComponent)
    meshWasCreated = true;

    return 0;
}


// Volume of steel in m3
double RSteelBarTrussComponent::getVolumeOfSteel()
{
    auto length = theLengthParameter->getCurrentValue();

    auto area = theCrossSectionArea->getCurrentValue();

    return length * area;
}
