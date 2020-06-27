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

#include "RQRLinearSolver.h"
#include "RDomain.h"
#include <math.h>

RQRLinearSolver::RQRLinearSolver(QObject *parent, QString name)
    : RLinearSolver(parent, name)
{

}


RQRLinearSolver::~RQRLinearSolver()
{

}


int RQRLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector<double> *b, QVector<double> *x)
{
    qCritical() << this->objectName() << "does not yet have any implementations";

    return -1;
}


int RQRLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector< QVector<double> > *B, QVector< QVector<double> > *X)
{
    qCritical() << "The matrix-right-hand-side method is not implement yet in" << this->objectName();

    return -1;
}


