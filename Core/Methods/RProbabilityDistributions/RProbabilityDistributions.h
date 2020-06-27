#ifndef RProbabilityDistributions_H
#define RProbabilityDistributions_H
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

class RProbabilityDistributions : public RMethod
{
	Q_OBJECT

public:
    RProbabilityDistributions(QObject *parent, QString name);
    virtual ~RProbabilityDistributions();

    // Methods for the Normal distribution:
    virtual double getNormalPDF(double x, double sigma) = 0;
    virtual double getNormalCDF(double x, double sigma) = 0;
    virtual double getInverseNormalCDF(double x, double sigma) = 0;

    virtual double generateStandardNormalNumber() = 0;
    virtual double generateBinomialNumber(double p, int n) = 0;
    virtual double gammaFunction(double x) = 0;
    virtual double generateExponentialNumber(double mu) = 0;

    // Methods for the Exponential distribution:
    virtual double getExponentialPDF(double x, double mu) = 0;
    virtual double getExponentialCDF(double x, double mu) = 0;
    virtual double getInverseExponentialCDF(double x, double mu) = 0;

    // Methods for the Laplace distribution:
    virtual double getLaplacePDF(double x, double a) = 0;
    virtual double getLaplaceCDF(double x, double a) = 0;
    virtual double getInverseLaplaceCDF(double x, double a) = 0;

    // Methods for the Rayleigh distribution:
    virtual double getRayleighPDF(double x, double sigma) = 0;
    virtual double getRayleighCDF(double x, double sigma) = 0;
    virtual double getInverseRayleighCDF(double x, double sigma) = 0;

    // Methods for the Gamma distribution:
    virtual double getGammaPDF(double x, double a, double b) = 0;
    virtual double getGammaCDF(double x, double a, double b) = 0;
    virtual double getInverseGammaCDF(double x, double a, double b) = 0;

    // Methods for the Uniform distribution:
    virtual double getUniformPDF(double x, double a, double b) = 0;
    virtual double getUniformCDF(double x, double a, double b) = 0;
    virtual double getInverseUniformCDF(double x, double a, double b) = 0;

    // Methods for the Lognormal distribution:
    virtual double getLognormalPDF(double x, double zeta, double sigma) = 0;
    virtual double getLognormalCDF(double x, double zeta, double sigma) = 0;
    virtual double getInverseLognormalCDF(double x, double zeta, double sigma) = 0;

    // Methods for the Chi-squared distribution:
    virtual double getChiSquaredPDF(double x, double nu) = 0;
    virtual double getChiSquaredCDF(double x, double nu) = 0;
    virtual double getInverseChiSquaredCDF(double x, double nu) = 0;

    // Methods for the t-distribution distribution:
    virtual double getTDistributionPDF(double x, double nu) = 0;
    virtual double getTDistributionCDF(double x, double nu) = 0;
    virtual double getInverseTDistributionCDF(double x, double nu) = 0;

    // Methods for the Beta distribution:
    virtual double getBetaPDF(double x, double a, double b) = 0;
    virtual double getBetaCDF(double x, double a, double b) = 0;
    virtual double getInverseBetaCDF(double x, double a, double b) = 0;

    // Methods for the Logistic distribution:
    virtual double getLogisticPDF(double x, double a) = 0;
    virtual double getLogisticCDF(double x, double a) = 0;
    virtual double getInverseLogisticCDF(double x, double a) = 0;

    // Methods for the Weibull distribution:
    virtual double getWeibullPDF(double x, double a, double b) = 0;
    virtual double getWeibullCDF(double x, double a, double b) = 0;
    virtual double getInverseWeibullCDF(double x, double a, double b) = 0;

    // Methods for the Gumbel distribution:
    virtual double getGumbelPDF(double x, double a, double b) = 0;
    virtual double getGumbelCDF(double x, double a, double b) = 0;
    virtual double getInverseGumbelCDF(double x, double a, double b) = 0;

protected:

private:
	
};

#endif
