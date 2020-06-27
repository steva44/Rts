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

#include "RRCSlabComponent.h"
#include "RLinearElasticSlabMesh.h"
#include "RShellDamageMesh.h"

RRCSlabComponent::RRCSlabComponent(QObject *parent, QString &name)
    : RPlateComponent(parent, name), RConcreteComponent()
{
    applyEarthquake = true;
    applySnow = true;
    evaluateDamage = true;

    theMesh = nullptr;

    // Along 1-2 (width) is "longitudinal"
    // Along 2-3 (depth) is "transverse"
    longitudinalReinforcementSpacing = 0.2;
    transverseReinforcementSpacing = 0.2;
    theConcreteStrength = "C35";
    longitudinalRebarDesignation = "15M";
    transverseRebarDesignation = "15M";

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


RRCSlabComponent::~RRCSlabComponent()
{

}


int RRCSlabComponent::createMesh()
{

    // Do not create mesh until the points and other required parameters are set
    if (thePoint1==nullptr || thePoint2==nullptr || thePoint3==nullptr || thePoint4==nullptr || getConcreteStrengthValue()==0.0)
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

    // Create knowledge for the concrete material
    theConcreteMaterialKnowledge = make_unique<RConcreteMaterialKnowledge>(theDomain, this->objectName(), theConcreteStrength);

    //Create the rebar steel material
    theReinforcementMaterialKnowledge = make_unique<RSteelMaterialKnowledge>(theDomain, this->objectName(), "NULL");

    // Get the mesh option for the slab
    int meshOption = -1;

    // Use the global mesh option only if the local mesh option is not specified
    if(localMeshOption == 0)
        meshOption = static_cast<int>((theGlobalMeshOption->getCurrentValue()));
    else
        meshOption = localMeshOption;

    // Create the finite element mesh -- (DDM) parameters are added to the list directly
    theMesh = make_unique<RLinearElasticSlabMesh>(theDomain,this->objectName(),theConcreteMaterialKnowledge.get(),thePoint1,thePoint2,thePoint3,thePoint4,theThicknessParameter,
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

    theDamageMesh = make_unique<RShellDamageMesh>(this);

    auto res2 = theDamageMesh->createDamageMesh();
    if(res2 != 0)
    {
        qCritical() << "Failed to create damage mesh in "<<this->objectName();
        return res2;
    }

    // Add the parameters from materials to the parameter list for DDM
    addToParameterList(theConcreteMaterialKnowledge->getParameterList());
    addToParameterList(theReinforcementMaterialKnowledge->getParameterList());
    addToParameterList(theLongitudinalReinforcementRatioParameter);
    addToParameterList(theTransverseReinforcementRatioParameter);
    addToParameterList(concreteCoverThickness);

    // Approve structural analysis if a mesh is created (this is a data member of the base class RComponent)
    meshWasCreated = true;

    return 0;
}


int RRCSlabComponent::createComponentSpecificParameters()
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

            auto slabVolume = this->getVolume();
            auto slabWidth = this->getWidth();

            auto numWidth = slabWidth/transverseReinforcementSpacing;

            auto reinfArea = this->getRebarArea(transverseRebarDesignation);

            auto rho = (numWidth*slabWidth*reinfArea)/slabVolume;

            return rho;

        };
        theTransverseReinforcementRatioParameter->setCurrentValue(theTransverseReinforcementRatioLambda);
    }


    if(!theLongitudinalReinforcementRatioParameter && longitudinalReinforcementSpacing != 0.0)
    {
        theDomain->createObject(objectName() + "theLongitudinalReinforcementRatioParameter", "RConstant");
        theLongitudinalReinforcementRatioParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
        auto theLongitudinalReinforcementRatioLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){

            auto slabVolume = this->getVolume();
            auto slabDepth = this->getDepth();

            auto numDepth= slabDepth/longitudinalReinforcementSpacing;

            auto reinfArea = this->getRebarArea(longitudinalRebarDesignation);

            auto rho = (numDepth*slabDepth*reinfArea)/slabVolume;

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
        theConcreteCoverThicknessRV->setMean(QString("0.05"));
        theConcreteCoverThicknessRV->setCoefficientOfVariation("0.10");
        concreteCoverThickness = qobject_cast<RParameter *>(theConcreteCoverThicknessRV);
    }

    return 0;
}


double RRCSlabComponent::getNumberOfWorkerHoursForConstruction()
{
    return 7.0;
}


double RRCSlabComponent::getVolumeOfConcrete()
{
    return this->getVolume();
}


double RRCSlabComponent::getVolumeOfSteel()
{
    return this->getVolumeOfReinforcement();
}


double RRCSlabComponent::getServiceabilityCost()
{
    auto serviceabilityCost =  0.0;

    auto theSlabMesh = dynamic_cast<RLinearElasticSlabMesh*>(theMesh.get());

    // Get maximum vertical deflection and compare to length of slab
    auto maxDeltaZ = theSlabMesh->getMaxVerticalDeflection();

    // qDebug()<<"The max delta z is: "<<maxDeltaZ;

    auto theConstructionCost = this->getConstructionCost();

    if(theConstructionCost == 0.0)
        qCritical()<<"Error the construction cost cannot be zero";

    auto length = this->getDepth();

    if(maxDeltaZ >= length/200.0)
        serviceabilityCost += theConstructionCost*0.20;

    if(maxDeltaZ >= length/360.0)
        serviceabilityCost += theConstructionCost*0.360;

    if(maxDeltaZ >= length/500.0)
        serviceabilityCost += theConstructionCost*0.50;

    return serviceabilityCost;
}


int RRCSlabComponent::commitState()
{

    return theMesh->commitState();
}


double RRCSlabComponent::getVolumeOfReinforcement()
{
    return (theLongitudinalReinforcementRatioParameter->getCurrentValue()+theTransverseReinforcementRatioParameter->getCurrentValue())*this->getVolume();
}


int RRCSlabComponent::createTopologicalMesh(void)
{
    theMesh->createTopologicalMesh();

    return 0;
}


double RRCSlabComponent::getRSIValue()
{
    // Get the thermal conductivity from the concrete (w/(m*deg C))
    auto thermConductivity = theConcreteMaterialKnowledge->getThermalConductivity();

    // Calculate the R value for concrete
    auto RValue = 1.0*this->getThickness()->getCurrentValue()/thermConductivity;

    // Add RValue for roof material
    if(thePlateType == plateType::ROOF)
    {
        //Asphalt, composition or prepared = 0.15 (imperial)
        RValue += 0.15* 0.1761101838;
    }

    // Add RValue from insulation
    // 1" rigid insulating board sheathing = 3.03 (imperial)
    // Convert into SI units * 0.1761101838
    RValue += 3.03 * 0.1761101838;

    return RValue;
}



