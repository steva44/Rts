#ifndef RSteelBarTrussComponent_H
#define RSteelBarTrussComponent_H
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

#include "RTrussComponent.h"
#include "RSteelMaterialKnowledge.h"
#include "RSectionKnowledge.h"
#include "RGenericSectionKnowledge.h"

class RSteelMaterialKnowledge;
class RPoint;
class R1DSteelMaterial;

class RSteelBarTrussComponent : public RTrussComponent
{
    Q_OBJECT
    Q_PROPERTY(QObject* Area READ getCrossSectionArea WRITE setCrossSectionArea)

public:
    RSteelBarTrussComponent(QObject *parent, QString &name);
    ~RSteelBarTrussComponent() override;

    // Create the finite element mesh
    int createMesh() override;

    // The cross-section area in m^2
    QObject* getCrossSectionArea();
    void    setCrossSectionArea(QObject * value);

    // Volume of steel in m3
    double  getVolumeOfSteel() override;

private:

    QPointer<RParameter> theCrossSectionArea;
    unique_ptr<RSectionKnowledge> theCrossSectionKnowledge;
    unique_ptr<RMesh> theMesh;
    unique_ptr<RSteelMaterialKnowledge> theMaterial;
};

#endif
