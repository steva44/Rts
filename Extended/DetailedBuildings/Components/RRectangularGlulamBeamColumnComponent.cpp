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

#include "RRectangularGlulamBeamColumnComponent.h"
#include "RElement.h"
#include "RPoint.h"
#include "RConstant.h"
#include "RRectangularLinearElasticSectionKnowledge.h"
#include "RRectangularFibreSectionKnowledge.h"
#include "RLinearElasticBeamColumnMesh.h"
#include "RGlulamMaterialKnowledge.h"
#include "RFrameDamageMesh.h"
#include "RNonlinearBeamColumnMesh.h"

RRectangularGlulamBeamColumnComponent::RRectangularGlulamBeamColumnComponent(QObject *parent, QString &name)
    : RColumnComponent(parent, name)
{
    applyEarthquake = true;
    evaluateDamage = true;

    theGlulamMaterial = nullptr;
    theCrossSectionWidthParameter = nullptr;
    theCrossSectionDepthParameter = nullptr;

    // Number of elements and sections
    numElements = 4;
    numSections = 4;

    // Number of fibres along the cross-section
    numFibresAlong12 = 4;
    numFibresAlong23 = 4;

    // Create random variables
    theDomain->createObject(objectName() + "Theta1", "RContinuousRandomVariable");
    theTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta1->setDistributionType(QString("Normal (mean, stdv)"));
    theTheta1->setMean(QString("1.12185"));
    theTheta1->setCoefficientOfVariation("0.0212455 ");

    theDomain->createObject(objectName() + "Theta2", "RContinuousRandomVariable");
    theTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta2->setDistributionType(QString("Normal (mean, stdv)"));
    theTheta2->setMean(QString("13.4026"));
    theTheta2->setCoefficientOfVariation("0.010852");

    theDomain->createObject(objectName() + "Epsilon", "RContinuousRandomVariable");
    theEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
    theEpsilon->setMean(QString("0.0"));
    theEpsilon->setStandardDeviation("0.161562");

    // This overrides the global mesh option
    // Set local mesh option to zero to use global mesh option
    // Local mesh options:
    //      1) Linear-elastic mesh
    //      2) Fibre discretized displacement based elements
    //      3) Fibre discretized force based elements
    localMeshOption = 2;
}


RRectangularGlulamBeamColumnComponent::~RRectangularGlulamBeamColumnComponent()
{

}


void RRectangularGlulamBeamColumnComponent::setCrossSectionWidth(QObject *value)
{
    theCrossSectionWidthParameter = qobject_cast<RParameter*>(value);
    theCrossSectionWidthParameter->setNodalParameterFlag(true);
    addToParameterList(theCrossSectionWidthParameter);
}


void RRectangularGlulamBeamColumnComponent::setCrossSectionDepth(QObject *value)
{
    theCrossSectionDepthParameter = qobject_cast<RParameter*>(value);
    theCrossSectionDepthParameter->setNodalParameterFlag(true);
    addToParameterList(theCrossSectionDepthParameter);
}


RGlulamMaterialKnowledge* RRectangularGlulamBeamColumnComponent::getTheGlulamMaterial(void) const
{
    return theGlulamMaterial.get();
}


int RRectangularGlulamBeamColumnComponent::createMesh()
{

    if (thePoint1 == nullptr || thePoint2 == nullptr || theCrossSectionWidthParameter->getCurrentValue() == 0.0 || theCrossSectionDepthParameter->getCurrentValue() == 0.0 || theOrientationParameter->getCurrentValue() == -123.0)
        return 0;

    // Re-meshing is too risky... (what if we delete shared nodes?)  Hence, just say:
    if (meshWasCreated == true) {
        qCritical() << "Changing the property of a component that has been meshed is risky! Instead, please reload the input file with the new values.";
        return -1;
    }

    // Create knowledge for the concrete material
    theGlulamMaterial = make_unique<RGlulamMaterialKnowledge>(theDomain, this->objectName());

    // 1) Linear-elastic mesh
    if (localMeshOption == 1)
    {
        // Create knowledge about the cross-section
        theCrossSection = make_unique<RRectangularLinearElasticSectionKnowledge>(theDomain, this->objectName(), theCrossSectionDepthParameter, theCrossSectionWidthParameter);

        // Create the finite element mesh
        theMesh = make_unique<RLinearElasticBeamColumnMesh>(theDomain,this->objectName(),theGlulamMaterial.get(),
                                                            theCrossSection.get(),thePoint1,thePoint2,theOrientationParameter,
                                                            theParameterList,theElementList);

        auto res = theMesh->createMesh();

        if(res!= 0)
        {
            qCritical()<<"Error: creating mesh in "<<this->objectName();
            return -1;
        }

        //In linear elastic mesh, the number of elements should be one
        numElements = 1;


    }
    // 2) Fibre discretized displacement based elements
    // 3) Fibre discretized force based elements
    else if (localMeshOption == 2 || localMeshOption == 3)
    {

        QString theMaterialModel = "RElasticBilin";

        theCrossSection = make_unique<RRectangularFibreSectionKnowledge>(theDomain,this,numSections,numFibresAlong12,numFibresAlong23,theMaterialModel);

        QString elementType;

        if(localMeshOption == 2)
            elementType = "RDispBeamColumn3d";
        else
            elementType = "RForceBeamColumn3d";

        theMesh = make_unique<RNonlinearBeamColumnMesh>(theDomain, this->objectName(), elementType, theElementList,theParameterList,numElements,theCrossSection.get(),
                                                        thePoint1,thePoint2,theOrientationParameter);

        auto beamColMesh = qobject_cast<RNonlinearBeamColumnMesh*>(theMesh.get());

        beamColMesh->setTheRhoParameter(theGlulamMaterial->getDensity());

        //For columns set coordinate transformation as P-Delta
        beamColMesh->setCrdTransformationType("PDeltaCrdTransf3d");

        beamColMesh->setIntegrationType(this->getTheIntegrationType());

        auto res = theMesh->createMesh();

        if(res!= 0 || theElementList.isEmpty())
        {
            qCritical()<<"Error: creating mesh in "<<this->objectName();
            return -1;
        }
    }
    else
    {
        return 0;
    }

    // Create the visual damage mesh
    if(theDamageMesh)
        theDamageMesh.reset(nullptr);

    theDamageMesh = make_unique<RFrameDamageMesh>(this);

    auto res2 = theDamageMesh->createDamageMesh();
    if(res2 != 0)
    {
        qCritical() << "Failed to create damage mesh in "<<this->objectName();
        return res2;
    }

    meshWasCreated = true;


    return 0;
}


double RRectangularGlulamBeamColumnComponent::getConstructionCost()
{
    double theta1 = theTheta1->getCurrentValue();
    double theta2 = theTheta2->getCurrentValue();
    double epsilon = theEpsilon->getCurrentValue();

    double w = theCrossSectionWidthParameter->getCurrentValue();
    double d = theCrossSectionDepthParameter->getCurrentValue();
    double h = theLengthParameter->getCurrentValue();

    // Gurvinder developed this model for circular timber columns, with diameter d, hence this temp fix:
    double cost = theta1 * h + theta2 * 0.5*(w+d) + epsilon;

    cost = pow(cost, 3.0);

    return cost;
}


double RRectangularGlulamBeamColumnComponent::getNumberOfWorkerHoursForConstruction()
{
    return 7.0;
}


double RRectangularGlulamBeamColumnComponent::getVolumeOfWood()
{
    return theCrossSection->getArea()->getCurrentValue()*getLength();
}


int RRectangularGlulamBeamColumnComponent::commitState()
{
    //Get the drift from the mesh
    theMesh->getDrift();

    return 0;
}




