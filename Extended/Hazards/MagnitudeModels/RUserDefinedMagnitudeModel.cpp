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
*                                                                    *
*********************************************************************/

#include "RUserDefinedMagnitudeModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"

#include <math.h>

RUserDefinedMagnitudeModel::RUserDefinedMagnitudeModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theOccurrence = nullptr;

    // Instantiating the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theMagnitude = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theMagnitude->setModel(this);

    // Create the standard normal random variable
    theDomain->createObject(objectName() + "_theta", "RContinuousRandomVariable");
    theStandardNormalRandomVariable = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theStandardNormalRandomVariable->setDistributionType(QString("Normal (mean, stdv)"));
    theStandardNormalRandomVariable->setMean("0.0");
    theStandardNormalRandomVariable->setStandardDeviation("1.0");
    theAutoParametersList << theStandardNormalRandomVariable;
}


RUserDefinedMagnitudeModel::~RUserDefinedMagnitudeModel()
{
}


QString RUserDefinedMagnitudeModel::getXPoints()
{
    return theXPoints;
}


void RUserDefinedMagnitudeModel::setXPoints(QString value)
{
    theXPoints = value;
}


QString RUserDefinedMagnitudeModel::getPDFPoints()
{
    return thePDFPoints;
}


void RUserDefinedMagnitudeModel::setPDFPoints(QString value)
{
    thePDFPoints = value;
}


QObject * RUserDefinedMagnitudeModel::getOccurrence() const
{
    return theOccurrence;
}


void RUserDefinedMagnitudeModel::setOccurrence(QObject *value)
{
    theOccurrence = qobject_cast<RParameter *>(value);
}


int RUserDefinedMagnitudeModel::evaluateModel(RGradientType theGradientType)
{	
    if (theOccurrence->getCurrentValue() == 0.0) {

        theMagnitude->setCurrentValue(0.0);

        return 0;
    }


    // Create an auxiliary tool to get the normal distribution
    RNormalDistribution theNormalDistribution(theDomain);


    // Calculate the probability of the model StandardNormalRandomVariable
    double realization = theStandardNormalRandomVariable->getCurrentValue();
    double probability = theNormalDistribution.getNormalCDF(realization, 1.0);


    // Create a local random variable
    RContinuousRandomVariable randomVariable(0, QString("RV"));
    randomVariable.setDistributionType("User Defined");
    randomVariable.setProperty("XPoints", theXPoints);
    randomVariable.setProperty("PDFPoints", thePDFPoints);
    double response = randomVariable.getInverseCDFValue(probability);


    // Updating the response current value
    theMagnitude->setCurrentValue(response);

    return 0;
}
