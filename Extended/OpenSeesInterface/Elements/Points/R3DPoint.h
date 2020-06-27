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

// This is a simplified 3D "point" class for plotting/meshing purposes, not the same type of point as for creating components (RPoint)

#ifndef R3DPoint_H
#define R3DPoint_H

#include <functional>

class R3DPoint
{
public:

    R3DPoint(const double x,const double y,const double z);
    R3DPoint();
    ~R3DPoint();

    void setXCoordinateValue(const double value);

    void setYCoordinateValue(const double value);

    void setZCoordinateValue(const double value);

    void setXCoordinateValue(const std::function<double(void)>& value);

    void setYCoordinateValue(const std::function<double(void)>& value);

    void setZCoordinateValue(const std::function<double(void)>& value);

    double getXCoordinateValue() const;

    double getYCoordinateValue() const;

    double getZCoordinateValue() const;

private:

    double xCoordinate = 0.0;
    double yCoordinate = 0.0;
    double zCoordinate = 0.0;

    std::function<double(void)> theXCoordinateValue = [&](void){return xCoordinate;};
    std::function<double(void)> theYCoordinateValue = [&](void){return yCoordinate;};
    std::function<double(void)> theZCoordinateValue = [&](void){return zCoordinate;};

};

#endif

