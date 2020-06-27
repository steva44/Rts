#ifndef RGSLProbabilityDistributions_H
#define RGSLProbabilityDistributions_H
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

#include "RProbabilityDistributions.h"

#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_integration.h>

class RGSLProbabilityDistributions : public RProbabilityDistributions
{
	Q_OBJECT

public:
    RGSLProbabilityDistributions(QObject *parent, QString name);
    ~RGSLProbabilityDistributions();

    // Methods for the Normal distribution
    double getNormalPDF(double x, double sigma);
    double getNormalCDF(double x, double sigma);
    double getInverseNormalCDF(double x, double sigma);

    double generateStandardNormalNumber();
    double generateBinomialNumber(double p, int n);
    double gammaFunction(double x);
    double generateExponentialNumber(double mu);

    // Methods for the Exponential distribution:
    double getExponentialPDF(double x, double mu);
    double getExponentialCDF(double x, double mu);
    double getInverseExponentialCDF(double x, double mu);

    // Methods for the Laplace distribution:
    double getLaplacePDF(double x, double a);
    double getLaplaceCDF(double x, double a);
    double getInverseLaplaceCDF(double x, double a);

    // Methods for the Rayleigh distribution:
    double getRayleighPDF(double x, double sigma);
    double getRayleighCDF(double x, double sigma);
    double getInverseRayleighCDF(double x, double sigma);

    // Methods for the Gamma distribution:
    double getGammaPDF(double x, double a, double b);
    double getGammaCDF(double x, double a, double b);
    double getInverseGammaCDF(double x, double a, double b);

    // Methods for the Uniform distribution:
    double getUniformPDF(double x, double a, double b);
    double getUniformCDF(double x, double a, double b);
    double getInverseUniformCDF(double x, double a, double b);

    // Methods for the Lognormal distribution:
    double getLognormalPDF(double x, double zeta, double sigma);
    double getLognormalCDF(double x, double zeta, double sigma);
    double getInverseLognormalCDF(double x, double zeta, double sigma);

    // Methods for the Chi-squared distribution:
    double getChiSquaredPDF(double x, double nu);
    double getChiSquaredCDF(double x, double nu);
    double getInverseChiSquaredCDF(double x, double nu);

    // Methods for the t-distribution distribution:
    double getTDistributionPDF(double x, double nu);
    double getTDistributionCDF(double x, double nu);
    double getInverseTDistributionCDF(double x, double nu);

    // Methods for the Beta distribution:
    double getBetaPDF(double x, double a, double b);
    double getBetaCDF(double x, double a, double b);
    double getInverseBetaCDF(double x, double a, double b);

    // Methods for the Logistic distribution:
    double getLogisticPDF(double x, double a);
    double getLogisticCDF(double x, double a);
    double getInverseLogisticCDF(double x, double a);

    // Methods for the Weibull distribution:
    double getWeibullPDF(double x, double a, double b);
    double getWeibullCDF(double x, double a, double b);
    double getInverseWeibullCDF(double x, double a, double b);

    // Methods for the Gumbel distribution:
    double getGumbelPDF(double x, double a, double b);
    double getGumbelCDF(double x, double a, double b);
    double getInverseGumbelCDF(double x, double a, double b);

    QStringList getActionList();

private:

    // Random number generation
    gsl_rng *theRandomNumberGenerator;
    int resetRandomNumberGenerator();

};

#endif // RGSLProbabilityDistributions_H
