#ifndef RWindowComponent_H
#define RWindowComponent_H
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

// A quadrilateral window component

//   4_____________3    /
//   |             |    |
//   |             |    |
//   |             |    | Height
//   |             |    |
//   |             |    |
//  1|_____________|2   /
//
//   /-------------/
//        Width

#include "RComponent.h"
#include "RPoint.h"

class RParameter;

class RWindowComponent : public RComponent
{
    Q_OBJECT
    Q_PROPERTY(QObject* Thickness READ getThickness WRITE setThickness)
    Q_PROPERTY(QObject *Point1 READ getPoint1 WRITE setPoint1)
    Q_PROPERTY(QObject *Point2 READ getPoint2 WRITE setPoint2)
    Q_PROPERTY(QObject *Point3 READ getPoint3 WRITE setPoint3)
    Q_PROPERTY(QObject *Point4 READ getPoint4 WRITE setPoint4)

public:
    RWindowComponent(QObject *parent, QString &name);
    ~RWindowComponent();

    // The thickness of the window in m
    inline RParameter* getThickness(){return theThicknessParameter;}
    void setThickness(QObject *value);

    // The corner points of the window
    inline QObject *getPoint1() const {return thePoint1;}
    void setPoint1(QObject *value);

    inline QObject *getPoint2() const {return thePoint2;}
    void setPoint2(QObject *value);

    inline QObject *getPoint3() const {return thePoint3;}
    void setPoint3(QObject *value);

    inline QObject *getPoint4() const {return thePoint4;}
    void setPoint4(QObject *value);

    // Volume of the window in m^3
    double getVolume();

    // Elevation area of the window mid-surface in m^2
    double getElevationArea();

    // Area of the all of the surfaces of the window in m^2
    double getSurfaceArea();

    // Lengths of the various sides of the window in m
    double getLength12();
    double getLength23();
    double getLength34();
    double getLength41();

    // The perimeter of the window in m
    double getPerimeter();

    // The width of the window in m
    double getWidthValue();

    // The height of the window in m
    double getHeightValue();

    // Provides the vector that is normal to the window surface
    std::vector<double> getwindowNormal(void);

    QList<RPoint*> getPointList();

private:

    // Corner points
    RPoint *thePoint1;
    RPoint *thePoint2;
    RPoint *thePoint3;
    RPoint *thePoint4;

    // Thickness of the window assembly
    QPointer<RParameter> theThicknessParameter;

};

#endif
