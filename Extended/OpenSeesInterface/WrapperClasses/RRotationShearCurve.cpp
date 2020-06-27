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

#include "RRotationShearCurve.h"
#include "RComponent.h"
#include "RConcreteComponent.h"
#include "RBeamColumnComponent.h"

#include "LimitCurve.h"
#include "RotationShearCurve.h"

RRotationShearCurve::RRotationShearCurve(QObject *parent, QString name)
    : RLimitCurve(parent, name)
{
    theRotationShearCurve = nullptr;
    eleTag = -1;
    ndI = -1;
    ndJ = -1;
    rotAxis = -1;
    VnParameter = nullptr;
    VrParameter = nullptr;
    KdegParameter  = nullptr;
    rotLimParameter = nullptr;
    theOSDomain = nullptr;
    theElement = nullptr;
    theNodeI = nullptr;
    theNodeJ = nullptr;
}


RRotationShearCurve::~RRotationShearCurve()
{

}


int RRotationShearCurve::update(void)
{
    theRotationShearCurve.reset();

    return this->initialize();
}


LimitCurve* RRotationShearCurve::getTheLimitCurve()
{
    if(!theRotationShearCurve)
        this->createRotationShearCurve();

    return theRotationShearCurve.get();
}


RComponent *RRotationShearCurve::getTheComponent() const
{
    return theComponent;
}

void RRotationShearCurve::setTheComponent(RComponent *value)
{
    theComponent = value;
}


int RRotationShearCurve::createRotationShearCurve(void)
{  
    //    $crvTag	unique limit curve object integer tag
    //    $eleTag	integer element tag to define the associated beam-column element used to extract axial load
    //    $ndI	integer node tag to define the node at the extreme end of the frame member bounding the plastic hinge (L1 or D1 for bottom spring and L4 or D6 for top spring in Figure)
    //    $ndJ	integer node tag to define the node bounding the plastic hinge (L2 or D3 for bottom spring and L3 or D4 for top spring in Figure)
    //    $rotAxis	integer to indicate axis of measured rotation when triggering lateral-strength degradation
    //    rotAxis = 3 – Rotation about z-axis – 2D
    //    rotAxis = 4 – Rotation about x-axis – 3D
    //    rotAxis = 5 – Rotation about y-axis – 3D
    //    rotAxis = 6 – Rotation about z-axis – 3D
    //    $Vn	floating point value to define the ultimate strength in material model
    //    Vn = -1 – strength limit is not used.
    //    Vn > 0 – strength limit is the input value
    //    $Vr	floating point value to define the backbone residual strength
    //    Vr = -1 – Residual strength = 0.2*(max. force in material model at initiation of degradation)
    //    -1 < Vr < 0 – Residual shear strength = Vr*(max. force in material model at initiation of degradation)
    //    Vr > 0 – Residual strength is the input value
    //    $Kdeg	floating point value to define the backbone degrading slope of the material model.
    //    Note: the degrading slope must be less than zero.
    //    $rotLim	floating point value to limit the rotational capacity across the plastic hinge (difference between $ndI and $ndJ in absolute value). When this value (radians) is exceeded during the analysis degrading behavior is triggered in the material model.

    //Get the rotation axis
    if(rotAxis == -1)
    {
        qDebug()<<"Error, need to set the 'rotation axis' parameter in "<<this->objectName();
        return -1;
    }


    if (KdegParameter == nullptr)
    {
        qDebug()<<"Error, need to set the 'Kdeg' parameter in "<<this->objectName();
        return -1;
    }

    if (rotLimParameter == nullptr)
    {
        qDebug()<<"Error, need to set the 'Rotation Limit' parameter in "<<this->objectName();
        return -1;
    }

    if(theOSDomain == nullptr)
        theOSDomain = theDomain->getTheOpenSeesDomain();

    theElement = theOSDomain->getElement(eleTag);
    if (theElement == nullptr)
    {
        qDebug()<<"Error, getting OpenSees element in "<<__FUNCTION__;
        return -1;
    }

    theNodeI = theOSDomain->getNode(ndI);
    if (theNodeI == nullptr)
    {
        qDebug()<<"Error, getting OpenSees node in "<<__FUNCTION__;
        return -1;
    }

    theNodeJ = theOSDomain->getNode(ndJ);
    if (theNodeJ == nullptr)
    {
        qDebug()<<"Error, getting OpenSees node in "<<__FUNCTION__;
        return -1;
    }

    return this->initialize();
}


int RRotationShearCurve::initialize(void)
{

    // -10.0 ksi, source: http://opensees.berkeley.edu/wiki/index.php/PinchingLimitStateMaterial_Example
    double KdegVal = KdegParameter->getCurrentValue();
    // double KdegVal = -68947572.8;

    double rotLim = rotLimParameter->getCurrentValue();
    // double rotLim  =  0.0175;

    // Default - strength limit is not used
    auto Vn = -1.0;

    // Default - Vr = -1 – Residual strength = 0.2*(max. force in material model at initiation of degradation)
    // -1 < Vr < 0 – Residual shear strength = Vr*(max. force in material model at initiation of degradation)
    // Vr > 0 – Residual strength is the input value
    auto Vr = -1.0;

    // If the parameter Vn is given, then update the value from default
    if(VnParameter != nullptr)
    {
        Vn = VnParameter->getCurrentValue();
    }

    // If the parameter Vr is given, then update the value from default
    if(VrParameter != nullptr)
    {
        Vr = VrParameter->getCurrentValue();
    }


    // limitCurve RotationShearCurve $crvTag $eleTag $ndI $ndJ $rotAxis $Vn $Vr $Kdeg $rotLim
    theRotationShearCurve = std::make_unique<RotationShearCurve>(objectTag, eleTag,ndI,ndJ,rotAxis,Vn,Vr,KdegVal,rotLim, 0,
                                                                 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                                                 0.0, 0.0, 0.0, theOSDomain, theElement, theNodeI, theNodeJ);

    if(theRotationShearCurve)
        return 0;

    return -1;
}


RParameter *RRotationShearCurve::getRotLim() const
{
    return rotLimParameter;
}


void RRotationShearCurve::setRotLim(RParameter *value)
{
    rotLimParameter = value;
}


RParameter *RRotationShearCurve::getVn() const
{
    return VnParameter;
}


void RRotationShearCurve::setVn(RParameter *value)
{
    VnParameter = value;
}


RParameter *RRotationShearCurve::getVr() const
{
    return VrParameter;
}


void RRotationShearCurve::setVr(RParameter *value)
{
    VrParameter = value;
}


RParameter *RRotationShearCurve::getKdeg() const
{
    return KdegParameter;
}


void RRotationShearCurve::setKdeg(RParameter *value)
{
    KdegParameter = value;
}


int RRotationShearCurve::getRotAxis() const
{
    return rotAxis;
}


void RRotationShearCurve::setRotAxis(int value)
{
    rotAxis = value;
}

int RRotationShearCurve::getNdJ() const
{
    return ndJ;
}


void RRotationShearCurve::setNdJ(int value)
{
    ndJ = value;
}


int RRotationShearCurve::getNdI() const
{
    return ndI;
}


void RRotationShearCurve::setNdI(int value)
{
    ndI = value;
}


int RRotationShearCurve::getEleTag() const
{
    return eleTag;
}


void RRotationShearCurve::setEleTag(int value)
{
    eleTag = value;
}


