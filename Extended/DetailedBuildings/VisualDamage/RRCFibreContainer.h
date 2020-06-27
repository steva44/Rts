#ifndef RRCFibreContainer_H
#define RRCFibreContainer_H
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

// Damage region for reinforced concrete components with core, cover, and reinforcement fibres

#include "RRCVisualDamage.h"
#include "RDamageRegion.h"

#include <vector>
#include <functional>

class RFiber;
struct fibreInfo;

class RRCFibreContainer : public RDamageRegion
{
public:
    RRCFibreContainer();
    ~RRCFibreContainer();

    // Vectors for the core, cover, and reinforcement fibres
    std::vector<RFiber*> theCoreFibres;
    std::vector<RFiber*> theCoverFibres;
    std::vector<RFiber*> theReinforcementFibres;

    // For the cracking and spalling damage modes where the quantity can vary at each time step
    double getMaxCrackWidth(int& index) const;
    void setMaxCrackWidth(int& index, double& value);

    // The maximum spalling strain
    double& getMaxStrainSpall();

    // Crack parameters for this region, width, depth, length, orientation
    void setCrackParameters(const int& index, const double& width,const double& depth,const double& length, const double& orientation);

    // Number of cracks
    void setNumCracks(const int& value);

    // Crack orientation
    double getCrackOrientation(size_t index) const;

    // This returns the area of the "visible surface" of the fiber container - this is NOT the DAMAGED surface area
    void setTheSurfaceArea(const std::function<double ()>& value);
    double getSurfaceArea(void);

    // Surface area that is damaged - not necessarily the surface area of the container, i.e., percentage of the surface area that is damaged
    void setSurfaceAreaOfDamage(const double& value);
    double getDamageSurfaceArea(void);

    // Depth of the damage in m
    void setDepthOfDamage(const double& value);
    double getDamageDepth(void);

    // Volume of the damage in m3
    void setVolumeOfDamage(const double& value);
    double getVolumeOfDamage(void);

    // Clear the damage
    void reset(void);

    // Return the vectors of the various damage parameters
    std::vector<double> getCrackWidthVec() const;
    std::vector<double> getCrackLengthVec() const;
    std::vector<double> getCrackDepthVec() const;
    std::vector<double> getCrackVolumeVec() const;

private:

    // Area of the visible surface
    std::function<double(void)> theArea;

    // Determines if the damage needs updating in a time series analysis
    // This only applies to cracking and spalling - the damage modes where the amount of damage varies according to the strain
    // For example, at any time step, the strain may be less than the previous time step. But, the spalling will not be less... the amount of  spalled cover remains the same.
    // However, if the strain increases again past the previous maximum threshold, then more cover will spall!
    RRCVisualDamage theDamageInfo;

    // Private functions to set the crack properties on the various "faces" of the damage region
    // Assume that a damage region is a "brick" with 6 faces, with each index corresponding to a face
    void setCrackWidth(const int& index, const double& value);
    void setCrackLength(const int& index, const double& value);
    void setCrackDepth(const int& index, const double& value);
    void setCrackVolume(const int& index, const double& value) ;
    void setCrackOrientation(const int& index, const double& value);

};


#endif
