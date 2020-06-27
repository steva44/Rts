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

#include "RConcreteMaterialKnowledge.h"
#include "RContinuousRandomVariable.h"
#include "RParameter.h"

#include <math.h>

RConcreteMaterialKnowledge::RConcreteMaterialKnowledge(RDomain *theDomain,
                                                       QString componentName,
                                                       QString strengthString)
    : RMaterialKnowledge(theDomain, componentName)
{
    theStrengthString = strengthString;

    E = nullptr;
    nu = nullptr;
    rho = nullptr;
    fpc = nullptr;
    fcUlt = nullptr;
    epsc0 = nullptr;
    epscu = nullptr;
    lambda = nullptr;
    beta  = nullptr;
    ft = nullptr;
    Ets = nullptr;
    Vn = nullptr;
    Kdeg = nullptr;
    G = nullptr;
    spallingStrainLimit = nullptr;
    coverLossStrainLimit = nullptr;
    coreCrushingStrainLimit  = nullptr;
    theReinforcementMaterial = nullptr;

    // Impact categories and emissions  were calculated from openLCA using EcoInvest
    // The values below are the impacts for 1 m3 of concrete produced
    // Product system:	Concrete-25-Canada
    // Reference process:	concrete production 25MPa, RNA only
    // Reference process location:	CA-QC
    // Product:	concrete, 25MPa
    // Amount:	1.0 m3
    // Impact method:	ReCiPe Midpoint (H)
    // Normalisation & weighting set:	none
    // Allocation method:	none
    // Cutoff:	none
    // Date:	09-28-2018 13:01

    // ReCiPe 2016 impact categories, per /m^3 concrete
    agriculturalLandOccupation     =  3.948867337;   // m2*a
    climateChange                  =  294.1178802;   // kg CO2 eq
    fossilDepletion                =  35.20209275;   // kg oil eq
    freshwaterEcotoxicity          =  0.925904547;   // kg 1,4-DB eq
    freshwaterEutrophication       =  0.0326664  ;   // kg P eq
    humanToxicity                  =  41.56353504;   // kg 1,4-DB eq
    ionisingRadiation              =  6.761222763;   // kg U235 eq
    marineEcotoxicity              =  0.937148433;   // kg 1,4-DB eq
    marineEutrophication           =  0.031803527;   // kg N eq
    metalDepletion                 =  6.309194194;   // kg Fe eq
    naturalLandTransformation      =  0.044047123;   // m2
    ozoneDepletion                 =  1.17363E-05;   // kg CFC-11 eq
    particulateMatterFormation     =  0.35475662 ;   // kg PM10 eq
    photochemicalOxidantFormation  =  0.712769951;   // kg NMVOC
    terrestrialAcidification       =  0.740506384;   // kg SO2 eq
    terrestrialEcotoxicity         =  0.013083817;   // kg 1,4-DB eq
    urbanLandOccupation            =  3.095752974;   // m2*a
    waterDepletion                 =  464.2603531;   // m3

    // Emissions /m^3 concrete
    carbonDioxideEmissions   = 291.0366945;   // kg/m^3
    carbonMonoxideEmissions  = 0.340182809;   // kg/m^3
    organicCarbonEmissions   = 2.1985E-07;    // kg/m^3
    methaneEmissions         = 0.313691199;   // kg/m^3
    nitrogenOxidesEmissions  = 0.590275706;   // kg/m^3
    HFC134aEmissions         = 1.28574E-07;   // kg/m^3
    sulfurDioxideEmissions   = 0.382277999;   // kg/m^3
    ammoniaEmissions         = 0.011294244;   // kg/m^3
    nitrousOxideEmissions    = 0.0022868;     // kg/m^3
    blackCarbonPMEmissions   = 0.093776425;   // kg/m^3

    // Concrete material R value, per m thickness of wall, for energy loss calculations
    // CANADIAN READY-MIXED CONCRETE ASSOCIATION + INSULATING CONCRETE FORMS MANUFACTURERS ASSOCIATION
    // Meeting and Exceeding Building Code Thermal Performance Requirements
    // ISO 10211:2007. Thermal bridges in building construction Heat flows and surface temperatures Detailed calculations. International Organization for Standardization, Geneva, 2007.
    thermalConductivity = 2.4; // w/mK

}


RConcreteMaterialKnowledge::~RConcreteMaterialKnowledge()
{

}


RParameter *RConcreteMaterialKnowledge::getYoungsModulus(void)
{
    // Create the parameter unless that was done before
    if (E == nullptr)
    {
        // Using the code equation from CSA 2004 for E
        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            //This needs to be in MPa
            auto fc = this->getFpc()->getCurrentValue()/1.0e6;

            // Equation gives modulus in Mpa, need to convert to Pa
            auto Ec = (3300.0*sqrt(fc)+6900.0)*1.0e6; // At f'c of 40 MPa, Ec is about 28 GPa

            return Ec;
        };

        theDomain->createObject(theComponentName + "YoungsModulus", "RConstant");
        E = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
        E->setCurrentValue(fxn);

        theParameterList<<E;
    }

    return E;
}


RParameter *RConcreteMaterialKnowledge::getShearModulus(void)
{

    if(G == nullptr)
    {
        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){

            // Assume concrete is isotropic... even though its not
            return this->getYoungsModulus()->getCurrentValue()/(2.0*(1.0 + this->getPoissonsRatio()->getCurrentValue()));
        };


        theDomain->createObject(theComponentName + "shearModulus", "RConstant");
        G = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        G->setCurrentValue(fxn);
    }

    return G;

}


RParameter *RConcreteMaterialKnowledge::getPoissonsRatio(void)
{

    if (nu == nullptr) {

        // Create the random variable
        theDomain->createObject(theComponentName + "ConcretePoissonRatio", "RContinuousRandomVariable");
        nu = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());

        // Set properties of that random variable
        nu->setDistributionType(QString("Lognormal (zeta, sigma)"));
        nu->setMean(QString("0.15"));
        nu->setCoefficientOfVariation("0.05");

        theParameterList<<nu;
    }

    return nu;
}


RParameter *RConcreteMaterialKnowledge::getDensity(void)
{

    if (rho == nullptr) {

        // Create the random variable
        theDomain->createObject(theComponentName + "ConcreteWeightDensity", "RContinuousRandomVariable");
        rho = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());

        // Set properties of that random variable
        rho->setDistributionType(QString("Lognormal (zeta, sigma)"));
        rho->setMean(QString("2450.0"));  // [kg/m^3]
        rho->setCoefficientOfVariation("0.05");

        theParameterList<<rho;
    }

    return rho;
}


RParameter* RConcreteMaterialKnowledge::getCompressiveStrength(void)
{  
    return this->getFpc();
}


RParameter* RConcreteMaterialKnowledge::getTensileStrength(void)
{  
    return this->getFt();
}


RParameter* RConcreteMaterialKnowledge::getStrainAtfc(void)
{
    return this->getEpsc0();
}


RParameter* RConcreteMaterialKnowledge::getUltimateCrushingStress(void)
{    
    return this->getFcUlt();
}


RParameter*  RConcreteMaterialKnowledge::getUltimateCrushingStrain(void)
{    
    return this->getEpscu();
}


RParameter* RConcreteMaterialKnowledge::getLambda(void)
{
    if (lambda == nullptr)
    {
        theDomain->createObject(theComponentName + "ratUltStrainOrigSlope", "RConstant");
        lambda = qobject_cast<RConstant *>(theDomain->getLastAddedObject());

        // Value used in OpenSees examples
        lambda->setCurrentValue(0.1);
    }

    return lambda;
}


RParameter* RConcreteMaterialKnowledge::getBeta(void)
{
    if (beta == nullptr)
    {
        theDomain->createObject(theComponentName + "tensStiffCurveParam", "RConstant");
        beta = qobject_cast<RConstant *>(theDomain->getLastAddedObject());

        // Default value in the concrete04 class constructor
        beta->setCurrentValue(0.1);
    }

    return beta;
}


RParameter *RConcreteMaterialKnowledge::getUltimateShearStrength(void)
{
    if(Vn == nullptr)
    {
        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){

            // Assume 20% of f'c
            auto shearReductionFactor = 0.20;

            return this->getCompressiveStrength()->getCurrentValue()*shearReductionFactor;
        };

        theDomain->createObject(theComponentName + "shearStrength", "RConstant");
        Vn = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        Vn->setCurrentValue(fxn);
    }

    return Vn;
}


RParameter* RConcreteMaterialKnowledge::getKdeg(void)
{
    if(Kdeg == nullptr)
    {
        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            // Must be negative
            auto elasticStiffnessReductionFactor = -0.005;

            return this->getShearModulus()->getCurrentValue()*elasticStiffnessReductionFactor;
        };

        theDomain->createObject(theComponentName + "ShearDegradationStiffness-Kdeg", "RConstant");
        Kdeg = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        Kdeg->setCurrentValue(fxn);
    }

    return Kdeg;
}


RParameter* RConcreteMaterialKnowledge::getTensionStiffeningSlope(void)
{    
    return this->getEts();
}


RParameter *RConcreteMaterialKnowledge::getSpallingStrainLimit(void)
{

    if (spallingStrainLimit == nullptr)
    {
        theDomain->createObject(objectName() + "spallingStrainLimit", "RContinuousRandomVariable");
        spallingStrainLimit = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        spallingStrainLimit->setDistributionType(QString("Lognormal (zeta, sigma)"));
        // Reference for value of mean and standard deviation: Lehman, D., Moehle, J., Mahin, S., Calderone, A., & Henry, L. (2004). Experimental evaluation of the seismic performance of reinforced concrete bridge columns. Journal of Structural Engineering, 130(6), 869-879.
        spallingStrainLimit->setMean(QString("0.0066"));
        spallingStrainLimit->setCoefficientOfVariation("0.33");
    }

    return spallingStrainLimit;
}


RParameter *RConcreteMaterialKnowledge::getCoverLossStrainLimit(void)
{

    if (coverLossStrainLimit == nullptr)
    {
        theDomain->createObject(objectName() + "coverLossStrainLimit", "RContinuousRandomVariable");
        coverLossStrainLimit = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        coverLossStrainLimit->setDistributionType(QString("Lognormal (zeta, sigma)"));
        // Reference for value of mean and standard deviation: Lehman, D., Moehle, J., Mahin, S., Calderone, A., & Henry, L. (2004). Experimental evaluation of the seismic performance of reinforced concrete bridge columns. Journal of Structural Engineering, 130(6), 869-879.
        coverLossStrainLimit->setMean(QString("0.011"));
        coverLossStrainLimit->setCoefficientOfVariation("0.22");
    }

    return coverLossStrainLimit;
}


RParameter * RConcreteMaterialKnowledge::getCoreCrushingStrainLimit(void)
{

    if (coreCrushingStrainLimit == nullptr)
    {
        theDomain->createObject(objectName() + "coreCrushingStrainLimit", "RContinuousRandomVariable");
        coreCrushingStrainLimit = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        coreCrushingStrainLimit->setDistributionType(QString("Lognormal (zeta, sigma)"));
        // Reference for value of mean and standard deviation: Lehman, D., Moehle, J., Mahin, S., Calderone, A., & Henry, L. (2004). Experimental evaluation of the seismic performance of reinforced concrete bridge columns. Journal of Structural Engineering, 130(6), 869-879.
        coreCrushingStrainLimit->setMean(QString("0.02"));
        coreCrushingStrainLimit->setCoefficientOfVariation("0.36");
    }

    return coreCrushingStrainLimit;
}


RSteelMaterialKnowledge* RConcreteMaterialKnowledge::getReinforcementMaterial(void)
{
    return theReinforcementMaterial;
}


void RConcreteMaterialKnowledge::setReinforcementMaterial(RSteelMaterialKnowledge* material)
{
    theReinforcementMaterial = material;
}


double RConcreteMaterialKnowledge::getThermalConductivity(void) const
{
    return thermalConductivity;
}


void RConcreteMaterialKnowledge::setThermalConductivity(double value)
{
    thermalConductivity = value;
}


QString RConcreteMaterialKnowledge::getTheConcreteMaterialModel(void) const
{
    return theConcreteMaterialModel;
}


void RConcreteMaterialKnowledge::setTheConcreteMaterialModel(const QString &value)
{
    theConcreteMaterialModel = value;
}


RParameter *RConcreteMaterialKnowledge::getFcUlt(void)
{
    if(fcUlt == nullptr)
    {
        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            //20% of the f'c; from OpenSees Days 2010 workshop
            auto reductionRatio = 0.2;

            auto fc = this->getFpc()->getCurrentValue();

            return fc*reductionRatio;
        };

        theDomain->createObject(theComponentName + "fcUlt", "RConstant");
        fcUlt = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        fcUlt->setCurrentValue(fxn);
    }

    return fcUlt;
}


RParameter *RConcreteMaterialKnowledge::getEpsc0(void)
{
    if (epsc0 == nullptr)
    {
        theDomain->createObject(theComponentName + "StrainAtFpc", "RContinuousRandomVariable");
        epsc0 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());

        // Set eps0 0.002; strain that corresponds to fc' (Caltrans SDC)
        epsc0->setDistributionType(QString("Lognormal (zeta, sigma)"));
        epsc0->setMean(QString("0.002"));
        epsc0->setCoefficientOfVariation("0.05");

    }

    return epsc0;
}


RParameter *RConcreteMaterialKnowledge::getEpscu(void)
{
    if (epscu == nullptr)
    {
        theDomain->createObject(theComponentName + "UltCrushStrain", "RConstant");
        epscu = qobject_cast<RConstant *>(theDomain->getLastAddedObject());

        // Set epscu 0.005; ultimate strain for unconfined concrete (Caltrans SDC)
        epscu->setCurrentValue(0.005);
    }

    return epscu;
}


RParameter *RConcreteMaterialKnowledge::getEts(void)
{
    if(Ets == nullptr)
    {
        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){

            auto ft = this->getTensileStrength()->getCurrentValue();

            auto fc = this->getFpc()->getCurrentValue();
            auto Ec = this->getYoungsModulus()->getCurrentValue();

            // From http://opensees.berkeley.edu/wiki/index.php/ConventionalColumn_Cyclic.tcl
            return ft/(2.0*fc/Ec);

        };

        theDomain->createObject(theComponentName + "tensStiffSlope", "RConstant");
        Ets = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        Ets->setCurrentValue(fxn);
    }

    return Ets;
}


void RConcreteMaterialKnowledge::setTss(RParameter *value)
{
    Ets = value;
}


void RConcreteMaterialKnowledge::setEpscu(RParameter *value)
{
    epscu = value;
}


void RConcreteMaterialKnowledge::setFcUlt(RParameter *value)
{
    fcUlt = value;
}


void RConcreteMaterialKnowledge::setFt(RParameter *value)
{
    ft = value;
}


RParameter *RConcreteMaterialKnowledge::getFt(void)
{
    if(ft == nullptr)
    {
        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){

            // Assume 10% of f'c
            auto tensileReductionFactor = 0.10;

            return this->getCompressiveStrength()->getCurrentValue()*tensileReductionFactor;
        };

        theDomain->createObject(theComponentName + "tensileStrength", "RConstant");
        ft = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        ft->setCurrentValue(fxn);
    }

    return ft;
}


RParameter *RConcreteMaterialKnowledge::getFpc(void)
{
    if(fpc == nullptr)
    {
        // f'c Random Variable
        theDomain->createObject(theComponentName + "Fpc", "RContinuousRandomVariable");
        fpc = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());

        // Set properties of that random varible
        fpc->setDistributionType(QString("Lognormal (zeta, sigma)"));

        if (theStrengthString == "C15") {
            fpc->setMean(QString("15.0e6"));
        }
        else if (theStrengthString == "C20") {
            fpc->setMean(QString("20.0e6")); // [Pa]
        }
        else if (theStrengthString == "C25") {
            fpc->setMean(QString("25.0e6")); // [Pa]
        }
        else if (theStrengthString == "C30") {
            fpc->setMean(QString("30.0e6")); // [Pa]
        }
        else if (theStrengthString == "C35") {
            fpc->setMean(QString("35.0e6")); // [Pa]
        }
        else if (theStrengthString == "C40") {
            fpc->setMean(QString("40.0e6")); // [Pa]
        }
        else if (theStrengthString == "C45") {
            fpc->setMean(QString("45.0e6")); // [Pa]
        }
        else
        {
            qCritical() << "Invalid concrete strength specified in " << theComponentName<<" using default f'c value of 25 MPa";
            fpc->setMean(QString("25.0e6")); // [Pa]
        }
        fpc->setCoefficientOfVariation("0.15");

    }

    return fpc;
}


