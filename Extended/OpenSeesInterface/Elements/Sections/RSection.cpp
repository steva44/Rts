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

#include "RSection.h"
#include "RObject.h"
#include "RFibreDamageRegion.h"

RSection::RSection(QObject *parent, QString &name)
    : RObject(parent, name)
{
    theFiberContainer = nullptr;
}


RSection::~RSection()
{

}


std::vector<R2DPoint> RSection::getSectionVertexLoop() const
{
    return sectionVertexLoop;
}


void RSection::setSectionVertexLoop(const std::vector<R2DPoint> &value)
{
    sectionVertexLoop = value;
}


std::shared_ptr<RFibreDamageRegion>& RSection::getSegmentDamageRegion()
{
    return theFiberContainer;
}


void RSection::setTheFibreContainer(const std::shared_ptr<RFibreDamageRegion> &value)
{
    theFiberContainer = value;
}
