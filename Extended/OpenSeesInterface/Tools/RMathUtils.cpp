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

#include "RMathUtils.h"
#include <QList>
#include <algorithm>

RMathUtils::RMathUtils()
{

}


RMathUtils::~RMathUtils()
{

}


std::vector<double> RMathUtils::subtractVectors(std::vector<double> const *a, std::vector<double> const *b)
{
    std::vector<double> result;

    // Check for null pointers
    if (a == 0 || b == 0) {
        qCritical() << "A null pointer came into the subtractVectors-function in the matrix operations.";
        return result;
    }

    // Get vector sizes
    int asize = a->size();
    int bsize = b->size();

    // Check for dimensional consistency
    if (asize != bsize ) {
        qCritical() << "Failed to subtract vectors due to dimensional inconsistency";
        return result;
    }

    result.resize(asize);

    // result = a - b
    for (int i=0; i<a->size(); ++i) {
        result[i] = a->at(i)-b->at(i);
    }

    return result;
}


std::vector<double> RMathUtils::addVectors(std::vector<double> const *a, std::vector<double> const *b)
{
    std::vector<double> result;

    // Check for null pointers
    if (a == 0 || b == 0) {
        qCritical() << "A null pointer came into the subtractVectors-function in the matrix operations.";
        return result;
    }

    // Get vector sizes
    int asize = a->size();
    int bsize = b->size();

    // Check for dimensional consistency
    if (asize != bsize ) {
        qCritical() << "Failed to subtract vectors due to dimensional inconsistency";
        return result;
    }

    result.resize(asize);

    // result = a - b
    for (int i=0; i<a->size(); ++i) {
        result[i] = a->at(i)+b->at(i);
    }

    return result;
}


double RMathUtils::sqrdDistanceBtwnPoints(const std::vector<double>& pt1, const std::vector<double>& pt2)
{
    return (pt2[0]-pt1[0])*(pt2[0]-pt1[0])+
            (pt2[1]-pt1[1])*(pt2[1]-pt1[1])+
            (pt2[2]-pt1[2])*(pt2[2]-pt1[2]);
}


std::vector<double> RMathUtils::crossProduct(std::vector<double> const *vector1, std::vector<double> const *vector2)
{
    std::vector<double> crossVector(3);

    // Check for null pointers
    if (!vector1 || !vector2) {
        qCritical() << "A null pointer came into the crossProduct-function in the matrix operations.";
        return crossVector;
    }

    // Check for dimensional consistency...
    if (vector1->size()!=3 || vector2->size()!= 3)
    {
        qCritical()<<"Error: The vectors in a cross-product must all the 3-dimensional.";
        return crossVector;

    } else {

        crossVector[0] = (*vector1)[1] * (*vector2)[2] - (*vector1)[2] * (*vector2)[1];
        crossVector[1] = (*vector1)[2] * (*vector2)[0] - (*vector1)[0] * (*vector2)[2];
        crossVector[2] = (*vector1)[0] * (*vector2)[1] - (*vector1)[1] * (*vector2)[0];

    }

    return crossVector;
}


double RMathUtils::determinant3x3(const std::vector<double> *vector1,const std::vector<double> *vector2,const std::vector<double> *vector3)
{
    // Check for null pointers
    if (vector1 == nullptr || vector2 == nullptr || vector3 == nullptr) {
        qCritical() << "A null pointer came into the determinant3x3-function in the matrix operations.";
        return 0.0;
    }


    // Check for dimensional consistency...
    if (vector1->size()!= 3 || vector2->size() != 3 || vector3->size() != 3)
    {
        qCritical()<<"Error: The vector dimensions do not match for a 3x3 determinant";
        return 0.0;
    }
    else {
        auto determinant =  (*vector1)[0] * (*vector2)[1] * (*vector3)[2] - (*vector1)[0] * (*vector2)[2] * (*vector3)[1] - (*vector1)[1] * (*vector2)[0] * (*vector3)[2] + (*vector1)[1] * (*vector2)[2] * (*vector3)[0] + (*vector1)[2] * (*vector2)[0] * (*vector3)[1] - (*vector1)[2] * (*vector2)[1] * (*vector3)[0];

        return determinant;
    }
}


double RMathUtils::dotProduct(std::vector<double> const *vector1, std::vector<double> const *vector2)
{
    // Check for null pointers
    if (vector1 == 0 || vector2 == 0) {
        qCritical() << "A null pointer came into the dotProduct-function in the matrix operations.";
        return 0.0;
    }

    // Check for dimensional consistency...
    if (vector1->size()!=vector2->size())
    {
        qCritical()<<"Error: The vector dimensions do not match for dot product.";
        return 0.0;
    } else {
        // Evaluate dot product
        double theDotProduct = 0.0;
        for (int i=0; i<vector1->size(); ++i) {
            theDotProduct += (*vector1)[i]*(*vector2)[i];
        }

        return theDotProduct;
    }
}


double RMathUtils::distanceFromPointToPlane(std::vector<RPoint*>& Pln, std::vector<double>& Pnt)
{
    // Distance=|v⋅n| where v = Pnt - Q,  Q is some point on the plane, n is the normalized vector of normal to the plane
    // Find Q, pick a point on a plane -- try the first
    std::vector<double> Q {Pln.at(0)->getXCoordinateValue(), Pln.at(0)->getYCoordinateValue(), Pln.at(0)->getZCoordinateValue()};

    std::vector<double> v = this->subtractVectors(&Pnt,&Q);

    std::vector<double> n = this->computePolygonNormal<RPoint*>(Pln);

    this->normalizeVector(&n);

    auto dist = fabs(this->dotProduct(&v,&n));

    return dist;
}


int RMathUtils::intersect3DTwoPlanes(const std::vector<std::vector<double>>& Pn1,const std::vector<std::vector<double>>& Pn2, std::vector<std::vector<double>>* L )
{
    // Ensure that the incoming vectors are valid and consistent
    // Each plane vector should have 2 subvectors, the first one being a point on the plane and the second the normal vector to the plane
    // Each sub-vector should only hold the x,y,z coordinates, i.e., have size 3!
    if(!L)
        return -1;

    if(L->size() != 2 || L->at(0).size() != 3)
        return -1;

    if(Pn1[0].size() != 3 || Pn1.size() != 2 || Pn2.size() != 2 || Pn2[0].size() != 3)
        return -1;

    double tol = 0.0001;

    // Cross product
    std::vector<double> u = crossProduct(&Pn1[1],&Pn2[1]);

    double ax = (u[0] >= 0 ? u[0] : -u[0]);
    double ay = (u[1] >= 0 ? u[1] : -u[1]);
    double az = (u[2] >= 0 ? u[2] : -u[2]);

    // Pn1 and Pn2 are near parallel
    if((ax+ay+az) < tol)
    {
        // Test if disjoint or coincide
        std::vector<double>   v = subtractVectors(&Pn2[0],&Pn1[0]);

        // If (dot(Pn1.n, v) == 0) Pn2 lies in Pn1
        if (fabs(dotProduct(&Pn1[1], &v)) <= tol)
            return 1; // Pn1 and Pn2 coincide
        else
            return 0; // Pn1 and Pn2 are disjoint
    }

    // Pn1 and Pn2 intersect in a line
    // first determine max abs coordinate of cross product
    int maxc;  // max coordinate
    if(ax > ay) {
        if(ax > az)
            maxc =  1;
        else maxc = 3;
    }
    else
    {
        if(ay > az)
            maxc =  2;
        else maxc = 3;
    }

    // Next, to get a point on the intersect line
    // Zero the max coord, and solve for the other two
    std::vector<double> iP(3); // Intersect point

    // The constants in the 2 plane equations
    double d1 = -dotProduct(&Pn1[1], &Pn1[0]);
    double d2 = -dotProduct(&Pn2[1], &Pn2[0]);

    switch (maxc) {             // select max coordinate
    case 1:                     // intersect with x=0
        iP[0] = Pn1[0][0]; // intersect with x coordinate of the first plane
        iP[1] = (d2*Pn1[1][2] - d1*Pn2[1][2]) /  u[0];
        iP[2] = (d1*Pn2[1][1] - d2*Pn1[1][1]) /  u[0];
        break;
    case 2:
        iP[0] = (d1*Pn2[1][2] - d2*Pn1[1][2]) /  u[1];
        iP[1]= Pn1[0][1]; // intersect with y coordinate of the first plane
        iP[2] = (d2*Pn1[1][0] - d1*Pn2[1][0]) /  u[1];
        break;
    case 3:
        iP[0] = (d2*Pn1[1][1] - d1*Pn2[1][1]) /  u[2];
        iP[1] = (d1*Pn2[1][0] - d2*Pn1[1][0]) /  u[2];
        iP[2] = Pn1[0][2]; // intersect with z coordinate of the first plane
    }

    // Intersection point found
    (*L)[0] = iP;
    (*L)[1] = this->subtractVectors(&iP, &u);

    return 2;
}


int RMathUtils::intersect3DTwoLines(const std::vector<double>& P11,const std::vector<double>& P12,
                                    const std::vector<double>& P21,const std::vector<double>& P22,
                                    std::vector<double>* IPoint )
{


    if(P11.size() !=3 || P12.size() !=3 || P21.size() !=3 || P22.size() !=3)
    {
        qDebug()<<"Error in intersect3DTwoLines, only vectors of three dimensions supported";
        return -1;
    }

    // Get the points (P1,P2) and direction vectors (V1 and V2)
    auto P1 = P11;
    auto V1 = subtractVectors(&P12,&P11);

    auto P2 = P21;
    auto V2 = subtractVectors(&P22,&P21);

    double epsilon = 1.0e-6;

    // Check if the lines are parallel
    auto V1crossV2 = crossProduct(&V1,&V2);

    auto normCrossV1V2 = vectorNorm(&V1crossV2);

    // Lines are parallel or colinear
    if(fabs(normCrossV1V2) < epsilon)
    {
        // Check if colinear
        // Get the endpoints of the lines
        auto P12 = addVectors(&P1,&V1);
        auto P22 = addVectors(&P2,&V2);

        // If the area of the triangle formed by three points is zero, they are colinear
        auto areatriangle = determinant3x3(&P1,&P12,&P2);

        areatriangle+= determinant3x3(&P1,&P12,&P22);

        if(fabs(areatriangle) < epsilon)
            return 2; //Colinear

        // Else they are only parallel
        return 1;
    }

    auto P2minusP1 = subtractVectors(&P2,&P1);

    // t=Det{(P2–P1),V2,V1×V2}/(|V1×V2|^2)
    // s=Det{(P2–P1),V1,V1×V2}/(|V1×V2|^2)
    // Calculate the determinant
    auto determinantt =  determinant3x3(&P2minusP1,&V2,&V1crossV2);
    auto determinants =  determinant3x3(&P2minusP1,&V1,&V1crossV2);

    auto normSqrd = normCrossV1V2*normCrossV1V2;
    // Solve for t and s
    auto t = determinantt/normSqrd;
    auto s = determinants/normSqrd;

    // Solve for the intersection point
    // if (0<=t<=1) and (0<=s<=1) then there is an intersection within the line segments
    // if s and t are outside the 0->1 range, then there is an intersection if PI1 == PI2
    // if PI1 != PI2 the two points are the "closest points" to an intersection of two skew lines

    int returncase =-1;

    // Line1(t)=P1+V1t
    std::vector<double> V1t = scaleVector(t,&V1);

    auto PI1 = addVectors(&P1,&V1t);

    // Line2(s)=P2+V2s
    std::vector<double> V2s = scaleVector(s,&V2);

    auto PI2 = addVectors(&P2,&V2s);

    // They intersect
    if(fabs(PI1[0] - PI2[0]) < epsilon && fabs(PI1[1] - PI2[1]) < epsilon  && fabs(PI1[2] - PI2[2]) < epsilon )
    {
        // Within segment intersection
        if(t>=0.0 && t<=1.0 && s>=0.0 && s<=1.0)
            returncase = 3;
        else // outside intersection
            returncase = 4;

        (*IPoint) = PI1;
    }
    else
    {
        // No intersection
        returncase = 5;
    }

    return returncase;
}


std::vector<double> RMathUtils::scaleVector(double factor, std::vector<double> const *v)
{
    std::vector<double> result;

    // Check for null pointers
    if (v == nullptr) {
        qCritical() << "A null pointer came into the scaleVector-function in the matrix operations.";
        return result;
    }

    // Get sizes
    int vSize = v->size();

    result.resize(vSize);

    for (int i=0; i<vSize; i++) {
        result[i] = factor * (*v)[i];
    }

    return result;
}


int RMathUtils::scaleVectorInPLace(double factor, std::vector<double> const *v)
{
    // Check for null pointers
    if (v == nullptr) {
        qCritical() << "A null pointer came into the scaleVector-function in the matrix operations.";
        return -1;
    }

    for (auto it : *v) {
        it *= factor;
    }

    return 0;
}


double RMathUtils::distance3DLineToLine(const std::vector<std::vector<double>>& L1,const std::vector<std::vector<double>>& L2)
{   
    auto tol = 0.0001;

    std::vector<double>   u = subtractVectors(&L1[1],&L1[0]);
    std::vector<double>   v = subtractVectors(&L2[1],&L2[0]);
    std::vector<double>   w = subtractVectors(&L1[0],&L2[0]);

    auto   a = dotProduct(&u,&u);  // Always >= 0
    auto   b = dotProduct(&u,&v);
    auto   c = dotProduct(&v,&v);  // Always >= 0
    auto   d = dotProduct(&u,&w);
    auto   e = dotProduct(&v,&w);
    auto   D = a*c - b*b;          // Always >= 0
    auto   sc = 0.0;
    auto   tc = 0.0;

    // Compute the line parameters of the two closest points
    // The lines are almost parallel
    if (D < tol)
    {
        sc = 0.0;
        tc = (b>c ? d/b : e/c);    // Use the largest denominator
    }
    else
        sc = (b*e - c*d) / D;
    tc = (a*e - b*d) / D;

    // Get the difference of the two closest points
    // Vector   dP = w + (sc * u) - (tc * v);
    scaleVectorInPLace(sc,&u);
    scaleVectorInPLace(tc,&v);

    auto subResult = subtractVectors(&u,&v);
    auto dP = addVectors(&w,&subResult);

    // return the closest distance
    return vectorNorm(&dP);
}


double RMathUtils::vectorNorm(const std::vector<double> *vector)
{

    // Check for null pointers
    if (vector == 0) {
        qCritical() << "A null pointer came into the norm-function in the matrix operations.";
        return -1.0;
    }

    // Declare and initilize the norm to be returned from this method
    double theNorm = 0.0;

    // Loop over all components of the vector to compute the sum of squares
    for (int i=0; i<vector->size(); ++i) {
        theNorm += vector->at(i)*vector->at(i);
    }

    // Take the square root of the sum of squares, namely the norm
    return sqrt(theNorm);
}


double RMathUtils::normalizeVector(std::vector<double> *vector)
{
    double norm = vectorNorm(vector);
    if (norm>0.0)
    {
        double inv = 1.0/norm;

        for(auto&& it: *vector){
            it *= inv;
        }
    }

    return norm ;
}


#ifdef OSG
// Gets the convex hull of a set of N points --> Graham sweep algo.
void RMathUtils::convexHull(QList<osg::Vec3>& thePointsList)
{
    typedef osg::Vec2 Point;

    // A global point needed for  sorting points with reference to the first point Used in compare function
    Point p0;

    // First convert the 3-D points into 2-D projection..
    int n = thePointsList.size();

    std::vector<Point>points = get2DProjection(thePointsList);

    // Save the original points layout
    std::vector<Point> OrigPoints = points;

    // A lamda function to find next to top in a stack
    auto nextToTop = [](std::stack<Point>&S)
    {
        Point p = S.top();
        S.pop();
        Point res = S.top();
        S.push(p);
        return res;
    };

    // A lamda function to swap two points
    auto swap = [](Point& p1, Point& p2)
    {
        Point temp = p1;
        p1 = p2;
        p2 = temp;
    };

    // A utility function to return square of distance between p1 and p2
    std::function<int(Point,Point)> distSq = [](Point p1, Point p2)
    {
        return (p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y());
    };


    // A function used by library function qsort() to sort an array of
    // points with respect to the first point
    auto compare = [&](Point p1, Point p2)
    {
        // Find orientation
        int o = orientation(p0, p1, p2);
        if (o == 0)
            return (distSq(p0, p2) >= distSq(p0, p1))? -1 : 1;

        return (o == 2)? -1 : 1;
    };


    // Find the bottom most point
    float ymin = points[0].y(), min = 0;

    for (int i = 1; i < n; i++)
    {
        float y = points[i].y();

        // Pick the bottom-most or chose the left
        // most point in case of tie
        if ((y < ymin) || (ymin == y && points[i].x() < points[min].x()))
            ymin = points[i].y(), min = i;
    }

    // Place the bottom-most point at first position
    swap(points[0], points[min]);

    // Sort n-1 points with respect to the first point.
    // A point p1 comes before p2 in sorted ouput if p2
    // has larger polar angle (in counterclockwise
    // direction) than p1
    p0 = points[0];

    std::sort(&points[1], &points[n-1], compare);

    // If two or more points make same angle with p0,
    // Remove all but the one that is farthest from p0
    // Remember that, in above sorting, our criteria was
    // to keep the farthest point at the end when more than
    // one points have same angle.
    int m = 1; // Initialize size of modified array
    for (int i=1; i<n; i++)
    {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while (i < n-1 && orientation(p0, points[i], points[i+1]) == 0)
            i++;

        points[m] = points[i];
        m++;  // Update size of modified array
    }

    // If modified array of points has less than 3 points,
    // convex hull is not possible
    if (m < 3) return;


    QList<osg::Vec3> theNewPointsList;

    // Now rearrage the 3-D Vector according to its sorted 2-D Vector
    for(int i=0; i<n; ++i)
    {
        for(int j=0; j<n; ++j)
        {
            if(points[i] == OrigPoints[j] && i!=j )
            {
                theNewPointsList<<thePointsList[j];
            }
        }
    }

    thePointsList.swap(theNewPointsList);
}


int RMathUtils::orientation(osg::Vec2 p, osg::Vec2 q, osg::Vec2 r)
{
    int val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());

    if (val == 0) return 0;  // Colinear
    return (val > 0)? 1: 2;  // Clock or counterclock wise
}


void RMathUtils::convexHullJarvis(QList<osg::Vec3>& thePointsList)
{
    typedef osg::Vec2 Point;

    // First convert the 3-D points into 2-D projection..

    int n = thePointsList.size();

    std::vector<Point>points = get2DProjection(thePointsList);

    auto orientation = [](Point p, Point q, Point r)
    {
        int val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());

        if (val == 0) return 0;  // colinear
        return (val > 0)? 1: 2; // clock or counterclock wise
    };

    // Initialize Result
    std::vector<Point> hull;

    // Find the bottommost point
    float ymin = points[0].y(), min = 0;

    for (int i = 1; i < n; i++)
    {
        float y = points[i].y();

        // Pick the bottom-most or chose the left
        // most point in case of tie
        if ((y < ymin) || (ymin == y && points[i].x() < points[min].x()))
            ymin = y, min = i;
    }

    // Start from leftmost point, keep moving counterclockwise
    // until reach the start point again.  This loop runs O(h)
    // times where h is number of points in result or output.
    int p = min, q;
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
        for (int i = 0; i < n; i++)
        {
            // If i is more counterclockwise than current q, then
            // update q
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }

        // Now q is the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to
        // result 'hull'
        p = q;

    } while (p != min);  // While we don't come to first point

    QList<osg::Vec3> theNewPointsList;

    //Now rearrage the 3-D Vector according to its sorted 2-D Vector
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(hull[i] == points[j])
            {
                theNewPointsList<<thePointsList[j];
            }
        }
    }

    thePointsList.swap(theNewPointsList);

}


osg::Vec3 RMathUtils::computePolygonNormal( const QList<osg::Vec3>& thePointsList )
{
    const int num_points = thePointsList.size();
    osg::Vec3 polygon_normal( osg::Vec3( 0, 0, 0 ) );

    for (int i=0,j = 1; i<num_points; i++, j++)
    {
        if (j == num_points) j = 0;
        const osg::Vec3& vertex_current = thePointsList[i];
        const osg::Vec3& vertex_next = thePointsList[j];

        polygon_normal.x()+= (((vertex_current.z()) + (vertex_next.z())) * ((vertex_next.y()) - (vertex_current.y())));
        polygon_normal.y()+= (((vertex_current.x()) + (vertex_next.x())) * ((vertex_next.z()) - (vertex_current.z())));
        polygon_normal.z()+= (((vertex_current.y()) + (vertex_next.y())) * ((vertex_next.x()) - (vertex_current.x())));
    }

    polygon_normal.normalize();

    polygon_normal*=-1.0;

    return polygon_normal;
}


std::vector<double> RMathUtils::computePolygonNormal( const std::vector<std::vector<double>>& thePointsList )
{
    const int num_points = thePointsList.size();
    std::vector<double> polygon_normal{0.0,0.0,0.0};

    for (int i=0,j = 1; i<num_points; i++, j++)
    {
        if (j == num_points) j = 0;
        const auto vertex_current = thePointsList[i];
        const auto vertex_next = thePointsList[j];

        polygon_normal[0]+= (((vertex_current[2]) + (vertex_next[2])) * ((vertex_next[1]) - (vertex_current[1])));
        polygon_normal[1]+= (((vertex_current[0]) + (vertex_next[0])) * ((vertex_next[2]) - (vertex_current[2])));
        polygon_normal[2]+= (((vertex_current[1]) + (vertex_next[1])) * ((vertex_next[0]) - (vertex_current[0])));
    }

    this->normalizeVector(&polygon_normal);

    polygon_normal[0] *=-1.0;
    polygon_normal[1] *=-1.0;
    polygon_normal[2] *=-1.0;

    return polygon_normal;
}


std::vector<osg::Vec2> RMathUtils::get2DProjection(QList<osg::Vec3> thePointsList)
{
    const int numPoints = thePointsList.size();

    std::vector<osg::Vec3> the3DPointList(numPoints);

    std::vector<osg::Vec2> the2DPointsList(numPoints);

    for(int i =0;i<numPoints;i++)
    {
        the3DPointList[i] = thePointsList[i];
    }

    auto getAbs = [](osg::Vec3& v)
    {
        osg::Vec3 temp(v);

        v.x()=sqrt(temp.x()*temp.x());
        v.y()=sqrt(temp.y()*temp.y());
        v.z()=sqrt(temp.z()*temp.z());
    };

    osg::Vec3 theNormal = computeTriangleNormal(the3DPointList);

    //then Compute two orthogonal direction vectors to form the orthogonal triplet

    float length = hypotf( theNormal.x(), hypotf(theNormal.y(), theNormal.z()));
    float dir_scalar = (theNormal.x() > 0.0) ? length : -length;
    float xt = theNormal.x() + dir_scalar;
    float dot = -theNormal.y() / (dir_scalar * xt);

    osg::Vec3 theAxis1 ( dot * xt, 1.0f + dot * theNormal.y(), dot * theNormal.z());

    //Cross product to get the second
    osg::Vec3  theAxis2 = theAxis1^(theNormal);

    theAxis2.normalize();

    //define the origin on the plane... use centroid!
    osg::Vec3  centroid = computePolygonCentroid(the3DPointList);

    getAbs(theAxis1);
    getAbs(theAxis2);
    getAbs(theNormal);

    for(int i=0;i<numPoints; i++ )
    {
        osg::Vec2 the2DPoint( osg::Vec2(0,0) );

        float p1 = (theAxis2*(the3DPointList[i]-centroid));
        float p2 = (theAxis1*(the3DPointList[i]-centroid));

        the2DPoint.x()= p1;
        the2DPoint.y()= p2;

        the2DPointsList[i] = the2DPoint;
    }

    return the2DPointsList;

}


osg::Vec3 RMathUtils::computePolygonCentroid( const std::vector<osg::Vec3>& polygon )
{
    osg::Vec3 polygonCentroid( osg::Vec3( 0, 0, 0 ) );

    for( std::vector<osg::Vec3>::const_iterator it = polygon.begin(); it != polygon.end(); ++it )
    {
        const osg::Vec3& vertex_current = ( *it );
        polygonCentroid += vertex_current;
    }
    polygonCentroid /= (double)( polygon.size() );

    return polygonCentroid;
}


osg::Vec3 RMathUtils::computePolygonCentroid( const QList<osg::Vec3>& polygonList )
{
    osg::Vec3 polygonCentroid( osg::Vec3( 0, 0, 0 ) );

    for(int i =0; i<polygonList.size();i++ )
    {
        const osg::Vec3& vertex_current = polygonList.at(i);
        polygonCentroid += vertex_current;
    }
    polygonCentroid /= (double)( polygonList.size() );

    return polygonCentroid;
}


osg::Vec3 RMathUtils::computeTriangleNormal( const std::vector<osg::Vec3> thePointsList)
{
    // (Triangle.p2 minus Triangle.p1)
    osg::Vec3 U = thePointsList[1] - thePointsList[0];
    // (Triangle.p3 minus Triangle.p1)
    osg::Vec3 V = thePointsList[2] - thePointsList[0];

    osg::Vec3 Normal( osg::Vec3( 0, 0, 0 ));

    Normal.x() = ( U.y() * V.z()) - ( U.z() * V.y());
    Normal.y() = ( U.z() * V.x()) - ( U.x() * V.z());
    Normal.z() = ( U.x() * V.y()) - ( U.y() * V.x());

    Normal.normalize();

    return Normal;
}


osg::Matrix RMathUtils::getSquareToQuadTransform(const QVector<double>& theXCoordVector, const QVector<double>& theYCoordVector)
{
    osg::Matrix tx;

    double a, b, c, d, e, f, g, h, i;

    double a11, a12, a13, a21, a22, a23, a31, a32, a33;

    double x0 = theXCoordVector[0];      double y0 = theYCoordVector[0];
    double x1 = theXCoordVector[1];      double y1 = theYCoordVector[1];
    double x2 = theXCoordVector[2];      double y2 = theYCoordVector[2];
    double x3 = theXCoordVector[3];      double y3 = theYCoordVector[3];

    double dx3 = x0 - x1 + x2 - x3;
    double dy3 = y0 - y1 + y2 - y3;

    i = 1.0;

    if ((dx3 == 0.0) && (dy3 == 0.0))
    {
        a = x1 - x0;
        b = x2 - x1;
        c = x0;
        d = y1 - y0;
        e = y2 - y1;
        f = y0;
        g = 0.0;
        h = 0.0;
    }
    else
    {
        double dx1 = x1 - x2;
        double dx2 = x3 - x2;
        double dy1 = y1 - y2;
        double dy2 = y3 - y2;

        double invdet = 1.0/(dx1*dy2 - dx2*dy1);

        g = (dx3*dy2 - dx2*dy3)*invdet;   //g
        h = (dx1*dy3 - dx3*dy1)*invdet;   //h

        a = x1 - x0 + g*x1;
        b = x3 - x0 + h*x3;
        c = x0;

        d = y1 - y0 + g*y1;
        e = y3 - y0 + h*y3;
        f = y0;

    }

    double invdet = 1.0/((x1-x2)*(y3-y2)-(x3-x2)*(y1-y2));

    a31 = (x0-x1+x2-x3)*(y3-y2)-(y0-y1+y2-y3)*(x3-x2);

    a32 = (y0-y1+y2-y3)*(x1-x2)-(x0-x1+x2-x3)*(y1-y2);

    a31 *= invdet;
    a32 *= invdet;

    a11 = x1-x0 + a31*x1;

    a12 = x3-x0 + a32*x3;

    a13 = x0;

    a21 = y1-y0 + a31*y1;

    a22 = y3-y0 + a32*y3;

    a23 = y0;

    a33 = 1.0;

    tx.set(a11,a12,a13,0.0,
           a21,a22,a23,0.0,
           a31,a32,a33,0.0,
           0.0,0.0,0.0,1.0);

    return tx;
}


osg::Matrix  RMathUtils::getQuadToQuadTransform(const QVector<double>& theXCoordVector1, const QVector<double>& theYCoordVector1,const QVector<double>& theXCoordVector2, const QVector<double>& theYCoordVector2)
{

    // First convert quad to square
    osg::Matrix m1 = getQuadToSquareTransform(theXCoordVector1,theYCoordVector1);

    // Then convert the square to quad
    osg::Matrix m2 = getSquareToQuadTransform(theXCoordVector2,theYCoordVector2);

    // Concatenate the two and return transform
    return m1*m2;
}


osg::Matrix  RMathUtils::getQuadToSquareTransform(const QVector<double>& theXCoordVector, const QVector<double>& theYCoordVector)
{
    osg::Matrix tx;

    osg::Matrix t1;

    t1 = this->getSquareToQuadTransform(theXCoordVector,theYCoordVector);

    double dett1 = 1/(t1(0,0)*t1(1,1)*t1(2,2) + t1(0,1)*t1(1,2)*t1(2,0) + t1(0,2)*t1(1,0)*t1(2,1) - t1(2,0)*t1(1,1)*t1(0,2) - t1(2,1)*t1(1,2)*t1(0,0) - t1(2,2)*t1(1,0)*t1(0,1));

    // Now make t1 adjoint
    double m00 = t1(1,1)*t1(2,2) - t1(1,2)*t1(2,1);
    double m01 = t1(1,2)*t1(2,0) - t1(1,0)*t1(2,2);
    double m02 = t1(1,0)*t1(2,1) - t1(1,1)*t1(2,0);
    double m10 = t1(0,2)*t1(2,1) - t1(0,1)*t1(2,2);
    double m11 = t1(0,0)*t1(2,2) - t1(0,2)*t1(2,0);
    double m12 = t1(0,1)*t1(2,0) - t1(0,0)*t1(2,1);
    double m20 = t1(0,1)*t1(1,2) - t1(0,2)*t1(1,1);
    double m21 = t1(0,2)*t1(1,0) - t1(0,0)*t1(1,2);
    double m22 = t1(0,0)*t1(1,1) - t1(0,1)*t1(1,0);


    tx.set(m00*dett1,m10*dett1,m20*dett1,0.0,
           m01*dett1,m11*dett1,m21*dett1,0.0,
           m02*dett1,m12*dett1,m22*dett1,0.0,
           0.0,0.0,0.0,1.0);

    return tx;
}


bool RMathUtils::checkOpenPolygonConvexity( const std::vector<osg::Vec2>& polygon )
{
    if( polygon.size() < 3 )
    {
        return true;
    }

    const size_t num_points = polygon.size();
    double zcrossproduct_previous = 0;
    for( size_t k = 0; k < num_points - 2; ++k )
    {
        const osg::Vec2& vertex_current = polygon[k];
        const osg::Vec2& vertex_next1 = polygon[k + 1];
        const osg::Vec2& vertex_next2 = polygon[k + 2];

        double dx1 = vertex_next1.x() - vertex_current.x();
        double dy1 = vertex_next1.y() - vertex_current.y();

        double dx2 = vertex_next2.x() - vertex_next1.x();
        double dy2 = vertex_next2.y() - vertex_next1.y();

        double zcrossproduct = dx1*dy2 - dy1*dx2;

        if( k > 0 )
        {
            if( std::abs( zcrossproduct ) > 0.0001 )
            {
                if( std::abs( zcrossproduct_previous ) > 0.0001 )
                {
                    if( zcrossproduct*zcrossproduct_previous < 0 )
                    {
                        // There is a change in direction -> not convex
                        return false;
                    }
                }
            }
        }
        zcrossproduct_previous = zcrossproduct;
    }
    return true;
}
#endif

#ifdef IFC
void RMathUtils::convexHullJarvis(std::vector<vec3>& thePointsVec)
{
    typedef vec2 Point;

    // First convert the 3-D points into 2-D projection..

    int n = thePointsVec.size();

    std::vector<Point>points = get2DProjection(thePointsVec);

    auto orientation = [](Point& p, Point& q, Point& r)
    {
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

        if (val == 0) return 0;  // Colinear
        return (val > 0)? 1: 2;  // Clock or counterclock wise
    };

    // Initialize Result
    std::vector<Point> hull;

    // Find the bottommost point
    float ymin = points[0].y, min = 0;

    for (int i = 1; i < n; i++)
    {
        float y = points[i].y;

        // Pick the bottom-most or chose the left most point in case of tie
        if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
            ymin = points[i].y, min = i;
    }

    // Start from leftmost point, keep moving counterclockwise until we reach the start point again.
    // This loop runs O(h) times where h is number of points in result or output.
    int p = min, q;
    do
    {
        // Add current point to result
        hull.push_back(points[p]);

        // Search for a point 'q' such that orientation(p, x,
        // q) is counterclockwise for all points 'x'.
        // The idea is to keep track of last visited most counterclock-wise point in q. If any point 'i' is more counterclock-wise than q, then update q.
        q = (p+1)%n;
        for (int i = 0; i < n; i++)
        {
            // If i is more counterclockwise than current q, then update q
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }

        // Now q is the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to result 'hull'
        p = q;

    } while (p != min);  // While not the first point

    std::vector<vec3> theNewPointsList;

    // Now rearrage the 3-D Vector according to its sorted 2-D Vector
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(hull[i].x == points[j].x && hull[i].y == points[j].y)
            {
                theNewPointsList.push_back(thePointsVec[j]);
            }
        }
    }

    std::swap(thePointsVec,theNewPointsList);
}


vec3 RMathUtils::getMidpointCarve3D(vec3 point1, vec3 point2)
{
    vec3 midpoint;

    midpoint[0] = point1[0]+ (point2[0]-point1[0])*0.5;
    midpoint[1] = point1[1]+ (point2[1]-point1[1])*0.5;
    midpoint[2] = point1[2]+ (point2[2]-point1[2])*0.5;

    return midpoint;
}


void RMathUtils::convexHullJarvis(std::vector<vec2>& thePointsVec)
{
    typedef vec2 Point;

    // First convert the 3-D points into 2-D projection..

    int n = thePointsVec.size();

    std::vector<Point>points = thePointsVec;

    auto orientation = [](Point& p, Point& q, Point& r)
    {
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

        if (val == 0) return 0;  // colinear
        return (val > 0)? 1: 2; // clock or counterclock wise
    };

    // Initialize Result
    std::vector<Point> hull;

    // Find the bottommost point
    float ymin = points[0].y, min = 0;

    for (int i = 1; i < n; ++i)
    {
        float y = points[i].y;

        // Pick the bottom-most or chose the left
        // most point in case of tie
        if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
            ymin = points[i].y, min = i;
    }

    // Start from leftmost point, keep moving counterclockwise until reach the start point again. This loop runs O(h) times where h is number of points in result or output.
    int p = min, q;
    do
    {
        // Add current point to result
        hull.push_back(points[p]);

        // Search for a point 'q' such that orientation(p, x,q) is counterclockwise for all points 'x'.
        // The idea is to keep track of last visited most counterclock-wise point in q.
        // If any point 'i' is more counterclock-wise than q, then update q.
        q = (p+1)%n;
        for (int i = 0; i < n; ++i)
        {
            // If i is more counterclockwise than current q, then update q
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }

        // Now q is the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to result 'hull'
        p = q;

    } while (p != min);

    std::vector<vec2> theNewPointsList;

    // Now rearrage the 3-D Vector according to its sorted 2-D Vector
    for(int i=0;i<n;++i)
    {
        for(int j=0;j<n;++j)
        {
            if(hull[i].x == points[j].x && hull[i].y == points[j].y)
            {
                theNewPointsList.push_back(thePointsVec[j]);
            }
        }
    }

    std::swap(thePointsVec,theNewPointsList);
}


vec2 RMathUtils::getMidpointCarve2D(vec2 point1, vec2 point2)
{
    vec2 midpoint;

    midpoint[0] = point1[0]+(point2[0]-point1[0])*0.5;
    midpoint[1] = point1[1]+(point2[1]-point1[1])*0.5;

    return midpoint;
}


double RMathUtils::polygonArea(const std::vector<vec3>& vertices)
{
    // This assumes that the polygon is convex; i.e. not self-intersecting and that all of the points sit on a plane
    // To do: check for this!!

    std::vector<double> total = {0, 0, 0};

    const size_t numVert = vertices.size();

    for(size_t i = 0; i<numVert; ++i)
    {
        std::vector<double> vi1 = {vertices[i][0],vertices[i][1],vertices[i][2]};

        std::vector<double>  vi2;

        if(i == numVert-1)
            vi2 = {vertices[0][0],vertices[0][1],vertices[0][2]};
        else
            vi2 = {vertices[i+1][0],vertices[i+1][1],vertices[i+1][2]};

        auto theCrossProduct = this->crossProduct(&vi1,&vi2);

        total[0] += theCrossProduct[0];
        total[1] += theCrossProduct[1];
        total[2] += theCrossProduct[2];
    }

    auto unitNormal = this->computePolygonNormal(vertices);

    auto result = this->dotProduct(&total,&unitNormal);

    return fabs(result/2.0);
}


std::vector<vec2> RMathUtils::get2DProjection(const std::vector<vec3>& thePointsVec)
{
    const int numPoints = thePointsVec.size();

    std::vector<vec3> the3DPointList = thePointsVec;

    std::vector<vec2> the2DPointsList(numPoints);

    auto getAbs = [](vec3& v)
    {
        vec3 temp(v);

        v.x=sqrt(temp.x*temp.x);
        v.y=sqrt(temp.y*temp.y);
        v.z=sqrt(temp.z*temp.z);
    };

    vec3 theNormal = computeTriangleNormal(the3DPointList);

    // Compute two orthogonal direction vectors to form the orthogonal triplet
    float length = hypotf( theNormal.x, hypotf(theNormal.y, theNormal.z));
    float dirScalar = (theNormal.x > 0.0) ? length : -length;
    float xt = theNormal.x + dirScalar;
    float dot = -theNormal.y / (dirScalar * xt);

    vec3 theAxis1 ( carve::geom::VECTOR(dot * xt, 1.0f + dot * theNormal.y, dot * theNormal.z));

    // Cross product to get the second
    vec3  theAxis2 = carve::geom::cross(theAxis1,theNormal);

    theAxis2.normalize();

    // Define the origin on the plane... use centroid!
    vec3  centroid = computePolygonCentroid(the3DPointList);

    getAbs(theAxis1);
    getAbs(theAxis2);
    getAbs(theNormal);

    for(int i=0;i<numPoints; ++i )
    {
        vec2 the2DPoint( carve::geom::VECTOR(0,0) );

        auto p1 = carve::geom::dot(theAxis2,the3DPointList[i]-centroid);
        auto p2 = carve::geom::dot(theAxis1,the3DPointList[i]-centroid);

        the2DPoint.x= p1;
        the2DPoint.y= p2;

        the2DPointsList[i] = the2DPoint;
    }

    return the2DPointsList;

}


vec3 RMathUtils::computeTriangleNormal(const std::vector<vec3>& thePointsList)
{
    // (Triangle.p2 minus Triangle.p1)
    vec3 U = thePointsList[1] - thePointsList[0];
    // (Triangle.p3 minus Triangle.p1)
    vec3 V = thePointsList[2] - thePointsList[0];

    vec3 Normal(carve::geom::VECTOR( 0, 0, 0 ));

    Normal.x = ( U.y * V.z) - ( U.z * V.y);
    Normal.y = ( U.z * V.x) - ( U.x * V.z);
    Normal.z = ( U.x * V.y) - ( U.y * V.x);

    Normal.normalize();

    return Normal;
}


std::vector<double> RMathUtils::computePolygonNormal( const std::vector<vec3>& polyVertices )
{
    //To do: check if the polygon is degenerate, if so then this will not work.

    const size_t num_points = polyVertices.size();
    std::vector<double> polygon_normal(3);

    for (size_t i=0,j = 1; i<num_points; ++i, ++j)
    {
        if (j == num_points) j = 0;
        const vec3& vertex_current = polyVertices[i];
        const vec3& vertex_next = polyVertices[j];

        polygon_normal[0]+= (((vertex_current[2]) + (vertex_next[2])) * ((vertex_next[1]) - (vertex_current[1])));
        polygon_normal[1]+= (((vertex_current[0]) + (vertex_next[0])) * ((vertex_next[2]) - (vertex_current[2])));
        polygon_normal[2]+= (((vertex_current[1]) + (vertex_next[1])) * ((vertex_next[0]) - (vertex_current[0])));
    }

    this->normalizeVector(&polygon_normal);

    return polygon_normal;
}
#endif


double RMathUtils::computeLengthWithPoints(RPoint *point1, RPoint *point2)
{
    auto xcoord1 = point1->getXCoordinateValue();
    auto xcoord2 = point2->getXCoordinateValue();
    auto ycoord1 = point1->getYCoordinateValue();
    auto ycoord2 = point2->getYCoordinateValue();
    auto zcoord1 = point1->getZCoordinateValue();
    auto zcoord2 = point2->getZCoordinateValue();

    auto dx = xcoord2-xcoord1;
    auto dy = ycoord2-ycoord1;
    auto dz = zcoord2-zcoord1;

    return sqrt(dx*dx + dy*dy + dz*dz);
}


double RMathUtils::computeLengthWithNodes(RNode *node1, RNode *node2)
{
    auto xcoord1 = node1->getXCoordinateValue();
    auto xcoord2 = node2->getXCoordinateValue();
    auto ycoord1 = node1->getYCoordinateValue();
    auto ycoord2 = node2->getYCoordinateValue();
    auto zcoord1 = node1->getZCoordinateValue();
    auto zcoord2 = node2->getZCoordinateValue();

    auto dx = xcoord2-xcoord1;
    auto dy = ycoord2-ycoord1;
    auto dz = zcoord2-zcoord1;

    return sqrt(dx*dx + dy*dy + dz*dz);
}


std::vector<double> RMathUtils::getMidpoint(std::vector<double>& point1, std::vector<double>& point2)
{
    std::vector<double> midpoint(3);

    midpoint.at(0) = point1.at(0)+(point2.at(0)-point1.at(0))*0.5;
    midpoint.at(1) = point1.at(1)+(point2.at(1)-point1.at(1))*0.5;
    midpoint.at(2) = point1.at(2)+(point2.at(2)-point1.at(2))*0.5;

    return midpoint;
}


int RMathUtils::makeRotate( const std::vector<double>& from, const std::vector<double>& to, std::vector<double>* Quat )
{
    //Test to make sure the vectors have are three dimensional
    if(from.size() != 3 || to.size() != 3)
    {
        qDebug()<<"The input vectors must be of dimension 3 in "<<__FUNCTION__;
        return -1;
    }

    if(Quat == nullptr)
    {
        qDebug()<<"The Quaternion in "<<__FUNCTION__<<" is  a nullptr";
        return -1;
    }

    if(Quat->size() != 4)
    {
        qDebug()<<"The Quaternion must be of dimension 4 in "<<__FUNCTION__;
        return -1;
    }

    // This routine takes any vector as argument but normalized vectors are necessary, if only for computing the dot product.
    // Too bad the API is that generic, it leads to performance loss.
    // Even in the case the 2 vectors are not normalized but same length, the sqrt could be shared, but we have no way to know beforehand at this point, while the caller may know.
    // So, we have to test... in the hope of saving at least a sqrt
    auto sourceVector = from;
    auto targetVector = to;

    auto fromLen2 = from[0]*from[0] + from[1]*from[1] + from[2]*from[2];
    auto fromLen = 0.0;

    // Normalize only when necessary, epsilon test
    if ((fromLen2 < 1.0-1e-7) || (fromLen2 > 1.0+1e-7)) {
        fromLen = sqrt(fromLen2);
        //        sourceVector /= fromLen;
        sourceVector[0] /= fromLen;
        sourceVector[1] /= fromLen;
        sourceVector[2] /= fromLen;

    } else fromLen = 1.0;

    auto toLen2 = to[0]*to[0] + to[1]*to[1] + to[2]*to[2];

    // Normalize only when necessary, epsilon test
    if ((toLen2 < 1.0-1e-7) || (toLen2 > 1.0+1e-7)) {
        auto toLen = 0.0;
        // Re-use fromLen for case of mapping 2 vectors of the same length
        if ((toLen2 > fromLen2-1e-7) && (toLen2 < fromLen2+1e-7)) {
            toLen = fromLen;
        }
        else toLen = sqrt(toLen2);
        targetVector[0] /= toLen;
        targetVector[1] /= toLen;
        targetVector[2] /= toLen;
    }


    // Now let's get into the real stuff
    // Use "dot product plus one" as test as it can be re-used later on
    auto dotProdPlus1 = 1.0 + this->dotProduct(&sourceVector,&targetVector);

    // Check for degenerate case of full u-turn. Use epsilon for detection
    if (dotProdPlus1 < 1e-7) {

        // Get an orthogonal vector of the given vector in a plane with maximum vector coordinates.
        // Then use it as quaternion axis with pi angle
        // Trick is to realize one value at least is >0.6 for a normalized vector.
        if (fabs(sourceVector[0]) < 0.6) {
            const double norm = sqrt(1.0 - sourceVector[0] * sourceVector[0]);
            (*Quat)[0] = 0.0;
            (*Quat)[1] = sourceVector[2] / norm;
            (*Quat)[2] = -sourceVector[1] / norm;
            (*Quat)[3] = 0.0;
        } else if (fabs(sourceVector[1]) < 0.6) {
            const double norm = sqrt(1.0 - sourceVector[1] * sourceVector[1]);
            (*Quat)[0] = -sourceVector[2] / norm;
            (*Quat)[1] = 0.0;
            (*Quat)[2] = sourceVector[0] / norm;
            (*Quat)[3] = 0.0;
        } else {
            const double norm = sqrt(1.0 - sourceVector[2] * sourceVector[2]);
            (*Quat)[0] = sourceVector[1] / norm;
            (*Quat)[1] = -sourceVector[0] / norm;
            (*Quat)[2] = 0.0;
            (*Quat)[3] = 0.0;
        }
    }

    else {
        // Find the shortest angle (*Quat)ernion that transforms normalized vectors into one other. Formula is still valid when vectors are colinear
        const double s = sqrt(0.5 * dotProdPlus1);
        std::vector<double> tmp = this->crossProduct(&sourceVector, &targetVector);
        tmp[0] /= (2.0*s);
        tmp[1] /= (2.0*s);
        tmp[2] /= (2.0*s);

        (*Quat)[0] = tmp[0];
        (*Quat)[1] = tmp[1];
        (*Quat)[2] = tmp[2];
        (*Quat)[3] = s;
    }

    return 0;
}


int RMathUtils::makeRotate(const double angle, std::vector<double>& vec, std::vector<double>* Quat)
{
    if(vec.size() != 3)
    {
        qDebug()<<"Error, the rotation axis must of dimension 3 in "<<__FUNCTION__;
        return -1;
    }

    return this->makeRotate(angle,vec[0],vec[1],vec[2],Quat);
}


int RMathUtils::makeRotate(const double angle,const double x,const double y,const double z, std::vector<double>* Quat)
{
    if(Quat == nullptr)
    {
        qDebug()<<"Error, the Quaternion in "<<__FUNCTION__<<" is a nullptr";
        return -1;
    }

    const double epsilon = 0.0000001;

    auto length = sqrt( x*x + y*y + z*z );
    if (length < epsilon)
    {
        // Zero length axis, so reset rotation to zero.
        (*Quat)[0]=0.0; (*Quat)[1]=0.0; (*Quat)[2]=0.0; (*Quat)[3]=0.0;
        return -1;
    }

    auto inversenorm  = 1.0/length;
    auto coshalfangle = cos( 0.5*angle );
    auto sinhalfangle = sin( 0.5*angle );

    (*Quat)[0] = x * sinhalfangle * inversenorm;
    (*Quat)[1] = y * sinhalfangle * inversenorm;
    (*Quat)[2] = z * sinhalfangle * inversenorm;
    (*Quat)[3] = coshalfangle;

    return 0;
}


int RMathUtils::getRotatationMatrixFromQuat(const std::vector<double>& Quat, std::vector<std::vector<double>>* Mat)
{

    if(Mat == nullptr)
    {
        qDebug()<<"The Matrix in "<<__FUNCTION__<<" is  a nullptr";
        return -1;
    }

    if(Quat.size() != 4)
    {
        qDebug()<<"The Quaternion must be of dimension 4 in "<<__FUNCTION__;
        return -1;
    }

    if(Mat->size() != 4 && Mat->at(0).size() != 4)
    {
        qDebug()<<"The Matrix must be of dimension 4x4 in "<<__FUNCTION__;
        return -1;
    }

    double length2 = Quat[0]*Quat[0] + Quat[1]*Quat[1] + Quat[2]*Quat[2] + Quat[3]*Quat[3];

    if (fabs(length2) <= std::numeric_limits<double>::min())
    {
        (*Mat)[0][0] = 0.0; (*Mat)[1][0] = 0.0; (*Mat)[2][0] = 0.0;
        (*Mat)[0][1] = 0.0; (*Mat)[1][1] = 0.0; (*Mat)[2][1] = 0.0;
        (*Mat)[0][2] = 0.0; (*Mat)[1][2] = 0.0; (*Mat)[2][2] = 0.0;
    }
    else
    {
        double rlength2;
        // Normalize quat if required.
        // We can avoid the expensive sqrt in this case since all 'coefficients' below are products of two q components.
        // That is a square of a square root, so it is possible to avoid that
        if (length2 != 1.0)
        {
            rlength2 = 2.0/length2;
        }
        else
        {
            rlength2 = 2.0;
        }

        // Source: Gamasutra, Rotating Objects Using Quaternions
        //http://www.gamasutra.com/features/19980703/quaternions_01.htm
        double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

        // calculate coefficients
        x2 = rlength2*Quat[0];
        y2 = rlength2*Quat[1];
        z2 = rlength2*Quat[2];

        xx = Quat[0] * x2;
        xy = Quat[0] * y2;
        xz = Quat[0] * z2;

        yy = Quat[1] * y2;
        yz = Quat[1] * z2;
        zz = Quat[2] * z2;

        wx = Quat[3] * x2;
        wy = Quat[3] * y2;
        wz = Quat[3] * z2;

        // Note.  Gamasutra gets the matrix assignments inverted, resulting in left-handed rotations, which is contrary to OpenGL and OSG's methodology.
        // The matrix assignment has been altered in the next few lines of code to do the right thing.
        // Don Burns - Oct 13, 2001
        (*Mat)[0][0] = 1.0 - (yy + zz);
        (*Mat)[1][0] = xy - wz;
        (*Mat)[2][0] = xz + wy;


        (*Mat)[0][1] = xy + wz;
        (*Mat)[1][1] = 1.0 - (xx + zz);
        (*Mat)[2][1] = yz - wx;

        (*Mat)[0][2] = xz - wy;
        (*Mat)[1][2] = yz + wx;
        (*Mat)[2][2] = 1.0 - (xx + yy);
    }

    return 0;
}


int RMathUtils::getRotatationMatrixFromQuat(const std::vector<double>& Quat, QVector< QVector<double> >* Mat)
{

    if(Mat == nullptr)
    {
        qDebug()<<"The Matrix in "<<__FUNCTION__<<" is  a nullptr";
        return -1;
    }

    if(Quat.size() != 4)
    {
        qDebug()<<"The Quaternion must be of dimension 4 in "<<__FUNCTION__;
        return -1;
    }

    if(Mat->size() != 4 && Mat->at(0).size() != 4)
    {
        qDebug()<<"The Matrix must be of dimension 4x4 in "<<__FUNCTION__;
        return -1;
    }

    double length2 = Quat[0]*Quat[0] + Quat[1]*Quat[1] + Quat[2]*Quat[2] + Quat[3]*Quat[3];

    if (fabs(length2) <= std::numeric_limits<double>::min())
    {
        (*Mat)[0][0] = 0.0; (*Mat)[1][0] = 0.0; (*Mat)[2][0] = 0.0;
        (*Mat)[0][1] = 0.0; (*Mat)[1][1] = 0.0; (*Mat)[2][1] = 0.0;
        (*Mat)[0][2] = 0.0; (*Mat)[1][2] = 0.0; (*Mat)[2][2] = 0.0;
    }
    else
    {
        double rlength2;
        // Normalize quat if required.
        // We can avoid the expensive sqrt in this case since all 'coefficients' below are products of two q components.
        // That is a square of a square root, so it is possible to avoid that
        if (length2 != 1.0)
        {
            rlength2 = 2.0/length2;
        }
        else
        {
            rlength2 = 2.0;
        }

        // Source: Gamasutra, Rotating Objects Using Quaternions
        //http://www.gamasutra.com/features/19980703/quaternions_01.htm

        double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

        // Calculate coefficients
        x2 = rlength2*Quat[0];
        y2 = rlength2*Quat[1];
        z2 = rlength2*Quat[2];

        xx = Quat[0] * x2;
        xy = Quat[0] * y2;
        xz = Quat[0] * z2;

        yy = Quat[1] * y2;
        yz = Quat[1] * z2;
        zz = Quat[2] * z2;

        wx = Quat[3] * x2;
        wy = Quat[3] * y2;
        wz = Quat[3] * z2;

        // Note.  Gamasutra gets the matrix assignments inverted, resulting in left-handed rotations, which is contrary to OpenGL and OSG's methodology.
        // The matrix assignment has been altered in the next few lines of code to do the right thing.
        // Don Burns - Oct 13, 2001
        (*Mat)[0][0] = 1.0 - (yy + zz);
        (*Mat)[1][0] = xy - wz;
        (*Mat)[2][0] = xz + wy;


        (*Mat)[0][1] = xy + wz;
        (*Mat)[1][1] = 1.0 - (xx + zz);
        (*Mat)[2][1] = yz - wx;

        (*Mat)[0][2] = xz - wy;
        (*Mat)[1][2] = yz + wx;
        (*Mat)[2][2] = 1.0 - (xx + yy);
    }


    return 0;
}


std::vector<double> RMathUtils::preMult(const std::vector<double>& v, std::vector<std::vector<double>>* Mat) const
{
    auto d = 1.0/((*Mat)[0][3]*v[0]+(*Mat)[1][3]*v[1]+(*Mat)[2][3]*v[2]+(*Mat)[3][3]) ;

    return std::vector<double>{ ((*Mat)[0][0]*v[0] + (*Mat)[1][0]*v[1] + (*Mat)[2][0]*v[2] + (*Mat)[3][0])*d,
                ((*Mat)[0][1]*v[0] + (*Mat)[1][1]*v[1] + (*Mat)[2][1]*v[2] + (*Mat)[3][1])*d,
                ((*Mat)[0][2]*v[0] + (*Mat)[1][2]*v[1] + (*Mat)[2][2]*v[2] + (*Mat)[3][2])*d};
}


std::vector<double> RMathUtils::postMult(const std::vector<double>& v, std::vector<std::vector<double>>* Mat) const
{
    auto d = 1.0/((*Mat)[3][0]*v[0]+(*Mat)[3][1]*v[1]+(*Mat)[3][2]*v[2]+(*Mat)[3][3]) ;
    return std::vector<double>{ ((*Mat)[0][0]*v[0] + (*Mat)[0][1]*v[1] + (*Mat)[0][2]*v[2] + (*Mat)[0][3])*d,
                ((*Mat)[1][0]*v[0] + (*Mat)[1][1]*v[1] + (*Mat)[1][2]*v[2] + (*Mat)[1][3])*d,
                ((*Mat)[2][0]*v[0] + (*Mat)[2][1]*v[1] + (*Mat)[2][2]*v[2] + (*Mat)[2][3])*d} ;
}


vec2 RMathUtils::computePolygonCentroid2D(const std::vector<vec2>& polygon)
{
    vec2 polygon_centroid( carve::geom::VECTOR( 0, 0) );

    for( auto&& it : polygon)
    {
        const vec2& vertex_current = it ;
        polygon_centroid += vertex_current;
    }
    auto size = (double)polygon.size();
    polygon_centroid /= size;
    return polygon_centroid;
}


vec3 RMathUtils::computePolygonCentroid( const std::vector<vec3>& polygon )
{
    vec3 polygon_centroid( carve::geom::VECTOR( 0, 0, 0 ) );

    for( auto&& it : polygon)
    {
        const vec3& vertex_current = it ;
        polygon_centroid += vertex_current;
    }
    auto size = (double)polygon.size();
    polygon_centroid /= size;
    return polygon_centroid;
}


bool RMathUtils::pointsColinear( const vec3& a, const vec3& b, const vec3& c, const double & dotTol)
{
    auto va = b - a;
    auto vb = c - b;

    va.normalize();
    vb.normalize();

    if(1.0 - std::abs(carve::geom::dot(va,vb)) <= dotTol)
        return true; // Edge normals match collinearity condition

    return false; // Not collinear
}

//vec3 getMidpointCarve3D(vec3 point1, vec3 point2);

//vec2 getMidpointCarve2D(vec2 point1, vec2 point2);


