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

#include "RRCWallLayeredShellSectionKnowledge.h"
#include "RConcreteMaterialKnowledge.h"
#include "RSteelMaterialKnowledge.h"
#include "RFibreSectionMeshGenerator.h"
#include "RWallComponent.h"
#include "RConcreteComponent.h"
#include "RPlateRebarMaterial.h"
#include "RPlateFromPlaneStress.h"
#include "RPlasticDamageConcrete3d.h"
#include "RNDMaterial.h"
#include "RUniaxialMaterial.h"
#include "RLayeredShellFiberSection.h"
#include "RSectionForceDeformation.h"
#include "RElasticIsotropicMaterial.h"

#include "SectionForceDeformation.h"
#include "NDMaterial.h"
#include "UniaxialMaterial.h"

RRCWallLayeredShellSectionKnowledge::RRCWallLayeredShellSectionKnowledge(RDomain *domain,
                                                                         RComponent *parentWall,
                                                                         QString objectName,
                                                                         int numCoreLayers=2,
                                                                         int numCoverLayers=1)
    : RSectionKnowledge(domain, objectName),theParentWall(parentWall),numberCoreLayers(numCoreLayers),numberCoverLayers(numCoverLayers)
{


}


RRCWallLayeredShellSectionKnowledge::~RRCWallLayeredShellSectionKnowledge()
{
}

//Mesh option 1 - where there is transvese reinforcement loops along the entire wall length as to confine the core
/*
*
*
*
*
*       /    P4*------------------------------------------^y----------------------------------------------*P3      /
*  cover|      |    UNCONFINED                            |                                               |        |
*       /      |    --------------------------------------|-------------------------------------------    |        |
*              |    |                                    z*--->x                                     |    |      Thickness
*              |    |                           CONFINED CONCRETE                                    |    |        |
*              |    ----------------------------------------------------------------------------------    |        |
*              |                                                                                          |        |
*            P1*------------------------------------------------------------------------------------------*P2      /
*
*
*/

//Mesh option 2 - where the transverse reinforcement loops are only at the boundary elements
/*
*
*
*                     l1                                                                      l2
*             \------------------\                                                    \------------------\
*
*      /    P4*------------------*P8---------------------^y---------------------------*P7----------------*P3      /
* cover|      |    UNCONFINED    |                       |                            |                  |        |
*      /      |    --------------|- - - - - - - - - - - -|- - - - - - - - - - - - - - |--------------    |        |
*             |    |  CONFINED   | *9                   z*--->x                   *10 |   CONFINED  |    |      Thickness
*             |    |  CONCRETE   |                      UNCONFINED                    |   CONCRETE  |    |        |
*             |    --------------|- - - - - - - - - - - - - - - - - - - - - - - - - - |--------------    |        |
*             |                  |                                                    |                  |        |
*           P1*------------------*P5--------------------------------------------------*P6----------------*P2      /
*
*
*
*
*  The points *9 and *10 specify the line of longitudinal reinforcement between the end cores
*
*
*/


//The layered shell looks something like this...
//********* Cover Concrete Layer *********
//********* Longitudinal Reinf Layer *********
//********* Transverse Reinf Layer *********
//********* Core Concrete Layer i *********
//********* Core Concrete Layer i+1 *********
//********* Core Concrete Layer  : *********
//********* Core Concrete Layer  m *********
//********* Longitudinal Reinf Layer *********
//********* Transverse Reinf Layer *********
//********* Cover Concrete Layer *********

RSectionForceDeformation *RRCWallLayeredShellSectionKnowledge::getTheSectionForceDeformation(void)
{

    auto theConcreteComponent = dynamic_cast<RConcreteComponent*>(theParentWall);
    auto theWallComponent = qobject_cast<RWallComponent*>(theParentWall);

    if(theConcreteComponent == nullptr || theWallComponent == nullptr)
    {
        qDebug()<<this->objectName()<<" needs to be both a wall and a concrete component "<<__FUNCTION__;
        return nullptr;
    }

    auto theUnconfinedConcreteMatKnowledge = theConcreteComponent->getTheConcreteMaterialKnowledge();
    auto theConfinedConcreteMatKnowledge = theConcreteComponent->getTheConfinedConcreteMaterialKnowledge();
    auto theReinfMaterialKnowledge = theConcreteComponent->getTheReinforcementMaterialKnowledge();

    if(theUnconfinedConcreteMatKnowledge == nullptr || theReinfMaterialKnowledge == nullptr || theConfinedConcreteMatKnowledge == nullptr)
    {
        qDebug()<<"Could not get a material knowledge class in "<<__FUNCTION__;
        return nullptr;
    }

    auto theCoverThickness = theConcreteComponent->getConcreteCoverThickness();

    if(theCoverThickness == nullptr)
    {
        qDebug()<<this->objectName()<<" needs a cover thickness parameter in "<<__FUNCTION__;
        return nullptr;
    }

    auto theWallThickness = theWallComponent->getThickness();

    if(theWallThickness == nullptr)
    {
        qDebug()<<this->objectName()<<" needs a wall thickness parameter in "<<__FUNCTION__;
        return nullptr;
    }

    if(theCoverThickness->getCurrentValue() == 0.0 || theWallThickness->getCurrentValue() == 0.0)
    {
        qDebug()<<this->objectName()<<" has one or more parameters that have a zero value; check them! "<<__FUNCTION__;
        return nullptr;
    }


    QString confinedConcreteMaterialModel = theConfinedConcreteMatKnowledge->getTheConcreteMaterialModel();
    QString unconfinedConcreteMaterialModel = theUnconfinedConcreteMatKnowledge->getTheConcreteMaterialModel();
    QString steelMaterialModel = theReinfMaterialKnowledge->getTheSteelMaterialModel();

    //First we need to create the layers
    //The wall will consist of two boundary elements and a middle section

    //************************ Step 1) create the materials that will be used for the reinforcement and concrete layers************************

    //First the uniaxial materials
    theDomain->createObject(this->objectName()+"ReinfMaterial",steelMaterialModel);
    auto reinfMaterial = qobject_cast<RUniaxialMaterial*>(theDomain->getLastAddedObject());
    if(reinfMaterial == nullptr)
    {
        qDebug()<<"Could not create a steel material for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    reinfMaterial->setMaterialKnowledge(theReinfMaterialKnowledge);

    theDomain->createObject(this->objectName()+"ConfinedConcreteMaterial",confinedConcreteMaterialModel);
    auto ccMaterial = qobject_cast<RUniaxialMaterial*>(theDomain->getLastAddedObject());
    if(ccMaterial == nullptr)
    {
        qDebug()<<"Could not create a confined concrete material for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    ccMaterial->setMaterialKnowledge(theConfinedConcreteMatKnowledge);

    theDomain->createObject(this->objectName()+"UnconfinedConcreteMaterial",unconfinedConcreteMaterialModel);
    auto ucMaterial = qobject_cast<RUniaxialMaterial*>(theDomain->getLastAddedObject());
    if(ucMaterial == nullptr)
    {
        qDebug()<<"Could not create an unconfined concrete material for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    ucMaterial->setMaterialKnowledge(theUnconfinedConcreteMatKnowledge);

    //************************Step 2) create the layers************************

    //******** the longitudinal reinforcement layer ********
    theDomain->createObject(this->objectName()+"LongReinfLayer","RPlateRebarMaterial");
    auto longReinfLayer = qobject_cast<RPlateRebarMaterial*>(theDomain->getLastAddedObject());
    if(longReinfLayer == nullptr)
    {
        qDebug()<<"Could not create a material layer for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    auto longReinfAngle = theConcreteComponent->getLongitudinalReinforcementAngle();

    longReinfLayer->setTheUniaxialMaterial(reinfMaterial);

    //The angle in degrees, positive from the x-y plane. Eg. transverse reinf angle is typically 0 while longitudinal is 90
    longReinfLayer->setReinforcementAngle(longReinfAngle);

    //******** the transverse reinforcement layer ********
    theDomain->createObject(this->objectName()+"TransReinfLayer","RPlateRebarMaterial");
    auto transReinfLayer = qobject_cast<RPlateRebarMaterial*>(theDomain->getLastAddedObject());
    if(transReinfLayer == nullptr)
    {
        qDebug()<<"Could not create a material layer for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    auto transReinfAngle = theConcreteComponent->getLongitudinalReinforcementAngle();

    transReinfLayer->setTheUniaxialMaterial(reinfMaterial);

    //The angle in degrees, positive from the x-y plane. Eg. transverse reinf angle is typically 0 while longitudinal is 90
    transReinfLayer->setReinforcementAngle(transReinfAngle);

    //******** the core concrete layer ********
    theDomain->createObject(this->objectName()+"CoreConcretePlaneStressMat","RPlasticDamageConcrete3d");
    auto coreConcMaterial = qobject_cast<RPlasticDamageConcrete3d*>(theDomain->getLastAddedObject());

    //    theDomain->createObject(this->objectName()+"CoreConcretePlaneStressMat","RElasticIsotropicMaterial");
    //    auto coreConcMaterial = qobject_cast<RElasticIsotropicMaterial*>(theDomain->getLastAddedObject());

    if(coreConcMaterial == nullptr)
    {
        qDebug()<<"Could not create a material layer for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    coreConcMaterial->setTheMaterialKnowledge(theConfinedConcreteMatKnowledge);

    //Now create the core concrete layer
    theDomain->createObject(this->objectName()+"CoreConcretePlaneStress","RPlateFromPlaneStress");
    auto coreConcLayer = qobject_cast<RPlateFromPlaneStress*>(theDomain->getLastAddedObject());
    if(coreConcLayer == nullptr)
    {
        qDebug()<<"Could not create a material layer for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    //Set the out of plane shear modulus for core concrete
    auto confShearModulus = theConfinedConcreteMatKnowledge->getShearModulus();

    //    qDebug()<<"Confined Shear modulus"<<confShearModulus->getCurrentValue();

    coreConcLayer->setOutOfPlaneShearModulus(confShearModulus);

    //Add the plastic damage 3d material to the core layer
    coreConcLayer->setTheNDMaterial(coreConcMaterial);

    //******** the cover concrete layer ********
    theDomain->createObject(this->objectName()+"CoverConcreteMaterial","RElasticIsotropicMaterial");
    auto coverConcMat = qobject_cast<RElasticIsotropicMaterial*>(theDomain->getLastAddedObject());
    if(coverConcMat == nullptr)
    {
        qDebug()<<"Could not create a material layer for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    //Set the concrete material knowledge to the cover concrete material
    coverConcMat->setTheMaterialKnowledge(theUnconfinedConcreteMatKnowledge);

    theDomain->createObject(this->objectName()+"CoverConcretePlaneStress","RPlateFromPlaneStress");
    auto coverConcLayer = qobject_cast<RPlateFromPlaneStress*>(theDomain->getLastAddedObject());
    if(coverConcLayer == nullptr)
    {
        qDebug()<<"Could not create a material layer for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    //Set the material to the cover concrete
    coverConcLayer->setTheNDMaterial(coverConcMat);

    //Set the out of plane shear modulus for cover concrete - need a way to calculate this and set it as a parameter
    auto unconfShearModulus = theUnconfinedConcreteMatKnowledge->getShearModulus();
    coverConcLayer->setOutOfPlaneShearModulus(unconfShearModulus);


    //************************Step 3) create a section and assign the layers************************

    //Create the layered shell section
    theDomain->createObject(this->objectName()+"LayeredShellSection","RLayeredShellFiberSection");
    auto theLayeredShellSect = qobject_cast<RLayeredShellFiberSection*>(theDomain->getLastAddedObject());
    if(theLayeredShellSect == nullptr)
    {
        qDebug()<<"Could not create a layered shell section for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    //Two ways that reinforcement can be specified: as spacing or as a ratio
    auto longReinfRatioParam = theConcreteComponent->getTheLongitudinalReinforcementRatioParameter();
    auto transReinfRatioParam = theConcreteComponent->getTheTransverseReinforcementRatioParameter();

    //Set the layers to the section
    theLayeredShellSect->setTheCoverLayerConcreteMaterial(coverConcLayer);
    theLayeredShellSect->setTheCoreLayerConcreteMaterial(coreConcLayer);
    theLayeredShellSect->setTheLongLayerReinfMaterial(longReinfLayer);
    theLayeredShellSect->setTheTransLayerReinfMaterial(transReinfLayer);

    //Set the number of cover and core layers
    theLayeredShellSect->setNumCoreLayers(numberCoreLayers);
    theLayeredShellSect->setNumCoverLayers(numberCoverLayers);

    //Set the parameters that determine the thicknesses of the various layers
    theLayeredShellSect->setTheShellThickness(theWallThickness);
    theLayeredShellSect->setTheCoverThickness(theCoverThickness);

    //auto longReinfLayerThickness = 0.002356;
    //auto TransReinfLayerThickness = 0.003023;

    theDomain->createObject(this->objectName()+"longReinfLayerThickness","RConstant");
    auto longReinfLayerThickness = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
    if(longReinfLayerThickness == nullptr)
    {
        qDebug()<<"Could not create a longReinfLayerThickness for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    theDomain->createObject(this->objectName()+"TransReinfLayerThickness","RConstant");
    auto TransReinfLayerThickness = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
    if(TransReinfLayerThickness == nullptr)
    {
        qDebug()<<"Could not create a TransReinfLayerThickness for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    //two layers so half
    auto tLong = longReinfRatioParam->getCurrentValue()*theWallThickness->getCurrentValue()*0.5;
    auto tTrans = transReinfRatioParam->getCurrentValue()*theWallThickness->getCurrentValue()*0.5;


    longReinfLayerThickness->setCurrentValue(tLong);
    TransReinfLayerThickness->setCurrentValue(tTrans);

    theLayeredShellSect->setTheLongReinfLayerThickness(longReinfLayerThickness);
    theLayeredShellSect->setTheTransReinfLayerThickness(TransReinfLayerThickness);

    return theLayeredShellSect;
}


int RRCWallLayeredShellSectionKnowledge::getNumCoverLayers() const
{
    return numberCoreLayers;
}


void RRCWallLayeredShellSectionKnowledge::setNumCoverLayers(int value)
{
    numberCoreLayers = value;
}







