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

#include "RGenericAttenuationModel.h"	
#include "RParameter.h"	
#include "RResponse.h"	
#include "RLocation.h"
#include "RLocationResponse.h"
#include "RMainWindow.h"
#include "RDomain.h"

#include <QCoreApplication>
#include <math.h>

RGenericAttenuationModel::RGenericAttenuationModel(QObject *parent, QString name)	
    : RIntensityModel(parent, name)
{	
    // Initializing the pointer data members
    theTheta1 = nullptr;
    theTheta2 = nullptr;
    theTheta3 = nullptr;
    theTheta4 = nullptr;
    theEpsilon = nullptr;

    // Create the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RGenericAttenuationModel::~RGenericAttenuationModel()	
{	

}


QString RGenericAttenuationModel::getMagnitudeList()	
{	
    return parameterListToString(theMagnitudeList);
}	


void RGenericAttenuationModel::setMagnitudeList(QString value)	
{	
    theMagnitudeList = stringToParameterList(value);
}


QString RGenericAttenuationModel::getHypocentreList()
{	
    return parameterListToString(theHypocentreList);
}	


void RGenericAttenuationModel::setHypocentreList(QString value)
{	
    theHypocentreList = stringToParameterList(value);

    // Checking if all the passed parameters are either RLocation or RLocationResponse
    parameterListToLocationList(theHypocentreList);
}	


QObject * RGenericAttenuationModel::getTheta1() const	
{	
    return theTheta1;
}	


void RGenericAttenuationModel::setTheta1(QObject *value)	
{	
    theTheta1 = qobject_cast<RParameter *>(value);
}


QObject * RGenericAttenuationModel::getTheta2() const	
{	
    return theTheta2;
}	


void RGenericAttenuationModel::setTheta2(QObject *value)	
{	
    theTheta2 = qobject_cast<RParameter *>(value);
}


QObject * RGenericAttenuationModel::getTheta3() const
{
    return theTheta3;
}


void RGenericAttenuationModel::setTheta3(QObject *value)
{
    theTheta3 = qobject_cast<RParameter *>(value);
}


QObject * RGenericAttenuationModel::getTheta4() const
{
    return theTheta4;
}


void RGenericAttenuationModel::setTheta4(QObject *value)
{
    theTheta4 = qobject_cast<RParameter *>(value);
}


QObject * RGenericAttenuationModel::getEpsilon() const	
{	
    return theEpsilon;
}	


void RGenericAttenuationModel::setEpsilon(QObject *value)	
{	
    theEpsilon = qobject_cast<RParameter *>(value);
}


bool RGenericAttenuationModel::isOn()
{
    // Check the magnitude: that is the model associated with an occurrence model
    bool hereIsARealMagnitude = false;
    for (int i = 0; i < theMagnitudeList.count(); i++) {

        double M = theMagnitudeList[i]->getCurrentValue();

        if (M != 0.0) {
            hereIsARealMagnitude = true;
        }
    }
    if (hereIsARealMagnitude) {
        return true;
    }
    else {
        return false;
    }
}


int RGenericAttenuationModel::evaluateModel(RGradientType theGradientType)
{
    // Check if this hazard is "on"
    if (!isOn()) {
        theResponse->setCurrentValue(0.0);
        return 0;
    }

    // NOTE: IN THIS MODEL, h IS NOT DEPTH. IT'S A PARAMETER EQUAL TO 7.3. THIS SHOULD BE FIXED.

    // Checking if the number of magnitudes is equal to the number of hypocentre Locations
    if (theHypocentreList.count() != theMagnitudeList.count()) {
        qCritical() << "Error: The number of magnitude parameters should be equal to the number of hypocentre Locations in the model" << objectName() << ".";
        return -1;
    }

    // For convenience, look up the values of the parameters first
    double theta1 = theTheta1->getCurrentValue();
    double theta2 = theTheta2->getCurrentValue();
    double theta3 = theTheta3->getCurrentValue();
    double theta4 = theTheta4->getCurrentValue();
    double epsilon = theEpsilon->getCurrentValue();

    QVector<double> MVector;
    QList<RLocation *> hypocentreLocationObjectList;
    for (int i = 0; i < theMagnitudeList.count(); i++) {
        double M = theMagnitudeList[i]->getCurrentValue();
        MVector << M;

        // Verification that the user has given an RLocationResponse or an RLocation
        RLocation *hypocentreLocationObject = parameterToLocation(theHypocentreList[i]);
        if (!hypocentreLocationObject) {
            qCritical() << "Error: The Location response" << theHypocentreList[i]->objectName() << "does not include any Location object.";
            return -1;
        }
        hypocentreLocationObjectList << hypocentreLocationObject;
    }

    double log_10 = log(10.0);

    double totalPGA = 0;

    QMap<RParameter *, double> thePGAGradientMap;
    double total_dPGA_dTheta1 = 0;
    double total_dPGA_dTheta2 = 0;
    double total_dPGA_dTheta3 = 0;
    double total_dPGA_dTheta4 = 0;
    double total_dPGA_dEpsilon = 0;

    for (int j = 0; j < theHypocentreList.count(); j++) {
        RLocation *hypocentreLocationObject = hypocentreLocationObjectList[j];
        double M = MVector[j];

        double alt = hypocentreLocationObject->getAltitude();
        double h = qAbs(alt);

        // Calculate distance R
        double R = hypocentreLocationObject->computeSurfaceDistance(theLocation);

        // Calulate the Joyner-Boore R
        double r = sqrt(R * R + h * h);

        double logPGA = -theta1 + theta2 * M - theta3 * log10(r) + theta4 * r + epsilon;
        double PGA = pow(10.0, logPGA);

        // IMPORTANT: If M = 0, it means that this hazard is off, so the resulting PGA is Zero
        if (M == 0) {
            PGA = 0;
        }

        // Adding the cumulative PGA calculated from other hazards so far to this PGA
        totalPGA += PGA;


        if (theGradientType >= RGradientType::DDM) {
            double factor = log_10 * PGA;

            double dPGA_dTheta1 = -factor;
            double dPGA_dTheta2 = M * factor;
            double dPGA_dTheta3 = -log10(r) * factor;
            double dPGA_dTheta4 = r * factor;
            double dPGA_dEpsilon = factor;

            double dPGA_dM = theta2 * factor;
            double dPGA_dr = (-theta3 / r + theta4 * log_10) * PGA;
            double dr_dR = R / sqrt(R * R + h * h);
            double dr_dh = h / sqrt(R * R + h * h);

            double dR_dLat;
            double dR_dLng;
            hypocentreLocationObject->computeSurfaceDistanceGradient(theLocation, &dR_dLat, &dR_dLng);
            double dPGA_dLat = dPGA_dr * dr_dR * dR_dLat;
            double dPGA_dLng = dPGA_dr * dr_dR * dR_dLng;

            double dh_dAlt = alt > 0 ? 1 : (alt < 0 ? -1 : 0); // signum function
            double dPGA_dAlt = dPGA_dr * dr_dh * dh_dAlt;

            total_dPGA_dTheta1 += dPGA_dTheta1;
            total_dPGA_dTheta2 += dPGA_dTheta2;
            total_dPGA_dTheta3 += dPGA_dTheta3;
            total_dPGA_dTheta4 += dPGA_dTheta4;
            total_dPGA_dEpsilon += dPGA_dEpsilon;

            thePGAGradientMap.insertMulti(theMagnitudeList[j], dPGA_dM);
            thePGAGradientMap.insertMulti(theHypocentreList[j], dPGA_dLat);
            thePGAGradientMap.insertMulti(theHypocentreList[j], dPGA_dLng);
            thePGAGradientMap.insertMulti(theHypocentreList[j], dPGA_dAlt);


            /*// Finite difefrence check
                double d = 0.0001;
                hypocentreLocationObject->setLatitude(hypocentreLocationObject->getLatitude()+d);
                R = hypocentreLocationObject->computeSurfaceDistance(structureLocationList[j]);
                r = sqrt(R * R + h * h);
                logPGA = -theta1 + theta2 * M - theta3 * log10(r) + theta4 * r + epsilon;
                double FDM_dPGA_dLat = (pow(10.0, logPGA)-PGA)/d;

                hypocentreLocationObject->setLatitude(hypocentreLocationObject->getLatitude()-d);
                hypocentreLocationObject->setLongitude(hypocentreLocationObject->getLongitude()+d);
                R = hypocentreLocationObject->computeSurfaceDistance(structureLocationList[j]);
                r = sqrt(R * R + h * h);
                logPGA = -theta1 + theta2 * M - theta3 * log10(r) + theta4 * r + epsilon;
                double FDM_dPGA_dLng = (pow(10.0, logPGA)-PGA)/d;
                hypocentreLocationObject->setLongitude(hypocentreLocationObject->getLongitude()-d);*/
        }
    }


    if (theGradientType >= RGradientType::DDM) {
        thePGAGradientMap.insertMulti(theTheta1, total_dPGA_dTheta1);
        thePGAGradientMap.insertMulti(theTheta2, total_dPGA_dTheta2);
        thePGAGradientMap.insertMulti(theTheta3, total_dPGA_dTheta3);
        thePGAGradientMap.insertMulti(theTheta4, total_dPGA_dTheta4);
        thePGAGradientMap.insertMulti(theEpsilon, total_dPGA_dEpsilon);

        theDDMMap.insertMulti(theResponse, thePGAGradientMap);
    }

    theResponse->setCurrentValue(totalPGA);

    return 1;
}
