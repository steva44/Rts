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
#include "RElement.h"

RMesh::RMesh(RDomain *theDomain, QString name)
    : RMethod(theDomain, name), theElementList(nullptr)
{
    theComponentName = name;
    maxDrift = 0.0;
}


RMesh::~RMesh()
{

}


int RMesh::getNumberOfElements()
{
    qCritical() << "A subclass mesh is not yet prepared to provide its number of elements";

    return 0;
}


void RMesh::reset(void)
{
    maxDrift = 0.0;
}


int RMesh::commitState(void)
{
    return 0;
}


double RMesh::getDrift()
{
    qCritical() << "A subclass mesh is not yet prepared to provide its number of elements";

    return 0.0;
}


double RMesh::getMaxDrift() const
{
    return maxDrift;
}


#ifdef OSG
shared_ptr<RTopoMesh> RMesh::getTopologicalMesh()
{
    return theTopoMesh;
}


QList<RElement *> *RMesh::getTheElementList() const
{
    return theElementList;
}


void RMesh::setTheElementList(QList<RElement *> *value)
{
    theElementList = value;
}


int RMesh::createTopologicalMesh(void)
{
    qCritical() << "Topolgical meshing is not yet implemented for this mesh";
    
    return -1;
}
#endif
