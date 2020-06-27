#ifndef RNonLoadBearingWallComponent_H
#define RNonLoadBearingWallComponent_H
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

// This class is non-load bearing walls, i.e., interior partition walls

#include "RWallComponent.h"

class RNonLoadBearingWallComponent : public RWallComponent
{
    Q_OBJECT

public:
    RNonLoadBearingWallComponent(QObject *parent, QString &name);
    ~RNonLoadBearingWallComponent();

    // Create the finite element mesh
    int createMesh();

    // Get the construction cost of component (Gurvinder)
    double getConstructionCost();

    // Get the number of worker hours for construction (Gurvinder)
    double getNumberOfWorkerHoursForConstruction();

    // Get the repair cost of component (simple)
    double getRepairCost();

private:

    // Return the drift in this component
    double getDrift();
};

#endif
