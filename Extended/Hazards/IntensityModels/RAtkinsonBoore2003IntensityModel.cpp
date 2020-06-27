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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RAtkinsonBoore2003IntensityModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RLocation.h"
#include "RLocationResponse.h"
#include "RMainWindow.h"

#include <QCoreApplication>
#include <math.h>

RAtkinsonBoore2003IntensityModel::RAtkinsonBoore2003IntensityModel(QObject *parent, QString name)
    : REarthquakeBaseClass(parent, name)
{
    thePeriod = 0.0;
    theEventType = RAtkinsonBoore2003IntensityModel::Interface;
    // This needs to be fixed eventually
    theResponseType = RAtkinsonBoore2003IntensityModel::PGA;
    theSmoothness = RAtkinsonBoore2003IntensityModel::Smoothed;
    theLinearSolver = 0;
    theRjb = 0.0;

    // Create the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    // Create the standard normal random variable
    theDomain->createObject(objectName() + "_epsilon", "RContinuousRandomVariable");
    theEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
    theEpsilon->setMean("0.0");
    theEpsilon->setStandardDeviation("1.0");
    theAutoParametersList << theEpsilon;
}


RAtkinsonBoore2003IntensityModel::~RAtkinsonBoore2003IntensityModel()
{

}


QString RAtkinsonBoore2003IntensityModel::getMagnitudeList()
{
    return parameterListToString(theMagnitudeList);
}


void RAtkinsonBoore2003IntensityModel::setMagnitudeList(QString value)
{
    theMagnitudeList = stringToParameterList(value);
}


QObject * RAtkinsonBoore2003IntensityModel::getLinearSolver() const
{
    return theLinearSolver;
}


void RAtkinsonBoore2003IntensityModel::setLinearSolver(QObject *value)
{
    theLinearSolver = qobject_cast<RLinearSolver *>(value);
}


QString RAtkinsonBoore2003IntensityModel::getHypocentreList()
{
    return parameterListToString(theHypocentreList);
}


void RAtkinsonBoore2003IntensityModel::setHypocentreList(QString value)
{
    theHypocentreList = stringToParameterList(value);

    // Checking if all the passed parameters are either RLocation or RLocationResponse
    parameterListToLocationList(theHypocentreList);
}


RAtkinsonBoore2003IntensityModel::REventType RAtkinsonBoore2003IntensityModel::getEventType() const
{
    return theEventType;
}


void RAtkinsonBoore2003IntensityModel::setEventType(RAtkinsonBoore2003IntensityModel::REventType value)
{
    theEventType = value;
}


RAtkinsonBoore2003IntensityModel::RSmoothness RAtkinsonBoore2003IntensityModel::getSmoothness() const
{
    return theSmoothness;
}


void RAtkinsonBoore2003IntensityModel::setSmoothness(RAtkinsonBoore2003IntensityModel::RSmoothness value)
{
    theSmoothness = value;
}


bool RAtkinsonBoore2003IntensityModel::isOn()
{
    // Check the magnitude: that is the model associated with an occurrence model
    bool hereIsARealMagnitude = false;
    for (int i = 0; i < theMagnitudeList.count(); i++) {

        double M = theMagnitudeList[i]->getCurrentValue();

        if (M != 0.0)
        {
            // qDebug()<<"For magnitude response "<<theMagnitudeList[i]->objectName()<<" the magnitude is "<<M;
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


int RAtkinsonBoore2003IntensityModel::evaluateModel(RGradientType theGradientType)
{
    // First check that we a location where the intensity shall be computed
    if (!theLocation) {
        theResponse->setCurrentValue(0.0);
        return 1;
    }

    // End this quickly if there is no magnitude
    if (!isOn()) {
        theResponse->setCurrentValue(0.0);
        return 0;
    }


    // Checking if the number of magnitudes is equal to the number of hypocentre Locations
    if (theHypocentreList.count() != theMagnitudeList.count()) {
        qCritical() << "Error: The number of magnitude parameters should be equal to the number of hypocentre Locations in the model" << objectName() << ".";
        return -1;
    }

    double epsilon = theEpsilon->getCurrentValue();

    // Period dependent coefficients
    QVector<double> periodTableVector;
    periodTableVector << 0 << 0.04 << 0.1 << 0.2 << 0.4 << 1 << 2 << 3.03030303;

    QVector<double> c1InterfaceVector;
    c1InterfaceVector << 2.991 << 2.8753 << 2.7789 << 2.6638 << 2.5249 << 2.1442 << 2.1907 << 2.301;
    QVector<double> c2InterfaceVector;
    c2InterfaceVector << 0.03525 << 0.07052 << 0.09841 << 0.12386 << 0.1477 << 0.1345 << 0.07148 << 0.02237;
    QVector<double> c3InterfaceVector;
    c3InterfaceVector << 0.00759 << 0.01004 << 0.00974 << 0.00884 << 0.00728 << 0.00521 << 0.00224 << 0.00012;
    QVector<double> c4InterfaceVector;
    c4InterfaceVector << -0.00206 << -0.00278 << -0.00287 << -0.0028 << -0.00235 << -0.0011 << 0 << 0;
    QVector<double> c5InterfaceVector;
    c5InterfaceVector << 0.19 << 0.15 << 0.15 << 0.15 << 0.13 << 0.1 << 0.1 << 0.1;
    QVector<double> c6InterfaceVector;
    c6InterfaceVector << 0.24 << 0.2 << 0.23 << 0.27 << 0.37 << 0.3 << 0.25 << 0.25;
    QVector<double> c7InterfaceVector;
    c7InterfaceVector << 0.29 << 0.2 << 0.2 << 0.25 << 0.38 << 0.55 << 0.4 << 0.36;
    QVector<double> sigmaInterfaceVector;
    sigmaInterfaceVector << 0.23 << 0.26 << 0.27 << 0.28 << 0.29 << 0.34 << 0.34 << 0.36;

    QVector<double> c1InSlabVector;
    c1InSlabVector << -0.04713 << 0.50697 << 0.43928 << 0.51589 << 0.005445 << -1.02133 << -2.39234 << -3.70012;
    QVector<double> c2InSlabVector;
    c2InSlabVector << 0.6909 << 0.63273 << 0.66675 << 0.69186 << 0.7727 << 0.8789 << 0.9964 << 1.1169;
    QVector<double> c3InSlabVector;
    c3InSlabVector << 0.0113 << 0.01275 << 0.0108 << 0.00572 << 0.00173 << 0.0013 << 0.00364 << 0.00615;
    QVector<double> c4InSlabVector;
    c4InSlabVector << -0.00202 << -0.00234 << -0.00219 << -0.00192 << -0.00178 << -0.00173 << -0.00118 << -0.00045;
    QVector<double> c5InSlabVector;
    c5InSlabVector << 0.19 << 0.15 << 0.15 << 0.15 << 0.13 << 0.1 << 0.1 << 0.1;
    QVector<double> c6InSlabVector;
    c6InSlabVector << 0.24 << 0.2 << 0.23 << 0.27 << 0.37 << 0.3 << 0.25 << 0.25;
    QVector<double> c7InSlabVector;
    c7InSlabVector << 0.29 << 0.2 << 0.2 << 0.25 << 0.38 << 0.55 << 0.4 << 0.36;
    QVector<double> sigmaInSlabVector;
    sigmaInSlabVector << 0.27 << 0.25 << 0.28 << 0.28 << 0.28 << 0.29 << 0.3 << 0.3;

    double T = 0.0;
    if (theResponseType == RAtkinsonBoore2003IntensityModel::PGA) {
        T = 0.0;
    }
    if (theResponseType == RAtkinsonBoore2003IntensityModel::Sa) {
        T = thePeriod;
    }
    double spectralResponse;


    for (int index = 0; index < theMagnitudeList.count(); index++)
    {
        // For convenience, look up the values of the parameters first
        double M = theMagnitudeList[index]->getCurrentValue();

        // IMPORTANT: If M = 0, it means that this hazard is off, so the resulting PGA is Zero
        if (M == 0.0) {
            continue;
        }

        if (theOutputLevel >= RObject::Medium) {
            if (M < 5.0) {
                qCritical() << "Warning: The magnitude in the model" << objectName() << "should be more than 5.";
            }
        }
        double gg;
        if (theEventType == RAtkinsonBoore2003IntensityModel::InSlab) {
            double maxM = 8.0;
            double MEpsilon = 0.1;
            if (M > maxM) {
                // Continuous
                if (theSmoothness == RAtkinsonBoore2003IntensityModel::Smoothed) {
                    if (M <= maxM + 2 * MEpsilon) {
                        // Smoothing
                        int result = cubicSmooth(maxM + MEpsilon, MEpsilon, maxM, maxM + MEpsilon, 1.0, 0.0, M, &M);
                    }
                    else {
                        M = maxM + MEpsilon;
                    }
                }
                else {
                    // Discontinuity
                    M = maxM;
                }
                if (theOutputLevel >= RObject::Medium) {
                    qCritical() << "Warning: The magnitude in the model" << objectName() << "is more than 8. Rts will set the megnitude to 8 for the in-slab event.";
                }
            }

            gg = pow(10.0, 0.301 - 0.01 * M);
        } else if (theEventType == RAtkinsonBoore2003IntensityModel::Interface) {
            double maxM = 8.5;
            double MEpsilon = 0.1;
            if (M > maxM) {
                // Continuous
                if (theSmoothness == RAtkinsonBoore2003IntensityModel::Smoothed) {
                    if (M <= maxM + 2 * MEpsilon) {
                        // Smoothing
                        int result = cubicSmooth(maxM + MEpsilon, MEpsilon, maxM, maxM + MEpsilon, 1.0, 0.0, M, &M);
                    }
                    else {
                        M = maxM + MEpsilon;
                    }
                }
                else {
                    // Discontinuity
                    M = maxM;
                }
                if (theOutputLevel >= RObject::Medium) {
                    qCritical() << "Warning: The magnitude in the model" << objectName() << "is more than 8.5. Qt will set the megnitude to 8 for the interface event.";
                }
            }

            gg = pow(10.0, 1.2 - 0.18 * M);
        }



        // Verification that the user has given an RLocationResponse
        RLocation *theHypocentreLocationObject = parameterToLocation(theHypocentreList[index]);
        if (!theHypocentreLocationObject) {
            qCritical() << "Error: The Location response" << theHypocentreList[index]->objectName() << "does not include any Location object.";
            return -1;
        }


        // Epicentral distance
        double Dfault = theHypocentreLocationObject->computeSurfaceDistance(parameterToLocation(theLocation));
        if (theEventType == RAtkinsonBoore2003IntensityModel::InSlab) {
            if (Dfault > 200.0) {
                if (theOutputLevel >= RObject::Medium) {
                    qDebug() << "The epicentre-to-site distance in" << objectName() << "is large at" << Dfault << "km; the intensity is set to zero";
                }
                theResponse->setCurrentValue(0.0);
                return 1;
            }
        } else if (theEventType == RAtkinsonBoore2003IntensityModel::Interface) {
            if (Dfault > 300.0) {
                if (theOutputLevel >= RObject::Medium) {
                    qDebug() << "The epicentre-to-site distance in" << objectName() << "is large at" << Dfault << "km; the intensity is set to zero";
                }
                theResponse->setCurrentValue(0.0);
                return 1;
            }
        }

        // Depth
        double h = qAbs(theHypocentreLocationObject->getAltitude());
        double maxh = 100.0;
        if (h > maxh) {
            // Continuous
            double hEpsilon = 5;
            if (theSmoothness == RAtkinsonBoore2003IntensityModel::Smoothed) {
                if (h <= maxh + 2 * hEpsilon) {
                    // Smoothing
                    int result = cubicSmooth(maxh + hEpsilon, hEpsilon, maxh, maxh + hEpsilon, 1.0, 0.0, h, &h);
                } else {
                    h = maxh + hEpsilon;
                }
            }
            else {
                // Discontinuity
                h = maxh;
            }
            if (theOutputLevel >= RObject::Medium) {
                qCritical() << "Warning: The depth in the model" << objectName() << "is more than 100 km. Qt will set the depth to 100 km.";
            }
        }

        // Near-source saturation term
        double Delta = 0.00724 * pow(10.0, 0.507 * M);

        // Compute the distance//??????????????????????????????????????????????????????????????????????????????????????????????
        // Rodrigo: Dfault is the distance
        double R = sqrt((Dfault)*(Dfault) + (Delta)*(Delta));
        theRjb = Dfault;


        if (T > periodTableVector.last() || T < periodTableVector.first()) {
            // RESTORE THE WARNING WHEN THE PAPER IS DONE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            // qCritical() << "Warning: The period" << T << "from the parameter" << thePeriodList[i]->objectName() << "entered in the model" << objectName() << "is out of the range of" << periodTableVector.first() << "and" << periodTableVector.last() << "sec.";
            if (T > periodTableVector.last()) {
                T = periodTableVector.last();
            }
            //return -1;
        }



        double Vs30 = theLocation->getShearWaveVelocityValue();
        double SC = 0;
        double SD = 0;
        double SE = 0;
        double Vs30Epsilon = 40; // m/s
        // Continuous
        if (theSmoothness == RAtkinsonBoore2003IntensityModel::Smoothed) {
            if (Vs30 <= 180 - Vs30Epsilon) {
                SE = 1.0;
            }
            else if (Vs30 > 180 - Vs30Epsilon && Vs30 <= 180 + Vs30Epsilon) {
                // Smoothing
                int result = cubicSmooth(180.0, Vs30Epsilon, 1.0, 0.0, 0.0, 0.0, Vs30, &SE);
                //result = cubicSmooth(180.0, Vs30Epsilon, 0.0, 1.0, 0.0, 0.0, Vs30, &SD);
                SD = 1.0 - SE;
            }
            else if (Vs30 > 180 + Vs30Epsilon && Vs30 <= 360 - Vs30Epsilon) {
                SD = 1.0;
            }
            else if (Vs30 > 360 - Vs30Epsilon && Vs30 <= 360 + Vs30Epsilon) {
                // Smoothing
                int result = cubicSmooth(360.0, Vs30Epsilon, 1.0, 0.0, 0.0, 0.0, Vs30, &SD);
                //result = cubicSmooth(360.0, Vs30Epsilon, 0.0, 1.0, 0.0, 0.0, Vs30, &SC);
                SC = 1.0 - SD;
            }
            else if (Vs30 > 360 + Vs30Epsilon && Vs30 <= 760 - Vs30Epsilon) {
                SC = 1.0;
            }
            else if (Vs30 > 760 - Vs30Epsilon && Vs30 <= 760 + Vs30Epsilon) {
                // Smoothing
                int result = cubicSmooth(760, Vs30Epsilon, 1.0, 0.0, 0.0, 0.0, Vs30, &SC);
            }
        }
        else {
            // Discountinuity
            if (Vs30 < 180) {
                SE = 1.0;
            } else if (Vs30 <= 360) {
                SD = 1.0;
            } else if (Vs30 <= 760) {
                SC = 1.0;
            }
        }




        int lowerIndex = periodTableVector.indexOf(T);
        double *upperIndexIterator;
        int upperIndex;
        if (lowerIndex == -1) {
            upperIndexIterator = qUpperBound(periodTableVector.begin(), periodTableVector.end(), T);
            upperIndex = periodTableVector.indexOf(*upperIndexIterator);
            lowerIndex = upperIndex - 1;
        } else {
            upperIndex = lowerIndex;
        }


        QVector<double> log10YVector;
        for (int j = lowerIndex; j <= upperIndex; j++) { // At max, there will be 2 iterations
            double c1;
            double c2;
            double c3;
            double c4;
            double c5;
            double c6;
            double c7;
            double sigma;

            if (theEventType == RAtkinsonBoore2003IntensityModel::InSlab) {
                c1 = c1InSlabVector[j];
                c2 = c2InSlabVector[j];
                c3 = c3InSlabVector[j];
                c4 = c4InSlabVector[j];
                c5 = c5InSlabVector[j];
                c6 = c6InSlabVector[j];
                c7 = c7InSlabVector[j];
                sigma = sigmaInSlabVector[j];
            } else if (theEventType == RAtkinsonBoore2003IntensityModel::Interface) {
                c1 = c1InterfaceVector[j];
                c2 = c2InterfaceVector[j];
                c3 = c3InterfaceVector[j];
                c4 = c4InterfaceVector[j];
                c5 = c5InterfaceVector[j];
                c6 = c6InterfaceVector[j];
                c7 = c7InterfaceVector[j];
                sigma = sigmaInterfaceVector[j];
            }

            double fnM = c1 + c2 * M;

            double PGArx;
            {
                // For PGArx, calculate the PGA on rock: SC=SD=SE=0.0
                double c1;
                double c2;
                double c3;
                double c4;
                double c5;
                double c6;
                double c7;
                double sigma;

                if (theEventType == RAtkinsonBoore2003IntensityModel::InSlab) {
                    c1 = c1InSlabVector[0];
                    c2 = c2InSlabVector[0];
                    c3 = c3InSlabVector[0];
                    c4 = c4InSlabVector[0];
                    c5 = c5InSlabVector[0];
                    c6 = c6InSlabVector[0];
                    c7 = c7InSlabVector[0];
                    sigma = sigmaInSlabVector[0];
                } else if (theEventType == RAtkinsonBoore2003IntensityModel::Interface) {
                    c1 = c1InterfaceVector[0];
                    c2 = c2InterfaceVector[0];
                    c3 = c3InterfaceVector[0];
                    c4 = c4InterfaceVector[0];
                    c5 = c5InterfaceVector[0];
                    c6 = c6InterfaceVector[0];
                    c7 = c7InterfaceVector[0];
                    sigma = sigmaInterfaceVector[0];
                }

                double log10Y = fnM + c3 * h + c4 * R - gg * log10(R) + sigma * epsilon;
                PGArx = pow(10.0, log10Y); // Units in cm/s/s
            }

            double sl;
            double f = 1.0 / T;
            double PGArxEpsilon = 25;
            // Continuous
            if (theSmoothness == RAtkinsonBoore2003IntensityModel::Smoothed) {
                if (f <= 1) {
                    sl = 1.0;
                } else {
                    if (PGArx <= 100 - PGArxEpsilon) {
                        sl = 1.0;
                    }
                    else if (PGArx > 100 - PGArxEpsilon && PGArx <= 100 + PGArxEpsilon) {
                        // Smoothing
                        if (f > 1 && f < 2) {
                            int result = cubicSmooth(100.0, PGArxEpsilon, 1.0, 1.0 - (f - 1.0) * (PGArx - 100.0) / 400.0, 0.0, - (f - 1.0) / 400.0, PGArx, &sl);
                        }
                        if (f >= 2) {
                            int result = cubicSmooth(100.0, PGArxEpsilon, 1.0, 1.0 - (PGArx - 100.0) / 400.0, 0.0, -1.0 / 400.0, PGArx, &sl);
                        }
                    }
                    else if (PGArx > 100 + PGArxEpsilon && PGArx <= 500 - PGArxEpsilon) {
                        if (f > 1 && f < 2) {
                            sl = 1.0 - (f - 1.0) * (PGArx - 100.0) / 400.0;
                        }
                        if (f >= 2) {
                            sl = 1.0 - (PGArx - 100.0) / 400.0;
                        }
                    }
                    else if (PGArx > 500 - PGArxEpsilon && PGArx <= 500 + PGArxEpsilon) {
                        // Smoothing
                        if (f > 1 && f < 2) {
                            int result = cubicSmooth(500.0, PGArxEpsilon, 1.0 - (f - 1.0) * (PGArx - 100.0) / 400.0, 1.0 - (f - 1.0), 0.0, 0.0, PGArx, &sl);
                        }
                        if (f >= 2) {
                            int result = cubicSmooth(500.0, PGArxEpsilon, 1.0 - (PGArx - 100.0) / 400.0, 0.0, 0.0, 0.0, PGArx, &sl);
                        }
                    }
                    else if (PGArx > 500 + PGArxEpsilon) {
                        if (f > 1 && f < 2) {
                            sl = 1.0 - (f - 1.0);
                        }
                        if (f >= 2) {
                            sl = 0.0;
                        }
                    }
                }
            }
            else {
                // Discontinuity
                if (PGArx <= 100 || f <= 1) {
                    sl = 1.0;
                }
                if (PGArx > 100 && PGArx < 500) {
                    if (f > 1 && f < 2) {
                        sl = 1.0 - (f - 1.0) * (PGArx - 100.0) / 400.0;
                    }
                    if (f >= 2) {
                        sl = 1.0 - (PGArx - 100.0) / 400.0;
                    }
                }
                if (PGArx >= 500) {
                    if (f > 1 && f < 2) {
                        sl = 1.0 - (f - 1.0);
                    }
                    if (f >= 2) {
                        sl = 0.0;
                    }
                }
            }

            double log10Y = fnM + c3 * h + c4 * R - gg * log10(R) +   c5 * sl * SC +   c6 * sl * SD +   c7 * sl * SE +   sigma * epsilon;
            log10YVector << log10Y;
        }


        double log10Y;
        if (log10YVector.count() == 1) {
            log10Y = log10YVector[0];
        } else {//Is this right?////////////////////////////////////////////////////???????????????????????????????????????????????
            log10Y = (log10YVector[1] - log10YVector[0]) / (1.0 / periodTableVector[upperIndex] - 1.0 / periodTableVector[lowerIndex]) * (1.0 / T - 1.0 / periodTableVector[lowerIndex]) + log10YVector[0];
        }

        double Y = pow(10.0, log10Y) / 981.0; // The units of Y is g for Sa and PGA

        spectralResponse += Y;

    }

    theResponse->setCurrentValue(spectralResponse);

    return 1;
}


int RAtkinsonBoore2003IntensityModel::cubicSmooth(double x0, double epsilon, double y1, double y2, double dy1, double dy2, double *a, double *b, double *c, double *d)
{
    double x1 = x0 - epsilon;
    double x2 = x0 + epsilon;

    QVector< QVector<double> > *AA = new QVector< QVector<double> >(4, QVector<double>(4));

    (*AA)[0][0] = (x1)*(x1)*(x1);
    (*AA)[0][1] = (x1)*(x1);
    (*AA)[0][2] = x1;
    (*AA)[0][3] = 1.0;

    (*AA)[1][0] = (x2)*(x2)*(x2);
    (*AA)[1][1] = (x2)*(x2);
    (*AA)[1][2] = x2;
    (*AA)[1][3] = 1.0;

    (*AA)[2][0] = 3 * (x1)*(x1);
    (*AA)[2][1] = 2 * x1;
    (*AA)[2][2] = 1.0;
    (*AA)[2][3] = 0.0;

    (*AA)[3][0] = 3 * (x2)*(x2);
    (*AA)[3][1] = 2 * x2;
    (*AA)[3][2] = 1.0;
    (*AA)[3][3] = 0.0;

    QVector<double> *YY = new QVector<double>(4);

    (*YY)[0] = y1;
    (*YY)[1] = y2;
    (*YY)[2] = dy1;
    (*YY)[3] = dy2;

    QVector<double> *XX = new QVector<double>(4);


    theLinearSolver->solveLinearSOE(AA,YY,XX);

    *a = (*XX)[0];
    *b = (*XX)[1];
    *c = (*XX)[2];
    *d = (*XX)[3];


    delete XX;
    delete YY;
    delete AA;

    return 0;
}


int RAtkinsonBoore2003IntensityModel::cubicSmooth(double x0, double epsilon, double y1, double y2, double dy1, double dy2, double x, double *y)
{
    double a;
    double b;
    double c;
    double d;
    int result = cubicSmooth(x0, epsilon, y1, y2, dy1, dy2, &a, &b, &c, &d);
    if (result < 0) {
        return result;
    }
    *y = a * x*x*x + b * x*x + c * x + d;
    return 0;
}



