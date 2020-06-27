#ifndef RConcreteMaterialKnowledge_H
#define RConcreteMaterialKnowledge_H
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

// Reinforced concrete material
// All units in N, m, Kg, Pa

#include "RMaterialKnowledge.h"
#include "RContinuousRandomVariable.h"
#include "RSteelMaterialKnowledge.h"

class RConcreteMaterialKnowledge : public RMaterialKnowledge
{
    Q_OBJECT

public:
    RConcreteMaterialKnowledge(RDomain *theDomain,
                               QString componentName,
                               QString strengthString);
    ~RConcreteMaterialKnowledge();

    // Youngs modulus parameter
    RParameter *getYoungsModulus(void);

    // Poissons ratio
    RParameter *getPoissonsRatio(void);

    // Mass density
    RParameter *getDensity(void);

    // Shear modulus
    RParameter *getShearModulus(void);

    // Get the compressive strength
    virtual RParameter* getCompressiveStrength(void);

    // Strain at compression strength
    virtual RParameter* getStrainAtfc(void);

    // Stress at ultimate (crushing) stress
    virtual RParameter* getUltimateCrushingStress(void);

    // Ultimate (crushing) strain
    virtual RParameter* getUltimateCrushingStrain(void);

    // Tensile strength
    virtual RParameter* getTensileStrength(void);

    // Ratio between unloading slope at ultimate (crushing) strain and original slope
    RParameter* getLambda(void);

    // value defining the exponential curve parameter to define the residual stress (as a factor of $ft) at $etu (Concrete04)
    RParameter* getBeta(void);

    // Tension stiffening slope
    RParameter* getTensionStiffeningSlope(void);

    // Ultimate shear strength
    RParameter *getUltimateShearStrength(void) ;

    // Degrading slope of the material model for shear spring (Kdeg)
    RParameter* getKdeg(void);

    // Get the reinforcement material
    RSteelMaterialKnowledge* getReinforcementMaterial(void);

    // Visual damage model parameters
    // Strain limit at which spalling occurs
    RParameter * getSpallingStrainLimit(void);

    // Strain limit at which cover loss occurs
    RParameter * getCoverLossStrainLimit(void);

    // Strain limit at which core crushing occurs
    RParameter * getCoreCrushingStrainLimit(void);

    // The reinforcement material - some parameters in this class depend on the reinforcement parameters
    void setReinforcementMaterial(RSteelMaterialKnowledge* material);

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

    double getThermalConductivity(void) const;
    void setThermalConductivity(double value);

    // Set and get the type of material model representing the behaviour of this material
    QString getTheConcreteMaterialModel(void) const;
    void setTheConcreteMaterialModel(const QString &value);

    RParameter *getFpc(void);

    RParameter *getFcUlt(void);

    RParameter *getEpsc0(void);

    RParameter *getEpscu(void);

    RParameter *getEts(void);

    RParameter *getFt(void);

    void setFt(RParameter *value);

    void setTss(RParameter *value);

    void setEpscu(RParameter *value);

    void setFcUlt(RParameter *value);

protected:

    QString theConcreteMaterialModel;

    QString theStrengthString;
    RContinuousRandomVariable* nu;      // Poisson's ratio
    RContinuousRandomVariable* rho;     // Mass density
    RContinuousRandomVariable* fpc;     // Compressive strength
    RContinuousRandomVariable* epsc0;   // Strain at compression strength
    RConstant* E;                       // Young's modulus
    RParameter* G;                      // Shear modulus
    RParameter* ft;                     // Tensile strength
    RParameter* Ets;                    // Tension stiffening slope
    RParameter* fcUlt;                  // Ultimate (crushing) compressive stress
    RParameter* epscu;                  // Ultimate (crushing) strain
    RParameter* lambda;                 // Ratio between unloading slope at epscu and original slope
    RParameter* Vn;                     // Ultimate shear strength
    RParameter* Kdeg;                   // Stiffness of shear degradation backbone
    RParameter* beta;                   // Value defining the exponential curve parameter to define the residual stress (as a factor of $ft) at $etu (Concrete04)

    // Visual damage model parameters
    QPointer<RContinuousRandomVariable> spallingStrainLimit;        // Strain limit at which spalling occurs
    QPointer<RContinuousRandomVariable> coverLossStrainLimit;       // Strain limit at which cover loss occurs
    QPointer<RContinuousRandomVariable> coreCrushingStrainLimit;    // Strain limit at which core crushing occurs

    // Reinforcement material, as some concrete properties explicitly depend on this
    QPointer<RSteelMaterialKnowledge> theReinforcementMaterial;

    // ReCiPe 2016 impact categories, per /m^3 concrete
    double agriculturalLandOccupation;      //m2*a
    double climateChange;                   //kg CO2 eq
    double fossilDepletion;                 //kg oil eq
    double freshwaterEcotoxicity;           //kg 1,4-DB eq
    double freshwaterEutrophication;        //kg P eq
    double humanToxicity;                   //kg 1,4-DB eq
    double ionisingRadiation;               //kg U235 eq
    double marineEcotoxicity;               //kg 1,4-DB eq
    double marineEutrophication;            //kg N eq
    double metalDepletion;                  //kg Fe eq
    double naturalLandTransformation;       //m2
    double ozoneDepletion;                  //kg CFC-11 eq
    double particulateMatterFormation;      //kg PM10 eq
    double photochemicalOxidantFormation;   //kg NMVOC
    double terrestrialAcidification;        //kg SO2 eq
    double terrestrialEcotoxicity;          //kg 1,4-DB eq
    double urbanLandOccupation;             //m2*a
    double waterDepletion;                  //m3

    // Emissions
    double carbonDioxideEmissions;   //kg/m^3
    double carbonMonoxideEmissions;  //kg/m^3
    double organicCarbonEmissions;   //kg/m^3
    double methaneEmissions;         //kg/m^3
    double nitrogenOxidesEmissions;  //kg/m^3
    double HFC134aEmissions;         //kg/m^3
    double sulfurDioxideEmissions;   //kg/m^3
    double ammoniaEmissions;         //kg/m^3
    double nitrousOxideEmissions;    //kg/m^3
    double blackCarbonPMEmissions;   //kg/m^3

    //Thermal conductivity for reinforced concrete wall
    double thermalConductivity;

};

#endif


