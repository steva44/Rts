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

#include "RRandomVariableWithRandomParametersModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include <math.h>

RRandomVariableWithRandomParametersModel::RRandomVariableWithRandomParametersModel(QObject *parent, QString name)
	: RModel(parent, name)
{
    theStandardNormalRandomVariable = nullptr;
	theDistributionType = "Normal (mean, stdv)";
	theRandomParameterType = RRandomVariableWithRandomParametersModel::Parameter1ToParameter4;
    theProbabilityDistributions = nullptr;
    theOccurrence = nullptr;

	// Instantiating the output response theResponse
	theDomain->createObject(objectName() + "Response", "RGenericResponse");
	theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
	theResponse->setModel(this);
}


RRandomVariableWithRandomParametersModel::~RRandomVariableWithRandomParametersModel()
{

}


QObject * RRandomVariableWithRandomParametersModel::getProbabilityDistributions() const
{
    return theProbabilityDistributions;
}


void RRandomVariableWithRandomParametersModel::setProbabilityDistributions(QObject *value)
{
    theProbabilityDistributions = qobject_cast<RProbabilityDistributions *>(value);
}


QObject * RRandomVariableWithRandomParametersModel::getStandardNormalRandomVariable() const
{
	return theStandardNormalRandomVariable;
}


void RRandomVariableWithRandomParametersModel::setStandardNormalRandomVariable(QObject *value)
{
	theStandardNormalRandomVariable = qobject_cast<RParameter *>(value);

	/*// Check that the user has given a standard random variable:
	RContinuousRandomVariable *theRV = qobject_cast<RContinuousRandomVariable *>(value);  
	if (!theRV) {
		qCritical() << "Warning: The parameter given to the" << objectName() << "as the StandardNormalRandomVariable must be a continuous standard normal random variable.";
		return;
	}

	QString type = theRV->getDistributionType();
	double mean = theRV->getParameter1Value();
	double stdv = theRV->getParameter2Value();
	if ((type != QString("Normal (mean, stdv)")) || (mean != 0.0) || (stdv != 1.0)) {
		qCritical() << "Warning: The random variable given to the" << objectName() << "as the StandardNormalRandomVariable must be a standard normal random variable.";
	}*/
}


QString RRandomVariableWithRandomParametersModel::getDistributionType() const	
{	
	return theDistributionType;
}	


void RRandomVariableWithRandomParametersModel::setDistributionType(QString value)	
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
}	


void RRandomVariableWithRandomParametersModel::setRandomParameterType(RRandomVariableWithRandomParametersModel::RRandomParameterType value)	
{	
	theRandomParameterType = value;
}	


RRandomVariableWithRandomParametersModel::RRandomParameterType RRandomVariableWithRandomParametersModel::getRandomParameterType() const	
{	
	return theRandomParameterType;
}	


QString RRandomVariableWithRandomParametersModel::getParameterList()
{
	return parameterListToString(theParameterList);
}


void RRandomVariableWithRandomParametersModel::setParameterList(QString value)
{
	theParameterList = stringToParameterList(value);
}


QList<RParameter *> RRandomVariableWithRandomParametersModel::getPhysicalParameterList()
{
	QList<RParameter *> paramList;
	paramList << theStandardNormalRandomVariable;
	return paramList;
}


QObject * RRandomVariableWithRandomParametersModel::getOccurrence() const
{
    return theOccurrence;
}


void RRandomVariableWithRandomParametersModel::setOccurrence(QObject *value)
{
    theOccurrence = qobject_cast<RParameter *>(value);
}


int RRandomVariableWithRandomParametersModel::evaluateModel(RGradientType theGradientType)
{	
    // If the hazard is not active, end this quickly
    if (theOccurrence->getCurrentValue() == 0.0)
    {
        theResponse->setCurrentValue(0.0);

        return 0;
    }

    //qCritical() << "currentTime=" << currentTime << "  value=" << QString("%1").arg(currentTime, 0, 'g', 14);

	// Clearing null pointers
	clearNullPointers(&theParameterList);

	// Calculate the probability of the model StandardNormalRandomVariable
	double realization = theStandardNormalRandomVariable->getCurrentValue();
    double probability = theProbabilityDistributions->getNormalCDF(realization, 1.0);

	RContinuousRandomVariable randomVariable(0, QString("RV"));
	randomVariable.setDistributionType(theDistributionType);

	if (theDistributionType == QString("User Defined")) {
		randomVariable.setDynamicProperty("XPoints", this->property("XPoints"));
		randomVariable.setDynamicProperty("PDFPoints", this->property("PDFPoints"));
	} else if (theRandomParameterType == RRandomVariableWithRandomParametersModel::Parameter1ToParameter4) {
		if (theParameterList.count() >= 1) {
			double parameterValue = theParameterList[0]->getCurrentValue();
			randomVariable.setParameter1(QString("%1").arg(parameterValue));
		}
		if (theParameterList.count() >= 2) {
			double parameterValue = theParameterList[1]->getCurrentValue();
			randomVariable.setParameter2(QString("%1").arg(parameterValue));
		}
		if (theParameterList.count() >= 3) {
			double parameterValue = theParameterList[2]->getCurrentValue();
			randomVariable.setParameter3(QString("%1").arg(parameterValue));
		}
		if (theParameterList.count() >= 4) {
			double parameterValue = theParameterList[3]->getCurrentValue();
			randomVariable.setParameter4(QString("%1").arg(parameterValue));
		}
	} else if (theRandomParameterType == RRandomVariableWithRandomParametersModel::MeanStdvParameter3Parameter4) {
		if (theParameterList.count() >= 3) {
			double parameterValue = theParameterList[2]->getCurrentValue();
			randomVariable.setParameter3(QString("%1").arg(parameterValue));
		}
		if (theParameterList.count() >= 4) {
			double parameterValue = theParameterList[3]->getCurrentValue();
			randomVariable.setParameter4(QString("%1").arg(parameterValue));
		}
		if (theParameterList.count() >= 1) {
			double parameterValue = theParameterList[0]->getCurrentValue();
			randomVariable.setMean(QString("%1").arg(parameterValue));
		}
		if (theParameterList.count() >= 2) {
			double parameterValue = theParameterList[1]->getCurrentValue();
			randomVariable.setStandardDeviation(QString("%1").arg(parameterValue));
		}
	} 

	double response = randomVariable.getInverseCDFValue(probability);

	// Updating the response current value
	theResponse->setCurrentValue(response);

	return 0;
}


void RRandomVariableWithRandomParametersModel::setDynamicProperty(QString propertyName, QVariant propertyValue)
{
	if (QString(propertyName) == QString("XPoints")) {
		QVector<double> XVector = stringToDoubleVector(propertyValue.toString());
		QString temp = doubleVectorToString(XVector);
		if (propertyValue != temp) {
			this->setProperty("XPoints", temp);
		}
	} else if (QString(propertyName) == QString("PDFPoints")) {
		QVector<double> PDFVector = stringToDoubleVector(propertyValue.toString());
		QString temp = doubleVectorToString(PDFVector);
		if (propertyValue != temp) {
			this->setProperty("PDFPoints", temp);
		}
	}
}
