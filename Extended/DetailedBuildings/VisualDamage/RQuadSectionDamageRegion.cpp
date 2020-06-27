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

#include "RQuadSectionDamageRegion.h"
#include "RSectionForceDeformation.h"
#include "RRCFibreContainer.h"
#include "RFiber.h"
#include "RElement.h"

RQuadSectionDamageRegion::RQuadSectionDamageRegion() : RFibreDamageRegion()
{
    theElement = nullptr;
    theSection = nullptr;
}


RQuadSectionDamageRegion::~RQuadSectionDamageRegion()
{

}


void RQuadSectionDamageRegion::reset(void)
{
    fiberMap.clear();
    quadrantI.reset();
    quadrantII.reset();
    quadrantIII.reset();
    quadrantIV.reset();
}


std::vector<RGenericFibreContainer*> RQuadSectionDamageRegion::getAllFibreContainers()
{
    std::vector<RGenericFibreContainer*> allQuadrants{&quadrantI,&quadrantII,&quadrantIII,&quadrantIV};

    return allQuadrants;
}


std::vector<RFiber*> RQuadSectionDamageRegion::getAllFibres(void)
{
    auto allContainers = this->getAllFibreContainers();

    auto numFib = this->numFibres();

    std::vector<RFiber*> allFibres;

    allFibres.reserve(numFib);

    for(auto&& it : allContainers)
    {
        allFibres.insert(allFibres.end(),  it->getAllFibres().begin(),  it->getAllFibres().end());
    }

    return allFibres;
}


int RQuadSectionDamageRegion::numFibres(void)
{
    auto numI = quadrantI.numFibres();
    auto numII = quadrantII.numFibres();
    auto numIII = quadrantIII.numFibres();
    auto numIV = quadrantIV.numFibres();

    return numI + numII + numIII + numIV;
}




