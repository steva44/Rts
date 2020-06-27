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

#include "RPlateRebarMaterial.h"
#include "RUniaxialMaterial.h"

#include "PlateRebarMaterial.h"

#include <sstream>

RPlateRebarMaterial::RPlateRebarMaterial(QObject *parent, QString &name)
    : RNDMaterial(parent, name)
{
    thePlateRebarMaterial = nullptr;
    theUniaxialMaterial = nullptr;
    reinforcementAngle = 0.0;
}


RPlateRebarMaterial::~RPlateRebarMaterial()
{
}


void RPlateRebarMaterial::setTheUniaxialMaterial(RUniaxialMaterial *value)
{
    theUniaxialMaterial = value;
}


int RPlateRebarMaterial::update(void)
{
    thePlateRebarMaterial.reset();

    return this->createPlateRebarMaterial();
}


NDMaterial* RPlateRebarMaterial::getTheNDMaterial(void)
{
    if(thePlateRebarMaterial == nullptr)
        this->createPlateRebarMaterial();

    return thePlateRebarMaterial.get();
}


double RPlateRebarMaterial::getReinforcementAngle() const
{
    return reinforcementAngle;
}


void RPlateRebarMaterial::setReinforcementAngle(double value)
{
    reinforcementAngle = value;
}


int RPlateRebarMaterial::createPlateRebarMaterial(void)
{
    if(theUniaxialMaterial == nullptr)
    {
        qDebug()<<"Material not set in "<<__FUNCTION__;
        return -1;
    }

    auto theMat = theUniaxialMaterial->getUniaxialMaterial();

    if(theMat == nullptr)
    {
        qDebug()<<"Could not get the material in "<<__FUNCTION__;
        return -1;
    }


    //    PlateRebarMaterial( int tag,
    //                        UniaxialMaterial &uniMat,
    //                        double ang ) ;
    thePlateRebarMaterial = std::make_unique<PlateRebarMaterial>(objectTag,*theMat,reinforcementAngle);

    return 0;
}


int RPlateRebarMaterial::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(4);

    auto tag = std::to_string(this->objectTag);

    std::string uniMatTag   = std::to_string(theUniaxialMaterial->getObjectTag());
    std::string sita = std::to_string(reinforcementAngle);

    if(flag == 1) //tcl file
    {
        //        out<<"# nDmaterial PlateRebar $newmatTag $matTag $sita"<<std::endl;
        out<<"nDmaterial PlateRebar "<<tag<<" "<<uniMatTag<<" "<<sita<<std::endl;
    }
    else if(flag == 2) //Python
    {
        qDebug()<<"Object 'PlateRebarMaterial' not yet available in OpenSeesPy";
        return -1;
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}
