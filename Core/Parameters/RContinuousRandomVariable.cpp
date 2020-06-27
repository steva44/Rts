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

#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RMainWindow.h"
#include "RDomain.h"

#include "qcustomplot.h"
#include <QInputDialog>
#include <math.h>

RContinuousRandomVariable::RContinuousRandomVariable(QObject *parent, QString name)
    : RRandomVariable(parent, name)
{
    theProbabilityDistributions = nullptr;
    theMeanParameter = nullptr;
    theStandardDeviationParameter = nullptr;
    theCoefficientOfVariationParameter = nullptr;
    theParameter1Parameter = nullptr;
    theParameter2Parameter = nullptr;
    theParameter3Parameter = nullptr;
    theParameter4Parameter = nullptr;

    theMeanDouble = 0.0;
    theStandardDeviationDouble = 0.0;
    theCoefficientOfVariationDouble = 0.0;
    theParameter1Double = 0.0;
    theParameter2Double = 0.0;
    theParameter3Double = 0.0;
    theParameter4Double = 0.0;

    theAlphaImportanceMeasure = 0.0;
    theGammaImportanceMeasure = 0.0;

    theDistributionType = "Normal (mean, stdv)";

    if (theDistributionType == QString("User Defined")) {
        QString XPointsStr = property("XPoints").toString();
        QString PDFPointsStr = property("PDFPoints").toString();
        setProperty("XPoints", XPointsStr);
        setProperty("PDFPoints", PDFPointsStr);
    } else {
        setProperty("XPoints", QVariant());
        setProperty("PDFPoints", QVariant());
    }
}


RContinuousRandomVariable::~RContinuousRandomVariable()
{

}


QString RContinuousRandomVariable::getDistributionType() const
{
    return theDistributionType;
}


QString RContinuousRandomVariable::getMean()
{
    return getStringParameter(&theMeanDouble, theMeanParameter);
}


QString RContinuousRandomVariable::getStandardDeviation()
{
    return getStringParameter(&theStandardDeviationDouble, theStandardDeviationParameter);
}


QString RContinuousRandomVariable::getCoefficientOfVariation()
{
    return getStringParameter(&theCoefficientOfVariationDouble, theCoefficientOfVariationParameter);
}


QString RContinuousRandomVariable::getParameter1()
{
    return getStringParameter(&theParameter1Double, theParameter1Parameter);
}


QString RContinuousRandomVariable::getParameter2()
{
    return getStringParameter(&theParameter2Double, theParameter2Parameter);
}


QString RContinuousRandomVariable::getParameter3()
{
    return getStringParameter(&theParameter3Double, theParameter3Parameter);
}


QString RContinuousRandomVariable::getParameter4()
{
    return getStringParameter(&theParameter4Double, theParameter4Parameter);
}


void RContinuousRandomVariable::setDistributionType(QString value)
{
    theDistributionType = value;

    if (theDistributionType == QString("User Defined")) {
        QString XPointsStr = property("XPoints").toString();
        QString PDFPointsStr = property("PDFPoints").toString();
        setProperty("XPoints", XPointsStr);
        setProperty("PDFPoints", PDFPointsStr);
    } else {
        setProperty("XPoints", QVariant());
        setProperty("PDFPoints", QVariant());
    }

    updateParameters();
}


void RContinuousRandomVariable::setMean(QString value)
{
    setStringParameter(value, &theMeanDouble, &theMeanParameter);
    setCurrentValue(getMeanValue());
    updateParameters();
}


void RContinuousRandomVariable::setStandardDeviation(QString value)
{

    setStringParameter(value, &theStandardDeviationDouble, &theStandardDeviationParameter);
    updateParameters();
}


void RContinuousRandomVariable::setCoefficientOfVariation(QString value)
{
    setStringParameter(value, &theCoefficientOfVariationDouble, &theCoefficientOfVariationParameter);
    updateMeanStdvByCOV();
    updateParameters();
}


void RContinuousRandomVariable::setParameter1(QString value)
{
    setStringParameter(value, &theParameter1Double, &theParameter1Parameter);
    updateMeanStdv();
    setCurrentValue(getMeanValue());
}


void RContinuousRandomVariable::setParameter2(QString value)
{
    setStringParameter(value, &theParameter2Double, &theParameter2Parameter);
    updateMeanStdv();
    setCurrentValue(getMeanValue());
}


void RContinuousRandomVariable::setParameter3(QString value)
{
    setStringParameter(value, &theParameter3Double, &theParameter3Parameter);
    updateMeanStdv();
    setCurrentValue(getMeanValue());
}


void RContinuousRandomVariable::setParameter4(QString value)
{
    setStringParameter(value, &theParameter4Double, &theParameter4Parameter);
    updateMeanStdv();
    setCurrentValue(getMeanValue());
}


double RContinuousRandomVariable::getMeanValue()
{
    return getDoubleParameter(&theMeanDouble, theMeanParameter);
}


double RContinuousRandomVariable::getCoefficientOfVariationValue()
{
    return getDoubleParameter(&theCoefficientOfVariationDouble, theCoefficientOfVariationParameter);
}


double RContinuousRandomVariable::getStandardDeviationValue()
{
    return getDoubleParameter(&theStandardDeviationDouble, theStandardDeviationParameter);
}


double RContinuousRandomVariable::getParameter1Value()
{
    return getDoubleParameter(&theParameter1Double, theParameter1Parameter);
}


double RContinuousRandomVariable::getParameter2Value()
{
    return getDoubleParameter(&theParameter2Double, theParameter2Parameter);
}


double RContinuousRandomVariable::getParameter3Value()
{
    return getDoubleParameter(&theParameter3Double, theParameter3Parameter);
}


double RContinuousRandomVariable::getParameter4Value()
{
    return getDoubleParameter(&theParameter4Double, theParameter4Parameter);
}


double RContinuousRandomVariable::getAlphaImportanceMeasure() const
{
    return theAlphaImportanceMeasure;
}


void RContinuousRandomVariable::setAlphaImportanceMeasure(double value)
{
    theAlphaImportanceMeasure = value;
}


double RContinuousRandomVariable::getGammaImportanceMeasure() const
{
    return theGammaImportanceMeasure;
}


void RContinuousRandomVariable::setGammaImportanceMeasure(double value)
{
    theGammaImportanceMeasure = value;
}


double RContinuousRandomVariable::getPDFValue(double x)
{
    if (theProbabilityDistributions==nullptr  && theDistributionType!=QString("User Defined") ) {
        createDefaultDistributions();
    }


    double result;

    if (theStandardDeviationDouble == 0.0) {
        qCritical() << "PDF warning: The standard deviation of the random variable" << objectName() << "is Zero.";
    }

    if (theDistributionType == QString("Normal (mean, stdv)")) {
        double mean = getParameter1Value();
        double stdv = getParameter2Value();
        result = theProbabilityDistributions->getNormalPDF(x - mean, stdv);
    }
    else if (theDistributionType == QString("Exponential (mu, xo)")) {
        double mu = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getExponentialPDF(x - xo, mu);
    }
    else if (theDistributionType == QString("Bounded Exponential (mu, xo, min, max)")) {
        double min = getParameter3Value();
        double max = getParameter4Value();
        if (x < min || x > max) {
            result = 0.0;
        }
        else {
            double mu = getParameter1Value();
            double xo = getParameter2Value();
            double k = 1.0 / (1.0 - exp(-mu * (max - min)));
            result = k * mu * exp(-mu * (x - xo - min));
        }
    }
    else if (theDistributionType == QString("Linear (a, b)")) {
        double min = getParameter1Value();
        double max = getParameter2Value();
        if (x < min || x > max) {
            result = 0.0;
        }
        else {
            result = 2 * x / (max * max - min * min);
        }
    }
    else if (theDistributionType == QString("Laplace (a, xo)")) {
        double a = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getLaplacePDF(x - xo, a);
    }
    else if (theDistributionType == QString("Rayleigh (sigma, xo)")) {
        double sigma = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getRayleighPDF(x - xo, sigma);
    }
    else if (theDistributionType == QString("Gamma (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getGammaPDF(x, a, b);
    }
    else if (theDistributionType == QString("Uniform (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getUniformPDF(x, a, b);
    }
    else if (theDistributionType == QString("Lognormal (zeta, sigma)")) {
        double zeta = getParameter1Value();
        double sigma = getParameter2Value();
        result = theProbabilityDistributions->getLognormalPDF(x, zeta, sigma);
    }
    else if (theDistributionType == QString("Chi-square (nu, xo)")) {
        double nu = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getChiSquaredPDF(x - xo, nu);
    }
    else if (theDistributionType == QString("t-distribution (nu, xo)")) {
        double nu = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getTDistributionPDF(x - xo, nu);
    }
    else if (theDistributionType == QString("Beta (a, b, min, max)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        double min = getParameter3Value();
        double max = getParameter4Value();
        result = theProbabilityDistributions->getBetaPDF((x - min)/(max - min), a, b) / (max - min);
    }
    else if (theDistributionType == QString("Logistic (a, xo)")) {
        double a = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getLogisticPDF(x - xo, a);
    }
    else if (theDistributionType == QString("Weibull (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getWeibullPDF(x, a, b);
    }
    else if (theDistributionType == QString("Gumbel (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getGumbelPDF(x, a, b);
    }
    else if (theDistributionType == QString("User Defined")) {
        if (userDefinedErrorCheck() < 0) {
            return 0.0;
        }
        result = -1.0;
        if (x < XVector[0]) {
            return 0.0;
        } else if (x > XVector.last()) {
            return 0.0;
        } else {
            for (int i = 1; i < XVector.count(); i++) {
                if (x <= XVector[i]) {
                    double a = (PDFVector[i] - PDFVector[i-1]) / (XVector[i] - XVector[i-1]);
                    result = (a * (x - XVector[i-1]) + PDFVector[i-1]);
                    break;
                }
            }
            if (result < -1e-10) {
                qCritical() << "Error: The user defined distribution function in random variable" << objectName() << "seems to be invalid. The distribution is returned as 0.0.";
                return 0.0;
            }
        }
    }
    else {
        qCritical() << "Warning: Illegal probability distribution type has been passed to the random variable" << objectName() << ".";
    }

    return result;
}


double RContinuousRandomVariable::getCDFValue(double x)
{
    if (theProbabilityDistributions==nullptr  && theDistributionType!=QString("User Defined") ) {
        createDefaultDistributions();
    }

    double result;

    if (theStandardDeviationDouble == 0.0) {
        qCritical() << "CDF warning: The standard deviation of the random variable" << objectName() << "is Zero.";
    }

    if (theDistributionType == QString("Normal (mean, stdv)")) {
        double mean = getParameter1Value();
        double stdv = getParameter2Value();
        result = theProbabilityDistributions->getNormalCDF(x-mean, stdv);
    }
    else if (theDistributionType == QString("Exponential (mu, xo)")) {
        double mu = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getExponentialCDF(x-xo,mu);
    }
    else if (theDistributionType == QString("Bounded Exponential (mu, xo, min, max)")) {
        double min = getParameter3Value();
        double max = getParameter4Value();
        if (x<min) {
            result = 0.0;
        }
        else if (x>max) {
            result = 1.0;
        }
        else {
            double mu = getParameter1Value();
            double xo = getParameter2Value();
            double k = 1.0 / (1.0 - exp(-mu*(max-min)) );
            result = k - k * exp(-mu*(x-xo-min));
        }
    }
    else if (theDistributionType == QString("Linear (a, b)")) {
        double min = getParameter1Value();
        double max = getParameter2Value();
        if (x<min) {
            result = 0.0;
        }
        else if (x>max) {
            result = 1.0;
        }
        else {
            result = (x*x - min*min) / (max*max - min*min);
        }
    }
    else if (theDistributionType == QString("Laplace (a, xo)")) {
        double a = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getLaplaceCDF(x-xo,a);
    }
    else if (theDistributionType == QString("Rayleigh (sigma, xo)")) {
        double sigma = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getRayleighCDF(x-xo,sigma);
    }
    else if (theDistributionType == QString("Gamma (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getGammaCDF(x,a,b);
    }
    else if (theDistributionType == QString("Uniform (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getUniformCDF(x,a,b);
    }
    else if (theDistributionType == QString("Lognormal (zeta, sigma)")) {
        double zeta = getParameter1Value();
        double sigma = getParameter2Value();
        result = theProbabilityDistributions->getLognormalCDF(x,zeta,sigma);
    }
    else if (theDistributionType == QString("Chi-square (nu, xo)")) {
        double nu = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getChiSquaredCDF(x-xo,nu);
    }
    else if (theDistributionType == QString("t-distribution (nu, xo)")) {
        double nu = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getTDistributionCDF(x-xo,nu);
    }
    else if (theDistributionType == QString("Beta (a, b, min, max)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        double min = getParameter3Value();
        double max = getParameter4Value();
        result = theProbabilityDistributions->getBetaCDF((x-min)/(max-min),a,b);
    }
    else if (theDistributionType == QString("Logistic (a, xo)")) {
        double a = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getLogisticCDF(x-xo,a);
    }
    else if (theDistributionType == QString("Weibull (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getWeibullCDF(x,a,b);
    }
    else if (theDistributionType == QString("Gumbel (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        // Attention: Bug in G S L: Cannot use the G S L method.
        //result = g s l_cdf_gumbel1_P(x,a,b);
        result = exp(-b * exp(-a * x));
    }
    else if (theDistributionType == QString("User Defined")) {
        if (userDefinedErrorCheck() < 0) {
            return 0.0;
        }
        result = -1.0;
        if (x < XVector[0]) {
            return 0.0;
        } else if (x > XVector.last()) {
            return 1.0;
        } else {
            double sum = 0.0;
            for (int i = 1; i < XVector.count(); i++) {
                if (x <= XVector[i]) {
                    result = (sum + 0.5 * (getPDFValue(x) + PDFVector[i-1]) * (x - XVector[i-1]));
                    break;
                }
                sum += 0.5 * (PDFVector[i] + PDFVector[i-1]) * (XVector[i] - XVector[i-1]);
            }
            if (result < -1e-10) {
                qCritical() << "Error: The user defined distribution function in random variable" << objectName() << "seems to be invalid. The distribution is returned as 0.0.";
                return 0.0;
            }
        }
    }
    else {
        qCritical() << "Warning: Illegal probability distribution type has been passed to the random variable" << objectName() << ".";
    }

    return result;
}


double RContinuousRandomVariable::getInverseCDFValue(double p)
{
    if (theProbabilityDistributions==nullptr  && theDistributionType!=QString("User Defined") ) {
        createDefaultDistributions();
    }


    if (p < 0.0 || p > 1.0) {
        qCritical() << "Warning: Illegal probability value input has been passed to the random variable" << objectName() << ".";

        /*// Is this okay?
        if (p < 0.0) {
            p = 0.0;
        } else if (p > 1.0) {
            p = 1.0;
        }*/
    }

    double result;

    if (theStandardDeviationDouble == 0.0) {
        qCritical() << "Inverse CDF warning: The standard deviation of the random variable" << objectName() << "is Zero.";
    }

    if (theDistributionType == QString("Normal (mean, stdv)")) {
        double mean = getParameter1Value();
        double stdv = getParameter2Value();
        result = theProbabilityDistributions->getInverseNormalCDF(p,stdv)+mean;
    }
    else if (theDistributionType == QString("Exponential (mu, xo)")) {
        double mu = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getInverseExponentialCDF(p,mu)+xo;
    }
    else if (theDistributionType == QString("Bounded Exponential (mu, xo, min, max)")) {
        double mu = getParameter1Value();
        double xo = getParameter2Value();
        double min = getParameter3Value();
        double max = getParameter4Value();
        double k = 1.0 / (1.0 - exp(-mu*(max-min)) );
        result = (mu*min-log((k-p)/k))/mu + xo;
    }
    else if (theDistributionType == QString("Linear (a, b)")) {
        double min = getParameter1Value();
        double max = getParameter2Value();
        result = sqrt(p * (max*max - min*min) + min*min);
    }
    else if (theDistributionType == QString("Laplace (a, xo)")) {
        double a = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getInverseLaplaceCDF(p,a)+xo;
    }
    else if (theDistributionType == QString("Rayleigh (sigma, xo)")) {
        double sigma = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getInverseRayleighCDF(p,sigma)+xo;
    }
    else if (theDistributionType == QString("Gamma (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getInverseGammaCDF(p,a,b);
    }
    else if (theDistributionType == QString("Uniform (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getInverseUniformCDF(p,a,b);
    }
    else if (theDistributionType == QString("Lognormal (zeta, sigma)")) {
        double zeta = getParameter1Value();
        double sigma = getParameter2Value();
        result = theProbabilityDistributions->getInverseLognormalCDF(p,zeta,sigma);
    }
    else if (theDistributionType == QString("Chi-square (nu, xo)")) {
        double nu = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getInverseChiSquaredCDF(p,nu)+xo;
    }
    else if (theDistributionType == QString("t-distribution (nu, xo)")) {
        double nu = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getInverseTDistributionCDF(p,nu)+xo;
    }
    else if (theDistributionType == QString("Beta (a, b, min, max)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        double min = getParameter3Value();
        double max = getParameter4Value();
        result = theProbabilityDistributions->getInverseBetaCDF(p,a,b) * (max-min) + min;
    }
    else if (theDistributionType == QString("Logistic (a, xo)")) {
        double a = getParameter1Value();
        double xo = getParameter2Value();
        result = theProbabilityDistributions->getInverseLogisticCDF(p,a)+xo;
    }
    else if (theDistributionType == QString("Weibull (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getInverseWeibullCDF(p,a,b);
    }
    else if (theDistributionType == QString("Gumbel (a, b)")) {
        double a = getParameter1Value();
        double b = getParameter2Value();
        result = theProbabilityDistributions->getInverseGumbelCDF(p,a,b);
    }
    else if (theDistributionType == QString("User Defined")) {
        if (userDefinedErrorCheck() < 0) {
            return 0.0;
        }
        if (p == 0.0) {
            result = XVector.first();
        } else if (p == 1.0) {
            result = XVector.last();
        } else {
            for (int i = 1; i < XVector.count(); i++) {
                if (getCDFValue(XVector[i]) > p) {
                    double a = (getPDFValue(XVector[i]) - getPDFValue(XVector[i-1])) / (XVector[i] - XVector[i-1]);
                    if (a == 0.0 && getPDFValue(XVector[i-1]) == 0.0) {
                        qCritical() << "Error: The user defined distribution function in random variable" << objectName() << "has a region of constant zero PDF. The distribution is returned as 0.0.";
                        return 0.0;
                    }
                    else if (a == 0.0) {
                        result = (p - getCDFValue(XVector[i-1]) + getPDFValue(XVector[i-1]) * XVector[i-1]) / getPDFValue(XVector[i-1]);
                    }
                    else {
                        double AA = 0.5 * a;
                        double BB = getPDFValue(XVector[i-1]);
                        double CC = getCDFValue(XVector[i-1]) - p;
                        double deltaX = (-BB + sqrt(BB * BB - 4.0 * AA * CC)) / (2.0 * AA);
                        result = deltaX + (XVector[i-1]);
                    }
                    break;
                }
            }
        }
    }
    else {
        qCritical() << "Warning: Illegal probability distribution type has been passed to the random variable" << objectName() << ".";
    }

    return result;
}


int RContinuousRandomVariable::updateParameters()
{
    // Initial declarations
    double parameter1 = 0.0;
    double parameter2 = 0.0;
    double parameter3 = 0.0;
    double parameter4 = 0.0;
    double mean = getMeanValue();
    double stdv = getStandardDeviationValue();
    double cov = qAbs(stdv / (mean));

    // Compute parameter values
    if (theDistributionType == QString("Normal (mean, stdv)")) {
        parameter1 = mean;
        parameter2 = stdv;
    }
    else if (theDistributionType == QString("Exponential (mu, xo)")) {
        parameter1 = stdv;
        parameter2 = mean-parameter1;
    }
    else if (theDistributionType == QString("Bounded Exponential (mu, xo, min, max)")) {
        //qCritical() << "Warning: Moments cannot be given for the" << theDistributionType << "distribution. Please give parameter values only.";
    }
    else if (theDistributionType == QString("Laplace (a, xo)")) {
        parameter2 = mean;
        parameter1 = stdv/sqrt(2.0);
    }
    else if (theDistributionType == QString("Linear (a, b)")) {
        //qCritical() << "Warning: Moments cannot be given for the" << theDistributionType << "distribution. Please give parameter values only.";
    }
    else if (theDistributionType == QString("Rayleigh (sigma, xo)")) {
        parameter1 = stdv/sqrt((4.0-3.14159265359)/2.0);
        parameter2 = mean - parameter1 * sqrt(3.14159265359/2.0);
    }
    else if (theDistributionType == QString("Gamma (a, b)")) {
        parameter1 = (mean/stdv)*(mean/stdv);
        parameter2 = stdv*stdv / mean;
    }
    else if (theDistributionType == QString("Uniform (a, b)")) {
        parameter1 = mean-sqrt(3.0)*stdv;
        parameter2 = mean+sqrt(3.0)*stdv;
    }
    else if (theDistributionType == QString("Lognormal (zeta, sigma)")) {
        parameter1 = log(mean)-0.5*log(1+(stdv/mean)*(stdv/mean));
        parameter2 = sqrt(log((stdv / mean)*(stdv / mean) + 1));
    }
    else if (theDistributionType == QString("Chi-square (nu, xo)")) {
        parameter1 = 0.5*stdv*stdv;
        parameter2 = mean - parameter1;
    }
    else if (theDistributionType == QString("t-distribution (nu, xo)")) {
        parameter2 = mean;
        parameter1 = 2*stdv*stdv/(stdv*stdv-1);
    }
    else if (theDistributionType == QString("Beta (a, b, min, max)")) {
        //qCritical() << "Warning: Moments cannot be given for the Beta distribution. Please give parameter values only.";
        parameter3 = getParameter3Value();
        parameter4 = getParameter4Value();
        double min = parameter3;
        double max = parameter4;
        double temp = (mean - min) * (max - mean) / (stdv * stdv) - 1.0;
        parameter1 = (mean - min) / (max - min) * temp;
        parameter2 = (max - mean) / (max - min) * temp;
    }
    else if (theDistributionType == QString("Logistic (a, xo)")) {
        parameter1 = stdv*sqrt(3.0)/3.14159265359;
        parameter2 = mean-parameter1;
    }
    else if (theDistributionType == QString("Weibull (a, b)")) {
        qCritical() << "Warning: Moments cannot be given for the Weibull distribution. Please give parameter values only.";
    }
    else if (theDistributionType == QString("Gumbel (a, b)")) {
        parameter1 = 3.14159265359 / (stdv * sqrt(6.0));
        parameter2 = exp(parameter1 * mean - 0.5772156649);
    }
    else if (theDistributionType == QString("User Defined")) {

    }
    else {
        qCritical() << "Warning: Illegal probability distribution type has been passed to the random variable" << objectName() << ".";
    }

    // Update the parameter values; either the Double or the Parameter
    if (theCoefficientOfVariationParameter == nullptr) {
        theCoefficientOfVariationDouble = cov;
    }
    else {
        theCoefficientOfVariationParameter->setCurrentValue(cov);
    }

    if (theParameter1Parameter == nullptr) {
        theParameter1Double = parameter1;
    }
    else {
        theParameter1Parameter->setCurrentValue(parameter1);
    }

    if (theParameter2Parameter == nullptr) {
        theParameter2Double = parameter2;
    }
    else {
        theParameter2Parameter->setCurrentValue(parameter2);
    }

    if (theParameter3Parameter == nullptr) {
        theParameter3Double = parameter3;
    }
    else {
        theParameter3Parameter->setCurrentValue(parameter3);
    }

    if (theParameter4Parameter == nullptr) {
        theParameter4Double = parameter4;
    }
    else {
        theParameter4Parameter->setCurrentValue(parameter4);
    }

    return 0;
}


int RContinuousRandomVariable::updateMeanStdvByCOV()
{
    double mean = getMeanValue();
    double cov = getCoefficientOfVariationValue();
    double stdv = qAbs((mean) * cov);

    if (theStandardDeviationParameter == nullptr) {
        theStandardDeviationDouble = stdv;
    }
    else {
        theStandardDeviationParameter->setCurrentValue(stdv);
    }

    return 0;
}


int RContinuousRandomVariable::updateMeanStdv()
{
    // Initial declarations
    double mean = 0.0;
    double stdv = 0.0;
    double parameter1 = getParameter1Value();
    double parameter2 = getParameter2Value();
    double parameter3 = getParameter3Value();
    double parameter4 = getParameter4Value();

    if (theDistributionType == QString("Normal (mean, stdv)")) {
        mean = parameter1;
        stdv = parameter2;
    }
    else if (theDistributionType == QString("Exponential (mu, xo)")) {
        mean = parameter1+parameter2;
        stdv = parameter1;
    }
    else if (theDistributionType == QString("Bounded Exponential (mu, xo, min, max)")) {
        double mu = parameter1;
        double xo = parameter2;
        double min = parameter3;
        double max = parameter4;
        double k = 1.0 / (1.0 - exp(-mu*(max-min)) );
        mean = -k*(mu*max+1.0)/mu * exp(-mu*(max-min)) + (1+mu*min)/(mu)*k;
        stdv = sqrt( (-exp(-mu*(max-min))) * (mu*mu*max*max+2.0*mu*max+2.0-2.0*mean*mu*mu*max-2*mean*mu+mean*mean*mu*mu)*k/(mu*mu)  +  (mu*mu*min*min+2.0*mu*min+2.0-2.0*min*mean*mu*mu-2.0*mean*mu+mean*mean*mu*mu)*k/(mu*mu) );
        mean += xo;
    }
    else if (theDistributionType == QString("Linear (a, b)")) {
        double min = parameter1;
        double max = parameter2;
        mean = 2.0 / 3.0 * (min*min + min*max + max*max) / (min + max);
        stdv = sqrt((min*min + max*max) / 2.0 - mean*mean);
    }
    else if (theDistributionType == QString("Laplace (a, xo)")) {
        mean = parameter2;
        stdv = sqrt(2.0)*parameter1;
    }
    else if (theDistributionType == QString("Rayleigh (sigma, xo)")) {
        mean = parameter1 * sqrt(3.14159265359/2.0) + parameter2;
        stdv = parameter1*sqrt((4-3.14159265359)/2.0);
    }
    else if (theDistributionType == QString("Gamma (a, b)")) {
        mean = parameter1*parameter2;
        stdv = sqrt(parameter1)*parameter2;
    }
    else if (theDistributionType == QString("Uniform (a, b)")) {
        mean = (parameter1+parameter2)/2.0;
        stdv = (parameter2-parameter1)/sqrt(12.0);
    }
    else if (theDistributionType == QString("Lognormal (zeta, sigma)")) {
        mean = exp(parameter1+0.5*parameter2*parameter2);
        stdv = sqrt((exp(parameter2*parameter2)-1))*exp(parameter1+0.5*parameter2*parameter2);
    }
    else if (theDistributionType == QString("Chi-square (nu, xo)")) {
        mean = parameter1+parameter2;
        stdv = sqrt(2.0*parameter1);
    }
    else if (theDistributionType == QString("t-distribution (nu, xo)")) {
        mean = parameter2;
        if (parameter1<=2.0) {
            qCritical() << "Warning: The standard deviation is not defined for the t-distribution for nu<=2.0. Stdv not set.";
        }
        else {
            stdv = sqrt(parameter1/(parameter1-2.0));
        }
    }
    else if (theDistributionType == QString("Beta (a, b, min, max)")) {
        double a = parameter1;
        double b = parameter2;
        double min = parameter3;
        double max = parameter4;
        mean = (a/(a+b))*(max-min)+min;
        stdv = ( 1/(a+b) * sqrt( a*b/(a+b+1) ) ) *(max-min);
    }
    else if (theDistributionType == QString("Logistic (a, xo)")) {
        mean = parameter1+parameter2;
        stdv = parameter1*3.14159265359/sqrt(3.0);
    }
    else if (theDistributionType == QString("Weibull (a, b)")) {
        mean = parameter1*theProbabilityDistributions->gammaFunction(1.0+1.0/parameter2);
        stdv = parameter1* sqrt(theProbabilityDistributions->gammaFunction(1+2.0/parameter2) - (theProbabilityDistributions->gammaFunction(1+1.0/parameter2))*(theProbabilityDistributions->gammaFunction(1+1.0/parameter2)));
    }
    else if (theDistributionType == QString("Gumbel (a, b)")) {
        mean = (log(parameter2) + 0.5772156649) / parameter1;
        stdv = 3.14159265359 / (parameter1 * sqrt(6.0));
    }
    else if (theDistributionType == QString("User Defined")) {
        //if (userDefinedErrorCheck() < 0) {
        //	return -1;
        //}

        for (int i = 1; i < XVector.count(); i++) {
            double a = (PDFVector[i] - PDFVector[i-1]) / (XVector[i] - XVector[i-1]);
            double b = PDFVector[i-1] - a * XVector[i-1];
            mean += a * ((XVector[i])*(XVector[i])*(XVector[i]) - (XVector[i-1])*(XVector[i-1])*(XVector[i-1])) / 3.0 +  0.5 * b * ((XVector[i])*(XVector[i]) - (XVector[i-1])*(XVector[i-1]));
        }

        for (int i = 1; i < XVector.count(); i++) {
            double a = (PDFVector[i] - PDFVector[i-1]) / (XVector[i] - XVector[i-1]);
            double b = PDFVector[i-1] - a * XVector[i-1];
            double x1 = XVector[i-1];
            double x2 = XVector[i];
            stdv += 0.25 * a * ((x2)*(x2)*(x2)*(x2) - (x1)*(x1)*(x1)*(x1)) + (b / 3.0 - 2.0 / 3.0 * mean * a) * ((x2)*(x2)*(x2) - (x1)*(x1)*(x1)) + (0.5 * a * (mean)*(mean) - mean * b) * ((x2)*(x2) - (x1)*(x1)) + (mean)*(mean) * b * (x2 - x1);
        }
        stdv =  sqrt(stdv);
    }
    else {
        qCritical() << "Warning: Illegal probability distribution type has been passed to the random variable" << objectName() << ".";
    }

    double cov = qAbs(stdv / (mean));

    // Update the parameter values; either the Double or the Parameter
    if (theMeanParameter == nullptr) {
        theMeanDouble = mean;
    }
    else {
        theMeanParameter->setCurrentValue(mean);
    }

    if (theStandardDeviationParameter == nullptr) {
        theStandardDeviationDouble = stdv;
    }
    else {
        theStandardDeviationParameter->setCurrentValue(stdv);
    }

    if (theCoefficientOfVariationParameter == nullptr) {
        theCoefficientOfVariationDouble = cov;
    }
    else {
        theCoefficientOfVariationParameter->setCurrentValue(cov);
    }

    return 0;
}


void RContinuousRandomVariable::setMeanValue(double value)
{
    if (theMeanParameter == nullptr) {
        theMeanDouble = value;
    }
    else {
        theMeanParameter->setCurrentValue(value);
    }
    updateParameters();
}


void RContinuousRandomVariable::setStandardDeviationValue(double value)
{
    if (theStandardDeviationParameter == nullptr) {
        theStandardDeviationDouble = value;
    }
    else {
        theStandardDeviationParameter->setCurrentValue(value);
    }
    updateParameters();
}


int RContinuousRandomVariable::computeCDFSensitivityToParameters(double rvValue, double *dCDFdMean, double *dCDFdStdv)
{
    double meanBackup = getMeanValue();
    double stdvBackup = getStandardDeviationValue();
    //double covBackup = getCoefficientOfVariationValue();
    //double parameter1Backup = getParameter1Value();
    //double parameter2Backup = getParameter2Value();
    //double parameter3Backup = getParameter3Value();
    //double parameter4Backup = getParameter4Value();
    double currentValueBackup = getCurrentValue();

    if (theDistributionType == QString("Normal (mean, stdv)") || theDistributionType == QString("Exponential (mu, xo)") || theDistributionType == QString("Laplace (a, xo)") || theDistributionType == QString("Rayleigh (sigma, xo)") || theDistributionType == QString("Gamma (a, b)") || theDistributionType == QString("Uniform (a, b)") || theDistributionType == QString("Lognormal (zeta, sigma)") || theDistributionType == QString("Chi-square (nu, xo)") || theDistributionType == QString("Beta (a, b, min, max)") || theDistributionType == QString("Logistic (a, xo)") || theDistributionType == QString("Gumbel (a, b)")) {
        double originalCDF = getCDFValue(rvValue);

        // Perturb the mean
        double mean = meanBackup;
        double h = stdvBackup / 200.0;
        mean += h;
        setMeanValue(mean);

        // Compute the sensitivity with finite difference
        double perturbedCDF = getCDFValue(rvValue);
        *dCDFdMean = (perturbedCDF - originalCDF) / h;

        // Restore the mean
        mean = meanBackup;
        setMeanValue(mean);
        setCurrentValue(currentValueBackup);



        // Perturb the stdv
        double stdv = stdvBackup;
        stdv += h;
        setStandardDeviationValue(stdv);

        // Compute the sensitivity with finite difference
        perturbedCDF = getCDFValue(rvValue);
        *dCDFdStdv = (perturbedCDF - originalCDF) / h;
        stdv = stdvBackup;

        // Restore the stdv
        setStandardDeviationValue(stdv);
        setCurrentValue(currentValueBackup);
    }
    else if (theDistributionType == QString("Bounded Exponential (mu, xo, min, max)") || theDistributionType == QString("Linear (a, b)") || theDistributionType == QString("Weibull (a, b)") || theDistributionType == QString("User Defined")) {
        qCritical() << "Warning: The sensitivity of the CDF with respect to the mean and the standard deviation of the random variable" << objectName() << "cannot be computed. Setting the sensitivity to zero.";
        *dCDFdMean = 0.0;
        *dCDFdStdv = 0.0;
    }
    else {
        qCritical() << "Warning: Illegal probability distribution type has been passed to the random variable" << objectName() << ".";
        *dCDFdMean = 0.0;
        *dCDFdStdv = 0.0;
    }

    return 0;
}


QList<RParameter *> RContinuousRandomVariable::getParameterList()
{
    QList<RParameter *> theParameterList;
    if (theMeanParameter) {
        theParameterList << theMeanParameter;
    }
    if (theStandardDeviationParameter) {
        theParameterList << theStandardDeviationParameter;
    }
    if (theCoefficientOfVariationParameter) {
        theParameterList << theCoefficientOfVariationParameter;
    }
    if (theParameter1Parameter) {
        theParameterList << theParameter1Parameter;
    }
    if (theParameter2Parameter) {
        theParameterList << theParameter2Parameter;
    }
    if (theParameter3Parameter) {
        theParameterList << theParameter3Parameter;
    }
    if (theParameter4Parameter) {
        theParameterList << theParameter4Parameter;
    }
    return theParameterList;
}


void RContinuousRandomVariable::plot(QString type)
{
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    QCustomPlot *customPlot = new QCustomPlot();
    mainWindow->addSubWindow(customPlot, QString("%1 Plot").arg(type), true);

    customPlot->addGraph();

    QString xAxisLabel;
    if (type == QString("Inverse CDF")) {
        xAxisLabel = "Probability";
    } else {
        xAxisLabel = "Random Variable";
    }

    customPlot->xAxis->setLabel(xAxisLabel);
    customPlot->yAxis->setLabel(type);

    QVector<double> xVector;
    QVector<double> yVector;

    double mean = getMeanValue();
    double standardDeviation = getStandardDeviationValue();
    int pointsCount = 100;

    for (int i = 0; i <= pointsCount; i++) {
        double x = (2.0 * i - pointsCount) / pointsCount * 3.0 * standardDeviation + mean;
        double y;
        if (type == QString("PDF")) {
            y = getPDFValue(x);
        } else if (type == QString("CDF")) {
            y = getCDFValue(x);
        } else if (type == QString("Inverse CDF")) {
            if ((i == 0) || (i == pointsCount)) {
                continue;
            }
            x = 1.0 * i / pointsCount;
            y = getInverseCDFValue(x);
        }
        xVector << x;
        yVector << y;
    }

    customPlot->graph(0)->setData(xVector, yVector);

    customPlot->graph(0)->rescaleAxes();

    customPlot->replot();
}


void RContinuousRandomVariable::setDynamicProperty(QString propertyName, QVariant propertyValue)
{
    if (QString(propertyName) == QString("XPoints")) {
        XVector = stringToDoubleVector(propertyValue.toString());
        QString temp = doubleVectorToString(XVector);
        if (propertyValue != temp) {
            this->setProperty("XPoints", temp);
        }
    } else if (QString(propertyName) == QString("PDFPoints")) {
        PDFVector = stringToDoubleVector(propertyValue.toString());
        QString temp = doubleVectorToString(PDFVector);
        if (propertyValue != temp) {
            this->setProperty("PDFPoints", temp);
        }
    }
    if (!XVector.isEmpty() && !PDFVector.isEmpty() && XVector.count() == PDFVector.count()) {
        setUserDefinedPDF(XVector, PDFVector, true);
    }
}


void RContinuousRandomVariable::setUserDefinedPDF(QVector<double> passedXVector, QVector<double> passedPDFVector, bool viaGUI)
{
    XVector = passedXVector;
    PDFVector = passedPDFVector;
    updateMeanStdv();
    setCurrentValue(getMeanValue());
    double lastCDF = getCDFValue(XVector.last());
    if (qAbs(lastCDF - 1.0) > 1e-10) {
        for (int i = 0; i < PDFVector.count(); i++) {
            PDFVector[i] /= lastCDF;
        }
        updateMeanStdv();
        setCurrentValue(getMeanValue());

        if (viaGUI) {
            qCritical() << "Warning: The total area under the given PDF curve of the user-defined random variable" << objectName() << "is not equal to 1.0. Rts will normalize the given PDF.";
            this->setProperty("PDFPoints", doubleVectorToString(PDFVector));
        }
    }
}


int RContinuousRandomVariable::userDefinedErrorCheck()
{
    if (XVector.isEmpty() || PDFVector.isEmpty()) {
        qCritical() << "Error: The X points and/or the PDF points in the user-defined random variable" << objectName() << "is empty. The distribution is returned as 0.0.";
        return -1;
    }
    if (XVector.count() != PDFVector.count()) {
        qCritical() << "Error: The number of X points and PDF points in the user-defined random variable" << objectName() << "are not equal. The distribution is returned as 0.0.";
        return -1;
    }
    return 0;
}


QStringList RContinuousRandomVariable::getActionList()
{
    QStringList actionList;
    actionList << "Plot &PDF" << "Plot &CDF" << "Plot &Inverse CDF" << "Separator" << "&Get PDF" << "G&et CDF" << "Ge&t Inverse CDF" << "Separator" << "&Help";
    return actionList;
}


void RContinuousRandomVariable::plotPDF()
{
    plot("PDF");
}


void RContinuousRandomVariable::plotCDF()
{
    plot("CDF");
}


void RContinuousRandomVariable::plotInverseCDF()
{
    plot("Inverse CDF");
}


void RContinuousRandomVariable::get(QString type)
{
    bool ok;
    QString text = QInputDialog::getText(nullptr, tr("Distribution"), tr("Enter value"), QLineEdit::Normal, "", &ok);
    double value = 0.0;
    if (ok && !text.isEmpty()) {
        value = text.toDouble(&ok);
    }
    if (ok) {
        if (type == QString("PDF")) {
            qDebug() << "The PDF of the random variable" << objectName() << "at" << value << "is" << getPDFValue(value);
        } else if (type == QString("CDF")) {
            qDebug() << "The CDF of the random variable" << objectName() << "at" << value << "is" << getCDFValue(value);
        } else if (type == QString("Inverse CDF")) {
            qDebug() << "The inverse CDF of the random variable" << objectName() << "at" << value << "is" << getInverseCDFValue(value);
        }
    }
}


void RContinuousRandomVariable::getPDF()
{
    get("PDF");
}


void RContinuousRandomVariable::getCDF()
{
    get("CDF");
}


void RContinuousRandomVariable::getInverseCDF()
{
    get("Inverse CDF");
}


void RContinuousRandomVariable::createDefaultDistributions()
{
    // ********** CREATE THE BEST TOOL IN THE BUSINESS ************
    QString theDefaultName = "defaultInHouseProbabilityDistributions";

    // Look for default distributions
    bool foundDefaultTool = false;
    QList<RProbabilityDistributions *> theProbDistList = theDomain->findChildren<RProbabilityDistributions *>();
    for (int i=0; i<theProbDistList.size(); i++) {

        if (theProbDistList[i]->objectName() == theDefaultName) {
            theProbabilityDistributions = theProbDistList[i];
            foundDefaultTool = true;
        }
    }

    // Create the tool if needed
    if (!foundDefaultTool) {
#ifdef GSL
        theDomain->createObject(theDefaultName, "RGSLProbabilityDistributions");
        theProbabilityDistributions = qobject_cast<RProbabilityDistributions *>(theDomain->getLastAddedObject());
#else
        theDomain->createObject(theDefaultName, "RInHouseProbabilityDistributions");
        theProbabilityDistributions = qobject_cast<RProbabilityDistributions *>(theDomain->getLastAddedObject());
#endif
    }

}
