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

#include "RConcreteComponent.h"
#include "RParameter.h"
#include "RDomain.h"

RConcreteComponent::RConcreteComponent()
{
    theConcreteStrength = "NULL";

    longitudinalRebarDesignation = "NULL";
    transverseRebarDesignation = "NULL";

    transverseReinforcementSpacing = 0.0;
    longitudinalReinforcementSpacing = 0.0;

    longitudinalReinforcementAngle = 90.0;
    transverseReinforcementAngle = 0.0;

    theConcreteMaterialKnowledge = nullptr;
    theConfinedConcreteMaterialKnowledge = nullptr;
    theReinforcementMaterialKnowledge = nullptr;

    concreteCoverThickness = nullptr;
    theLongitudinalReinforcementRatioParameter = nullptr;
    theTransverseReinforcementRatioParameter = nullptr;

}


RConcreteComponent::~RConcreteComponent()
{

}


int RConcreteComponent::setConcreteStrength(const QString& value)
{
    if (value=="C15")
        theConcreteStrength = value;
    else if (value=="C20")
        theConcreteStrength = value;
    else if (value=="C25")
        theConcreteStrength = value;
    else if (value=="C30")
        theConcreteStrength = value;
    else if (value == "C35")
        theConcreteStrength = value;
    else if (value == "C40")
        theConcreteStrength = value;
    else if (value == "C45")
        theConcreteStrength = value;
    else
    {
        theConcreteStrength = "C20";
        qDebug()<<"Default";
        return -1;
    }

    return 0;
}


void RConcreteComponent::setConcreteCover(QObject *value)
{
    concreteCoverThickness = qobject_cast<RParameter*>(value);
    concreteCoverThickness->setNodalParameterFlag(true);
}


void RConcreteComponent::setLongitudinalReinforcementRatio(QObject* value)
{
    theLongitudinalReinforcementRatioParameter = qobject_cast<RParameter*>(value);
}


void RConcreteComponent::setTransverseReinforcementRatio(QObject* value)
{
    theTransverseReinforcementRatioParameter = qobject_cast<RParameter*>(value);
}


void RConcreteComponent::setLongitudinalRebarDesignation(const QString &value)
{
    if(value == "10M")
        longitudinalRebarDesignation = value;
    else if(value == "15M")
        longitudinalRebarDesignation = value;
    else if(value == "20M")
        longitudinalRebarDesignation = value;
    else if(value == "25M")
        longitudinalRebarDesignation = value;
    else if(value == "30M")
        longitudinalRebarDesignation = value;
    else if(value == "35M")
        longitudinalRebarDesignation = value;
    else if(value == "45M")
        longitudinalRebarDesignation = value;
    else if(value == "55M")
        longitudinalRebarDesignation = value;
    else
    {
        qDebug()<<"The longitudinal reinforcement designation of -"<<value<<"- is not recognized, setting to default value: 15M";
        longitudinalRebarDesignation = "15M";
    }
}


double RConcreteComponent::getRebarDiameter(const QString& rebarDesignation) const
{
    if(rebarDesignation == "10M")
    {
        return 0.0113;
    }
    else if(rebarDesignation == "15M")
    {
        return 0.0160;
    }
    else if(rebarDesignation == "20M")
    {
        return 0.0195;
    }
    else if(rebarDesignation == "25M")
    {
        return 0.0252;
    }
    else if(rebarDesignation == "30M")
    {
        return 0.0299;
    }
    else if(rebarDesignation == "35M")
    {
        return 0.0357;
    }
    else if(rebarDesignation == "45M")
    {
        return 0.0437;
    }
    else if(rebarDesignation == "55M")
    {
        return 0.0564;
    }
    else
    {
        return 0.0;
    }
}


double RConcreteComponent::getRebarArea(const QString& rebarDesignation) const
{
    if(rebarDesignation == "10M")
    {
        return 0.000100;
    }
    else if(rebarDesignation == "15M")
    {
        return  0.000200;
    }
    else if(rebarDesignation == "20M")
    {
        return  0.000300;
    }
    else if(rebarDesignation == "25M")
    {
        return  0.000500;
    }
    else if(rebarDesignation == "30M")
    {
        return  0.000700;
    }
    else if(rebarDesignation == "35M")
    {
        return  0.0001000;
    }
    else if(rebarDesignation == "45M")
    {
        return  0.0001500;
    }
    else if(rebarDesignation == "55M")
    {
        return  0.0002500;
    }
    else
    {
        qDebug()<<"Reinforcement type not recognized";
        return 0.0;
    }
}


double RConcreteComponent::getConcreteStrengthValue() const
{
    if (theConcreteStrength == "C15") {
        return 15.0e6; // [Pa]
    }
    else if (theConcreteStrength == "C20") {
        return 20.0e6; // [Pa]
    }
    else if (theConcreteStrength == "C25") {
        return 25.0e6; // [Pa]
    }
    else if (theConcreteStrength == "C30") {
        return 30.0e6; // [Pa]
    }
    else if (theConcreteStrength == "C35") {
        return 35.0e6; // [Pa]
    }
    else if (theConcreteStrength == "C40") {
        return 40.0e6; // [Pa]
    }
    else if (theConcreteStrength == "C45") {
        return 45.0e6; // [Pa]
    }
    else if (theConcreteStrength == "NULL") {
        qDebug()<<"Concrete strength is not set! Got NULL";
        return 0.0;
    }
    else {
        return 0.0;
    }
}


QString RConcreteComponent::getConcreteStrength()
{
    return theConcreteStrength;
}


RParameter* RConcreteComponent::getConcreteCoverThickness() const
{
    return concreteCoverThickness;
}


RParameter* RConcreteComponent::getTheLongitudinalReinforcementRatioParameter() const
{
    return theLongitudinalReinforcementRatioParameter;
}


RParameter* RConcreteComponent::getTheTransverseReinforcementRatioParameter() const
{
    return theTransverseReinforcementRatioParameter;
}


RConcreteMaterialKnowledge* RConcreteComponent::getTheConcreteMaterialKnowledge() const
{
    return theConcreteMaterialKnowledge.get();
}


RConcreteMaterialKnowledge* RConcreteComponent::getTheConfinedConcreteMaterialKnowledge() const
{
    return theConfinedConcreteMaterialKnowledge.get();
}


RSteelMaterialKnowledge* RConcreteComponent::getTheReinforcementMaterialKnowledge() const
{
    return theReinforcementMaterialKnowledge.get();
}


double RConcreteComponent::getLongitudinalReinforcementAngle() const
{
    return longitudinalReinforcementAngle;
}


void RConcreteComponent::setLongitudinalReinforcementAngle(double value)
{
    longitudinalReinforcementAngle = value;
}


double RConcreteComponent::getTransverseReinforcementAngle() const
{
    return transverseReinforcementAngle;
}


void RConcreteComponent::setTransverseReinforcementAngle(double value)
{
    transverseReinforcementAngle = value;
}


double RConcreteComponent::getLongitudinalBarArea(void) const
{
    return this->getRebarArea(longitudinalRebarDesignation);
}


double RConcreteComponent::getTransBarArea(void) const
{
    return this->getRebarArea(transverseRebarDesignation);
}


double RConcreteComponent::getLongitudinalReinforcementSpacing() const
{
    return longitudinalReinforcementSpacing;
}


double RConcreteComponent::getTransverseReinforcementSpacing() const
{
    return transverseReinforcementSpacing;
}

void RConcreteComponent::setLongitudinalReinforcementSpacing(double value)
{
    longitudinalReinforcementSpacing = value;
}


void RConcreteComponent::setTransverseReinforcementSpacing(double value)
{
    transverseReinforcementSpacing = value;
}


QString RConcreteComponent::getLongitudinalRebarDesignation() const
{
    return longitudinalRebarDesignation;
}


QString RConcreteComponent::getTransverseRebarDesignation() const
{
    return transverseRebarDesignation;
}


double RConcreteComponent::getLongitudinalReinforcementDiameter(void)
{
    return this->getRebarDiameter(longitudinalRebarDesignation);
}


double RConcreteComponent::getTransverseReinforcementDiameter(void)
{
    return this->getRebarDiameter(transverseRebarDesignation);
}






