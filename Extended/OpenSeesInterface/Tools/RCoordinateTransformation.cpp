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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RCoordinateTransformation.h"
#include "RDomain.h"
#include "RMatrixOperations.h"
#include "RPoint.h"
#include "RNode.h"
#include "RMathUtils.h"

#ifdef OSG
#include <osg/Vec3>
#include <osg/Group>
#endif

#include <math.h>
#include <assert.h>
#include <qmath.h>

RCoordinateTransformation::RCoordinateTransformation(RDomain *domain)
    : RMethod(domain, nullptr)
{

}


RCoordinateTransformation::~RCoordinateTransformation()
{

}


int RCoordinateTransformation::getRotationMatrixWithPoints(RPoint *point1, RPoint *point2, double orientation, QVector< QVector<double> > *R)
{
    // Compute length and cosines
    double dx, dy, dz, L;
    computeLengthAndCosinesWithPoints(point1, point2, dx, dy, dz, L);


    // Establish the rotation matrix
    getRotationMatrixWithCosines(dx, dy, dz, L, orientation, R);

    return 0;
}


int RCoordinateTransformation::getRotationMatrixWithNodes(RNode *node1, RNode *node2, double orientation, QVector< QVector<double> > *R)
{
    // Compute length and cosines
    double dx, dy, dz, L;
    computeLengthAndCosinesWithNodes(node1, node2, dx, dy, dz, L);


    // Establish the rotation matrix
    getRotationMatrixWithCosines(dx, dy, dz, L, orientation, R);

    return 0;
}


int RCoordinateTransformation::getRotationMatrixWithCosines(double dx, double dy, double dz, double L, double orientation, QVector< QVector<double> > *R)
{
    if (L==0.0) {
        qCritical() << "A coordinate transformation cannot be established for two coinciding nodes.";
        return -1;
    }

    // Declare the local vectors that are sitting the global system
    double x0, x1, x2, y0, y1, y2, z0, z1, z2;

    if ( (dy==0.0) && (dx==0.0) ) {

        // If the element is vertical then things are simple;
        // align the local x-axis with the global z-axis
        // align the local y-axis with the global y-axs
        // align the local z-axis with the negative global x-axis
        x0 = 0.0;
        x1 = 0.0;
        x2 = 1.0;

        y0 = 0.0;
        y1 = 1.0;
        y2 = 0.0;

        z0 = -1.0;
        z1 = 0.0;
        z2 = 0.0;

    }
    else {

        // The element is NOT vertical; define a v-vector that lies in the x-z-plane
        // of the local element, and point it in the global z-direction
        double v0, v1, v2;
        v0 = 0.0;
        v1 = 0.0;
        v2 = 1.0;


        // The x-vector is along the element
        x0 = dx/L;
        x1 = dy/L;
        x2 = dz/L;


        // The y-vector is the cross product of v and x
        y0 = v1*x2 - v2*x1;
        y1 = v2*x0 - v0*x2;
        y2 = v0*x1 - v1*x0;


        // Normalize it
        double norm = sqrt(y0*y0+y1*y1+y2*y2);
        y0 /= norm;
        y1 /= norm;
        y2 /= norm;


        // The z-vector is cross product of x and y
        z0 = x1*y2 - x2*y1;
        z1 = x2*y0 - x0*y2;
        z2 = x0*y1 - x1*y0;


        // Normalize it (is this necessary?)
        norm = sqrt(z0*z0+z1*z1+z2*z2);
        z0 /= norm;
        z1 /= norm;
        z2 /= norm;

    }


    // Set the rotation matrix, which has the above x, y, z vectors as COLUMS
    (*R)[0][0] = x0;       (*R)[0][1] = y0;       (*R)[0][2] = z0;
    (*R)[1][0] = x1;       (*R)[1][1] = y1;       (*R)[1][2] = z1;
    (*R)[2][0] = x2;       (*R)[2][1] = y2;       (*R)[2][2] = z2;


    // If the "orientation" is non-zero, add a local rotation matrix
    if (orientation!=0.0) {

        // Initial declarations
        RMatrixOperations theMatrixOperations(theDomain);
        QVector< QVector<double> > localR(3, QVector<double>(3));


        // Set the local x-vector
        double x0 = 1.0;
        double x1 = 0.0;
        double x2 = 0.0;


        // Compute the y-vector
        double y0 = 0.0;
        double y1 = cos(orientation/180.0*PI);
        double y2 = sin(orientation/180.0*PI);


        // Set the local z-vector
        double z0 = 0.0;
        double z1 = -sin(orientation/180.0*PI);
        double z2 = cos(orientation/180.0*PI);


        // Set the rotation matrix, which has the x, y, and z vectors above as COLUMNS
        localR[0][0] = x0;       localR[0][1] = y0;       localR[0][2] = z0;
        localR[1][0] = x1;       localR[1][1] = y1;       localR[1][2] = z1;
        localR[2][0] = x2;       localR[2][1] = y2;       localR[2][2] = z2;


        // Apply the local rotation first, then the global rotation
        theMatrixOperations.multiplyMatrixMatrix(false, false, 1.0, R, &localR, 0.0, R);
    }


    // Clean out numerical noise (this is tricky...)
    double tol = 1e-13;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if (fabs((*R)[i][j]) < tol) {
                (*R)[i][j] = 0.0;
            }
        }
    }


    return 0;
}


int RCoordinateTransformation::getRotationMatrixWithCosinesTwo(double dx, double dy, double dz, double L, double orientation, QVector< QVector<double> > *R)
{
    if (L==0.0) {
        qCritical() << "A coordinate transformation cannot be established for two coinciding nodes.";
        return -1;
    }

    // Declare the local vectors that are sitting the global system
    double x0, x1, x2, y0, y1, y2, z0, z1, z2;

    if ( (dy==0.0) && (dx==0.0) ) {

        // If the element is vertical then things are simple;
        // align the local x-axis with the global z-axis
        // align the local y-axis with the global y-axs
        // align the local z-axis with the negative global x-axis
        x0 = 0.0;
        x1 = 0.0;
        x2 = 1.0;

        y0 = 0.0;
        y1 = 1.0;
        y2 = 0.0;

        if(dz > 0.0)
            z0 = 1.0;
        else
            z0 = -1.0;
        z1 = 0.0;
        z2 = 0.0;

    }
    else {

        // The element is NOT vertical; define a v-vector that lies in the x-z-plane
        // of the local element, and point it in the global z-direction
        double v0, v1, v2;
        v0 = 0.0;
        v1 = 0.0;
        v2 = 1.0;


        // The x-vector is along the element
        x0 = dx/L;
        x1 = dy/L;
        x2 = dz/L;


        // The y-vector is the cross product of v and x
        y0 = v1*x2 - v2*x1;
        y1 = v2*x0 - v0*x2;
        y2 = v0*x1 - v1*x0;


        // Normalize it
        double norm = sqrt(y0*y0+y1*y1+y2*y2);
        y0 /= norm;
        y1 /= norm;
        y2 /= norm;


        // The z-vector is cross product of x and y
        z0 = x1*y2 - x2*y1;
        z1 = x2*y0 - x0*y2;
        z2 = x0*y1 - x1*y0;


        // Normalize it (is this necessary?)
        norm = sqrt(z0*z0+z1*z1+z2*z2);
        z0 /= norm;
        z1 /= norm;
        z2 /= norm;

    }


    // Set the rotation matrix, which has the above x, y, z vectors as COLUMS
    (*R)[0][0] = x0;       (*R)[0][1] = y0;       (*R)[0][2] = z0;
    (*R)[1][0] = x1;       (*R)[1][1] = y1;       (*R)[1][2] = z1;
    (*R)[2][0] = x2;       (*R)[2][1] = y2;       (*R)[2][2] = z2;


    // If the "orientation" is non-zero, add a local rotation matrix
    if (orientation!=0.0) {

        // Initial declarations
        RMatrixOperations theMatrixOperations(theDomain);
        QVector< QVector<double> > localR(3, QVector<double>(3));


        // Set the local x-vector
        double x0 = 1.0;
        double x1 = 0.0;
        double x2 = 0.0;


        // Compute the y-vector
        double y0 = 0.0;
        double y1 = cos(orientation/180.0*PI);
        double y2 = sin(orientation/180.0*PI);


        // Set the local z-vector
        double z0 = 0.0;
        double z1 = -sin(orientation/180.0*PI);
        double z2 = cos(orientation/180.0*PI);


        // Set the rotation matrix, which has the x, y, and z vectors above as COLUMNS
        localR[0][0] = x0;       localR[0][1] = y0;       localR[0][2] = z0;
        localR[1][0] = x1;       localR[1][1] = y1;       localR[1][2] = z1;
        localR[2][0] = x2;       localR[2][1] = y2;       localR[2][2] = z2;


        // Apply the local rotation first, then the global rotation
        theMatrixOperations.multiplyMatrixMatrix(false, false, 1.0, R, &localR, 0.0, R);
    }


    // Clean out numerical noise (this is tricky...)
    double tol = 1e-13;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if (fabs((*R)[i][j]) < tol) {
                (*R)[i][j] = 0.0;
            }
        }
    }


    return 0;
}


int RCoordinateTransformation::computeLengthAndCosinesWithPoints(RPoint *point1, RPoint *point2, double &dx, double &dy, double &dz, double &L)
{
    double xcoord1 = point1->getXCoordinateValue();
    double xcoord2 = point2->getXCoordinateValue();
    double ycoord1 = point1->getYCoordinateValue();
    double ycoord2 = point2->getYCoordinateValue();
    double zcoord1 = point1->getZCoordinateValue();
    double zcoord2 = point2->getZCoordinateValue();

    dx = xcoord2-xcoord1;
    dy = ycoord2-ycoord1;
    dz = zcoord2-zcoord1;

    L = sqrt(dx*dx + dy*dy + dz*dz);

    return 0;
}


int RCoordinateTransformation::computeLengthAndCosinesWithNodes(RNode *node1, RNode *node2, double &dx, double &dy, double &dz, double &L)
{
    double xcoord1 = node1->getXCoordinateValue();
    double xcoord2 = node2->getXCoordinateValue();
    double ycoord1 = node1->getYCoordinateValue();
    double ycoord2 = node2->getYCoordinateValue();
    double zcoord1 = node1->getZCoordinateValue();
    double zcoord2 = node2->getZCoordinateValue();

    dx = xcoord2-xcoord1;
    dy = ycoord2-ycoord1;
    dz = zcoord2-zcoord1;

    L = sqrt(dx*dx + dy*dy + dz*dz);

    return 0;
}


int RCoordinateTransformation::transformVectorToGlobal(RNode *node1, RNode *node2, double orientation, QVector<double> *vector)
{
    // Descipher the size of the vector
    int size = vector->size();
    if (size % 3 != 0) {
        qCritical() << "Cannot transform a vector with number of components not divisible by 3.";
        return -1;
    }
    int numBlocks = size/3;


    // Compute length and cosines
    double dx, dy, dz, L;
    computeLengthAndCosinesWithNodes(node1, node2, dx, dy, dz, L);


    // Establish the rotation matrix
    QVector< QVector<double> > R(3, QVector<double>(3));
    getRotationMatrixWithCosines(dx, dy, dz, L, orientation, &R);


    // Carry out the multiplication globalvector = T^T * localvector = R * localvector
    RMatrixOperations theMatrixOperations(theDomain);
    QVector<double> tempLocal(3);
    QVector<double> tempGlobal(3);
    for (int i=0; i<numBlocks; i++) {
        tempLocal[0] = (*vector)[i*3];
        tempLocal[1] = (*vector)[i*3+1];
        tempLocal[2] = (*vector)[i*3+2];
        theMatrixOperations.multiplyMatrixVector(false, 1.0, &R, &tempLocal, 0.0, &tempGlobal);
        (*vector)[i*3] = tempGlobal[0];
        (*vector)[i*3+1] = tempGlobal[1];
        (*vector)[i*3+2] = tempGlobal[2];
    }

    return 0;
}


#ifdef OSG
int RCoordinateTransformation::transformVectorToGlobal(osg::Vec3 *Vec1, osg::Vec3 *Vec2, double orientation, osg::Vec3 *result)
{
    QVector<double> vector(3);

    vector[0]= (*result)[0];
    vector[1]= (*result)[1];
    vector[2]= (*result)[2];


    // Compute length and cosines
    double dx, dy, dz, L;

    dx = (*Vec2)[0]-(*Vec1)[0];
    dy = (*Vec2)[1]-(*Vec1)[1];
    dz = (*Vec2)[2]-(*Vec1)[2];

    L = sqrt(dx*dx + dy*dy + dz*dz);


    // Establish the rotation matrix
    QVector< QVector<double> > R(3, QVector<double>(3));
    getRotationMatrixWithCosinesTwo(dx, dy, dz, L, orientation, &R);

    // Carry out the multiplication globalvector = T^T * localvector = R * localvector
    RMatrixOperations theMatrixOperations(theDomain);

    QVector<double> tempGlobal(3);

    theMatrixOperations.multiplyMatrixVector(false, 1.0, &R, &vector, 0.0, &tempGlobal);

    (*result)[0] = tempGlobal[0];
    (*result)[1] = tempGlobal[1];
    (*result)[2] = tempGlobal[2];

    return 0;
}
#endif


int RCoordinateTransformation::transformVectorToLocal(RNode *node1, RNode *node2, double orientation, QVector<double> *vector)
{
    // Descipher the size of the vector
    int size = vector->size();
    if (size % 3 != 0) {
        qCritical() << "Cannot transform a vector with number of components not divisible by 3.";
        return -1;
    }
    int numBlocks = size/3;


    // Compute length and cosines
    double dx, dy, dz, L;
    computeLengthAndCosinesWithNodes(node1, node2, dx, dy, dz, L);


    // Establish the rotation matrix
    QVector< QVector<double> > R(3, QVector<double>(3));
    getRotationMatrixWithCosines(dx, dy, dz, L, orientation, &R);


    // Carry out the multiplication localvector = T * globalvector = R^T * globalvector
    RMatrixOperations theMatrixOperations(theDomain);
    QVector<double> tempLocal(3);
    QVector<double> tempGlobal(3);
    for (int i=0; i<numBlocks; i++) {
        tempLocal[0] = (*vector)[i*3];
        tempLocal[1] = (*vector)[i*3+1];
        tempLocal[2] = (*vector)[i*3+2];
        theMatrixOperations.multiplyMatrixVector(true, 1.0, &R, &tempLocal, 0.0, &tempGlobal);
        (*vector)[i*3] = tempGlobal[0];
        (*vector)[i*3+1] = tempGlobal[1];
        (*vector)[i*3+2] = tempGlobal[2];
    }

    return 0;
}


int RCoordinateTransformation::transformMatrixToGlobal(RNode *node1, RNode *node2, double orientation, QVector< QVector<double> > *matrix)
{
    // Descipher the size of the matrix
    int size = matrix->size();
    if (size % 3 != 0) {
        qCritical() << "Cannot transform a matrix with number of rows not divisible by 3.";
        return -1;
    }
    int numBlocks = size/3;


    // Compute length and cosines
    double dx, dy, dz, L;
    computeLengthAndCosinesWithNodes(node1, node2, dx, dy, dz, L);


    // Establish the rotation matrix
    QVector< QVector<double> > R(3, QVector<double>(3));
    getRotationMatrixWithCosines(dx, dy, dz, L, orientation, &R);


    // Carry out the multiplication globalvector = T^T * localmatrix * T = R * localmatrix * R^T
    RMatrixOperations theMatrixOperations(theDomain);
    QVector< QVector<double> > tempLocal(3, QVector<double>(3));
    QVector< QVector<double> > tempGlobal(3, QVector<double>(3));
    QVector< QVector<double> > temp(3, QVector<double>(3));

    for (int i=0; i<numBlocks; i++) {

        for (int j=0; j<numBlocks; j++) {

            tempLocal[0][0] = (*matrix)[i*3][j*3];     tempLocal[0][1] = (*matrix)[i*3][j*3+1];     tempLocal[0][2] = (*matrix)[i*3][j*3+2];
            tempLocal[1][0] = (*matrix)[i*3+1][j*3];   tempLocal[1][1] = (*matrix)[i*3+1][j*3+1];   tempLocal[1][2] = (*matrix)[i*3+1][j*3+2];
            tempLocal[2][0] = (*matrix)[i*3+2][j*3];   tempLocal[2][1] = (*matrix)[i*3+2][j*3+1];   tempLocal[2][2] = (*matrix)[i*3+2][j*3+2];

            theMatrixOperations.multiplyMatrixMatrix(false, false, 1.0, &R, &tempLocal, 0.0, &temp);
            theMatrixOperations.multiplyMatrixMatrix(false, true, 1.0, &temp, &R, 0.0, &tempGlobal);

            (*matrix)[i*3][j*3] = tempGlobal[0][0];     (*matrix)[i*3][j*3+1] = tempGlobal[0][1];     (*matrix)[i*3][j*3+2] = tempGlobal[0][2];
            (*matrix)[i*3+1][j*3] = tempGlobal[1][0];   (*matrix)[i*3+1][j*3+1] = tempGlobal[1][1];   (*matrix)[i*3+1][j*3+2] = tempGlobal[1][2];
            (*matrix)[i*3+2][j*3] = tempGlobal[2][0];   (*matrix)[i*3+2][j*3+1] = tempGlobal[2][1];   (*matrix)[i*3+2][j*3+2] = tempGlobal[2][2];
        }
    }

    return 0;
}


double RCoordinateTransformation::getAreaWithFourNodes(RNode *node1, RNode *node2, RNode *node3, RNode *node4)
{
    // Please check that the points lie in a plane


    // Create a matrix operations tool
    RMatrixOperations theMatrixOperations(theDomain);


    // Original nodal coordinates
    double xcoord1 = node1->getXCoordinateValue();
    double xcoord2 = node2->getXCoordinateValue();
    double xcoord3 = node3->getXCoordinateValue();
    double xcoord4 = node4->getXCoordinateValue();

    double ycoord1 = node1->getYCoordinateValue();
    double ycoord2 = node2->getYCoordinateValue();
    double ycoord3 = node3->getYCoordinateValue();
    double ycoord4 = node4->getYCoordinateValue();

    double zcoord1 = node1->getZCoordinateValue();
    double zcoord2 = node2->getZCoordinateValue();
    double zcoord3 = node3->getZCoordinateValue();
    double zcoord4 = node4->getZCoordinateValue();


    // Determine vectors shooting out from 1 and 3
    QVector<double> vector12(3);
    vector12[0] = xcoord2-xcoord1;
    vector12[1] = ycoord2-ycoord1;
    vector12[2] = zcoord2-zcoord1;

    QVector<double> vector14(3);
    vector14[0] = xcoord4-xcoord1;
    vector14[1] = ycoord4-ycoord1;
    vector14[2] = zcoord4-zcoord1;

    QVector<double> vector32(3);
    vector32[0] = xcoord2-xcoord3;
    vector32[1] = ycoord2-ycoord3;
    vector32[2] = zcoord2-zcoord3;

    QVector<double> vector34(3);
    vector34[0] = xcoord4-xcoord3;
    vector34[1] = ycoord4-ycoord3;
    vector34[2] = zcoord4-zcoord3;





    // The area of each triangle is half the norm of the cross-product
    QVector<double> cross12by14(3);
    theMatrixOperations.crossProduct(&vector12, &vector14, &cross12by14);

    QVector<double> cross34by32(3);
    theMatrixOperations.crossProduct(&vector34, &vector32, &cross34by32);

    return 0.5 * (theMatrixOperations.norm(&cross12by14) + theMatrixOperations.norm(&cross34by32));
}


double RCoordinateTransformation::getProjectedCoordinatesWithFourNodes(RNode *node1, RNode *node2, RNode *node3, RNode *node4, QVector<double> *x, QVector<double> *y)
{
    // The element is oriented in 3D space, but calculations are done in the 2D x-y plane.


    // Create a matrix operations tool
    RMatrixOperations theMatrixOperations(theDomain);


    // Original nodal coordinates
    double xcoord1 = node1->getXCoordinateValue();
    double xcoord2 = node2->getXCoordinateValue();
    double xcoord3 = node3->getXCoordinateValue();
    double xcoord4 = node4->getXCoordinateValue();

    double ycoord1 = node1->getYCoordinateValue();
    double ycoord2 = node2->getYCoordinateValue();
    double ycoord3 = node3->getYCoordinateValue();
    double ycoord4 = node4->getYCoordinateValue();

    double zcoord1 = node1->getZCoordinateValue();
    double zcoord2 = node2->getZCoordinateValue();
    double zcoord3 = node3->getZCoordinateValue();
    double zcoord4 = node4->getZCoordinateValue();


    // Determine a few vectors in the element plane
    QVector<double> vector12(3);
    vector12[0] = xcoord2-xcoord1;
    vector12[1] = ycoord2-ycoord1;
    vector12[2] = zcoord2-zcoord1;

    QVector<double> vector13(3);
    vector13[0] = xcoord3-xcoord1;
    vector13[1] = ycoord3-ycoord1;
    vector13[2] = zcoord3-zcoord1;

    QVector<double> vector14(3);
    vector14[0] = xcoord4-xcoord1;
    vector14[1] = ycoord4-ycoord1;
    vector14[2] = zcoord4-zcoord1;


    // Check that the element is plane, i.e., that two normal vectors are parallel
    QVector<double> cross12by13(3);
    theMatrixOperations.crossProduct(&vector12, &vector13, &cross12by13);

    QVector<double> cross12by14(3);
    theMatrixOperations.crossProduct(&vector12, &vector14, &cross12by14);

    double dotProductOfNormals = theMatrixOperations.dotProduct(&cross12by13, &cross12by14);
    double normOfCross12by13 = theMatrixOperations.norm(&cross12by13);
    double normOfCross12by14 = theMatrixOperations.norm(&cross12by14);

    double shouldBeUnity = dotProductOfNormals / (normOfCross12by13 * normOfCross12by14);


    if (qAbs(shouldBeUnity-1.0) > 0.00000001) {
        qCritical() << "A linear Quad4 element is not plane.";
    }


    // The first node is by definition set to the origin
    (*x)[0] = 0.0;
    (*y)[0] = 0.0;


    // The second node is by definition set along the x-axis
    double distance12 = theMatrixOperations.norm(&vector12);
    (*x)[1] = distance12;
    (*y)[1] = 0.0;


    // The y-vector is the negative of the cross-product of the x-axis and the normal vector
    QVector<double> yvector(3);
    theMatrixOperations.crossProduct(&vector12, &cross12by14, &yvector);
    theMatrixOperations.scaleVector(-1.0, &yvector, &yvector);


    // Normalize the x and y vectors
    QVector<double> unitx(3);
    theMatrixOperations.scaleVector(1.0/distance12, &vector12, &unitx);

    QVector<double> unity(3);
    theMatrixOperations.scaleVector(1.0/theMatrixOperations.norm(&yvector), &yvector, &unity);


    // The coordinates of the third and fourth node are obtained by dot products
    (*x)[2] = theMatrixOperations.dotProduct(&vector13, &unitx);
    (*x)[3] = theMatrixOperations.dotProduct(&vector14, &unitx);

    (*y)[2] = theMatrixOperations.dotProduct(&vector13, &unity);
    (*y)[3] = theMatrixOperations.dotProduct(&vector14, &unity);



    // Determine the orientation
    // (The vectors unit_x, unit_y, and unit_z are the columns of the R matrix, but we like to express
    //  things in terms of the x-axis and an "orientation" so we can use the methods created for beams)
    double orientation=0;
    if (unitx[0]==0.0 && unitx[1]==0.0) {

        // The local x-axis is vertical; determine the angle between the local y-axis and the global y-axis
        QVector<double> globaly(3);
        globaly[1] = 1.0;
        orientation = 180.0/PI*acos( theMatrixOperations.dotProduct(&unity, &globaly) / (theMatrixOperations.norm(&unity) * theMatrixOperations.norm(&globaly)) );

    }
    else {

        // Here we want the angle between the local y-vector and the local y-axis if that axis was horizontal
        QVector<double> globalz(3);
        globalz[2] = 1.0;
        QVector<double> horizontalLocalY(3);
        theMatrixOperations.crossProduct(&unitx, &globalz, &horizontalLocalY);

        double dotUnity = theMatrixOperations.dotProduct(&unity, &horizontalLocalY);
        double normUnitY = theMatrixOperations.norm(&unity);
        double normHorizY =  theMatrixOperations.norm(&horizontalLocalY);

        double aCos = dotUnity/normUnitY*normHorizY;

        aCos = qRound(aCos*10000)/10000.0;  //SG Add CHECK THIS!!!

        orientation = 180.0/PI*acos(aCos);
    }



    // If the orientation is different from 0, 90, 180, etc. there may be problems with the drilling DOF...   
    double result = std::remainder(std::fabs(orientation), 90.0);

    if (result >= 1e-6) {
        qCritical() << "A plane element is oriented with an incline: That means drilling DOFs are NOT automatically removed and the structure may be unstable.";
    }

    return orientation;
}


double RCoordinateTransformation::getProjectedCoordinatesWithNineNodes(RNode *node1, RNode *node2, RNode *node3, RNode *node4, RNode *node5,RNode *node6,RNode *node7,RNode *node8,RNode *node9, QVector<double> *x, QVector<double> *y) //this is called to convert x and y in "projected" coordinates SG add
{
    //SG add this whole function
    // The element is oriented in 3D space, but calculations are done in the 2D x-y plane.


    // Create a matrix operations tool
RMatrixOperations theMatrixOperations(theDomain);


    // Original nodal coordinates
    double xcoord1 = node1->getXCoordinateValue();
    double xcoord2 = node2->getXCoordinateValue();
    double xcoord3 = node3->getXCoordinateValue();
    double xcoord4 = node4->getXCoordinateValue();
    double xcoord5 = node5->getXCoordinateValue();
    double xcoord6 = node6->getXCoordinateValue();
    double xcoord7 = node7->getXCoordinateValue();
    double xcoord8 = node8->getXCoordinateValue();
    double xcoord9 = node9->getXCoordinateValue();

    double ycoord1 = node1->getYCoordinateValue();
    double ycoord2 = node2->getYCoordinateValue();
    double ycoord3 = node3->getYCoordinateValue();
    double ycoord4 = node4->getYCoordinateValue();
    double ycoord5 = node5->getYCoordinateValue();
    double ycoord6 = node6->getYCoordinateValue();
    double ycoord7 = node7->getYCoordinateValue();
    double ycoord8 = node8->getYCoordinateValue();
    double ycoord9 = node9->getYCoordinateValue();

    double zcoord1 = node1->getZCoordinateValue();
    double zcoord2 = node2->getZCoordinateValue();
    double zcoord3 = node3->getZCoordinateValue();
    double zcoord4 = node4->getZCoordinateValue();
    double zcoord5 = node5->getZCoordinateValue();
    double zcoord6 = node6->getZCoordinateValue();
    double zcoord7 = node7->getZCoordinateValue();
    double zcoord8 = node8->getZCoordinateValue();
    double zcoord9 = node9->getZCoordinateValue();



    // Determine a few vectors in the element plane
    QVector<double> vector12(3);
    vector12[0] = xcoord2-xcoord1;
    vector12[1] = ycoord2-ycoord1;
    vector12[2] = zcoord2-zcoord1;

    QVector<double> vector13(3);
    vector13[0] = xcoord3-xcoord1;
    vector13[1] = ycoord3-ycoord1;
    vector13[2] = zcoord3-zcoord1;

    QVector<double> vector14(3);
    vector14[0] = xcoord4-xcoord1;
    vector14[1] = ycoord4-ycoord1;
    vector14[2] = zcoord4-zcoord1;

    QVector<double> vector15(3);
    vector15[0] = xcoord5-xcoord1;
    vector15[1] = ycoord5-ycoord1;
    vector15[2] = zcoord5-zcoord1;

    QVector<double> vector16(3);
    vector16[0] = xcoord6-xcoord1;
    vector16[1] = ycoord6-ycoord1;
    vector16[2] = zcoord6-zcoord1;

    QVector<double> vector17(3);
    vector17[0] = xcoord7-xcoord1;
    vector17[1] = ycoord7-ycoord1;
    vector17[2] = zcoord7-zcoord1;

    QVector<double> vector18(3);
    vector18[0] = xcoord8-xcoord1;
    vector18[1] = ycoord8-ycoord1;
    vector18[2] = zcoord8-zcoord1;

    QVector<double> vector19(3);
    vector19[0] = xcoord9-xcoord1;
    vector19[1] = ycoord9-ycoord1;
    vector19[2] = zcoord9-zcoord1;


    // Check that the element is plane, i.e., that two normal vectors are parallel
    QVector<double> cross12by13(3);
    theMatrixOperations.crossProduct(&vector12, &vector13, &cross12by13);

    QVector<double> cross12by14(3);
    theMatrixOperations.crossProduct(&vector12, &vector14, &cross12by14);

    double dotProductOfNormals = theMatrixOperations.dotProduct(&cross12by13, &cross12by14);
    double normOfCross12by13 = theMatrixOperations.norm(&cross12by13);
    double normOfCross12by14 = theMatrixOperations.norm(&cross12by14);

    double shouldBeUnity = dotProductOfNormals / (normOfCross12by13 * normOfCross12by14);


    if (qAbs(shouldBeUnity-1.0) > 0.00000001) {
        qCritical() << "A linear Quad4 element is not plane.";
    }


    // The first node is by definition set to the origin
    (*x)[0] = 0.0;  //Node1
    (*y)[0] = 0.0;


    // The second node is by definition set along the x-axis
    double distance12 = theMatrixOperations.norm(&vector12);
    (*x)[1] = distance12;   //Node2
    (*y)[1] = 0.0;


    // The y-vector is the negative of the cross-product of the x-axis and the normal vector
    QVector<double> yvector(3);
    theMatrixOperations.crossProduct(&vector12, &cross12by13, &yvector);
    theMatrixOperations.scaleVector(-1.0, &yvector, &yvector);


    // Normalize the x and y vectors
    QVector<double> unit_x(3);
    theMatrixOperations.scaleVector(1.0/distance12, &vector12, &unit_x);

    QVector<double> unit_y(3);
    theMatrixOperations.scaleVector(1.0/theMatrixOperations.norm(&yvector), &yvector, &unit_y);


    // The coordinates of the remaining nodes are obtained by dot products
    (*x)[2] = theMatrixOperations.dotProduct(&vector13, &unit_x); //Node3
    (*y)[2] = theMatrixOperations.dotProduct(&vector13, &unit_y);

    (*x)[3] = theMatrixOperations.dotProduct(&vector14, &unit_x);  //Node4
    (*y)[3] = theMatrixOperations.dotProduct(&vector14, &unit_y);

    (*x)[4] = theMatrixOperations.dotProduct(&vector15, &unit_x);  //Node5
    (*y)[4] = theMatrixOperations.dotProduct(&vector15, &unit_y);

    (*x)[5] = theMatrixOperations.dotProduct(&vector16, &unit_x);  //Node6
    (*y)[5] = theMatrixOperations.dotProduct(&vector16, &unit_y);

    (*x)[6] = theMatrixOperations.dotProduct(&vector17, &unit_x);  //Node7
    (*y)[6] = theMatrixOperations.dotProduct(&vector17, &unit_y);

    (*x)[7] = theMatrixOperations.dotProduct(&vector18, &unit_x);  //Node8
    (*y)[7] = theMatrixOperations.dotProduct(&vector18, &unit_y);

    (*x)[8] = theMatrixOperations.dotProduct(&vector19, &unit_x);  //Node9
    (*y)[8] = theMatrixOperations.dotProduct(&vector19, &unit_y);



    // Determine the orientation
    // (The vectors unit_x, unit_y, and unit_z are the columns of the R matrix, but we like to express
    //  things in terms of the x-axis and an "orientation" so we can use the methods created for beams) ??
    if (unit_x[0]==0.0 && unit_x[1]==0.0) {

        // The local x-axis is vertical; determine the angle between the local y-axis and the global y-axis
        QVector<double> globaly(3);
        globaly[1] = 1.0;
        return 180.0/PI*acos( theMatrixOperations.dotProduct(&unit_y, &globaly) / (theMatrixOperations.norm(&unit_y) * theMatrixOperations.norm(&globaly)) );

    }
    else {

        // Here we want the angle between the local y-vector and the local y-axis if that axis was horizontal
        QVector<double> globalz(3);
        globalz[2] = 1.0;
        QVector<double> horizontalLocalY(3);
        theMatrixOperations.crossProduct(&unit_x, &globalz, &horizontalLocalY);
        return 180.0/PI*acos( theMatrixOperations.dotProduct(&unit_y, &horizontalLocalY) / (theMatrixOperations.norm(&unit_y) * theMatrixOperations.norm(&horizontalLocalY)) );
    }
}


int RCoordinateTransformation::getVectorInXZPlane(RNode *node1, RNode *node2, std::vector<double>* vector, double orientation)
{

    if(node1 == nullptr || node2 == nullptr)
    {
        qDebug()<<"Error the function "<<__FUNCTION__<<" required nodes";
        return -1;
    }

    if(vector == nullptr)
    {
        qDebug()<<"Error the function "<<__FUNCTION__<<" required an input vector";
        return -1;
    }

    if(vector->size() != 3)
    {
        qDebug()<<"Error the input vector in "<<__FUNCTION__<<" needs to be of dimension 3";
        return -1;
    }

    RMathUtils theMathUtils;

    // Assume the element has a local x-axis along the global x-axis (1.0,0.0,0.0) and its "orientation" or depth is along the global z-axis (0.0,0.0,1.0)
    double xcoord1 = node1->getXCoordinateValue();
    double xcoord2 = node2->getXCoordinateValue();
    double ycoord1 = node1->getYCoordinateValue();
    double ycoord2 = node2->getYCoordinateValue();
    double zcoord1 = node1->getZCoordinateValue();
    double zcoord2 = node2->getZCoordinateValue();

    auto dx = xcoord2-xcoord1;
    auto dy = ycoord2-ycoord1;
    auto dz = zcoord2-zcoord1;

    auto L = sqrt(dx*dx + dy*dy + dz*dz);

    if(L < 0.001)
    {
        qDebug()<<"Error, the element length is zero in"<<__FUNCTION__;
        return -1;
    }

    // The global x-axis is along the element
    auto xL0 = dx/L;
    auto xL1 = dy/L;
    auto xL2 = dz/L;

    // Initialize the  vectors
    std::vector<double> xLocal = {1.0,0.0,0.0};
    std::vector<double> zLocal = {0.0,0.0,1.0};
    std::vector<double> xGlobal  = {xL0,xL1,xL2};

    // Create the result vector - for now z-global is aligned with the local, it will stay that way if the orientation is zero
    std::vector<double> result = zLocal;

    // If an orientation is given, first rotate the local-z by the orientation amount before doing anything else
    if(orientation != 0.0)
    {
        // Create the empty Quaternion
        std::vector<double> Quat(4);

        // Orientation is in radians
        auto angleRad = -1.0*orientation*PI/180.0;

        // Set a quaternion which will perform a rotation of an angle around the axis given by the vector (x,y,z).
        // In this case the rotation axis is along the global x-axis as the element is still aling along this axis
        auto res = theMathUtils.makeRotate(angleRad,xLocal,&Quat);
        if(res != 0)
        {
            qDebug()<<"Error making the rotation Quaternion in "<<__FUNCTION__;
            return -1;
        }

        // Define a 4x4 rotation matrix
        std::vector< std::vector<double> > RMat(4, std::vector<double>(4));

        // Make identity
        RMat[0][0] = 1.0;     RMat[1][1] = 1.0;    RMat[2][2] = 1.0;    RMat[3][3] = 1.0;

        // Get the rotation matrix from the quaternion
        auto res2 = theMathUtils.getRotatationMatrixFromQuat(Quat,&RMat);
        if(res2 != 0)
        {
            qDebug()<<"Error making the rotation Matrix in "<<__FUNCTION__;
            return -1;
        }

        // Rotate the local z-axis according to the orientation amount by multiplying with the rotation matrix
        result = theMathUtils.preMult(zLocal,&RMat);
    }


    // The next step involves rotating the local element into its global position
    // Right now the local element x-axis is still aligned with the global x-axis

    // Create the empty Quaternion
    std::vector<double> Quat(4);

    // Make a rotation Quaternion, which will rotate local x-axis to global x-axis
    auto res = theMathUtils.makeRotate(xLocal,xGlobal,&Quat);
    if(res != 0)
    {
        qDebug()<<"Error making the rotation Quaternion in "<<__FUNCTION__;
        return -1;
    }

    // Define the 4x4 rotation matrix
    std::vector< std::vector<double> > RMat2(4, std::vector<double>(4));

    // Make identity
    RMat2[0][0] = 1.0;     RMat2[1][1] = 1.0;    RMat2[2][2] = 1.0;    RMat2[3][3] = 1.0;

    // Get the rotation matrix from the quaternion
    auto res2 = theMathUtils.getRotatationMatrixFromQuat(Quat,&RMat2);
    if(res2 != 0)
    {
        qDebug()<<"Error making the rotation Matrix in "<<__FUNCTION__;
        return -1;
    }

    // Transform the local z-vector into the global z-vector
    auto finalResult = theMathUtils.preMult(result,&RMat2);

    (*vector)[0]=finalResult[0];
    (*vector)[1]=finalResult[1];
    (*vector)[2]=finalResult[2];

    // Test to compare to OSG algorithm that do something similar
    // osg::Vec3 xGlobalT(1.0,0.0,0.0);
    // osg::Vec3 xLocalT(xL0,xL1,xL2);
    // osg::Vec3 zGlobalT(0.0,0.0,1.0);
    // osg::Matrix Mat1;
    // Mat1.makeRotate(xGlobalT,xLocalT);
    // auto testQuat = Mat1.getRotate();
    // auto resTest = zGlobalT*Mat1;

    return 0;
}


int RCoordinateTransformation::transformVectorToLocalII(RNode *node1, RNode *node2, std::vector<double> orientationVec, QVector<double> *vector)
{
    // Descipher the size of the vector
    int size = vector->size();
    if (size % 3 != 0) {
        qCritical() << "Cannot transform a vector with number of components not divisible by 3.";
        return -1;
    }
    int numBlocks = size/3;


    // Compute length and cosines
    double dx, dy, dz, L;
    computeLengthAndCosinesWithNodes(node1, node2, dx, dy, dz, L);

    RMathUtils theMathUtils;

    // The global x-axis is along the element
    auto xL0 = dx/L;
    auto xL1 = dy/L;
    auto xL2 = dz/L;
    std::vector<double> xLocal  = {xL0,xL1,xL2};

    //The global x-axis
    std::vector<double> xGlobal = {1.0,0.0,0.0};

    //Create an empty Quaternion
    std::vector<double> Quat(4);

    //Make a rotation Quat which will rotate global x-axis to local x-axis
    auto res = theMathUtils.makeRotate(xGlobal,xLocal,&Quat);
    if(res != 0)
    {
        qDebug()<<"Error making the rotation Quaternion in "<<__FUNCTION__;
        return -1;
    }

//    //Define a 4x4 rotation matrix
//    QVector< QVector<double> > R(4, QVector<double>(4));

//    //Make identity
//    R[0][0] = 1.0;     R[1][1] = 1.0;    R[2][2] = 1.0;    R[3][3] = 1.0;

//    //Get the rotation matrix from the quaternion
//    auto res2 = theMathUtils.getRotatationMatrixFromQuat(Quat,&R);
//    if(res2 != 0)
//    {
//        qDebug()<<"Error making the rotation Matrix in "<<__FUNCTION__;
//        return -1;
//    }

     QVector< QVector<double> > R(3, QVector<double>(3));

     this->getRotationMatrix(node1,node2,orientationVec,&R);

//    QVector<double> temp(3);
//    QVector<double> tempTest(3);
//    temp[0] = 0.0;
//    temp[1] = 0.0;
//    temp[2] = 1.0;

//    auto ug1 = temp[0];
//    auto ug2 = temp[1];
//    auto ug3 = temp[2];

//    tempTest[0] = R[0][0]*ug1 + R[0][1]*ug2 + R[0][2]*ug3;
//    tempTest[1] = R[1][0]*ug1 + R[1][1]*ug2 + R[1][2]*ug3;
//    tempTest[2] = R[2][0]*ug1 + R[2][1]*ug2 + R[2][2]*ug3;

    for (int i=0; i<numBlocks; ++i)
    {
        auto ind1 = i*3;
        auto ind2 = ind1+1;
        auto ind3 = ind1+2;

        auto ug1 = (*vector)[ind1];
        auto ug2 = (*vector)[ind2];
        auto ug3 = (*vector)[ind3];

        (*vector)[ind1] = R[0][0]*ug1 + R[0][1]*ug2 + R[0][2]*ug3;
        (*vector)[ind2] = R[1][0]*ug1 + R[1][1]*ug2 + R[1][2]*ug3;
        (*vector)[ind3] = R[2][0]*ug1 + R[2][1]*ug2 + R[2][2]*ug3;

    }


    return 0;
}


int RCoordinateTransformation::transformVectorToGlobalII(RNode *node1, RNode *node2, std::vector<double> orientationVec, QVector<double> *vector)
{
    // Descipher the size of the vector
    int size = vector->size();
    if (size % 3 != 0) {
        qCritical() << "Cannot transform a vector with number of components not divisible by 3.";
        return -1;
    }
    int numBlocks = size/3;


    // Compute length and cosines
    double dx, dy, dz, L;
    computeLengthAndCosinesWithNodes(node1, node2, dx, dy, dz, L);

    RMathUtils theMathUtils;

    // The global x-axis is along the element
    auto x0 = dx/L;
    auto x1 = dy/L;
    auto x2 = dz/L;
    std::vector<double> xLocal  = {x0,x1,x2};

    //The local x-axis is along the global x-axis
    std::vector<double> xGlobal = {1.0,0.0,0.0};

//    //Create an empty Quaternion
//    std::vector<double> Quat(4);

//    //Make a rotation Quat which will rotate local x-axis to global x-axis
//    auto res = theMathUtils.makeRotate(xLocal,xGlobal,&Quat);
//    if(res != 0)
//    {
//        qDebug()<<"Error making the rotation Quaternion in "<<__FUNCTION__;
//        return -1;
//    }

//    //Define a 4x4 rotation matrix
//    QVector< QVector<double> > R(4, QVector<double>(4));

//    //Make identity
//    R[0][0] = 1.0;     R[1][1] = 1.0;    R[2][2] = 1.0;    R[3][3] = 1.0;

//    //Get the rotation matrix from the quaternion
//    auto res2 = theMathUtils.getRotatationMatrixFromQuat(Quat,&R);
//    if(res2 != 0)
//    {
//        qDebug()<<"Error making the rotation Matrix in "<<__FUNCTION__;
//        return -1;
//    }

    QVector< QVector<double> > R(3, QVector<double>(3));

    this->getRotationMatrix(node1,node2,orientationVec,&R);

//    QVector<double> temp(4);
//    QVector<double> tempTest(4);
//    temp[0] = -1.0;
//    temp[1] = 0.0;
//    temp[2] = 0.0;

//    auto ug1 = temp[0];
//    auto ug2 = temp[1];
//    auto ug3 = temp[2];

//    tempTest[0] = R[0][0]*ug1 + R[1][0]*ug2 + R[2][0]*ug3;
//    tempTest[1] = R[0][1]*ug1 + R[1][1]*ug2 + R[2][1]*ug3;
//    tempTest[2] = R[0][2]*ug1 + R[1][2]*ug2 + R[2][2]*ug3;

    for (int i=0; i<numBlocks; ++i)
    {
        auto ind1 = i*3;
        auto ind2 = ind1+1;
        auto ind3 = ind1+2;

        auto ug1 = (*vector)[ind1];
        auto ug2 = (*vector)[ind2];
        auto ug3 = (*vector)[ind3];

        (*vector)[ind1] = R[0][0]*ug1 + R[1][0]*ug2 + R[2][0]*ug3;
        (*vector)[ind2] = R[0][1]*ug1 + R[1][1]*ug2 + R[2][1]*ug3;
        (*vector)[ind3] = R[0][2]*ug1 + R[1][2]*ug2 + R[2][2]*ug3;

        //                               |
        //This one gives correct results V
//        (*vector)[ind1] = R[0][0]*ug1 + R[0][1]*ug2 + R[0][2]*ug3;
//        (*vector)[ind2] = R[1][0]*ug1 + R[1][1]*ug2 + R[1][2]*ug3;
//        (*vector)[ind3] = R[2][0]*ug1 + R[2][1]*ug2 + R[2][2]*ug3;
    }


    return 0;
}


int RCoordinateTransformation::getRotationMatrix(RNode *node1, RNode *node2, std::vector<double>& vAxis, QVector< QVector<double>> *R)
{
    // Compute length and cosines
    double dx, dy, dz, L;
    computeLengthAndCosinesWithNodes(node1, node2, dx, dy, dz, L);

    if (L == 0.0) {
        qCritical() << "Zero length in "<<__FUNCTION__;
        return -1;
    }

    // calculate the element local x axis components (direction cosines)
    // wrt to the global coordinates

    std::vector<double> xAxis{dx/L,dy/L, dz/L};

    (*R)[0][0] = xAxis[0];
    (*R)[0][1] = xAxis[1];
    (*R)[0][2] = xAxis[2];


    // Compute y = v cross x
    std::vector<double> yAxis(3);
    yAxis[0] = vAxis[1]*xAxis[2] - vAxis[2]*xAxis[1];
    yAxis[1] = vAxis[2]*xAxis[0] - vAxis[0]*xAxis[2];
    yAxis[2] = vAxis[0]*xAxis[1] - vAxis[1]*xAxis[0];

    double ynorm = yAxis[0]*yAxis[0]+yAxis[1]*yAxis[1]+yAxis[2]*yAxis[2];

    if (ynorm == 0) {
        qCritical() << "Vector v that defines plane xz is parallel to x axis in "<<__FUNCTION__;
        return -1;
    }

    yAxis[0] /= ynorm;
    yAxis[1] /= ynorm;
    yAxis[2] /= ynorm;

    // Compute z = x cross y
    std::vector<double> zAxis(3);

    zAxis[0] = xAxis[1]*yAxis[2] - xAxis[2]*yAxis[1];
    zAxis[1] = xAxis[2]*yAxis[0] - xAxis[0]*yAxis[2];
    zAxis[2] = xAxis[0]*yAxis[1] - xAxis[1]*yAxis[0];

    // Fill in transformation matrix
    (*R)[1][0] = yAxis[0];
    (*R)[1][1] = yAxis[1];
    (*R)[1][2] = yAxis[2];

    (*R)[2][0] = zAxis[0];
    (*R)[2][1] = zAxis[1];
    (*R)[2][2] = zAxis[2];

    return 0;
}



