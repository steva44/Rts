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

#include "RGradientNormConvergenceCheck.h"
#include "RDomain.h"
#include "RMatrixOperations.h"

#include <math.h>

RGradientNormConvergenceCheck::RGradientNormConvergenceCheck(QObject *parent, QString name)
    : RConvergenceCheck(parent, name)
{
    theTolerance = 0.001;
    theScaleValue = 0;
}


RGradientNormConvergenceCheck::~RGradientNormConvergenceCheck()
{

}


double RGradientNormConvergenceCheck::getTolerance()
{
    return theTolerance;
}


void RGradientNormConvergenceCheck::setTolerance(double value)
{
    theTolerance = value;
}


double RGradientNormConvergenceCheck::getScaleValue()
{
    return theScaleValue;
}


void RGradientNormConvergenceCheck::setScaleValue(double value)
{
    theScaleValue = value;
}


int RGradientNormConvergenceCheck::check(QVector<double> const *point, double function, QVector<double> const *gradient)
{
    // In this check we only need/use the gradient, because we simply check that its norm is near zero

    if (gradient==nullptr) {
        qCritical() << this->objectName() << "did not get the gradient vector needed to check convergence.";
        return -1;
    }
    else {
        RMatrixOperations theMatrixOperations(theDomain);
        double norm = theMatrixOperations.norm(gradient);

        if (theOutputLevel == RDomain::Maximum) {
            qDebug() << "The norm in" << this->objectName() << "is" << norm;
        }

        if (norm < theTolerance) {
            // Converged
            return 1;
        }
        else {
            // Did not converge
            return -1;
        }
    }
}






