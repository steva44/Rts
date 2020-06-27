#ifndef RDamageRegion_H
#define RDamageRegion_H
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

// Damage region face indices; for when surfaces of a region may have different types of damage, such as the top and bottom of a slab
//
//    *--------*
//   /|  2    /|
//  / |      / |
// *--|---4-*  |
// |0 |     | 3|
// |  *-1---+--*
// | /      | /
// |/   5   |/
// *--------*
//
// Front = 1
// Back = 4
// Top = 2
// Bottom =5
// Left = 0
// Right = 3

#include <vector>
#include <string>

class RDamageRegion
{

public:
    RDamageRegion();
    virtual ~RDamageRegion();

    // The local damage type for the damage region (according to the index)
    virtual int getDamageType(size_t index);
    virtual void setDamageType(size_t index, const int mode);

    // The local damage "type" in text, i.e, spalling, cracking, etc.
    virtual void setDamageTypeString(size_t index, const std::string &value);
    virtual std::string getDamageTypeString(size_t index);

    virtual void reset() = 0;

    // The global damage type for the entire component
    int getGlobalDamageType(void);
    void setGlobalDamageType(const int mode);

    // The global damage "type" in text, i.e, crushing, fracture, etc.
    std::string getGlobalDamageTypeString(void);
    void setGlobalDamageTypeString(const std::string type);

    // The crack orientation
    virtual double getCrackOrientation(size_t index) const;

    void resetDamageRegion(void);

protected:

    std::vector<int> damageTypes;

    std::vector<std::string> damageTypeString;
    
};


#endif
