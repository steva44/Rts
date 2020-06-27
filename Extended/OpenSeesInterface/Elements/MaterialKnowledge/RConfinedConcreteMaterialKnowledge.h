#ifndef RConfinedConcreteMaterialKnowledge_H
#define RConfinedConcreteMaterialKnowledge_H
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

// This class is the confined concrete version of the RConcreteMaterialKnowledge class
// Notice the overloaded functions to return the same parameters, but with values appropriate for confined concrete
// All units in N, m, Kg, Pa

#include "RConcreteMaterialKnowledge.h"
#include "RContinuousRandomVariable.h"

class RConfinedConcreteMaterialKnowledge : public RConcreteMaterialKnowledge
{
    Q_OBJECT

public:
    RConfinedConcreteMaterialKnowledge(RDomain *theDomain,
                                       QString componentName,
                                       QString strengthString);
    ~RConfinedConcreteMaterialKnowledge() override;

    // Confined compressive strength
    RParameter* getCompressiveStrength(void) override;
    RParameter* getFpcc(void);

    // Strain at compression strength confined
    RParameter* getStrainAtfc(void) override;
    RParameter* getStrainAtfcc(void);

    // Ultimate (crushing) confined compressive stress
    RParameter* getUltimateCrushingStress(void) override;
    RParameter* getUltimateConfinedCrushingStress(void);

    // Ultimate (crushing) strain for confined concrete
    RParameter* getUltimateCrushingStrain(void) override;
    RParameter* getUltimateConfinedCrushingStrain(void);

    // Confined tensile strength
    RParameter* getTensileStrength(void) override;
    RParameter *getFtcc();

    // Ratio between fc and f'c for confined concrete
    RParameter* getStRatio(void);

    // Yield strength of the hoop reinforcement
    void setFyh(RParameter *value);

    // Reinforcement ratio of transverse reinforcement
    void setRhoTrans(RParameter *value);

private:

    RParameter* fcc;        // Confined compressive strength
    RParameter* fccUlt;     // Ultimate (crushing) confined compressive stress
    RParameter* epscc;      // Strain at compression strength confined
    RParameter* epsccu;     // Ultimate (crushing) strain for confined concrete
    RParameter* stRatio;    // Ratio between fc and f'c for confined concrete
    RParameter* ftcc;       // Confined tensile strength
    RParameter* fyh;        // Yield strength of the hoop reinforcement
    RParameter* rhoTrans;   // Reinforcement ratio of transverse reinforcement

};

#endif


