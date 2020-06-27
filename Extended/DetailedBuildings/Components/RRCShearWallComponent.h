#ifndef RRCShearWallComponent_H
#define RRCShearWallComponent_H
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
*                                                                    *
*********************************************************************/

//   4_____________3    /
//   |             |    |
//   |             |    |
//   |             |    | Height
//   |             |    |
//   |             |    |
//  1|_____________|2   /
//
//   /-------------/
//        Length

// Things that need to be set for this component to work
// Thickness [Parameter]
// Point1 [Parameter]
// Point2 [Parameter]
// Point3 [Parameter]
// Point4 [Parameter]
// LongitudinalReinforcementRatio [Parameter] OR set the longitudinal spacing [double] -> this is created automatically if one is not set with default spacing of 200mm
// TransverseReinforcementRatio [Parameter] OR set the transverse spacing [double] -> this is created automatically if one is not set with default spacing of 200mm
// ConcreteStrength  [String] -> default value is C40
// ConcreteCover [Parameter] -> this is created automatically if one is not set with a default value of 1" or 25mm
// longitudinalRebarDesignation [String] -> default value is 20M
// transverseRebarDesignation [String] -> default value is 15M

#include "RWallComponent.h"
#include "RConcreteComponent.h"

class RMesh;
class RSectionKnowledge;

class RRCShearWallComponent : public RWallComponent, public RConcreteComponent
{
    Q_OBJECT
    Q_PROPERTY(int ReinforcementMeshOption READ getReinforcementMeshOption WRITE setReinforcementMeshOption)

public:
    RRCShearWallComponent(QObject *parent, QString &name);
    ~RRCShearWallComponent();

    // Create the finite element mesh
    int createMesh();

    int commitState();

    // The reinforcement mesh option
    inline void setReinforcementMeshOption(int value){reinforcementMeshOption = value;}
    inline double getReinforcementMeshOption(void){return reinforcementMeshOption;}

    // The volume of reinforcing steel in m^3
    double getVolumeofReinforcement();

    // The volume of steel in m^3
    double getVolumeOfSteel();

    // Volume of concrete in m3
    double getVolumeOfConcrete();

    // Finite element mesh
    inline RMesh* getMesh(void){return theMesh.get();}

private:

    int createComponentSpecificParameters();

    // Distributed plasticity discretization parameters
    int numElements;
    int numSections;
    int numFibresInCoreAlong12;
    int numFibresInCoreAlong23;
    int numFibresInCoverAlong12;
    int numFibresInCoverAlong23;
    int reinforcementMeshOption;

    unique_ptr<RMesh> theMesh;
    unique_ptr<RSectionKnowledge> theCrossSection;

    // This function creates a hexahedron out of 6 faces that will be used for visualization of the component
    int createTopologicalMesh();

};

#endif
