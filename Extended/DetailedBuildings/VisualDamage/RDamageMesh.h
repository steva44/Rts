#ifndef RDamageMesh_H
#define RDamageMesh_H
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

// Abstract base class for all types of damage meshes

#include <QVector>
#include <string>
#include <memory>

class RDamageRegion;
class RComponent;

class RDamageMesh
{

public:
    RDamageMesh(RComponent* component);
    virtual ~RDamageMesh();

    // Reset the damage mesh
    virtual void resetDamageMesh(void) = 0;

    // Create the damage mesh
    virtual int createDamageMesh(void) = 0;

    // Vector of damage regions
    virtual std::vector<std::shared_ptr<RDamageRegion>> getDamageRegionVector() const = 0;
    virtual void setDamageRegionVector(const std::vector<std::shared_ptr<RDamageRegion> > &value) = 0;

protected:

    // Component containing the damage mesh
    RComponent* theComponent;

};

#endif
