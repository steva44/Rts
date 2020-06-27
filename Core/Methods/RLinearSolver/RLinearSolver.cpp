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

#include "RLinearSolver.h"
#include "RMethod.h"

RLinearSolver::RLinearSolver(QObject *parent, QString name)
    : RMethod(parent, name)
{

}


RLinearSolver::~RLinearSolver()
{

}


int RLinearSolver::forwardSubstitution(QVector< QVector<double> > *L, QVector<double> *b, QVector<double> *y)
{
    // Determine system size
    int n = L->size();


    // Rough check that L is actually lower-triangular
    if ((*L)[0][n-1]) {
        qCritical() << this->objectName() << "is supposed to get a lower-triangular matrix";
    }


    // Forward substitution algorithm
    double sum;
    for (int i=0; i<n; i++) {

        sum = 0.0;
        for (int j=0; j<i; j++) {

            sum += (*L)[i][j] * (*y)[j];
        }

        if ((*L)[i][i] != 0) {

            (*y)[i] = ((*b)[i] - sum) / (*L)[i][i];
        }
        else {
            qCritical() << "Error in" << this->objectName();
        }
    }

    return 0;
}


int RLinearSolver::backwardSubstitution(bool lowerTriangular, QVector< QVector<double> > *U, QVector<double> *y, QVector<double> *x)
{
    // Determine system size
    int n = U->size();


    // Rough check that U is actually upper-triangular
    if ((*U)[n-1][0]) {
        qCritical() << this->objectName() << "is supposed to get an upper-triangular matrix";
    }


    // Backward substitution algorithm
    double sum;
    for (int i=n-1; i>=0; i--) {

        sum = 0.0;
        for (int j=i+1; j<n; j++) {

            if (lowerTriangular) {
                sum += (*U)[j][i] * (*x)[j];
            }
            else {
                sum += (*U)[i][j] * (*x)[j];
            }
        }

        if ((*U)[i][i] != 0) {

            (*x)[i] = ((*y)[i] - sum) / (*U)[i][i];
        }
        else {
            qCritical() << "Error in" << this->objectName();
        }
    }

    return 0;
}


