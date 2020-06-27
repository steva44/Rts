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

#include "RGSLProbabilityDistributions.h"
#include "RDomain.h"
#include "RMainWindow.h"

#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_integration.h>

#include <math.h>

RGSLProbabilityDistributions::RGSLProbabilityDistributions(QObject *parent, QString name)
    : RProbabilityDistributions(parent, name)
{
    // Create a random number generator
    gsl_rng_env_setup();
    theRandomNumberGenerator = gsl_rng_alloc(gsl_rng_default);
    gsl_rng_set(theRandomNumberGenerator, 18);

}


RGSLProbabilityDistributions::~RGSLProbabilityDistributions()
{
    if (theRandomNumberGenerator) {
        gsl_rng_free(theRandomNumberGenerator);
        theRandomNumberGenerator = 0;
    }
}


QStringList RGSLProbabilityDistributions::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    return actionList;
}


int RGSLProbabilityDistributions::resetRandomNumberGenerator()
{
    if (theRandomNumberGenerator) {
        gsl_rng_free(theRandomNumberGenerator);
        theRandomNumberGenerator = 0;
    }

    // Create a random number generator
    gsl_rng_env_setup();
    theRandomNumberGenerator = gsl_rng_alloc(gsl_rng_default);
    gsl_rng_set(theRandomNumberGenerator, 18);

    return 0;
}


double RGSLProbabilityDistributions::getNormalPDF(double x, double sigma)
{
    return gsl_ran_gaussian_pdf(x, sigma);
}


double RGSLProbabilityDistributions::getNormalCDF(double x, double sigma)
{

    return gsl_cdf_gaussian_P(x, sigma);
}


double RGSLProbabilityDistributions::getInverseNormalCDF(double x, double sigma)
{
    return gsl_cdf_gaussian_Pinv(x, sigma);
}


double RGSLProbabilityDistributions::generateStandardNormalNumber()
{
    return gsl_ran_ugaussian(theRandomNumberGenerator);

}


double RGSLProbabilityDistributions::generateBinomialNumber(double p, int n)
{
    return gsl_ran_binomial(theRandomNumberGenerator, p, n);

}


double RGSLProbabilityDistributions::gammaFunction(double x)
{
    return gsl_sf_gamma(x);
}


double RGSLProbabilityDistributions::getExponentialPDF(double x, double mu)
{
    return 	gsl_ran_exponential_pdf(x, mu);
}


double RGSLProbabilityDistributions::getExponentialCDF(double x, double mu)
{
    return gsl_cdf_exponential_P(x, mu);
}


double RGSLProbabilityDistributions::getInverseExponentialCDF(double x, double mu)
{
    return gsl_cdf_exponential_Pinv(x, mu);
}


double RGSLProbabilityDistributions::generateExponentialNumber(double mu)
{
    return gsl_ran_exponential(theRandomNumberGenerator, mu);
}


double RGSLProbabilityDistributions::getLaplacePDF(double x, double a)
{
    return gsl_ran_laplace_pdf(x, a);
}


double RGSLProbabilityDistributions::getLaplaceCDF(double x, double a)
{
    return gsl_cdf_laplace_P(x, a);
}


double RGSLProbabilityDistributions::getInverseLaplaceCDF(double x, double a)
{
    return gsl_cdf_laplace_Pinv(x, a);
}


double RGSLProbabilityDistributions::getRayleighPDF(double x, double sigma)
{
    return gsl_ran_rayleigh_pdf(x, sigma);
}


double RGSLProbabilityDistributions::getRayleighCDF(double x, double sigma)
{
    return gsl_cdf_rayleigh_P(x, sigma);
}


double RGSLProbabilityDistributions::getInverseRayleighCDF(double x, double sigma)
{
    return gsl_cdf_rayleigh_Pinv(x, sigma);
}


double RGSLProbabilityDistributions::getGammaPDF(double x, double a, double b)
{
    return gsl_ran_gamma_pdf(x, a, b);
}


double RGSLProbabilityDistributions::getGammaCDF(double x, double a, double b)
{
    return gsl_cdf_gamma_P(x, a, b);
}


double RGSLProbabilityDistributions::getInverseGammaCDF(double x, double a, double b)
{
    return gsl_cdf_gamma_Pinv(x, a, b);
}


double RGSLProbabilityDistributions::getUniformPDF(double x, double a, double b)
{
    return gsl_ran_flat_pdf(x, a, b);
}


double RGSLProbabilityDistributions::getUniformCDF(double x, double a, double b)
{
    return gsl_cdf_flat_P(x, a, b);
}


double RGSLProbabilityDistributions::getInverseUniformCDF(double x, double a, double b)
{
    return gsl_cdf_flat_Pinv(x, a, b);
}


double RGSLProbabilityDistributions::getLognormalPDF(double x, double zeta, double sigma)
{
    return gsl_ran_lognormal_pdf(x, zeta, sigma);
}


double RGSLProbabilityDistributions::getLognormalCDF(double x, double zeta, double sigma)
{
    return gsl_cdf_lognormal_P(x, zeta, sigma);
}


double RGSLProbabilityDistributions::getInverseLognormalCDF(double x, double zeta, double sigma)
{
    return gsl_cdf_lognormal_Pinv(x, zeta, sigma);
}


double RGSLProbabilityDistributions::getChiSquaredPDF(double x, double nu)
{
    return gsl_ran_chisq_pdf(x, nu);
}


double RGSLProbabilityDistributions::getChiSquaredCDF(double x, double nu)
{
    return gsl_cdf_chisq_P(x, nu);
}


double RGSLProbabilityDistributions::getInverseChiSquaredCDF(double x, double nu)
{
    return gsl_cdf_chisq_Pinv(x, nu);
}


double RGSLProbabilityDistributions::getTDistributionPDF(double x, double nu)
{
    return gsl_ran_tdist_pdf(x, nu);
}


double RGSLProbabilityDistributions::getTDistributionCDF(double x, double nu)
{
    return gsl_cdf_tdist_P(x, nu);
}


double RGSLProbabilityDistributions::getInverseTDistributionCDF(double x, double nu)
{
    return gsl_cdf_tdist_Pinv(x, nu);
}


double RGSLProbabilityDistributions::getBetaPDF(double x, double a, double b)
{
    return gsl_ran_beta_pdf(x, a, b);
}


double RGSLProbabilityDistributions::getBetaCDF(double x, double a, double b)
{
    return gsl_cdf_beta_P(x, a, b);
}


double RGSLProbabilityDistributions::getInverseBetaCDF(double x, double a, double b)
{
    return gsl_cdf_beta_Pinv(x, a, b);
}


double RGSLProbabilityDistributions::getLogisticPDF(double x, double a)
{
    return gsl_ran_logistic_pdf(x, a);
}


double RGSLProbabilityDistributions::getLogisticCDF(double x, double a)
{
    return gsl_cdf_logistic_P(x, a);
}


double RGSLProbabilityDistributions::getInverseLogisticCDF(double x, double a)
{
    return gsl_cdf_logistic_Pinv(x, a);
}


double RGSLProbabilityDistributions::getWeibullPDF(double x, double a, double b)
{
    return gsl_ran_weibull_pdf(x, a, b);
}


double RGSLProbabilityDistributions::getWeibullCDF(double x, double a, double b)
{
    return gsl_cdf_weibull_P(x, a, b);
}


double RGSLProbabilityDistributions::getInverseWeibullCDF(double x, double a, double b)
{
    return gsl_cdf_weibull_Pinv(x, a, b);
}


double RGSLProbabilityDistributions::getGumbelPDF(double x, double a, double b)
{
    return gsl_ran_gumbel1_pdf(x, a, b);
}


double RGSLProbabilityDistributions::getGumbelCDF(double x, double a, double b)
{
    return gsl_cdf_gumbel1_P(x,a,b);;
}


double RGSLProbabilityDistributions::getInverseGumbelCDF(double x, double a, double b)
{
    return gsl_cdf_gumbel1_Pinv(x, a, b);
}



