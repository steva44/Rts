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

#include "RPlateFromPlaneStress.h"
#include "RUniaxialMaterial.h"
#include "RParameter.h"

#include "PlateFromPlaneStressMaterial.h"

#include <sstream>

RPlateFromPlaneStress::RPlateFromPlaneStress(QObject *parent, QString &name)
    : RNDMaterial(parent, name)
{
    thePlateFromPlaneStressMaterial = nullptr;
    planeStressMaterial = nullptr;
    OutOfPlaneShearModulus = nullptr;
}


RPlateFromPlaneStress::~RPlateFromPlaneStress()
{
}


int RPlateFromPlaneStress::update(void)
{
    thePlateFromPlaneStressMaterial.reset();

    return this->createPlateFromPlaneStressMaterial();
}


RParameter *RPlateFromPlaneStress::getOutOfPlaneShearModulus() const
{
    return OutOfPlaneShearModulus;
}


void RPlateFromPlaneStress::setOutOfPlaneShearModulus(RParameter *value)
{
    OutOfPlaneShearModulus = value;
}


NDMaterial* RPlateFromPlaneStress::getTheNDMaterial(void)
{
    if(thePlateFromPlaneStressMaterial == nullptr)
        this->createPlateFromPlaneStressMaterial();

    return thePlateFromPlaneStressMaterial.get();
}


void RPlateFromPlaneStress::setTheNDMaterial(RNDMaterial *value)
{
    planeStressMaterial = value;
}


int RPlateFromPlaneStress::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(2);

    auto tag = std::to_string(this->objectTag);

    std::string psMatTag     = std::to_string(planeStressMaterial->getObjectTag());
    std::string g    = std::to_string(OutOfPlaneShearModulus->getCurrentValue());

    if(flag == 1) //tcl file
    {
        //        out<<"# nDmaterial PlateFromPlaneStress $newmatTag $matTag $OutofPlaneModulus"<<std::endl;
        out<<"nDMaterial PlateFromPlaneStress "<<tag<<" "<<psMatTag<<" "<<g<<std::endl;
    }
    else if(flag == 2) //Python
    {
        qDebug()<<"Object 'PlateFromPlaneStressMaterial' not yet available in OpenSeesPy";
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


int RPlateFromPlaneStress::createPlateFromPlaneStressMaterial(void)
{
    if(planeStressMaterial == nullptr)
    {
        qDebug()<<"Material not set in "<<__FUNCTION__;
        return -1;
    }


    if(OutOfPlaneShearModulus == nullptr)
    {
        qDebug()<<"The out of plane modulus is not set in "<<__FUNCTION__;
        return -1;
    }


    //Get the OpenSees Material
    auto material = planeStressMaterial->getTheNDMaterial();

    if(material == nullptr)
    {
        qDebug()<<"Could not create an OpenSees material layer for"<<planeStressMaterial->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    auto g = OutOfPlaneShearModulus->getCurrentValue();

    //    PlateFromPlaneStressMaterial(int tag,
    //			NDMaterial &ndMat, double g ) ; //Where G is OutOfPlaneShearModulus
    thePlateFromPlaneStressMaterial = std::make_unique<PlateFromPlaneStressMaterial>(objectTag,*material,g);

    return 0;
}
