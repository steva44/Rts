#ifndef RNatafProbabilityTransformation_H
#define RNatafProbabilityTransformation_H
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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RProbabilityTransformation.h"
#include "RRandomVariable.h"
#include "RCholeskyDecomposition.h"
#include "RProbabilityDistributions.h"

#include <memory>

class RNatafProbabilityTransformation : public RProbabilityTransformation
{
    Q_OBJECT
    Q_PROPERTY(QObject *ProbabilityDistributions READ getProbabilityDistributions WRITE setProbabilityDistributions)
    Q_PROPERTY(QObject *CholeskyDecomposition READ getCholeskyDecomposition WRITE setCholeskyDecomposition)

public:
    RNatafProbabilityTransformation(QObject *parent, QString &name);
    ~RNatafProbabilityTransformation();

    QObject *getProbabilityDistributions() const;
    void setProbabilityDistributions(QObject *value);
    QObject *getCholeskyDecomposition() const;
    void setCholeskyDecomposition(QObject *value);

    // Establishes the correlation matrix and the computes the Cholesky decomposition at the start of the analysis
    int initializeAtStartOfAnalysis();

    // Transforms random variable realizations from the standard normal to the original space
    int transformPointToOriginalSpace(const QVector<double> *standardNormalVector, QVector<double> *originalVector);

    // Transforms random variable realizations from the original to the standard normal space
    int transformPointToStandardNormalSpace(const QVector<double> *originalVector, QVector<double> *standardNormalVector);

    // Transforms a gradient vector from the standard normal to the original space
    int transformGradientToOriginalSpace(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, const QVector<double> *vectorInStandardNormalSpace, QVector<double> *vectorInOriginalSpace);

    // Transforms a gradient vector from the original to the standard normal space
    int transformGradientToStandardNormalSpace(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, const QVector<double> *vectorInOriginalSpace, QVector<double> *vectorInStandardNormalSpace);

    // Computes and returns the Jacobiam matrix
    int computeJacobian(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, QVector< QVector<double> > *jacobian, RProbabilityTransformation::RJacobianType jacobianType);

    // Computes and returns the Jacobian matrix of the vector of standard normal random variables wrt. the means and stdvs of original random variables
    int computeSensitivityToDistributionParameters(const QVector<double> *pointInOriginalSpace, QVector< QVector<double> > *dy_dMean, QVector< QVector<double> > *dy_dStdv);

private:
    // Auxiliary method to transform the vector of original random variables (x) to uncorelated standard normal random variables (z)
    int xToz(const QVector<double> *x, QVector<double> *z);

    // Auxiliary method to transform the vector of corelated standard normal random variables (y) to uncorelated standard normal random variables (z)
    int yToz(const QVector<double> *y, QVector<double> *z);

    // Auxiliary method to establish the modified correlation matrix; the optional argument will perturb one of the random variables, which results in a purturbed correlation matrix that is used in computing sensitivities
    int establishCorrelationMatrix(QVector< QVector<double> > *R0, RRandomVariable *perturbedRV = nullptr);

    // Auxiliary methods to compute the modified correlation coefficients
    double phi2(double z_i, double z_j, double rho);
    double integrand(RRandomVariable *rv_i, double z_i, double mean_i, double stdv_i, RRandomVariable *rv_j, double z_j, double mean_j, double stdv_j, double rho_zero, bool evaluateDerivative);
    double doubleIntegral(RRandomVariable *rv_i, double mean_i, double stdv_i, RRandomVariable *rv_j, double mean_j, double stdv_j, double rho, bool evaluateDerivative);
    void residualFunction(double rho_original, double rho, RRandomVariable *rv_i, double mean_i, double stdv_i, RRandomVariable *rv_j, double mean_j, double stdv_j, double *f, double *df);
    double modifyCorrelation(RRandomVariable *rv_i, RRandomVariable *rv_j, double rho_original);

    // Data members for meta-properties
    QPointer<RCholeskyDecomposition> theCholeskyDecomposition;
    QPointer<RProbabilityDistributions> theProbabilityDistributions;

    // Data member that stores the number of random variables
    int numRVs;

    // Data member that stores the correlation matrix
    std::unique_ptr<QVector<QVector<double>>> R0 = nullptr;

    // Data member that stores the Cholesky decomposition of the correlation matrix
    std::unique_ptr<QVector<QVector<double>>> L = nullptr;

    // Data member that stores the inverse Cholesky decomposition of the correlation matrix
    std::unique_ptr<QVector<QVector<double>>> Linv = nullptr;

};

#endif
