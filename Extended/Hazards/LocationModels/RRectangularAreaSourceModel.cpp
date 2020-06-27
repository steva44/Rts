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

#include "RRectangularAreaSourceModel.h"
#include "RParameter.h"
#include "RLocationResponse.h"
#include "RContinuousRandomVariable.h"
#include "RLocation.h"
#include "RMainWindow.h"
#include "RDomain.h"

#include <math.h>

RRectangularAreaSourceModel::RRectangularAreaSourceModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    // Initializing the pointer data members
    thePoint1Location = nullptr;
    thePoint2Location = nullptr;
    theTheta1 = nullptr;
    theTheta2 = nullptr;
    theDepth = nullptr;

    // Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RLocationResponse");
    theHypocentreLocation = qobject_cast<RLocationResponse *>(theDomain->getLastAddedObject());
    theHypocentreLocation->setModel(this);
}


RRectangularAreaSourceModel::~RRectangularAreaSourceModel()
{

}


QObject * RRectangularAreaSourceModel::getPoint1Location() const
{
    return thePoint1Location;
}


void RRectangularAreaSourceModel::setPoint1Location(QObject *value)
{
    thePoint1Location = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(thePoint1Location);
}


QObject * RRectangularAreaSourceModel::getPoint2Location() const
{
    return thePoint2Location;
}


void RRectangularAreaSourceModel::setPoint2Location(QObject *value)
{
    thePoint2Location = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(thePoint2Location);
}


QObject * RRectangularAreaSourceModel::getTheta1() const
{
    return theTheta1;
}


void RRectangularAreaSourceModel::setTheta1(QObject *value)
{
    theTheta1 = qobject_cast<RParameter *>(value);
}


QObject * RRectangularAreaSourceModel::getTheta2() const
{
    return theTheta2;
}


void RRectangularAreaSourceModel::setTheta2(QObject *value)
{
    theTheta2 = qobject_cast<RParameter *>(value);
}


QObject * RRectangularAreaSourceModel::getDepth() const
{
    return theDepth;
}


void RRectangularAreaSourceModel::setDepth(QObject *value)
{
    theDepth = qobject_cast<RParameter *>(value);
}


QList<RParameter *> RRectangularAreaSourceModel::getPhysicalParameterList()
{
    QList<RParameter *> paramList;
    paramList << theTheta1 << theTheta2 << theDepth;
    return paramList;
}


int RRectangularAreaSourceModel::evaluateModel(RGradientType theGradientType)
{
    RLocation *thePoint1LocationObject = parameterToLocation(thePoint1Location);
    RLocation *thePoint2LocationObject = parameterToLocation(thePoint2Location);

    // Get information about the area source geometry
    double lng1 = thePoint1LocationObject->getLongitude();
    double lat1 = thePoint1LocationObject->getLatitude();
    double lng2 = thePoint2LocationObject->getLongitude();
    double lat2 = thePoint2LocationObject->getLatitude();

    double CC = 3.14159265359 / 180.0;
    double northLat = qMax(lat1, lat2) * CC;
    double southLat = qMin(lat1, lat2) * CC;
    double eastLng = qMax(lng1, lng2);
    double westLng = qMin(lng1, lng2);

    double deltaLng = eastLng - westLng;

    // Get information about the uncertainty in the hypocentre Location
    double theta1 = theTheta1->getCurrentValue();
    double theta2 = theTheta2->getCurrentValue();

    // Getting the pointer to the RLocation object from the RLocationResponse object (theHypocentreLocation)
    RLocation *theHypocentreLocationObject = parameterToLocation(theHypocentreLocation);
    if (!theHypocentreLocationObject) {
        qCritical() << "Error: The Location response" << theHypocentreLocation->objectName() << "does not include any Location object.";
        return -1;
    }

    double lat = asin(theta1 * (sin(northLat) - sin(southLat)) + sin(southLat)) / CC;
    double lng = westLng + deltaLng * theta2;
    double depth = -1.0 * theDepth->getCurrentValue();

    theHypocentreLocationObject->setLatitude(lat);
    theHypocentreLocationObject->setLongitude(lng);
    theHypocentreLocationObject->setAltitude(depth);

    if (theOutputLevel >= RObject::Medium) {
        qDebug() << qPrintable(QString("%1\t%2").arg(theHypocentreLocationObject->getLongitude(), 0, 'g', 12).arg(theHypocentreLocationObject->getLatitude(), 0, 'g', 12));
    }

    theHypocentreLocationObject->redisplayonMap();

    if (theGradientType >= RGradientType::DDM) {
        double dLat_dTheta1 = (sin(northLat) - sin(southLat)) / CC / sqrt(1.0 - (theta1 * (sin(northLat) - sin(southLat)) + sin(southLat))*(theta1 * (sin(northLat) - sin(southLat)) + sin(southLat)));
        double dLat_dTheta2 = 0.0;
        double dLng_dTheta1 = 0.0;
        double dLng_dTheta2 = deltaLng;

        double dAlt_dDepth = -1.0;

        QMap<RParameter *, double> theLatGradientMap;
        theLatGradientMap.insertMulti(theTheta1, dLat_dTheta1);
        theLatGradientMap.insertMulti(theTheta2, dLat_dTheta2);
        theDDMMap.insertMulti(theHypocentreLocation, theLatGradientMap);

        QMap<RParameter *, double> theLngGradientMap;
        theLngGradientMap.insertMulti(theTheta1, dLng_dTheta1);
        theLngGradientMap.insertMulti(theTheta2, dLng_dTheta2);
        theDDMMap.insertMulti(theHypocentreLocation, theLngGradientMap);

        QMap<RParameter *, double> theAltGradientMap;
        theAltGradientMap.insertMulti(theDepth, dAlt_dDepth);
        theDDMMap.insertMulti(theHypocentreLocation, theAltGradientMap);

        /*// Finite difference check
        {
            theta1 *= 1.001;
            double newlat = asin(theta1 * (sin(northLat) - sin(southLat)) + sin(southLat)) / CC;
            double newlng = westLng + deltaLng * theta2;
            double dLat_dTheta1DF = (newlat - lat) / (0.001 * theta1);
            double dLng_dTheta1DF = (newlng - lng) / (0.001 * theta1);
            theta1 /= 1.001;
        }
        {
            theta2 *= 1.001;
            double newlat = asin(theta1 * (sin(northLat) - sin(southLat)) + sin(southLat)) / CC;
            double newlng = westLng + deltaLng * theta2;
            double dLat_dTheta2DF = (newlat - lat) / (0.001 * theta2);
            double dLng_dTheta2DF = (newlng - lng) / (0.001 * theta2);
            theta2 /= 1.001;
        }*/
    }

    return 1;
}


QStringList RRectangularAreaSourceModel::getActionList()
{
    QStringList actionList;
    actionList << "&Display on Map" << "&Remove from Map";
    return actionList;
}


void RRectangularAreaSourceModel::displayonMap()
{
    /*
    if (!thePoint1Location || !thePoint2Location) {
        qCritical() << "Warning: Please enter the Location objects in the model" << objectName() << ".";
        return;
    }
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    RMapWidget *theMapWidget = theMainWindow->findChild<RMapWidget *>();
    theMainWindow->setActiveSubWindow(theMapWidget);
    RLocation *thePoint1LocationObject = parameterToLocation(thePoint1Location);
    RLocation *thePoint3LocationObject = parameterToLocation(thePoint2Location);
    if (!theMapWidget) {
        thePoint1LocationObject->openMap();
        return;
    }

    QList<RLocation *> LocationList;

    LocationList << thePoint1LocationObject;

    RLocation thePoint2LocationObject(0, "Point2");
    thePoint2LocationObject.setLatitude(thePoint1LocationObject->getLatitude());
    thePoint2LocationObject.setLongitude(thePoint3LocationObject->getLongitude());
    LocationList << &thePoint2LocationObject;

    LocationList << parameterToLocation(thePoint3LocationObject);

    RLocation thePoint4LocationObject(0, "Point4");
    thePoint4LocationObject.setLatitude(thePoint3LocationObject->getLatitude());
    thePoint4LocationObject.setLongitude(thePoint1LocationObject->getLongitude());
    LocationList << &thePoint4LocationObject;

    theMapWidget->drawPolygon(this, LocationList, Qt::red);
    */
}


void RRectangularAreaSourceModel::removefromMap()
{
    /*
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    RMapWidget *theMapWidget = theMainWindow->findChild<RMapWidget *>();
    if (theMapWidget) {
        theMainWindow->setActiveSubWindow(theMapWidget);
        theMapWidget->removePolygon(this);
    }
    */
}
