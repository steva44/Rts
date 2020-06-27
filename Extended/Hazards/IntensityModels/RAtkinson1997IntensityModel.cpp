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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RAtkinson1997IntensityModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RDomain.h"
#include "RLocation.h"
#include "RLocationResponse.h"

#include <math.h>

RAtkinson1997IntensityModel::RAtkinson1997IntensityModel(QObject *parent, QString name)
    : REarthquakeBaseClass(parent, name)
{
    // Initializing the pointer data members
    theMagnitude = nullptr;
    theEpsilon = nullptr;
    theHypocentreLocation = nullptr;
    theC0Uncertainty = nullptr;
    theC1Uncertainty = nullptr;
    theC2Uncertainty = nullptr;
    theC3Uncertainty = nullptr;
    theCa3Uncertainty = nullptr;
    theCa4Uncertainty = nullptr;
    theCsigmaUncertainty = nullptr;
    theCsUncertainty = nullptr;

    theSoilType = RAtkinson1997IntensityModel::Rock;

    // Create the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RAtkinson1997IntensityModel::~RAtkinson1997IntensityModel()
{

}


QObject * RAtkinson1997IntensityModel::getHypocentreLocation() const
{
    return theHypocentreLocation;
}


void RAtkinson1997IntensityModel::setHypocentreLocation(QObject *value)
{
    theHypocentreLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theHypocentreLocation);
}


QObject * RAtkinson1997IntensityModel::getMagnitude() const
{
    return theMagnitude;
}


void RAtkinson1997IntensityModel::setMagnitude(QObject *value)
{
    theMagnitude = qobject_cast<RParameter *>(value);
}


RAtkinson1997IntensityModel::RSoilType RAtkinson1997IntensityModel::getSoilType() const
{
    return theSoilType;
}


void RAtkinson1997IntensityModel::setSoilType(RAtkinson1997IntensityModel::RSoilType value)
{
    theSoilType = value;
}


QObject * RAtkinson1997IntensityModel::getEpsilon() const
{
    return theEpsilon;
}


void RAtkinson1997IntensityModel::setEpsilon(QObject *value)
{
    theEpsilon = qobject_cast<RParameter *>(value);
}


QObject * RAtkinson1997IntensityModel::getC0Uncertainty() const
{
    return theC0Uncertainty;
}


void RAtkinson1997IntensityModel::setC0Uncertainty(QObject *value)
{
    theC0Uncertainty = qobject_cast<RParameter *>(value);
}


QObject * RAtkinson1997IntensityModel::getC1Uncertainty() const
{
    return theC1Uncertainty;
}


void RAtkinson1997IntensityModel::setC1Uncertainty(QObject *value)
{
    theC1Uncertainty = qobject_cast<RParameter *>(value);
}


QObject * RAtkinson1997IntensityModel::getC2Uncertainty() const
{
    return theC2Uncertainty;
}


void RAtkinson1997IntensityModel::setC2Uncertainty(QObject *value)
{
    theC2Uncertainty = qobject_cast<RParameter *>(value);
}


QObject * RAtkinson1997IntensityModel::getC3Uncertainty() const
{
    return theC3Uncertainty;
}


void RAtkinson1997IntensityModel::setC3Uncertainty(QObject *value)
{
    theC3Uncertainty = qobject_cast<RParameter *>(value);
}


QObject * RAtkinson1997IntensityModel::getCa3Uncertainty() const
{
    return theCa3Uncertainty;
}


void RAtkinson1997IntensityModel::setCa3Uncertainty(QObject *value)
{
    theCa3Uncertainty = qobject_cast<RParameter *>(value);
}


QObject * RAtkinson1997IntensityModel::getCa4Uncertainty() const
{
    return theCa4Uncertainty;
}


void RAtkinson1997IntensityModel::setCa4Uncertainty(QObject *value)
{
    theCa4Uncertainty = qobject_cast<RParameter *>(value);
}


QObject * RAtkinson1997IntensityModel::getCsigmaUncertainty() const
{
    return theCsigmaUncertainty;
}


void RAtkinson1997IntensityModel::setCsigmaUncertainty(QObject *value)
{
    theCsigmaUncertainty = qobject_cast<RParameter *>(value);
}


QObject * RAtkinson1997IntensityModel::getCsUncertainty() const
{
    return theCsUncertainty;
}


void RAtkinson1997IntensityModel::setCsUncertainty(QObject *value)
{
    theCsUncertainty = qobject_cast<RParameter *>(value);
}


bool RAtkinson1997IntensityModel::isOn()
{
    double M = theMagnitude->getCurrentValue();

    if (M != 0.0) {
        return true;
    }
    else {
        return false;
    }
}


int RAtkinson1997IntensityModel::evaluateModel(RGradientType theGradientType)
{
    // First check that we a location where the intensity shall be computed
    if (!theLocation) {
        theResponse->setCurrentValue(0.0);
        return 1;
    }

    // For convenice, look up the values of the parameters first
    double M = theMagnitude->getCurrentValue();
    double S = 0;
    if (theSoilType == RAtkinson1997IntensityModel::Rock) {
        S = 0;
    } else if (theSoilType == RAtkinson1997IntensityModel::FirmSoil) {
        S = 1;
    }

    double sigma = theEpsilon->getCurrentValue();

    double c0Uncertainty = theC0Uncertainty->getCurrentValue();
    double c1Uncertainty = theC1Uncertainty->getCurrentValue();
    double c2Uncertainty = theC2Uncertainty->getCurrentValue();
    double c3Uncertainty = theC3Uncertainty->getCurrentValue();
    double ca3Uncertainty = theCa3Uncertainty->getCurrentValue();
    double ca4Uncertainty = theCa4Uncertainty->getCurrentValue();
    double csigmaUncertainty = theCsigmaUncertainty->getCurrentValue();
    double csUncertainty = theCsUncertainty->getCurrentValue();


    // Verification that the user has given an RLocationResponse
    RLocation *theHypocentreLocationObject = parameterToLocation(theHypocentreLocation);
    if (!theHypocentreLocationObject) {
        qCritical() << "Error: The Location response" << theHypocentreLocation->objectName() << "does not include any Location object.";
        return -1;
    }

    // Calculating h
    double h = qAbs(theHypocentreLocationObject->getAltitude());


    // Declare variables to be used below
    double logPSA;


    // First check whether the magnitude is greater than 8
    if (M>8.0) {
        M = 8.0;
    }

    // Calculating R
    double R = theHypocentreLocationObject->computeSurfaceDistance(theLocation);
    double T = thePeriod;

    // Compute the natural frequency
    double f = 1.0/T;

    double PSA;


    double c0 = -0.035721735 * f*f + 0.450333079 * f + (-0.363220796*M+4.504677765) + c0Uncertainty * 0.114018678;
    double c1 = 0.006940234 * f*f - 0.099194239 * f + 0.691458651 + c1Uncertainty * 0.039043779;
    double c2 = -0.00087448 * f*f + 0.01172348 * f - 0.149527434 + c2Uncertainty * 0.017587579;
    double c3 = -2.75373e-5 * f*f*f*f + 0.000620298 * f*f*f - 0.004613233 * f*f + 0.013309472 *f - 0.005367818 + c3Uncertainty * 0.00124175;
    double ca3 = (1.65694e-5*M-0.000179439) * f*f + (-0.00028233*M+0.002859206) * f + (0.000457772*M-0.003509723) + ca3Uncertainty *  (0.00033395 + 0.000266524) / 2.0;
    double ca4 = (1.08039e-7*M-8.64311e-7) * f*f*f*f + (-2.47397e-6*M+1.97918e-5) * f*f*f + (1.8202e-5*M-0.000145616) * f*f + (-4.54508e-5*M+0.000363606) *f + (1.72283e-6*M-1.37826e-5) + ca4Uncertainty * (8.14736E-06 + 0.0) / 2.0;
    double csigma = (-0.007509084*M+0.051049657) * f + (0.035196642*M+0.044869466) + csigmaUncertainty * (0.013917106 + 0.037899501) / 2.0;
    double cs = 0.007649599 * f*f - 0.119314869 * f + 0.683941418 + csUncertainty * 0.021367993;
    double ca1 = -0.25*M+2.5;

    logPSA = c0 + c1*(M - 6) + c2*(M-6)*(M-6) + c3*h - ca1*log10(R) - (ca3 + ca4*h)*R + cs*S + csigma*sigma;//???????????????????????

    // Assign the PSA value to the response object of this model (converted to g)
    PSA = pow(10.0, logPSA) / 981.0;

    theResponse->setCurrentValue(PSA);

    return 1;
}

