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

#include "RWallComponent.h"
#include "RParameter.h"
#include "RMathUtils.h"

RWallComponent::RWallComponent(QObject *parent, QString &name)
    : RComponent(parent, name)
{
    thePoint1 = nullptr;
    thePoint2 = nullptr;
    thePoint3 = nullptr;
    thePoint4 = nullptr;
    theNumElementsAlong12 = nullptr;
    theNumElementsAlong23 = nullptr;
    theThicknessParameter = nullptr;

    //Create the parameters for the number of elements along each axis
    theDomain->createObject(objectName() + "theNumElAlong12Parameter", "RConstant");
    theNumElementsAlong12 = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
    theNumElementsAlong12->setNodalParameterFlag(true);
    addToParameterList(theNumElementsAlong12);

    theDomain->createObject(objectName() + "theNumElAlong23Parameter", "RConstant");
    theNumElementsAlong23 = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
    theNumElementsAlong23->setNodalParameterFlag(true);
    addToParameterList(theNumElementsAlong23);
}


RWallComponent::~RWallComponent()
{

}


double RWallComponent::getVolume()
{
    return getElevationArea()*theThicknessParameter->getCurrentValue();
}


double RWallComponent::getElevationArea()
{
    QVector<double> vector12(3);
    QVector<double> vector14(3);
    QVector<double> vector32(3);
    QVector<double> vector34(3);

    // Original nodal coordinates
    auto xcoord1 = thePoint1->getXCoordinateValue();
    auto xcoord2 = thePoint2->getXCoordinateValue();
    auto xcoord3 = thePoint3->getXCoordinateValue();
    auto xcoord4 = thePoint4->getXCoordinateValue();

    auto ycoord1 = thePoint1->getYCoordinateValue();
    auto ycoord2 = thePoint2->getYCoordinateValue();
    auto ycoord3 = thePoint3->getYCoordinateValue();
    auto ycoord4 = thePoint4->getYCoordinateValue();

    auto zcoord1 = thePoint1->getZCoordinateValue();
    auto zcoord2 = thePoint2->getZCoordinateValue();
    auto zcoord3 = thePoint3->getZCoordinateValue();
    auto zcoord4 = thePoint4->getZCoordinateValue();

    // Determine vectors shooting out from 1 and 3
    vector12[0] = xcoord2-xcoord1;
    vector12[1] = ycoord2-ycoord1;
    vector12[2] = zcoord2-zcoord1;

    vector14[0] = xcoord4-xcoord1;
    vector14[1] = ycoord4-ycoord1;
    vector14[2] = zcoord4-zcoord1;

    vector32[0] = xcoord2-xcoord3;
    vector32[1] = ycoord2-ycoord3;
    vector32[2] = zcoord2-zcoord3;

    vector34[0] = xcoord4-xcoord3;
    vector34[1] = ycoord4-ycoord3;
    vector34[2] = zcoord4-zcoord3;

    // The area of each triangle is half the norm of the cross-product
    QVector<double> cross12by14(3);
    cross12by14[0] = vector12[1] * vector14[2] - vector12[2] * vector14[1];
    cross12by14[1] = vector12[2] * vector14[0] - vector12[0] * vector14[2];
    cross12by14[2] = vector12[0] * vector14[1] - vector12[1] * vector14[0];

    QVector<double> cross34by32(3);
    cross34by32[0] = vector34[1] * vector32[2] - vector34[2] * vector32[1];
    cross34by32[1] = vector34[2] * vector32[0] - vector34[0] * vector32[2];
    cross34by32[2] = vector34[0] * vector32[1] - vector34[1] * vector32[0];

    auto normLambda = [](QVector<double> const& vector)
    {
        auto theNorm = 0.0;

        for (auto&& it: vector)
            theNorm += it*it;

        // Take the square root of the sum of squares, namely the norm
        return sqrt(theNorm);
    };

    auto area = 0.5*(normLambda(cross12by14) + normLambda(cross34by32));

    return area;
}


double RWallComponent::getSurfaceArea()
{
    auto areaWall = this->getElevationArea()*2.0;

    return areaWall + this->getElevationPerimeter()*theThicknessParameter->getCurrentValue();
}


double RWallComponent::getPlanArea()
{
    return this->getLengthValue()*theThicknessParameter->getCurrentValue();
}


double RWallComponent::getLengthValue()
{
    auto length12 = this->getLength12();
    auto length34 = this->getLength34();

    return 0.5*(length12+length34);
}


double RWallComponent::getHeightValue()
{
    auto length23 = this->getLength23();
    auto length41 = this->getLength41();

    return 0.5*(length23+length41);
}


void RWallComponent::setPoint1(QObject *value)
{
    thePoint1 = qobject_cast<RPoint *>(value);

    // Get the nodal parameters for DDM calculations
    QList<RParameter*> theXNodalParameterList = thePoint1->getXCoordinateParameters();
    QList<RParameter*> theYNodalParameterList = thePoint1->getYCoordinateParameters();
    QList<RParameter*> theZNodalParameterList = thePoint1->getZCoordinateParameters();

    for(auto&& itx : theXNodalParameterList)
    {
        itx->setNodalParameterFlag(true);
        theXNodalParameterMap.insertMulti(itx,thePoint1);
        addToParameterList(itx);
    }

    for(auto&& ity : theYNodalParameterList)
    {
        ity->setNodalParameterFlag(true);
        theYNodalParameterMap.insertMulti(ity,thePoint1);
        addToParameterList(ity);
    }

    for(auto&& itz : theZNodalParameterList)
    {
        itz->setNodalParameterFlag(true);
        theZNodalParameterMap.insertMulti(itz,thePoint1);
        addToParameterList(itz);
    }
}


void RWallComponent::setPoint2(QObject *value)
{
    thePoint2 = qobject_cast<RPoint *>(value);

    // Get the nodal parameters for DDM calculations
    QList<RParameter*> theXNodalParameterList = thePoint2->getXCoordinateParameters();
    QList<RParameter*> theYNodalParameterList = thePoint2->getYCoordinateParameters();
    QList<RParameter*> theZNodalParameterList = thePoint2->getZCoordinateParameters();

    for(auto&& itx : theXNodalParameterList)
    {
        itx->setNodalParameterFlag(true);
        theXNodalParameterMap.insertMulti(itx,thePoint2);
        addToParameterList(itx);
    }


    for(auto&& ity : theYNodalParameterList)
    {
        ity->setNodalParameterFlag(true);
        theYNodalParameterMap.insertMulti(ity,thePoint2);
        addToParameterList(ity);
    }

    for(auto&& itz : theZNodalParameterList)
    {
        itz->setNodalParameterFlag(true);
        theZNodalParameterMap.insertMulti(itz,thePoint2);
        addToParameterList(itz);
    }

}


void RWallComponent::setPoint3(QObject *value)
{
    thePoint3 = qobject_cast<RPoint *>(value);

    // Get the nodal parameters for DDM calculations
    QList<RParameter*> theXNodalParameterList = thePoint3->getXCoordinateParameters();
    QList<RParameter*> theYNodalParameterList = thePoint3->getYCoordinateParameters();
    QList<RParameter*> theZNodalParameterList = thePoint3->getZCoordinateParameters();

    for(auto&& itx : theXNodalParameterList)
    {
        itx->setNodalParameterFlag(true);
        theXNodalParameterMap.insertMulti(itx,thePoint3);
        addToParameterList(itx);
    }

    for(auto&& ity : theYNodalParameterList)
    {
        ity->setNodalParameterFlag(true);
        theYNodalParameterMap.insertMulti(ity,thePoint3);
        addToParameterList(ity);
    }

    for(auto&& itz : theZNodalParameterList)
    {
        itz->setNodalParameterFlag(true);
        theZNodalParameterMap.insertMulti(itz,thePoint3);
        addToParameterList(itz);
    }

}


void RWallComponent::setPoint4(QObject *value)
{
    thePoint4 = qobject_cast<RPoint *>(value);

    // Get the nodal parameters for DDM calculations
    QList<RParameter*> theXNodalParameterList = thePoint4->getXCoordinateParameters();
    QList<RParameter*> theYNodalParameterList = thePoint4->getYCoordinateParameters();
    QList<RParameter*> theZNodalParameterList = thePoint4->getZCoordinateParameters();

    for(auto&& itx : theXNodalParameterList)
    {
        itx->setNodalParameterFlag(true);
        theXNodalParameterMap.insertMulti(itx,thePoint4);
        addToParameterList(itx);
    }

    for(auto&& ity : theYNodalParameterList)
    {
        ity->setNodalParameterFlag(true);
        theYNodalParameterMap.insertMulti(ity,thePoint4);
        addToParameterList(ity);
    }

    for(auto&& itz : theZNodalParameterList)
    {
        itz->setNodalParameterFlag(true);
        theZNodalParameterMap.insertMulti(itz,thePoint4);
        addToParameterList(itz);
    }

}


void RWallComponent::setThickness(QObject* value)
{
    theThicknessParameter = qobject_cast<RParameter*>(value);
    theThicknessParameter->setNodalParameterFlag(true);
    addToParameterList(theThicknessParameter);
}


double RWallComponent::getLength12()
{
    auto xcoord1 = thePoint1->getXCoordinateValue();
    auto xcoord2 = thePoint2->getXCoordinateValue();

    auto ycoord1 = thePoint1->getYCoordinateValue();
    auto ycoord2 = thePoint2->getYCoordinateValue();

    auto zcoord1 = thePoint1->getZCoordinateValue();
    auto zcoord2 = thePoint2->getZCoordinateValue();

    // Length of vector from 1 to 2
    auto length12 = sqrt( (xcoord2-xcoord1) * (xcoord2-xcoord1) + (ycoord2-ycoord1) * (ycoord2-ycoord1) + (zcoord2-zcoord1) * (zcoord2-zcoord1));

    return length12;
}


double RWallComponent::getLength23()
{
    QVector<double> vector23(3);

    auto xcoord2 = thePoint2->getXCoordinateValue();
    auto xcoord3 = thePoint3->getXCoordinateValue();

    auto ycoord2 = thePoint2->getYCoordinateValue();
    auto ycoord3 = thePoint3->getYCoordinateValue();

    auto zcoord2 = thePoint2->getZCoordinateValue();
    auto zcoord3 = thePoint3->getZCoordinateValue();

    // Length of vector from 2 to 3
    auto length23 = sqrt( (xcoord3-xcoord2) * (xcoord3-xcoord2) + (ycoord3-ycoord2) * (ycoord3-ycoord2) + (zcoord3-zcoord2) * (zcoord3-zcoord2));

    return length23;
}


double RWallComponent::getLength34()
{
    QVector<double> vector34(3);

    auto xcoord3 = thePoint3->getXCoordinateValue();
    auto xcoord4 = thePoint4->getXCoordinateValue();

    auto ycoord3 = thePoint3->getYCoordinateValue();
    auto ycoord4 = thePoint4->getYCoordinateValue();

    auto zcoord3 = thePoint3->getZCoordinateValue();
    auto zcoord4 = thePoint4->getZCoordinateValue();

    // Length of vector from 3 to 4
    auto length34 = sqrt( (xcoord4-xcoord3) * (xcoord4-xcoord3) + (ycoord4-ycoord3) * (ycoord4-ycoord3) + (zcoord4-zcoord3) * (zcoord4-zcoord3));

    return length34;
}


double RWallComponent::getLength41()
{
    QVector<double> vector14(3);

    auto xcoord1 = thePoint1->getXCoordinateValue();
    auto xcoord4 = thePoint4->getXCoordinateValue();

    auto ycoord1 = thePoint1->getYCoordinateValue();
    auto ycoord4 = thePoint4->getYCoordinateValue();

    auto zcoord1 = thePoint1->getZCoordinateValue();
    auto zcoord4 = thePoint4->getZCoordinateValue();

    // Length of vector from 1 to 4
    auto length41 = sqrt( (xcoord4-xcoord1) * (xcoord4-xcoord1) + (ycoord4-ycoord1) * (ycoord4-ycoord1) + (zcoord4-zcoord1) * (zcoord4-zcoord1));

    return length41;
}


double RWallComponent::getElevationPerimeter()
{
    double length12 = this->getLength12();
    double length34 = this->getLength34();

    double length23 = this->getLength23();
    double length41 = this->getLength41();

    return length12+length23+length34+length41;
}


double RWallComponent::getAreaOfVoids()
{
    // To do: implement area of voids in wall
    return 0.0;
}


QList<RPoint*> RWallComponent::getPointList()
{
    QList<RPoint*> pointsList{thePoint1,thePoint2,thePoint3,thePoint4};

    return pointsList;
}


std::vector<double> RWallComponent::getWallNormal(void)
{
    std::vector<RPoint*> thePointsList = {thePoint1,thePoint2,thePoint3,thePoint4};

    RMathUtils theUtils;

    return theUtils.computePolygonNormal<RPoint*>(thePointsList);
}

