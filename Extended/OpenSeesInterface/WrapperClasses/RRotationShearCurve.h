#ifndef RRotationShearCurve_H
#define RRotationShearCurve_H
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

// Wrapper class for OpenSees LimitCurve
// Stevan Gavrilovic 12/2018

// This class creates a shear limit curve for incorporating shear damage
//    MODE 1: Direct Input
//    limitCurve RotationShearCurve $crvTag $eleTag $ndI $ndJ $rotAxis $Vn $Vr $Kdeg $rotLim

#include "RLimitCurve.h"

class RComponent;
class RParameter;
class RotationShearCurve;
class Domain;
class Element;
class Node;

class RRotationShearCurve : public RLimitCurve
{
    Q_OBJECT

public:
    RRotationShearCurve(QObject *parent, QString name);
    ~RRotationShearCurve();

    LimitCurve* getTheLimitCurve();
    int update(void);

    RComponent *getTheComponent() const;
    void setTheComponent(RComponent *value);

    int getEleTag() const;
    void setEleTag(int value);

    int getNdI() const;
    void setNdI(int value);

    int getNdJ() const;
    void setNdJ(int value);

    int getRotAxis() const;
    void setRotAxis(int value);

    RParameter *getKdeg() const;
    void setKdeg(RParameter *value);

    RParameter *getVr() const;
    void setVr(RParameter *value);

    RParameter *getVn() const;
    void setVn(RParameter *value);

    RParameter *getRotLim() const;
    void setRotLim(RParameter *value);

private:

    int createRotationShearCurve(void);
    int initialize(void);

    RComponent* theComponent;

    std::unique_ptr<LimitCurve> theRotationShearCurve;

    int eleTag;  //    $eleTag	integer element tag to define the associated beam-column element used to extract axial load
    int ndI;     //    $ndI	integer node tag to define the node at the extreme end of the frame member bounding the plastic hinge (L1 or D1 for bottom spring and L4 or D6 for top spring in Figure)
    int ndJ;     //    $ndJ	integer node tag to define the node bounding the plastic hinge (L2 or D3 for bottom spring and L3 or D4 for top spring in Figure)
    //    rotAxis = 3 – Rotation about z-axis – 2D
    //    rotAxis = 4 – Rotation about x-axis – 3D
    //    rotAxis = 5 – Rotation about y-axis – 3D
    //    rotAxis = 6 – Rotation about z-axis – 3D
    int rotAxis;        //    $rotAxis	integer to indicate axis of measured rotation when triggering lateral-strength degradation
    //    Vn = -1 – strength limit is not used.
    //    Vn > 0 – strength limit is the input value
    RParameter* VnParameter;     //    $Vn	floating point value to define the ultimate strength in material model
    //    Vr = -1 – Residual strength = 0.2*(max. force in material model at initiation of degradation)
    //    -1 < Vr < 0 – Residual shear strength = Vr*(max. force in material model at initiation of degradation)
    //    Vr > 0 – Residual strength is the input value
    RParameter* VrParameter;     //    $Vr	floating point value to define the backbone residual strength
    //    Note: the degrading slope must be less than zero;
    RParameter* KdegParameter;   //    $Kdeg	floating point value to define the backbone degrading slope of the material model.
    //    $rotLim	floating point value to limit the rotational capacity across the plastic hinge (difference between $ndI and $ndJ in absolute value).
    //    When this value (radians) is exceeded during the analysis degrading behavior is triggered in the material model.
    RParameter* rotLimParameter;

    Domain* theOSDomain;
    Element *theElement;
    Node *theNodeI;
    Node *theNodeJ;

};

#endif
