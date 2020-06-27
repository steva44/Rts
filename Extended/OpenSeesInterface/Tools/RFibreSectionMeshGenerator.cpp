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

#include "RFibreSectionMeshGenerator.h"
#include "RNode.h"
#include "RPoint.h"
#include "RElement.h"
#include "RParameter.h"
#include "RMathUtils.h"
#include "RMaterialKnowledge.h"
#include "RConcreteMaterialKnowledge.h"
#include "RFiber.h"
#include "RUniaxialFiber3d.h"
#include "RUniaxialMaterial.h"
#include "RFibreSectionKnowledge.h"
#include "RQuadRCSectionDamageRegion.h"
#include "RQuadSectionDamageRegion.h"
#include "R2DPoint.h"
#include "RSection.h"

#include <math.h>

RFibreSectionMeshGenerator::RFibreSectionMeshGenerator(RDomain *theDomain)
    : RMethod(theDomain, nullptr)
{
    fiberCounter = 0;
    matCounter  = 0;

    nodalDebug = false;

    tol = 1.0e-6;

    theFiberType = "RUniaxialFiber3d";
}


RFibreSectionMeshGenerator::~RFibreSectionMeshGenerator()
{

}


int RFibreSectionMeshGenerator::meshRCQuadrilateralSection(const R2DPoint &point1,
                                                           const R2DPoint &point2,
                                                           const R2DPoint &point3,
                                                           const R2DPoint &point4,
                                                           const QString& parent,
                                                           const int numCoverFibresAlong12,
                                                           const int numCoverFibresAlong23,
                                                           const int numCoreFibresAlong12,
                                                           const int numCoreFibresAlong23,
                                                           RParameter* offseta,
                                                           RQuadRCSectionDamageRegion* theSegment,
                                                           RConcreteMaterialKnowledge *theCoreMaterialKnowledge,
                                                           RConcreteMaterialKnowledge *theCovereMaterialKnowledge)
{

    //This algorithm works for all arbitrary quadrilaterals that are hollow, the points are 2D points in the y-z plane.  The x axis being along the element
    /*
     *         ^y
     *         |
     *    z    |
     *   < ____|
     *
     * 4^------------------^3
     *  |  \     I      /  |
     *  |   8^________^7   |
     *  |    | \ I  / |    |
     *  | IV |  \  /  | II |
     *  |    |IV 9  II|    |
     *  |    | /III \ |    |
     *  |   5^________^6   |
     *  |  /    III      \ |
     * 1^------------------^2
     *
     * It splits the hollow section into 4 quadrilaterals at the corner points and then each quadrilateral is treated separately as a patch
     *
     *
     * For example, one quad would be
     *    5^________^6
     *   /            \
     * 1^--------------^2
     *
     */

    //Check the number of elements coming in
    if(numCoverFibresAlong12 < 0 || numCoverFibresAlong23 < 0 || numCoreFibresAlong12 < 0 || numCoreFibresAlong23 < 0)
    {
        qCritical()<<"The number of fibres along each side of the quadrilaterial must be greater than or equal to zero in "<<__FUNCTION__;
        return -1;
    }

    //This function calculates the 2-D distance between two points
    auto distanceLambda=[](const R2DPoint& point1,const R2DPoint& point2)
    {
        auto ycoord1 = point1.getYCoordinateValue();
        auto ycoord2 = point2.getYCoordinateValue();
        auto zcoord1 = point1.getZCoordinateValue();
        auto zcoord2 = point2.getZCoordinateValue();

        auto dy = ycoord2-ycoord1;
        auto dz = zcoord2-zcoord1;

        return sqrt(dy*dy + dz*dz);
    };


    //Get the lengths and test to see if any lengths are zero
    auto L12 = distanceLambda(point1,point2);
    auto L23 = distanceLambda(point2,point3);
    auto L41 = distanceLambda(point1,point4);
    auto L34 = distanceLambda(point3,point4);

    if (L12<=0.0 || L23<=0.0 || L41<=0.0 || L34<=0.0 ) {
        qCritical() << "Coinciding points were given to the mesh generator in "<<__FUNCTION__;
        return -1;
    }


    //For point 5 calculate the intersection of the offset vectors 1-2 and 4-1
    R2DPoint point5;
    //For point 6 calculate the intersection of the offset vectors 1-2 and 2-3
    R2DPoint point6;
    //For point 7 calculate the intersection of the offset vectors 2-3 and 3-4
    R2DPoint point7;
    //For point 8 calculate the intersection of the offset vectors 3-4 and 4-1
    R2DPoint point8;

    std::function<double(void)> point5y = [=](){

        auto offset = offseta->getCurrentValue();

        auto pAy = point1.getYCoordinateValue();   auto pBy = point2.getYCoordinateValue();   auto pDy = point4.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pBz = point2.getZCoordinateValue();   auto pDz = point4.getZCoordinateValue();

        //qDebug()<<"5y"<< (double) (pBy - pAy) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pBz - pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pBy + pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pBz + pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pBy + pAy))) + offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy;

        return (double) (pBy - pAy) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pBz - pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pBy + pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pBz + pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pBy + pAy))) + offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy;
    };

    std::function<double(void)> point5z = [=](){

        auto offset = offseta->getCurrentValue();

        auto pAy = point1.getYCoordinateValue();   auto pBy = point2.getYCoordinateValue();  auto pDy = point4.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pBz = point2.getZCoordinateValue();  auto pDz = point4.getZCoordinateValue();

        // qDebug()<<"5z"<<     (double) (pBz - pAz) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pBz - pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pBy + pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pBz + pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pBy + pAy))) + offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz;

        return (double) (pBz - pAz) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pBz - pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pBy + pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pBz + pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pBy + pAy))) + offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz;
    };

    point5.setYCoordinateValue(point5y);
    point5.setZCoordinateValue(point5z);


    std::function<double(void)> point6y = [=](){

        auto offset = offseta->getCurrentValue();

        auto pAy = point1.getYCoordinateValue();   auto pBy = point2.getYCoordinateValue();    auto pCy = point3.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pBz = point2.getZCoordinateValue();    auto pCz = point3.getZCoordinateValue();

        //qDebug()<<"6y"<< (double) (pBy - pAy) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy)) / ((double) (pBz - pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (double) (-pBy + pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) + (double) (-pBz + pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) - (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (double) (-pBy + pAy))) + offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy;

        return (double) (pBy - pAy) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy)) / ((double) (pBz - pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (double) (-pBy + pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) + (double) (-pBz + pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) - (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (double) (-pBy + pAy))) + offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy;
    };

    std::function<double(void)> point6z = [=](){

        auto offset = offseta->getCurrentValue();

        auto pAy = point1.getYCoordinateValue();   auto pBy = point2.getYCoordinateValue();    auto pCy = point3.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pBz = point2.getZCoordinateValue();    auto pCz = point3.getZCoordinateValue();

        //qDebug()<<"6z"<< (double) (pBz - pAz) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy)) / ((double) (pBz - pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (double) (-pBy + pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) + (double) (-pBz + pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) - (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (double) (-pBy + pAy))) + offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz;

        return (double) (pBz - pAz) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy)) / ((double) (pBz - pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (double) (-pBy + pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) + (double) (-pBz + pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) - (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (double) (-pBy + pAy))) + offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz;
    };

    point6.setYCoordinateValue(point6y);
    point6.setZCoordinateValue(point6z);

    std::function<double(void)> point7y = [=](){

        auto offset = offseta->getCurrentValue();

        auto pCy = point3.getYCoordinateValue();   auto pDy = point4.getYCoordinateValue();    auto pBy = point2.getYCoordinateValue();
        auto pCz = point3.getZCoordinateValue();   auto pDz = point4.getZCoordinateValue();    auto pBz = point2.getZCoordinateValue();

        //qDebug()<<"7y"<< (double) (pCy - pBy) * fabs(((-offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBz + offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy - offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) - (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) + (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy)) / ((double) (pCz - pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (double) (-pCy + pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) + (double) (-pCz + pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) - (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (double) (-pCy + pBy))) + offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy;

        return (double) (pCy - pBy) * fabs(((-offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBz + offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy - offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) - (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) + (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy)) / ((double) (pCz - pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (double) (-pCy + pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) + (double) (-pCz + pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) - (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (double) (-pCy + pBy))) + offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy;

    };

    std::function<double(void)> point7z = [=](){

        auto offset = offseta->getCurrentValue();

        auto pCy = point3.getYCoordinateValue();   auto pDy = point4.getYCoordinateValue();     auto pBy = point2.getYCoordinateValue();
        auto pCz = point3.getZCoordinateValue();   auto pDz = point4.getZCoordinateValue();     auto pBz = point2.getZCoordinateValue();

        //qDebug()<<"7z"<<  (double) (pCz - pBz) * fabs(((-offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBz + offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy - offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) - (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) + (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy)) / ((double) (pCz - pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (double) (-pCy + pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) + (double) (-pCz + pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) - (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (double) (-pCy + pBy))) + offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz;

        return (double) (pCz - pBz) * fabs(((-offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBz + offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy - offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) - (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) + (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy)) / ((double) (pCz - pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (double) (-pCy + pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) + (double) (-pCz + pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) - (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (double) (-pCy + pBy))) + offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz;
    };

    point7.setYCoordinateValue(point7y);
    point7.setZCoordinateValue(point7z);

    std::function<double(void)> point8y = [=](){

        auto offset = offseta->getCurrentValue();

        auto pAy = point1.getYCoordinateValue();   auto pCy = point3.getYCoordinateValue();   auto pDy = point4.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pCz = point3.getZCoordinateValue();   auto pDz = point4.getZCoordinateValue();

        //qDebug()<<"8y"<<  (double) (pDy - pCy) * fabs(((-offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pDz - pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pDy + pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pDz + pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pDy + pCy))) + offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy;

        return (double) (pDy - pCy) * fabs(((-offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pDz - pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pDy + pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pDz + pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pDy + pCy))) + offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy;
    };

    std::function<double(void)> point8z = [=](){

        auto offset = offseta->getCurrentValue();

        auto pAy = point1.getYCoordinateValue();   auto pCy = point3.getYCoordinateValue();   auto pDy = point4.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pCz = point3.getZCoordinateValue();   auto pDz = point4.getZCoordinateValue();

        //qDebug()<<"8z"<<    (double) (pDz - pCz) * fabs(((-offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pDz - pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pDy + pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pDz + pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pDy + pCy))) + offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz;

        return (double) (pDz - pCz) * fabs(((-offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pDz - pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pDy + pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pDz + pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pDy + pCy))) + offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz;
    };

    point8.setYCoordinateValue(point8y);
    point8.setZCoordinateValue(point8z);

    QString coreMaterialType  = theCoreMaterialKnowledge->getTheConcreteMaterialModel();
    QString coverMaterialType = theCovereMaterialKnowledge->getTheConcreteMaterialModel();

    //Now that we have all necessary points, proceed to meshing of each Quad and laying out the necessay fibers
    /*** FIRST MESH THE CORE -- IF THERE IS ONE THAT IS; THIS ALGORITHM CAN BE USED FOR A HOLLOW SECTION AS WELL ***/

    if(numCoreFibresAlong12 != 0 && numCoreFibresAlong23 != 0)
    {

        std::vector<RFiber*> theCoreFibreVector;

        //Reserve some memory for speed
        theCoreFibreVector.reserve(numCoreFibresAlong12*numCoreFibresAlong23);

        auto res = this->meshQuadrilateralFibrePatch(point5,point6,point7,point8,numCoreFibresAlong12,numCoreFibresAlong23,coreMaterialType,parent,&theCoreFibreVector,theCoreMaterialKnowledge);
        if(res != 0)
            return res;

        //This function gives the Y-Coordinate of the intersection point of the two lines made with points 1-2 and points 3-4
        auto intersectionLambdaY=[](R2DPoint& point1,R2DPoint& point2,R2DPoint& point3,R2DPoint& point4)
        {
            auto p1y = point1.getYCoordinateValue();     auto p1z = point1.getZCoordinateValue();
            auto p2y = point2.getYCoordinateValue();     auto p2z = point2.getZCoordinateValue();
            auto p3y = point3.getYCoordinateValue();     auto p3z = point3.getZCoordinateValue();
            auto p4y = point4.getYCoordinateValue();     auto p4z = point4.getZCoordinateValue();

            return  (p2y - p1y) * fabs(((-p1z + p4z) * p3y + (p1y - p4y) * p3z - p1y * p4z + p1z * p4y) / ((p2z - p1z) * p3y + (-p2y + p1y) * p3z + (-p2z + p1z) * p4y - p4z * (-p2y + p1y))) + p1y;
        };

        //This function gives the Z-Coordinate of the intersection point of the two lines made with points 1-2 and points 3-4
        auto intersectionLambdaZ=[](R2DPoint& point1,R2DPoint& point2,R2DPoint& point3,R2DPoint& point4)
        {
            auto p1y = point1.getYCoordinateValue();     auto p1z = point1.getZCoordinateValue();
            auto p2y = point2.getYCoordinateValue();     auto p2z = point2.getZCoordinateValue();
            auto p3y = point3.getYCoordinateValue();     auto p3z = point3.getZCoordinateValue();
            auto p4y = point4.getYCoordinateValue();     auto p4z = point4.getZCoordinateValue();

            return (p2z - p1z) * fabs(((-p1z + p4z) * p3y + (p1y - p4y) * p3z - p1y * p4z + p1z * p4y) / ((p2z - p1z) * p3y + (-p2y + p1y) * p3z + (-p2z + p1z) * p4y - p4z * (-p2y + p1y))) + p1z;
        };

        //Find the intersection of the two lines that connect the four corners diagonally
        R2DPoint point9;
        auto point9y = intersectionLambdaY(point5,point7,point6,point8);
        auto point9z = intersectionLambdaZ(point5,point7,point6,point8);
        point9.setYCoordinateValue(point9y);
        point9.setZCoordinateValue(point9z);

        //function that returns true if a point lies within a triangle or along any edge (2D); returns false otherwise
        //This is used to figure out which quadrant a core fiber belongs in
        auto inTriangle =  [](R2DPoint vertex1, R2DPoint vertex2, R2DPoint vertex3, R2DPoint pointToCheck)
        {
            auto p1y = vertex1.getYCoordinateValue();
            auto p1z = vertex1.getZCoordinateValue();

            auto p2y = vertex2.getYCoordinateValue();
            auto p2z = vertex2.getZCoordinateValue();

            auto p3y = vertex3.getYCoordinateValue();
            auto p3z = vertex3.getZCoordinateValue();

            auto pChecky = pointToCheck.getYCoordinateValue();
            auto pCheckz = pointToCheck.getZCoordinateValue();

            double alpha = ((p2z - p3z)*(pChecky - p3y) + (p3y - p2y)*(pCheckz - p3z)) /
                    ((p2z - p3z)*(p1y - p3y) + (p3y - p2y)*(p1z - p3z));

            double beta = ((p3z - p1z)*(pChecky - p3y) + (p1y - p3y)*(pCheckz - p3z)) /
                    ((p2z - p3z)*(p1y - p3y) + (p3y - p2y)*(p1z - p3z));

            double gamma = 1.0 - alpha - beta;

            //Had to change from zero to epsilon to account for some noise when the point is directly on the line
            if(alpha < -DBL_EPSILON || beta < -DBL_EPSILON || gamma < -DBL_EPSILON)
                return false;
            else
                return true;
        };

        //Now place each fiber into its container
        for(auto&& fibreIt : theCoreFibreVector)
        {
            R2DPoint pointTest;
            pointTest.setYCoordinateValue(fibreIt->getFibreYCoordinate());
            pointTest.setZCoordinateValue(fibreIt->getFibreZCoordinate());

            //For quadrant I --> Triangle point 5,6,9
            if(inTriangle(point5,point6,point9,pointTest))
                theSegment->quadrantI.theCoreFibres.push_back(fibreIt);
            else if(inTriangle(point6,point7,point9,pointTest))  //For quadrant II --> Triangle point 6,7,9
                theSegment->quadrantII.theCoreFibres.push_back(fibreIt);
            else if(inTriangle(point7,point8,point9,pointTest))  //For quadrant III --> Triangle point  7,8,9
                theSegment->quadrantIII.theCoreFibres.push_back(fibreIt);
            else if(inTriangle(point8,point5,point9,pointTest)) //For quadrant IV --> Triangle point
                theSegment->quadrantIV.theCoreFibres.push_back(fibreIt);
            else
                qDebug()<<"Warning, a core fibre was created but it does not appear to be in the core!: "<<__FUNCTION__;
        }

    }

    /*
     * 4^------------------^3
     *  |  \     I      /  |
     *  |   8^________^7   |
     *  |    | \ I  / |    |
     *  | IV |  \  /  | II |
     *  |    |IV 9  II|    |
     *  |    | /III \ |    |
     *  |   5^________^6   |
     *  |  /    III      \ |
     * 1^------------------^2
     *
     */

    //*** NOW MESH THE COVER FIBRES ***
    if(numCoverFibresAlong12 > 0 || numCoverFibresAlong23 > 0)
    {
        //*** NOW MESH THE COVER FIBRES ***
        //first mesh the quad 1-2-6-5
        auto res = this->meshQuadrilateralFibrePatch(point1,point2,point6,point5,numCoverFibresAlong12,numCoverFibresAlong23,coverMaterialType,parent,&theSegment->quadrantIII.theCoverFibres,theCovereMaterialKnowledge);
        if(res != 0)
            return res;

        //then mesh the quad 2-3-7-6
        res = this->meshQuadrilateralFibrePatch(point2,point3,point7,point6,numCoverFibresAlong12,numCoverFibresAlong23,coverMaterialType,parent,&theSegment->quadrantII.theCoverFibres,theCovereMaterialKnowledge);
        if(res != 0)
            return res;

        //then mesh the quad 3-4-8-7
        res = this->meshQuadrilateralFibrePatch(point8,point7,point3,point4,numCoverFibresAlong12,numCoverFibresAlong23,coverMaterialType,parent,&theSegment->quadrantI.theCoverFibres,theCovereMaterialKnowledge);
        if(res != 0)
            return res;

        //finally mesh the quad 4-1-5-8
        res = this->meshQuadrilateralFibrePatch(point4,point1,point5,point8,numCoverFibresAlong12,numCoverFibresAlong23,coverMaterialType,parent,&theSegment->quadrantIV.theCoverFibres,theCovereMaterialKnowledge);
        if(res != 0)
            return res;
    }


    return 0;
}


int RFibreSectionMeshGenerator::meshHomogeneousQuadrilateralSection(const R2DPoint &point1,
                                                                    const R2DPoint &point2,
                                                                    const R2DPoint &point3,
                                                                    const R2DPoint &point4,
                                                                    const int numFibresAlong12,
                                                                    const int numFibresAlong23,
                                                                    const QString& materialModel,
                                                                    const QString& parent,
                                                                    RQuadSectionDamageRegion* theSegment,
                                                                    RMaterialKnowledge *theMaterialKnowledge)
{

    //A rectangular section discretized into quandrants:
    /*
     * 4^------------^3
     *  | \   I    / |
     *  |  \      /  |
     *  |   \    /   |
     *  |    \  /    |
     *  |     \/     |
     *  | IV  /\  II |
     *  |    /  \    |
     *  |   /    \   |
     *  |  /      \  |
     *  | /   III  \ |
     * 1^------------^
     *
    */

    //Check the number of elements coming in
    if(numFibresAlong12 < 0 || numFibresAlong23 < 0)
    {
        qCritical()<<"The number of fibres along each side of the quadrilaterial must be greater than or equal to zero in "<<__FUNCTION__;
        return -1;
    }

    //This function calculates the 2-D distance between two points
    auto distanceLambda=[](const R2DPoint& point1,const R2DPoint& point2)
    {
        auto ycoord1 = point1.getYCoordinateValue();
        auto ycoord2 = point2.getYCoordinateValue();
        auto zcoord1 = point1.getZCoordinateValue();
        auto zcoord2 = point2.getZCoordinateValue();

        auto dy = ycoord2-ycoord1;
        auto dz = zcoord2-zcoord1;

        return sqrt(dy*dy + dz*dz);
    };


    //Get the lengths and test to see if any lengths are zero
    auto L12 = distanceLambda(point1,point2);
    auto L23 = distanceLambda(point2,point3);
    auto L41 = distanceLambda(point1,point4);
    auto L34 = distanceLambda(point3,point4);

    if (L12<=0.0 || L23<=0.0 || L41<=0.0 || L34<=0.0 ) {
        qCritical() << "Coinciding points were given to the mesh generator in "<<__FUNCTION__;
        return -1;
    }


    std::vector<RFiber*> theFibreVector;

    //Reserve some memory for speed
    theFibreVector.reserve(numFibresAlong12*numFibresAlong23);

    auto res = this->meshQuadrilateralFibrePatch(point1,point2,point3,point4,numFibresAlong12,numFibresAlong23,materialModel,parent,&theFibreVector,theMaterialKnowledge);
    if(res != 0)
        return res;

    //This function gives the Y-Coordinate of the intersection point of the two lines made with points 1-2 and points 3-4
    auto intersectionLambdaY=[](const R2DPoint& point1,const R2DPoint& point2,const R2DPoint& point3,const R2DPoint& point4)
    {
        auto p1y = point1.getYCoordinateValue();     auto p1z = point1.getZCoordinateValue();
        auto p2y = point2.getYCoordinateValue();     auto p2z = point2.getZCoordinateValue();
        auto p3y = point3.getYCoordinateValue();     auto p3z = point3.getZCoordinateValue();
        auto p4y = point4.getYCoordinateValue();     auto p4z = point4.getZCoordinateValue();

        return  (p2y - p1y) * fabs(((-p1z + p4z) * p3y + (p1y - p4y) * p3z - p1y * p4z + p1z * p4y) / ((p2z - p1z) * p3y + (-p2y + p1y) * p3z + (-p2z + p1z) * p4y - p4z * (-p2y + p1y))) + p1y;
    };

    //This function gives the Z-Coordinate of the intersection point of the two lines made with points 1-2 and points 3-4
    auto intersectionLambdaZ=[](const R2DPoint& point1,const R2DPoint& point2,const R2DPoint& point3,const R2DPoint& point4)
    {
        auto p1y = point1.getYCoordinateValue();     auto p1z = point1.getZCoordinateValue();
        auto p2y = point2.getYCoordinateValue();     auto p2z = point2.getZCoordinateValue();
        auto p3y = point3.getYCoordinateValue();     auto p3z = point3.getZCoordinateValue();
        auto p4y = point4.getYCoordinateValue();     auto p4z = point4.getZCoordinateValue();

        return (p2z - p1z) * fabs(((-p1z + p4z) * p3y + (p1y - p4y) * p3z - p1y * p4z + p1z * p4y) / ((p2z - p1z) * p3y + (-p2y + p1y) * p3z + (-p2z + p1z) * p4y - p4z * (-p2y + p1y))) + p1z;
    };

    //Find the intersection of the two lines that connect the four corners diagonally
    R2DPoint point9;
    auto point9y = intersectionLambdaY(point1,point2,point3,point4);
    auto point9z = intersectionLambdaZ(point1,point2,point3,point4);
    point9.setYCoordinateValue(point9y);
    point9.setZCoordinateValue(point9z);

    //function that returns true if a point lies within a triangle or along any edge (2D); returns false otherwise
    //This is used to figure out which quadrant a core fiber belongs in
    auto inTriangle =  [](R2DPoint vertex1, R2DPoint vertex2, R2DPoint vertex3, R2DPoint pointToCheck)
    {
        auto p1y = vertex1.getYCoordinateValue();
        auto p1z = vertex1.getZCoordinateValue();

        auto p2y = vertex2.getYCoordinateValue();
        auto p2z = vertex2.getZCoordinateValue();

        auto p3y = vertex3.getYCoordinateValue();
        auto p3z = vertex3.getZCoordinateValue();

        auto pChecky = pointToCheck.getYCoordinateValue();
        auto pCheckz = pointToCheck.getZCoordinateValue();

        double alpha = ((p2z - p3z)*(pChecky - p3y) + (p3y - p2y)*(pCheckz - p3z)) /
                ((p2z - p3z)*(p1y - p3y) + (p3y - p2y)*(p1z - p3z));

        double beta = ((p3z - p1z)*(pChecky - p3y) + (p1y - p3y)*(pCheckz - p3z)) /
                ((p2z - p3z)*(p1y - p3y) + (p3y - p2y)*(p1z - p3z));

        double gamma = 1.0 - alpha - beta;

        //Had to change from zero to epsilon to account for some noise when the point is directly on the line
        if(alpha < -DBL_EPSILON || beta < -DBL_EPSILON || gamma < -DBL_EPSILON)
            return false;
        else
            return true;
    };

    /*
     * 4^------------------^3
     *  |  \     I      /  |
     *  |   8^________^7   |
     *  |    | \ I  / |    |
     *  | IV |  \  /  | II |
     *  |    |IV 9  II|    |
     *  |    | /III \ |    |
     *  |   5^________^6   |
     *  |  /    III      \ |
     * 1^------------------^2
     *
     */

    //Now place each fiber into its container
    for(auto&& fibreIt : theFibreVector)
    {
        R2DPoint pointTest;
        pointTest.setYCoordinateValue(fibreIt->getFibreYCoordinate());
        pointTest.setZCoordinateValue(fibreIt->getFibreZCoordinate());

        //For quadrant I --> Triangle point 5,6,9
        if(inTriangle(point1,point2,point9,pointTest))
            theSegment->quadrantI.getAllFibres().push_back(fibreIt);
        else if(inTriangle(point2,point3,point9,pointTest))  //For quadrant II --> Triangle point 6,7,9
            theSegment->quadrantII.getAllFibres().push_back(fibreIt);
        else if(inTriangle(point3,point4,point9,pointTest))  //For quadrant III --> Triangle point  7,8,9
            theSegment->quadrantIII.getAllFibres().push_back(fibreIt);
        else if(inTriangle(point4,point1,point9,pointTest)) //For quadrant IV --> Triangle point
            theSegment->quadrantIV.getAllFibres().push_back(fibreIt);
        else
            qDebug()<<"Warning, a core fibre was created but it does not appear to be in the core!: "<<__FUNCTION__;
    }

    return 0;
}


int RFibreSectionMeshGenerator::meshQuadrilateralRebarLayer(const R2DPoint& point1,
                                                            const R2DPoint& point2,
                                                            const R2DPoint& point3,
                                                            const R2DPoint& point4,
                                                            const int numRebarAlong12,
                                                            const int numRebarAlong23,
                                                            const int numRebarAlong34,
                                                            const int numRebarAlong41,
                                                            const double rebarArea,
                                                            const QString materialType,
                                                            const QString& parent,
                                                            RParameter*  offseta,
                                                            RParameter*  offsetb,
                                                            RParameter*  offsetc,
                                                            RParameter*  offsetd,
                                                            RQuadRCSectionDamageRegion* theSegment,
                                                            RMaterialKnowledge *theMaterialKnowledge)
{

    //This algorithm works for all arbitrary quadrilaterals, the points are 2D points in the y-z plane.  The x axis being along the element
    //This uses lambda functions for the fibre coordinates -- so that meshing is only required once, after that the values are always updated
    /*
     *
     *         ^y
     *         |
     *    z    |
     *   < ____|
     *
     *
     *
     * 4^------------------^3
     *  |         c        |
     *  |   8^________^7   |
     *  |    |        |    |
     *  | d  |        | b  |
     *  |    |  core  |    |
     *  |    |        |    |
     *  |   5^________^6   |
     *  |         a        |
     * 1^------------------^2
     *
     *  It meshes the rebar along each offset from the edge of the element
     *  Each rebar is one fiber
     *
     *  a,b,c,d are offsets from the edge lines of the points
     *
     */

    //Check the number of elements coming in
    if(numRebarAlong12 < 0 || numRebarAlong23 < 0 || numRebarAlong34 < 0 || numRebarAlong41 < 0)
    {
        qCritical()<<"The number of fibres along each side of the Quadrilateral in RFibreSectionMeshGenerator is inconsistent";
        return -1;
    }

    //This function calculates the 2-D distance between two points
    auto distanceLambda=[](const R2DPoint& point1, const R2DPoint& point2)
    {
        auto xcoord1 = point1.getYCoordinateValue();
        auto xcoord2 = point2.getYCoordinateValue();
        auto ycoord1 = point1.getZCoordinateValue();
        auto ycoord2 = point2.getZCoordinateValue();

        auto dy = xcoord2-xcoord1;
        auto dz = ycoord2-ycoord1;

        return sqrt(dy*dy + dz*dz);
    };

    //Get the lengths and test to see if any lengths are zero
    auto L12 = distanceLambda(point1,point2);
    auto L23 = distanceLambda(point2,point3);
    auto L41 = distanceLambda(point1,point4);
    auto L34 = distanceLambda(point3,point4);

    if (L12<=0.0 || L23<=0.0 || L41<=0.0 || L34<=0.0 ) {
        qCritical() << "Coinciding points were given to the mesh generator in the component called" << theMaterialKnowledge->getTheComponentName();
        return -1;
    }

    //For point 5 calculate the intersection of the offset vectors 1-2 and 4-1
    R2DPoint point5;
    //For point 6 calculate the intersection of the offset vectors 1-2 and 2-3
    R2DPoint point6;
    //For point 7 calculate the intersection of the offset vectors 2-3 and 3-4
    R2DPoint point7;
    //For point 8 calculate the intersection of the offset vectors 3-4 and 4-1
    R2DPoint point8;

    //TODO:: Changethat the offsets are properly represented for each edge, not just a general offset

    std::function<double(void)> point5y = [=](){

        auto offset = offseta->getCurrentValue();
        auto pAy = point1.getYCoordinateValue();   auto pBy = point2.getYCoordinateValue();   auto pDy = point4.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pBz = point2.getZCoordinateValue();   auto pDz = point4.getZCoordinateValue();

        return (double) (pBy - pAy) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pBz - pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pBy + pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pBz + pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pBy + pAy))) + offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy;
    };

    std::function<double(void)> point5z = [=](){

        auto offset = offseta->getCurrentValue();
        auto pAy = point1.getYCoordinateValue();   auto pBy = point2.getYCoordinateValue();  auto pDy = point4.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pBz = point2.getZCoordinateValue();  auto pDz = point4.getZCoordinateValue();

        return (double) (pBz - pAz) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pBz - pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pBy + pAy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pBz + pAz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pBy + pAy))) + offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz;
    };

    point5.setYCoordinateValue(point5y);
    point5.setZCoordinateValue(point5z);


    std::function<double(void)> point6y = [=](){

        auto offset = offseta->getCurrentValue();
        auto pAy = point1.getYCoordinateValue();   auto pBy = point2.getYCoordinateValue();    auto pCy = point3.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pBz = point2.getZCoordinateValue();    auto pCz = point3.getZCoordinateValue();

        return (double) (pBy - pAy) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy)) / ((double) (pBz - pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (double) (-pBy + pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) + (double) (-pBz + pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) - (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (double) (-pBy + pAy))) + offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy;
    };

    std::function<double(void)> point6z = [=](){

        auto offset = offseta->getCurrentValue();
        auto pAy = point1.getYCoordinateValue();   auto pBy = point2.getYCoordinateValue();    auto pCy = point3.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pBz = point2.getZCoordinateValue();    auto pCz = point3.getZCoordinateValue();

        return (double) (pBz - pAz) * fabs(((-offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) - (double) pAz + offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy - offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) - (offset * (double) (-pBz + pAz) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) + (offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy)) / ((double) (pBz - pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCy) + (double) (-pBy + pAy) * (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pCz) + (double) (-pBz + pAz) * (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) - (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (double) (-pBy + pAy))) + offset * (double) (pBy - pAy) / (double) (abs(-pBy + pAy) >= abs(-pBz + pAz) ? abs(-pBy + pAy) : abs(-pBz + pAz)) + (double) pAz;
    };

    point6.setYCoordinateValue(point6y);
    point6.setZCoordinateValue(point6z);

    std::function<double(void)> point7y = [=](){

        auto offset = offseta->getCurrentValue();
        auto pCy = point3.getYCoordinateValue();   auto pDy = point4.getYCoordinateValue();    auto pBy = point2.getYCoordinateValue();
        auto pCz = point3.getZCoordinateValue();   auto pDz = point4.getZCoordinateValue();    auto pBz = point2.getZCoordinateValue();

        return (double) (pCy - pBy) * fabs(((-offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBz + offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy - offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) - (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) + (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy)) / ((double) (pCz - pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (double) (-pCy + pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) + (double) (-pCz + pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) - (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (double) (-pCy + pBy))) + offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy;

    };

    std::function<double(void)> point7z = [=](){

        auto offset = offseta->getCurrentValue();
        auto pCy = point3.getYCoordinateValue();   auto pDy = point4.getYCoordinateValue();     auto pBy = point2.getYCoordinateValue();
        auto pCz = point3.getZCoordinateValue();   auto pDz = point4.getZCoordinateValue();     auto pBz = point2.getZCoordinateValue();

        return (double) (pCz - pBz) * fabs(((-offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) - (double) pBz + offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy - offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) - (offset * (double) (-pCz + pBz) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) + (offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy)) / ((double) (pCz - pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDy) + (double) (-pCy + pBy) * (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pDz) + (double) (-pCz + pBz) * (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) - (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (double) (-pCy + pBy))) + offset * (double) (pCy - pBy) / (double) (abs(-pCy + pBy) >= abs(-pCz + pBz) ? abs(-pCy + pBy) : abs(-pCz + pBz)) + (double) pBz;
    };

    point7.setYCoordinateValue(point7y);
    point7.setZCoordinateValue(point7z);

    std::function<double(void)> point8y = [=](){

        auto offset = offseta->getCurrentValue();
        auto pAy = point1.getYCoordinateValue();   auto pCy = point3.getYCoordinateValue();   auto pDy = point4.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pCz = point3.getZCoordinateValue();   auto pDz = point4.getZCoordinateValue();

        return (double) (pDy - pCy) * fabs(((-offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pDz - pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pDy + pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pDz + pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pDy + pCy))) + offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy;
    };

    std::function<double(void)> point8z = [=](){

        auto offset = offseta->getCurrentValue();
        auto pAy = point1.getYCoordinateValue();   auto pCy = point3.getYCoordinateValue();   auto pDy = point4.getYCoordinateValue();
        auto pAz = point1.getZCoordinateValue();   auto pCz = point3.getZCoordinateValue();   auto pDz = point4.getZCoordinateValue();

        return (double) (pDz - pCz) * fabs(((-offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) - (double) pCz + offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy - offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) - (double) pDy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) - (offset * (double) (-pDz + pCz) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) + (offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy)) / ((double) (pDz - pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAy) + (double) (-pDy + pCy) * (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pAz) + (double) (-pDz + pCz) * (offset * (double) (-pAz + pDz) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDy) - (offset * (double) (pAy - pDy) / (double) (abs(pAy - pDy) >= abs(pAz - pDz) ? abs(pAy - pDy) : abs(pAz - pDz)) + (double) pDz) * (double) (-pDy + pCy))) + offset * (double) (pDy - pCy) / (double) (abs(-pDy + pCy) >= abs(-pDz + pCz) ? abs(-pDy + pCy) : abs(-pDz + pCz)) + (double) pCz;
    };

    point8.setYCoordinateValue(point8y);
    point8.setZCoordinateValue(point8z);

    //Now that we have all necessary points, proceed to meshing of each rebar layer and laying out the necessay fibers

    /*
     * 4^------------------^3
     *  |  \     I      /  |
     *  |   8^________^7   |
     *  |    | \ I  / |    |
     *  | IV |  \  /  | II |
     *  |    |IV 9  II|    |
     *  |    | /III \ |    |
     *  |   5^________^6   |
     *  |  /    III      \ |
     * 1^------------------^2
     *
     */

    if (numRebarAlong12 != 0)
    {
        auto res = this->meshLinearRebarLayer(point5,point6,materialType,parent,numRebarAlong12,rebarArea,&theSegment->quadrantIII.theReinforcementFibres,theMaterialKnowledge);
        if(res != 0)
            return res;
    }

    if (numRebarAlong23 != 0)
    {
        //Need to skip the first and last one so that we do not double up on rebar - hence the meshLinearRebarLayerII
        auto res = this->meshLinearRebarLayerII(point6,point7,materialType,parent,numRebarAlong23,rebarArea,&theSegment->quadrantII.theReinforcementFibres,theMaterialKnowledge);
        if(res != 0)
            return res;
    }

    if (numRebarAlong34 != 0)
    {
        auto res = this->meshLinearRebarLayer(point7,point8,materialType,parent,numRebarAlong34,rebarArea,&theSegment->quadrantI.theReinforcementFibres,theMaterialKnowledge);
        if(res != 0)
            return res;
    }

    if (numRebarAlong41 != 0)
    {
        //Need to skip the first and last one so that we do not double up on rebar - hence the meshLinearRebarLayerII
        auto res = this->meshLinearRebarLayerII(point8,point5,materialType,parent,numRebarAlong41,rebarArea,&theSegment->quadrantIV.theReinforcementFibres,theMaterialKnowledge);
        if(res != 0)
            return res;
    }


    return 0;
}


int RFibreSectionMeshGenerator::meshRCCircularSection(const double outerRadius,
                                                      const double innerRadius,
                                                      const int numFibresAlongR,
                                                      const int numFibresAlongC,
                                                      const QString coreMaterialType,
                                                      const QString coverMaterialType,
                                                      RQuadRCSectionDamageRegion* theSegment,
                                                      RMaterialKnowledge *theMaterialKnowledge)
{
    // P(x,y) = (Rcos(theta),Rsin(theta)) -- start from the RH quadrant x-axis going counter-clockwise

    // This is like a pizza; evey cut has two halves
    auto fibresAlongC = numFibresAlongC *2;


    // We want the centroid of each circle section -- the centroid is halfway across the arclength and half way across the radius...
    // First sweep through the number of 'circles' across R
    for(int j =0; j<numFibresAlongR; ++j)
    {
        //Then sweep along each circle
        for(int i=0; i<fibresAlongC; ++i)
        {

            std::function<double(void)> yCoordinate = [innerRadius,outerRadius,numFibresAlongR,fibresAlongC,i,j](void){

                auto radius = innerRadius+0.5*(outerRadius-innerRadius)*(2.0*j+1.0)/numFibresAlongR;
                auto theta =  PI*(2.0*i+1.0)/fibresAlongC;

                return radius*cos(theta);
            };

            std::function<double(void)> zCoordinate = [innerRadius,outerRadius,numFibresAlongR,fibresAlongC,i,j](void){

                auto radius = innerRadius+0.5*(outerRadius-innerRadius)*(2.0*j+1.0)/numFibresAlongR;
                auto theta =  PI*(2.0*i+1.0)/fibresAlongC;

                return radius*sin(theta);
            };

            std::function<double(void)> AreaLambda = [innerRadius,outerRadius,fibresAlongC,numFibresAlongR,j](void){

                auto r2 = innerRadius + j*(outerRadius-innerRadius)/numFibresAlongR;
                auto r1 = innerRadius + (j+1.0)*(outerRadius-innerRadius)/numFibresAlongR;

                auto theta1 = 2*PI*r1/fibresAlongC;
                auto theta2 = 2*PI*r2/fibresAlongC;

                return 0.5*(theta1*r1-theta2*r2);
            };

            // qDebug()<<"The y coordinate of fiber "<<i<<" is "<<yCoordinate();
            // qDebug()<<"The z coordinate of fiber "<<i<<" is "<<zCoordinate();
            // qDebug()<<"The area of fiber "<<i<<" is "<<AreaLambda();

            // To do: Create the 1D Material
            // auto materialName = materialType + QString("%1%2ofSection%3ofElement%4").arg(i).arg(j).arg(s).arg(it->objectName());
            // theDomain->createObject(objectName() + materialName, materialType);
            // auto the1DMaterial = qobject_cast<R1DMaterial *>(theDomain->getLastAddedObject());
            // //theMaterialList->append(the1DMaterial);

            // the1DMaterial->setArea(AreaLambda);
            // the1DMaterial->setFibreCentroidYCoordinate(yCoordinate);
            // the1DMaterial->setFibreCentroidZCoordinate(zCoordinate);

        }
    }

    return 0;
}


int RFibreSectionMeshGenerator::meshQuadrilateralFibrePatch(const R2DPoint& point1,
                                                            const R2DPoint& point2,
                                                            const R2DPoint& point3,
                                                            const R2DPoint& point4,
                                                            const int numFibresAlong12,
                                                            const int numFibresAlong23,
                                                            const QString& materialType,
                                                            const QString& parent,
                                                            std::vector<RFiber*> *theFibreVector,
                                                            RMaterialKnowledge *theMaterialKnowledge)
{

    //This algorithm works for all arbitrary quadrilaterals, the points are 2D points in the y-z plane.  The x axis being along the element
    /*
     * 4*-------*3
     *  |       |            ^y
     *  |       |            |
     *  |       |       z    |
     * 1*-------*2     < ____|
     *
     *  Points should be given counter-clockwise from the bottom left corner
     */

    auto distanceLambda=[](const R2DPoint& point1,const R2DPoint& point2)
    {
        auto xcoord1 = point1.getYCoordinateValue();
        auto xcoord2 = point2.getYCoordinateValue();
        auto ycoord1 = point1.getZCoordinateValue();
        auto ycoord2 = point2.getZCoordinateValue();

        auto dy = xcoord2-xcoord1;
        auto dz = ycoord2-ycoord1;

        return sqrt(dy*dy + dz*dz);
    };


    //Check the number of elements coming in
    if(numFibresAlong12 <= 0 || numFibresAlong23 <= 0)
    {
        qCritical()<<"The number of fibres must be greater than or equal to zero in"<<__FUNCTION__;
        return -1;
    }

    auto distance12 = distanceLambda(point1,point2);
    auto distance23 = distanceLambda(point2,point3);
    auto distance14 = distanceLambda(point1,point4);
    auto distance34 = distanceLambda(point3,point4);


    if (distance12<=0.0 || distance23<=0.0 || distance14<=0.0 || distance34<=0.0 ) {
        qCritical() << "Coinciding points were given to the mesh generator in "<<__FUNCTION__;
        return -1;
    }


    // INITIAL DECLARATIONS BEFORE LOOP
    int ni = numFibresAlong12;
    int nj = numFibresAlong23;

    // qDebug()<<"The number of elements along 12"<<numElementsAlong12;
    // qDebug()<<"The number of elements along 23"<<numElementsAlong23;
    // qDebug()<<"Distance 12"<<distance12;
    // qDebug()<<"Distance 23"<<distance23;

    // **** LAY OUT THE FIBERS AND ASSIGN COORDINATES *****
    for (int i=0; i<ni; ++i) {
        for (int j=0 ; j<nj; ++j) {

            // Centroid Y COORDINATE
            std::function<double(void)> yCoordinatelambda = [ni, nj, i, j, point1, point2, point3, point4]
            {
                auto n1Y = point1.getYCoordinateValue();
                auto n2Y = point2.getYCoordinateValue();
                auto n3Y = point3.getYCoordinateValue();
                auto n4Y = point4.getYCoordinateValue();

                //                qDebug()<<"yCoordinatelambda"<<"of quadrant "<<creatorName<<"of node "<<i<<j<<(double)((((4 * n1Y - 4 * n2Y + 4 * n3Y - 4 * n4Y) * j + (-4 * nj + 2) * n1Y + (4 * nj - 2) * n2Y + 2 * n3Y - 2 * n4Y) * i +
                //                                                 ((-4 * ni + 2) * n1Y - 2 * n2Y + (4 * ni - 2) * n4Y + 2 * n3Y) * j + ((4 * nj - 2) * ni - 2 * nj + 1) * n1Y + (2 * nj - 1) * n2Y +
                //                                                 (2 * ni - 1) * n4Y + n3Y) / ni / nj) / 0.4e1;

                return (double) ((((4.0 * n1Y - 4.0 * n2Y + 4.0 * n3Y - 4.0 * n4Y) * j + (-4.0 * nj + 2.0) * n1Y + (4.0 * nj - 2.0) * n2Y + 2.0 * n3Y - 2.0 * n4Y) * i +
                                  ((-4.0 * ni + 2.0) * n1Y - 2.0 * n2Y + (4.0 * ni - 2.0) * n4Y + 2.0 * n3Y) * j + ((4.0 * nj - 2.0) * ni - 2.0 * nj + 1.0) * n1Y + (2.0 * nj - 1.0) * n2Y +
                                  (2.0 * ni - 1.0) * n4Y + n3Y) / ni / nj) / 4.0;


            };

            // Centroid Z COORDINATE
            std::function<double(void)> zCoordinatelambda = [ni, nj, i, j, point1, point2, point3, point4]
            {
                auto n1Z = point1.getZCoordinateValue();
                auto n2Z = point2.getZCoordinateValue();
                auto n3Z = point3.getZCoordinateValue();
                auto n4Z = point4.getZCoordinateValue();


                //                qDebug()<<"zCoordinatelambda"<<"of quadrant "<<creatorName<<"of node "<<i<<j<<(double) ((((4 * n1Z - 4 * n2Z + 4 * n3Z - 4 * n4Z) * j + (-4 * nj + 2) * n1Z + (4 * nj - 2) * n2Z + 2 * n3Z - 2 * n4Z) * i +
                //                                                                            ((-4 * ni + 2) * n1Z - 2 * n2Z + (4 * ni - 2) * n4Z + 2 * n3Z) * j + ((4 * nj - 2) * ni - 2 * nj + 1) * n1Z + (2 * nj - 1) * n2Z +
                //                                                                            (2 * ni - 1) * n4Z + n3Z) / ni / nj) / 0.4e1;


                return (double) ((((4.0 * n1Z - 4.0 * n2Z + 4.0 * n3Z - 4.0 * n4Z) * j + (-4.0 * nj + 2.0) * n1Z + (4.0 * nj - 2.0) * n2Z + 2.0 * n3Z - 2.0 * n4Z) * i +
                                  ((-4.0 * ni + 2.0) * n1Z - 2.0 * n2Z + (4.0 * ni - 2.0) * n4Z + 2.0 * n3Z) * j + ((4.0 * nj - 2.0) * ni - 2.0 * nj + 1.0) * n1Z + (2.0 * nj - 1.0) * n2Z +
                                  (2.0 * ni - 1.0) * n4Z + n3Z) / ni / nj) / 0.4e1;

            };

            // AREA
            std::function<double(void)> arealambda = [ni, nj, i, j, point1, point2, point3, point4]
            {
                auto n1Y = point1.getYCoordinateValue();   auto n1Z = point1.getZCoordinateValue();
                auto n2Y = point2.getYCoordinateValue();   auto n2Z = point2.getZCoordinateValue();
                auto n3Y = point3.getYCoordinateValue();   auto n3Z = point3.getZCoordinateValue();
                auto n4Y = point4.getYCoordinateValue();   auto n4Z = point4.getZCoordinateValue();

                auto area1 = 0.5 * (double) fabs(((((n4Z - n2Z) * n1Y + (-n4Y + n2Y) * n1Z + n4Y * n2Z - n4Z * n2Y) * ni + (i + 1.0) * ((n3Z - n4Z) * n1Y + (n4Y - n3Y) * n1Z + (-n3Z + n4Z) * n2Y - n2Z * (n4Y - n3Y))) * nj + ni * ((-n3Z + n2Z) * n1Y + (-n2Y + n3Y) * n1Z + n4Y * n3Z - n4Y * n2Z + n4Z * n2Y - n4Z * n3Y) * j) * (1.0/(double)(ni*ni*nj*nj)));
                auto area2 = 0.5 * (double) fabs(((((n4Z - n2Z) * n1Y + (-n4Y + n2Y) * n1Z + n4Y * n2Z - n4Z * n2Y) * nj + (j + 1.0) * ((-n3Z + n2Z) * n1Y + (-n2Y + n3Y) * n1Z + n4Y * n3Z - n4Y * n2Z + n4Z * n2Y - n4Z * n3Y)) * ni + i * nj * ((n3Z - n4Z) * n1Y + (n4Y - n3Y) * n1Z + (-n3Z + n4Z) * n2Y - n2Z * (n4Y - n3Y))) * (1.0/(double)(ni*ni*nj*nj)));

                return area1+area2;;

            };

            // qDebug()<<"yCoord"<<y;
            // qDebug()<<"zCoord"<<z;
            // qDebug()<<"area"<<area;

            // Create the fiber
            auto fiberName = QString("Fibre") + parent + QString("%1-%2of%3-ObjNo:%4").arg(i).arg(j).arg(theMaterialKnowledge->getTheComponentName()).arg(fiberCounter);
            theDomain->createObject(fiberName,theFiberType);
            auto theFiber = qobject_cast<RFiber *>(theDomain->getLastAddedObject());

            if(theFiber == nullptr)
            {
                qDebug()<<"Error creating a fiber in RDomain in "<<__FUNCTION__;
                return -1;
            }
            ++fiberCounter;

            // Create the material
            auto materialName = materialType + parent + QString("%1-%2of%3-ObjNo:%4").arg(i).arg(j).arg(theMaterialKnowledge->getTheComponentName()).arg(matCounter);
            theDomain->createObject(materialName, materialType);
            auto theUniaxialMaterial = qobject_cast<RUniaxialMaterial *>(theDomain->getLastAddedObject());

            if(theUniaxialMaterial == nullptr)
            {
                qDebug()<<"Error creating the uniaxial material in RDomain in "<<__FUNCTION__;
                return -1;
            }
            ++matCounter;

            //Set the material knowledge to the material
            theUniaxialMaterial->setMaterialKnowledge(theMaterialKnowledge);

            //Set the material, coordinates, and area to the fiber
            theFiber->setTheUniaxialMaterial(theUniaxialMaterial);
            theFiber->setArea(arealambda);
            theFiber->setFibreYCoordinate(yCoordinatelambda);
            theFiber->setFibreZCoordinate(zCoordinatelambda);

            theFibreVector->push_back(theFiber);
        }
    }


    return 0;
}


int RFibreSectionMeshGenerator::meshLinearRebarLayer(const R2DPoint& point1,
                                                     const R2DPoint& point2,
                                                     const QString& materialType,
                                                     const QString& parent,
                                                     const int numRebar,
                                                     const double rebarArea,
                                                     std::vector<RFiber*> *theFibreVector,
                                                     RMaterialKnowledge *theMaterialKnowledge)
{

    //Calculate the length; Note: this is not the length of the fibre!
    auto xcoord1 = point1.getYCoordinateValue();
    auto xcoord2 = point2.getYCoordinateValue();
    auto ycoord1 = point1.getZCoordinateValue();
    auto ycoord2 = point2.getZCoordinateValue();

    auto dy = xcoord2-xcoord1;
    auto dz = ycoord2-ycoord1;

    auto length =  sqrt(dy*dy + dz*dz);

    if (length<=0.0 ) {
        qCritical() << "Coinciding points were given to the mesh generator in the component called" << theMaterialKnowledge->getTheComponentName();
        return -1;
    }

    int ni = numRebar;

    double y = 0.0;
    double z = 0.0;


    for (int i=0; i<ni; ++i)
    {

        // y COORDINATE
        std::function<double(void)> yCoordinatelambda = [ni, i, point1, point2]
        {
            double n1y = point1.getYCoordinateValue();
            double n2y = point2.getYCoordinateValue();

            double ini = i/(double)(ni-1);

            return (n1y+ini*(n2y-n1y));
        };


        // z COORDINATE
        std::function<double(void)> zCoordinatelambda = [ni, i, point1, point2]
        {
            double n1z = point1.getZCoordinateValue();
            double n2z = point2.getZCoordinateValue();

            double ini = i/(double)(ni-1);

            return (n1z+ini*(n2z-n1z));
        };

        y = yCoordinatelambda();
        z = zCoordinatelambda();

        // qDebug()<<"For node "<<nodeName<<" x coord is "<<x;
        // qDebug()<<"For node "<<nodeName<<" y coord is "<<y;

        //Create the fiber
        auto fiberName = QString("Fibre") + parent + QString("%1of%2-ObjNo:%3").arg(i).arg(theMaterialKnowledge->getTheComponentName()).arg(fiberCounter);
        theDomain->createObject(fiberName,theFiberType);
        auto theFiber = qobject_cast<RFiber *>(theDomain->getLastAddedObject());

        if(theFiber == nullptr)
        {
            qDebug()<<"Error creating a fiber in RDomain in "<<__FUNCTION__;
            return -1;
        }
        ++fiberCounter;

        //Create the material
        auto materialName = QString("Material") + materialType + parent + QString("%1of%2-ObjNo:%3").arg(i).arg(theMaterialKnowledge->getTheComponentName()).arg(matCounter);
        theDomain->createObject(materialName, materialType);
        auto theUniaxialMaterial = qobject_cast<RUniaxialMaterial *>(theDomain->getLastAddedObject());

        if(theUniaxialMaterial == nullptr)
        {
            qDebug()<<"Error creating the uniaxial material in RDomain in "<<__FUNCTION__;
            return -1;
        }
        ++matCounter;

        //Set the material knowledge to the material
        theUniaxialMaterial->setMaterialKnowledge(theMaterialKnowledge);

        //Set the material, coordinates, and area to the fiber
        theFiber->setTheUniaxialMaterial(theUniaxialMaterial);
        theFiber->setArea(rebarArea); //This is a "fixed" quantity, IE the area of the manufactured rebar
        theFiber->setFibreYCoordinate(yCoordinatelambda);
        theFiber->setFibreZCoordinate(zCoordinatelambda);

        theFibreVector->push_back(theFiber);

    }


    return 0;
}


int RFibreSectionMeshGenerator::meshLinearRebarLayerII(const R2DPoint& point1,
                                                       const R2DPoint& point2,
                                                       const QString& materialType,
                                                       const QString& parent,
                                                       const int numRebar,
                                                       const double rebarArea,
                                                       std::vector<RFiber*> *theFibreVector,
                                                       RMaterialKnowledge *theMaterialKnowledge)
{

    //Calculate the length; Note: this is not the length of the fibre!
    auto xcoord1 = point1.getYCoordinateValue();
    auto xcoord2 = point2.getYCoordinateValue();
    auto ycoord1 = point1.getZCoordinateValue();
    auto ycoord2 = point2.getZCoordinateValue();

    auto dy = xcoord2-xcoord1;
    auto dz = ycoord2-ycoord1;

    auto length =  sqrt(dy*dy + dz*dz);


    if (length<=0.0 ) {
        qCritical() << "Coinciding points were given to the mesh generator in the component called" << theMaterialKnowledge->getTheComponentName();
        return -1;
    }


    int ni = numRebar;

    double y = 0.0;
    double z = 0.0;


    for (int i=1; i<ni-1; ++i)
    {
        // y COORDINATE
        std::function<double(void)> yCoordinatelambda = [ni, i, point1, point2]
        {
            double n1y = point1.getYCoordinateValue();
            double n2y = point2.getYCoordinateValue();

            double ini = i/(double)(ni-1);

            return (n1y+ini*(n2y-n1y));
        };


        // z COORDINATE
        std::function<double(void)> zCoordinatelambda = [ni, i, point1, point2]
        {
            double n1z = point1.getZCoordinateValue();
            double n2z = point2.getZCoordinateValue();

            double ini = i/(double)(ni-1);

            return (n1z+ini*(n2z-n1z));
        };

        y = yCoordinatelambda();
        z = zCoordinatelambda();

        // qDebug()<<"For node "<<nodeName<<" x coord is "<<x;
        // qDebug()<<"For node "<<nodeName<<" y coord is "<<y;

        //Create the fiber
        auto fiberName = QString("Fibre") + parent + QString("%1of%2-ObjNo:%3").arg(i).arg(theMaterialKnowledge->getTheComponentName()).arg(fiberCounter);
        theDomain->createObject(fiberName,theFiberType);
        auto theFiber = qobject_cast<RFiber *>(theDomain->getLastAddedObject());

        if(theFiber == nullptr)
        {
            qDebug()<<"Error creating a fiber in RDomain in "<<__FUNCTION__;
            return -1;
        }
        ++fiberCounter;

        //Create the material
        auto materialName = QString("Material") + materialType + parent + QString("%1of%2-ObjNo:%3").arg(i).arg(theMaterialKnowledge->getTheComponentName()).arg(matCounter);
        theDomain->createObject(materialName, materialType);
        auto theUniaxialMaterial = qobject_cast<RUniaxialMaterial *>(theDomain->getLastAddedObject());

        if(theUniaxialMaterial == nullptr)
        {
            qDebug()<<"Error creating the uniaxial material in RDomain in "<<__FUNCTION__;
            return -1;
        }
        ++matCounter;

        //Set the material knowledge to the material
        theUniaxialMaterial->setMaterialKnowledge(theMaterialKnowledge);

        //Set the material, coordinates, and area to the fiber
        theFiber->setTheUniaxialMaterial(theUniaxialMaterial);
        theFiber->setArea(rebarArea); //This is a "fixed" quantity, IE the area of the manufactured rebar
        theFiber->setFibreYCoordinate(yCoordinatelambda);
        theFiber->setFibreZCoordinate(zCoordinatelambda);

        theFibreVector->push_back(theFiber);
    }


    return 0;
}


int RFibreSectionMeshGenerator::meshPlateLayer(const R2DPoint& point1,
                                               const R2DPoint& point2,
                                               const QString& materialType,
                                               const QString& parent,
                                               const int numFibres,
                                               const RParameter* thickness,
                                               std::vector<RFiber*> *theFibreVector,
                                               RMaterialKnowledge *theMaterialKnowledge)
{

    //Calculate the distance between the points as a check
    auto xcoord1 = point1.getYCoordinateValue();
    auto xcoord2 = point2.getYCoordinateValue();
    auto ycoord1 = point1.getZCoordinateValue();
    auto ycoord2 = point2.getZCoordinateValue();

    auto dy = xcoord2-xcoord1;
    auto dz = ycoord2-ycoord1;

    auto dist =  sqrt(dy*dy + dz*dz);

    if (dist<=0.0 ) {
        qCritical() << "Coinciding points were given to the mesh generator in the component called" << theMaterialKnowledge->getTheComponentName();
        return -1;
    }

    int ni = numFibres;

    double y = 0.0;
    double z = 0.0;

    std::function<double(void)> fibreArea = [=]
    {
        auto xcoord1 = point1.getYCoordinateValue();
        auto xcoord2 = point2.getYCoordinateValue();
        auto ycoord1 = point1.getZCoordinateValue();
        auto ycoord2 = point2.getZCoordinateValue();

        auto dy = xcoord2-xcoord1;
        auto dz = ycoord2-ycoord1;

        auto dist =  sqrt(dy*dy + dz*dz);

        return thickness->getCurrentValue()*(dist/static_cast<double>(numFibres));
    };


    for (int i=0; i<ni; ++i)
    {

        // y COORDINATE
        std::function<double(void)> yCoordinatelambda = [=]
        {
            double n1y = point1.getYCoordinateValue();
            double n2y = point2.getYCoordinateValue();

            double ini = i/static_cast<double>(ni-1);

            return (n1y+ini*(n2y-n1y));
        };


        // z COORDINATE
        std::function<double(void)> zCoordinatelambda = [=]
        {
            double n1z = point1.getZCoordinateValue();
            double n2z = point2.getZCoordinateValue();

            double ini = i/static_cast<double>(ni-1);

            return (n1z+ini*(n2z-n1z));
        };

        y = yCoordinatelambda();
        z = zCoordinatelambda();

        //qDebug()<<"For node "<<nodeName<<" x coord is "<<x;
        //qDebug()<<"For node "<<nodeName<<" y coord is "<<y;

        //Create the fiber
        auto fiberName = QString("Fibre") + parent + QString("%1of%2-ObjNo:%3").arg(i).arg(theMaterialKnowledge->getTheComponentName()).arg(fiberCounter);
        theDomain->createObject(fiberName,theFiberType);
        auto theFiber = qobject_cast<RFiber *>(theDomain->getLastAddedObject());

        if(theFiber == nullptr)
        {
            qDebug()<<"Error creating a fiber in RDomain in "<<__FUNCTION__;
            return -1;
        }
        ++fiberCounter;

        //Create the material
        auto materialName = QString("Material") + materialType + parent + QString("%1of%2-ObjNo:%3").arg(i).arg(theMaterialKnowledge->getTheComponentName()).arg(matCounter);
        theDomain->createObject(materialName, materialType);
        auto theUniaxialMaterial = qobject_cast<RUniaxialMaterial *>(theDomain->getLastAddedObject());

        if(theUniaxialMaterial == nullptr)
        {
            qDebug()<<"Error creating the uniaxial material in RDomain in "<<__FUNCTION__;
            return -1;
        }
        ++matCounter;

        //Set the material knowledge to the material
        theUniaxialMaterial->setMaterialKnowledge(theMaterialKnowledge);

        //Set the material, coordinates, and area to the fiber
        theFiber->setTheUniaxialMaterial(theUniaxialMaterial);
        theFiber->setArea(fibreArea);
        theFiber->setFibreYCoordinate(yCoordinatelambda);
        theFiber->setFibreZCoordinate(zCoordinatelambda);

        theFibreVector->push_back(theFiber);
    }

    return 0;
}




