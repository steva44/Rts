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

#include "RSteepestDescentSearchDirection.h"
#include "RDomain.h"

#include <math.h>

RSteepestDescentSearchDirection::RSteepestDescentSearchDirection(QObject *parent, QString name)
    : RSearchDirection(parent, name)
{

}


RSteepestDescentSearchDirection::~RSteepestDescentSearchDirection()
{

}


int RSteepestDescentSearchDirection::getSearchDirection(QVector<double> const *point, double function, QVector<double> const *gradient, QVector<double> *result)
{    
    // Simply set the search direction equal to the gradient
    for (int i=0; i<gradient->size(); i++) {

        (*result)[i] = -gradient->at(i);
    }


    // Output
    if (theOutputLevel >= RObject::Medium) {

        qDebug() << this->objectName() << "is returning the following search direction:";
        theDomain->print(result);
    }


    return 0;
}
