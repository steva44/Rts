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

#include "RTopoFace.h"
#include "RDamageRegion.h"

#include <algorithm>

RTopoFace::RTopoFace(std::shared_ptr<RTopoEdge>& edge1, std::shared_ptr<RTopoEdge>& edge2,
                     std::shared_ptr<RTopoEdge>& edge3, std::shared_ptr<RTopoEdge>& edge4)
{
    index = 0;

    theDamageRegion = nullptr;

    theEdgeVector.push_back(edge1);
    theEdgeVector.push_back(edge2);
    theEdgeVector.push_back(edge3);
    theEdgeVector.push_back(edge4);
}


RTopoFace::~RTopoFace()
{

}


std::vector<RTopoVertex*> RTopoFace::getVerticesVector(void) const
{

    std::vector<RTopoVertex*> verticesVector;

    for(auto&& it: theEdgeVector)
    {
        verticesVector.push_back(it->getVertex1().get());
    }

    return verticesVector;
}


std::vector<osg::Vec3d> RTopoFace::getOSGVerticesVector(void) const
{
    std::vector<osg::Vec3d> verticesVector;

    for(auto&& it: theEdgeVector)
    {
        verticesVector.push_back(it->getVertex1().get()->getOSGPoint());
        verticesVector.push_back(it->getVertex2().get()->getOSGPoint());
    }

    std::sort( verticesVector.begin(), verticesVector.end() );
    verticesVector.erase( std::unique( verticesVector.begin(), verticesVector.end() ), verticesVector.end() );

    return verticesVector;
}


void RTopoFace::addEdge(std::shared_ptr<RTopoEdge>& edge)
{
    if(edge)
        theEdgeVector.push_back(edge);
}


RDamageRegion *RTopoFace::getTheDamageRegion() const
{
    return theDamageRegion;
}


void RTopoFace::setTheDamageRegion(RDamageRegion *value)
{
    theDamageRegion = value;
}


size_t RTopoFace::getIndex() const
{
    return index;
}


void RTopoFace::setIndex(size_t value)
{
    index = value;
}


int RTopoFace::getDamageType(void)
{
    if(theDamageRegion == nullptr)
        return -1;

    return theDamageRegion->getDamageType(index);
}
