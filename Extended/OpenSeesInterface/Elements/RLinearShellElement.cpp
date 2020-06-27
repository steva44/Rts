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

#include "RLinearShellElement.h"
#include "RCoordinateTransformation.h"
#include "RSectionForceDeformation.h"
#include "RMathUtils.h"

RLinearShellElement::RLinearShellElement(QObject *parent, QString &name)
    : RElement(parent, name)
{  
    theNode1 = nullptr;
    theNode2 = nullptr;
    theNode3 = nullptr;
    theNode4 = nullptr;
    theSectionForceDeformation = nullptr;
}


RLinearShellElement::~RLinearShellElement()
{

}


int RLinearShellElement::getNumNodes()
{
    return 4;
}


RNode * RLinearShellElement::getNode(int i) const
{
    if (i==0) {
        return theNode1;
    }
    else if (i==1) {
        return theNode2;
    }
    else if (i==2) {
        return theNode3;
    }
    else if (i==3) {
        return theNode4;
    }
    else {
        return nullptr;
    }
}


int RLinearShellElement::setNode(int i, RNode *theNode)
{
    if (i==0) {
        theNode1 = theNode;
    }
    else if (i==1) {
        theNode2 = theNode;
    }
    else if (i==2) {
        theNode3 = theNode;
    }
    else if (i==3) {
        theNode4 = theNode;
    }
    else {
        qCritical() << "The Quad4 element only has nodes associated with i=0, i=1, i=2, and i=3";
    }

    return 0;
}


RSectionForceDeformation *RLinearShellElement::getTheSectionForceDeformation() const
{
    return theSectionForceDeformation;
}


void RLinearShellElement::setTheSectionForceDeformation(RSectionForceDeformation *value)
{
    theSectionForceDeformation = value;
}


double RLinearShellElement::getLength12()
{
    auto xcoord1 = theNode1->getXCoordinateValue();
    auto xcoord2 = theNode2->getXCoordinateValue();

    auto ycoord1 = theNode1->getYCoordinateValue();
    auto ycoord2 = theNode2->getYCoordinateValue();

    auto zcoord1 = theNode1->getZCoordinateValue();
    auto zcoord2 = theNode2->getZCoordinateValue();

    // Length of vector from 1 to 2
    auto length12 = sqrt( (xcoord2-xcoord1) * (xcoord2-xcoord1) + (ycoord2-ycoord1) * (ycoord2-ycoord1) + (zcoord2-zcoord1) * (zcoord2-zcoord1));

    return length12;
}


double RLinearShellElement::getLength23()
{
    QVector<double> vector23(3);

    auto xcoord2 = theNode2->getXCoordinateValue();
    auto xcoord3 = theNode3->getXCoordinateValue();

    auto ycoord2 = theNode2->getYCoordinateValue();
    auto ycoord3 = theNode3->getYCoordinateValue();

    auto zcoord2 = theNode2->getZCoordinateValue();
    auto zcoord3 = theNode3->getZCoordinateValue();

    // Length of vector from 2 to 3
    auto length23 = sqrt( (xcoord3-xcoord2) * (xcoord3-xcoord2) + (ycoord3-ycoord2) * (ycoord3-ycoord2) + (zcoord3-zcoord2) * (zcoord3-zcoord2));

    return length23;
}


double RLinearShellElement::getLength34()
{
    QVector<double> vector34(3);

    auto xcoord3 = theNode3->getXCoordinateValue();
    auto xcoord4 = theNode4->getXCoordinateValue();

    auto ycoord3 = theNode3->getYCoordinateValue();
    auto ycoord4 = theNode4->getYCoordinateValue();

    auto zcoord3 = theNode3->getZCoordinateValue();
    auto zcoord4 = theNode4->getZCoordinateValue();

    // Length of vector from 3 to 4
    auto length34 = sqrt( (xcoord4-xcoord3) * (xcoord4-xcoord3) + (ycoord4-ycoord3) * (ycoord4-ycoord3) + (zcoord4-zcoord3) * (zcoord4-zcoord3));

    return length34;
}


double RLinearShellElement::getLength41()
{
    QVector<double> vector14(3);

    auto xcoord1 = theNode1->getXCoordinateValue();
    auto xcoord4 = theNode4->getXCoordinateValue();

    auto ycoord1 = theNode1->getYCoordinateValue();
    auto ycoord4 = theNode4->getYCoordinateValue();

    auto zcoord1 = theNode1->getZCoordinateValue();
    auto zcoord4 = theNode4->getZCoordinateValue();

    // Length of vector from 1 to 4
    auto length41 = sqrt( (xcoord4-xcoord1) * (xcoord4-xcoord1) + (ycoord4-ycoord1) * (ycoord4-ycoord1) + (zcoord4-zcoord1) * (zcoord4-zcoord1));

    return length41;
}


double RLinearShellElement::getCharacteristicLength()
{
    auto length1 = this->getLength11();
    auto length2 = this->getLength22();

    return (length1 < length2 ? length1 : length2);
}


double RLinearShellElement::getArea(void)
{
    QVector<double> vector12(3);
    QVector<double> vector14(3);
    QVector<double> vector32(3);
    QVector<double> vector34(3);

    // Original nodal coordinates
    auto xcoord1 = theNode1->getXCoordinateValue();
    auto xcoord2 = theNode2->getXCoordinateValue();
    auto xcoord3 = theNode3->getXCoordinateValue();
    auto xcoord4 = theNode4->getXCoordinateValue();

    auto ycoord1 = theNode1->getYCoordinateValue();
    auto ycoord2 = theNode2->getYCoordinateValue();
    auto ycoord3 = theNode3->getYCoordinateValue();
    auto ycoord4 = theNode4->getYCoordinateValue();

    auto zcoord1 = theNode1->getZCoordinateValue();
    auto zcoord2 = theNode2->getZCoordinateValue();
    auto zcoord3 = theNode3->getZCoordinateValue();
    auto zcoord4 = theNode4->getZCoordinateValue();

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


std::vector<double> RLinearShellElement::getCentroid(void)
{
    auto xcoord1 = theNode1->getXCoordinateValue();
    auto xcoord2 = theNode2->getXCoordinateValue();
    auto xcoord3 = theNode3->getXCoordinateValue();
    auto xcoord4 = theNode4->getXCoordinateValue();

    auto ycoord1 = theNode1->getYCoordinateValue();
    auto ycoord2 = theNode2->getYCoordinateValue();
    auto ycoord3 = theNode3->getYCoordinateValue();
    auto ycoord4 = theNode4->getYCoordinateValue();

    auto zcoord1 = theNode1->getZCoordinateValue();
    auto zcoord2 = theNode2->getZCoordinateValue();
    auto zcoord3 = theNode3->getZCoordinateValue();
    auto zcoord4 = theNode4->getZCoordinateValue();

    std::vector<double> point1{xcoord1,ycoord1,zcoord1};
    std::vector<double> point2{xcoord2,ycoord2,zcoord2};
    std::vector<double> point3{xcoord3,ycoord3,zcoord3};
    std::vector<double> point4{xcoord4,ycoord4,zcoord4};

    std::vector<std::vector<double>> points{point1,point2,point3,point4};

    return RMathUtils::computePolygonCentroid<std::vector<double>>(points);
}


std::vector<double> RLinearShellElement::getNormalVector(void)
{
    auto xcoord1 = theNode1->getXCoordinateValue();
    auto xcoord2 = theNode2->getXCoordinateValue();
    auto xcoord3 = theNode3->getXCoordinateValue();

    auto ycoord1 = theNode1->getYCoordinateValue();
    auto ycoord2 = theNode2->getYCoordinateValue();
    auto ycoord3 = theNode3->getYCoordinateValue();

    auto zcoord1 = theNode1->getZCoordinateValue();
    auto zcoord2 = theNode2->getZCoordinateValue();
    auto zcoord3 = theNode3->getZCoordinateValue();

    std::vector<double> U(3);
    std::vector<double> V(3);
    std::vector<double> Normal(3);

    //(Triangle.p2 minus Triangle.p1)
    U[0] = xcoord2 - xcoord1;
    U[1] = ycoord2 - ycoord1;
    U[2] = zcoord2 - zcoord1;

    //(Triangle.p3 minus Triangle.p1)
    V[0] = xcoord3 - xcoord1;
    V[1] = ycoord3 - ycoord1;
    V[2] = zcoord3 - zcoord1;

    Normal[0] = ( U[1] * V[2]) - ( U[2] * V[1]);
    Normal[1] = ( U[2] * V[0]) - ( U[0] * V[2]);
    Normal[2] = ( U[0] * V[1]) - ( U[1] * V[0]);

    RMathUtils::normalizeVector(&Normal);

    return Normal;
}


double RLinearShellElement::getLength22()
{
    // Length of vector from 1 to 2
    double length23 = this->getLength23();
    double length41 = this->getLength41();

    auto length2 = 0.5*(length23+length41);

    return length2;
}


double RLinearShellElement::getLength11()
{
    double length12 = this->getLength12();
    double length34 = this->getLength34();

    auto length1 = 0.5*(length12+length34);

    return length1;
}


