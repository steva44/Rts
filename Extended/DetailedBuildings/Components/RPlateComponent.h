#ifndef RPlateComponent_H
#define RPlateComponent_H
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

// The plate parent class for slabs, which inherit information and functionality from this class

#include "RComponent.h"
#include "RPoint.h"
#include "RContinuousRandomVariable.h"

class RParameter;

//     y
//    ^
//    |__>x
//
//     4_____________3    /
//     |             |    |
//     |             |    |
//     |             |    | DEPTH
//     |             |    |
//     |             |    |
//    1|_____________|2   /
//
//     /-------------/
//          WIDTH


class RPlateComponent : public RComponent
{
    Q_OBJECT

    Q_PROPERTY(QObject *Thickness READ getThickness WRITE setThickness)
    Q_PROPERTY(QObject *Point1 READ getPoint1 WRITE setPoint1)
    Q_PROPERTY(QObject *Point2 READ getPoint2 WRITE setPoint2)
    Q_PROPERTY(QObject *Point3 READ getPoint3 WRITE setPoint3)
    Q_PROPERTY(QObject *Point4 READ getPoint4 WRITE setPoint4)

public:
    RPlateComponent(QObject *parent, QString &name);
    ~RPlateComponent();

    // The type of plate
    enum plateType{ROOF,FLOOR,BASESLAB,LANDING,NA};

    // The thickness of the plate
    inline RParameter* getThickness(){return theThicknessParameter;}
    void setThickness(QObject* value);

    // The corner points
    inline QObject *getPoint1() const {return thePoint1;}
    void setPoint1(QObject *value);

    inline QObject *getPoint2() const {return thePoint2;}
    void setPoint2(QObject *value);

    inline QObject *getPoint3() const {return thePoint3;}
    void setPoint3(QObject *value);

    inline QObject *getPoint4() const {return thePoint4;}
    void setPoint4(QObject *value);

    QList<RPoint*> getPointList();

    // Volume of the plate in m^3
    double getVolume();

    // Area of the plate in m^2
    double getPlanArea();

    // The lengths of the sides of the plate
    double getLength12();
    double getLength23();
    double getLength34();
    double getLength41();

    // Depth of the plate in m
    double getDepth();

    // Width of the plate in m
    double getWidth();

    // Perimeter of the plate in m
    double getPerimeter();

    // Type of plate, e.g.,ROOF,FLOOR,BASESLAB,LANDING
    void setPlateType(plateType type);
    plateType getPlateType(void);

protected:

    // Corner points
    RPoint *thePoint1;
    RPoint *thePoint2;
    RPoint *thePoint3;
    RPoint *thePoint4;

    // Parameters common to all slabs
    QPointer<RParameter> theThicknessParameter;
    QPointer<RParameter> theNumElementsAlong12;
    QPointer<RParameter> theNumElementsAlong23;
    plateType thePlateType;

private:

    QMap<RParameter*,RPoint*>theXNodalParameterMap;
    QMap<RParameter*,RPoint*>theYNodalParameterMap;
    QMap<RParameter*,RPoint*>theZNodalParameterMap;

};

#endif
