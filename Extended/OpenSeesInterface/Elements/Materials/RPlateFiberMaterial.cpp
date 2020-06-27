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

#include "RPlateFiberMaterial.h"
#include "RParameter.h"
#include "RUniaxialMaterial.h"
#include "RConcreteMaterialKnowledge.h"

#include "PlateFiberMaterial.h"

RPlateFiberMaterial::RPlateFiberMaterial(QObject *parent, QString &name)
    : RNDMaterial(parent, name)
{
    the3DMaterial = nullptr;
}


RPlateFiberMaterial::~RPlateFiberMaterial()
{
}


int RPlateFiberMaterial::update(void)
{
    thePlateFiberMaterial.reset();

    return this->createRPlateFiberMaterial();
}


NDMaterial* RPlateFiberMaterial::getTheNDMaterial(void)
{
    if(thePlateFiberMaterial == nullptr)
        this->createRPlateFiberMaterial();

    return thePlateFiberMaterial.get();
}


int RPlateFiberMaterial::createRPlateFiberMaterial(void)
{
    if(the3DMaterial == nullptr)
    {
        qDebug()<<"The function "<<__FUNCTION__<<" needs a three-dimensional material as an input";
        return -1;
    }


    //    PlateFiberMaterial(int    tag,
    //                       NDMaterial &the3DMaterial ) ;
    thePlateFiberMaterial = std::make_unique<PlateFiberMaterial>(objectTag,*the3DMaterial);

    return 0;
}


NDMaterial *RPlateFiberMaterial::getThe3DMaterial() const
{
    return the3DMaterial;
}


void RPlateFiberMaterial::setThe3DMaterial(NDMaterial *value)
{
    the3DMaterial = value;
}
