#ifndef RSteelMaterialKnowledge_H
#define RSteelMaterialKnowledge_H
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

// A36 Structural Steel material
// ALL UNITS N, m, Kg, Pa

#include "RMaterialKnowledge.h"

class RContinuousRandomVariable;

class RSteelMaterialKnowledge : public RMaterialKnowledge
{
    Q_OBJECT

public:
    RSteelMaterialKnowledge(RDomain *theDomain,
                            QString componentName,
                            QString strengthString);
    ~RSteelMaterialKnowledge();

    // Youngs modulus
    RParameter *getYoungsModulus(void);

    // Poissons ratio
    RParameter *getPoissonsRatio(void);

    // Mass density
    RParameter *getDensity(void);

    // Yield stress
    RParameter *getYieldStress(void);

    // Strain-hardening ratio (ratio between post-yield tangent and initial elastic tangent)
    RParameter *getb(void);
    RParameter *getStrainHardeningRatio(void);

    // The ductility factor for steel - mueps
    RParameter *getDuctilityFactor(void);

    // Parameters to control the transition from elastic to plastic branches -- mainly used in steel material 02
    RParameter * getRo(void);
    RParameter * getcR1(void);
    RParameter * getcR2(void);
    RParameter * geta1(void);
    RParameter * geta2(void);
    RParameter * geta3(void);
    RParameter * geta4(void);
    RParameter * getsiginit(void);

    // Visual damage model parameters

    // The strain limit at which reinforcment buckling occurs
    RParameter * getBucklingStrainLimit(void);

    // The strain limit at which reinforcment fracture occurs
    RParameter * getFractureStrainLimit(void);

    // Functions that return the ReCiPe impact categories for this material
    double getAgriculturalLandOccupationImpact     (void){return agriculturalLandOccupation;}
    double getClimateChangeImpact                  (void){return climateChange;}
    double getFossilDepletionImpact                (void){return fossilDepletion;}
    double getFreshwaterEcotoxicityImpact          (void){return freshwaterEcotoxicity;}
    double getFreshwaterEutrophicationImpact       (void){return freshwaterEutrophication;}
    double getHumanToxicityImpact                  (void){return humanToxicity;}
    double getIonisingRadiationImpact              (void){return ionisingRadiation;}
    double getMarineEcotoxicityImpact              (void){return marineEcotoxicity;}
    double getMarineEutrophicationImpact           (void){return marineEutrophication;}
    double getMetalDepletionImpact                 (void){return metalDepletion;}
    double getNaturalLandTransformationImpact      (void){return naturalLandTransformation;}
    double getOzoneDepletionImpact                 (void){return ozoneDepletion;}
    double getParticulateMatterFormationImpact     (void){return particulateMatterFormation;}
    double getPhotochemicalOxidantFormationImpact  (void){return photochemicalOxidantFormation;}
    double getTerrestrialAcidificationImpact       (void){return terrestrialAcidification;}
    double getTerrestrialEcotoxicityImpact         (void){return terrestrialEcotoxicity;}
    double getUrbanLandOccupationImpact            (void){return urbanLandOccupation;}
    double getWaterDepletionImpact                 (void){return waterDepletion;}

    // Functions that return the emissions for this material
    double getCarbonDioxideEmissions   (void){return carbonDioxideEmissions; }
    double getCarbonMonoxideEmissions  (void){return carbonMonoxideEmissions;}
    double getOrganicCarbonEmissions   (void){return organicCarbonEmissions; }
    double getMethaneEmissions         (void){return methaneEmissions;       }
    double getNitrogenOxidesEmissions  (void){return nitrogenOxidesEmissions;}
    double getHFC134aEmissions         (void){return HFC134aEmissions;       }
    double getSulfurDioxideEmissions   (void){return sulfurDioxideEmissions; }
    double getAmmoniaEmissions         (void){return ammoniaEmissions;       }
    double getNitrousOxideEmissions    (void){return nitrousOxideEmissions;  }
    double getBlackCarbonPMEmissions   (void){return blackCarbonPMEmissions; }

    // Set and get the type of material model representing the behaviour of this material
    QString getTheSteelMaterialModel(void) const;
    void setTheSteelMaterialModel(const QString &value);

private:

    QString theSteelMaterialModel;

    QString theStrengthString;
    QPointer<RContinuousRandomVariable> theYoungsModulus;   // Young's modulus
    QPointer<RContinuousRandomVariable> thePoissonsRatio;   // Poisson's ratio
    QPointer<RContinuousRandomVariable> theDensity;         // Mass density
    QPointer<RContinuousRandomVariable> theYieldStress;     // Yield stress
    QPointer<RParameter> thebParameter;                     // Strain-hardening ratio
    QPointer<RParameter> haRatio;                           // Hardening ratio of transverse reinforcement
    QPointer<RParameter> mueps;                             // Ductility factor of transverse reinforcement

    // Parameters to control the transition from elastic to plastic branches -- mainly used in steel material 02
    QPointer<RParameter> Ro;
    QPointer<RParameter> cR1;
    QPointer<RParameter> cR2;
    QPointer<RParameter> a1;
    QPointer<RParameter> a2;
    QPointer<RParameter> a3;
    QPointer<RParameter> a4;
    QPointer<RParameter> siginit;

    // Visual damage model parameters
    QPointer<RContinuousRandomVariable> bucklingStrainLimit; // The strain limit at which reinforcment buckling occurs
    QPointer<RContinuousRandomVariable> fractureStrainLimit; // The strain limit at which reinforcment fracture occurs

    // ReCiPe 2016 impact categories, per kg steel
    double agriculturalLandOccupation;      // m2*a
    double climateChange;                   // kg CO2 eq
    double fossilDepletion;                 // kg oil eq
    double freshwaterEcotoxicity;           // kg 1,4-DB eq
    double freshwaterEutrophication;        // kg P eq
    double humanToxicity;                   // kg 1,4-DB eq
    double ionisingRadiation;               // kg U235 eq
    double marineEcotoxicity;               // kg 1,4-DB eq
    double marineEutrophication;            // kg N eq
    double metalDepletion;                  // kg Fe eq
    double naturalLandTransformation;       // m2
    double ozoneDepletion;                  // kg CFC-11 eq
    double particulateMatterFormation;      // kg PM10 eq
    double photochemicalOxidantFormation;   // kg NMVOC
    double terrestrialAcidification;        // kg SO2 eq
    double terrestrialEcotoxicity;          // kg 1,4-DB eq
    double urbanLandOccupation;             // m2*a
    double waterDepletion;                  // m3

    // Emissions, per kg steel
    double carbonDioxideEmissions;   // kg/kg
    double carbonMonoxideEmissions;  // kg/kg
    double organicCarbonEmissions;   // kg/kg
    double methaneEmissions;         // kg/kg
    double nitrogenOxidesEmissions;  // kg/kg
    double HFC134aEmissions;         // kg/kg
    double sulfurDioxideEmissions;   // kg/kg
    double ammoniaEmissions;         // kg/kg
    double nitrousOxideEmissions;    // kg/kg
    double blackCarbonPMEmissions;   // kg/kg

};

#endif


