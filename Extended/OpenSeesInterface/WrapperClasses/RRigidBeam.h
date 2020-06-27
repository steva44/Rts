#ifndef RRigidBeam_H
#define RRigidBeam_H
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

// Wrapper class for OpenSees RigidBeam
// Stevan Gavrilovic 12/2018

#include "RObject.h"
#include "RigidBeam.h"

class RNode;
class RigidBeam;
class Domain;

class RRigidBeam : public RObject
{
    Q_OBJECT

public:
    RRigidBeam(QObject *parent, QString name);
    ~RRigidBeam();

    int  update(void);
    void print(void);

    int getNodeR() const;
    void setNodeR(int value);

    int getNodeC() const;
    void setNodeC(int value);

    int generateOpenSeesInput(std::string& object, int flag);

    int createRigidBeamElement(void);

private:

    Domain* theOpenSeesDomain;
    int nodeR;
    int nodeC;

    int initialize(void);

};

#endif
