#ifndef RTOPOUTILS_H
#define RTOPOUTILS_H
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

// This class provides common functions required for creating the topological mesh

#include "RTopoVertex.h"

#include <vector>
#include <osg/Vec3d>
#include <osg/Vec2d>

class RTopoUtils
{
    template <class T>
    using vector = std::vector<T>;
    using vec3d = osg::Vec3d;
    using Point = osg::Vec2d;

public:
    RTopoUtils();

    // Gets the convex hull of a set of N points --> Jarvis sweep algo
    // This one starts from the left-most point
    void convexHullJarvis(vector<RTopoVertex*>& thePointsList);

    // Uses Newell's Method to compute the normal of an arbitrary polygon - if the polygon is degenerate, then will not work.
    vec3d computePolygonNormal( const vector<vec3d>& thePointsList );

    // Gets the 2-D projection of points in 3-D space on a given plane -- should only be used for traingles or quads
    std::vector<Point> get2DProjection(const vector<RTopoVertex*> thePointsList);

    // Compute the centroid of an arbitrary polygon - if the polygon is degenerate, or not convex, then will not work.
    vec3d computePolygonCentroid( const vector<vec3d>& polygon );

    // Computes the normal of a triangle
    vec3d computeTriangleNormal( const vector<vec3d> thePointsList);

    // Returns the index of the left-most point in a vector
    int getLeftMostPnt(const vector<Point>& points);

    // Returns the index of the bottom-left -most point in a vector
    int getBottomLeftMostPnt(const vector<Point>& points);

    // Checks to see if the point P is to the left of the line AB made by the points A & B
    // Returns true if the points is to the left (cross-product is positive) or on the line (cross-product is zero)
    inline bool pntToTheLeft (const Point& AB,const Point& AP){

        double val = AB.x()*AP.y() - AB.y()*AP.x();

        return val<DBL_EPSILON ? false : true;
    }

    // Computes the normal of a triangle
    inline vec3d computeCrossProduct( const vec3d& U,const vec3d& V)
    {
        vec3d product( vec3d( 0, 0, 0 ));

        product.x() = ( U.y() * V.z()) - ( U.z() * V.y());
        product.y() = ( U.z() * V.x()) - ( U.x() * V.z());
        product.z() = ( U.x() * V.y()) - ( U.y() * V.x());

        return product;
    }

    // Compute the normal of a quadrilateral using Newell's Method - if the quad is degenerate, then will not work. Note that the sign of the vector will depend on the winding order of the points
    vec3d  computeQuadNormal( const std::vector<vec3d>& vertices );

    // Gets the orientation of a 2D point triplet
    // Returns 0 if colinear, 1 if clockwise, 2 if counterclockwise
    int orientation(const Point p,const Point q,const Point r);

};

#endif // RTOPOUTILS_H
