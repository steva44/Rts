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

#include "RCholeskyLinearSolver.h"
#include "RCholeskyDecomposition.h"
#include "RDomain.h"

#include <math.h>

RCholeskyLinearSolver::RCholeskyLinearSolver(QObject *parent, QString name)
    : RLinearSolver(parent, name)
{
    theCholeskyDecomposition = nullptr;
}


RCholeskyLinearSolver::~RCholeskyLinearSolver()
{

}


QObject * RCholeskyLinearSolver::getCholeskyDecomposition() const
{
    return theCholeskyDecomposition;
}


void RCholeskyLinearSolver::setCholeskyDecomposition(QObject *value)
{
    theCholeskyDecomposition = qobject_cast<RCholeskyDecomposition *>(value);
}


int RCholeskyLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector<double> *b, QVector<double> *x)
{
    // Establish equation size
    int n = A->size();


    // Prepare storage for the lower-triangular Cholesky matrix
    QVector< QVector<double> > L(n, QVector<double>(n));


    // Obtain the Cholesky decomposition of A
    if (theCholeskyDecomposition) {

        int result = theCholeskyDecomposition->getCholesky(A, &L, nullptr);

        if(result != 0)
        {
            qDebug()<<"Error in Cholesky Linear Solver: "<<this->objectName();
            return -1;
        }
    }
    else {
        qCritical() << this->objectName() << "needs a Cholesky decomposition";
    }


    // Prepare storage for the auxiliary vector y
    QVector<double> y(n);


    // Carry out forward substitution
    forwardSubstitution(&L, b, &y);


    // Carry out backward substitution
    backwardSubstitution(true, &L, &y, x);

    return 0;
}


int RCholeskyLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector< QVector<double> > *B, QVector< QVector<double> > *X)
{
    // Establish equation size
    int n = A->size();


    // Prepare storage for the lower-triangular Cholesky matrix
    QVector< QVector<double> > L(n, QVector<double>(n));


    // Obtain the Cholesky decomposition of A
    if (theCholeskyDecomposition) {

        theCholeskyDecomposition->getCholesky(A, &L, nullptr);
    }
    else {
        qCritical() << this->objectName() << "needs a Cholesky decomposition";
    }


    // Prepare storage for a few auxiliary vectors
    QVector<double> y(n);
    QVector<double> b(n);
    QVector<double> x(n);


    // Determine the number of right-hand-sides
    int numRHS = B->at(0).size();


    // Loop over all right-hand-sides
    for (int i=0; i<numRHS; i++) {


        // Pick up the correct right-hand-side vector
        for (int j=0; j<n; j++) {

            b[j] = (*B)[j][i];
        }


        // Carry out forward substitution
        forwardSubstitution(&L, &b, &y);


        // Carry out backward substitution
        backwardSubstitution(true, &L, &y, &x);


        // Put that solution x into the output matrix X
        for (int j=0; j<n; j++) {

            (*X)[j][i] = x.at(j);
        }
    }

    return 0;
}

