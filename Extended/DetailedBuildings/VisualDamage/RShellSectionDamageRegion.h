#ifndef RShellSectionDamageRegion_H
#define RShellSectionDamageRegion_H
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

// This class encompasses a damage region for components with thin shell cross-sections, e.g., wall, slab

#include "RDamageRegion.h"
#include "RRCVisualDamage.h"

#include <QVector>
#include <osg/PositionAttitudeTransform>
#include <functional>

class RElement;
class RLinearShellElement;
class RSectionForceDeformation;

class RShellSectionDamageRegion : public RDamageRegion
{
public:
    RShellSectionDamageRegion();
    ~RShellSectionDamageRegion();

    // The crack orientation on the surface, i.e., angle counterclockwise from the 1-2 direction
    double getCrackOrientation(size_t index) const;

    // The depth of the crack in m
    double getCrackDepth(size_t index) const;

    // The length of the crack in m
    double getCrackLength(size_t index) const;

    // The volume of the crack in m
    double getCrackVolume(size_t index) const;

    // Set the crack parameters
    void setCrackParameters(size_t index, const double& width,const double& depth,const double& length, const double& orientation);

    // Maximum crack width experienced by the section in m
    double& getMaxCrackWidth(size_t index);

    // Average lengths along the two axes in m
    double getLength11();
    double getLength22();

    // Area of the plan surface of the shell in m2
    void setTheSurfaceArea(const std::function<double ()> value);
    double getSurfaceArea(void);

    // Surface area that is damaged - not necessarily the surface area of the container, i.e., percentage of the surface area that is damaged
    void setSurfaceAreaOfDamage(size_t index,const double& value);
    double getDamageSurfaceArea(size_t index);

    // Depth of the damage in m
    void setDepthOfDamage(size_t index, const double& value);
    double getDamageDepth(size_t index);

    // Volume of the damage in m3
    void setVolumeOfDamage(size_t index, const double& value);
    double getDamageVolume(size_t index);

    // Reset the visual damage on the surface
    void reset(void);

    // For plotting the crack direction arrow, for debugging purposes
    osg::PositionAttitudeTransform* getTheCrackDirectionArrow() const;
    void setTheCrackDirectionArrow(const osg::ref_ptr<osg::PositionAttitudeTransform> &value);

    // Element corresponding to this patch... get the stress-strain responses from this element
    RLinearShellElement *getTheElement() const;
    void setTheElement(RLinearShellElement *value);

    // Finite element strains of this damage region
    QVector<double> getStrainResponse(void);

    // Finite element stresses of this damage region
    QVector<double> getStressResponse(void);

    // Maximum spalling strain experienced by this region at any point in time
    double& getMaxStrainSpall(size_t index);

private:

    // Element linked to this damage region
    RLinearShellElement* theElement;

    // Function to return the area of the visual surface
    std::function<double(void)> theArea;

    // Determines if the damage needs updating in a time series analysis
    // This only applies to cracking and spalling - the damage modes where the amount of damage varies according to the strain
    // For example, at any time step, the strain may be less than the previous time step. But, the spalling will not be less... the amount of  spalled cover remains the same.
    // However, if the strain increases again past the previous maximum threshold, then more cover will spall!
    std::vector<RRCVisualDamage> damageInfo;

    // For plotting the crack direction arrow, for debugging purposes
    osg::ref_ptr<osg::PositionAttitudeTransform> theCrackDirectionArrow;

    // This vector stores the crack direction information for each time instant
    std::vector<std::tuple<osg::Vec3,osg::Quat,osg::Vec3>> theCrackDirectionData;

};


#endif
