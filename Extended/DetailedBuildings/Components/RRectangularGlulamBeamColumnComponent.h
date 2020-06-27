#ifndef RRectangularGlulamBeamColumnComponent_H
#define RRectangularGlulamBeamColumnComponent_H
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

//   Point1                           Point2
//           1       2       3      ..n
//       .-------.-------.-------.---->
//

#include "RColumnComponent.h"

class RPoint;
class RSectionKnowledge;
class RContinuousRandomVariable;
class RGlulamMaterialKnowledge;
class RBeamColumnMesh;
class RLinearElasticBeamColumnMesh;
class RRectangularLinearElasticSectionKnowledge;

class RRectangularGlulamBeamColumnComponent : public RColumnComponent
{
    Q_OBJECT 
    Q_PROPERTY(QObject *Width READ getCrossSectionWidth WRITE setCrossSectionWidth)
    Q_PROPERTY(QObject *Depth READ getCrossSectionDepth WRITE setCrossSectionDepth)

public:
    RRectangularGlulamBeamColumnComponent(QObject *parent, QString &name);
    ~RRectangularGlulamBeamColumnComponent() override;

    // Create the finite element mesh
    int createMesh() override;

    int commitState();

    // Get the construction cost of component (Gurvinder)
    double getConstructionCost() override;

    // Get the number of worker hours for construction (Gurvinder)
    double getNumberOfWorkerHoursForConstruction() override;

    // Volume of wood in m3
    double getVolumeOfWood() override;

    // The width of the rectangular cross-section in m
    void setCrossSectionWidth(QObject *value);
    inline RParameter *getCrossSectionWidth(){return theCrossSectionWidthParameter;}

    // The depth of the rectangular cross-section in m
    void setCrossSectionDepth(QObject *value);
    inline RParameter *getCrossSectionDepth(){return theCrossSectionDepthParameter;}

    // Material knowledge containing material properties
    RGlulamMaterialKnowledge* getTheGlulamMaterial(void) const;

private:
    QPointer<RParameter> theCrossSectionWidthParameter;
    QPointer<RParameter> theCrossSectionDepthParameter;

    RContinuousRandomVariable *theTheta1;
    RContinuousRandomVariable *theTheta2;
    RContinuousRandomVariable *theEpsilon;

    // Inelastic analysis parameters
    int numElements;
    int numSections;
    int numFibresAlong12;
    int numFibresAlong23;

    // Material knowledge containing material properties
    unique_ptr<RGlulamMaterialKnowledge> theGlulamMaterial;

    // Vector of nodes
    QVector<RNode *> theNodeVector;

};

#endif
