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

#include "RBeamColumnMesh.h"
#include "RPoint.h"

RBeamColumnMesh::RBeamColumnMesh(RDomain *domain, QString componentName,RPoint* point1, RPoint* point2)
    : RMesh(domain, componentName), thePoint1(point1), thePoint2(point2)
{
}


RBeamColumnMesh::~RBeamColumnMesh()
{
}


double RBeamColumnMesh::getDrift()
{
    auto node1 = thePoint1->getAssociatedNode();
    auto node2 = thePoint2->getAssociatedNode();

    const Vector &disp1 = node1->getTrialDisp();
    const Vector &disp2 = node2->getTrialDisp();

    auto dx = disp2(0)-disp1(0);
    auto dy = disp2(1)-disp1(1);

    auto dz = node2->getZCoordinateValue() - node1->getZCoordinateValue();

    //Get the drift value
    auto delta  = sqrt(dx*dx+dy*dy);

    auto drift = delta/dz;

    if(drift > maxDrift)
        maxDrift = drift;

    return drift;
}


double RBeamColumnMesh::getAxialLoad(int elementNumber)
{
    qCritical()<<"Method getAxialLoad not implemented in this mesh";
    return 0.0;
}


double RBeamColumnMesh::getLength()
{
    auto xcoord1 = thePoint1->getXCoordinateValue();
    auto xcoord2 = thePoint2->getXCoordinateValue();
    auto ycoord1 = thePoint1->getYCoordinateValue();
    auto ycoord2 = thePoint2->getYCoordinateValue();
    auto zcoord1 = thePoint1->getZCoordinateValue();
    auto zcoord2 = thePoint2->getZCoordinateValue();

    auto dx = xcoord2-xcoord1;
    auto dy = ycoord2-ycoord1;
    auto dz = zcoord2-zcoord1;

    return sqrt(dx*dx + dy*dy + dz*dz);
}


