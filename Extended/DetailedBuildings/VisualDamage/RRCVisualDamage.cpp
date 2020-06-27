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

#include "RRCVisualDamage.h"

RRCVisualDamage::RRCVisualDamage()
{
    areaDamage = 0.0;
    volumeDamage = 0.0;
    depthDamage = 0.0;
    maxStrainSpall = 0.0;

    numCracks = 0;
}


RRCVisualDamage::~RRCVisualDamage()
{

}


void RRCVisualDamage::reset(void)
{
    areaDamage = 0.0;
    volumeDamage = 0.0;
    depthDamage = 0.0;
    maxStrainSpall = 0.0;

    numCracks = 0;

    crackWidthVec.clear();
    crackLengthVec.clear();
    crackDepthVec.clear();
    crackVolumeVec.clear();
    crackOrientationVec.clear();
    maxCrackWidth.clear();
}


void RRCVisualDamage::resize(const int& value)
{
    if(value == numCracks)
        return;

    numCracks = value;

    crackWidthVec.resize(numCracks);
    crackLengthVec.resize(numCracks);
    crackDepthVec.resize(numCracks);
    crackVolumeVec.resize(numCracks);
    crackOrientationVec.resize(numCracks);
    maxCrackWidth.resize(numCracks);
}


int RRCVisualDamage::getNumCracks(void)
{
    return numCracks;
}


