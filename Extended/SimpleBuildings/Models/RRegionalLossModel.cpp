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

#include "RRegionalLossModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RLocation.h"
#include "RMainWindow.h"
#include "RProbabilityDistributions.h"


const double sphereRadius = 4.0 * 3.14159265359 * (EarthRadius * 1000.0)*(EarthRadius * 1000.0);

RRegionalLossModel::RRegionalLossModel(QObject *parent, QString name)
	: RModel(parent, name)
{
    theCentroidLocation = 0;
	theSa1 = 0;
	theSa2 = 0;
	theTime = 0;
	theAnnualInterestRate = 0;
	theColor = Qt::blue;
	theTotalArea = 0.0;
    theProbabilityDistributions = 0;

	
	theDomain->createObject(objectName() + "Response", "RGenericResponse");
	theLoss = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
	theLoss->setModel(this);
}

RRegionalLossModel::~RRegionalLossModel()
{

}



QObject * RRegionalLossModel::getProbabilityDistributions() const
{
    return theProbabilityDistributions;
}

void RRegionalLossModel::setProbabilityDistributions(QObject *value)
{
    theProbabilityDistributions = qobject_cast<RProbabilityDistributions *>(value);
}



QString RRegionalLossModel::getBoundaryLocationList()
{	
    return parameterListToString(theBoundaryLocationList);
}	
	
void RRegionalLossModel::setBoundaryLocationList(QString value)
{	
    theBoundaryLocationList = stringToParameterList(value);
	
    if (theBoundaryLocationList.count() < 3) {
		theTotalArea = 0.0;
        qCritical() << "Warning: The number of boundary Locations in the model" << objectName() << "is less than 3.";
	}
	
    // Checking if all the passed parameters are either RLocation or RLocationResponse
    parameterListToLocationList(theBoundaryLocationList, true);
}	

QObject * RRegionalLossModel::getCentroidLocation() const
{	
    return theCentroidLocation;
}	
	
void RRegionalLossModel::setCentroidLocation(QObject *value)
{	
    theCentroidLocation = qobject_cast<RParameter *>(value);

    // Checking the parameter for being a Location or Location response object
    parameterToLocation(theCentroidLocation);
}	

QObject * RRegionalLossModel::getSa1() const	
{	
	return theSa1;
}	
	
void RRegionalLossModel::setSa1(QObject *value)	
{	
	theSa1 = qobject_cast<RParameter *>(value);
}	
	
QObject * RRegionalLossModel::getSa2() const	
{	
	return theSa2;
}	
	
void RRegionalLossModel::setSa2(QObject *value)	
{	
	theSa2 = qobject_cast<RParameter *>(value);
}	

QString RRegionalLossModel::getUnitAreaCostList()	
{	
	return parameterListToString(theUnitAreaCostList);
}	
	
void RRegionalLossModel::setUnitAreaCostList(QString value)	
{	
	theUnitAreaCostList = stringToParameterList(value);
}	
	
QString RRegionalLossModel::getAreaRatioList()	
{	
	return parameterListToString(theAreaRatioList);
}	
	
void RRegionalLossModel::setAreaRatioList(QString value)	
{	
	theAreaRatioList = stringToParameterList(value);
}	

QObject * RRegionalLossModel::getTime() const	
{	
	return theTime;
}	
	
void RRegionalLossModel::setTime(QObject *value)	
{	
	theTime = qobject_cast<RParameter *>(value);
}	

QObject * RRegionalLossModel::getAnnualInterestRate() const	
{	
	return theAnnualInterestRate;
}	
	
void RRegionalLossModel::setAnnualInterestRate(QObject *value)	
{	
	theAnnualInterestRate = qobject_cast<RParameter *>(value);
}	
	
QString RRegionalLossModel::getTheta1List()	
{	
	return parameterListToString(theTheta1List);
}	
	
void RRegionalLossModel::setTheta1List(QString value)	
{	
	theTheta1List = stringToParameterList(value);
}	
	
QString RRegionalLossModel::getTheta2List()	
{	
	return parameterListToString(theTheta2List);
}	
	
void RRegionalLossModel::setTheta2List(QString value)	
{	
	theTheta2List = stringToParameterList(value);
}	
	
QString RRegionalLossModel::getTheta3List()	
{	
	return parameterListToString(theTheta3List);
}	
	
void RRegionalLossModel::setTheta3List(QString value)	
{	
	theTheta3List = stringToParameterList(value);
}	
	
QString RRegionalLossModel::getTheta4List()	
{	
	return parameterListToString(theTheta4List);
}	
	
void RRegionalLossModel::setTheta4List(QString value)	
{	
	theTheta4List = stringToParameterList(value);
}	
		
QString RRegionalLossModel::getTheta5List()	
{	
	return parameterListToString(theTheta5List);
}	
	
void RRegionalLossModel::setTheta5List(QString value)	
{	
	theTheta5List = stringToParameterList(value);
}	
	
QString RRegionalLossModel::getEpsilonList()	
{	
	return parameterListToString(theEpsilonList);
}	
	
void RRegionalLossModel::setEpsilonList(QString value)	
{	
	theEpsilonList = stringToParameterList(value);
}	

QColor RRegionalLossModel::getColor() const	
{	
	return theColor;
}	
	
void RRegionalLossModel::setColor(QColor value)	
{	
	theColor = value;
}	

double RRegionalLossModel::getTotalArea()	
{	
    if (theBoundaryLocationList.count() < 3) {
		theTotalArea = 0.0;
	} else {
		calculateArea();
		calculateCentroid();
	}
	return theTotalArea;
}	

int RRegionalLossModel::evaluateModel(RGradientType theGradientType)
{	
	if (theTheta1List.count() != theTheta2List.count() || theTheta2List.count() != theTheta3List.count() || theTheta3List.count() != theTheta4List.count() || theTheta4List.count() != theTheta5List.count() || theTheta5List.count() != theEpsilonList.count() || theEpsilonList.count() != theUnitAreaCostList.count() || theUnitAreaCostList.count() != theAreaRatioList.count()) {
		qCritical() << "Error: The number of input parameters in the Theta 1 List, Theta 2 List, Theta 3 List, Theta 4 List, Theta 5 List, Epsilon List, Unit Area Cost List, and Area Ratio List in the object" << objectName() << "are not the same.";
		return -1;
	}
	if (theTheta1List.isEmpty()) {
		qCritical() << "Error: The property Theta 1 List in the object" << objectName() << "is empty.";
		return -1;
	}
	if (theTheta2List.isEmpty()) {
		qCritical() << "Error: The property Theta 2 List in the object" << objectName() << "is empty.";
		return -1;
	}
	if (theTheta3List.isEmpty()) {
		qCritical() << "Error: The property Theta 3 List in the object" << objectName() << "is empty.";
		return -1;
	}
	if (theTheta4List.isEmpty()) {
		qCritical() << "Error: The property Theta 4 List in the object" << objectName() << "is empty.";
		return -1;
	}
	if (theTheta5List.isEmpty()) {
		qCritical() << "Error: The property Theta 4 List in the object" << objectName() << "is empty.";
		return -1;
	}
	if (theEpsilonList.isEmpty()) {
		qCritical() << "Error: The property Epsilon List in the object" << objectName() << "is empty.";
		return -1;
	}
	if (theUnitAreaCostList.isEmpty()) {
		qCritical() << "Error: The property Unit Area Cost List in the object" << objectName() << "is empty.";
		return -1;
	}
	if (theUnitAreaCostList.isEmpty()) {
		qCritical() << "Error: The property Unit Area Cost List in the object" << objectName() << "is empty.";
		return -1;
	}

    if (theBoundaryLocationList.count() < 3) {
		theTotalArea = 0.0;
        qCritical() << "Warning: The number of boundary Locations in the model" << objectName() << "is less than 3.";
		return -1;
	}

	double Sa1 = theSa1->getCurrentValue();
	double Sa0_3 = theSa2->getCurrentValue();
	double currentTime = theTime->getCurrentValue();
	double interestRate = theAnnualInterestRate->getCurrentValue();
	
	double totalLoss = 0.0;
	for (int i = 0; i < theUnitAreaCostList.count(); i++) {
		double theta1 = theTheta1List[i]->getCurrentValue();
		double theta2 = theTheta2List[i]->getCurrentValue();
		double theta3 = theTheta3List[i]->getCurrentValue();
		double theta4 = theTheta4List[i]->getCurrentValue();
		double theta5 = theTheta5List[i]->getCurrentValue();
		double epsilon = theEpsilonList[i]->getCurrentValue();
		double C = theUnitAreaCostList[i]->getCurrentValue();
		double ARatio = theAreaRatioList[i]->getCurrentValue();

		
        double l = theta1 * theProbabilityDistributions->getNormalCDF(theta2 * pow(Sa0_3, theta5) + theta3 * pow(Sa1, theta5) + theta4, 1.0) * exp(epsilon);
		double loss = l * C * ARatio;
		totalLoss += loss;
	}

	double futureDiscountFactor = 1.0;
	if (currentTime > theInitialTime) {
		//futureDiscountFactor = pow(1 + interestRate, -(currentTime - theInitialTime) / 365.0);
		futureDiscountFactor = exp(-interestRate * (currentTime - theInitialTime) / 365.0);
	}

	totalLoss *= theTotalArea * futureDiscountFactor;

	theLoss->setCurrentValue(totalLoss);

    if (theGradientType == RGradientType::DDM  ) {
		// IMPLEMENT
	}

	/*int rr,gg,bb,aa;
	theColor.getRgb(&rr,&gg,&bb,&aa);
	qDebug() << objectName() << totalLoss << theTotalArea << rr << gg << bb << theColor.name();*/

	return 0;
}

void RRegionalLossModel::reset()
{
	calculateArea();
	calculateCentroid();
	theInitialTime = theTime->getCurrentValue();
}

void RRegionalLossModel::calculateArea()
{
    QList<RLocation *> LocationList = parameterListToLocationList(theBoundaryLocationList);

	double angleSum = 0.0;
    int n = LocationList.count();

	for (int i = 0; i < n; i++) {
		// j is the next point
		int j = i + 1;
		if (j == n) {
			j = 0;
		}
		
		// k is the previous point
		int k = i - 1;
		if (k == -1) {
			k = n - 1;
		}

        double bearing1 = LocationList[i]->computeBearing(LocationList[j]);
        double bearing2 = LocationList[i]->computeBearing(LocationList[k]);
		double angle = bearing2 - bearing1;
		if (angle < 0) {
            angle += 2 * 3.14159265359;
		}
		angleSum += angle;
	}

    theTotalArea = (angleSum - (n - 2) * 3.14159265359) * (EarthRadius * 1000.0)*(EarthRadius * 1000.0);

	if (theTotalArea > sphereRadius / 2.0) {
		theTotalArea = sphereRadius - theTotalArea;
	}

	//// Calculate the area using Google Maps for comparison (the map should be open)
	//RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
	//RMapWidget *theMapWidget = theMainWindow->findChild<RMapWidget *>();
	//double aa;
	//if (theMapWidget) {
	//	theTotalArea = theMapWidget->getArea(0);
	//}
}

void RRegionalLossModel::calculateCentroid()
{
	// ATTENTION: THESE CALCULATIONS ARE FOR A PLANAR POLYGON, ALSO ITY DOESN'T WORK FOR A POLYGON CROSSING THE DATELINE

    RLocation *Location = parameterToLocation(theCentroidLocation);
    if (!Location) {
		return;
	}

    QList<RLocation *> LocationList = parameterListToLocationList(theBoundaryLocationList);
    if (LocationList.count() < 3) {
		return;
	}

    LocationList << LocationList[0];
	double A = 0.0;
	double Cx = 0.0;
	double Cy = 0.0;

    for (int i = 0; i < LocationList.count() - 1; i++) {
        double xi = LocationList[i]->getLongitude();
        double xi_1 = LocationList[i+1]->getLongitude();
        double yi = LocationList[i]->getLatitude();
        double yi_1 = LocationList[i+1]->getLatitude();

		Cx += (xi + xi_1) * (xi * yi_1 - xi_1 * yi);
		Cy += (yi + yi_1) * (xi * yi_1 - xi_1 * yi);

		A += xi * yi_1 - xi_1 * yi;
	}

	A *= 0.5;
	Cx *= 1.0 / 6.0 / A;
	Cy *= 1.0 / 6.0 / A;

    Location->setLatitude(Cy);
    Location->setLongitude(Cx);

	//A = qAbs(A);
}

bool RRegionalLossModel::hasInside(RLocation *passedLocation)
{
    QList<RLocation *> LocationList = parameterListToLocationList(theBoundaryLocationList);

	// This code adopted from the following URL: http://msdn.microsoft.com/en-us/library/cc451895.aspx
	int i;
    int j = LocationList.count() - 1;
	bool isInside = false;
    double lat = passedLocation->getLatitude();
    double lng = passedLocation->getLongitude();
    for (i = 0; i < LocationList.count(); i++) {
        if (LocationList[i]->getLongitude() < lng && LocationList[j]->getLongitude() >= lng ||  LocationList[j]->getLongitude() < lng && LocationList[i]->getLongitude() >= lng) {
            if (LocationList[i]->getLatitude() + (lng - LocationList[i]->getLongitude()) / (LocationList[j]->getLongitude() - LocationList[i]->getLongitude()) * (LocationList[j]->getLatitude() - LocationList[i]->getLatitude()) < lat) {
				isInside = !isInside; 
			}
		}
		j = i; 
	}
	return isInside; 
}

QStringList RRegionalLossModel::getActionList()
{    
    QStringList actionList;

    actionList << "&Run" << "Separator" << "&Help" << "Separator" << "&Display on Map" << "Remove from Map" << "&Display &Locations Inside";

    return actionList;
}

void RRegionalLossModel::displayonMap()
{
    /*
    QList<RLocation *> LocationList = parameterListToLocationList(theBoundaryLocationList);
    if (LocationList.count() < 3) {
        qCritical() << "Warning: Please enter at least 3 Location objects in the model" << objectName() << ".";
		return;
	}
	RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
	RMapWidget *theMapWidget = theMainWindow->findChild<RMapWidget *>();
	theMainWindow->setActiveSubWindow(theMapWidget);
	if (!theMapWidget) {
        LocationList[0]->openMap();
		return;
	}
    theMapWidget->drawPolygon(this, parameterListToLocationList(theBoundaryLocationList), theColor, 0.6);

	calculateCentroid();
	
    RLocation *Location = parameterToLocation(theCentroidLocation);
    if (Location) {
        Location->displayonMap();
	}
    */
}

void RRegionalLossModel::removefromMap()
{
    /*
	RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
	RMapWidget *theMapWidget = theMainWindow->findChild<RMapWidget *>();
	if (theMapWidget) {
		theMainWindow->setActiveSubWindow(theMapWidget);
		theMapWidget->removePolygon(this);
	
        RLocation *Location = parameterToLocation(theCentroidLocation);
        if (Location) {
            Location->removefromMap();
		}
	}
    */
}

void RRegionalLossModel::displayLocationsInside()
{
    /*
    QList<RLocation *> LocationList = theDomain->findChildren<RLocation *>();
    if (LocationList.isEmpty()) {
		return;
	}

    QList<RLocation *> boundaryList = parameterListToLocationList(theBoundaryLocationList);
	if (boundaryList.isEmpty()) {
		return;
	}
	
	for (int i = 0; i < boundaryList.count(); i++) {
        LocationList.removeOne(boundaryList[i]);
	}

    QList<RLocation *> insideList;
    for (int i = 0; i < LocationList.count(); i++) {
        if (hasInside(LocationList[i])) {
            insideList << LocationList[i];
		}
		// A line to keep the GUI responsive
		if (i % 10 == 0) {
			QCoreApplication::processEvents();
		}
	}

	if (insideList.isEmpty()) {
        qDebug() << "No Locations found inside the sector" << objectName() << ".";
	} else {
		qDebug() << "The locaions inside the sector" << objectName() << ":";
	}

	RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
	RMapWidget *theMapWidget = theMainWindow->findChild<RMapWidget *>();
	theMainWindow->setActiveSubWindow(theMapWidget);

	for (int i = 0; i < insideList.count(); i++) {
		qDebug() << qPrintable(insideList[i]->objectName());

		if (theMapWidget) {
			insideList[i]->displayonMap();
		}

		// A line to keep the GUI responsive
		if (i % 10 == 0) {
			QCoreApplication::processEvents();
		}
	}

	qDebug() << " ";
    */
}
