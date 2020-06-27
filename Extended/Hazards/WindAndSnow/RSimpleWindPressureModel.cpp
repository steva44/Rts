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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RSimpleWindPressureModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RDomain.h"

#include <math.h>

RSimpleWindPressureModel::RSimpleWindPressureModel(QObject *parent, QString name)
    : RWindBaseClass(parent, name)
{
	// Initializing the data members
    theVelocity = nullptr;
    theAirDensity = nullptr;
    theExposureFactor = nullptr;
    theExternalPressureFactor = nullptr;
    theGustFactor = nullptr;
    theEpsilon = nullptr;
    theOccurrence =nullptr;
	
	// Instantiating the response object
	theDomain->createObject(objectName() + "Response", "RGenericResponse");
	theWindPressure = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
	theWindPressure->setModel(this);
}


RSimpleWindPressureModel::~RSimpleWindPressureModel()
{

}


QObject* RSimpleWindPressureModel::getOccurrence() const
{
    return theOccurrence;
}


void RSimpleWindPressureModel::setOccurrence(QObject *value)
{
    theOccurrence = qobject_cast<RParameter *>(value);
}
	

QObject * RSimpleWindPressureModel::getVelocity() const	
{	
	return theVelocity;
}	
	

void RSimpleWindPressureModel::setVelocity(QObject *value)	
{	
	theVelocity = qobject_cast<RParameter *>(value);
}	
	

QObject * RSimpleWindPressureModel::getAirDensity() const	
{	
	return theAirDensity;
}	
	

void RSimpleWindPressureModel::setAirDensity(QObject *value)	
{	
	theAirDensity = qobject_cast<RParameter *>(value);
}	


QObject * RSimpleWindPressureModel::getExposureFactor() const	
{	
	return theExposureFactor;
}	
	

void RSimpleWindPressureModel::setExposureFactor(QObject *value)	
{	
	theExposureFactor = qobject_cast<RParameter *>(value);
}	
	

QObject * RSimpleWindPressureModel::getExternalPressureFactor() const	
{	
	return theExternalPressureFactor;
}	
	

void RSimpleWindPressureModel::setExternalPressureFactor(QObject *value)	
{	
	theExternalPressureFactor = qobject_cast<RParameter *>(value);
}	
	

QObject * RSimpleWindPressureModel::getGustFactor() const	
{	
	return theGustFactor;
}	
	

void RSimpleWindPressureModel::setGustFactor(QObject *value)	
{	
	theGustFactor = qobject_cast<RParameter *>(value);
}	
	

QObject * RSimpleWindPressureModel::getEpsilon() const	
{	
	return theEpsilon;
}	
	

void RSimpleWindPressureModel::setEpsilon(QObject *value)	
{	
	theEpsilon = qobject_cast<RParameter *>(value);
}	


int RSimpleWindPressureModel::evaluateModel(RGradientType theGradientType)
{	
    if (theOccurrence->getCurrentValue() == 0.0) {
        theWindPressure->setCurrentValue(0.0);
        return 0;
    }

	// Formulation from:  Bartlett, F M.; Hong, H P.; Zhou, W, "Load factor calibration for the proposed 2005 edition of the National Building Code of Canada: Statistics of loads and load effects", Canadian Journal of Civil Engineering, Volume 30, Number 2, 1 April 2003 , pp. 429-439(11).
	double velocity = theVelocity->getCurrentValue();

	double roh = theAirDensity->getCurrentValue();
	double exposureFactor = theExposureFactor->getCurrentValue();
	double externalPressureFactor = theExternalPressureFactor->getCurrentValue();
	double gustFactor = theGustFactor->getCurrentValue();
	double epsilon = theEpsilon->getCurrentValue();

	double q = 0.5 * roh * velocity * velocity;
	double windPressure = q * exposureFactor * externalPressureFactor * gustFactor;
	windPressure *= epsilon;

    // ATTENTION: SHOULD MAKE SURE THAT A ZERO MAGNITUDE (VELOCITY) RESULTS IN A ZERO INTENSITY (PRESSURE). IN THIS CASE, THIS RELATION IS IN PLACE.

	theWindPressure->setCurrentValue(windPressure);

    if (theGradientType >= RGradientType::DDM)
    {
        double dMagnitudedVelocity = 0.0;
        double dMagnitudedroh = 0.0;
        double dMagnitudedexposureFactor = 0.0;
        double dMagnitudedexternalPressureFactor = 0.0;
        double dMagnitudedgustFactor = 0.0;
        double dMagnitudedepsilon = 0.0;

        QMap<RParameter *, double> theWindPressureGradientMap;
        theWindPressureGradientMap.insertMulti(theVelocity, dMagnitudedVelocity);
        theWindPressureGradientMap.insertMulti(theAirDensity, dMagnitudedroh);
        theWindPressureGradientMap.insertMulti(theExposureFactor, dMagnitudedexposureFactor);
        theWindPressureGradientMap.insertMulti(theExternalPressureFactor, dMagnitudedexternalPressureFactor);
        theWindPressureGradientMap.insertMulti(theGustFactor, dMagnitudedgustFactor);
        theWindPressureGradientMap.insertMulti(theEpsilon, dMagnitudedepsilon);

        theDDMMap.insertMulti(theWindPressure, theWindPressureGradientMap);
    }

	return 0;
}
