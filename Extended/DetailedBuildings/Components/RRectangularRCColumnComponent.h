#ifndef RRectangularRCColumnComponent_H
#define RRectangularRCColumnComponent_H
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
*   - Abbas Javaherian                                               *
*                                                                    *
*********************************************************************/

#include "RColumnComponent.h"
#include "RConcreteComponent.h"

class RFibreSection;
class RContinuousRandomVariable;

// Things that need to be set for this component to work
// CrossSectionDepth [Parameter]
// CrossSectionWidth [Parameter]
// Point1 [Parameter]
// Point2 [Parameter]
// LongitudinalReinforcementRatio [Parameter]  -> this is created automatically if one is not set with default value of 0.02 or 2%
// ConcreteStrength  [String] -> default value is C25
// TransverseReinforcementRatio [Parameter] OR set the transverse spacing [double] -> this is created automatically if one is not set with default spacing of 200mm
// ConcreteCover [Parameter] -> this is created automatically if one is not set with a default value of 1" or 25mm
// longitudinalRebarDesignation [String] -> default value is 15M
// transverseRebarDesignation [String] -> default value is 10M
// typeTransverseReinforcement -> default value is S1

//   Point1                           Point2
//           1       2       3      ..n
//       .-------.-------.-------.---->
//

class RRectangularRCColumnComponent : public RColumnComponent, public RConcreteComponent
{
    Q_OBJECT
    
    Q_PROPERTY(QObject* CrossSectionDepth READ getCrossSectionDepth WRITE setCrossSectionDepth)
    Q_PROPERTY(QObject* CrossSectionWidth READ getCrossSectionWidth WRITE setCrossSectionWidth)
    Q_PROPERTY(double CrackLength READ getCrackLength)

public:
    RRectangularRCColumnComponent(QObject *parent, QString &name);
    ~RRectangularRCColumnComponent();

    // Create the finite element mesh
    int createMesh();

    // The cross-section depth in m
    void setCrossSectionDepth(QObject *value);
    inline RParameter* getCrossSectionDepth(){return theCrossSectionDepthParameter;}

    // The cross-section width in m
    void setCrossSectionWidth(QObject *value);
    inline RParameter* getCrossSectionWidth(){return theCrossSectionWidthParameter;}

    // Total volume of reinforcing steel in m3
    double getTotalVolumeOfReinforcement();

    // Volume of transverse reinforcing steel in m3
    double getVolumeOfTransverseReinforcement();

    // Volume of longitudinal reinforcing steel in m3
    double getVolumeOfLongitudinalReinforcement();

    // Volume of concrete in m3
    double getVolumeOfConcrete();

    // Volume of reinforcing steel in m3
    double getVolumeOfSteel();

    // Surface area of entire component in m2
    double getSurfaceArea(void);

    // Surface area along the width direction in m2
    double getSurfaceAreaWidth();

    // Surface area along the depth direction in m2
    double getSurfaceAreaDepth();

    // Surface area of the cross-section in m2
    double getSurfaceAreaCrossSection();

    // Total volume of component in m3
    double getVolume();

    // Axial load on the column in N
    double getAxialLoad();

    // Number of longitudinal reinforcement bars
    int getNumberOfLongitudinalRebar();

    // Type of transverse reinforcement
    QString getTypeTransverseReinforcement(void) const;

    // Returns the crack length in m
    double getCrackLength();

private:

    int createComponentSpecificParameters(void);

    // Cross-section parameters
    QPointer<RParameter> theCrossSectionDepthParameter;
    QPointer<RParameter> theCrossSectionWidthParameter;

    // Crack length model parameters
    RContinuousRandomVariable *theTheta1;
    RContinuousRandomVariable *theTheta2;
    RContinuousRandomVariable *theTheta3;
    RContinuousRandomVariable *theEpsilon;
    RContinuousRandomVariable *theSigma;

    // This function creates a hexahedron out of 6 faces that will be used for visualization of the component
    int createTopologicalMesh(void);

    // A list of the fibre discretized sections
    QList<RFibreSection *> theSectionList;

    //Inelastic analysis parameters
    int numElements;
    int numSections;
    int numFibresInCoreAlong12;
    int numFibresInCoreAlong23;
    int numFibresInCoverAlong12;
    int numFibresInCoverAlong23;
    int rebarAlong12;
    int rebarAlong23;
    int rebarAlong34;
    int rebarAlong41;

    //The type of transverse reinforcement -- hoop reinforcement since this is a column
    //    S1	square section with S1 type of transverse reinforcement
    //    S2	square section with S2 type of transverse reinforcement
    //    S3	square section with S3 type of transverse reinforcement
    //    S4a	square section with S4a type of transverse reinforcement
    //    S4b	square section with S4b type of transverse reinforcement
    //    S5	square section with S5 type of transverse reinforcement
    //    R	rectangular section
    //    L$ is length/diameter of square/circular core section measured respect to the hoop center line [mm]
    //    L1 is required for all sections
    //    L2 is required when using section type S4a, or S4b or R
    //    L3 is required when using section type S4a
    // Go to http://opensees.berkeley.edu/wiki/index.php/ConfinedConcrete01_Material for more info and diagrams

    QString typeTransverseReinforcement;

};



#endif
