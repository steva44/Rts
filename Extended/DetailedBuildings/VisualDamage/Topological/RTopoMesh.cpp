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

#include "RTopoMesh.h"
#include "RMesh.h"

RTopoMesh::RTopoMesh(RMesh* mesh) : theMesh(mesh)
{
    theTopoTexture = Texture::CONCRETE;
}


RTopoMesh::~RTopoMesh()
{

}


void RTopoMesh::addFace(std::shared_ptr<RTopoFace>& face)
{
    theFaceVector.push_back(face);
}


void RTopoMesh::addFace(RTopoFace& face)
{
    theFaceVector.push_back(std::make_shared<RTopoFace>(face));
}


std::shared_ptr<RTopoVertex> RTopoMesh::addVertex(RNode* node, const double dx, const double dy, const double dz)
{
    RTopoVertex vertex(node,dx,dy,dz);

    for(auto& it :theVertexVector)
    {
        if(vertex == *it)
            return it;
    }

    theVertexVector.push_back(std::make_shared<RTopoVertex>(vertex));

    return theVertexVector.back();
}


std::shared_ptr<RTopoVertex> RTopoMesh::addVertex(RNode* node1, RNode* node2, const double xi, const double dx, const double dy, const double dz)
{
    RTopoVertex vertex(node1,node2,xi,dx,dy,dz);

    for(auto& it :theVertexVector)
    {
        if(vertex == *it)
            return it;
    }

    theVertexVector.push_back(std::make_shared<RTopoVertex>(vertex));

    return theVertexVector.back();
}


std::shared_ptr<RTopoEdge> RTopoMesh::addEdge(std::shared_ptr<RTopoVertex>& vert1, std::shared_ptr<RTopoVertex>& vert2)
{
    RTopoEdge edge(vert1,vert2);

    for(auto& it: theEdgeVector)
    {
        if(edge == *it)
            return it;
    }

    theEdgeVector.push_back(std::make_shared<RTopoEdge>(edge));

    return theEdgeVector.back();
}


void RTopoMesh::commitState(double time, osg::ref_ptr<osg::Vec3Array>& vertArray)
{
    verticesArray.push_back(vertArray);
    timeVector.push_back(time);

}


std::vector<double> RTopoMesh::getTimeVector() const
{
    return timeVector;
}


std::vector<osg::ref_ptr<osg::Vec3Array> > RTopoMesh::getVerticesArray() const
{
    return verticesArray;
}


osg::Vec3Array* RTopoMesh::getTheVertexBufferArray() const
{
    return theVertexBufferArray.get();
}


RTopoMesh::Texture RTopoMesh::getTexture() const
{
    return theTopoTexture;
}


void RTopoMesh::setVertexBufferArrayValues(const int index,const double x,const double y,const double z)
{
    theVertexBufferArray->at(index).set(x,y,z);
}

