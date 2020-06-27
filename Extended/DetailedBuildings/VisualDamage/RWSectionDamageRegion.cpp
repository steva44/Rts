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

#include "RWSectionDamageRegion.h"
#include "RSectionForceDeformation.h"
#include "RRCFibreContainer.h"
#include "RFiber.h"
#include "RElement.h"

RWSectionDamageRegion::RWSectionDamageRegion() : RFibreDamageRegion()
{
    theElement = nullptr;
    theSection = nullptr;
}


RWSectionDamageRegion::~RWSectionDamageRegion()
{

}


void RWSectionDamageRegion::reset(void)
{
    fiberMap.clear();
    flangeTop.reset();
    web.reset();
    flangeBottom.reset();
}


std::vector<RGenericFibreContainer*> RWSectionDamageRegion::getAllFibreContainers()
{
    std::vector<RGenericFibreContainer*> allContainers{&flangeTop,&web,&flangeBottom};

    return allContainers;
}


std::vector<RFiber*> RWSectionDamageRegion::getAllFibres(void)
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


int RWSectionDamageRegion::numFibres(void)
{
    auto numI = flangeTop.numFibres();
    auto numII = flangeBottom.numFibres();
    auto numIII = web.numFibres();

    return numI + numII + numIII;
}









