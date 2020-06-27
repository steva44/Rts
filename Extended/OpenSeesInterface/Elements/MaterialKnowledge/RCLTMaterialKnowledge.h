#ifndef RCLTMaterialKnowledge_H
#define RCLTMaterialKnowledge_H
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

// Cross-laminated timber material
// All values in this class taken from the CLT Handbook USA. for a 5-layer, E1 panel: P.113 Figure 6
// All units in N, m, Kg, Pa

#include "RMaterialKnowledge.h"
#include "RContinuousRandomVariable.h"

class RCLTMaterialKnowledge : public RMaterialKnowledge
{

public:
    RCLTMaterialKnowledge(RDomain *theDomain,
                          QString componentName);
    ~RCLTMaterialKnowledge();

    // Youngs modulus parameter
    RParameter *getYoungsModulus();

    // Poissons ratio
    RParameter *getPoissonsRatio();

    // Mass density
    RParameter *getDensity();

    //Assume the same strength along minor and major axes
    // Compressive strength
    RParameter* getFc0();

    // Tensile strength
    RParameter* getFt0();

    // Shear strength
    RParameter* getFv0();

private:

    RContinuousRandomVariable *theYoungsModulus;   // Young's modulus
    RContinuousRandomVariable *thePoissonsRatio;   // Poisson's ratio
    RContinuousRandomVariable *theDensity;         // Mass density
    RContinuousRandomVariable *theFc0;             // Compressive strength
    RContinuousRandomVariable *theFt0;             // Tensile strength
    RContinuousRandomVariable *theFv0;             // Shear strength
};

#endif


