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

#include "RInHouseLinearSolver.h"
#include "RDomain.h"
#include <math.h>

RInHouseLinearSolver::RInHouseLinearSolver(QObject *parent, QString name)
    : RLinearSolver(parent, name)
{

}


RInHouseLinearSolver::~RInHouseLinearSolver()
{

}


int RInHouseLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector< QVector<double> > *B, QVector< QVector<double> > *X)
{
    qCritical() << "The matrix-right-hand-side method is not implement yet in" << this->objectName();

    return -1;
}


int RInHouseLinearSolver::solveLinearSOE(QVector< QVector<double> > *A, QVector<double> *b, QVector<double> *x)
{
    // Check dimensional consistency please...
    int size = A->size();


    // Create a copy of the A-matrix to avoid destroying it!
    QVector< QVector<double> > Atemp(size, QVector<double>(size));
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            Atemp[i][j] = (*A)[i][j];
        }
    }

    // Also create a copy of the b-vector
    QVector<double> btemp(size);
    for (int i=0; i<size; i++) {
        btemp[i] = (*b)[i];
    }


    // Conduct LU decomposition
    // (Note, this can be done once and for all, and the results stored before solving the SOE)
    QVector<int> *order = new QVector<int> (size);
    ludcmp(&Atemp, order);


    // Solve the SOE
    solvlu(&Atemp,&btemp,x,order);


    delete order;


    return 0;
}


int RInHouseLinearSolver::cholesky(QVector< QVector<double> > *inputMatrix,
                                   QVector< QVector<double> > *cholesky,
                                   QVector< QVector<double> > *inverseCholesky)
{
    // Check for null pointers
    if (inputMatrix==nullptr || cholesky==nullptr) {

        qCritical() << "The Cholesky method in" << this->objectName() << "is receiving null pointers as input";
    }


    // Check dimensional consistency
    int n = inputMatrix->size();
    if (inputMatrix->at(0).size()!=n || cholesky->size()!=n || cholesky->at(0).size()!=0) {

        qCritical() << "The Cholesky method in" << this->objectName() << "is receiving matrices with inconsistent dimensions";
    }
    if (inverseCholesky != nullptr) {
        if (inverseCholesky->size()!=n || inverseCholesky->at(0).size()!=0) {

            qCritical() << "The Cholesky method in" << this->objectName() << "is receiving matrices with inconsistent dimensions";
        }
    }

    // Calculate Cholesky decomposition
    double sum;
    for (int i=0; i<n; i++) {

        for (int j=0; j<n; j++) {

            if (i<j) {

                (*cholesky)[i][j] = 0.0;
            }
            else if (i==j) {

                sum = 0.0;
                for (int k=0; k<i; k++) {

                    sum += (*cholesky)[i][k] * (*cholesky)[i][k];
                }

                if ((*inputMatrix)[i][j] - sum > 0.0) {

                    (*cholesky)[i][j] = sqrt((*inputMatrix)[i][j] - sum);
                }
                else {

                    qCritical() << "The Cholesky decomposition failed in" << this->objectName();
                    return -1;
                }
            }
            else if (i>j) {

                sum = 0.0;
                for (int k=0; k<j; k++) {

                    sum += (*cholesky)[i][k] * (*cholesky)[j][k];
                }

                if ((*cholesky)[j][j] != 0.0) {

                    (*cholesky)[i][j] = ((*inputMatrix)[i][j] - sum) / (*cholesky)[j][j];

                }
                else {

                    qCritical() << "The Cholesky decomposition failed in" << this->objectName();
                    return -1;
                }
            }
        }
    }


    // Compute the inverse Cholesky matrix if requested
    if (inverseCholesky != nullptr) {

        for (int i=0; i<n; i++) {

            for (int j=n-1; j>=0; j--) {

                if (i<j) {

                    (*inverseCholesky)[i][j] = 0.0;
                }
                else if (i==j) {

                    if ((*cholesky)[i][j] != 0.0) {

                        (*inverseCholesky)[i][j] = 1.0 / (*cholesky)[i][j];
                    }
                    else {

                        qCritical() << "The Cholesky inversion failed in" << this->objectName();
                        return -1;
                    }
                }
                else if (i>j) {

                    sum = 0.0;
                    for (int k=j; k<i; k++) {

                        sum += (*cholesky)[i][k] * (*inverseCholesky)[k][j];
                    }

                    if ((*cholesky)[i][i] != 0.0) {

                        (*inverseCholesky)[i][j] = -sum / (*cholesky)[i][i];
                    }
                }
            }
        }
    }


    return 0;
}


int RInHouseLinearSolver::lcm(int x,int y)
{
    int t;
    while (y != 0)
    {
        t=y;
        y=x%y;
        x=t;
    }
    return x;
}


int RInHouseLinearSolver::ludcmp(QVector< QVector<double> > *a, QVector<int> *order)
{
    // This code is from Professor Loomis: http://www.johnloomis.org/ece538/notes/Matrix/ludcmp.html
    // Apparently based on Chapter 2, Programs 3-5, Fig. 2.8-2.10 of Gerald/Wheatley, APPLIED NUMERICAL ANALYSIS (fourth edition), Addison-Wesley, 1989


    // This method finds the LU decomposition of matrix by computing the lower L and upper U triangular
    // matrices equivalent to the A Matrix, such that L U = A.  These
    // matrices are returned in the space of A, in compact form.
    // The U Matrix has ones on its diagonal.  Partial pivoting is used
    // to give maximum valued elements on the diagonal of L.  The order of
    // the rows after pivoting is returned in the integer vector "order".
    // This should be used to reorder the right-hand-side vectors before
    // solving the system A x = b.
    //  a     - n by n Matrix of coefficients
    // order - integer vector holding row order after pivoting.



    int i, j, k, n, nm1;
    int flag = 1;    /* changes sign with each row interchange */
    double sum, diag;

    n = a->size();
    // /////assert(a.cols()==n);

    /* establish initial ordering in order vector */


    for (i=0; i<n; i++) (*order)[i] = i;

    /* do pivoting for first column and check for singularity */


    // Terje modified this to exit for singular matrices
    int status = pivot(a,order,0);

    if (status == 1) {

        flag = -flag;
    }
    else if (status == -1) {

        return -1;
    }

    diag = 1.0/(*a)[0][0];
    for (i=1; i<n; i++) (*a)[0][i] *= diag;



    /*
    *  Now complete the computing of L and U elements.
    *  The general plan is to compute a column of L's, then
    *  call pivot to interchange rows, and then compute
    *  a row of U's.
    */

    nm1 = n - 1;
    for (j=1; j<nm1; j++) {
        /* column of L's */
        for (i=j; i<n; i++) {
            sum = 0.0;
            for (k=0; k<j; k++) sum += (*a)[i][k]*(*a)[k][j];
            (*a)[i][j] -= sum;
        }



        /* pivot, and check for singularity */


        // Terje modified this to exit for singular matrices
        status = pivot(a,order,j);
        if (status == 1) {
            flag = -flag;
        }
        else if (status == -1) {
            return -1;
        }



        /* row of U's */
        diag = 1.0/(*a)[j][j];
        for (k=j+1; k<n; k++) {
            sum = 0.0;
            for (i=0; i<j; i++) sum += (*a)[j][i]*(*a)[i][k];
            (*a)[j][k] = ((*a)[j][k]-sum)*diag;
        }
    }

    /* still need to get last element in L Matrix */

    sum = 0.0;
    for (k=0; k<nm1; k++) sum += (*a)[nm1][k]*(*a)[k][nm1];
    (*a)[nm1][nm1] -= sum;

    return flag;
}


void RInHouseLinearSolver::solvlu(const QVector< QVector<double> > *a, const QVector<double> *b, QVector<double> *x, const QVector<int> *order)
{
    // This code is from Professor Loomis: http://www.johnloomis.org/ece538/notes/Matrix/ludcmp.html
    // Apparently based on Chapter 2, Programs 3-5, Fig. 2.8-2.10 of Gerald/Wheatley, APPLIED NUMERICAL ANALYSIS (fourth edition), Addison-Wesley, 1989

    // This method finds the solution to a system of equations,
    // A x = b, after LU decomposition of A has been found.
    // Within this routine, the elements of b are rearranged in the same way
    // that the rows of a were interchanged, using the order vector.
    // The solution is returned in x.
    //  a     - the LU decomposition of the original coefficient Matrix.
    //  b     - the vector of right-hand sides
    //  x     - the solution vector
    //  order - integer array of row order as arranged during pivoting

    int i,j,n;
    double sum;
    n = a->size();


    /* rearrange the elements of the b vector. x is used to hold them. */

    for (i=0; i<n; i++) {
        j = (*order)[i];
        (*x)[i] = (*b)[j];
    }


    /* do forward substitution, replacing x vector. */

    (*x)[0] /= (*a)[0][0];
    for (i=1; i<n; i++) {
        sum = 0.0;
        for (j=0; j<i; j++) sum += (*a)[i][j]*(*x)[j];
        (*x)[i] = ((*x)[i]-sum)/(*a)[i][i];
    }

    /* now get the solution vector, x[n-1] is already done */

    for (i=n-2; i>=0; i--) {
        sum = 0.0;
        for (j=i+1; j<n; j++) sum += (*a)[i][j] * (*x)[j];
        (*x)[i] -= sum;
    }
}


int RInHouseLinearSolver::pivot(QVector< QVector<double> > *a, QVector<int> *order, int jcol)
{
    // This code is from Professor Loomis: http://www.johnloomis.org/ece538/notes/Matrix/ludcmp.html
    // Apparently based on Chapter 2, Programs 3-5, Fig. 2.8-2.10 of Gerald/Wheatley, APPLIED NUMERICAL ANALYSIS (fourth edition), Addison-Wesley, 1989

    // This method finds the pivot element, i.e., the largest element for a pivot in "jcol"
    // of Matrix "a", performs interchanges of the appropriate
    // rows in "a", and also interchanges the corresponding elements in
    // the order vector.
    // a      -  n by n Matrix of coefficients
    // order  - integer vector to hold row ordering
    // jcol   - column of "a" being searched for pivot element


    double TINY = 1e-20;
    int i, ipvt,n;
    double big, anext;
    n = a->size();

    /*
    *  Find biggest element on or below diagonal.
    *  This will be the pivot row.
    */

    ipvt = jcol;



    big = fabs((*a)[ipvt][ipvt]);
    for (i = ipvt+1; i<n; i++) {
        anext = fabs((*a)[i][jcol]);
        if (anext>big) {
            big = anext;
            ipvt = i;
        }
    }

    // Terje modified this to exit for singular matrices
    if(fabs(big)>TINY) {
        // Ok
    }
    else {
        qCritical() << "The in-house linear solver encountered a coefficient matrix that is SINGULAR.";
        return -1;
        // otherwise Matrix is singular
    }

    /*
    *   Interchange pivot row (ipvt) with current row (jcol).
    */

    if (ipvt==jcol) return 0;


    // Swap row jcol with row ipvt
    // a.swaprows(jcol,ipvt);
    QVector<double> temp(n);
    temp = (*a)[jcol];
    (*a)[jcol] = (*a)[ipvt];
    (*a)[ipvt] = temp;



    i = (*order)[jcol];
    (*order)[jcol] = (*order)[ipvt];
    (*order)[ipvt] = i;
    return 1;
}
