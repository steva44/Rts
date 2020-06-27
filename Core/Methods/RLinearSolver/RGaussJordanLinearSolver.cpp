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

#include "RGaussJordanLinearSolver.h"
#include "RDomain.h"
#include <math.h>

RGaussJordanLinearSolver::RGaussJordanLinearSolver(QObject *parent, QString name)
    : RLinearSolver(parent, name)
{

}


RGaussJordanLinearSolver::~RGaussJordanLinearSolver()
{

}


int RGaussJordanLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector<double> *b, QVector<double> *x)
{
    //***************************
    int maximum = A->size();

    //QVector< QVector<double> > *a = new QVector< QVector<double> >(r,QVector<double>(c));
    //****************************

    QVector< QVector<double> > augmentedmatrix(maximum,QVector<double>(2*maximum));

    /* 2D array declared to store augmented matrix */
    double temporary, r;
    int i, j, k, temp; /* counter variables for loops */
    double minvalue = 0.0005;


    int dimension = maximum;

    /*   storing augmented matrix as a matrix of dimension
         (dimension)x(2*dimension) in 2D array  */

    for (i = 0; i < dimension; i++)
        for (j = 0; j < dimension; j++)
            augmentedmatrix[i][j] = (*A)[i][j];


    /* augmenting with identity matrix of similar dimensions */
    for (i = 0; i < dimension; i++)
        for (j = dimension; j < 2 * dimension; j++)
            if (i == j % dimension)
                augmentedmatrix[i][j] = 1;
            else
                augmentedmatrix[i][j] = 0;


    /* using gauss-jordan elimination */
    for (j = 0; j < dimension; j++) {
        temp = j;

        /* finding maximum jth column element in last (dimension-j) rows */
        for (i = j + 1; i < dimension; i++)
            if (augmentedmatrix[i][j] > augmentedmatrix[temp][j])
                temp = i;

        if (fabs(augmentedmatrix[temp][j]) < minvalue) {
            qDebug() << "Elements are too small to deal with, with j=" << j << "and temp=" << temp << "with augmentedmatrix[temp][j])=" << augmentedmatrix[temp][j];
            theDomain->print(&augmentedmatrix);
            return -1;
        }

        /* swapping row which has maximum jth column element */
        if (temp != j)
            for (k = 0; k < 2 * dimension; k++) {
                temporary = augmentedmatrix[j][k];
                augmentedmatrix[j][k] = augmentedmatrix[temp][k];
                augmentedmatrix[temp][k] = temporary;
            }

        /* performing row operations to form required identity matrix out of the input matrix */
        for (i = 0; i < dimension; i++)
            if (i != j) {
                r = augmentedmatrix[i][j];
                for (k = 0; k < 2 * dimension; k++)
                    augmentedmatrix[i][k] -= augmentedmatrix[j][k] * r / augmentedmatrix[j][j];
            } else {
                r = augmentedmatrix[i][j];
                for (k = 0; k < 2 * dimension; k++)
                    augmentedmatrix[i][k] /= r;
            }
    }

    /* Display augmented matrix */
    //printf("\n After Gauss-Jordan elimination, augmented matrix is : \n\n");
    /*
    for (i = 0; i < dimension; i++) {
        for (j = 0; j < 2 * dimension; j++)
            printf("  %4.2f", augmentedmatrix[i][j]);
        printf("\n");
    }
    */

    /* displaying inverse of the non-singular matrix */
    //printf("\n\n\n The inverse of the entered non-singular matrix is : \n\n");

    for (i = 0; i < dimension; i++) {
        for (j = dimension; j < 2 * dimension; j++) {

            // Terje commented out for now:
            // (*inverse)[i][j-dimension] = augmentedmatrix[i][j];
        }
    }

    return 0;
}


int RGaussJordanLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector< QVector<double> > *B, QVector< QVector<double> > *X)
{
    qCritical() << "The matrix-right-hand-side method is not implement yet in" << this->objectName();

    return -1;
}

