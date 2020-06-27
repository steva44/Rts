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

#include "RGlulamMaterialKnowledge.h"
#include "RContinuousRandomVariable.h"

#include <functional>

RGlulamMaterialKnowledge::RGlulamMaterialKnowledge(RDomain *theDomain, QString componentName)
    : RMaterialKnowledge(theDomain, componentName)
{   

    theYoungsModulus = nullptr;
    thePoissonsRatio = nullptr;
    theDensity = nullptr;
    ultCompStress = nullptr;
    ultTensStress = nullptr;
    epscu = nullptr;
    epstu = nullptr;
    mEParameter = nullptr;

    // These numbers were calculated from openLCA using EcoInvest
    // The values below are the impacts for 1 m3 of laminated wood produced
    // Product system:	Laminated-Canada
    // Reference process:	glued laminated timber production, for indoor use
    // Reference process location:	CA-QC
    // Product:	residual wood, dry
    // Amount:	1.0 m3
    // Impact method:	ReCiPe Midpoint (H)
    // Normalisation & weighting set:	none
    // Allocation method:	none
    // Cutoff:	none
    // Date:	09-28-2018 13:23

    // Midpoint impacts /m^3 wood
    agriculturalLandOccupation     =  11813.39632;   // m2*a
    climateChange                  =  133.9100359;   // kg CO2 eq
    fossilDepletion                =  49.15569255;   // kg oil eq
    freshwaterEcotoxicity          =  0.954334754;   // kg 1,4-DB eq
    freshwaterEutrophication       =  0.016851971;   // kg P eq
    humanToxicity                  =  30.20578014;   // kg 1,4-DB eq
    ionisingRadiation              =  8.187941621;   // kg U235 eq
    marineEcotoxicity              =  0.950433436;   // kg 1,4-DB eq
    marineEutrophication           =  0.073373149;   // kg N eq
    metalDepletion                 =  7.59591863 ;   // kg Fe eq
    naturalLandTransformation      =  8.573037543;   // m2
    ozoneDepletion                 =  1.78898E-05;   // kg CFC-11 eq
    particulateMatterFormation     =  0.49059426 ;   // kg PM10 eq
    photochemicalOxidantFormation  =  1.19781433 ;   // kg NMVOC
    terrestrialAcidification       =  0.81859508 ;   // kg SO2 eq
    terrestrialEcotoxicity         =  0.020021385;   // kg 1,4-DB eq
    urbanLandOccupation            =  625.1405507;   // m2*a
    waterDepletion                 =  1189.1284  ;   // m3

    // Emissions /m^3 of wood
    carbonDioxideEmissions   = 195.1174187;  // kg/m^3
    carbonMonoxideEmissions  = 0.542386839;  // kg/m^3
    organicCarbonEmissions   = 9.78337E-05;  // kg/m^3
    methaneEmissions         = 0.39680757;   // kg/m^3
    nitrogenOxidesEmissions  = 0.953139272;  // kg/m^3
    HFC134aEmissions         = 2.38E-07;     // kg/m^3
    sulfurDioxideEmissions   = 0.262471005;  // kg/m^3
    ammoniaEmissions         = 0.009127661;  // kg/m^3
    nitrousOxideEmissions    = 0.005577019;  // kg/m^3
    blackCarbonPMEmissions   = 0.173413817;  // kg/m^3

}


RGlulamMaterialKnowledge::~RGlulamMaterialKnowledge()
{

}


RParameter *RGlulamMaterialKnowledge::getYoungsModulus()
{
    if (theYoungsModulus == nullptr) {

        // Set the Young's modulus for glulam
        theDomain->createObject(objectName() + "GlulamYoungsModulus", "RContinuousRandomVariable");
        theYoungsModulus = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theYoungsModulus->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theYoungsModulus->setMean(QString("10620.0e6"));     //From ReLam: Nonlinear probabilistic model for the analysis of reinforced glulam beams in bending
        theYoungsModulus->setCurrentValue(10620.0e6);
        theYoungsModulus->setStandardDeviation("1103.0e6");

        theParameterList<<theYoungsModulus;
    }

    return theYoungsModulus;
}


RParameter *RGlulamMaterialKnowledge::getPoissonsRatio()
{
    if (thePoissonsRatio == nullptr) {

        // Set the Poissons Ratio for glulam
        theDomain->createObject(objectName() + "GlulamPoissonRatio", "RContinuousRandomVariable");
        thePoissonsRatio = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        thePoissonsRatio->setDistributionType(QString("Lognormal (zeta, sigma)"));
        thePoissonsRatio->setMean(QString("0.3"));
        thePoissonsRatio->setCurrentValue(0.3);
        thePoissonsRatio->setCoefficientOfVariation("0.05");

        theParameterList<<thePoissonsRatio;
    }

    return thePoissonsRatio;
}


RParameter *RGlulamMaterialKnowledge::getDensity()
{
    if (theDensity == nullptr) {

        // Set the mass density glulam
        theDomain->createObject(objectName() + "GlulamDensity", "RContinuousRandomVariable");
        theDensity = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDensity->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theDensity->setMean(QString("500.0"));
        theDensity->setCurrentValue(500.0);
        theDensity->setCoefficientOfVariation("0.10");

        theParameterList<<theDensity;
    }

    return theDensity;
}


RParameter* RGlulamMaterialKnowledge::getUltimateCompressiveStrain(void)
{
    if (epscu == nullptr)
    {
        theDomain->createObject(theComponentName + "UltCompStrain", "RConstant");
        epscu = qobject_cast<RConstant *>(theDomain->getLastAddedObject());

        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto fcult = this->getUltimateCompressiveStress()->getCurrentValue();

            auto E = this->getYoungsModulus()->getCurrentValue();

            // Negative since compression
            auto eps = -fcult/E;

            return eps;
        };

        epscu->setCurrentValue(fxn);
        theParameterList<<epscu;
    }

    return epscu;
}


RParameter* RGlulamMaterialKnowledge::getUltimateTensileStrain(void)
{
    if (epstu == nullptr)
    {
        theDomain->createObject(theComponentName + "UltTensileStrain", "RConstant");
        epstu = qobject_cast<RConstant *>(theDomain->getLastAddedObject());

        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto ftult = this->getUltimateTensileStress()->getCurrentValue();

            auto E = this->getYoungsModulus()->getCurrentValue();

            auto eps = ftult/E;

            return eps;
        };

        epstu->setCurrentValue(fxn);
        theParameterList<<epstu;
    }

    return epstu;
}


RParameter* RGlulamMaterialKnowledge::getmEParameter(void)
{

    if (mEParameter == nullptr) {

        auto Emod = this->getYoungsModulus()->getCurrentValue();

        // Set the m for the downward post ultimate branch
        theDomain->createObject(theComponentName + "glulamMParameter", "RConstant");
        mEParameter = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        // Reference is ReLam: Nonlinear probabilistic model for the analysis of reinforced glulam beams in bending
        mEParameter->setCurrentValue(-0.001*Emod);
        theParameterList<<mEParameter;
    }

    return mEParameter;
}


RParameter* RGlulamMaterialKnowledge::getUltimateCompressiveStress()
{

    if (ultCompStress == nullptr) {
        theDomain->createObject(objectName() + "GlulamUltCompressiveStress", "RContinuousRandomVariable");
        ultCompStress = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        ultCompStress->setDistributionType(QString("Lognormal (zeta, sigma)"));
        // Reference is ReLam: Nonlinear probabilistic model for the analysis of reinforced glulam beams in bending
        ultCompStress->setMean(QString("41.637e6"));
        ultCompStress->setCurrentValue(41.637e6);
        ultCompStress->setStandardDeviation("5.805e6");
        theParameterList<<ultCompStress;
    }

    return ultCompStress;
}


RParameter* RGlulamMaterialKnowledge::getUltimateTensileStress()
{

    if (ultTensStress == nullptr) {
        theDomain->createObject(objectName() + "GlulamUltTensileStress", "RContinuousRandomVariable");
        ultTensStress = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        ultTensStress->setDistributionType(QString("Lognormal (zeta, sigma)"));
        // Reference is ReLam: Nonlinear probabilistic model for the analysis of reinforced glulam beams in bending
        ultTensStress->setMean(QString("25.449e6"));
        ultTensStress->setCurrentValue(25.449e6);
        ultTensStress->setStandardDeviation("6.198e6");
        theParameterList<<ultTensStress;
    }

    return ultTensStress;
}

