#ifndef RLocation_H
#define RLocation_H
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

#include "RParameter.h"

class QNetworkReply;
class RMainWindow;

const double EarthRadius = 6372.796924;

class RLocation : public RParameter
{
	Q_OBJECT	
	Q_PROPERTY(double Latitude READ getLatitude WRITE setLatitude)
	Q_PROPERTY(double Longitude READ getLongitude WRITE setLongitude)
	Q_PROPERTY(double Altitude READ getAltitude WRITE setAltitude)
	Q_PROPERTY(QString Address READ getAddress WRITE setAddress)
    Q_PROPERTY(QString ShearWaveVelocity READ getShearWaveVelocity WRITE setShearWaveVelocity)

public:
    RLocation(QObject *parent, QString name);
    virtual ~RLocation();
	
	double getLatitude() const;
	void setLatitude(double value);

    double getLongitude() const;
	void setLongitude(double value);

    double getAltitude() const;
	void setAltitude(double value);

    QString getAddress() const;
	void setAddress(QString value);

    QString getShearWaveVelocity();
    double getShearWaveVelocityValue();
    void setShearWaveVelocity(QString value);

	// Computes the surface distance to another location
    double computeSurfaceDistance(const RLocation *destination);

	// Computes the bearing (angle with north direction)
    double computeBearing(const RLocation *destination);

	// Computes the destination location given the distance and bearing
    void computeDestination(RLocation *destination, double distance, double bearing);

	// Computes the derivative of the destination latitude and longitude wrt. the bearing and distance
	void computeDestinationGradient(double distance, double bearing, double *dLat_dDistance, double *dLat_dBearing, double *dLng_dDistance, double *dLng_dBearing);

	// Computes the derivative of the distance with respect to the latitude and longitude of the destination location by DDM
    void computeSurfaceDistanceGradient(const RLocation *destination, double *dDistance_dLat, double *dDistance_dLng);

	// Computes the coordinates of the an intermediate point given a ratio of the distance
    void computeIntermediatePoint(RLocation *endPoint, RLocation *intermediatePoint, double distanceRatioFromStartPoint);

	// Computes the derivative of the latitude and longitude of the intermediate point wrt. the ratio of the distance by DDM
    void computeIntermediatePointGradient(RLocation *endPoint, double distanceRatioFromStartPoint ,double *dLat_dDistanceRatio, double *dLng_dDistanceRatio);

	// Computes the coordinates of the intersection with a given latitude
    int computeIntersectionWithLatitude(double latitude, RLocation *endPoint, double *intersectionLongitude);

	// Returns a list of "right-click" menu items
	QStringList getActionList();

public slots:

    // Opens the MapWidget object in the main window
    void openMap();

    // Displays a marker for the new realization of the location on the map while keeping the markers for previous realizations
    void redisplayonMap();

	// Removes all the markers for this location from the map
	void removefromMap();

	// Displays a marker for the current realization of the location on the map while removing all previous markers
    int displayonMap();
    int displayonMap(QString colour);

	// Starts the operation of getting the latitude and longitude from the user's click on the map
	void getCoordinatesfromMap();

	// Assigns the latitude and longitude once the user is done clicking on the map
	void assignCoordinatesFromMap(double latitude, double longitude);

	// Starts the operation of getting the latitude and longitude by sending the address to Google
	void getCoordinatesfromAddress();

	// Assigns the latitude and longitude once Google sends back the latitude and longitude for the given address
	void assignCoordinatesFromAddress(QNetworkReply *reply);

private:

    double degreeToRadian(double degree);
	double radianToDegree(double radian);

	double theLatitude;
	double theLongitude;
	double theAltitude;
	QString theAddress;
    QPointer<RParameter> theShearWaveVelocityParameter;
    double theShearWaveVelocityDouble;

	RMainWindow *theMainWindow;	
};

#endif
