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

#include "RUniaxialFiber3d.h"
#include "RUniaxialMaterial.h"
#include "RDomain.h"

#include "UniaxialFiber3d.h"

RUniaxialFiber3d::RUniaxialFiber3d(QObject *parent, QString &name)
    : RFiber(parent, name)
{
    theUniaxialFiber3d = nullptr;
}


RUniaxialFiber3d::~RUniaxialFiber3d()
{
}


int RUniaxialFiber3d::update(void)
{
    theUniaxialFiber3d.reset();

    auto res = this->createUniaxialFiber3d();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees 'UniaxialFiber3d' in "<<this->objectName();
        return -1;
    }

    //Visual damage
    damageType = "NONE";

    return 0;
}


Fiber* RUniaxialFiber3d::getTheFiber(void)
{
    if(theUniaxialFiber3d == nullptr)
        this->createUniaxialFiber3d();

    return theUniaxialFiber3d.get();
}


int RUniaxialFiber3d::createUniaxialFiber3d(void)
{
    if(theMaterial == nullptr)
    {
        qDebug()<<"Material not set in "<<__FUNCTION__;
        return -1;
    }

    auto theMat = theMaterial->getUniaxialMaterial();

    if(theMat == nullptr)
    {
        qDebug()<<"Could not get the material in "<<__FUNCTION__;
        return -1;
    }

    auto Area = this->getArea();
    auto YCoordinate = this->getFibreYCoordinate();
    auto ZCoordinate = this->getFibreZCoordinate();

    static Vector position(2);
    position(0) = YCoordinate;
    position(1) = ZCoordinate;

    //       UniaxialFiber3d (int tag, UniaxialMaterial &theMat, double Area,
    //                            const Vector &position);
    theUniaxialFiber3d = std::make_unique<UniaxialFiber3d>(objectTag,*theMat,Area,position);

    if(theUniaxialFiber3d == nullptr)
        return -1;

    return 0;
}



