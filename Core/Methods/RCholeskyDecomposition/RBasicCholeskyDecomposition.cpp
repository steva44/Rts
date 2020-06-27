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

#include "RBasicCholeskyDecomposition.h"
#include "RDomain.h"

#include <math.h>

RBasicCholeskyDecomposition::RBasicCholeskyDecomposition(QObject *parent, QString name)
    : RCholeskyDecomposition(parent, name)
{

}


RBasicCholeskyDecomposition::~RBasicCholeskyDecomposition()
{

}


int RBasicCholeskyDecomposition::getCholesky(QVector< QVector<double> > *inputMatrix, QVector< QVector<double> > *cholesky, QVector< QVector<double> > *inverseCholesky)
{
    // Check pointers and dimensional consistency
    int result = checkPointersAndDimensions(inputMatrix, cholesky, inverseCholesky);
    if (result < 0) {
        qCritical() << "Error in the Cholesky decomposition in" << this->objectName();
        return -1;
    }

    
    // Get matrix size
    int n = inputMatrix->size();


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
                    
                    qCritical() << "The Cholesky decomposition failed at operation 1 in" << this->objectName();
                    qDebug()<<(*inputMatrix)[i][j] - sum;


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
                    
                    qCritical() << "The Cholesky decomposition failed at operation 2 in" << this->objectName();
                    return -1;
                }
            }
        }
    }
    
    
    // Compute the inverse Cholesky matrix if requested
    if (inverseCholesky != nullptr)
    {
        
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
                    else {

                        qCritical() << "The Cholesky inversion failed in" << this->objectName();
                        return -1;
                    }
                }
            }
        }
    }
    
    return 0;
}

