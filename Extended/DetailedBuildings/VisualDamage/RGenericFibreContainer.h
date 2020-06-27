#ifndef RGenericFibreContainer_H
#define RGenericFibreContainer_H
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

// A generic fibre container that holds fibres in a damage region

#include "RFibreDamageRegion.h"

#include <vector>
#include <string>
#include <unordered_map>

class RFiber;
class RElement;
class RSectionForceDeformation;
class RParameter;
struct fibreInfo;

class RGenericFibreContainer  : public RFibreDamageRegion
{

public:
    RGenericFibreContainer();
    ~RGenericFibreContainer();

    int numFibres(void);

    void reset(void);

    std::vector<RFiber *>& getAllFibres();

private:

    // The fibres of the container
    std::vector<RFiber*> theFibres;

};







#endif
