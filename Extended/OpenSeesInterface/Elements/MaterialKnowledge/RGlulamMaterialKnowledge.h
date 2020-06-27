#ifndef RGlulamMaterialKnowledge_H
#define RGlulamMaterialKnowledge_H
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

#include "RMaterialKnowledge.h"

class RContinuousRandomVariable;
class RParameter;

class RGlulamMaterialKnowledge : public RMaterialKnowledge
{
    Q_OBJECT

public:
    RGlulamMaterialKnowledge(RDomain *theDomain, QString componentName);
    ~RGlulamMaterialKnowledge();

    // Youngs modulus
    RParameter *getYoungsModulus();

    // Poissons ratio
    RParameter *getPoissonsRatio();

    // Mass density
    RParameter *getDensity();

    // Stress at which compressive failure occurs
    RParameter* getUltimateCompressiveStress();

    // Stress at which tensile failure occurs
    RParameter* getUltimateTensileStress();

    // Strain at which compressive failure occurs
    RParameter* getUltimateCompressiveStrain(void);

    // Strain at which tensile failure occurs
    RParameter* getUltimateTensileStrain(void);

    // Slope of the post crushing branch
    RParameter* getmEParameter(void);

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

private:

    RContinuousRandomVariable *theYoungsModulus; // Young's modulus
    RContinuousRandomVariable *thePoissonsRatio; // Poisson's ratio
    RContinuousRandomVariable *theDensity;       // Mass density
    RParameter* epscu;                           // Ultimate crushing strain
    RParameter* epstu;                           // Ultimate tensile strain
    RParameter* mEParameter;                     // Slope of the post crushing branch in bilinear model
    RContinuousRandomVariable* ultCompStress;    // Ultimate crushing stress
    RContinuousRandomVariable* ultTensStress;    // Ultimate tensile stress

    // ReCiPe 2016 impact categories, per m3 wood
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

    // Emissions
    double carbonDioxideEmissions;   // kg/m^3
    double carbonMonoxideEmissions;  // kg/m^3
    double organicCarbonEmissions;   // kg/m^3
    double methaneEmissions;         // kg/m^3
    double nitrogenOxidesEmissions;  // kg/m^3
    double HFC134aEmissions;         // kg/m^3
    double sulfurDioxideEmissions;   // kg/m^3
    double ammoniaEmissions;         // kg/m^3
    double nitrousOxideEmissions;    // kg/m^3
    double blackCarbonPMEmissions;   // kg/m^3

};

#endif


