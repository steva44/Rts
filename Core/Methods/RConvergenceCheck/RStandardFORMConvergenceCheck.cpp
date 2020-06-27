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

#include "RStandardFORMConvergenceCheck.h"
#include "RDomain.h"
#include "RMatrixOperations.h"

#include <math.h>

RStandardFORMConvergenceCheck::RStandardFORMConvergenceCheck(QObject *parent, QString name)
	: RConvergenceCheck(parent, name)
{
    theTolerance1 = 0.001;
    theTolerance2 = 0.001;
	theScaleValue = 0;
}


RStandardFORMConvergenceCheck::~RStandardFORMConvergenceCheck()
{

}


double RStandardFORMConvergenceCheck::getTolerance1()
{
    return theTolerance1;
}


void RStandardFORMConvergenceCheck::setTolerance1(double value)
{
    theTolerance1 = value;
}


double RStandardFORMConvergenceCheck::getTolerance2()
{
    return theTolerance2;
}


void RStandardFORMConvergenceCheck::setTolerance2(double value)
{
    theTolerance2 = value;
}


double RStandardFORMConvergenceCheck::getScaleValue()
{
	return theScaleValue;
}


void RStandardFORMConvergenceCheck::setScaleValue(double value)
{
	theScaleValue = value;
}


int RStandardFORMConvergenceCheck::check(QVector<double> const *u, double functionValue, QVector<double> const *gradientInStandardNormalSpace)
{
    // Check that the caller is of a sound mind
    if (u==nullptr) {
        qCritical() << this->objectName() << "cannot check convergence with the call made.";
    }

    // Create a matrix operations tool
    RMatrixOperations theMatrixOperations(theDomain);

	if (theScaleValue == 0.0) {
		qCritical() << "Error: Scale value has not been set in the convergence checker" << objectName();
		return -1;
	}

	// Cerate the alpha-vector
    QVector<double> *alpha = new QVector<double>(gradientInStandardNormalSpace->size());

	// Compute the norm of the gradient
    double gradientNorm = theMatrixOperations.norm(gradientInStandardNormalSpace);

	// Check that the norm is not zero
	if (gradientNorm == 0.0) {
		qCritical() << "Error: The norm of the entered gradient in direction searcher is zero.";
		return -1;
	}
	// Compute alpha-vector using BLAS: alpha = gradientInStandardNormalSpace * (-1 / gradientNorm)
    for (int i=0; i<alpha->size(); i++) {
        (*alpha)[i] = -gradientInStandardNormalSpace->at(i)/gradientNorm;
    }


	// The scaling factor
    double temp0 = theMatrixOperations.norm(u);

	if (temp0 < 1.0) {
		temp0 = 1.0; 
	}

	// Compute scaled u-vector using BLAS: u_scaled = (1 / temp0) * u
    QVector<double> *u_scaled = new QVector<double>(u->size());
    for (int i=0; i<u->size(); i++) {
        (*u_scaled)[i] = u->at(i)/temp0;
    }


	// First convergence criteria
    double criterium1 = qAbs(functionValue / theScaleValue);

	// Second convergence criteria
	// Compute the u_scaled.alpha using BLAS: u_scaled_times_alpha = u_scaled * alpha
    double u_scaled_times_alpha = 0.0;
    for (int i=0; i<alpha->size(); i++) {
        u_scaled_times_alpha += u_scaled->at(i)*alpha->at(i);
    }


    // Compute: u_scaled = u_scaled - u_scaled_times_alpha * alpha;
    for (int i=0; i<alpha->size(); i++) {
        (*u_scaled)[i] = u_scaled->at(i) - u_scaled_times_alpha * alpha->at(i);
    }

    double criterium2 = theMatrixOperations.norm(u_scaled);


	// Inform user about convergence status 
    double anotherTemp = theMatrixOperations.norm(u);

    if (theOutputLevel >= RObject::Medium) {
        qDebug() << "Check1 =" << criterium1 << ", Check2 =" << criterium2 << ", Distance =" << anotherTemp;
        qDebug() << "";
	}

	// Delete the allocated vectors
	if (alpha) {
        delete alpha;
        alpha = nullptr;
	}
	if (u_scaled) {
        delete u_scaled;
        u_scaled = nullptr;
	}

	// Return '1' if the analysis converged ('-1' otherwise)
    if ((criterium1 < theTolerance1) && (criterium2 < theTolerance2)) {
		return 1;
	}
	else {
		return -1;
	}
}
