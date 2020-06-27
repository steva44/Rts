#ifndef RMathUtils_H
#define RMathUtils_H
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

// This class contains utility functions for computational geometry algorithms
// For full functionality, requires that both OSG and IFC defines be ON

#include "RPoint.h"
#include "RNode.h"
#ifdef OSG
#include <osg/Vec3>
#include <osg/Array>
#include <osg/Matrix>
#endif
#ifdef IFC
#include "IfcPlusPlusSystem.h"
#include <Carve/IncludeCarveHeaders.h>
#endif
#include <QList>
#include <math.h>
#include <assert.h>
#include <stack>
#include <functional>
#include <stdlib.h>

class RMathUtils
{

public:
    RMathUtils();
    ~RMathUtils();

    // Computes a vector that is normal to a polygon, provided as a vector of points or nodes
    // Template class so that it works with both points and nodes
    template<class T>
    std::vector<double> computePolygonNormal( const std::vector<T>& thePointsList )
    {
        const int num_points = thePointsList.size();
        std::vector<double> polygon_normal(3);

        for (int i=0,j = 1; i<num_points; i++, j++)
        {
            if (j == num_points) j = 0;
            const T& vertex_current = thePointsList[i];
            const T& vertex_next = thePointsList[j];

            polygon_normal[0]+= (((vertex_current->getZCoordinateValue()) + (vertex_next->getZCoordinateValue())) * ((vertex_next->getYCoordinateValue()) - (vertex_current->getYCoordinateValue())));
            polygon_normal[1]+= (((vertex_current->getXCoordinateValue()) + (vertex_next->getXCoordinateValue())) * ((vertex_next->getZCoordinateValue()) - (vertex_current->getZCoordinateValue())));
            polygon_normal[2]+= (((vertex_current->getYCoordinateValue()) + (vertex_next->getYCoordinateValue())) * ((vertex_next->getXCoordinateValue()) - (vertex_current->getXCoordinateValue())));
        }

        normalizeVector(&polygon_normal);

        for(auto&& it : polygon_normal)
        {
            it *= -1.0;
        }

        return polygon_normal;
    }

    // Computes the squared distance between two points or nodes, provided as two points or nodes
    // Template class so that it works with both points and nodes
    template<class T>
    double sqrdDistanceBtwnPointsOrNodes(const T* pt1, const T* pt2)
    {
        std::vector<double> point1{pt1->getXCoordinateValue(),pt1->getYCoordinateValue(),pt1->getZCoordinateValue()};
        std::vector<double> point2{pt2->getXCoordinateValue(),pt2->getYCoordinateValue(),pt2->getZCoordinateValue()};

        return sqrdDistanceBtwnPoints(point1,point2);
    }

    // Computes the squared distance between two points or nodes, where one is given as a vector of coordinates, one as a point or node class
    // Template class so that it works with both points and nodes
    template<class T>
    double sqrdDistanceBtwnPointsOrNodes(const std::vector<double>* pt1, T* pt2)
    {
        std::vector<double> point2{pt2->getXCoordinateValue(),pt2->getYCoordinateValue(),pt2->getZCoordinateValue()};

        return sqrdDistanceBtwnPoints(*pt1,point2);
    }

    // Computes the squared distance between two points or nodes, where both points are given in a three dimensional vector of coordinates
    double sqrdDistanceBtwnPoints(const std::vector<double>& pt1, const std::vector<double>& pt2);

    // Computes the the centroid of an arbitrary polygon, provided as a vector of points or nodes
    // Template class so that it works with both points and nodes
    template<class T>
    static std::vector<double> computePolygonCentroid( const std::vector<T>& polygon )
    {
        std::vector<double> polygonCentroid{0.0,0.0,0.0};

        for(auto&& it : polygon)
        {
            polygonCentroid[0] += it[0];
            polygonCentroid[1] += it[1];
            polygonCentroid[2] += it[2];
        }

        double size = static_cast<double>(polygon.size());

        polygonCentroid[0] /= size;
        polygonCentroid[1] /= size;
        polygonCentroid[2] /= size;

        return polygonCentroid;
    }

    // Computes the distance between two points or nodes, provided as two points or nodes
    // Template class so that it works with both points and nodes
    template<class T>
    double distanceBtwnPoints(const T& pt1, const T& pt2)
    {
        double distSq =  (pt2[0]-pt1[0])*(pt2[0]-pt1[0])+(pt2[1]-pt1[1])*(pt2[1]-pt1[1])+(pt2[2]-pt1[2])*(pt2[2]-pt1[2]);

        return sqrt(distSq);
    }

    // Computes the closest object of a vector of points or nodes, to a given point or node
    // Template class so that it works with both points and nodes
    template<class T>
    T* getClosestPointOrNode(const T* object, std::vector<T*>& vectorOfObjs)
    {
        T* closestObject = nullptr;
        if(vectorOfObjs.empty() || !object)
            return closestObject;

        auto distance = sqrdDistanceBtwnPointsOrNodes<T>(object,vectorOfObjs.front());

        for(auto&& it: vectorOfObjs)
        {
            auto newDistance = sqrdDistanceBtwnPointsOrNodes<T>(object,it);

            if(newDistance<=distance)
            {
                distance = newDistance;
                closestObject = it;
            }
        }

        return closestObject;
    }

    // Computes the closest object of a vector of points or nodes, to a given point or node
    // Template class so that it works with both points and nodes
    template<class T>
    T* getClosestPointOrNode(const std::vector<double>& object, std::vector<T*>& vectorOfObjs)
    {
        T* closestObject = nullptr;
        if(vectorOfObjs.empty() || object.empty())
            return closestObject;

        auto distance = sqrdDistanceBtwnPointsOrNodes<T>(&object,vectorOfObjs.front());

        for(auto&& it: vectorOfObjs)
        {
            auto newDistance = sqrdDistanceBtwnPointsOrNodes<T>(&object,it);

            if(newDistance<=distance)
            {
                distance = newDistance;
                closestObject = it;
            }
        }

        return closestObject;
    }

    // Computes the distance between two lines in three dimensions, each line is provdided as a vector of two points or nodes
    // Template class so that it works with both points and nodes
    template<class T>
    double distance3DLineToLine(const std::vector<std::vector<T*>> L1,const std::vector<std::vector<T*>> L2)
    {
        std::vector<double> point1L1{L1.at(0)->getXCoordinateValue(),L1.at(0)->getYCoordinateValue(),L1.at(0)->getZCoordinateValue()};
        std::vector<double> point2L1{L1.at(1)->getXCoordinateValue(),L1.at(1)->getYCoordinateValue(),L1.at(1)->getZCoordinateValue()};

        std::vector<double> point1L2{L2.at(0)->getXCoordinateValue(),L2.at(0)->getYCoordinateValue(),L2.at(0)->getZCoordinateValue()};
        std::vector<double> point2L2{L2.at(1)->getXCoordinateValue(),L2.at(1)->getYCoordinateValue(),L2.at(1)->getZCoordinateValue()};

        std::vector<std::vector<double>> line1 {point1L1,point2L1};
        std::vector<std::vector<double>> line2 {point1L2,point2L2};

        return this->distance3DLineToLine(line1,line2);
    }

    // Computes the distance between two lines in three dimensions, each line is provdided as two end points or nodes
    // Template class so that it works with both points and nodes
    template<class T>
    double distance3DLineToLine(T* L1P0,T* L1P1,T* L2P0,T* L2P1)
    {
        std::vector<double> vecL1P0 = {L1P0->getXCoordinateValue(),L1P0->getYCoordinateValue(),L1P0->getZCoordinateValue()};
        std::vector<double> vecL1P1 = {L1P1->getXCoordinateValue(),L1P1->getYCoordinateValue(),L1P1->getZCoordinateValue()};

        std::vector<double> vecL2P0 = {L2P0->getXCoordinateValue(),L2P0->getYCoordinateValue(),L2P0->getZCoordinateValue()};
        std::vector<double> vecL2P1 = {L2P1->getXCoordinateValue(),L2P1->getYCoordinateValue(),L2P1->getZCoordinateValue()};

        std::vector<std::vector<double>> line1 {vecL1P0,vecL1P1};
        std::vector<std::vector<double>> line2 {vecL2P0,vecL2P1};

        return this->distance3DLineToLine(line1,line2);
    }

    // Computes the midpoint of a line
    // Template class so that it works with both points and nodes
    template<class T>
    std::vector<double> getMidpoint(T* point1, T* point2)
    {
        std::vector<double> p1{point1->getXCoordinateValue(),point1->getYCoordinateValue(),point1->getZCoordinateValue()};
        std::vector<double> p2{point2->getXCoordinateValue(),point2->getYCoordinateValue(),point2->getZCoordinateValue()};

        return this->getMidpoint(p1,p2);
    }
    std::vector<double> getMidpoint(std::vector<double>& point1, std::vector<double>& point2);

    // Computes the norm of a vector
    static double vectorNorm(const std::vector<double> *vector);

    // Computes the deteminant of a 3x3 matrix
    double determinant3x3(const std::vector<double> *vector1,const std::vector<double> *vector2,const std::vector<double> *vector3);

    // Computes the dot-product of a vector
    double dotProduct(std::vector<double> const *vector1, std::vector<double> const *vector2);

    // Normalizes a vector
    static double normalizeVector(std::vector<double> *vector);

    // Returns a scaled vector
    std::vector<double> scaleVector(double factor, std::vector<double> const *v);

    // Scales a vector
    int scaleVectorInPLace(double factor, std::vector<double> const *v);

    // Computes the dot-product of a vector
    std::vector<double> crossProduct(std::vector<double> const *vector1, std::vector<double> const *vector2);

    // Computes the absolute value of the distance from a point to a plane
    // Plane should be given as 3 points that lie on the plane
    double distanceFromPointToPlane(std::vector<RPoint*>& Pln, std::vector<double>& Pnt);

    // Compute the intersection line of two planes
    // Line and Ray and Segment with defining  points {Point P0, P1;}
    // Plane with a point and a normal {Point V0; Vector  n;}
    // Input:  two planes Pn1 and Pn2, the first vector is any point on the plane and the second vector is the normal vector
    // Output: *L = the intersection line (when it exists)
    // Return:
    //      0 = disjoint (no intersection)
    //      1 = the two  planes coincide
    //      2 =  intersection in the unique line *L
    //      -1 = there was an error
    int intersect3DTwoPlanes(const std::vector<std::vector<double>>& Pn1,const std::vector<std::vector<double>>& Pn2, std::vector<std::vector<double>>* L );

    // Compute the point where two lines intersect
    // P11 and P12 are the start an end points of segment line 1
    // P21 and P22 are the start an end points of segment line 2
    // IPoint is the intersection point, if it exists
    // Return:
    //      1 = the two lines are parallel
    //      2 = the two lines are colinear
    //      3 = there is an intersection within the linesegments
    //      4 = there is an intersection outside of the linesegments
    //      5 = there is no intersection
    //      -1 = there was an error
    int intersect3DTwoLines(const std::vector<double>& P11,const std::vector<double>& P12,
                            const std::vector<double>& P21,const std::vector<double>& P22,
                            std::vector<double>* IPoint );


    // Computes the shortest distance between two lines in 3D
    double distance3DLineToLine(const std::vector<std::vector<double>>& L1,const std::vector<std::vector<double>>& L2);

    // Subtracts vector b from a and returns a vector
    std::vector<double> subtractVectors(std::vector<double> const *a, std::vector<double> const *b);

    // Adds vector b to a and returns a vector
    std::vector<double>  addVectors(std::vector<double> const *a, std::vector<double> const *b);

#ifdef OSG
    // Computes the convex hull of a set of N points using the Graham sweep algorithm in 3D
    void convexHull(QList<osg::Vec3>& thePointsList);

    // Find orientation of ordered triplet (p, q, r).
    //    Returns following values:
    //    0 --> p, q and r are colinear
    //    1 --> Clockwise
    //    2 --> Counterclockwise
    int orientation(osg::Vec2 p, osg::Vec2 q, osg::Vec2 r);

    // Computes the convex hull of a set of N points using the Jarvis algorithm in 3D
    void convexHullJarvis(QList<osg::Vec3>& thePointsList);
    void convexHullJarvis(std::vector<vec3>& thePointsVec);

    // Compute the normal of an arbitray polygon using Newell's Method, in 3D - if the polygon is degenerate, then will not work
    osg::Vec3 computePolygonNormal(const QList<osg::Vec3>& thePointsList);
    std::vector<double> computePolygonNormal( const std::vector<vec3>& polyVertices );
    std::vector<double> computePolygonNormal(const std::vector<std::vector<double>>& thePointsList);

    // Compute the 2D projection of points in 3D space on a given plane
    std::vector<osg::Vec2> get2DProjection(QList<osg::Vec3> thePointsList);

    // Compute the centroid of an arbitrary polygon in 3D
    osg::Vec3 computePolygonCentroid(const std::vector<osg::Vec3>& polygon);
    osg::Vec3 computePolygonCentroid(const QList<osg::Vec3>& polygonList);

    //Computes the normal vector of a triangle of three points
    osg::Vec3 computeTriangleNormal(const std::vector<osg::Vec3> thePointsList);

    // Returns true if the polygon is convex
    bool checkOpenPolygonConvexity(const std::vector<osg::Vec2>& polygon);

    // Returns the transformation matrix to transform a square to a quad, and vice versa
    osg::Matrix  getSquareToQuadTransform(const QVector<double>& theXCoordVector, const QVector<double>& theYCoordVector);
    osg::Matrix  getQuadToSquareTransform(const QVector<double>& theXCoordVector, const QVector<double>& theYCoordVector);
    osg::Matrix  getQuadToQuadTransform(const QVector<double>& theXCoordVector1, const QVector<double>& theYCoordVector1,
                                        const QVector<double>& theXCoordVector2, const QVector<double>& theYCoordVector2);

#ifdef IFC
    // Compute the area of a polygon in 3D, given a vector of vertices
    double polygonArea(const std::vector<vec3>& vertices);

    // Computes the convex hull of a set of N points using the Jarvis algorithm in 2D
    void convexHullJarvis(std::vector<vec2>& thePointsVec);

    // Compute the centroid of an arbitrary polygon in 2D
    vec2 computePolygonCentroid2D(const std::vector<vec2>& polygon);

    // Compute the 2D projection of points in 3D space on a given plane
    std::vector<vec2> get2DProjection(const std::vector<vec3>& thePointsVec);

    // Compute the normal vector to a triangle of three points
    vec3 computeTriangleNormal(const std::vector<vec3>& thePointsList);

    // Compute the centroid of an arbitrary polygon in 3D
    vec3 computePolygonCentroid(const std::vector<vec3>& polygon);

    // Returns true if three points are colinear
    bool pointsColinear(const vec3& a, const vec3& b, const vec3& c, const double & dotTol);

    // Midpoint of a line in 3D and 2D
    vec3 getMidpointCarve3D(vec3 point1, vec3 point2);
    vec2 getMidpointCarve2D(vec2 point1, vec2 point2);
#endif
#endif

    // Compute the length of a line with two points
    double computeLengthWithPoints(RPoint *point1, RPoint *point2);
    double computeLengthWithNodes(RNode *node1, RNode *node2);

    // Returns a quaternion that will rotate from vector a to b
    /*This routine uses only fast geometric transforms, without costly acos/sin computations.
    It's exact, fast, and with less degenerate cases than the acos/sin method.

    For an explanation of the math used, you may see for example:
    http://logiciels.cnes.fr/MARMOTTES/marmottes-mathematique.pdf

    This is the rotation with shortest angle, which is the one equivalent to the
    acos/sin transform method. Other rotations exists, for example to additionally keep
    a local horizontal attitude.

    author Nicolas Brodu - modified to work with STL data members by Stevan Gavrilovic
    */
    int makeRotate(const std::vector<double>& a, const std::vector<double>& b, std::vector<double>* Quat);

    // Returns a quaternion that will rotate a vector by a given angle
    // Set the elements of the Quat to represent a rotation of angle
    // (radians) around the axis (x,y,z)
    // Positive rotation is counter-clockwise
    int makeRotate(const double angle, std::vector<double>& vec, std::vector<double>* Quat);
    int makeRotate(const double angle, const double x,const double y,const double z, std::vector<double>* Quat);

    // Returns the rotation matrix part of a quaternion
    // Adapted from OSG code
    int getRotatationMatrixFromQuat(const std::vector<double>& Quat, std::vector<std::vector<double>>* Mat);
    int getRotatationMatrixFromQuat(const std::vector<double>& Quat, QVector< QVector<double> >* Mat);

    // Pre-multiplies a 4x4 rotation matrix with a 3 dim vector
    std::vector<double> preMult(const std::vector<double>& v, std::vector<std::vector<double>>* Mat) const;

    // Post-multiplies a 4x4 rotation matrix with a 3 dim vector
    std::vector<double> postMult(const std::vector<double>& v, std::vector<std::vector<double>>* Mat) const;

};

#endif








