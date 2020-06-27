#ifndef RQuadSectionDamageRegion_H
#define RQuadSectionDamageRegion_H
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

// This class encompasses a set of fibre containers for a quadrilateral component cross=section
// The section is partitioned into four quadrants
// The quandrants are:
/*
 * 4^------------^3
 *  | \   I    / |
 *  |  \      /  |
 *  |   \    /   |
 *  |    \  /    |
 *  |     \/     |
 *  | IV  /\  II |
 *  |    /  \    |
 *  |   /    \   |
 *  |  /      \  |
 *  | /   III  \ |
 * 1^------------^
 *
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

class RQuadSectionDamageRegion  : public RFibreDamageRegion
{

public:
    RQuadSectionDamageRegion();
    ~RQuadSectionDamageRegion();

    // Fibre container corresponding to each quadrant
    RGenericFibreContainer quadrantI;
    RGenericFibreContainer quadrantII;
    RGenericFibreContainer quadrantIII;
    RGenericFibreContainer quadrantIV;

    // Returns all fibre containers
    std::vector<RGenericFibreContainer*> getAllFibreContainers(void);

    // Returns all of the fibres, from all of the containers
    std::vector<RFiber*> getAllFibres(void);

    // Numer of fibres from all of the containers
    int numFibres(void);

    // Reset the fibres
    void reset(void);

private:


};







#endif
