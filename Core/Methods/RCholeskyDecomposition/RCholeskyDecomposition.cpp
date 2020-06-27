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

#include "RCholeskyDecomposition.h"
#include "RMethod.h"

RCholeskyDecomposition::RCholeskyDecomposition(QObject *parent, QString name)
    : RMethod(parent, name)
{

}


RCholeskyDecomposition::~RCholeskyDecomposition()
{

}


int RCholeskyDecomposition::checkPointersAndDimensions(QVector< QVector<double> > *inputMatrix, QVector< QVector<double> > *cholesky, QVector< QVector<double> > *inverseCholesky)
{
    // Check for null pointers and check dimensional consistency
    if (inputMatrix==nullptr || cholesky==nullptr) {
        qCritical() << this->objectName() << "is receiving null pointers as input";
        return -1;
    }

    int n = inputMatrix->size();

    if (inputMatrix->at(0).size()!=n || cholesky->size()!=n || cholesky->at(0).size()!=n) {
        qCritical() << this->objectName() << "is receiving matrices with inconsistent dimensions.";
        return -1;
    }

    if (inverseCholesky != nullptr) {
        if (inverseCholesky->size()!=n || inverseCholesky->at(0).size()!=n) {
            qCritical() << this->objectName() << "is receiving matrices with inconsistent dimensions...";
            return -1;
        }
    }

    return 1;
}
