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

#include "RQuadRCSectionDamageRegion.h"
#include "RSectionForceDeformation.h"
#include "RRCFibreContainer.h"
#include "RFiber.h"
#include "RElement.h"

RQuadRCSectionDamageRegion::RQuadRCSectionDamageRegion() : RFibreDamageRegion()
{
    theElement = nullptr;
    theSection = nullptr;
}


RQuadRCSectionDamageRegion::~RQuadRCSectionDamageRegion()
{

}


void RQuadRCSectionDamageRegion::reset(void)
{
    fiberMap.clear();
    quadrantI.reset();
    quadrantII.reset();
    quadrantIII.reset();
    quadrantIV.reset();
}


std::vector<RRCFibreContainer*> RQuadRCSectionDamageRegion::getAllFibreContainers()
{
    std::vector<RRCFibreContainer*> allQuadrants{&quadrantI,&quadrantII,&quadrantIII,&quadrantIV};

    return allQuadrants;
}


std::vector<RFiber*> RQuadRCSectionDamageRegion::getAllFibres(void)
{
    auto allContainers = this->getAllFibreContainers();

    auto numFib = this->numFibres();

    std::vector<RFiber*> allFibres;

    allFibres.reserve(numFib);

    for(auto&& it : allContainers)
    {
        allFibres.insert(allFibres.end(), it->theCoreFibres.begin(), it->theCoreFibres.end());
        allFibres.insert(allFibres.end(), it->theCoverFibres.begin(), it->theCoverFibres.end());
        allFibres.insert(allFibres.end(), it->theReinforcementFibres.begin(), it->theReinforcementFibres.end());
    }

    return allFibres;
}


int RQuadRCSectionDamageRegion::numFibres(void)
{
    auto numI = quadrantI.theCoreFibres.size() + quadrantI.theCoverFibres.size() + quadrantI.theReinforcementFibres.size();
    auto numII = quadrantII.theCoreFibres.size() + quadrantII.theCoverFibres.size() + quadrantII.theReinforcementFibres.size();
    auto numIII = quadrantIII.theCoreFibres.size() + quadrantIII.theCoverFibres.size() + quadrantIII.theReinforcementFibres.size();
    auto numIV = quadrantIV.theCoreFibres.size() + quadrantIV.theCoverFibres.size() + quadrantIV.theReinforcementFibres.size();

    return numI + numII + numIII + numIV;
}




