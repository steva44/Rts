#ifndef RWallComponent_H
#define RWallComponent_H
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

// Base class with functionality common to wall components

//
//  4--------3
//  |        |  Height
//  1--------2
//    Length
//
//   y
//  ^
//  |
//  *--->x
//
//  *z is out of screen along the wall's height
//
//            The cross section
//   ________________________________________   /
//   |                                      |   |
//   |                                      |   |  Thickness
//   |                                      |   |
//   |______________________________________|   /
//
//   /--------------------------------------/
//                    Length

#include "RComponent.h"
#include "RPoint.h"
#include "RContinuousRandomVariable.h"

class RParameter;

class RWallComponent : public RComponent
{
    Q_OBJECT
    Q_PROPERTY(QObject* Thickness READ getThickness WRITE setThickness)
    Q_PROPERTY(QObject *Point1 READ getPoint1 WRITE setPoint1)
    Q_PROPERTY(QObject *Point2 READ getPoint2 WRITE setPoint2)
    Q_PROPERTY(QObject *Point3 READ getPoint3 WRITE setPoint3)
    Q_PROPERTY(QObject *Point4 READ getPoint4 WRITE setPoint4)
    Q_PROPERTY(QString MeshOption READ getMeshOption WRITE setMeshOption)

public:
    RWallComponent(QObject *parent, QString &name);
    ~RWallComponent();

    inline RParameter* getThickness(){return theThicknessParameter;}

    void setThickness(QObject *value);

    inline QObject *getPoint1() const {return thePoint1;}

    void setPoint1(QObject *value);

    inline QObject *getPoint2() const {return thePoint2;}

    void setPoint2(QObject *value);

    inline QObject *getPoint3() const {return thePoint3;}

    void setPoint3(QObject *value);

    inline QObject *getPoint4() const {return thePoint4;}

    void setPoint4(QObject *value);

    QList<RPoint*> getPointList();

    // Set and get the finite element mesh options
    QString getMeshOption(){return QString::number(localMeshOption);}
    void setMeshOption(QString value){localMeshOption = value.toDouble();}

    // The volume of the wall in m^3
    double getVolume();

    // The area of the wall mid-surface; the elevation area in m^2
    double getElevationArea();

    // The area of the wall at the top and bottom in m^2
    double getPlanArea();

    // The area of the all of the surfaces of the wall in m^2
    double getSurfaceArea();

    // The lengths of the various sides of the wall in m
    double getLength12();
    double getLength23();
    double getLength34();
    double getLength41();

    // The perimeter of the wall in m
    double getElevationPerimeter();

    // The length of the wall in m
    double getLengthValue();

    // The height of the wall in m
    double getHeightValue();

    // The area of the voids within the wall, i.e., doors, windows in m^2
    double getAreaOfVoids();

    // The number of elements for meshing
    inline QPointer<RParameter> getNumElementsAlong12(void){return  theNumElementsAlong12;}
    inline QPointer<RParameter> getNumElementsAlong23(void){return  theNumElementsAlong23;}

    // Provides the vector that is normal to the wall elevation surface
    std::vector<double> getWallNormal(void);

protected:

    // The corner points
    RPoint *thePoint1;
    RPoint *thePoint2;
    RPoint *thePoint3;
    RPoint *thePoint4;

    // Wall discretization parameters
    QPointer<RParameter> theNumElementsAlong12;
    QPointer<RParameter> theNumElementsAlong23;

    // Wall thickness in m
    QPointer<RParameter> theThicknessParameter;

private:

    QMap<RParameter*,RPoint*>theXNodalParameterMap;
    QMap<RParameter*,RPoint*>theYNodalParameterMap;
    QMap<RParameter*,RPoint*>theZNodalParameterMap;
};

#endif
