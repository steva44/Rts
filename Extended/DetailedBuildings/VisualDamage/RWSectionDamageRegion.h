#ifndef RWSectionDamageRegion_H
#define RWSectionDamageRegion_H
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
*                                                                    *
*********************************************************************/

// This class encompasses a damage region for W sections
// The section is discretized into 3 containers (top flange, bottom flange, and web:
/*
*       Top Flange
*  ^------------------^
*            |
*            |
*            |
*            | Web
*            |
*            |
*            |
*            |
*  ^------------------^
*      Bottom Flange
*/

#include "RGenericFibreContainer.h"
#include "RFibreDamageRegion.h"

#include <vector>
#include <string>
#include <unordered_map>

class RFiber;
class RElement;
class RSectionForceDeformation;
class RParameter;
struct fibreInfo;

class RWSectionDamageRegion  : public RFibreDamageRegion
{

public:
    RWSectionDamageRegion();
    ~RWSectionDamageRegion();

    // Fibre containers for the web and flanges
    RGenericFibreContainer flangeTop;
    RGenericFibreContainer web;
    RGenericFibreContainer flangeBottom;

    // Returns all fibre containers
    std::vector<RGenericFibreContainer*>  getAllFibreContainers(void);

    // Returns all of the fibres, from all of the containers
    std::vector<RFiber*> getAllFibres(void);

    // Numer of fibres from all of the containers
    int numFibres(void);

    // Reset the fibres
    void reset(void);

private:

};
#endif
