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

#include "R3DPoint.h"

R3DPoint::R3DPoint(const double x,const double y,const double z):xCoordinate(x),yCoordinate(y),zCoordinate(z)
{
}


R3DPoint::R3DPoint()
{
}


R3DPoint::~R3DPoint()
{
}


void R3DPoint::setXCoordinateValue(const double value)
{
    xCoordinate = value;
}


void R3DPoint::setYCoordinateValue(const double value)
{
    yCoordinate = value;
}


void R3DPoint::setZCoordinateValue(const double value)
{
    zCoordinate = value;
}


void R3DPoint::setXCoordinateValue(const std::function<double(void)>& value)
{
    theXCoordinateValue = value;
}


void R3DPoint::setYCoordinateValue(const std::function<double(void)>& value)
{
    theYCoordinateValue = value;
}


void R3DPoint::setZCoordinateValue(const std::function<double(void)>& value)
{
    theZCoordinateValue = value;
}


double R3DPoint::getXCoordinateValue() const
{
    return theXCoordinateValue();
}


double R3DPoint::getYCoordinateValue() const
{
    return theYCoordinateValue();
}


double R3DPoint::getZCoordinateValue() const
{
    return theZCoordinateValue();
}
