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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RComboSearchDirection.h"
#include "RMatrixOperations.h"
#include "RDomain.h"

#include <math.h>

RComboSearchDirection::RComboSearchDirection(QObject *parent, QString name)
    : RSearchDirection(parent, name)
{
    theSearchDirection1 = nullptr;
    theSearchDirection2 = nullptr;
    stepNumber = 0;
    theSwitchStep = 100;
}


RComboSearchDirection::~RComboSearchDirection()
{

}


QObject * RComboSearchDirection::getSearchDirection1() const
{
    return theSearchDirection1;
}


void RComboSearchDirection::setSearchDirection1(QObject *value)
{
    theSearchDirection1 = qobject_cast<RSearchDirection *>(value);
}


QObject * RComboSearchDirection::getSearchDirection2() const
{
    return theSearchDirection2;
}


void RComboSearchDirection::setSearchDirection2(QObject *value)
{
    theSearchDirection2 = qobject_cast<RSearchDirection *>(value);
}


int RComboSearchDirection::getSwitchStep() const
{
    return theSwitchStep;
}


void RComboSearchDirection::setSwitchStep(int value)
{
    theSwitchStep = value;
}


int RComboSearchDirection::getSearchDirection(QVector<double> const *point, double function, QVector<double> const *gradient, QVector<double> *result)
{
    stepNumber ++;

    if (stepNumber < theSwitchStep) {

        theSearchDirection1->getSearchDirection(point, function, gradient, result);
    }
    else {

        theSearchDirection2->getSearchDirection(point, function, gradient, result);
    }

    return 0;
}

