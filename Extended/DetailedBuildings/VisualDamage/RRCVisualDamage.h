#ifndef RRCVisualDamage_H
#define RRCVisualDamage_H
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

// This class stores the quantities of visual damage for reinforced concrete components, with an emphasis on reinforced concrete damage types such as cracking, spalling, etc.

#include <vector>
#include <stddef.h>

class RRCVisualDamage
{
public:
    RRCVisualDamage();
    ~RRCVisualDamage();

    void resize(const int& value);

    // Number of cracks
    int getNumCracks(void);

    // Vectors to store the damage quantites for cracking
    std::vector<double> crackWidthVec;
    std::vector<double> crackLengthVec;
    std::vector<double> crackDepthVec;
    std::vector<double> crackVolumeVec;
    std::vector<double> crackOrientationVec;

    // Store the crack width in a vector since there could be multiple cracks
    std::vector<double> maxCrackWidth;

    // Other damage parameters that are not cracking related
    double areaDamage;
    double volumeDamage;
    double depthDamage;
    double maxStrainSpall;

    // Reset the damage
    void reset(void);

private:

    size_t numCracks;

};


#endif
