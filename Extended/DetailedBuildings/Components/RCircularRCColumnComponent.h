#ifndef RCircularRCColumnComponent_H
#define RCircularRCColumnComponent_H
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

// Things that need to be set for this component to work
// Diameter [Parameter]

#include "RColumnComponent.h"
#include "RConcreteComponent.h"
#include "RParameter.h"

class RBeamColumnMesh;
class RConcreteMaterialKnowledge;
class RCircularLinearElasticSectionKnowledge;

class RCircularRCColumnComponent : public RColumnComponent, public RConcreteComponent
{
    Q_OBJECT
    Q_PROPERTY(QObject *Diameter READ getDiameter WRITE setDiameter)

public:
    RCircularRCColumnComponent(QObject *parent, QString &name);
    ~RCircularRCColumnComponent();

    // Create the finite element mesh
    int createMesh();

    // The diameter of the circular cross-section in m
    RParameter *getDiameter() const;
    void setDiameter(QObject *value);

    // Gurvinder's and Seadon's implementations
    double getManufacturingCost();
    double getNumberOfWorkerHoursForConstruction();
    double getRepairCost();
    QList<QPair<double, QString> > getTriggerTimes(double startTime, double endTime);
    double getPreventativeRepairCost();

    // Surface area in m2
    double getSurfaceArea();

    // Surface area of the cross-section in m2
    double getSurfaceAreaCrossSection();

    // Volume of concrete in m3
    double getVolumeOfConcrete();

    // Volume of reinforcing steel in m3
    double getVolumeOfSteel();

    // Axial load on the column in N
    double getAxialLoad();

    // Total volume of component in m3
    double getVolume();

private:

    // The diameter of the circular cross-section
    QPointer<RParameter> theDiameter;

    // Material knowledge containing material properties
    unique_ptr<RConcreteMaterialKnowledge> theMaterial;
};

#endif
