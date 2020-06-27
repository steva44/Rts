#ifndef RHSSRoundBeamColumnComponent_H
#define RHSSRoundBeamColumnComponent_H
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

//   Point1                           Point2
//           1       2       3      ..n
//       .-------.-------.-------.---->
//

#include "RColumnComponent.h"
#include "RSectionStructs.h"
#include "RSteelMaterialKnowledge.h"
#include "RBeamColumnMesh.h"
#include "RBeamColumnComponent.h"
#include "RCircularHSSLinearElasticSectionKnowledge.h"

class RBeamColumnMesh;
class RSteelMaterialKnowledge;
class RCircularHSSLinearElasticSectionKnowledge;
class RPoint;
class R1DSteelMaterial;

class RHSSRoundBeamColumnComponent : public RColumnComponent
{
    Q_OBJECT

    Q_PROPERTY(QString Orientation READ getOrientation WRITE setOrientation)
    Q_PROPERTY(QString WallThickness READ getWallThickness WRITE setWallThickness)
    Q_PROPERTY(QString Diameter READ getDiameter WRITE setDiameter)

public:
    RHSSRoundBeamColumnComponent(QObject *parent, QString &name);
    ~RHSSRoundBeamColumnComponent();

    // Create the finite element mesh
    int createMesh();

    // The wall thickness of the HSS section in m
    inline QString getWallThickness(){return getStringParameter(&theWallThicknessDouble, theWallThicknessParameter);}
    void setWallThickness(QString value);    

    // The outer diameter of the HSS section in m
    inline QString getDiameter(){return getStringParameter(&theOuterDiameterDouble, theOuterDiameterParameter);}
    void setDiameter(QString value);

    // Volume of steel in m^3
    double getVolumeOfSteel();

    // Get the construction cost of component (simple)
    double getConstructionCost();

    // Get the number of worker hours for construction (simple)
    double getNumberOfWorkerHoursForConstruction();

    // Function for setting the standard steel section type, i.e., HSS 32x32x5/8
    inline void setStandardSection(HSSRoundSection& section){standardHSSRoundSection = section;}

private:

    // Component parameters
    QPointer<RParameter> theWallThicknessParameter;
    QPointer<RParameter> theOuterDiameterParameter;
    double theOuterDiameterDouble;
    double theWallThicknessDouble;

    // Material knowledge containing material properties
    unique_ptr<RSteelMaterialKnowledge> theMaterial;

    // The cross-section
    HSSRoundSection standardHSSRoundSection;

};

#endif
