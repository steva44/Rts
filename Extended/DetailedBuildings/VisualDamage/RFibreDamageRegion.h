#ifndef RFibreDamageRegion_H
#define RFibreDamageRegion_H
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

// Abstract base class for a damage region that depends on fibres for its stresses and strains

#include "RDamageRegion.h"

#include <vector>
#include <string>
#include <unordered_map>

class RFiber;
class RNonlinearBeamColumnElement;
class RFiberSection3d;

// Need this struct to make the link between the OpenSees fibers and the RFibers
struct fibreInfo{

    int fibreTag = -1;
    RFiber* associatedFiber = nullptr;

    double yLoc = 0.0;
    double zLoc = 0.0;
    double area = 0.0;
    double stress = 0.0;
    double strain = 0.0;
    double length = 0.0;
};



class RFibreDamageRegion : public RDamageRegion
{

public:
    RFibreDamageRegion();
    virtual ~RFibreDamageRegion();

    virtual void reset(void);

    // Fibre stresses and strains
    double getFibreStrain(int tag);
    double getFibreStress(int tag);

    std::unordered_map<int, fibreInfo>* getFiberMap();

    // Number of fibres
    virtual int numFibres(void) = 0;

    // Update the fibre responses from the finite elements
    int updateFibreResponses(void);

    // Distance to centroid of the section
    double getYBar() const;
    double getZBar() const;

    // Fibre length in m
    void setFibreLength(double value);
    double getFibreLength() const;

    // Fibre section linked to this damage region
    RFiberSection3d *getTheSection() const;
    void setTheSection(RFiberSection3d *value);

    // Element linked to this damage region
    RNonlinearBeamColumnElement *getTheElement() const;
    void setTheElement(RNonlinearBeamColumnElement *value);

protected:

    // Fibre properties
    double fibreLength;
    double yBar;
    double zBar;

    // Element linked to this damage region
    RNonlinearBeamColumnElement* theElement;

    // Fibre section linked to this damage region
    RFiberSection3d* theSection;

    // Map to store the results
    std::unordered_map<int,fibreInfo> fiberMap;

};







#endif
