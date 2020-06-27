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

#include "RWindowComponent.h"
#include "RParameter.h"
#include "RMathUtils.h"

RWindowComponent::RWindowComponent(QObject *parent, QString &name)
    : RComponent(parent, name)
{
    meshWasCreated = true;
    isStructural = false;

    thePoint1 = nullptr;
    thePoint2 = nullptr;
    thePoint3 = nullptr;
    thePoint4 = nullptr;
    theThicknessParameter = nullptr;

    // Assume that the windows are exterior
    theComponentExposure = RInteriorOrExteriorComponent::EXTERIOR;

    // The RSI of the window for energy loss calculations (sq m * degree C) / W
    // https://www.open.edu/openlearn/nature-environment/the-environment/energy-buildings/content-section-2.2.2
    // Double-glazing (hard coat low-e, emissivity = 0.2, argon-filled) - U value = 2.0 (Rvalue = 1 / U)
    RSIValue =  0.5 ;
}


RWindowComponent::~RWindowComponent()
{

}


double RWindowComponent::getVolume()
{
    return getElevationArea()*theThicknessParameter->getCurrentValue();
}


double RWindowComponent::getElevationArea()
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


double RWindowComponent::getSurfaceArea()
{
    auto areawindow = this->getElevationArea()*2.0;

    return areawindow + this->getPerimeter()*theThicknessParameter->getCurrentValue();
}


double RWindowComponent::getWidthValue()
{
    auto length12 = this->getLength12();
    auto length34 = this->getLength34();

    return 0.5*(length12+length34);
}


double RWindowComponent::getHeightValue()
{
    auto length23 = this->getLength23();
    auto length41 = this->getLength41();

    return 0.5*(length23+length41);
}


void RWindowComponent::setPoint1(QObject *value)
{
    thePoint1 = qobject_cast<RPoint *>(value);
}


void RWindowComponent::setPoint2(QObject *value)
{
    thePoint2 = qobject_cast<RPoint *>(value);
}


void RWindowComponent::setPoint3(QObject *value)
{
    thePoint3 = qobject_cast<RPoint *>(value);
}


void RWindowComponent::setPoint4(QObject *value)
{
    thePoint4 = qobject_cast<RPoint *>(value);
}


void RWindowComponent::setThickness(QObject* value)
{
    theThicknessParameter = qobject_cast<RParameter*>(value);
    theThicknessParameter->setNodalParameterFlag(true);
    addToParameterList(theThicknessParameter);
}


double RWindowComponent::getLength12()
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


double RWindowComponent::getLength23()
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


double RWindowComponent::getLength34()
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


double RWindowComponent::getLength41()
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


double RWindowComponent::getPerimeter()
{
    double length12 = this->getLength12();
    double length34 = this->getLength34();

    double length23 = this->getLength23();
    double length41 = this->getLength41();

    return length12+length23+length34+length41;
}


QList<RPoint*> RWindowComponent::getPointList()
{
    QList<RPoint*> pointsList{thePoint1,thePoint2,thePoint3,thePoint4};

    return pointsList;
}


std::vector<double> RWindowComponent::getwindowNormal(void)
{
     std::vector<RPoint*> thePointsList = {thePoint1,thePoint2,thePoint3,thePoint4};

     RMathUtils theUtils;

     return theUtils.computePolygonNormal<RPoint*>(thePointsList);
}





