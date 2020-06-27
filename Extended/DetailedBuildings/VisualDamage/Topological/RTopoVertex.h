#ifndef RTOPOVERTEX_H
#define RTOPOVERTEX_H
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

// This class represents a vertex object in a topological mesh, with x,y, and z coordinates

#include<algorithm>
#include <functional>

#include "osg/Vec3d"

class RNode;
class RSection;

class RTopoVertex
{
public:
    RTopoVertex(RNode* node, const double dx, const double dy, const double dz);
    RTopoVertex(RNode* node1, RNode* node2, const double XI, const double dx, const double dy, const double dz);
    RTopoVertex();
    ~RTopoVertex();

    // Copy-constructor
    RTopoVertex(const RTopoVertex& other)
    {
        this->theNode1 = other.theNode1;
        this->theNode2 = other.theNode2;
        this->xi = other.xi;
        this->OSGPoint = other.OSGPoint;
        this->deltax   = other.deltax;
        this->deltay   = other.deltay;
        this->deltaz   = other.deltaz;   
    }

    // Assignment operator
    RTopoVertex& operator=(RTopoVertex other)
    {
        this->swap(*this, other);
        return *this;
    }

    // Swap operator
    void swap(RTopoVertex& first, RTopoVertex& second)
    {
        // By swapping the members of two objects, the two objects are effectively swapped
        std::swap(first.theNode1, second.theNode1);
        std::swap(first.theNode2, second.theNode2);
        std::swap(first.xi, second.xi);
        std::swap(first.OSGPoint, second.OSGPoint);
        std::swap(first.deltax, second.deltax);
        std::swap(first.deltay, second.deltay);
        std::swap(first.deltaz, second.deltaz);
    }

    // Comparison operator
    inline bool operator==(const RTopoVertex& vertex)
     {
         if(vertex.OSGPoint == OSGPoint)
             return true;
         else
             return false;
     }

    // Overload the [] operator to return the i'th coordinate of the OSG point
    inline double operator[](unsigned i) {
      return OSGPoint[i];
    }

    // Overload the - operator to subtract another point from the current point
    inline const osg::Vec3d operator - (const RTopoVertex& rhs) const {
        return osg::Vec3d(OSGPoint.x()-rhs.x(), OSGPoint.y()-rhs.y(), OSGPoint.z()-rhs.z());
    }

    // X coordinate of the vertex
    inline double x(void) const {
      return OSGPoint.x();
    }

    // Y coordinate of the vertex
    inline double y(void) const {
      return OSGPoint.y();
    }

    // Z coordinate of the vertex
    inline double z(void) const {
      return OSGPoint.z();
    }

    // Displacements of the vertex
    double getXDisplacement();
    double getYDisplacement();
    double getZDisplacement();

    // Rotations of the vertex
    double getXRotation();
    double getYRotation();
    double getZRotation();

    // OSG point corresponding to this vertex
    inline osg::Vec3d getOSGPoint(void){return OSGPoint;}

    // RNode corresponding to this vertex
    inline RNode* getNode(void){return theNode1;}

    void commitState(bool deformed);

private:

    // The vertex may coincide with a node, or be some distance xi between two nodes
    RNode* theNode1;
    RNode* theNode2;
    double xi;

    // An OSG point corresponding to this vertex
    osg::Vec3d OSGPoint;
    double deltax;
    double deltay;
    double deltaz;

    // Displacements and rotations of the vertex
    double dispx;
    double dispy;
    double dispz;
    double rotx;
    double roty;
    double rotz;

};

#endif
