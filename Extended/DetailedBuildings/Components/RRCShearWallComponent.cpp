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

#include "RRCShearWallComponent.h"
#include "RRCShearWallFibreSectionKnowledge.h"
#include "RRCWallLayeredShellSectionKnowledge.h"
#include "RPlateFibreWallMesh.h"
#include "RLayeredShellWallMesh.h"
#include "RLinearElasticWallMesh.h"
#include "RShellDamageMesh.h"
#include "RSectionKnowledge.h"

RRCShearWallComponent::RRCShearWallComponent(QObject *parent, QString &name)
    : RWallComponent(parent, name), RConcreteComponent()
{
    applyEarthquake = true;
    evaluateDamage = true;

    theMesh = nullptr;
    theCrossSection = nullptr;

    // Initialize the default properties of this component
    longitudinalReinforcementSpacing = 0.20;
    transverseReinforcementSpacing = 0.15;
    theConcreteStrength = "C40";
    longitudinalRebarDesignation = "20M";
    transverseRebarDesignation = "15M";


    // Layout of points that is used for meshing - each point denotes a corner of the wall
    //  4--------3
    //  |        |
    //  1--------2

    // Distributed plasticity discretization parameters
    numElements = 2;
    numSections = 4;

    // The number of fibres in the core and cover for fibre discretized cross-sections
    numFibresInCoreAlong12 = 4;
    numFibresInCoreAlong23 = 4;
    numFibresInCoverAlong12 = 2;
    numFibresInCoverAlong23 = 2;

    // Option 1 assumes confined concrete zone along the entire wall
    // Option 2 has confined zones only at the boundary ends
    reinforcementMeshOption = 1;

    // This overrides the global mesh option
    // Set local mesh option to zero to use global mesh option
    // Local mesh options:
    //      1) Linear-elastic plate mesh
    //      2) Fibre plate membrane mesh
    //      3) Layered shell element mesh
    localMeshOption = 3;

    // Default mesh discretization
    theNumElementsAlong12->setCurrentValue(6); // left-to-right
    theNumElementsAlong23->setCurrentValue(6); // down-to-up
}


RRCShearWallComponent::~RRCShearWallComponent()
{

}


int RRCShearWallComponent::createComponentSpecificParameters()
{

    if(!theTransverseReinforcementRatioParameter && transverseReinforcementSpacing == 0.0)
    {
        qCritical()<<"Either a transverse reinforcement ratio must be given or the transverse reinforcement spacing must be specified ";
        return -1;
    }

    if(!theLongitudinalReinforcementRatioParameter && longitudinalReinforcementSpacing == 0.0)
    {
        qCritical()<<"Either a longitudinal reinforcement ratio must be given or the longitudinal reinforcement spacing must be specified ";
        return -1;
    }


    if(!theTransverseReinforcementRatioParameter && transverseReinforcementSpacing != 0.0)
    {
        theDomain->createObject(objectName() + "theTransverseReinforcementRatioParameter", "RConstant");
        theTransverseReinforcementRatioParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
        auto theTransverseReinforcementRatioLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){

            auto wallWidth = this->getThickness()->getCurrentValue();;
            auto wallHeight = this->getHeightValue();

            auto numHeight = wallHeight/transverseReinforcementSpacing;

            auto reinfArea = this->getRebarArea(transverseRebarDesignation);

            //Two layers
            auto rho = 2.0*numHeight*reinfArea/(wallHeight*wallWidth);

            return rho;

        };
        theTransverseReinforcementRatioParameter->setCurrentValue(theTransverseReinforcementRatioLambda);
    }




    if(!theLongitudinalReinforcementRatioParameter && longitudinalReinforcementSpacing != 0.0)
    {
        theDomain->createObject(objectName() + "theLongitudinalReinforcementRatioParameter", "RConstant");
        theLongitudinalReinforcementRatioParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
        auto theLongitudinalReinforcementRatioLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){

            auto wallWidth = this->getThickness()->getCurrentValue();;
            auto wallLength = this->getLengthValue();

            auto numDepth= wallLength/longitudinalReinforcementSpacing;

            auto reinfArea = this->getRebarArea(longitudinalRebarDesignation);

            //Two layers
            auto rho = 2.0*numDepth*reinfArea/(wallLength*wallWidth);

            return rho;

        };
        theLongitudinalReinforcementRatioParameter->setCurrentValue(theLongitudinalReinforcementRatioLambda);
    }



    // Set the concrete cover thickness
    if(!concreteCoverThickness)
    {
        theDomain->createObject(objectName() + "CoverConcreteThickness", "RContinuousRandomVariable");
        auto theConcreteCoverThicknessRV = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theConcreteCoverThicknessRV->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theConcreteCoverThicknessRV->setMean(QString("0.025"));
        theConcreteCoverThicknessRV->setCoefficientOfVariation("0.10");
        concreteCoverThickness = qobject_cast<RParameter *>(theConcreteCoverThicknessRV);
    }

    return 0;
}


int RRCShearWallComponent::createMesh()
{

    // Do not create mesh until the points and other required parameters are set
    if (thePoint1==nullptr || thePoint2==nullptr || thePoint3==nullptr || thePoint4==nullptr || theThicknessParameter->getCurrentValue()==0.0)
        return 0;

    // Re-meshing is too risky... (what if we delete shared nodes?)  Hence, just say:
    if (meshWasCreated == true) {
        qCritical() << "Changing the property of a component that has been meshed is risky! Instead, please reload the input file with the new values.";
        return -1;
    }

    // Create the parameters that are specific to this component
    auto result = createComponentSpecificParameters();

    if(result != 0)
    {
        qCritical() << "Error creating component parameters in "<<__FUNCTION__;
        return -1;
    }

    // Create knowledge for the unconfined concrete material
    theConcreteMaterialKnowledge = make_unique<RConcreteMaterialKnowledge>(theDomain, this->objectName(), theConcreteStrength);

    // Create knowledge for the confined concrete material
    theConfinedConcreteMaterialKnowledge = make_unique<RConfinedConcreteMaterialKnowledge>(theDomain, this->objectName()+"confined", theConcreteStrength);

    // Create the rebar steel material
    theReinforcementMaterialKnowledge = make_unique<RSteelMaterialKnowledge>(theDomain, this->objectName(), "NULL");

    // Specify the type of material models
    // QString concreteMaterialModel = "RConcrete01";
    QString concreteMaterialModel = "RConcrete02";
    // QString concreteMaterialModel = "RConcrete04";

    QString steelMaterialModel = "RSteel02";

    theConfinedConcreteMaterialKnowledge->setTheConcreteMaterialModel(concreteMaterialModel);
    theConfinedConcreteMaterialKnowledge->setFyh(theReinforcementMaterialKnowledge->getYieldStress());
    theConfinedConcreteMaterialKnowledge->setRhoTrans(this->getTheTransverseReinforcementRatioParameter());
    theConcreteMaterialKnowledge->setTheConcreteMaterialModel(concreteMaterialModel);
    theReinforcementMaterialKnowledge->setTheSteelMaterialModel(steelMaterialModel);

    // Use the global mesh option only if the local mesh option is not specified
    if(localMeshOption == 0)
        localMeshOption = static_cast<int>((theGlobalMeshOption->getCurrentValue()));

    // Linear-elastic plate mesh
    if (localMeshOption == 1)
    {

        // Specify the type of shell element
        QString elementType = "RShellMITC4";

        // Create the finite element mesh -- (DDM) parameters are added to the list directly
        theMesh = make_unique<RLinearElasticWallMesh>(theDomain,
                                                      this->objectName(),
                                                      theConcreteMaterialKnowledge.get(),
                                                      thePoint1,thePoint2,thePoint3,thePoint4,
                                                      theThicknessParameter,
                                                      theNumElementsAlong12,
                                                      theNumElementsAlong23,
                                                      theParameterList,
                                                      theElementList,
                                                      elementType);

        auto res = theMesh->createMesh();
        if(res != 0)
        {
            qCritical() << "Failed to create mesh in "<<this->objectName();
            return res;
        }

        //Add the parameters from materials to the parameter list for DDM
        addToParameterList(theConcreteMaterialKnowledge->getParameterList());
        addToParameterList(theReinforcementMaterialKnowledge->getParameterList());
    }
    // Fibre plate membrane mesh
    else if (localMeshOption == 2)
    {
        // Create knowledge about the cross-section
        theCrossSection = make_unique<RRCShearWallFibreSectionKnowledge>(theDomain, this, theThicknessParameter, this->objectName()+"PlateFiberSection");


        QString elementType = "RShellMITC4";

        // Create the finite element mesh
        theMesh = make_unique<RPlateFibreWallMesh>(theDomain,this->objectName(),elementType,
                                                   theCrossSection.get(),theNumElementsAlong12,
                                                   theNumElementsAlong23,thePoint1,thePoint2,
                                                   thePoint3,thePoint4,theThicknessParameter,
                                                   theElementList);

        auto res = theMesh->createMesh();

        if(res!= 0)
        {
            qCritical()<<"Error: creating mesh in "<<this->objectName();
            return -1;
        }

    }
    // Layered shell element mesh
    else if (localMeshOption == 3)
    {

        int numCoverLayers = 1;
        int numCoreLayers = 2;

        // Create knowledge about the cross-section
        theCrossSection = make_unique<RRCWallLayeredShellSectionKnowledge>(theDomain, this, this->objectName()+"LayeredShellSection", numCoreLayers, numCoverLayers);

        QString elementType = "RShellMITC4";

        // Create the finite element mesh
        theMesh = make_unique<RLayeredShellWallMesh>(theDomain,this->objectName(),elementType,
                                                     theCrossSection.get(),theNumElementsAlong12,
                                                     theNumElementsAlong23,thePoint1,thePoint2,
                                                     thePoint3,thePoint4,theThicknessParameter,
                                                     theElementList);

        auto res = theMesh->createMesh();

        if(res!= 0)
        {
            qCritical()<<"Error: creating mesh in "<<this->objectName();
            return -1;
        }
    }


    // Create the visual damage mesh
    if(theDamageMesh)
        theDamageMesh.reset(nullptr);

    theDamageMesh = make_unique<RShellDamageMesh>(this);

    auto res2 = theDamageMesh->createDamageMesh();
    if(res2 != 0)
    {
        qCritical() << "Failed to create damage mesh in "<<this->objectName();
        return res2;
    }

    // Approve structural analysis if a mesh is created (this is a data member of the base class RComponent)
    meshWasCreated = true;

    return 0;
}


int RRCShearWallComponent::commitState()
{
    //Get the drift from the mesh
    theMesh->getDrift();

    return 0;
}


double RRCShearWallComponent::getVolumeOfSteel()
{
    return this->getVolumeofReinforcement();
}


double RRCShearWallComponent::getVolumeOfConcrete()
{
    return this->getVolume();
}


double RRCShearWallComponent::getVolumeofReinforcement()
{
    return this->getVolume()*(theTransverseReinforcementRatioParameter->getCurrentValue()+theLongitudinalReinforcementRatioParameter->getCurrentValue());
}


int RRCShearWallComponent::createTopologicalMesh(void)
{
    return theMesh->createTopologicalMesh();
}
