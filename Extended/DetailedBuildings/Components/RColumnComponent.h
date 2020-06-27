#ifndef RColumnComponent_H
#define RColumnComponent_H
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

// Parent class of a column component, classes that are derived from this class inherit its information and functionality

#include "RBeamColumnComponent.h"

class RColumnComponent : public RBeamColumnComponent
{
    Q_OBJECT
    Q_PROPERTY(double DriftRatio READ getDrift)
    Q_PROPERTY(double AxialForce READ getAxialLoad)

public:
    RColumnComponent(QObject *parent, QString &name);
    ~RColumnComponent();

    // Get the column drift
    double getDrift();

    int commitState();

    // Axial load in column in N
    virtual double getAxialLoad();

private:


};

#endif
