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

#include "RDamageRegion.h"

#include <QDebug>

RDamageRegion::RDamageRegion()
{
    damageTypes = {0,0,0,0,0,0};
    damageTypeString.resize(6);
}


RDamageRegion::~RDamageRegion()
{

}


int RDamageRegion::getDamageType(size_t index)
{
    if(index > damageTypes.size())
    {
        qCritical()<<"Error, index out of bounds in "<<__FUNCTION__;
        return  -1;
    }

    return damageTypes.at(index);
}


int RDamageRegion::getGlobalDamageType(void)
{
    int mode = 0;

    for(auto&& it : damageTypes)
    {
        if(it>mode)
            mode = it;
    }

    return mode;
}


void RDamageRegion::setGlobalDamageType(const int mode)
{
    for(auto&& it : damageTypes)
    {
        it = mode;
    }
}


void RDamageRegion::setDamageType(size_t index, const int mode)
{
    if(index > damageTypes.size())
    {
        qCritical()<<"Error, index out of bounds in "<<__FUNCTION__;
        return;
    }

    damageTypes.at(index) = mode;
}


void RDamageRegion::setDamageTypeString(size_t index, const std::string &value)
{
    if(index > damageTypeString.size())
    {
        qCritical()<<"Error, index out of bounds in "<<__FUNCTION__;
        return;
    }

    damageTypeString.at(index) = value;
}


std::string RDamageRegion::getDamageTypeString(size_t index)
{
    if(index > damageTypeString.size())
    {
        qCritical()<<"Error, index out of bounds in "<<__FUNCTION__;
        return "NULL";
    }

    return damageTypeString.at(index);
}


std::string RDamageRegion::getGlobalDamageTypeString(void)
{
    int maxMode = 0;
    size_t index = 0;

    for(size_t i = 0; i<6; ++i)
    {
        auto damageMode = damageTypes.at(i);
        if(damageMode > maxMode)
        {
            maxMode = damageMode;
            index = i;
        }
    }

    return damageTypeString.at(index);
}


void RDamageRegion::setGlobalDamageTypeString(const std::string type)
{
    for(auto&& it : damageTypeString)
    {
        it = type;
    }
}


double RDamageRegion::getCrackOrientation(size_t index) const
{
    // If the damage mesh does not support cracks, return 0.0
    return static_cast<double>(index)*0.0;
}


void RDamageRegion::resetDamageRegion(void)
{
    for(size_t i = 0; i<6; ++i)
    {
        damageTypes.at(i) = 0;
        damageTypeString.at(i) = "NONE";
    }
}
