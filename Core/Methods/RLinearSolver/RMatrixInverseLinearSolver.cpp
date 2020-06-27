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

#include "RMatrixInverseLinearSolver.h"
#include "RDomain.h"
#include "RMatrixOperations.h"

#include <math.h>

RMatrixInverseLinearSolver::RMatrixInverseLinearSolver(QObject *parent, QString name)
    : RLinearSolver(parent, name)
{

}


RMatrixInverseLinearSolver::~RMatrixInverseLinearSolver()
{

}


int RMatrixInverseLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector<double> *b, QVector<double> *x)
{    
    // Determine system size
    int n = A->size();


    // Calculate the determinant of the entire input matrix
    double fullDet = determinant(A);


    // The solution COULD now be calculated by first calculating the inverse of A, using
    // using the private method of this class, followed by the multiplication x = inverse(A) * b


    // Inverse determinant
    double oneOverFullDet = 0.0;
    if (fullDet != 0) {
        oneOverFullDet = 1.0 / fullDet;
    }
    else {
        qCritical() << "Error: Zero determinant in" << this->objectName();
    }


    // Loop over number of entries in the x-vector
    QVector< QVector<double> > auxA(n, QVector<double>(n));
    for (int k=0; k<n; k++) {

        // Establish Ak, namely A with column number k replaced by b
        for (int i=0; i<n; i++) {

            for (int j=0; j<n; j++) {

                if (j==k) {

                    auxA[i][j] = (*b)[i];
                }
                else {

                    auxA[i][j] = (*A)[i][j];
                }
            }
        }


        // Calculate the determinant of Ak
        double detAk = determinant(&auxA);


        // Calculate x-value
        (*x)[k] = detAk * oneOverFullDet;
    }

    return 0;
}


int RMatrixInverseLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector< QVector<double> > *B, QVector< QVector<double> > *X)
{
    // In the approach implemented in this class it is not particulary efficient to solve the
    // equation system once for many right-hand sides, i.e., for b-vectors collected in the
    // matrix B. This is nonetheless implemented here in case the method is called.


    // Determine number of equations, n, and number of right-hand sides, m
    int n = A->size();
    int m = B->at(0).size();


    // Check that the dimension of X matches
    if (X->size() != n || X->at(0).size() != m) {

        qCritical() << "Error: Wrong size of X-matrix given to" << this->objectName();
    }


    // Declare local variables
    QVector<double> b(n);
    QVector<double> x(n);


    // Loop over all right-hand sides, solving the SOE anew each time
    for (int i=0; i<m; i++) {


        // Pick up the right-hand side, b, from the matrix B
        b = B->at(i);


        // Solve the SOE for that right-hand side
        solveLinearSOE(A, &b, &x);


        // Store the result in the output X-matrix
        (*X)[i] = x;
    }

    return 0;
}


double RMatrixInverseLinearSolver::cofactor(QVector< QVector<double> > *input, int i, int j)
{
    // Determine the size of the input matrix
    int n = input->size();

    // Create an auxiliary matrix one size smaller
    QVector< QVector<double> > reduced(n-1, QVector<double>(n-1));

    // Get ready to form the reduced matrix
    int smallRow = 0;
    int smallColumn;

    // Loop over the rows of the bigger matrix
    for (int bigRow=0; bigRow<n; bigRow++) {


        // Skip the i'th row
        if (bigRow != i) {


            // Reset the column counter for the smaller matrix
            smallColumn = 0;


            // Loop over the columns of the bigger matrix
            for (int bigColumn=0; bigColumn<n; bigColumn++) {


                // Skip the j'th column
                if (bigColumn != j) {

                    reduced[smallRow][smallColumn] = (*input)[bigRow][bigColumn];
                    smallColumn++;
                }
            }

            smallRow++;
        }
    }


    // Calculate cofactor but stop when the reduced matrix is 1-by-1
    double result;
    if (reduced.size() > 1) {
        result = pow(-1.0,i+j) * determinant(&reduced);
    }
    else {
        result = pow(-1.0,i+j) * reduced[0][0];
    }

    return result;
}


double RMatrixInverseLinearSolver::determinant(QVector< QVector<double> > *input)
{
    // Determine the size of the input matrix
    int n = input->size();


    // Loop along first row
    double det = 0;
    for (int j=0;j<n;j++) {


        // The determinant is the current matrix component times its cofactor
        det += (*input)[0][j] * cofactor(input, 0, j);

    }

    return det;
}


int RMatrixInverseLinearSolver::inverse(QVector< QVector<double> > *input, QVector< QVector<double> > *inverse)
{
    // Determine matrix size
    int n = input->size();


    // Calculate the determinant of the entire input matrix
    double fullDet = determinant(input);


    // Take its inverse
    double oneOverFullDet = 0.0;
    if (fullDet != 0) {
        oneOverFullDet = 1.0 / fullDet;
    }
    else {
        qCritical() << "Error: Zero determinant in" << this->objectName();
    }


    // Calculate cofactors, divide by full determinant, and insert the result in transpose positions
    for (int i=0; i<n; i++) {

        for (int j=0; j<n; j++) {

            (*inverse)[j][i] = oneOverFullDet * cofactor(input, i, j);
        }
    }

    return 0;
}


double determinantAlternative(QVector< QVector<double> > *A)
{
    // Determine the size of the input matrix
    int n = A->size();


    // Create an auxiliary matrix one size smaller
    QVector< QVector<double> > aux(n-1, QVector<double>(n-1));


    // Initial declarations
    double det = 0;
    int m;


    // Loop over columns, walking along first row
    for (int k=0;k<n;k++) {

        // Loop over the rows below the first, to build the reduced matrix
        for (int i=1;i<n;i++) {

            m = 0;

            // Loop over all columns, to build the reduced matrix
            for (int j=0;j<n;j++) {

                // Skip the k'th column it's the one we calculate the minor for
                if (j != k) {

                    // Pick up contribution to the reduced matrix
                    aux[i-1][m] = (*A)[i][j];
                    m++;
                }
            }
        }

        // Calculate the determinant by multiplying the matrix element by its cofactor
        det += pow(-1.0,k+2.0) * (*A)[0][k] * determinantAlternative(&aux);

    }

    return det;
}

