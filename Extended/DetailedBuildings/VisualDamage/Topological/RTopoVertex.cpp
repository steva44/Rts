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

#include "RTopoVertex.h"
#include "RNode.h"
#include <math.h>

RTopoVertex::RTopoVertex()
{

}


RTopoVertex::RTopoVertex(RNode* node, const double dx, const double dy, const double dz)
{ 
    theNode1 = node;
    theNode2 = nullptr;
    xi = 0.0;
    deltax = dx;
    deltay = dy;
    deltaz = dz;

    dispx = 0.0;
    dispy = 0.0;
    dispz = 0.0;
    rotx  = 0.0;
    roty  = 0.0;
    rotz  = 0.0;

    // Initial commit
    commitState(false);
}


RTopoVertex::RTopoVertex(RNode* node1, RNode* node2, const double XI, const double dx, const double dy, const double dz)
{

    theNode1 = node1;
    theNode2 = node2;
    xi = XI;
    deltax = dx;
    deltay = dy;
    deltaz = dz;

    // Initial commit
    commitState(false);
}


RTopoVertex::~RTopoVertex()
{

}


void RTopoVertex::commitState(bool deformed)
{

    if(theNode1 == nullptr && theNode2 == nullptr)
        return;

    auto x = theNode1->getXCoordinateValue();
    auto y = theNode1->getYCoordinateValue();
    auto z = theNode1->getZCoordinateValue();

    auto u0 = 0.0;
    auto u1 = 0.0;
    auto u2 = 0.0;
    auto u3 = 0.0;
    auto u4 = 0.0;
    auto u5 = 0.0;

    if(deformed)
    {
        auto disp1 = theNode1->getDisp();

        u0 = disp1(0);
        u1 = disp1(1);
        u2 = disp1(2);
        u3 = disp1(3);
        u4 = disp1(4);
        u5 = disp1(5);
    }

    if(theNode2 != nullptr)
    {
        x += (theNode2->getXCoordinateValue() - x)*xi;
        y += (theNode2->getYCoordinateValue() - y)*xi;
        z += (theNode2->getZCoordinateValue() - z)*xi;

        if(deformed)
        {
            auto disp2 = theNode2->getDisp();

            u0 += (disp2(0) - u0)*xi;
            u1 += (disp2(1) - u1)*xi;
            u2 += (disp2(2) - u2)*xi;
            u3 += (disp2(3) - u3)*xi;
            u4 += (disp2(4) - u4)*xi;
            u5 += (disp2(5) - u5)*xi;
        }
    }

    x += deltax;
    y += deltay;
    z += deltaz;

    //We need the rotations and deltas from the displaced point to the original
    dispx = u0;
    dispy = u1;
    dispz = u2;
    rotx = u3;
    roty = u4;
    rotz = u5;

    //These are then applied to the point that is to be translated in the order of: translation, rotation.. this ensures a rigid-link effect.
    // auto A = cos(rotx);      auto D = sin(roty);
    // auto B = sin(rotx);      auto E = cos(rotz);
    // auto C = cos(roty);      auto F = sin(rotz);

    // auto AD = A*D;    auto BD = B*D;

    // auto X = ((x + dx) * E - F * (y + dy)) * C + D * (z + dz);
    // auto Y = ((y + dy) * E + F * (x + dx)) * A + ((x + dx) * E - F * (y + dy)) * BD - B * C * (z + dz);
    // auto Z = ((-x - dx) * E + F * (y + dy)) * AD + ((y + dy) * E + F * (x + dx)) * B + A * C * (z + dz);

    // auto X = (x * E - F * y) * C + D * z;
    // auto Y = (y * E + F * x) * A + (x * E - F * y) * BD - B * C * z;
    // auto Z = (-x * E + F * y) * AD + (y * E + F * x) * B + A * C * z;

    auto XCoord = x; //+ dx;
    auto YCoord = y; //+ dy;
    auto ZCoord = z; //+ dz;

    OSGPoint.set(XCoord,YCoord,ZCoord);
}


double RTopoVertex::getXDisplacement()
{
    return dispx;
}


double RTopoVertex::getYDisplacement()
{
    return dispy;
}


double RTopoVertex::getZDisplacement()
{
    return dispz;
}


double RTopoVertex::getXRotation()
{
    return rotx;
}


double RTopoVertex::getYRotation()
{
    return roty;
}


double RTopoVertex::getZRotation()
{
    return rotz;
}
