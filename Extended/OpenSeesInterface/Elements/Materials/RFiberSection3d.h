#ifndef RFiberSection3d_H
#define RFiberSection3d_H
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

//Wrapper class for OpenSees FiberSection3d
//Stevan Gavrilovic 12/2018

#include "RSectionForceDeformation.h"

class RFiber;
class RUniaxialMaterial;
class SectionForceDeformation;
class FiberSection3d;
class Fiber;

class RFiberSection3d : public RSectionForceDeformation
{
    Q_OBJECT

public:

    RFiberSection3d(QObject *parent, QString &name);
    ~RFiberSection3d();

    SectionForceDeformation* getTheSectionForceDeformation(void);
    int update(void);

    std::vector<RFiber*> getFibers() const;
    void setFibers(std::vector<RFiber*> value);

    int getNumFibres() const;

    RUniaxialMaterial *getTorsion() const;

    void setTorsion(RUniaxialMaterial *value);

    int generateOpenSeesInput(std::string& object, int flag);

private:

    int createFiberSection3d();

    std::unique_ptr<FiberSection3d> theFiberSection3d;
    std::vector<RFiber*> fibers;
    RUniaxialMaterial *torsionMaterial;

};

#endif
