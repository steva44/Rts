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

#include "RLocation.h"
#include "RMainWindow.h"

#include <QMessageBox>
#include <QNetworkReply>
#include <QEventLoop>

RLocation::RLocation(QObject *parent, QString name)
    : RParameter(parent, name)
{
    theShearWaveVelocityParameter = nullptr;

    theLatitude = 0.0;
    theLongitude = 0.0;
    theAltitude = 0.0;
    theShearWaveVelocityDouble = 500.0; // Set default value

    if (parent) {
        theMainWindow = qobject_cast<RMainWindow *>(parent->parent());
    }
}


RLocation::~RLocation()
{

}


double RLocation::getLatitude() const
{
    return theLatitude;
}


void RLocation::setLatitude(double value)
{
    theLatitude = value;
}


double RLocation::getLongitude() const
{
    return theLongitude;
}


void RLocation::setLongitude(double value)
{
    theLongitude = value;
}


double RLocation::getAltitude() const
{
    return theAltitude;
}


void RLocation::setAltitude(double value)
{
    theAltitude = value;
}


QString RLocation::getAddress() const
{
    return theAddress;
}


void RLocation::setAddress(QString value)
{
    theAddress = value;
}


QString RLocation::getShearWaveVelocity()
{
    return getStringParameter(&theShearWaveVelocityDouble, theShearWaveVelocityParameter);
}


void RLocation::setShearWaveVelocity(QString value)
{
    setStringParameter(value, &theShearWaveVelocityDouble, &theShearWaveVelocityParameter);
    setCurrentValue(getShearWaveVelocityValue());
}


double RLocation::getShearWaveVelocityValue()
{
    return getDoubleParameter(&theShearWaveVelocityDouble, theShearWaveVelocityParameter);
}


double RLocation::computeSurfaceDistance(const RLocation *destination)
{
    double lat1 = degreeToRadian(theLatitude);
    double lng1 = degreeToRadian(theLongitude);

    double lat2 = degreeToRadian(destination->getLatitude());
    double lng2 = degreeToRadian(destination->getLongitude());

    double distance = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lng2 - lng1)) * EarthRadius;

    return distance;
}


void RLocation::computeSurfaceDistanceGradient(const RLocation *destination, double *dDistance_dLat, double *dDistance_dLng)
{
    double lat1 = degreeToRadian(theLatitude);
    double lng1 = degreeToRadian(theLongitude);

    double lat2 = degreeToRadian(destination->getLatitude());
    double lng2 = degreeToRadian(destination->getLongitude());

    double AA = sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lng1 - lng2);

    double dDistance_dLat1 = -EarthRadius * (cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lng1 - lng2)) / sqrt(1 - AA * AA);
    double dDistance_dLng1 = EarthRadius * (cos(lat1) * cos(lat2) * sin(lng1 - lng2)) / sqrt(1 - AA * AA);

    *dDistance_dLat = degreeToRadian(dDistance_dLat1);
    *dDistance_dLng = degreeToRadian(dDistance_dLng1);




    /*// Finite difference to check
    double distance = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lng1 - lng2)) * EarthRadius;
    double d = 0.001;
    double FDMdLat = degreeToRadian((acos(sin(lat1+d) * sin(lat2) + cos(lat1+d) * cos(lat2) * cos(lng1 - lng2)) * EarthRadius - distance) / d);
    double FDMdLng = degreeToRadian((acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lng1+d - lng2)) * EarthRadius - distance) / d);
    double DDMdLat = *dDistance_dLat;
    double DDMdLng = *dDistance_dLng;*/
}


double RLocation::computeBearing(const RLocation *destination)
{
    double lat1 = degreeToRadian(theLatitude);
    double lng1 = degreeToRadian(theLongitude);

    double lat2 = degreeToRadian(destination->getLatitude());
    double lng2 = degreeToRadian(destination->getLongitude());

    double bearing = atan2(sin(lng2 - lng1) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lng2 - lng1));
    //bearing += 3.14159265359;

    if (bearing < 0) {
        bearing += 2 * 3.14159265359;
    }

    return bearing;
}


void RLocation::computeDestination(RLocation *destination, double distance, double bearing)
{
    double lat1 = degreeToRadian(theLatitude);
    double lng1 = degreeToRadian(theLongitude);

    double d_R = distance/EarthRadius;

    double lat2 = asin(sin(lat1) * cos(d_R) + cos(lat1) * sin(d_R) * cos(bearing));
    double lng2 = lng1 + atan2(sin(bearing) * sin(d_R) * cos(lat1), cos(d_R) - sin(lat1) * sin(lat2));

    destination->setLatitude(radianToDegree(lat2));
    destination->setLongitude(radianToDegree(lng2));
}


void RLocation::computeDestinationGradient(double distance, double bearing, double *dLat_dDistance, double *dLat_dBearing, double *dLng_dDistance, double *dLng_dBearing)
{
    double lat1 = degreeToRadian(theLatitude);
    double lng1 = degreeToRadian(theLongitude);

    double d_R = distance/EarthRadius;

    double AA = sin(lat1) * cos(d_R) + cos(lat1) * sin(d_R) * cos(bearing);
    double dLat2_dDistance = (-sin(lat1) * sin(d_R) + cos(lat1) * cos(d_R) * cos(bearing)) / EarthRadius / sqrt(1 - AA * AA);
    double dLat2_dBearing = -cos(lat1) * sin(d_R) * sin(bearing) / sqrt(1 - AA * AA);

    double BB = sin(bearing) * sin(d_R) * cos(lat1);
    double CC = cos(d_R) - sin(lat1) * AA;
    double dLng2_dDistance = (sin(bearing) * cos(d_R) * cos(lat1) * CC - (-sin(d_R) + sin(d_R) * sin(lat1) * sin(lat1) - cos(lat1) * sin(lat1) * cos(d_R) * cos(bearing)) * BB) / EarthRadius / (BB * BB + CC * CC);
    double dLng2_dBearing = (cos(bearing) * sin(d_R) * cos(lat1) * CC - cos(lat1) * sin(lat1) * sin(d_R) * sin(bearing) * BB) / (BB * BB + CC * CC);

    *dLat_dDistance = radianToDegree(dLat2_dDistance);
    *dLat_dBearing = radianToDegree(dLat2_dBearing);
    *dLng_dDistance = radianToDegree(dLng2_dDistance);
    *dLng_dBearing = radianToDegree(dLng2_dBearing);
}


void RLocation::computeIntermediatePoint(RLocation *endPoint, RLocation *intermediatePoint, double distanceRatioFromStartPoint)
{
    double lat1 = degreeToRadian(theLatitude);
    double lng1 = degreeToRadian(theLongitude);

    double lat2 = degreeToRadian(endPoint->getLatitude());
    double lng2 = degreeToRadian(endPoint->getLongitude());

    double d = computeSurfaceDistance(endPoint) / EarthRadius;
    double f = distanceRatioFromStartPoint;
    double AA = sin((1 - f) * d) / sin(d);
    double BB = sin(f * d) / sin(d);
    double xx = AA * cos(lat1) * cos(lng1)+ BB * cos(lat2) * cos(lng2);
    double yy = AA * cos(lat1) * sin(lng1)+ BB * cos(lat2) * sin(lng2);
    double zz = AA * sin(lat1) + BB * sin(lat2);
    double lat = atan2(zz, sqrt((xx)*(xx) + (yy)*(yy)));
    double lng = atan2(yy, xx);

    intermediatePoint->setLatitude(radianToDegree(lat));
    intermediatePoint->setLongitude(radianToDegree(lng));
}


void RLocation::computeIntermediatePointGradient(RLocation *endPoint, double distanceRatioFromStartPoint ,double *dLat_dDistanceRatio, double *dLng_dDistanceRatio)
{
    double lat1 = degreeToRadian(theLatitude);
    double lng1 = degreeToRadian(theLongitude);

    double lat2 = degreeToRadian(endPoint->getLatitude());
    double lng2 = degreeToRadian(endPoint->getLongitude());

    double d = computeSurfaceDistance(endPoint) / EarthRadius;
    double f = distanceRatioFromStartPoint;
    double AA = sin((1 - f) * d) / sin(d);
    double BB = sin(f * d) / sin(d);
    double xx = AA * cos(lat1) * cos(lng1)+ BB * cos(lat2) * cos(lng2);
    double yy = AA * cos(lat1) * sin(lng1)+ BB * cos(lat2) * sin(lng2);
    double zz = AA * sin(lat1) + BB * sin(lat2);

    double dAA_df = -d * cos((1 - f) * d) / sin(d);
    double dBB_df = d * cos(f * d) / sin(d);
    double dxx_df = cos(lat1) * cos(lng1) * dAA_df + cos(lat2) * cos(lng2) * dBB_df;
    double dyy_df = cos(lat1) * sin(lng1) * dAA_df + cos(lat2) * sin(lng2) * dBB_df;
    double dzz_df = sin(lat1) * dAA_df + sin(lat2) * dBB_df;
    double cc = 1.0 / sqrt((xx)*(xx) + (yy)*(yy));
    double dLat_df = (-zz * xx * (cc)*(cc)*(cc)) / (1 + (zz * cc)*(zz * cc)) * dxx_df + (-zz * yy * (cc)*(cc)*(cc)) / (1 + (zz * cc)*(zz * cc)) * dyy_df + cc / (1 + (zz * cc)*(zz * cc)) * dzz_df;
    double dLng_df = (1.0 / xx / (1 + (yy / xx)*(yy / xx))) * dyy_df + (-yy / (xx * xx + yy * yy)) * dxx_df;

    *dLat_dDistanceRatio = radianToDegree(dLat_df);
    *dLng_dDistanceRatio = radianToDegree(dLng_df);


    /*// Finite difference to check
    double lat = radianToDegree(atan2(zz, sqrt(pow(xx, 2.0) + pow(yy, 2.0))));
    double lng = radianToDegree(atan2(yy, xx));
    double df = 0.01;
    f += df;
    AA = sin((1 - f) * d) / sin(d);
    BB = sin(f * d) / sin(d);
    xx = AA * cos(lat1) * cos(lng1)+ BB * cos(lat2) * cos(lng2);
    yy = AA * cos(lat1) * sin(lng1)+ BB * cos(lat2) * sin(lng2);
    zz = AA * sin(lat1) + BB * sin(lat2);
    double newLat = radianToDegree(atan2(zz, sqrt(pow(xx, 2.0) + pow(yy, 2.0))));
    double newLng = radianToDegree(atan2(yy, xx));
    double FDMdLat = (newLat - lat) / df;
    double FDMdLng = (newLng - lng) / df;

    double DDMdLat = *dLat_dDistanceRatio;
    double DDMdLng = *dLng_dDistanceRatio;*/
}


int RLocation::computeIntersectionWithLatitude(double latitude, RLocation *endPoint, double *intersectionLongitude)
{
    /* There will be one or two crossings.

    In Cartesian space we have three equations and three unknowns.
    x^2 + y^2 + z^2 = r^2 : sphere
    ax + by + cz = 0      : plane of the great circle
    z = z[0]              : plane of latitude

    We can simplify by substituting some constant terms.
    Let s = r^2 - z^2    so x^2 + y^2 = s
    and let p = -cz      so ax + by = p

    According to Mathematica

    x = (p - ((b^2 * p)/(a^2 + b^2)) (-+) (ab * sqrt(-p^2 +sa^2 +sb^2)/(a^2+b^2)))/a
    and
    y = (bp (+-) a * sqrt(-p^2 +sa^2 +sb^2)) / (a^2 + b^2)  */

    // The given latitude
    double lat = degreeToRadian(latitude);

    // Coordinates of the points on the great circle
    double lat1 = degreeToRadian(theLatitude);
    double lng1 = degreeToRadian(theLongitude);
    double lat2 = degreeToRadian(endPoint->getLatitude());
    double lng2 = degreeToRadian(endPoint->getLongitude());

    if (!(lat >= qMin(lat1, lat2) && (lat <= qMax(lat1, lat2)))) {
        qCritical() << "There is no intersection.";
        return -1;
    }

    // Special cases
    if (lat2 == lat1) {
        qCritical() << "The locations have the exact same latitude.";
        return -1;
    }
    if (lng1 == lng2) {
        *intersectionLongitude = theLongitude;
        return 0;
    }
    if (lat == lat1) {
        *intersectionLongitude = theLongitude;
        return 0;
    }
    if (lat == lat2) {
        *intersectionLongitude = endPoint->getLongitude();
        return 0;
    }

    // Conversion to Cartesian coordinates
    double xx1 = EarthRadius * cos(lng1) * cos(lat1);
    double yy1 = EarthRadius * sin(lng1) * cos(lat1);
    double zz1 = EarthRadius * sin(lat1);
    double xx2 = EarthRadius * cos(lng2) * cos(lat2);
    double yy2 = EarthRadius * sin(lng2) * cos(lat2);
    double zz2 = EarthRadius * sin(lat2);

    // The equation for the plane of the great circle: ax + by + cz = 0
    double aa = yy1 * zz2 - yy2 * zz1;
    double bb = xx2 * zz1 - xx1 * zz2;
    double cc = xx1 * yy2 - xx2 * yy1;

    // Some constants
    double latZ = EarthRadius * sin(lat);
    double planarConst = -cc * latZ;
    double sphereConst = EarthRadius * EarthRadius - latZ * latZ;
    double scale = aa * aa + bb * bb;
    double rad = sphereConst * aa * aa + sphereConst * bb * bb - planarConst * planarConst;

    /* If this is negative taking the square root would require imaginary numbers.
    It probably means the two planes intersect outside the sphere.
    That could happen if the latitude was above or below the inflection point. */

    if (rad < 0) {
        qCritical() << "There is no intersection.";
        return -1;
    }
    rad = sqrt(rad);

    double firstX = bb * bb * planarConst / scale;
    double secondX = aa * bb * rad / scale;

    // Coordinates of the intersection
    double xxi = (planarConst - firstX + secondX) / aa; //(planarConst - firstX - secondX) / aa
    double yyi = (bb * planarConst - aa * rad) / scale; //(bb * planarConst + aa * rad) / scale;
    double zzi = latZ;

    // Converting the intersection to lat and lng
    double lati = asin(zzi / EarthRadius);
    double lngi = atan2(yyi, xxi);


    if (!(lngi >= qMin(lng1, lng2) && (lngi <= qMax(lng1, lng2)))) {
        // Try the other answer of the quadratic equation
        xxi = (planarConst - firstX - secondX) / aa;
        yyi = (bb * planarConst + aa * rad) / scale;
        lati = asin(zzi / EarthRadius);
        lngi = atan2(yyi, xxi);

        if (!(lngi >= qMin(lng1, lng2) && (lngi <= qMax(lng1, lng2)))) {
            qCritical() << "There is no intersection.";
            return -1;
        }
    }

    *intersectionLongitude = radianToDegree(lngi);

    return 0;
}


double RLocation::degreeToRadian(double degree)
{
    return degree / 180 * 3.14159265359;
}


double RLocation::radianToDegree(double radian)
{
    return radian / 3.14159265359 * 180;
}


void RLocation::openMap()
{

}


QStringList RLocation::getActionList()
{
    QStringList actionList;
    actionList << "&Open Map" << "&Display on Map" << "Display &All Locations" << "&Get Coordinates from Map" << "&Remove from Map" << "&Help";
    return actionList;
}


int RLocation::displayonMap()
{

    return 0;
}


int RLocation::displayonMap(QString colour)
{

    return 0;
}


void RLocation::redisplayonMap()
{

}


void RLocation::removefromMap()
{

}


void RLocation::getCoordinatesfromMap()
{

}


void RLocation::assignCoordinatesFromMap(double latitude, double longitude)
{

}


void RLocation::getCoordinatesfromAddress()
{

}


void RLocation::assignCoordinatesFromAddress(QNetworkReply *reply)
{

}
