#ifndef RTrussComponent_H
#define RTrussComponent_H
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
*   - Gurvinder Gill                                                 *
*   - Seadon Chui                                                    *
*                                                                    *
*********************************************************************/

//    Point1   Point2
//       1       2
//       .-------.--->
//

// A base class for truss components

#include "RComponent.h"
#include "RPoint.h"

class RTrussComponent : public RComponent
{
    Q_OBJECT

    Q_PROPERTY(QObject *Point1 READ getPoint1 WRITE setPoint1)
    Q_PROPERTY(QObject *Point2 READ getPoint2 WRITE setPoint2)
    Q_PROPERTY(QString Orientation READ getOrientation WRITE setOrientation)

public:
    RTrussComponent(QObject *parent, QString &name);
    ~RTrussComponent();

    // The orientation along the centroidal axis
    QString getOrientation();
    void setOrientation(QString value);

    // Volume of the component in m^3
    double getVolume();

    // The end points
    inline QObject *getPoint1() const {return thePoint1;}
    void setPoint1(QObject *value);
    inline QObject *getPoint2() const {return thePoint2;}
    void setPoint2(QObject *value);

protected:

    // The end points
    RPoint *thePoint1;
    RPoint *thePoint2;

    // The orientation parameter
    QPointer<RParameter> theOrientationParameter;
    QPointer<RParameter> theLengthParameter;
    QPointer<RParameter> theAreaParameter;

    QMap<RParameter*,RPoint*>theXNodalParameterMap;
    QMap<RParameter*,RPoint*>theYNodalParameterMap;
    QMap<RParameter*,RPoint*>theZNodalParameterMap;

};

#endif
