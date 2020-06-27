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

#include "RFiber.h"
#include "RUniaxialMaterial.h"

#include "Fiber.h"
#include "UniaxialMaterial.h"

#include <sstream>

RFiber::RFiber(QObject *parent, QString &name)
    : RObject(parent, name)
{
    theLengthLambda = [&](void){return theLength;};
    theAreaLambda = [&](void){return theArea;};
    theCentroidYCoordinateLambda = [&](void){return theYCoordinate;};
    theCentroidZCoordinateLambda = [&](void){return theZCoordinate;};

    theMaterial = nullptr;
    damageType = "NONE";
}


RFiber::~RFiber()
{
}


void RFiber::setFibreYCoordinate(double value)
{
    theYCoordinate = value;
}


void RFiber::setFibreZCoordinate(double value)
{
    theZCoordinate = value;
}


void RFiber::setFibreYCoordinate(std::function<double(void)> value)
{
    theCentroidYCoordinateLambda = std::move(value);
}


void RFiber::setFibreZCoordinate(std::function<double(void)> value)
{
    theCentroidZCoordinateLambda = std::move(value);
}


void RFiber::setLength(double value)
{
    theLength = value;
}


double RFiber::getLength(void)
{
    return theLengthLambda();
}


double RFiber::getArea(void)
{
    return theAreaLambda();
}


UniaxialMaterial *RFiber::getTheMaterial() const
{
    return theMaterial->getUniaxialMaterial();
}


void RFiber::setTheUniaxialMaterial(RUniaxialMaterial *value)
{
    theMaterial = value;
}


QString RFiber::getDamageType() const
{
    return damageType;
}


void RFiber::setDamageType(const QString &value)
{
    damageType = value;
}


int RFiber::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(4);

    auto tag = std::to_string(this->objectTag);

    auto Area = this->getArea();
    auto YCoordinate = this->getFibreYCoordinate();
    auto ZCoordinate = this->getFibreZCoordinate();

    std::string yloc    = std::to_string(YCoordinate);
    std::string zloc    = std::to_string(ZCoordinate);
    std::string A       = std::to_string(Area);
    std::string matTag  = std::to_string(theMaterial->getObjectTag());


    if(flag == 1) //tcl file
    {
        //        out<<"# fiber $yLoc $zLoc $A $matTag"<<std::endl;
        out<<"fiber "<<yloc<<" "<<zloc<<" "<<A<<" "<<matTag<<";"<<std::endl;
    }
    else if(flag == 2) //Python
    {
        //        out<<"# fiber(yloc, zloc, A, matTag)"<<std::endl;
        out<<"fiber("<<yloc<<", "<<zloc<<", "<<A<<", "<<matTag<<")"<<std::endl;
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


void RFiber::setArea(double value)
{
    theArea = value;
}


void RFiber::setLength(std::function<double(void)> value)
{
    theLengthLambda = std::move(value);
}


void RFiber::setArea(std::function<double(void)> value)
{
    theAreaLambda = std::move(value);
}


double RFiber::getVolume()
{
    return theLengthLambda()*theAreaLambda();
}


double RFiber::getFibreYCoordinate(void)
{
    return theCentroidYCoordinateLambda();
}


double RFiber::getFibreZCoordinate(void)
{
    return theCentroidZCoordinateLambda();
}
