#ifndef RProbabilityTransformation_H
#define RProbabilityTransformation_H
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
*   - Mojtaba Mahsuli                                                *
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RMethod.h"


class RRandomVariable;
class RContinuousRandomVariable;
class RProbabilityDistributions;

class RProbabilityTransformation : public RMethod
{
	Q_OBJECT

	Q_ENUMS(RJacobianType)

public:
	RProbabilityTransformation(QObject *parent, QString name);
	virtual ~RProbabilityTransformation();

	// Declaring the RJacobianType enum values
	enum RJacobianType{u_x, x_u};

    int setRandomVariableList(QList<RRandomVariable *>* passedRandomVariableList);

    // Establishes the correlation matrix and the computes the Cholesky decomposition at the start of the analysis
    virtual int initializeAtStartOfAnalysis() = 0;

    // Transforms random variable realizations from the standard normal to the original space
    virtual int transformPointToOriginalSpace(const QVector<double> *standardNormalVector, QVector<double> *originalVector) = 0;

	// Transforms random variable realizations from the original to the standard normal space
    virtual int transformPointToStandardNormalSpace(const QVector<double> *originalVector, QVector<double> *standardNormalVector) = 0;

    // Transforms a gradient vector from the standard normal to the original space
    virtual int transformGradientToOriginalSpace(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, const QVector<double> *vectorInStandardNormalSpace, QVector<double> *vectorInOriginalSpace) = 0;

	// Transforms a gradient vector from the original to the standard normal space
    virtual int transformGradientToStandardNormalSpace(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, const QVector<double> *vectorInOriginalSpace, QVector<double> *vectorInStandardNormalSpace) = 0;

	// Computes and returns the Jacobiam matrix
    virtual int computeJacobian(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, QVector< QVector<double> > *jacobian, RProbabilityTransformation::RJacobianType jacobianType) = 0;

	// Computes and returns the Jacobian matrix of the vector of standard normal random variables wrt. the means and stdvs of original random variables
    virtual int computeSensitivityToDistributionParameters(const QVector<double> *pointInOriginalSpace, QVector< QVector<double> > *dy_dMean, QVector< QVector<double> > *dy_dStdv) = 0;

protected:
	// Auxiliary method to compute the derivative of the uncorrelated standard normal variables (z) wrt. the means and stdvs of original random variables
    int computeDzDMeanStdv(RProbabilityDistributions *theProbDistTool, RContinuousRandomVariable *randomVariable, double x, double z, double *DzDMean, double *DzDStdv);

	// Data members that are inherited by subclasses
	int randomVariablesCount;
    QList<RRandomVariable *> *randomVariableList;

private:
	
};

#endif 
