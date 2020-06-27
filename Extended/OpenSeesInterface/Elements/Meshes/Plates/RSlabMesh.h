#ifndef RSlabMesh_H
#define RSlabMesh_H
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

#include "RMesh.h"

class RPoint;

class RSlabMesh : public RMesh
{

public:
    RSlabMesh(RDomain *domain, QString name);
    ~RSlabMesh();

    virtual QVector< QVector<RNode *> >getNodeMatrix(void);

    double getDrift(void);

protected:

    int theMeshOption;

    QPointer<RParameter> theThickness;
    QPointer<RPoint> thePoint1;
    QPointer<RPoint> thePoint2;
    QPointer<RPoint> thePoint3;
    QPointer<RPoint> thePoint4;
    
};

#endif
