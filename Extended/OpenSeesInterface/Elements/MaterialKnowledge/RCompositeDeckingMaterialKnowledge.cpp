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

#include "RCompositeDeckingMaterialKnowledge.h"
#include "RContinuousRandomVariable.h"

RCompositeDeckingMaterialKnowledge::RCompositeDeckingMaterialKnowledge(RDomain *theDomain,
                                                                       QString componentName)
    : RMaterialKnowledge(theDomain, componentName)
{   
    theYoungsModulus = nullptr;
    thePoissonsRatio = nullptr;
    theDensity = nullptr;
    fpc = nullptr;
    theYieldStress = nullptr;
}


RCompositeDeckingMaterialKnowledge::~RCompositeDeckingMaterialKnowledge()
{

}


RParameter *RCompositeDeckingMaterialKnowledge::getYoungsModulus()
{
    if (theYoungsModulus == nullptr) {

        // Set the Young's modulus
        theDomain->createObject(objectName() + "CompositePanelYoungsModulus", "RContinuousRandomVariable");
        RContinuousRandomVariable *youngsModulus = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        youngsModulus->setDistributionType(QString("Lognormal (zeta, sigma)"));
        youngsModulus->setMean(QString("25.0e9"));
        youngsModulus->setCurrentValue(25.0e9);      // [Pa]
        youngsModulus->setCoefficientOfVariation("0.10");

        theYoungsModulus = youngsModulus;
        theParameterList<<theYoungsModulus;
    }

    return theYoungsModulus;
}


RParameter *RCompositeDeckingMaterialKnowledge::getPoissonsRatio()
{
    if (thePoissonsRatio == nullptr) {

        // Set the Poissons ratio
        theDomain->createObject(objectName() + "CompositePanelPoissonRatio", "RContinuousRandomVariable");
        RContinuousRandomVariable *poissonRatio = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        poissonRatio->setDistributionType(QString("Lognormal (zeta, sigma)"));
        poissonRatio->setMean(QString("0.3"));
        poissonRatio->setCoefficientOfVariation("0.05");

        thePoissonsRatio = poissonRatio;
        theParameterList<<thePoissonsRatio;
    }

    return thePoissonsRatio;
}


RParameter *RCompositeDeckingMaterialKnowledge::getDensity()
{
    if (theDensity == nullptr) {

        // Set the mass density
        theDomain->createObject(objectName() + "CompositePanelDensity", "RContinuousRandomVariable");
        RContinuousRandomVariable *density = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        density->setDistributionType(QString("Lognormal (zeta, sigma)"));
        density->setMean(QString("1600.0"));
        density->setCoefficientOfVariation("0.10");

        theDensity = density;
        theParameterList<<theDensity;
    }

    return theDensity;
}


RParameter* RCompositeDeckingMaterialKnowledge::getLWCfc()
{
    if(fpc == nullptr)
    {
        theDomain->createObject(theComponentName + "LWCFpc", "RContinuousRandomVariable");
        fpc = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        fpc->setDistributionType(QString("Lognormal (zeta, sigma)"));
        // https://www.escsi.org/wp-content/themes/escsi/assets/images/6%20Chapter%206%20Physical%20Properties%20of%20SLWC%20Complete.pdf
        fpc->setMean(QString("20.0e6")); //[Pa] 20 MPa
        fpc->setCoefficientOfVariation("0.2");
    }

    return fpc;
}


RParameter* RCompositeDeckingMaterialKnowledge::getSteelYieldStress()
{
    if (theYieldStress == nullptr)
    {
        theDomain->createObject(theComponentName + "A36SteelYieldStress", "RContinuousRandomVariable");
        theYieldStress = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theYieldStress->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theYieldStress->setMean(QString("400000000.0")); // [Pa] A36 350 or 400 MPa
        theYieldStress->setCurrentValue(400000000.0); // [Pa] A36 350 or 400 MPa
        theYieldStress->setCoefficientOfVariation("0.2");
    }

    return theYieldStress;
}

