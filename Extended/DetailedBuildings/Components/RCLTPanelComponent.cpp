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

#include "RCLTPanelComponent.h"
#include "RLinearElasticSlabMesh.h"
#include "RCLTMaterialKnowledge.h"
#include "RShellDamageMesh.h"

RCLTPanelComponent::RCLTPanelComponent(QObject *parent, QString &name)
    : RPlateComponent(parent, name)
{
    applyEarthquake = true;
    applySnow = true;
    evaluateDamage = true;

    theMesh = nullptr;
    theMaterialKnowledge = nullptr;

    // RSI for roof slab energy loss calculations
    // https://www.richmond.ca/__shared/assets/part9energyefficiencyrequirementsdrawings41752.pdf
    // CATHEDRAL/FLAT ROOF: 4.67 RSI
    // RSI CALCULATION FOR COMPONENT
    // 285mm RAFTER @ 406mm O/C WITH R-31 BATT INSULATION
    // 100 / ((13% )+(87%)) 2.422 5.46
    //   4.694
    // ADDITIONAL COMPONENTS
    // EXTERIOR AIR FILM
    // 0.03 (FROM TABLE)
    // 0.03
    // 12.7mm GYPSUM WALL BOARD
    // 12.7mm x 0.0061 RSI/mm
    // 0.07747
    // INTERIOR AIR FILM
    // 0.11 (FROM TABLE)
    // 0.11
    // TOTAL RSI: 4.91

    RSIValue = 4.91;

    // This overrides the global mesh option
    // Set local mesh option to zero to use global mesh option
    localMeshOption = 1;

    // Default mesh discretization
    theNumElementsAlong12->setCurrentValue(15); // Along width
    theNumElementsAlong23->setCurrentValue(15); // Along depth
}


RCLTPanelComponent::~RCLTPanelComponent()
{

}


int RCLTPanelComponent::createMesh()
{

    // Do not create mesh until the points and other required parameters are set
    if (thePoint1==nullptr || thePoint2==nullptr || thePoint3==nullptr || thePoint4==nullptr)
        return 0;

    // Re-meshing is too risky... (what if we delete shared nodes?)  Hence, just say:
    if (meshWasCreated == true) {
        qCritical() << "Changing the property of a component that has been meshed is risky! Instead, please reload the input file with the new values.";
        return -1;
    }

    //Create the material knowledge
    theMaterialKnowledge = make_unique<RCLTMaterialKnowledge>(theDomain,this->objectName());

    // Get the mesh option for the slab
    int meshOption = -1;

    // Use the global mesh option only if the local mesh option is not specified
    if(localMeshOption == 0)
        meshOption = static_cast<int>((theGlobalMeshOption->getCurrentValue()));
    else
        meshOption = localMeshOption;


    //Create the finite element mesh -- (DDM) parameters are added to the list directly
    theMesh = make_unique<RLinearElasticSlabMesh>(theDomain,this->objectName(),theMaterialKnowledge.get(),thePoint1,thePoint2,thePoint3,thePoint4,theThicknessParameter,
                                                 theNumElementsAlong12,theNumElementsAlong23,theParameterList,theElementList,meshOption);

    auto res = theMesh->createMesh();
    if(res != 0)
    {
        qCritical() << "Failed to create mesh in "<<this->objectName();
        return res;
    }

    // Create the visual damage mesh
    if(theDamageMesh)
        theDamageMesh.reset(nullptr);

    // Create the damage mesh
    theDamageMesh = make_unique<RShellDamageMesh>(this);

    auto res2 = theDamageMesh->createDamageMesh();
    if(res2 != 0)
    {
        qCritical() << "Failed to create damage mesh in "<<this->objectName();
        return res2;
    }

    //Add the parameters from materials to the parameter list for DDM
    addToParameterList(theMaterialKnowledge->getParameterList());

    // Approve structural analysis if a mesh is created (this is a data member of the base class RComponent)
    meshWasCreated = true;

    return 0;
}


double RCLTPanelComponent::getVolumeOfWood()
{
    return getVolume();
}


int RCLTPanelComponent::commitState()
{
    return theMesh->commitState();
}


RCLTMaterialKnowledge* RCLTPanelComponent::getTheMaterialKnowledge(void) const
{
    return theMaterialKnowledge.get();
}


RMesh*  RCLTPanelComponent::getMesh(void)
{
    return theMesh.get();
}
