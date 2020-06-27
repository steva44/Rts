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

#include "RFixedSourceModel.h"	
#include "RLocation.h"
#include "RParameter.h"	
#include "RResponse.h"	
#include "RLocationResponse.h"
#include "RMainWindow.h"
#include "RDomain.h"

#include <math.h>

RFixedSourceModel::RFixedSourceModel(QObject *parent, QString name)	
	: RModel(parent, name)
{	
    theLocation = nullptr;
	
	// Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RLocationResponse");
    theHypocentreLocation = qobject_cast<RLocationResponse *>(theDomain->getLastAddedObject());
    theHypocentreLocation->setModel(this);
}	


RFixedSourceModel::~RFixedSourceModel()	
{	
	
}	


QObject * RFixedSourceModel::getLocation() const
{	
    return theLocation;
}	


void RFixedSourceModel::setLocation(QObject *value)
{	
    theLocation = qobject_cast<RParameter *>(value);
}	


QObject * RFixedSourceModel::getDepth() const	
{	
	return theDepth;
}	


void RFixedSourceModel::setDepth(QObject *value)	
{	
	theDepth = qobject_cast<RParameter *>(value);
}	


int RFixedSourceModel::evaluateModel(RGradientType theGradientType)	
{	
    RLocation *theLocationObject = parameterToLocation(theLocation);

    RLocation *theHypocentreLocationObject = qobject_cast<RLocation *>(theHypocentreLocation->getLocation());
    if (!theHypocentreLocationObject) {
        qCritical() << "Error: The Location response" << theHypocentreLocation->objectName() << "does not have a Location object.";
		return -1;
	}

    theHypocentreLocationObject->setLatitude(theLocationObject->getLatitude());

    theHypocentreLocationObject->setLongitude(theLocationObject->getLongitude());

    theHypocentreLocationObject->setAltitude(-1.0 * theDepth->getCurrentValue());

    theHypocentreLocationObject->redisplayonMap();


	return 1;
}	


QStringList RFixedSourceModel::getActionList()
{
	QStringList actionList;
	actionList << "&Display on Map" << "&Remove from Map";
	return actionList;
}


void RFixedSourceModel::displayonMap()
{
    /*
    QList<RLocation *> LocationList;
    LocationList << parameterToLocation(theLocation);
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


void RFixedSourceModel::removefromMap()
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
