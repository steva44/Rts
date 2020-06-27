#ifndef RUniaxialFiber3d_H
#define RUniaxialFiber3d_H
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

//Wrapper class for OpenSees UniaxialFiber3d
//Stevan Gavrilovic 12/2018

//       UniaxialFiber3d (int tag, UniaxialMaterial &theMat, double Area,
//                            const Vector &position);

#include "RFiber.h"

class UniaxialFiber3d;

class RUniaxialFiber3d : public RFiber
{
    Q_OBJECT

public:
    RUniaxialFiber3d(QObject *parent, QString &name);
    ~RUniaxialFiber3d();

    Fiber* getTheFiber(void);

    int update(void);

private:

    std::unique_ptr<UniaxialFiber3d> theUniaxialFiber3d;
    int createUniaxialFiber3d(void);
};

#endif


