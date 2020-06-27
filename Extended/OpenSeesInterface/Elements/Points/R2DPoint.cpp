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

#include "R2DPoint.h"

R2DPoint::R2DPoint()
{
}


R2DPoint::~R2DPoint()
{
}


void R2DPoint::setYCoordinateValue(const double value)
{
    yCoordinate = value;
}


void R2DPoint::setZCoordinateValue(const double value)
{
    zCoordinate = value;
}


void R2DPoint::setYCoordinateValue(const std::function<double(void)>& value)
{
    theYCoordinateValue = value;
}


void R2DPoint::setZCoordinateValue(const std::function<double(void)>& value)
{
    theZCoordinateValue = value;
}

double R2DPoint::getYCoordinateValue() const
{
    return theYCoordinateValue();
}


double R2DPoint::getZCoordinateValue() const
{
    return theZCoordinateValue();
}
