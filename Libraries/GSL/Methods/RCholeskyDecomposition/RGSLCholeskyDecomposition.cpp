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

#include "RGSLCholeskyDecomposition.h"
#include "RDomain.h"
#include "RMainWindow.h"

#include <gsl/gsl_blas.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_linalg.h>

RGSLCholeskyDecomposition::RGSLCholeskyDecomposition(QObject *parent, QString name)
    : RCholeskyDecomposition(parent, name)
{

}


RGSLCholeskyDecomposition::~RGSLCholeskyDecomposition()
{

}


int RGSLCholeskyDecomposition::getCholesky(QVector< QVector<double> > *inputMatrix, QVector< QVector<double> > *cholesky, QVector< QVector<double> > *inverseCholesky)
{
    // Check pointers and dimensional consistency
    int result = checkPointersAndDimensions(inputMatrix, cholesky, inverseCholesky);
    if (result < 0) {
        qCritical() << "Error in the Cholesky decomposition in" << this->objectName();
        return -1;
    }


    // Get matrix size
    int size = inputMatrix->size();


    // Transform input into a GSL matrix
    gsl_matrix *theGSLMatrix = gsl_matrix_calloc(size, size);
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            gsl_matrix_set(theGSLMatrix,i,j,(*inputMatrix)[i][j]);
        }
    }


    // Perform the Cholesky decomposition
    int status = gsl_linalg_cholesky_decomp(theGSLMatrix);
    if (status) {
        qCritical() << "Error in GSL:" << gsl_strerror(status);
        return -1;
    }


    // Convert the resulting matrix back to a C++ vector-vector
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            (*cholesky)[i][j] = gsl_matrix_get(theGSLMatrix,i,j);
        }
    }

    // Set the upper triangle equal to zero, as it should be (is this necessary? depends on input...)
    for (int i = 0; i < size; i++) {
        for (int j = i+1; j < size; j++) {
            (*cholesky)[i][j] = 0.0;
        }
    }


    // Compute the inverse if requested
    if (inverseCholesky != 0) {
        status = gsl_linalg_cholesky_invert(theGSLMatrix);
        if (status) {
            qCritical() << "Error in GSL:" << gsl_strerror(status);
            return -1;
        }


        // Convert the resulting matrix back to a C++ vector-vector
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                (*inverseCholesky)[i][j] = gsl_matrix_get(theGSLMatrix,i,j);
            }
        }


        // Set the upper triangle equal to zero, as it should be (is this necessary? depends on input...)
        for (int i = 0; i < size; i++) {
            for (int j = i+1; j < size; j++) {
                (*inverseCholesky)[i][j] = 0.0;
            }
        }
    }


    // Free the GSL matrix
    gsl_matrix_free(theGSLMatrix);


    return 0;
}
