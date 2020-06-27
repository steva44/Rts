#ifndef RFixedPlaneBoundaryCondition_H
#define RFixedPlaneBoundaryCondition_H
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

#include "RObject.h"

class RPoint;

class RFixedPlaneBoundaryCondition : public RObject
{
    Q_OBJECT
    Q_PROPERTY(QObject *Point1 READ getPoint1 WRITE setPoint1)
    Q_PROPERTY(QObject *Point2 READ getPoint2 WRITE setPoint2)
    Q_PROPERTY(QObject *Point3 READ getPoint3 WRITE setPoint3)
    Q_PROPERTY(QObject *Point4 READ getPoint4 WRITE setPoint4)

public:
    RFixedPlaneBoundaryCondition(QObject *parent, QString &name);
    ~RFixedPlaneBoundaryCondition();

    QObject *getPoint1() const;
    void setPoint1(QObject *value);

    QObject *getPoint2() const;
    void setPoint2(QObject *value);

    QObject *getPoint3() const;
    void setPoint3(QObject *value);

    QObject *getPoint4() const;
    void setPoint4(QObject *value);

private:

    RPoint *thePoint1;
    RPoint *thePoint2;
    RPoint *thePoint3;
    RPoint *thePoint4;

};

#endif
