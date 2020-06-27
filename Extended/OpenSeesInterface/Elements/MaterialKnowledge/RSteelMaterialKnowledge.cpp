#include "RSteelMaterialKnowledge.h"
#include "RContinuousRandomVariable.h"
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

RSteelMaterialKnowledge::RSteelMaterialKnowledge(RDomain *theDomain,
                                                 QString componentName,
                                                 QString strengthString)
    : RMaterialKnowledge(theDomain, componentName)
{
    theStrengthString = strengthString;

    theYoungsModulus = nullptr;
    thePoissonsRatio = nullptr;
    theDensity = nullptr;
    theYieldStress = nullptr;
    thebParameter = nullptr;
    haRatio = nullptr; // hardening ratio of transverse reinforcement
    mueps = nullptr;   // ductility factor of transverse reinforcement
    Ro = nullptr;
    cR1 = nullptr;
    cR2 = nullptr;
    a1 = nullptr;
    a2 = nullptr;
    a3 = nullptr;
    a4 = nullptr;
    siginit = nullptr;
    bucklingStrainLimit = nullptr;
    fractureStrainLimit = nullptr;

    // These numbers were calculated from openLCA using EcoInvest
    // The values below are the impacts for 1 kg of steel produced
    // Product system:	Steel-Canada
    // Reference process:	steel production, electric, low-alloyed
    // Reference process location:	CA-QC
    // Product:	steel, low-alloyed
    // Amount:	1.0 kg
    // Impact method:	ReCiPe Midpoint (H)
    // Normalisation & weighting set:	none
    // Allocation method:	none
    // Cutoff:	none
    // Date:	09-28-2018 13:12

    // Midpoint impacts /kg of steel
    agriculturalLandOccupation     =  0.051567236;   //m2*a
    climateChange                  =  1.167666813;   //kg CO2 eq
    fossilDepletion                =  0.237350009;   //kg oil eq
    freshwaterEcotoxicity          =  0.237989897;   //kg 1,4-DB eq
    freshwaterEutrophication       =  0.006098822;   //kg P eq
    humanToxicity                  =  10.37489212;   //kg 1,4-DB eq
    ionisingRadiation              =  0.051679595;   //kg U235 eq
    marineEcotoxicity              =  0.218897471;   //kg 1,4-DB eq
    marineEutrophication           =  0.000655108;   //kg N eq
    metalDepletion                 =  3.153976325;   //kg Fe eq
    naturalLandTransformation      =  0.000649774;   //m2
    ozoneDepletion                 =  1.15461E-07;   //kg CFC-11 eq
    particulateMatterFormation     =  0.008345461;   //kg PM10 eq
    photochemicalOxidantFormation  =  0.006205178;   //kg NMVOC
    terrestrialAcidification       =  0.017020582;   //kg SO2 eq
    terrestrialEcotoxicity         =  0.000290948;   //kg 1,4-DB eq
    urbanLandOccupation            =  0.029914761;   //m2*a
    waterDepletion                 =  14.45714241;   //m3

    // Emissions /kg of steel
    carbonDioxideEmissions   = 1.115644997;   //kg
    carbonMonoxideEmissions  = 0.004102893;   //kg
    organicCarbonEmissions   = 7.15616E-09;   //kg
    methaneEmissions         = 0.002124103;   //kg
    nitrogenOxidesEmissions  = 0.004110674;   //kg
    HFC134aEmissions         = 1.71769E-09;   //kg
    sulfurDioxideEmissions   = 0.01389679;    //kg
    ammoniaEmissions         = 0.000335425;   //kg
    nitrousOxideEmissions    = 6.24857E-05;   //kg
    blackCarbonPMEmissions   = 0.002654646;   //kg

}


RSteelMaterialKnowledge::~RSteelMaterialKnowledge()
{

}


RParameter *RSteelMaterialKnowledge::getYoungsModulus(void)
{ 
    if (theYoungsModulus == nullptr) {

        // Set the Young's modulus for steel
        theDomain->createObject(theComponentName + "A36SteelYoungsModulus", "RContinuousRandomVariable");
        theYoungsModulus = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theYoungsModulus->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theYoungsModulus->setMean(QString("200000000000.0"));   // [Pa]
        theYoungsModulus->setCurrentValue(200000000000.0);      // [Pa]
        theYoungsModulus->setCoefficientOfVariation("0.15");
    }

    return theYoungsModulus;
}


RParameter *RSteelMaterialKnowledge::getYieldStress(void)
{
    if (theYieldStress == nullptr) {

        // Set the yield stress for steel
        theDomain->createObject(theComponentName + "A36SteelYieldStress", "RContinuousRandomVariable");
        theYieldStress = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theYieldStress->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theYieldStress->setMean(QString("400000000.0")); //[Pa] A36 350 or 400 MPa
        theYieldStress->setCurrentValue(400000000.0); //[Pa] A36 350 or 400 MPa
        theYieldStress->setCoefficientOfVariation("0.2");
    }

    return theYieldStress;
}


RParameter *RSteelMaterialKnowledge::getPoissonsRatio(void)
{
    if (thePoissonsRatio == nullptr) {

        // Set the Poissons ratio for steel
        theDomain->createObject(theComponentName + "A36SteelPoissonRatio", "RContinuousRandomVariable");
        thePoissonsRatio = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        thePoissonsRatio->setDistributionType(QString("Lognormal (zeta, sigma)"));
        thePoissonsRatio->setMean(QString("0.300"));
        thePoissonsRatio->setCurrentValue(0.300);
        thePoissonsRatio->setCoefficientOfVariation("0.03");
    }

    return thePoissonsRatio;
}


RParameter *RSteelMaterialKnowledge::getDensity(void)
{
    if (theDensity == nullptr) {

        // Set the mass density for steel
        theDomain->createObject(theComponentName + "A36SteelDensity", "RContinuousRandomVariable");
        theDensity = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDensity->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theDensity->setMean(QString("7900"));
        theDensity->setCurrentValue(7900);
        theDensity->setCoefficientOfVariation("0.05");
    }

    return theDensity;
}


RParameter *RSteelMaterialKnowledge::getb(void)
{
    if (thebParameter == nullptr) {

        // Set the b for steel
        theDomain->createObject(theComponentName + "A36Steelb", "RConstant");
        thebParameter = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        thebParameter->setCurrentValue(0.02);
    }

    return thebParameter;
}


RParameter *RSteelMaterialKnowledge::getStrainHardeningRatio(void)
{
    return this->getb();
}


RParameter *RSteelMaterialKnowledge::getDuctilityFactor(void)
{
    // Create the parameter unless that was done before
    if (mueps == nullptr) {

        // Set the ductility factor for steel
        theDomain->createObject(theComponentName + "A36SteelDuctilityFactor", "RConstant");
        mueps = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        mueps->setCurrentValue(1000); // From example http://opensees.berkeley.edu/wiki/index.php/ConfinedConcrete01_Material
    }

    // Return the parameter
    return mueps;
}


RParameter *RSteelMaterialKnowledge::getRo(void)
{
    if (Ro == nullptr)
    {
        theDomain->createObject(theComponentName + "RoFactor", "RConstant");
        Ro = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        // Default value from OpenSees Steel02 constructor
        Ro->setCurrentValue(15);
    }

    return Ro;
}


RParameter *RSteelMaterialKnowledge::getcR1(void)
{
    if (cR1 == nullptr)
    {
        theDomain->createObject(theComponentName + "cR1Factor", "RConstant");
        cR1 = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        // Default value from OpenSees Steel02 constructor
        cR1->setCurrentValue(0.925);
    }

    return cR1;

}


RParameter *RSteelMaterialKnowledge::getcR2(void)
{
    if (cR2 == nullptr) {
        theDomain->createObject(theComponentName + "cR2Factor", "RConstant");
        cR2 = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        // Default value from OpenSees Steel02 constructor
        cR2->setCurrentValue(0.15);
    }

    return cR2;
}


RParameter *RSteelMaterialKnowledge::geta1(void)
{
    if (a1 == nullptr) {
        theDomain->createObject(theComponentName + "a1Factor", "RConstant");
        a1 = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        // Default value from OpenSees Steel02 constructor
        a1->setCurrentValue(0.0);
    }

    return a1;
}


RParameter *RSteelMaterialKnowledge::geta2(void)
{
    if (a2 == nullptr) {
        theDomain->createObject(theComponentName + "a2Factor", "RConstant");
        a2 = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        // Default value from OpenSees Steel02 constructor
        a2->setCurrentValue(1.0);
    }

    return a2;
}


RParameter *RSteelMaterialKnowledge::geta3(void)
{
    if (a3 == nullptr) {
        theDomain->createObject(theComponentName + "a3Factor", "RConstant");
        a3 = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        // Default value from OpenSees Steel02 constructor
        a3->setCurrentValue(0.0);
    }

    return a3;
}


RParameter *RSteelMaterialKnowledge::geta4(void)
{
    if (a4 == nullptr) {
        theDomain->createObject(theComponentName + "a4Factor", "RConstant");
        a4 = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        // Default value from OpenSees Steel02 constructor
        a4->setCurrentValue(1.0);
    }

    return a4;
}


RParameter *RSteelMaterialKnowledge::getsiginit(void)
{
    if (siginit == nullptr) {
        theDomain->createObject(theComponentName + "siginitFactor", "RConstant");
        siginit = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        siginit->setCurrentValue(0.0);
    }

    return siginit;
}


RParameter *RSteelMaterialKnowledge::getBucklingStrainLimit(void)
{
    if (bucklingStrainLimit == nullptr)
    {
        theDomain->createObject(objectName() + "SteelBucklingStrainLimit", "RContinuousRandomVariable");
        bucklingStrainLimit = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        bucklingStrainLimit->setDistributionType(QString("Lognormal (zeta, sigma)"));
        // Reference for mean and standard deviation: Lehman, D., Moehle, J., Mahin, S., Calderone, A., & Henry, L. (2004). Experimental evaluation of the seismic performance of reinforced concrete bridge columns. Journal of Structural Engineering, 130(6), 869-879.
        bucklingStrainLimit->setMean(QString("0.038"));
        bucklingStrainLimit->setCurrentValue(0.038);
        bucklingStrainLimit->setCoefficientOfVariation("0.30");
    }

    return bucklingStrainLimit;
}


RParameter *RSteelMaterialKnowledge::getFractureStrainLimit(void)
{
    if (fractureStrainLimit == nullptr)
    {
        theDomain->createObject(objectName() + "SteelFractureStrainLimit", "RContinuousRandomVariable");
        fractureStrainLimit = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        fractureStrainLimit->setDistributionType(QString("Lognormal (zeta, sigma)"));
        // Wang, C, K. and Salmon, , G (1985) Reinforced Concrete design .'II, Fourth Edition
        fractureStrainLimit->setMean(QString("0.125"));
        fractureStrainLimit->setCurrentValue(0.125);
        fractureStrainLimit->setCoefficientOfVariation("0.20");
    }

    return fractureStrainLimit;
}


QString RSteelMaterialKnowledge::getTheSteelMaterialModel(void) const
{
    return theSteelMaterialModel;
}


void RSteelMaterialKnowledge::setTheSteelMaterialModel(const QString &value)
{
    theSteelMaterialModel = value;
}
