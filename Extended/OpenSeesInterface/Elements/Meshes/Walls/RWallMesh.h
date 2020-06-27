#ifndef RWallMesh_H
#define RWallMesh_H
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
#include "RPoint.h"

class RWallMesh : public RMesh
{
    Q_OBJECT

public:
    RWallMesh(RDomain *theDomain, QString name);
    ~RWallMesh();

    // Returns the drift ratio of the wall
    double getDrift();

    // This function creates a hexahedron out of 6 NURB faces that will be used to create the visualization topology of the component
    int createTopologicalMesh(void);

protected:

   QString theElementType;

   QPointer<RPoint> thePoint1;
   QPointer<RPoint> thePoint2;
   QPointer<RPoint> thePoint3;
   QPointer<RPoint> thePoint4;

   QPointer<RParameter> theThickness;

   QPointer<RParameter> theNumElementsAlongU;
   QPointer<RParameter> theNumElementsAlongV;

private:
    
};

#endif
