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

#include "RCompositePanelComponent.h"
#include "RLinearElasticSlabMesh.h"
#include "RCompositeDeckingMaterialKnowledge.h"
#include "RShellDamageMesh.h"

RCompositePanelComponent::RCompositePanelComponent(QObject *parent, QString &name)
    : RPlateComponent(parent, name)
{
    applyEarthquake = true;
    applySnow = true;
    evaluateDamage = true;

    theMesh = nullptr;
    theCompositeMaterial = nullptr;

    // The gauge of the steel deck
    guage = 18;

    // The depth of the decking is 3 inches, or (0.0762 m)
    deckingDepth = 0.0762;

    // The thickness of the concrete topping
    // Assume 3" thick topping  (0.0762 m)
    concreteToppingThickness = 0.0762;

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


RCompositePanelComponent::~RCompositePanelComponent()
{

}


int RCompositePanelComponent::createMesh()
{

    // Do not create mesh until the points and other required parameters are set
    if (thePoint1 == nullptr || thePoint2 == nullptr ||  thePoint3 == nullptr ||  thePoint4 == nullptr || theThicknessParameter->getCurrentValue()==0.0)
        return 0;

    // Re-meshing is too risky... (what if we delete shared nodes?)  Hence, just say:
    if (meshWasCreated == true) {
        qCritical() << "Changing the property of a component that has been meshed is risky! Instead, please reload the input file with the new values.";
        return -1;
    }

    //Check to make sure thickness is valid
    if(theThicknessParameter->getCurrentValue() <= 0.0)
    {
        qCritical()<<"The panel thickness cannot be less than or equal to zero in "<<this->objectName();
        return 0;
    }

    // Create the material knowledge
    theCompositeMaterial = make_unique<RCompositeDeckingMaterialKnowledge>(theDomain,this->objectName()+"CompMaterial");

    // Get the mesh option for the slab
    int meshOption = -1;

    // Use the global mesh option only if the local mesh option is not specified
    if(localMeshOption == 0)
        meshOption = static_cast<int>((theGlobalMeshOption->getCurrentValue()));
    else
        meshOption = localMeshOption;


    // Create the finite element mesh -- (DDM) parameters are added to the list directly
    theMesh = make_unique<RLinearElasticSlabMesh>(theDomain,this->objectName(), theCompositeMaterial.get(),
                                                  thePoint1,thePoint2,thePoint3,thePoint4,theThicknessParameter,
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

    // Add the parameters from materials to the parameter list for DDM
    addToParameterList(theCompositeMaterial->getParameterList());

    // Approve structural analysis if a mesh is created (this is a data member of the base class RComponent)
    meshWasCreated = true;

    return 0;
}


double RCompositePanelComponent::getAverageConcreteToppingThickness(void)
{
    // Since the decking is corrugated, assume a thickness of 0.4 of the pan depth
    return (0.4*deckingDepth + concreteToppingThickness);
}


double RCompositePanelComponent::getDeckingDepth() const
{
    return deckingDepth;
}


void RCompositePanelComponent::setDeckingDepth(double value)
{
    deckingDepth = value;
}


int RCompositePanelComponent::getGuage() const
{
    return guage;
}


void RCompositePanelComponent::setGuage(int value)
{
    guage = value;
}


double RCompositePanelComponent::getVolumeOfSteel()
{
    // Decking mass in kg per sq. m
    // https://www.canam-construction.com/wp-content/uploads/2014/12/canam-steel-deck-catalogue-canada.pdf
    auto massPerArea = 0.0;

    if(guage == 16)
    {
        massPerArea = 22.71;
    }
    else if(guage == 18)
    {
        massPerArea = 14.71;
    }
    else if(guage == 20)
    {
        massPerArea = 11.21;
    }
    else if(guage == 22)
    {
        massPerArea = 9.46;
    }
    else
    {
        qDebug()<<"The guage value of "<<guage<<" is not supported in "<<__FUNCTION__;
        return 0.0;
    }

    // Get the area of the component
    auto area = this->getPlanArea();

    // Get the mass of the component
    auto mass = massPerArea*area;

    // Add the mass of the steel wire mesh in the concrete->  0.012635 kg/m2
    mass += 0.012635*area;

    // Steel is 7900 kg/m3
    auto vol = mass/7900.0;

    return vol;
}


double RCompositePanelComponent::getVolumeOfConcrete()
{
    auto area = this->getPlanArea();

    auto thickness = this->getAverageConcreteToppingThickness();

    auto volume = thickness*area;

    return volume;
}


RMesh*  RCompositePanelComponent::getMesh(void)
{
    return theMesh.get();
}


int RCompositePanelComponent::commitState()
{
    return theMesh->commitState();
}


RCompositeDeckingMaterialKnowledge* RCompositePanelComponent::getTheCompositeMaterial(void) const
{
    return theCompositeMaterial.get();
}


double RCompositePanelComponent::getConcreteToppingThickness() const
{
    return concreteToppingThickness;
}


