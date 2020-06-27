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

#include "RSingleLineSourceModel.h"	
#include "RLocation.h"
#include "RParameter.h"	
#include "RResponse.h"	
#include "RLocationResponse.h"
#include "RMainWindow.h"
#include "RDomain.h"

#include <math.h>

RSingleLineSourceModel::RSingleLineSourceModel(QObject *parent, QString name)	
	: RModel(parent, name)
{	
	// Initializing the pointer data members
    thePoint1Location = nullptr;
    thePoint2Location = nullptr;
    theTheta = nullptr;
    theDepth = nullptr;
	
	// Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RLocationResponse");
    theHypocentreLocation = qobject_cast<RLocationResponse *>(theDomain->getLastAddedObject());
    theHypocentreLocation->setModel(this);
}	


RSingleLineSourceModel::~RSingleLineSourceModel()	
{	
	
}	


QObject * RSingleLineSourceModel::getPoint1Location() const
{	
    return thePoint1Location;
}	


void RSingleLineSourceModel::setPoint1Location(QObject *value)
{	
    thePoint1Location = qobject_cast<RParameter *>(value);
}	


QObject * RSingleLineSourceModel::getPoint2Location() const
{	
    return thePoint2Location;
}	


void RSingleLineSourceModel::setPoint2Location(QObject *value)
{	
    thePoint2Location = qobject_cast<RParameter *>(value);
}	


QObject * RSingleLineSourceModel::getTheta() const	
{	
	return theTheta;
}	


QObject * RSingleLineSourceModel::getDepth() const	
{	
	return theDepth;
}	


void RSingleLineSourceModel::setTheta(QObject *value)	
{	
	theTheta = qobject_cast<RParameter *>(value);

	/*// Check that the user has given a uniform random variable between 0 and 1:
	RContinuousRandomVariable *theRV = qobject_cast<RContinuousRandomVariable *>(value);  
	if (!theRV) {
		qCritical() << "Warning: The parameter given to the Line Source Model must be a continuous random variable.";
		return;
	}

	QString type = theRV->getDistributionType();
	double a = theRV->getParameter1Value();
	double b = theRV->getParameter2Value();
	if ((type != QString("Uniform (a, b)")) || (a != 0.0) || (b != 1.0)) {
		qCritical() << "Warning: The random variable given to the Line Source Model must be uniformly distributed between 0 and 1.";
	}*/
}	


void RSingleLineSourceModel::setDepth(QObject *value)	
{	
	theDepth = qobject_cast<RParameter *>(value);
}	


QList<RParameter *> RSingleLineSourceModel::getPhysicalParameterList()
{
	QList<RParameter *> paramList;
	paramList << theTheta << theDepth;
	return paramList;
}


int RSingleLineSourceModel::evaluateModel(RGradientType theGradientType)	
{	
    RLocation *thePoint1LocationObject = parameterToLocation(thePoint1Location);
    RLocation *thePoint2LocationObject = parameterToLocation(thePoint2Location);

	double theta = theTheta->getCurrentValue();

    // Getting the pointer to the RLocation object from the RLocationResponse object (theHypocentreLocation)
    RLocation *theHypocentreLocationObject = qobject_cast<RLocation *>(theHypocentreLocation->getLocation());
    if (!theHypocentreLocationObject) {
        qCritical() << "Error: The Location response" << theHypocentreLocation->objectName() << "does not include any Location object.";
		return -1;
	}
	
	
	// Implementation assuming that the earth is flat!
    /*double long1 = thePoint1LocationObject->getLongitude();
    double lat1 = thePoint1LocationObject->getLatitude();
		
    double long2 = thePoint2LocationObject->getLongitude();
    double lat2 = thePoint2LocationObject->getLatitude();

	double delta_long = long2-long1;
	double delta_lat = lat2-lat1;

    double LocationLat = lat1 + delta_lat * theta;
    double LocationLng = long1 + delta_long * theta;*/



	// Implementation assuming that the earth is spherical
    thePoint1LocationObject->computeIntermediatePoint(thePoint2LocationObject, theHypocentreLocationObject, theta);
	


    theHypocentreLocationObject->setAltitude(-1.0 * theDepth->getCurrentValue());


    theHypocentreLocationObject->redisplayonMap();


    if (theGradientType >= RGradientType::DDM) {
		double dLat_dTheta;
		double dLat_dDepth;
		double dLng_dTheta;
		double dLng_dDepth;
		double dAlt_dTheta;
		double dAlt_dDepth;


		// Implementation assuming that the earth is flat!
		/*dLat_dTheta = delta_lat;
		dLng_dTheta = delta_long;*/

		// Implementation assuming that the earth is spherical
        thePoint1LocationObject->computeIntermediatePointGradient(thePoint2LocationObject, theta, &dLat_dTheta, &dLng_dTheta);



		dLat_dDepth = 0.0;
		dLng_dDepth = 0.0;
		dAlt_dTheta = 0.0;
		dAlt_dDepth = -1.0;


		QMap<RParameter *, double> theLatGradientMap;
		theLatGradientMap.insertMulti(theTheta, dLat_dTheta);
		theLatGradientMap.insertMulti(theDepth, dLat_dDepth);
        theDDMMap.insertMulti(theHypocentreLocation, theLatGradientMap);

		QMap<RParameter *, double> theLngGradientMap;
		theLngGradientMap.insertMulti(theTheta, dLng_dTheta);
		theLngGradientMap.insertMulti(theDepth, dLng_dDepth);
        theDDMMap.insertMulti(theHypocentreLocation, theLngGradientMap);

		QMap<RParameter *, double> theAltGradientMap;
		theAltGradientMap.insertMulti(theDepth, dAlt_dDepth);
        theDDMMap.insertMulti(theHypocentreLocation, theAltGradientMap);
	}

	return 1;
}	


QStringList RSingleLineSourceModel::getActionList()
{
	QStringList actionList;
	actionList << "&Display on Map" << "&Remove from Map";
	return actionList;
}


void RSingleLineSourceModel::displayonMap()
{
    /*
    QList<RLocation *> LocationList;
    LocationList << parameterToLocation(thePoint1Location);
    LocationList << parameterToLocation(thePoint2Location);
    if (!LocationList[0] || !LocationList[1]) {
        qCritical() << "Warning: Please enter the 2 Location objects in the model" << objectName() << ".";
		return;
	}
	RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
	RMapWidget *theMapWidget = theMainWindow->findChild<RMapWidget *>();
	theMainWindow->setActiveSubWindow(theMapWidget);
	if (!theMapWidget) {
        LocationList[0]->openMap();
		return;
	}
    theMapWidget->drawPolyline(this, LocationList, Qt::black);

    */
}


void RSingleLineSourceModel::removefromMap()
{
    /*
	RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
	RMapWidget *theMapWidget = theMainWindow->findChild<RMapWidget *>();
	if (theMapWidget) {
		theMainWindow->setActiveSubWindow(theMapWidget);
		theMapWidget->removePolyline(this);
	}
    */
}
