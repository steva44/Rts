#ifndef RTOPOFACE_H
#define RTOPOFACE_H
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

// This class represents a face object in a topological mesh
// The face objects make up the surfaces of a component and they display the visual damge
// Each face is bounded by a loop of edges
// In this case all faces are quadrilaterals with four edges

#include "RTopoEdge.h"
#include "RTopoVertex.h"

class RDamageRegion;

class RTopoFace
{
public:
    RTopoFace(std::shared_ptr<RTopoEdge>& edge1, std::shared_ptr<RTopoEdge>& edge2,
              std::shared_ptr<RTopoEdge>& edge3, std::shared_ptr<RTopoEdge>& edge4);
    ~RTopoFace();

    // Vector of vertices from the edges, each entry in the vector is unique, i.e., there are no coinciding vertices from adjoining edges, the two adjoining edges share the same vertex
    std::vector<RTopoVertex*>   getVerticesVector(void) const;
    std::vector<osg::Vec3d>     getOSGVerticesVector(void) const;

    // Adds an edge to the face
    void addEdge(std::shared_ptr<RTopoEdge>& edge);

    // Damage region for this face, provides the visual damage to be displayed
    RDamageRegion *getTheDamageRegion() const;
    void setTheDamageRegion(RDamageRegion *value);

    // Face index number
    size_t getIndex() const;
    void setIndex(size_t value);

    // Type of damage that is displayed on the face
    int getDamageType(void);

protected:

    // Vector of edges
    std::vector<std::shared_ptr<RTopoEdge>> theEdgeVector;

    // Vector that is normal to the face
    std::vector<double> normalVector;

    // Face index number
    size_t index;

    // Damage region for this face, provides the visual damage to be displayed
    RDamageRegion* theDamageRegion;
};

#endif // RTOPOFACE_H
