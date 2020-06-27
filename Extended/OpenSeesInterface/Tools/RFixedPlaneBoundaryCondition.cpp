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

#include "RFixedPlaneBoundaryCondition.h"
#include "RPoint.h"
#include "RParameter.h"

RFixedPlaneBoundaryCondition::RFixedPlaneBoundaryCondition(QObject *parent, QString &name)
    : RObject(parent, name)
{
    thePoint1 = nullptr;
    thePoint2 = nullptr;
    thePoint3 = nullptr;
    thePoint4 = nullptr;
}


RFixedPlaneBoundaryCondition::~RFixedPlaneBoundaryCondition()
{

}


QObject * RFixedPlaneBoundaryCondition::getPoint1() const
{
    return thePoint1;
}


void RFixedPlaneBoundaryCondition::setPoint1(QObject *value)
{
    thePoint1 = qobject_cast<RPoint *>(value);

    thePoint1->setIsOn(false);
}


QObject * RFixedPlaneBoundaryCondition::getPoint2() const
{
    return thePoint2;
}


void RFixedPlaneBoundaryCondition::setPoint2(QObject *value)
{
    thePoint2 = qobject_cast<RPoint *>(value);

    thePoint2->setIsOn(false);
}


QObject * RFixedPlaneBoundaryCondition::getPoint3() const
{
    return thePoint3;
}


void RFixedPlaneBoundaryCondition::setPoint3(QObject *value)
{
    thePoint3 = qobject_cast<RPoint *>(value);

    thePoint3->setIsOn(false);
}


QObject * RFixedPlaneBoundaryCondition::getPoint4() const
{
    return thePoint4;
}


void RFixedPlaneBoundaryCondition::setPoint4(QObject *value)
{
    thePoint4 = qobject_cast<RPoint *>(value);

    thePoint4->setIsOn(false);
}



