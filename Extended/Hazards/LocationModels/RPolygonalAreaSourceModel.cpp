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

#include "RPolygonalAreaSourceModel.h"	
#include "RParameter.h"	
#include "RLocationResponse.h"
#include "RDomain.h"
#include "RContinuousRandomVariable.h"
#include "RLocation.h"
#include "RMainWindow.h"

#include <math.h>

RPolygonalAreaSourceModel::RPolygonalAreaSourceModel(QObject *parent, QString name)	
    : RModel(parent, name)
{	
    theTheta1 = nullptr;
    theTheta2 = nullptr;
    theDepth = nullptr;

    theLatitudeRandomVariable = nullptr;

    // Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RLocationResponse");
    theHypocentreLocation = qobject_cast<RLocationResponse *>(theDomain->getLastAddedObject());
    theHypocentreLocation->setModel(this);

    // Set the two uniformly distributed random variables (theta1, theta2)
    theDomain->createObject(objectName() + "_theta1", "RContinuousRandomVariable");
    theTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta1->setDistributionType(QString("Uniform (a, b)"));
    theTheta1->setParameter1("0.0");
    theTheta1->setParameter2("1.0");
    theAutoParametersList << theTheta1;

    theDomain->createObject(objectName() + "_theta2", "RContinuousRandomVariable");
    theTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta2->setDistributionType(QString("Uniform (a, b)"));
    theTheta2->setParameter1("0.0");
    theTheta2->setParameter2("1.0");
    theAutoParametersList << theTheta2;
}	


RPolygonalAreaSourceModel::~RPolygonalAreaSourceModel()	
{	
    if (theLatitudeRandomVariable) {
        delete theLatitudeRandomVariable;
    }
    theLatitudeRandomVariable = 0;
}	


QString RPolygonalAreaSourceModel::getBoundaryLocationList()
{
    return parameterListToString(theBoundaryLocationList);
}


void RPolygonalAreaSourceModel::setBoundaryLocationList(QString value)
{
    theBoundaryLocationList = stringToParameterList(value);

    // Checking if all the passed parameters are either RLocation or RLocationResponse
    parameterListToLocationList(theBoundaryLocationList, true);

    if (theLatitudeRandomVariable) {
        delete theLatitudeRandomVariable;
    }
    theLatitudeRandomVariable = 0;
}


QObject * RPolygonalAreaSourceModel::getDepth() const
{
    return theDepth;
}


void RPolygonalAreaSourceModel::setDepth(QObject *value)	
{	
    theDepth = qobject_cast<RParameter *>(value);
}


QList<RParameter *> RPolygonalAreaSourceModel::getPhysicalParameterList()
{
    QList<RParameter *> paramList;
    paramList << theTheta1 << theTheta2 << theDepth;
    return paramList;
}


void RPolygonalAreaSourceModel::reset()
{
    if (theLatitudeRandomVariable) {
        delete theLatitudeRandomVariable;
    }
    theLatitudeRandomVariable = 0;
}


int RPolygonalAreaSourceModel::evaluateModel(RGradientType theGradientType)	
{	

    QList<RLocation *> theLocationList = parameterListToLocationList(theBoundaryLocationList);

    // Getting the pointer to the RLocation object from the RLocationResponse object (theHypocentreLocation)
    RLocation *theHypocentreLocationObject = parameterToLocation(theHypocentreLocation);
    if (!theHypocentreLocationObject) {
        qCritical() << "Error: The Location response" << theHypocentreLocation->objectName() << "does not include any Location object.";
        return -1;
    }


    if (!theLatitudeRandomVariable) {
        QMap<double, double> latLngMap;
        for (int i = 0; i < theLocationList.count(); i++) {
            latLngMap.insertMulti(theLocationList[i]->getLatitude(), theLocationList[i]->getLongitude());
        }

        QVector<double> latVector = latLngMap.uniqueKeys().toVector();
        QVector<double> PDFVector;
        for (int i = 0; i < latVector.count(); i++) {
            double lat = latVector[i];

            QVector<double> lngVector;

            for (int j = 0; j < theLocationList.count(); j++) {
                // k is the next point
                int k = j + 1;
                if (k == theLocationList.count()) {
                    k = 0;
                }
                if ((theLocationList[j]->getLatitude() - lat) * (theLocationList[k]->getLatitude() - lat) < 0) {
                    // The line between j and k intersects with this latitude
                    double intersectionLng;
                    /*// TEMP....................................................................................................................................................................
                    double lng1 = theLocationList[j]->getLongitude();
                    double lng2 = theLocationList[k]->getLongitude();
                    double lat1 = theLocationList[j]->getLatitude();
                    double lat2 = theLocationList[k]->getLatitude();
                    intersectionLng = lng1 + (lng2 - lng1)/(lat2 - lat1) * (lat - lat1);*/



                    int result = theLocationList[j]->computeIntersectionWithLatitude(lat, theLocationList[k], &intersectionLng);
                    if (result < 0) {
                        qCritical() << "Error 2: Cannot compute the intersection in model" << objectName() << " Please check your coordinates...";
                    }
                    lngVector << intersectionLng;

                    // OR CALCULATE THE INTERSECTION USING A LINEAR RELATIONSHIP BETWEEN LAT AND LNG. THIS IS HOW WE HAVE EVALAUTE THE PDF OF LATITUDE AFTER ALL.

                } else if (theLocationList[j]->getLatitude() == lat) {
                    // The point j iteself is on the latitude

                    // l is the previous point
                    int l = j - 1;
                    if (l == -1) {
                        l = theLocationList.count() - 1;
                    }

                    // Add the point j's longitude to the lngVector only if the the lines that go out of point j are not at the same side of the latitude
                    if ((theLocationList[l]->getLatitude() - lat) * (theLocationList[k]->getLatitude() - lat) <= 0) {
                        lngVector << theLocationList[j]->getLongitude();
                    }
                }
            }

            // Terje
            if (remainder(lngVector.size(), 2.0) != 0.0) {
                qCritical() << "Something is wrong in" << this->objectName() << " Please check your coordinates...";
                return 0;
            }



            qSort(lngVector.begin(), lngVector.end());

            double length = 0;
            for (int j = 0; j < lngVector.count(); j += 2) {
                length += qAbs(lngVector[j+1] - lngVector[j]);
            }
            length *= cos(lat / 180 * 3.14159265359);

            PDFVector << length;
        }

        theLatitudeRandomVariable = new RContinuousRandomVariable(0, objectName() + "LatRV");
        theLatitudeRandomVariable->setDistributionType("User Defined");
        theLatitudeRandomVariable->setUserDefinedPDF(latVector, PDFVector);
    }



    double theta1 = theTheta1->getCurrentValue();
    double theta2 = theTheta2->getCurrentValue();

    double latitude = theLatitudeRandomVariable->getInverseCDFValue(theta1);

    bool isDifferentiable = true;

    QVector<double> lngVector;
    QVector<double> dLngVector;
    double topOrBottomMostLongitude;
    for (int i = 0; i < theLocationList.count(); i++) {
        int j = i + 1;
        if (j == theLocationList.count()) {
            j = 0;
        }
        if ((theLocationList[i]->getLatitude() - latitude) * (theLocationList[j]->getLatitude() - latitude) < 0) {
            double intersectionLng;
            int result = theLocationList[i]->computeIntersectionWithLatitude(latitude, theLocationList[j], &intersectionLng);
            if (result < 0) {
                qCritical() << "Error 1: Cannot compute the intersection in model" << objectName() << " Please check your coordinates...";
            }
            lngVector << intersectionLng;

            if (theGradientType >= RGradientType::DDM) {
                double lngForDiff;
                double perturb = 0.0001;
                int result = theLocationList[i]->computeIntersectionWithLatitude(latitude * (1 + perturb), theLocationList[j], &lngForDiff);
                if (result < 0) {
                    isDifferentiable = false;
                }
                double dIntersectionLongitude_dLatitude = (lngForDiff - intersectionLng) / (perturb * latitude);
                dLngVector << dIntersectionLongitude_dLatitude;
            }
        } else if (theLocationList[i]->getLatitude() == latitude) {
            // The point i iteself is on the latitude

            // k is the previous point
            int k = i - 1;
            if (k == -1) {
                k = theLocationList.count() - 1;
            }

            // Add the point i's longitude to the lngVector only if the the lines that go out of point j are not at the same side of the latitude
            if ((theLocationList[k]->getLatitude() - latitude) * (theLocationList[j]->getLatitude() - latitude) <= 0) {
                lngVector << theLocationList[i]->getLongitude();
            } else {
                topOrBottomMostLongitude = theLocationList[i]->getLongitude();
            }

            if (theGradientType >= RGradientType::DDM) {
                if ((theLocationList[k]->getLatitude() - latitude) * (theLocationList[j]->getLatitude() - latitude) < 0) {
                    isDifferentiable = false;
                } else {
                    // This condition is only for the cases when DDM is needed and the point is a vertex whose outgoing lines go in the same side of the latitude or one of the lines goes on the latitude
                    double lngForDiff;
                    double perturb = 0.0001;
                    double intersectionLng = theLocationList[i]->getLongitude();

                    double sign = 1.0;
                    if (latitude > theLocationList[j]->getLatitude()) {
                        sign = -1;
                    }
                    int result = theLocationList[i]->computeIntersectionWithLatitude(latitude * (1 + sign * perturb), theLocationList[j], &lngForDiff);
                    if (result >= 0) {
                        double dIntersectionLongitude_dLatitude = (lngForDiff - intersectionLng) / (sign * perturb * latitude);
                        dLngVector << dIntersectionLongitude_dLatitude;
                    }

                    sign = 1.0;
                    if (latitude > theLocationList[k]->getLatitude()) {
                        sign = -1;
                    }
                    result = theLocationList[i]->computeIntersectionWithLatitude(latitude * (1 + sign * perturb), theLocationList[k], &lngForDiff);
                    if (result >= 0) {
                        double dIntersectionLongitude_dLatitude = (lngForDiff - intersectionLng) / (sign * perturb * latitude);
                        dLngVector << dIntersectionLongitude_dLatitude;
                    }

                    // Correcting the order of the derivatives in the vector if this is a vertex whose outgoing lines go in the same side of the latitude
                    if ((theLocationList[k]->getLatitude() - latitude) * (theLocationList[j]->getLatitude() - latitude) != 0) {
                        if (theLocationList[j]->getLongitude() > theLocationList[k]->getLongitude()) {
                            // Swapping
                            dLngVector.append(dLngVector[0]);
                            dLngVector.remove(0);
                        }
                    }
                }
            }
        }
    }
    QVector<double> lngVectorBackup = lngVector;
    qSort(lngVector.begin(), lngVector.end());

    double deltaLng = 0;
    for (int i = 0; i < lngVector.count(); i += 2) {
        deltaLng += qAbs(lngVector[i+1] - lngVector[i]);
    }


    double dLng_dLat = 0;
    if (dLngVector.count() != 2) {
        isDifferentiable = false;
    } else {
        if (theGradientType >= RGradientType::DDM) {
            if (lngVector.count() == 2) {
                // Correcting the order of the derivatives in the vector
                if (lngVectorBackup[0] > lngVectorBackup[1]) {
                    dLngVector.append(dLngVector[0]);
                    dLngVector.remove(0);
                }
            }
            dLng_dLat = (dLngVector[1] - dLngVector[0]) * theta2 +  dLngVector[0];
        }
    }


    double lngPortion = deltaLng * theta2;
    double lngSoFar = 0;
    double longitude;
    for (int i = 0; i < lngVector.count(); i += 2) {
        lngSoFar += qAbs(lngVector[i+1] - lngVector[i]);
        if (lngSoFar >= lngPortion) {
            longitude = lngVector[i + 1] - (lngSoFar - lngPortion);
            break;
        }
    }

    // If the point is topmost ot bottommost point
    if (lngVector.isEmpty()) {
        longitude = topOrBottomMostLongitude;
    }

    theHypocentreLocationObject->setLatitude(latitude);
    theHypocentreLocationObject->setLongitude(longitude);


    //if (theOutputLevel >= RObject::Medium) {
    //	qDebug() << qPrintable(QString("%1\t%2").arg(longitude, 0, 'g', 12).arg(latitude, 0, 'g', 12));
    //}

    double depth = theDepth->getCurrentValue();
    theHypocentreLocationObject->setAltitude(-1.0 * depth);

    theHypocentreLocationObject->redisplayonMap();

    if (theGradientType >= RGradientType::DDM) {
        if (!isDifferentiable) {
            //qCritical() << "Warning: The model" << objectName() << "is discontinuous and thus, not differentiable using DDM.";
            //return -10;  //?????????????????????????????
        }

        double dLat_dTheta1 = 1.0 / theLatitudeRandomVariable->getPDFValue(latitude);
        double dLat_dTheta2 = 0.0;
        double dLng_dTheta1 = dLng_dLat * dLat_dTheta1;
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
            double pert;
            if (theta1 == 0) {
                pert = 0.01;
            } else {
                pert = 0.001 * theta1;
            }
            pert *= -1;
            theta1 += pert;
            double newlat = theLatitudeRandomVariable->getInverseCDFValue(theta1);


            QVector<double> lngVector;
            for (int i = 0; i < theLocationList.count(); i++) {
                int j = i + 1;
                if (j == theLocationList.count()) {
                    j = 0;
                }
                if ((theLocationList[i]->getLatitude() - newlat) * (theLocationList[j]->getLatitude() - newlat) < 0) {
                    double intersectionLng;
                    int result = theLocationList[i]->computeIntersectionWithLatitude(newlat, theLocationList[j], &intersectionLng);
                    if (result < 0) {
                        qCritical() << "Error: Cannot compute the intersection in model" << objectName() << ".";
                    }
                    lngVector << intersectionLng;
                } else if (theLocationList[i]->getLatitude() == newlat) {
                    // The point i iteself is on the newlat

                    // k is the previous point
                    int k = i - 1;
                    if (k == -1) {
                        k = theLocationList.count() - 1;
                    }

                    // Add the point i's longitude to the lngVector only if the the lines that go out of point j are not at the same side of the newlat
                    if ((theLocationList[k]->getLatitude() - newlat) * (theLocationList[j]->getLatitude() - newlat) <= 0) {
                        lngVector << theLocationList[i]->getLongitude();
                    }
                }
            }
            qSort(lngVector.begin(), lngVector.end());

            double deltaLng = 0;
            for (int i = 0; i < lngVector.count(); i += 2) {
                deltaLng += qAbs(lngVector[i+1] - lngVector[i]);
            }
            double lngPortion = deltaLng * theta2;
            double lngSoFar = 0;
            double newlng;
            for (int i = 0; i < lngVector.count(); i += 2) {
                lngSoFar += qAbs(lngVector[i+1] - lngVector[i]);
                if (lngSoFar >= lngPortion) {
                    newlng = lngVector[i + 1] - (lngSoFar - lngPortion);
                    break;
                }
            }



            double dLat_dTheta1DF = (newlat - latitude) / pert;
            double dLng_dTheta1DF = (newlng - longitude) / pert;
            theta1 -= pert;
        }
        {
            theta2 *= 1.001;
            double newlat = theLatitudeRandomVariable->getInverseCDFValue(theta1);





            QVector<double> lngVector;
            for (int i = 0; i < theLocationList.count(); i++) {
                int j = i + 1;
                if (j == theLocationList.count()) {
                    j = 0;
                }
                if ((theLocationList[i]->getLatitude() - newlat) * (theLocationList[j]->getLatitude() - latitude) < 0) {
                    double intersectionLng;
                    int result = theLocationList[i]->computeIntersectionWithLatitude(latitude, theLocationList[j], &intersectionLng);
                    if (result < 0) {
                        qCritical() << "Error: Cannot compute the intersection in model" << objectName() << ".";
                    }
                    lngVector << intersectionLng;
                } else if (theLocationList[i]->getLatitude() == latitude) {
                    // The point i iteself is on the latitude

                    // k is the previous point
                    int k = i - 1;
                    if (k == -1) {
                        k = theLocationList.count() - 1;
                    }

                    // Add the point i's longitude to the lngVector only if the the lines that go out of point j are not at the same side of the latitude
                    if ((theLocationList[k]->getLatitude() - latitude) * (theLocationList[j]->getLatitude() - latitude) <= 0) {
                        lngVector << theLocationList[i]->getLongitude();
                    }
                }
            }
            qSort(lngVector.begin(), lngVector.end());

            double deltaLng = 0;
            for (int i = 0; i < lngVector.count(); i += 2) {
                deltaLng += qAbs(lngVector[i+1] - lngVector[i]);
            }
            double lngPortion = deltaLng * theta2;
            double lngSoFar = 0;
            double newlng;
            for (int i = 0; i < lngVector.count(); i += 2) {
                lngSoFar += qAbs(lngVector[i+1] - lngVector[i]);
                if (lngSoFar >= lngPortion) {
                    newlng = lngVector[i + 1] - (lngSoFar - lngPortion);
                    break;
                }
            }



            double dLat_dTheta2DF = (newlat - latitude) / (0.001 * theta2);
            double dLng_dTheta2DF = (newlng - longitude) / (0.001 * theta2);
            theta2 /= 1.001;
        }*/
    }

    return 1;
}	


QStringList RPolygonalAreaSourceModel::getActionList()
{
    QStringList actionList;
    actionList << "&Display on Map" << "&Remove from Map";
    return actionList;
}


void RPolygonalAreaSourceModel::displayonMap()
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
    theMapWidget->drawPolygon(this, parameterListToLocationList(theBoundaryLocationList), Qt::red);

    */
}


void RPolygonalAreaSourceModel::removefromMap()
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
