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

#include "RGSLHHLinearSolver.h"
#include "RDomain.h"
#include "RMainWindow.h"

#include <gsl/gsl_blas.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_linalg.h>

RGSLHHLinearSolver::RGSLHHLinearSolver(QObject *parent, QString name)
    : RLinearSolver(parent, name)
{

}


RGSLHHLinearSolver::~RGSLHHLinearSolver()
{

}


QStringList RGSLHHLinearSolver::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    return actionList;
}


int RGSLHHLinearSolver::inverse(QVector< QVector<double> > *inputMatrix, QVector< QVector<double> > *inverse)
{
    // Figure out the matrix size (remember, it is square!)
    int size = inputMatrix->size();


    // Create temporary GSL matrix
    gsl_matrix *theGSLMatrix = gsl_matrix_calloc(size, size);
    gsl_matrix *matrixCopy = gsl_matrix_calloc(size, size);



    // Make matrix into a proper GSL matrix
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            gsl_matrix_set(theGSLMatrix,i,j,(*inputMatrix)[i][j]);
        }
    }


    // Take a copy of that matrix, because the matrix will be messed up when we check if it can be inverted
    gsl_matrix_memcpy(matrixCopy,theGSLMatrix);


    // Check that the deomposition is possible
    int signum = 0;
    gsl_permutation *p = gsl_permutation_alloc(size);

    int status = gsl_linalg_LU_decomp(matrixCopy, p, &signum);
    if (status) {
        qCritical() << "Error in GSL:" << gsl_strerror(status);
        return -1;
    }

    status = gsl_linalg_LU_invert(matrixCopy, p, theGSLMatrix);
    if (status) {
        qCritical() << "Error in GSL:" << gsl_strerror(status);
        return -1;
    }


    // Convert the result back from GSL to standar C++
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            (*inverse)[i][j] = gsl_matrix_get(theGSLMatrix,i,j);
        }
    }


    gsl_permutation_free(p);
    gsl_matrix_free(theGSLMatrix);
    gsl_matrix_free(matrixCopy);



    return 0;
}


int RGSLHHLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector<double> *b, QVector<double> *x)
{
    // Solve the following SOE: A*x=b for x


    // Get the size of the SOE
    int size = b->size();


    // Create temporary GSL vectors and matrices
    gsl_matrix *theAmatrix = gsl_matrix_calloc(size, size);
    gsl_vector *theBvector = gsl_vector_calloc(size);
    gsl_vector *theXvector = gsl_vector_calloc(size);


    // Make A into a proper GSL matrix
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            gsl_matrix_set(theAmatrix,i,j,(*A)[i][j]);
        }
    }

    // Make b into a proper GSL vector
    for (int i=0; i<size; i++) {
        gsl_vector_set(theBvector,i,(*b)[i]);
    }



    // Check the determinant to see if the SOE is solvable
    int signum = 0;
    gsl_permutation *p = gsl_permutation_alloc(size);
    gsl_linalg_LU_decomp(theAmatrix, p, &signum);
    double determinant = gsl_linalg_LU_det(theAmatrix,signum);

    // Solve the SOE if it is solvable
    if (determinant <= 0.0) {
        qCritical() << "Error: The coefficient matrix in the SOE is singular!";
        return -1;
    }
    else {
        gsl_linalg_LU_solve (theAmatrix, p, theBvector, theXvector);
    }


    // Convert the resulting vector back to a C++ vector
    for (int i=0; i<size; i++) {
        (*x)[i] = gsl_vector_get(theXvector,i);
    }

    gsl_permutation_free(p);
    gsl_matrix_free(theAmatrix);
    gsl_vector_free(theBvector);
    gsl_vector_free(theXvector);




    // An alternative approach for solving the system of equations:
    //gsl_linalg_HH_solve(K,F,u);



    return 0;
}


int RGSLHHLinearSolver::cholesky(QVector< QVector<double> > *inputMatrix, QVector< QVector<double> > *cholesky, QVector< QVector<double> > *inverseCholesky)
{
    // Figure out the matrix size (remember, it is square!)
    int size = inputMatrix->size();


    // Create temporary GSL matrices
    gsl_matrix *theGSLMatrix = gsl_matrix_calloc(size, size);
    gsl_matrix *matrixCopy = gsl_matrix_calloc(size, size);



    // Make matrix into a proper GSL matrix
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            gsl_matrix_set(theGSLMatrix,i,j,(*inputMatrix)[i][j]);
        }
    }


    // Take a copy of that matrix, because the matrix will be messed up when we check if it can be decomposed
    gsl_matrix_memcpy(matrixCopy,theGSLMatrix);


    // Check that the Cholesky deomposition is possible
    int signum = 0;
    gsl_permutation *p = gsl_permutation_alloc(size);

    int status = gsl_linalg_LU_decomp(matrixCopy, p, &signum);
    if (status) {
        qCritical() << "Error in GSL:" << gsl_strerror(status);
        return -1;
    }

    double determinant = gsl_linalg_LU_det(matrixCopy, signum);
    if (qAbs(determinant) < 0.00001) {
        qCritical() << "Error: The correlation matrix is not positive definite.";
        return -1;
    }


    // Perform the Cholesky decomposition
    status = gsl_linalg_cholesky_decomp(theGSLMatrix);
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


    // Finally compute the inverse
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



    // Set the upper triangle equal to zero, as it should be
    for (int i = 0; i < size; i++) {
        for (int j = i+1; j < size; j++) {
            (*cholesky)[i][j] = 0.0;
            (*inverseCholesky)[i][j] = 0.0;
        }
    }


    // Free the GSL matrix and the permutation
    gsl_permutation_free(p);
    gsl_matrix_free(theGSLMatrix);


    return 0;
}


int RGSLHHLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector< QVector<double> > *B, QVector< QVector<double> > *X)
    {
        return 0;
    }
