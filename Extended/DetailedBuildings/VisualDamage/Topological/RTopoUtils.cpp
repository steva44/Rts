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

#include "RTopoUtils.h"
#include <functional>
#include <iostream>

RTopoUtils::RTopoUtils()
{

}

void RTopoUtils::convexHullJarvis(vector<RTopoVertex*>& thePointsList)
{
    //first convert the 3-D points into 2-D projection..
    int n = thePointsList.size();

    vector<Point>points = get2DProjection(thePointsList);

    // Initialize Result
    vector<Point> hull;

    //Must start from the left-most point
    int minpoint = getLeftMostPnt(points);


    std::function<int(int)> factorial = [&](int n)
    {
        return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
    };

    auto maxIterations = factorial(n);

    // Start from bottom-leftmost point, keep moving counterclockwise
    // until reach the start point again.  This loop runs O(h)
    // times where h is number of points in result or output.
    int p = minpoint, q, iterationNum = 0;
    do
    {
        // Add current point to result
        hull.push_back(points[p]);

        // Search for a point 'q' such that orientation(p, x,
        // q) is counterclockwise for all points 'x'. The idea
        // is to keep track of last visited most counterclock-
        // wise point in q. If any point 'i' is more counterclock-
        // wise than q, then update q.
        q = (p+1)%n;
        for (int i = 0; i < n; ++i)
        {
            // If i is more counterclockwise than current q, then
            // update q
            auto orient = orientation(points[p], points[i], points[q]);

            if (orient == 2)
                q = i;
        }

        // Now q is the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to
        // result 'hull'
        p = q;

        ++iterationNum;

    } while (p != minpoint && iterationNum < maxIterations);  // While we don't come to first point

    if(iterationNum == maxIterations || hull.size() != n)  //Something went wrong
    {
        std::cout<<"Error, convex hull could not be established";
        return;
    }

    vector<RTopoVertex*> theNewPointsList;

    //Now rearrage the 3-D Vector according to its sorted 2-D Vector
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(hull[i] == points[j])
            {
                theNewPointsList.push_back(thePointsList[j]);
            }
        }
    }


    thePointsList.swap(theNewPointsList);
}


int RTopoUtils::getBottomLeftMostPnt(const vector<Point>& points)
{

    //Take a wild guess and assume the first point is the one...
    double xmin = points[0].x();
    double ymin = points[0].y();

    int xminpos = 0, yminpos = 0;

    for (size_t i = 1; i < points.size(); ++i)
    {
        auto x = points[i].x();
        auto y = points[i].y();

        if  (x < xmin || (xmin == x && y < points[yminpos].y()))
            xmin = x, xminpos = i;

        if  (y < ymin || (ymin == y && x < points[xminpos].x()))
            ymin = y, yminpos = i;
    }

    //If this is already the bottom-most points, stop and return
    if(xminpos == yminpos)
    {
        return xminpos;
    }
    //Its not, check to see if there are any points to the left of the line made by the left-most and bottom-most
    else
    {
        vector<Point> pointsToCheck;

        auto pntXmin = points.at(xminpos);
        auto pntYmin = points.at(yminpos);

        auto AB = pntYmin-pntXmin;

        for (int i = 0; i < points.size(); ++i)
        {
            if(i == xminpos || i == yminpos)
                continue;

            auto AP = pntYmin - points.at(i);

            if(pntToTheLeft(AB,AP))
                pointsToCheck.push_back(points.at(i));
        }

        if(pointsToCheck.size() == 0)
        {
            return yminpos;
        }
        //There are some more points to the left of, or on the line, of the bottom-left -most points
        else
        {
            int pos = getBottomLeftMostPnt(pointsToCheck);

            //Now need to get the position of that point in the original list
            for (int i = 0; i < points.size(); ++i)
            {
                if(points.at(i)==pointsToCheck.at(pos))
                    return i;
            }
        }
    }

    //Throw error code
    return -1;
}


int RTopoUtils::getLeftMostPnt(const vector<Point>& points)
{

    //Take a wild guess and assume the first point is the one...
    double xmin = points[0].x();

    int xminpos = 0;

    for (size_t i = 1; i < points.size(); ++i)
    {
        auto x = points[i].x();

        //In case of tie get the bottom-most point
        if  (x < xmin || (xmin == x && points[i].y() < points[xminpos].y()))
            xmin = x, xminpos = i;
    }

    return xminpos;
}


RTopoUtils::vec3d RTopoUtils::computePolygonNormal( const vector<vec3d>& thePointsList )
{
    const int num_points = thePointsList.size();
    vec3d polygon_normal( vec3d( 0, 0, 0 ) );

    for (int i=0,j = 1; i<num_points; i++, j++)
    {
        if (j == num_points) j = 0;
        const vec3d& vertex_current = thePointsList[i];
        const vec3d& vertex_next = thePointsList[j];

        polygon_normal.x()+= (((vertex_current.z()) + (vertex_next.z())) * ((vertex_next.y()) - (vertex_current.y())));
        polygon_normal.y()+= (((vertex_current.x()) + (vertex_next.x())) * ((vertex_next.z()) - (vertex_current.z())));
        polygon_normal.z()+= (((vertex_current.y()) + (vertex_next.y())) * ((vertex_next.x()) - (vertex_current.x())));
    }

    polygon_normal.normalize();

    return polygon_normal;
}


RTopoUtils::vector<RTopoUtils::Point> RTopoUtils::get2DProjection(const vector<RTopoVertex*> thePointsList)
{
    const int numPoints = thePointsList.size();

    vector<vec3d> the3DPointList(numPoints);

    vector<Point> the2DPointsList(numPoints);

    for(int i =0;i<numPoints;i++)
    {
        the3DPointList[i] = thePointsList[i]->getOSGPoint();
    }

    auto getAbs = [](vec3d& v)
    {
        vec3d temp(v);

        v.x()=fabs(temp.x());
        v.y()=fabs(temp.y());
        v.z()=fabs(temp.z());
    };

    //    // Determine a few vectors in the element plane
    //    vec3d vector12 = the3DPointList[1] - the3DPointList[0];
    //    vec3d vector13 = the3DPointList[2] - the3DPointList[0];
    //    vec3d vector14 = the3DPointList[3] - the3DPointList[0];

    //    // Check that the element is plane, i.e., that two normal vectors are parallel
    //    vec3d cross12by13 = computeCrossProduct(vector12,vector13);
    //    vec3d cross12by14 = computeCrossProduct(vector12,vector14);

    //    double dotProductOfNormals = cross12by13*cross12by14;
    //    double normOfCross12by13 =  cross12by13.length();
    //    double normOfCross12by14 =  cross12by14.length();

    //    double shouldBeUnity = dotProductOfNormals / (normOfCross12by13 * normOfCross12by14);

    //    if (fabs(shouldBeUnity-1.0) > 0.00000001) {
    //        std::cout<< "is not plane.";
    //    }

    //    // The first node is by definition set to the origin
    //    Point thePoint0( Point(0,0) );

    //    // The second node is by definition set along the x-axis
    //    double distance12 = vector12.length();

    //    Point thePoint1(distance12,0.0);

    //    // The y-vector is the negative of the cross-product of the x-axis and the normal vector
    //    vec3d yvector = computeCrossProduct(vector12,cross12by13);

    //    yvector *= -1.0;

    //    // Normalize the x and y vectors
    //    vector12.normalize();
    //    yvector.normalize();

    //    // The coordinates of the third and fourth node are obtained by dot products
    //    Point thePoint2(vector13*vector12,vector13*yvector);
    ////    (*x)[2] = theMatrixOperations.dotProduct(&vector13, &unitx);
    ////    (*x)[3] = theMatrixOperations.dotProduct(&vector14, &unitx);

    //    Point thePoint3(vector14*vector12,vector14*yvector);
    ////    (*y)[2] = theMatrixOperations.dotProduct(&vector13, &unity);
    ////    (*y)[3] = theMatrixOperations.dotProduct(&vector14, &unity);

    //    the2DPointsList[0] = thePoint0;
    //    the2DPointsList[1] = thePoint1;
    //    the2DPointsList[2] = thePoint2;
    //    the2DPointsList[3] = thePoint3;



    //vec3d theNormal = computeTriangleNormal(the3DPointList);

    vec3d U = the3DPointList[1] - the3DPointList[0];

    vec3d V = the3DPointList[2] - the3DPointList[0];

    vec3d theNormal1 = computeCrossProduct(U,V);

    vec3d axis1 = computeCrossProduct(theNormal1,U);

    theNormal1.normalize();

    axis1.normalize();

    U.normalize();

    getAbs(U);
    getAbs(theNormal1);
    getAbs(axis1);


    //then Compute two orthogonal direction vectors to form the orthogonal triplet
    //    float length = hypotf( theNormal.x(), hypotf(theNormal.y(), theNormal.z()));
    //    float dir_scalar = (theNormal.x() > 0.0) ? length : -length;
    //    float xt = theNormal.x() + dir_scalar;
    //    float dot = -theNormal.y() / (dir_scalar * xt);

    //    vec3d theAxis1 ( dot * xt, 1.0f + dot * theNormal.y(), dot * theNormal.z());

    //    theAxis1.normalize();

    //    getAbs(theAxis1);

    //Cross product to get the second
    //    vec3d  theAxis2 = computeCrossProduct(theAxis1, theNormal);

    //    theAxis2.normalize();

    //    getAbs(theAxis2);

    //define the origin on the plane... use centroid!
    //vec3d  centroid = computePolygonCentroid(the3DPointList);
    vec3d  centroid (0.0,0.0,0.0);

    for(int i=0;i<numPoints; ++i)
    {
        Point the2DPoint( Point(0,0) );

        float p1=0.0, p2=0.0;

        if(U.x() > axis1.x() && axis1.y() >= U.y())
        {
            p1 = (U*(the3DPointList[i]-centroid));
            p2 = (axis1*(the3DPointList[i]-centroid));
        }
        else if(axis1.x() > U.x() && U.y() >= axis1.y())
        {
            p1 = (axis1*(the3DPointList[i]-centroid));
            p2 = (U*(the3DPointList[i]-centroid));
        }

        else if(U.y() > axis1.y() && axis1.x() >= U.x())
        {
            p1 = (U*(the3DPointList[i]-centroid));
            p2 = (axis1*(the3DPointList[i]-centroid));
        }
        else if(axis1.y() > U.y() && U.x() >= axis1.x())
        {
            p1 = (axis1*(the3DPointList[i]-centroid));
            p2 = (U*(the3DPointList[i]-centroid));
        }

        else
        {
            p1 = (axis1*(the3DPointList[i]-centroid));
            p2 = (U*(the3DPointList[i]-centroid));
        }


        the2DPoint.x()= p1;
        the2DPoint.y()= p2;

        the2DPointsList[i] = the2DPoint;
    }

    return the2DPointsList;

}


RTopoUtils::vec3d RTopoUtils::computePolygonCentroid( const vector<vec3d>& polygon )
{
    vec3d polygonCentroid( vec3d( 0, 0, 0 ) );

    for( vector<vec3d>::const_iterator it = polygon.begin(); it != polygon.end(); ++it )
    {
        const vec3d& vertex_current = ( *it );
        polygonCentroid += vertex_current;
    }
    polygonCentroid /= (double)( polygon.size() );

    return polygonCentroid;
}


RTopoUtils::vec3d RTopoUtils::computeTriangleNormal( const vector<vec3d> thePointsList)
{
    //(Triangle.p2 minus Triangle.p1)
    vec3d U = thePointsList[1] - thePointsList[0];
    //(Triangle.p3 minus Triangle.p1)
    vec3d V = thePointsList[2] - thePointsList[0];

    vec3d Normal = computeCrossProduct(U,V);

    Normal.normalize();

    return Normal;
}


RTopoUtils::vec3d  RTopoUtils::computeQuadNormal( const std::vector<vec3d>& vertices )
{
    vec3d normal(0,0,0) ;

    if(vertices.size() != 4)
    {
        std::cout<<"Error, the number of vertices must be equal to 4 in RTopoUtils::computeQuadNormal";
        return normal;
    }

    for (int i=0; i<4; ++i)
    {
        int j = (i+1)%4;

        normal.x() += (vertices[i].y() - vertices[j].y())*(vertices[i].z() + vertices[j].z());
        normal.y() += (vertices[i].z() - vertices[j].z())*(vertices[i].x() + vertices[j].x());
        normal.z() += (vertices[i].x() - vertices[j].x())*(vertices[i].y() + vertices[j].y());
    }

    normal.normalize();

    return normal;
}


int RTopoUtils::orientation(const Point p,const Point q,const Point r)
{
    double val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());

    if (fabs(val) < DBL_EPSILON)
        return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}
