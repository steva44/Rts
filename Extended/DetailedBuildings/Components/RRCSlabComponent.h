#ifndef RRCSlabComponent_H
#define RRCSlabComponent_H
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
*   - Terje Haukaas                                                  *
*   - Gurvinder Gill                                                 *
*   - Seadon Chui                                                    *
*                                                                    *
*********************************************************************/

// Things that need to be set for this component to work
// Thickness [Parameter]
// Point1 [Parameter]
// Point2 [Parameter]
// Point3 [Parameter]
// Point4 [Parameter]
// LongitudinalReinforcementRatio [Parameter] OR set the longitudinal spacing [double] -> this is created automatically if one is not set with default spacing of 200mm
// TransverseReinforcementRatio [Parameter] OR set the transverse spacing [double] -> this is created automatically if one is not set with default spacing of 200mm
// ConcreteStrength [String] -> default value is C30
// ConcreteCover [Parameter] -> this is created automatically if one is not set with a default value of 1" or 25mm
// longitudinalRebarDesignation [String] -> default value is 15M
// transverseRebarDesignation [String] -> default value is 15M
//   y
//  ^
//  |__>x
//        Longitudinal along x
//   __________________________
//   |                        |
//   |    |    |    |    |    |
//   |  --|----|----|----|--  |  Transverse along y
//   |    |    |    |    |    |
//   |  --|----|----|----|--  |
//   |    |    |    |    |    |
//   |________________________|
//
//   4_____________3    /
//   |             |    |
//   |             |    |
//   |             |    | DEPTH
//   |             |    |
//   |             |    |
//  1|_____________|2   /
//
//   /-------------/
//        WIDTH

#include "RPlateComponent.h"
#include "RConcreteComponent.h"

class RMesh;
class RDamageMesh;

class RRCSlabComponent : public RPlateComponent, public RConcreteComponent
{
    Q_OBJECT

public:
    RRCSlabComponent(QObject *parent, QString &name);
    ~RRCSlabComponent();

    // Create the finite element mesh
    int createMesh();

    int commitState();

    // Get the number of worker hours for construction (Gurvinder)
    double getNumberOfWorkerHoursForConstruction();

    // Volume of concrete in m3
    double getVolumeOfConcrete();

    // The volume of steel in m^3
    double getVolumeOfSteel();

    // The cost of maintanining this component (Seadon)
    double getServiceabilityCost();

    // Volume of reinforcing steel in m3
    double getVolumeOfReinforcement();

    // RSI for roof slab energy loss calculations
    double getRSIValue();

    // Finite element mesh
    inline RMesh* getMesh(){return theMesh.get();}

private:

    int createComponentSpecificParameters();

    // This function creates a hexahedron out of 6 faces that will be used for visualization of the component
    int createTopologicalMesh(void);

    // The finite element mesh
    unique_ptr<RMesh> theMesh;

};

#endif
