#ifndef RInHouseLinearSolver_H
#define RInHouseLinearSolver_H
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

class RInHouseLinearSolver : public RLinearSolver
{
	Q_OBJECT

public:
    RInHouseLinearSolver(QObject *parent, QString name);
    ~RInHouseLinearSolver();

    int solveLinearSOE(QVector< QVector<double> > *A, QVector<double> *b, QVector<double> *x);

    int cholesky(QVector< QVector<double> > *inputMatrix, QVector< QVector<double> > *cholesky, QVector< QVector<double> > *inverseCholesky);

    int solveLinearSOE(QVector< QVector<double> > *A, QVector< QVector<double> > *B, QVector< QVector<double> > *X);

private:

    int ludcmp(QVector< QVector<double> > *a, QVector<int> *order);
    void solvlu(const QVector< QVector<double> > *a, const QVector<double> *b, QVector<double> *x, const QVector<int> *order);
    static int pivot(QVector< QVector<double> > *a, QVector<int> *order, int jcol);
    int lcm(int x,int y);
};

#endif
