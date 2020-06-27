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

#include "RRCShearWallFibreSectionKnowledge.h"
#include "RConcreteComponent.h"
#include "RWallComponent.h"
#include "RPlasticDamageConcrete3d.h"
#include "RMembranePlateFiberSection.h"
#include "RPlateFromPlaneStress.h"
#include "RElasticIsotropicMaterial.h"


RRCShearWallFibreSectionKnowledge::RRCShearWallFibreSectionKnowledge(RDomain *domain,
                                                                     RWallComponent* parent,
                                                                     RParameter* thickness,
                                                                     QString objectName)
    : RSectionKnowledge(domain, objectName), theParentWall(parent), theThickness(thickness)
{


}


RRCShearWallFibreSectionKnowledge::~RRCShearWallFibreSectionKnowledge()
{

}


RSectionForceDeformation *RRCShearWallFibreSectionKnowledge::getTheSectionForceDeformation(void)
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

    // Create the plastic damage concrete 3d material
    theDomain->createObject(this->objectName()+"concretePlaneStressMat","RPlasticDamageConcrete3d");
    auto coreConcMaterial = qobject_cast<RPlasticDamageConcrete3d*>(theDomain->getLastAddedObject());
    if(coreConcMaterial == nullptr)
    {
        qDebug()<<"Could not create a material layer for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }

    // Another option is an elastic material
    // theDomain->createObject(this->objectName()+"concretePlaneStressMat","RElasticIsotropicMaterial");
    // auto coreConcMaterial = qobject_cast<RElasticIsotropicMaterial*>(theDomain->getLastAddedObject());

    // Create the core concrete layer
    theDomain->createObject(this->objectName()+"CoreConcretePlaneStress","RPlateFromPlaneStress");
    auto coreConcLayer = qobject_cast<RPlateFromPlaneStress*>(theDomain->getLastAddedObject());
    if(coreConcLayer == nullptr)
    {
        qDebug()<<"Could not create a material layer for"<<this->objectName()<<" in "<<__FUNCTION__;
        return nullptr;
    }


    // Set the out of plane shear modulus to the core concrete
    auto confShearModulus = theConfinedConcreteMatKnowledge->getShearModulus();

    coreConcLayer->setOutOfPlaneShearModulus(confShearModulus);

    // Set the plastic damage 3d material to the core layer
    coreConcLayer->setTheNDMaterial(coreConcMaterial);

    // Set the material knowledge to the material model
    coreConcMaterial->setTheMaterialKnowledge(theConfinedConcreteMatKnowledge);

    // Create the plate fibre section
    theDomain->createObject(this->objectName()+"RMembranePlateFiberSection","RMembranePlateFiberSection");
    auto theFibreSection = qobject_cast<RMembranePlateFiberSection*>(theDomain->getLastAddedObject());

    if(theFibreSection == nullptr)
    {
        qCritical()<<"Error, was not able to create a plate section from the domain in"<<__FUNCTION__;
        return nullptr;
    }

    // Set the section properties
    theFibreSection->setTheThicknessParameter(theThickness);
    theFibreSection->setTheNDMaterial(coreConcMaterial);

    return theFibreSection;
}



