#ifndef RCoordinateTransformation_H
#define RCoordinateTransformation_H
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

// Class containing methods for coordinate and geometry transformations

#include "RMethod.h"

#ifdef OSG
#include <osg/Vec3>
#endif

class RPoint;
class RNode;
class RDomain;

class RCoordinateTransformation : public RMethod
{

public:
    RCoordinateTransformation(RDomain *domain);
    ~RCoordinateTransformation();

    // Establish rotation matrix
    int getRotationMatrixWithPoints(RPoint *point1, RPoint *point2, double orientation, QVector< QVector<double> > *R);
    int getRotationMatrixWithNodes(RNode *node1, RNode *node2, double orientation, QVector< QVector<double> > *R);
    int computeLengthAndCosinesWithPoints(RPoint *point1, RPoint *point2, double &dx, double &dy, double &dz, double &L);
    int computeLengthAndCosinesWithNodes(RNode *node1, RNode *node2, double &dx, double &dy, double &dz, double &L);
    int getRotationMatrixWithCosines(double dx, double dy, double dz, double L, double orientation, QVector< QVector<double> > *R);
    int getRotationMatrixWithCosinesTwo(double dx, double dy, double dz, double L, double orientation, QVector< QVector<double> > *R);

    // Carry out transformations
    int transformVectorToGlobal(RNode *node1, RNode *node2, double orientation, QVector<double> *vector);
    int transformVectorToGlobalII(RNode *node1, RNode *node2, std::vector<double> orientationVec, QVector<double> *vector);

#ifdef OSG
    int transformVectorToGlobal(osg::Vec3 *Vec1, osg::Vec3 *Vec2, double orientation, osg::Vec3 *result);
#endif

    // This function gets a vector in the xz plane of a frame element - used in OpenSees
    int getVectorInXZPlane(RNode *node1, RNode *node2, std::vector<double>* vector, double orientation);

    int transformMatrixToGlobal(RNode *node1, RNode *node2, double orientation, QVector< QVector<double> > *matrix);
    int transformVectorToLocal(RNode *node1, RNode *node2, double orientation, QVector<double> *vector);
    int transformVectorToLocalII(RNode *node1, RNode *node2, std::vector<double> orientationVec, QVector<double> *vector);

    int getRotationMatrix(RNode *node1, RNode *node2, std::vector<double>& vAxis, QVector< QVector<double>> *R);

    // Services for 4-node elements
    double getProjectedCoordinatesWithFourNodes(RNode *node1, RNode *node2, RNode *node3, RNode *node4, QVector<double> *x, QVector<double> *y);

    double getAreaWithFourNodes(RNode *node1, RNode *node2, RNode *node3, RNode *node4);
    double getProjectedCoordinatesWithNineNodes(RNode *node1, RNode *node2, RNode *node3, RNode *node4, RNode *node5,RNode *node6,RNode *node7,RNode *node8,RNode *node9, QVector<double> *x, QVector<double> *y); //this is called to convert x and y in "projected" coordinates SG add

private:


};

#endif
