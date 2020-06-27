#ifndef RTOPOMESH_H
#define RTOPOMESH_H
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

// This class holds the topological, i.e., visualization mesh of a component
// It stores the vertices, edges, and face of the topological mesh

#include "RTopoEdge.h"
#include "RTopoFace.h"
#include "RTopoVertex.h"

#include <vector>
#include <osg/ref_ptr>
#include <osg/Array>

class RMesh;

class RTopoMesh
{

public:
    RTopoMesh(RMesh* mesh);
    ~RTopoMesh();

    // Texture to display on the face
    enum Texture{CONCRETE,STEEL,WOOD};

    // Add a face to the mesh
    void addFace(std::shared_ptr<RTopoFace>& face);
    void addFace(RTopoFace& face);

    // Add a vertex to the mesh
    std::shared_ptr<RTopoVertex> addVertex(RNode* node, const double dx, const double dy, const double dz);
    std::shared_ptr<RTopoVertex> addVertex(RNode* node1, RNode* node2, const double xi, const double dx, const double dy, const double dz);

    // Add an edge to the mesh
    std::shared_ptr<RTopoEdge> addEdge(std::shared_ptr<RTopoVertex>& vert1, std::shared_ptr<RTopoVertex>& vert2);

    // Getters for the number of faces, edges, and vertices
    inline size_t getNumberOfEdges(void){return theEdgeVector.size();}
    inline size_t getNumberOfVertices(void){return theVertexVector.size();}
    inline size_t getNumberOfFaces(void){return theFaceVector.size();}
    inline size_t getNumberOfEdges(void) const {return theEdgeVector.size();}
    inline size_t getNumberOfVertices(void) const {return theVertexVector.size();}
    inline size_t getNumberOfFaces(void) const {return theFaceVector.size();}

    // Vector of all the faces in the mesh
    const std::vector<std::shared_ptr<RTopoFace>> getFaceVector(void) const {return theFaceVector;}

    // Commit the state of the vertices, i.e., the coordinates, at a specific time
    void commitState(double time, osg::ref_ptr<osg::Vec3Array>& vertArray);

    // Get/set the vertex buffer array for updating the vertex coordinates in a time history
    void setVertexBufferArray(osg::ref_ptr<osg::Vec3Array>& vertexBufferArray){theVertexBufferArray = vertexBufferArray;}
    void setVertexBufferArrayValues(const int index,const double x,const double y,const double z);
    osg::Vec3Array* getTheVertexBufferArray() const;

    // Vector of time points at which the state of the mesh is stored
    std::vector<double> getTimeVector() const;

    // Array of all of the vertices of the mesh
    std::vector<osg::ref_ptr<osg::Vec3Array> > getVerticesArray() const;

    // Return the texture of the mesh
    Texture getTexture() const;

private:

    // The vertex buffer array for updating vertex coordinates
    osg::ref_ptr<osg::Vec3Array> theVertexBufferArray;

    // The mesh
    RMesh* theMesh;

    // Vectors for the edges, faces, and vertices
    std::vector<std::shared_ptr<RTopoEdge>> theEdgeVector;
    std::vector<std::shared_ptr<RTopoFace>> theFaceVector;
    std::vector<std::shared_ptr<RTopoVertex>> theVertexVector;

    // Vectors to store the state of the mesh over time
    std::vector<double> timeVector;
    std::vector<osg::ref_ptr<osg::Vec3Array>> verticesArray;
    std::vector<Texture> textureArray;

    // The texture of the mesh
    Texture theTopoTexture;
};

#endif // RTOPOMESH_H
