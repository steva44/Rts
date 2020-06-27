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

#include "RLinearCrdTransf3d.h"
#include "LinearCrdTransf3d.h"

RLinearCrdTransf3d::RLinearCrdTransf3d(QObject *parent, QString name)
    : RCrdTransf(parent, name)
{
    theCrdTransformation = nullptr;
}


RLinearCrdTransf3d::~RLinearCrdTransf3d()
{

}


CrdTransf* RLinearCrdTransf3d::getTheCoordinateTransformation()
{
    if(!theCrdTransformation)
    {
        Vector vecxzPlane(3);

        vecxzPlane[0] = 1.0;
        vecxzPlane[1] = 0.0;
        vecxzPlane[2] = 0.0;

        theCrdTransformation = std::make_unique<LinearCrdTransf3d>(objectTag,vecxzPlane);
    }

    return theCrdTransformation.get();
}




