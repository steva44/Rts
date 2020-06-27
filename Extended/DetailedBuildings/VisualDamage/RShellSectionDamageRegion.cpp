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

#include "RShellSectionDamageRegion.h"
#include "RLinearShellElement.h"
#include "RElement.h"
#include "RSectionForceDeformation.h"

RShellSectionDamageRegion::RShellSectionDamageRegion()
    : RDamageRegion()
{

    damageInfo.resize(6);

    theCrackDirectionArrow = nullptr;

    this->reset();
}


RShellSectionDamageRegion::~RShellSectionDamageRegion()
{

}


void RShellSectionDamageRegion::reset(void)
{ 
    for(auto&& it : damageInfo)
    {
        it.reset();
        it.resize(1);
    }

    this->resetDamageRegion();

    return;
}


double RShellSectionDamageRegion::getCrackOrientation(size_t index) const
{
    return damageInfo.at(index).crackOrientationVec[0];
}


double RShellSectionDamageRegion::getCrackDepth(size_t index) const
{
    return damageInfo.at(index).crackDepthVec[0];

}


double RShellSectionDamageRegion::getCrackLength(size_t index) const
{
    return damageInfo.at(index).crackLengthVec[0];
}


double RShellSectionDamageRegion::getCrackVolume(size_t index) const
{
    return damageInfo.at(index).crackVolumeVec[0];
}


osg::PositionAttitudeTransform* RShellSectionDamageRegion::getTheCrackDirectionArrow() const
{
    return theCrackDirectionArrow.get();
}


void RShellSectionDamageRegion::setTheCrackDirectionArrow(const osg::ref_ptr<osg::PositionAttitudeTransform> &value)
{
    theCrackDirectionArrow = value;
}


RLinearShellElement *RShellSectionDamageRegion::getTheElement() const
{
    return theElement;
}


void RShellSectionDamageRegion::setTheElement(RLinearShellElement *value)
{
    theElement = value;

    theElement->setTheDamageRegion(this);
}


double RShellSectionDamageRegion::getSurfaceArea(void)
{
    return theElement->getArea();
}


double& RShellSectionDamageRegion::getMaxCrackWidth(size_t index)
{
    return damageInfo.at(index).maxCrackWidth[0];
}


void RShellSectionDamageRegion::setCrackParameters(size_t index, const double& width,const double& depth,const double& length, const double& orientation)
{
    damageInfo.at(index).crackWidthVec[0] = width;
    damageInfo.at(index).crackDepthVec[0] = depth;
    damageInfo.at(index).crackLengthVec[0] = length;
    damageInfo.at(index).crackOrientationVec[0] = orientation;
    damageInfo.at(index).crackVolumeVec[0] = width*depth*length;
}


double RShellSectionDamageRegion::getLength22()
{
    // Length of vector from 1 to 2
    double length23 = theElement->getLength23();
    double length41 = theElement->getLength41();

    auto length2 = 0.5*(length23+length41);

    return length2;
}


double RShellSectionDamageRegion::getLength11()
{
    double length12 = theElement->getLength12();
    double length34 = theElement->getLength34();

    auto length1 = 0.5*(length12+length34);

    return length1;
}


void RShellSectionDamageRegion::setSurfaceAreaOfDamage(size_t index, const double& value)
{
    damageInfo.at(index).areaDamage = value;
}


double RShellSectionDamageRegion::getDamageSurfaceArea(size_t index)
{
    return damageInfo.at(index).areaDamage;
}


void RShellSectionDamageRegion::setDepthOfDamage(size_t index,const double& value)
{
    damageInfo.at(index).depthDamage = value;
}


double RShellSectionDamageRegion::getDamageDepth(size_t index)
{
    return damageInfo.at(index).depthDamage;
}


void RShellSectionDamageRegion::setVolumeOfDamage(size_t index, const double& value)
{
    damageInfo.at(index).volumeDamage = value;
}


double RShellSectionDamageRegion::getDamageVolume(size_t index)
{
    return damageInfo.at(index).volumeDamage;
}


QVector<double> RShellSectionDamageRegion::getStrainResponse(void)
{
    // Even though we will not be using the forces here - need to get the forces for this element before we can get the strains, otherwise strains will be zero
    QVector<double> forceResponse(24);
    theElement->getGlobalResponse(&forceResponse,"Forces");

    // Get the element strains
    QVector<double> strainResponse(8);
    // membrane strains at each integration point (4) total points:
    // response(0) =   eps_xx     i.e.   (11)-strain
    // response(1) =   eps_yy     i.e.   (22)-strain
    // response(2) =   gamma_xy   i.e.   (12)-shear
    // response(3) =   kappa_xx   i.e.   (11)-curvature
    // response(4) =   kappa_yy   i.e.   (22)-curvature
    // response(5) = 2*kappa_xy   i.e. 2*(12)-curvature
    // response(6) =   gamma_xz   i.e.   (13)-shear
    // response(7) =   gamma_yz   i.e.   (23)-shear
    // Shear strains gamma02, gamma12 constant through cross section
    theElement->getLocalResponse(&strainResponse,"Strain");

    return strainResponse;
}


QVector<double> RShellSectionDamageRegion::getStressResponse(void)
{
    // Even though we will not be using the forces here - need to get the forces for this element before we can get the strains, otherwise strains will be zero
    QVector<double> forceResponse(24);
    theElement->getGlobalResponse(&forceResponse,"Forces");

    // Get the element stresses
    QVector<double> stressResponse(8);
    // response(0) =   sigma_xx                                                       i.e.   (11)-axial strain
    // response(1) =   sigma_yy                                                       i.e.   (22)-axial strain
    // response(2) =   tau_xy                                                         i.e.   (12)-in-plane shear strain
    // response(3) =   sigma_xx = -z * kappa00  + eps00_membrane                      i.e.   (11)-bending curvature PLUS axial strain
    // response(4) =   sigma_yy = -z * kappa11  + eps11_membrane                      i.e.   (22)-bending curvature PLUS axial strain
    // response(5) =   tau_xy   = 2*epsilon01 = -z * (2*kappa01) + gamma01_membrane   i.e. 2*(12)-bending curvature PLUS in-plane shear strain
    // response(6) =   tau_xz                                                         i.e.   (13)-out-of-plane shear
    // response(7) =   tau_yz                                                         i.e.   (23)-out-of-plane shear
    theElement->getLocalResponse(&stressResponse,"Stress");

    return stressResponse;
}


double& RShellSectionDamageRegion::getMaxStrainSpall(size_t index)
{
    return damageInfo.at(index).maxStrainSpall;
}


