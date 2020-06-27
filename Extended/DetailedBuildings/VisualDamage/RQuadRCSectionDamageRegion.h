#ifndef RQuadRCSectionDamageRegion_H
#define RQuadRCSectionDamageRegion_H
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

// This class encompasses a set of fibre containers for a quadrilateral reinforced concrete component cross=section
// The section is partitioned into four quadrants, each quadrant is a fibre container with core, cover, and rebar fibres
// The quandrants are:
/*
* 4^------------------^3
*  |  \     I      /  |
*  |   8^________^7   |
*  |    | \ I  / |    |
*  | IV |  \  /  | II |
*  |    |IV 9  II|    |
*  |    | /III \ |    |
*  |   5^________^6   |
*  |  /    III      \ |
* 1^------------------^2
*/

#include "RRCFibreContainer.h"
#include "RFibreDamageRegion.h"

#include <vector>
#include <string>
#include <unordered_map>

class RFiber;
class RElement;
class RSectionForceDeformation;
class RParameter;
struct fibreInfo;

class RQuadRCSectionDamageRegion  : public RFibreDamageRegion
{

public:
    RQuadRCSectionDamageRegion();
    ~RQuadRCSectionDamageRegion();

    // Fibre container corresponding to each quadrant
    RRCFibreContainer quadrantI;
    RRCFibreContainer quadrantII;
    RRCFibreContainer quadrantIII;
    RRCFibreContainer quadrantIV;

    // Returns all fibre containers
    std::vector<RRCFibreContainer*> getAllFibreContainers(void);

    // Returns all of the fibres, from all of the containers
    std::vector<RFiber*> getAllFibres(void);

    // Numer of fibres from all of the containers
    int numFibres(void);

    // Reset the fibres
    void reset(void);

private:


};
#endif
