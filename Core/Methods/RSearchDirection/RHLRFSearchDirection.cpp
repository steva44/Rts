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

#include "RHLRFSearchDirection.h"
#include "RMatrixOperations.h"
#include "RDomain.h"

#include <math.h>

RHLRFSearchDirection::RHLRFSearchDirection(QObject *parent, QString name)
    : RSearchDirection(parent, name)
{

}


RHLRFSearchDirection::~RHLRFSearchDirection()
{

}


int RHLRFSearchDirection::getSearchDirection(QVector<double> const *u, double functionValue, QVector<double> const *gradientInStandardNormalSpace, QVector<double> *result)
{
    // Create a matrix operations tool
    RMatrixOperations theMatrixOperations(theDomain);


    // Determine vector sizes
    int size = gradientInStandardNormalSpace->size();


    // Compute the norm of the gradient
    double gradientNorm = theMatrixOperations.norm(gradientInStandardNormalSpace);


    // Check that the norm is not zero
    if (gradientNorm == 0.0) {

        qCritical() << "Error: The norm of the gradient in the HLRF search direction is zero.";

        for (int i=0; i<size; i++) {

            (*result)[i] = 1.0;
        }

        return 0;
    }


    // Compute alpha-vector using BLAS: alpha = gradientInStandardNormalSpace * (-1 / gradientNorm)
    QVector<double> alpha(size);
    for (int i=0; i<size; i++) {

        alpha[i] = -gradientInStandardNormalSpace->at(i)/gradientNorm;
    }


    // Compute the alpha.u using BLAS: alpha_times_u = alpha * u
    double alpha_times_u = 0.0;
    for (int i=0; i<u->size(); i++) {

        alpha_times_u += alpha.at(i)*u->at(i);
    }


    // Compute the search direction vector using BLAS: direction = alpha * (functionValue / gradientNorm + alpha_times_u) - u
    for (int i=0; i<size; i++) {

        (*result)[i] = ((functionValue / gradientNorm + alpha_times_u) * alpha.at(i)) - u->at(i);
    }

    return 0;
}

