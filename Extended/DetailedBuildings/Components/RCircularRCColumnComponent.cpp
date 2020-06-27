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

#include "RCircularRCColumnComponent.h"
#include "RMeshGenerator.h"
#include "RElement.h"
#include "RPoint.h"
#include "RContinuousRandomVariable.h"
#include "RConstant.h"
#include "RMathUtils.h"
#include "RLinearElasticBeamColumnMesh.h"
#include "RConcreteMaterialKnowledge.h"
#include "RCircularLinearElasticSectionKnowledge.h"
#include "math.h"

RCircularRCColumnComponent::RCircularRCColumnComponent(QObject *parent, QString &name)
    : RColumnComponent(parent, name), RConcreteComponent()
{
    applyEarthquake = true;

    theMaterial = nullptr;
    theDiameter = nullptr;

    // Initialize some default parameters for this component
    theConcreteStrength = "C20";
    longitudinalRebarDesignation = "15M";
    transverseRebarDesignation  = "10M";

    transverseReinforcementSpacing = 0.2;

    //Note: We can set this safely since its a round section, whatever orientation it is in does not matter.
    auto theOrientationLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){
        return 0.0;
    };
    theOrientationParameter->setCurrentValue(theOrientationLambda);

}


RCircularRCColumnComponent::~RCircularRCColumnComponent()
{

}


RParameter * RCircularRCColumnComponent::getDiameter() const
{
    return theDiameter;
}


void RCircularRCColumnComponent::setDiameter(QObject *value)
{
    theDiameter = qobject_cast<RParameter*>(value);

    theDiameter->setNodalParameterFlag(true);
    addToParameterList(theDiameter);
}


int RCircularRCColumnComponent::createMesh()
{
    // Create knowledge about the material that will be used in meshing schemes below
    theMaterial = make_unique<RConcreteMaterialKnowledge>(theDomain, this->objectName(), theConcreteStrength);

    if (theGlobalMeshOption->getCurrentValue() >= 1.0)
    {
        // Create knowledge about the cross-section
        theCrossSection = make_unique<RCircularLinearElasticSectionKnowledge>(theDomain, this->objectName(), theDiameter);

        // Create the finite element mesh
        theMesh = make_unique<RLinearElasticBeamColumnMesh>(theDomain, this->objectName(), theMaterial.get(), theCrossSection.get(), thePoint1, thePoint2, theOrientationParameter, theParameterList, theElementList);
    }

    meshWasCreated = true;

    return 0;
}


double RCircularRCColumnComponent::getSurfaceArea()
{
    return 3.1415 * getDiameter()->getCurrentValue() * getLength();
}


double RCircularRCColumnComponent::getManufacturingCost()
{
    return getVolumeOfConcrete()*500 + getVolumeOfSteel()*5000;
}


double RCircularRCColumnComponent::getNumberOfWorkerHoursForConstruction()
{
    return 7.0;
}


double RCircularRCColumnComponent::getVolumeOfConcrete()
{    
    return this->getVolume() - this->getVolumeOfSteel();
}


double RCircularRCColumnComponent::getVolumeOfSteel()
{
    return (theLongitudinalReinforcementRatioParameter->getCurrentValue() + theLongitudinalReinforcementRatioParameter->getCurrentValue())*this->getVolume();
}


double RCircularRCColumnComponent::getSurfaceAreaCrossSection()
{
    return PI*theDiameter->getCurrentValue()*theDiameter->getCurrentValue()/4.0;
}


double RCircularRCColumnComponent::getVolume()
{
    return theLengthParameter->getCurrentValue()*this->getSurfaceAreaCrossSection();
}


double  RCircularRCColumnComponent::getAxialLoad()
{
    //Get the axial load of the first element
    return theMesh->getAxialLoad(0);
}


double RCircularRCColumnComponent::getRepairCost()
{
    double theDrift = getDrift();
    if (theDrift < 0.01)
    {
        return 0.0;
    }
    else if (theDrift < 0.05)
    {
        return getConstructionCost()*theDrift/0.05;
    }
    else
    {
        return getConstructionCost();
    }
}


QList<QPair<double, QString> > RCircularRCColumnComponent::getTriggerTimes(double startTime, double endTime)
{
    // Clear this class variable list
    repairTimes.clear();


    // Get the value of the depth of the cover concrete
    double concreteCoverVal = concreteCoverThickness->getCurrentValue();


    // Error if there is no cover
    if (concreteCoverVal <= 0.0) {
        qCritical() << this->objectName() << "doesn't have any cover concrete depth";
        return repairTimes;
    }


    // Prompt repairs after 30 years if the cover depth is 0.02 metres and scale linearly from there
    double timeIncrement = (30*365.0) * concreteCoverVal/0.02;


    // Add repair trigger times within the period from startTime to endTime
    double newTriggerTime = startTime + timeIncrement;
    while (newTriggerTime < endTime)
    {
        repairTimes.append(qMakePair(newTriggerTime, this->objectName() + QString("ProactiveRepairs")));
        newTriggerTime += timeIncrement;
    }

    return repairTimes;
}


double RCircularRCColumnComponent::getPreventativeRepairCost()
{
    bool repair = false;

    // Check list of repair times
    for (int i=0; i<repairTimes.size(); i++)
    {
        if (fabs(repairTimes.at(i).first - theCurrentTime->getCurrentValue()) < 1.0/(24.0*60.0))
        {
            repair = true;
        }
    }
    if (repair)
    {
        return 0.05 * getConstructionCost();
    }
    else
    {
        return 0.0;
    }
}


