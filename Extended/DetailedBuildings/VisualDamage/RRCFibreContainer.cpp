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

#include "RRCFibreContainer.h"
#include "RFiber.h"

RRCFibreContainer::RRCFibreContainer() : RDamageRegion()
{
    theArea = [](){return 0.0;};
}


RRCFibreContainer::~RRCFibreContainer()
{

}


void RRCFibreContainer::reset(void)
{
    theDamageInfo.reset();

    this->resetDamageRegion();
}


void RRCFibreContainer::setCrackWidth(const int& index, const double& value)
{
    theDamageInfo.crackWidthVec[index] = value;
}


void RRCFibreContainer::setCrackLength(const int& index, const double& value)
{
    theDamageInfo.crackLengthVec[index] = value;
}


void RRCFibreContainer::setCrackDepth(const int& index, const double& value)
{
    theDamageInfo.crackDepthVec[index] = value;
}


void RRCFibreContainer::setCrackVolume(const int& index, const double& value)
{
    theDamageInfo.crackVolumeVec[index] = value;
}


void RRCFibreContainer::setCrackOrientation(const int& index, const double& value)
{
    theDamageInfo.crackOrientationVec[index] = value;
}


void RRCFibreContainer::setCrackParameters(const int& index, const double& width,const double& depth,const double& length, const double& orientation)
{
    this->setCrackDepth(index,depth);
    this->setCrackWidth(index,width);
    this->setCrackLength(index,length);
    this->setCrackOrientation(index,orientation);
    this->setCrackVolume(index,depth*width*length);
}


double RRCFibreContainer::getCrackOrientation(size_t index) const
{
    auto vecCrackOrientations = theDamageInfo.crackOrientationVec;

    auto avgOrientation = 0.0;
    for(auto&& it : vecCrackOrientations)
        avgOrientation += it;

    return avgOrientation/static_cast<double>(vecCrackOrientations.size());

}


double RRCFibreContainer::getMaxCrackWidth(int& index) const
{
    return theDamageInfo.maxCrackWidth[index];
}


void RRCFibreContainer::setMaxCrackWidth(int& index, double& value)
{
    theDamageInfo.maxCrackWidth[index] = value;
}


void RRCFibreContainer::setNumCracks(const int& value)
{
    theDamageInfo.resize(value);
}


void RRCFibreContainer::setTheSurfaceArea(const std::function<double ()>& value)
{
    theArea = std::move(value);
}


double RRCFibreContainer::getSurfaceArea(void)
{
    return theArea();
}


void RRCFibreContainer::setSurfaceAreaOfDamage(const double& value)
{
    theDamageInfo.areaDamage = value;
}


double RRCFibreContainer::getDamageSurfaceArea(void)
{
    return theDamageInfo.areaDamage;
}


void RRCFibreContainer::setDepthOfDamage(const double& value)
{
    theDamageInfo.depthDamage = value;
}


double RRCFibreContainer::getDamageDepth(void)
{
    return theDamageInfo.depthDamage;
}


void RRCFibreContainer::setVolumeOfDamage(const double& value)
{
    theDamageInfo.volumeDamage = value;
}


double RRCFibreContainer::getVolumeOfDamage(void)
{
    return  theDamageInfo.volumeDamage;
}


double& RRCFibreContainer::getMaxStrainSpall()
{
    return theDamageInfo.maxStrainSpall;
}


std::vector<double> RRCFibreContainer::getCrackVolumeVec() const
{
    return theDamageInfo.crackVolumeVec;
}


std::vector<double> RRCFibreContainer::getCrackDepthVec() const
{
    return theDamageInfo.crackDepthVec;
}


std::vector<double> RRCFibreContainer::getCrackLengthVec() const
{
    return theDamageInfo.crackLengthVec;
}


std::vector<double> RRCFibreContainer::getCrackWidthVec() const
{
    return theDamageInfo.crackWidthVec;
}


