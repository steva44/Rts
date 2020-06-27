#ifndef RShellDamageMesh_H
#define RShellDamageMesh_H
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

// Damage mesh for shell elements such as walls and slabs

#include "RShellSectionDamageRegion.h"
#include "RDamageMesh.h"

#include <QVector>
#include <string>

class RLinearShellElement;
class RComponent;

class RShellDamageMesh : public RDamageMesh
{

public:
    RShellDamageMesh(RComponent* component);
    virtual ~RShellDamageMesh();

    // Reset the damage on the surface
    void resetDamageMesh(void);

    // Get and set the visual damage regions
    std::vector<std::shared_ptr<RDamageRegion>> getDamageRegionVector() const;
    void setDamageRegionVector(const std::vector<std::shared_ptr<RDamageRegion> > &value);

    // Create the damage mesh
    int createDamageMesh(void);

    // Number of damage regions along the 1-2 direction
    size_t getNumRegionsAlong12() const;
    void setNumRegionsAlong12(const size_t &value);

    // Number of damage regions along the 2-3 direction
    size_t getNumRegionsAlong23() const;
    void setNumRegionsAlong23(const size_t &value);

private:

    // The visual damage regions of this damage mesh
    std::vector<std::shared_ptr<RDamageRegion>> theDamageRegionVector;

    // Number of damage regions along the 1-2 direction
    size_t numRegionsAlong12;

    // Number of damage regions along the 2-3 direction
    size_t numRegionsAlong23;

};







#endif
