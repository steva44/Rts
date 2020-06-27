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

#include "RMaterial.h"
#include "RObject.h"

RMaterial::RMaterial(QObject *parent, QString &name)
    : RObject(parent, name)
{
    theLength = 0.0;
    theArea = 0.0;
    theCentroidYCoordinate = 0.0;
    theCentroidZCoordinate = 0.0;

    theLengthLambda = [&](void){return theLength;};
    theAreaLambda = [&](void){return theArea;};
    theCentroidYCoordinateLambda = [&](void){return theCentroidYCoordinate;};
    theCentroidZCoordinateLambda = [&](void){return theCentroidZCoordinate;};
}


RMaterial::~RMaterial()
{

}


void RMaterial::setMaterialKnowledge(RMaterialKnowledge *materialKnowledge)
{
    qCritical()<<"ERROR: setMaterialKnowledge function has not been implemented for this material";
}


void RMaterial::setFibreCentroidYCoordinate(double value)
{
    theCentroidYCoordinate = value;
}


void RMaterial::setFibreCentroidZCoordinate(double value)
{
    theCentroidZCoordinate = value;
}


void RMaterial::setFibreCentroidYCoordinate(std::function<double(void)> value)
{
    theCentroidYCoordinateLambda = std::move(value);
}


void RMaterial::setFibreCentroidZCoordinate(std::function<double(void)> value)
{
    theCentroidZCoordinateLambda = std::move(value);
}


void RMaterial::setLength(double value)
{
    theLength = value;
}


double RMaterial::getLength(void)
{
    return theLengthLambda();
}


double RMaterial::getArea(void)
{
    return theAreaLambda();
}


void RMaterial::setArea(double value)
{
    theArea = value;
}


void RMaterial::setLength(std::function<double(void)> value)
{
    theLengthLambda = std::move(value);
}


void RMaterial::setArea(std::function<double(void)> value)
{
    theAreaLambda = std::move(value);
}


double RMaterial::getVolume()
{
    return theLengthLambda()*theAreaLambda();
}


double RMaterial::getFibreCentroidYCoordinate(void)
{
    return theCentroidYCoordinateLambda();
}


double RMaterial::getFibreCentroidZCoordinate(void)
{
    return theCentroidZCoordinateLambda();
}
