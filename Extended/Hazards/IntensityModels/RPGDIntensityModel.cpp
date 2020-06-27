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

#include "RPGDIntensityModel.h"
#include <QCoreApplication>
#include "RParameter.h"
#include "RResponse.h"
#include "RLocation.h"
#include "RLocationResponse.h"
#include "RMainWindow.h"
#include <math.h>


RPGDIntensityModel::RPGDIntensityModel(QObject *parent, QString name)
    : RIntensityModel(parent, name)
{
    thePeakGroundAcceleration = nullptr;
    theLiquefactionSusceptibility = 0.0;
    theProbabilityOfLiquefaction = 0.0;
    theVerticalPGD = 0.0;
    theLateralPGD = 0.0;
    theExpectedPGD = 0.0;

    // landslide parameters
    theLandslideSusceptibility = 0.0;
    cprime = 0.0;
    phiprime = 0.0;
    minimumSlopeAngle = 0.0;
    minimumCriticalAcceleration = 0.0;

    // Create the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}


RPGDIntensityModel::~RPGDIntensityModel()
{

}


int RPGDIntensityModel::resetTime()
{
    // ground displacement parameters
    theLiquefactionSusceptibility = 0.0;
    theProbabilityOfLiquefaction = 0.0;
    theVerticalPGD = 0.0;
    theLateralPGD = 0.0;
    theResponse->setCurrentValue(0.0);

    // landslide parameters
    theLandslideSusceptibility = 0.0;
    cprime = 0.0;
    phiprime = 0.0;
    minimumSlopeAngle = 0.0;
    minimumCriticalAcceleration = 0.0;

    return 0;
}


QString RPGDIntensityModel::getMagnitudeList()
{
    return parameterListToString(theMagnitudeList);
}


void RPGDIntensityModel::setMagnitudeList(QString value)
{
    theMagnitudeList = stringToParameterList(value);
}


QObject * RPGDIntensityModel::getPeakGroundAcceleration() const
{
    return thePeakGroundAcceleration;
}


void RPGDIntensityModel::setPeakGroundAcceleration(QObject *value)
{
    thePeakGroundAcceleration = qobject_cast<RParameter *>(value);
}


QString RPGDIntensityModel::getHypocentreList()
{
    return parameterListToString(theHypocentreList);
}


void RPGDIntensityModel::setHypocentreList(QString value)
{
    theHypocentreList = stringToParameterList(value);

    // Checking if all the passed parameters are either RLocation or RLocationResponse
    parameterListToLocationList(theHypocentreList);
}


void RPGDIntensityModel::setSoilSusceptibilityLevel(QString name)
{
    theSoilSusceptibilityLevel = name;
}


double RPGDIntensityModel::getProbabilityOfLiquefaction()
{
    return theProbabilityOfLiquefaction;
}


double RPGDIntensityModel::getLateralPGD()
{
    return theLateralPGD;
}


bool RPGDIntensityModel::isOn()
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


int RPGDIntensityModel::evaluateModel(RGradientType theGradientType)
{
    // THIS MODEL CALCULATES PGD CAUSED BY LIQUEFACTION

    // This model was double checked agains the algorithm presented in the latest Hazus manual.
    // It suffers from the problem of not having a map of liquefaction susceptibility built-in Rts.
    // It is assumed that ports are at high susceptibility of liquefaction due to being close to the water.
    // All other models are considered at moderate susceptibility of liquefaction.
    // This will need to be improved eventually.

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

    // Declare the reponse and intensity model producing the PGA
    RResponse *theResponseObject;
    RIntensityModel *theIntensityModel;


    // Get hold of the intensity model
    theResponseObject = qobject_cast<RResponse *>(thePeakGroundAcceleration);
    if (theResponseObject == 0) {

        qCritical() << this->objectName() << "did not get a PGA response as input.";
    }

    // get the PGA intensity model
    theIntensityModel = qobject_cast<RIntensityModel *>(theResponseObject->getModel());

    if (theIntensityModel == 0) {
        qCritical() << this->objectName() << "could not find a PGA model in the input.";
    }


    // Tell the PGA model what we want (location and type of intensity measure) and compute the PGA
    theIntensityModel->setLocation(theLocation);
    theIntensityModel->setIntensityMeasure(QString("PGA"));
    theIntensityModel->evaluateModel(RGradientType::NONE);
    double thePGA = thePeakGroundAcceleration->getCurrentValue();


    // This is where we need code to calculate PGD, namely theExpected PGD

    // 0 - variables
    double theKm = 0.0;
    double theKw = 0.0;
    double theGroundwaterDepth = 5.0; //in feet (the factors were obtained for GWD of 5 feet - page 140 Hazus 2.1)
    double theDisplacementCorrectionFactor = 0.0;
    double theThresholdPGA = 0.0;
    double theProportionOfTheMap = 0.0;

    QVector<double> groundSettlementSusceptibilityVector;
    QVector<double> thresholdPGAVector;
    QVector<double> theProportionOfTheMapVector;


    // See Table 4.13 Hazus report - a large number here denotes a soil with no susceptibility to liquefaction.
    //                   None << Very Low << Low << Moderate << High << Very High susceptibilities
    thresholdPGAVector << 1000.0 << 0.26 << 0.21 << 0.15 << 0.12 << 0.09;
    groundSettlementSusceptibilityVector << 0.0 << 0.0 << 1.0 << 2.0 << 6.0 << 12.0;
    theProportionOfTheMapVector << 0.0 << 0.02 << 0.05 << 0.10 << 0.20 << 0.25;


    // 1 - Define if the fixed structure is or is not on a zone susceptible to liquefaction
    // This is being done by the RFixedLocationNetworkModelN now


    // 2 - Conditional probability of liquefaction for a susceptibility category (this is the Prob. of Liquefaction for a given set of conditions)
    // The vertical PGD is a simple function of the susceptibility level, therefore it is calculated here too
    // See Hazus 2.1 item 4.2.2.1.3 on page 138 of the pdf file
    if (theSoilSusceptibilityLevel == QString("None")){
        theThresholdPGA = thresholdPGAVector[0];
        theVerticalPGD = groundSettlementSusceptibilityVector[0];
        theLiquefactionSusceptibility = 0.0;
        theProportionOfTheMap = theProportionOfTheMapVector[0];

    } else if (theSoilSusceptibilityLevel == QString("VeryLow")){
        theThresholdPGA = thresholdPGAVector[1];
        theVerticalPGD = groundSettlementSusceptibilityVector[1];
        theLiquefactionSusceptibility = 4.16 * thePGA - 1.08;
        theProportionOfTheMap = theProportionOfTheMapVector[1];

    } else if (theSoilSusceptibilityLevel == QString("Low")){
        theThresholdPGA = thresholdPGAVector[2];
        theVerticalPGD = groundSettlementSusceptibilityVector[2];
        theLiquefactionSusceptibility = 5.57 * thePGA - 1.18;
        theProportionOfTheMap = theProportionOfTheMapVector[2];

    } else if (theSoilSusceptibilityLevel == QString("Moderate")){
        theThresholdPGA = thresholdPGAVector[3];
        theVerticalPGD = groundSettlementSusceptibilityVector[3];
        theLiquefactionSusceptibility = 6.67 * thePGA - 1.0;
        theProportionOfTheMap = theProportionOfTheMapVector[3];

    } else if (theSoilSusceptibilityLevel == QString("High")){
        theThresholdPGA = thresholdPGAVector[4];
        theVerticalPGD = groundSettlementSusceptibilityVector[4];
        theLiquefactionSusceptibility = 7.67 * thePGA - 0.92;
        theProportionOfTheMap = theProportionOfTheMapVector[4];


    } else if (theSoilSusceptibilityLevel == QString("VeryHigh")){
        theThresholdPGA = thresholdPGAVector[5];
        theVerticalPGD = groundSettlementSusceptibilityVector[5];
        theLiquefactionSusceptibility = 9.09 * thePGA - 0.82;
        theProportionOfTheMap = theProportionOfTheMapVector[5];
    }


    // 2.5 - Bounds for the conditional probability of liquefaction
    if (theLiquefactionSusceptibility < 0.0){
        theLiquefactionSusceptibility = 0.0;

    } else if (theLiquefactionSusceptibility > 1.0) {
        theLiquefactionSusceptibility = 1.0;
    }


    // 3 - Expected PGD

    // get the earthquake magnitude
    double M = theMagnitudeList[0]->getCurrentValue();

    // Calculating the probability of soil liquefaction
    theKm = 0.0027 * M*M*M - 0.0267 * M*M - 0.2055 * M + 2.9188;
    theKw = 0.022 * theGroundwaterDepth + 0.93;
    theProbabilityOfLiquefaction = theLiquefactionSusceptibility*(theProportionOfTheMap/(theKm*theKw));

    // For lateral spreading Eq 4.24 Hazus 2.1
    theDisplacementCorrectionFactor = 0.0086 * M*M*M - 0.0914 * M*M + 0.4698 * M - 0.9835;

    // Depending on the ratio between PGA and threshold PGA, calculated the Lateral Spreading in inches
    if (thePGA/theThresholdPGA < 1.0){
        theLateralPGD = 0.0;

        // See Figure 4.9 Hazus 2.1 page 143 of the pdf file
    } else if ( (thePGA/theThresholdPGA) > 1.0 && (thePGA/theThresholdPGA) < 2.0 ) {
        theLateralPGD = (12.0*(thePGA / theThresholdPGA) - 12.0) * theDisplacementCorrectionFactor;

    } else if ( (thePGA/theThresholdPGA) > 2.0 && (thePGA/theThresholdPGA) < 3.0) {
        theLateralPGD = (18.0*(thePGA / theThresholdPGA) - 24.0) * theDisplacementCorrectionFactor;

    } else if ( (thePGA/theThresholdPGA) > 3.0 && (thePGA/theThresholdPGA) < 4.0){
        theLateralPGD = (70.0*(thePGA / theThresholdPGA) - 180.0) * theDisplacementCorrectionFactor;

        // Defining an forced upper bound for lateral spreading
    } else if (thePGA / theThresholdPGA > 4.0){
        //qCritical() << this->objectName() << "the PGA/PGA(t) ratio is too high." << thePGA << theThresholdPGA;
        theLateralPGD = (70.0*4.0 - 180.0) * theDisplacementCorrectionFactor;

    }


    // Ground Settlement
    // The expected value was calculated in the item 2 above.
    // The uncertainty associated with these settlement values is assumed to have a uniform probability distribution within bounds of one-half to two times the respective value.

    // 3.5 - Define the dominant PGD
    if(theLateralPGD != 0.0 || theVerticalPGD != 0.0){

        theExpectedPGD = qMax(theLateralPGD,theVerticalPGD) * theProbabilityOfLiquefaction;
        //qCritical() << theDisplacementCorrectionFactor << thePGA / theThresholdPGA << theProbabilityOfLiquefaction;
    }
    else{

        theExpectedPGD = 0.0;
    }

    theResponse->setCurrentValue(theExpectedPGD);

    return 1;
}


void RPGDIntensityModel::getLandslideSuceptibility(QString geologicalType, bool dry, double angle){


    // If the geologic group is type A: Strongly cemented rocks
    if (geologicalType == QString("A")){

        cprime   = 300;
        phiprime = 35;

        // dry soil - groundwater level below level of sliding
        if (dry){

            minimumSlopeAngle = 15.0;
            minimumCriticalAcceleration = 0.20;

            // check the slope angle
            if (angle < 10.0){
                theLandslideSusceptibility = 0;
            }
            else if (angle > 10.0 && angle <= 15.0){
                theLandslideSusceptibility = 0;

            }
            else if (angle > 15.0 && angle <= 20.0){
                theLandslideSusceptibility = 1;
            }
            else if (angle > 20.0 && angle <= 30.0){
                theLandslideSusceptibility = 2;
            }
            else if (angle > 30.0 && angle <= 40.0){
                theLandslideSusceptibility = 4;
            }
            else if (angle > 40.0){
                theLandslideSusceptibility = 6;
            }
        }

        // wet soil - groundwater level at ground surface
        else if (!dry){

            minimumSlopeAngle = 10.0;
            minimumCriticalAcceleration = 0.15;

            // check the slope angle
            if (angle < 10.0){
                theLandslideSusceptibility = 0;
            }
            else if (angle > 10.0 && angle <= 15.0){
                theLandslideSusceptibility = 3;
            }
            else if (angle > 15.0 && angle <= 20.0){
                theLandslideSusceptibility = 6;
            }
            else if (angle > 20.0 && angle <= 30.0){
                theLandslideSusceptibility = 7;
            }
            else if (angle > 30.0 && angle <= 40.0){
                theLandslideSusceptibility = 8;
            }
            else if (angle > 40.0){
                theLandslideSusceptibility = 8;
            }
        }
    }

    // If the geologic group is type B: Weakly cemented rocks and soil
    else if (geologicalType == QString("B")){

        cprime   = 0;
        phiprime = 35;

        // dry soil - groundwater level below level of sliding
        if (dry){

            minimumSlopeAngle = 10.0;
            minimumCriticalAcceleration = 0.15;

            // check the slope angle
            if (angle < 10.0){
                theLandslideSusceptibility = 0;
            }
            else if (angle > 10.0 && angle <= 15.0){
                theLandslideSusceptibility = 3;
            }
            else if (angle > 15.0 && angle <= 20.0){
                theLandslideSusceptibility = 4;
            }
            else if (angle > 20.0 && angle <= 30.0){
                theLandslideSusceptibility = 5;
            }
            else if (angle > 30.0 && angle <= 40.0){
                theLandslideSusceptibility = 6;
            }
            else if (angle > 40.0){
                theLandslideSusceptibility = 7;
            }
        }

        // wet soil - groundwater level at ground surface
        else if (!dry){

            minimumSlopeAngle = 5.0;
            minimumCriticalAcceleration = 0.10;

            // check the slope angle
            if (angle < 10.0){
                theLandslideSusceptibility = 5;
            }
            else if (angle > 10.0 && angle <= 15.0){
                theLandslideSusceptibility = 8;
            }
            else if (angle > 15.0 && angle <= 20.0){
                theLandslideSusceptibility = 9;
            }
            else if (angle > 20.0 && angle <= 30.0){
                theLandslideSusceptibility = 9;
            }
            else if (angle > 30.0 && angle <= 40.0){
                theLandslideSusceptibility = 9;
            }
            else if (angle > 40.0){
                theLandslideSusceptibility = 10;
            }
        }
    }


    // If the geologic group is type C: Argillaceous rocks
    else if (geologicalType == QString("C")){

        cprime   = 0;
        phiprime = 20;

        // dry soil - groundwater level below level of sliding
        if (dry){

            minimumSlopeAngle = 5;
            minimumCriticalAcceleration = 0.10;

            // check the slope angle
            if (angle < 10.0){
                theLandslideSusceptibility = 5;
            }
            else if (angle > 10.0 && angle <= 15.0){
                theLandslideSusceptibility = 6;
            }
            else if (angle > 15.0 && angle <= 20.0){
                theLandslideSusceptibility = 7;
            }
            else if (angle > 20.0 && angle <= 30.0){
                theLandslideSusceptibility = 9;
            }
            else if (angle > 30.0 && angle <= 40.0){
                theLandslideSusceptibility = 9;
            }
            else if (angle > 40.0){
                theLandslideSusceptibility = 9;
            }
        }

        // wet soil - groundwater level at ground surface
        else if (!dry){

            minimumSlopeAngle = 3.0;
            minimumCriticalAcceleration = 0.05;

            // check the slope angle
            if (angle < 10.0){
                theLandslideSusceptibility = 7;
            }
            else if (angle > 10.0 && angle <= 15.0){
                theLandslideSusceptibility = 9;
            }
            else if (angle > 15.0 && angle <= 20.0){
                theLandslideSusceptibility = 10;
            }
            else if (angle > 20.0 && angle <= 30.0){
                theLandslideSusceptibility = 10;
            }
            else if (angle > 30.0 && angle <= 40.0){
                theLandslideSusceptibility = 10;
            }
            else if (angle > 40.0){
                theLandslideSusceptibility = 10;
            }
        }
    }

}


double RPGDIntensityModel::getLandslidePGD()
{
    /*
    // THIS MODEL CALCULATES PGD CAUSED BY LANDSLIDE

    // First check that we a location where the intensity shall be computed
    if (!theLocation) {
        theResponse->setCurrentValue(0.0);
        return 1;
    }


    // End this quickly if there is no magnitude
    bool hereIsARealMagnitude = false;
    for (int i = 0; i < theMagnitudeList.count(); i++) {

        double M = theMagnitudeList[i]->getCurrentValue();

        if (M != 0.0) {
            hereIsARealMagnitude = true;
        }
    }
    if (!hereIsARealMagnitude) {

        // No real (non-zero) magnitudes, quit now
        theResponse->setCurrentValue(0.0);
        return 0;
    }


    // Checking if the number of magnitudes is equal to the number of hypocentre Locations
    if (theHypocentreList.count() != theMagnitudeList.count()) {
        qCritical() << "Error: The number of magnitude parameters should be equal to the number of hypocentre Locations in the model" << objectName() << ".";
        return -1;
    }

    // Declare the reponse and intensity model producing the PGA
    RResponse *theResponseObject;
    RIntensityModel *theIntensityModel;


    // Get hold of the intensity model
    theResponseObject = qobject_cast<RResponse *>(thePeakGroundAcceleration);
    if (theResponseObject == 0) {

        qCritical() << this->objectName() << "did not get a PGA response as input.";
    }

    // get the PGA intensity model
    theIntensityModel = qobject_cast<RIntensityModel *>(theResponseObject->getModel());

    if (theIntensityModel == 0) {
        qCritical() << this->objectName() << "could not find a PGA model in the input.";
    }


    // vector containing the critical accelerations - see Hazus 2.1 page 149, table 4.17
    QVector<double> criticalAccelerationVector;
    criticalAccelerationVector << 100.0 << 0.60 << 0.50 << 0.40 << 0.35 << 0.30 << 0.25 << 0.20 << 0.15 << 0.10 << 0.05;

    // vector containing the percentage of map area having a landslide-susceptible deposit - see Hazus 2.1 page 149 table 4.18
    QVector<double> mapAreaVector;
    mapAreaVector << 0.00 << 0.01 << 0.02 << 0.03 << 0.05 << 0.08 << 0.10 << 0.15 << 0.20 << 0.25 << 0.30;

    // Get landslide susceptibility - Ad Hoc forcing one set of soil conditions, this should be passed by the model requesting this method
    QString soilType = QString("C");
    bool wetSoil = true;
    double slopeAngle = 20.0;

    getLandslideSuceptibility(soilType, wetSoil, slopeAngle);


    // get the earthquake magnitude
    double M = theMagnitudeList[0]->getCurrentValue();
    double numberOfCyles = 0.3419* M * M * M - 5.5214 * M * M + 33.6154 * M - 70.7692;


    // Tell the PGA model what we want (location and type of intensity measure) and compute the PGA
    theIntensityModel->setLocation(theLocation);
    theIntensityModel->setIntensityMeasure(QString("PGA"));
    theIntensityModel->evaluateModel(RGradientType::NONE);
    double thePGA = thePeakGroundAcceleration->getCurrentValue();


    // Expected displacement factor - see Hazus 2.1 page 151, Figure 4.14
    // This is a tri-linear approximation of the graph in Figure 4.14
    double x1 = 0.0;
    double x2 = 0.0;
    double y1 = 0.0;
    double y2 = 0.0;
    double m = 0.0;

    // ratio between the critical acceleration to cause landslides and the PGA at the site
    double theAccelerationRatio = criticalAccelerationVector[theLandslideSusceptibility] / thePGA;

    // define the displacement factor to be calculated
    double theDisplacementFactor = 0.0;

    if (theAccelerationRatio < 0.1){
        theDisplacementFactor = 30.0;
    }

    else if(theAccelerationRatio >= 0.1 && theAccelerationRatio < 0.65){
        x1 = 0.1;
        x2 = 0.65;
        y1 = 30.0;
        y2 = 0.7;

        // the displacement factor is calculated for a line equation
        m = (y2 - y1)/(x2 - x1);
        theDisplacementFactor = y1 + m * (theAccelerationRatio - x1);
    }

    else if(theAccelerationRatio >= 0.65 && theAccelerationRatio < 0.81){
        x1 = 0.65;
        x2 = 0.81;
        y1 = 0.7;
        y2 = 0.15;

        // the displacement factor is calculated for a line equation
        m = (y2 - y1)/(x2 - x1);
        theDisplacementFactor = y1 + m * (theAccelerationRatio - x1);
    }

    else if(theAccelerationRatio >= 0.81 && theAccelerationRatio < 0.9){
        x1 = 0.81;
        x2 = 0.90;
        y1 = 0.15;
        y2 = 0.02;

        // the displacement factor is calculated for a line equation
        m = (y2 - y1)/(x2 - x1);
        theDisplacementFactor = y1 + m * (theAccelerationRatio - x1);
    }

    else if(theAccelerationRatio > 0.9){
        theDisplacementFactor = 0.02;
    }


    // calculate the expected PGD due to landslides
    // the first three terms calculate how much displacement will happen if a landslide occurs
    // the mapArea term defines the probability that the area will contain landslide-susceptible deposits
    double expectedPGD = theDisplacementFactor * thePGA * numberOfCyles * mapAreaVector[theLandslideSusceptibility];

    return expectedPGD;
    */
    return 0.0;
}


