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

#include "RCLTMaterialKnowledge.h"
#include "RContinuousRandomVariable.h"

RCLTMaterialKnowledge::RCLTMaterialKnowledge(RDomain *theDomain,
                                             QString componentName)
    : RMaterialKnowledge(theDomain, componentName)
{   
    theYoungsModulus = nullptr;
    thePoissonsRatio = nullptr;
    theDensity = nullptr;
    theFc0 = nullptr;
    theFt0 = nullptr;
    theFv0 = nullptr;
}


RCLTMaterialKnowledge::~RCLTMaterialKnowledge()
{

}


RParameter *RCLTMaterialKnowledge::getYoungsModulus()
{
    if (theYoungsModulus == nullptr) {

        // Set the Young's modulus for CLT Panel
        theDomain->createObject(objectName() + "CLTPanelYoungsModulus", "RContinuousRandomVariable");
        RContinuousRandomVariable *youngsModulus = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        youngsModulus->setDistributionType(QString("Lognormal (zeta, sigma)"));
        youngsModulus->setMean(QString("10.62e9"));
        youngsModulus->setCurrentValue(10.62e9);
        youngsModulus->setCoefficientOfVariation("0.15");

        theYoungsModulus = youngsModulus;
        theParameterList<<theYoungsModulus;
    }

    return theYoungsModulus;
}


RParameter *RCLTMaterialKnowledge::getPoissonsRatio()
{
    if (thePoissonsRatio == nullptr) {

        // Set the Poissons Ratio for CLT Panel
        theDomain->createObject(objectName() + "CLTPanelPoissonRatio", "RContinuousRandomVariable");
        RContinuousRandomVariable *poissonRatio = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        poissonRatio->setDistributionType(QString("Lognormal (zeta, sigma)"));
        poissonRatio->setMean(QString("0.25"));
        poissonRatio->setCurrentValue(0.25);
        poissonRatio->setCoefficientOfVariation("0.05");

        thePoissonsRatio = poissonRatio;
        theParameterList<<thePoissonsRatio;
    }

    return thePoissonsRatio;
}


RParameter *RCLTMaterialKnowledge::getDensity()
{
    if (theDensity == nullptr) {

        // Set the mass density for CLT Panel
        theDomain->createObject(objectName() + "CLTPanelDensity", "RContinuousRandomVariable");
        RContinuousRandomVariable *density = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        density->setDistributionType(QString("Lognormal (zeta, sigma)"));
        density->setMean(QString("600.0"));
        density->setCurrentValue(600.0);
        density->setCoefficientOfVariation("0.10");

        theDensity = density;
        theParameterList<<theDensity;
    }

    return theDensity;
}


RParameter* RCLTMaterialKnowledge::getFc0()
{
    if (theFc0 == nullptr) {

        // Set the compressive strength for CLT Panel
        theDomain->createObject(objectName() + "CLTPanelfc0", "RContinuousRandomVariable");
        theFc0 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theFc0->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theFc0->setMean(QString("41.64e6"));
        theFc0->setCurrentValue(41.64e6);
        theFc0->setCoefficientOfVariation("0.10");

        theParameterList<<theFc0;
    }

    return  theFc0;
}


RParameter* RCLTMaterialKnowledge::getFt0()
{
    if (theFt0 == nullptr) {

        // Set the tensile strength  for CLT Panel
        theDomain->createObject(objectName() + "CLTPanelft0", "RContinuousRandomVariable");
        theFt0 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theFt0->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theFt0->setMean(QString("25.44e6"));
        theFt0->setCurrentValue(25.44e6);
        theFt0->setCoefficientOfVariation("0.10");

        theParameterList<<theFt0;
    }

    return  theFt0;
}


RParameter* RCLTMaterialKnowledge::getFv0()
{
    if (theFv0 == nullptr) {

        // Set the shear strength for CLT Panel
        theDomain->createObject(objectName() + "CLTPanelfv0", "RContinuousRandomVariable");
        theFv0 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theFv0->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theFv0->setMean(QString("0.93e6"));
        theFv0->setCurrentValue(0.93e6);
        theFv0->setCoefficientOfVariation("0.10");

        theParameterList<<theFv0;
    }

    return  theFv0;
}

