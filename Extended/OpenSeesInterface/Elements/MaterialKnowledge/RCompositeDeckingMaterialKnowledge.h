#ifndef RCompositeDeckingMaterialKnowledge_H
#define RCompositeDeckingMaterialKnowledge_H
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

// Composite decking material, with a steel pan and lightweight concrete topping
// All units in N, m, Kg, Pa

#include "RMaterialKnowledge.h"
#include "RContinuousRandomVariable.h"

class RCompositeDeckingMaterialKnowledge : public RMaterialKnowledge
{

public:
    RCompositeDeckingMaterialKnowledge(RDomain *theDomain,
                                       QString componentName);
    ~RCompositeDeckingMaterialKnowledge();

    // Youngs modulus parameter
    RParameter *getYoungsModulus();

    // Poissons ratio
    RParameter *getPoissonsRatio();

    // Mass density
    RParameter *getDensity();

    // Compressive strength of lightweight concrete
    RParameter* getLWCfc();

    // Yield stress of the steel decking
    RParameter* getSteelYieldStress();

private:

    RContinuousRandomVariable* theYoungsModulus;    // Young's modulus
    RContinuousRandomVariable* thePoissonsRatio;    // Poisson's ratio
    RContinuousRandomVariable* theDensity;          // Mass density
    RContinuousRandomVariable* fpc;                 // Compressive strength of lightweight concrete
    RContinuousRandomVariable* theYieldStress;      // Yield stress of the steel decking
};

#endif


