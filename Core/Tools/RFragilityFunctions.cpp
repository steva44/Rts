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

#include "RFragilityFunctions.h"
#include "RDomain.h"
#include "RNormalDistribution.h"

#include <QList>
#include <math.h>
#include <assert.h>
#include <stack>
#include <functional>
#include <stdlib.h>

RFragilityFunctions::RFragilityFunctions(RDomain *domain)
    : RMethod(domain, nullptr)
{
    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();
    theDamageRatios.clear();
}


RFragilityFunctions::~RFragilityFunctions()
{

}


int RFragilityFunctions::setPGADamageMediansAndDispersions(QVector<double> medians, QVector<double> dispersions)
{
    thePGADamageMedians = medians;
    thePGADamageDispersions = dispersions;

    if (thePGADamageMedians.size() != 4){
        qCritical() << "Cannot define 5 damage states with only " << thePGADamageMedians.size() << " damage medians.";
    }

    if (thePGADamageDispersions.size() != 4){
        qCritical() << "Cannot define 5 damage states with only " << thePGADamageDispersions.size() << " damage dispersions.";
    }

    for (int i = 0; i < 4; i++){
        if (thePGADamageMedians.at(0) > thePGADamageMedians.at(i)){
            qCritical() << "The damage medians are not in ascending order.";
        }
    }

    return 0;
}


int RFragilityFunctions::setPGDDamageMediansAndDispersions(QVector<double> medians, QVector<double> dispersions)
{
    thePGDDamageMedians = medians;
    thePGDDamageDispersions = dispersions;

    if (thePGDDamageMedians.size() != 4){
        qCritical() << "Cannot define 5 damage states with only " << thePGDDamageMedians.size() << " damage medians.";
    }

    if (thePGDDamageDispersions.size() != 4){
        qCritical() << "Cannot define 5 damage states with only " << thePGDDamageDispersions.size() << " damage dispersions.";
    }

    for (int i = 0; i < 4; i++){
        if (thePGDDamageMedians.at(0) > thePGDDamageMedians.at(i)){
            qCritical() << "The damage medians are not in ascending order.";
        }
    }

    return 0;
}


int RFragilityFunctions::setRestorationMediansAndStandardDeviations(QVector<double> means, QVector<double> standardDeviations)
{
    theRepairTimeMedians = means;
    theRepairTimeStandardDeviations = standardDeviations;


    if (theRepairTimeMedians.size() != 4){
        qCritical() << "Cannot define 5 damage states with only " << theRepairTimeMedians.size() << " restoration medians.";
    }

    if (theRepairTimeStandardDeviations.size() != 4){
        qCritical() << "Cannot define 5 damage states with only " << theRepairTimeStandardDeviations.size() << " restoration dispersions.";
    }

    for (int i = 0; i < 4; i++){
        if (theRepairTimeMedians.at(0) > theRepairTimeMedians.at(i)){
            qCritical() << "The damage restoration medians are not in ascending order.";
        }
    }

    return 0;
}


QVector<double> RFragilityFunctions::getDamageProbabilities(double PGAintensity, double PGDIntensity)
{
    // Declare the vector to be returned
    QVector<double> theProbabilities;


    // Check if there is anything to do
    if (thePGADamageMedians.size() == 0 && thePGDDamageMedians.size() == 0 || PGAintensity <= 0.0) {
        return theProbabilities;
    }

    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);


    // Calculate the value of each fragility function: P(D >= d_i)
    QVector<double> thePGAFragilityFunctionValues;
    for (int i = 0; i < thePGADamageMedians.count(); i++) {

        thePGAFragilityFunctionValues << theNormalDistribution.getNormalCDF(1.0 / thePGADamageDispersions[i] * log(PGAintensity / thePGADamageMedians[i]), 1);
    }


    QVector<double> thePGDFragilityFunctionValues;
    for (int i = 0; i < thePGDDamageMedians.count(); i++) {

        if (PGDIntensity != 0.0) {

            thePGDFragilityFunctionValues << theNormalDistribution.getNormalCDF(1.0 / thePGDDamageDispersions[i] * log(PGDIntensity / thePGDDamageMedians[i]), 1);
        }
        else {

            thePGDFragilityFunctionValues << 0.0;
        }
    }


    QVector<double> theCombinedFragilityFunctionValues;
    for (int i = 0; i < thePGADamageMedians.size(); i++) {

        theCombinedFragilityFunctionValues <<  thePGAFragilityFunctionValues[i] + thePGDFragilityFunctionValues[i] - thePGAFragilityFunctionValues[i] * thePGDFragilityFunctionValues[i];
    }

    // Calculate the damage probabilities
    double oldProb = 1.0;

    for (int i=0; i<theCombinedFragilityFunctionValues.size(); i++) {

        theProbabilities << oldProb - theCombinedFragilityFunctionValues.at(i);

        oldProb = theCombinedFragilityFunctionValues.at(i);
    }

    // The last damage state gets oldprob - 0.0
    theProbabilities << oldProb;

    return theProbabilities;
}


QVector<double> RFragilityFunctions::getRestorationPercentages(double timeOfEvent)
{
    // Declare the vector to be returned
    QVector<double> percentFunctionalVector;


    // Get time since the event occurred
    double timeSinceEvent = theCurrentTime->getCurrentValue()-timeOfEvent;


    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);


    // Calculate retoration percentages for each damage state
    percentFunctionalVector << 1.0;
    for (int i = 0; i < theRepairTimeMedians.count(); i++) {

        percentFunctionalVector << 1.0 * (theNormalDistribution.getNormalCDF(timeSinceEvent - theRepairTimeMedians[i], theRepairTimeStandardDeviations[i]));
    }

    return percentFunctionalVector;
}


QVector<double> RFragilityFunctions::getRestorationPercentagesGivenDelta(double delta)
{
    // Declare the vector to be returned
    QVector<double> percentFunctionalVector;

    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);


    // Calculate retoration percentages for each damage state
    percentFunctionalVector << 1.0;
    for (int i = 0; i < theRepairTimeMedians.count(); i++) {

        percentFunctionalVector << 1.0 * (theNormalDistribution.getNormalCDF(delta - theRepairTimeMedians[i], theRepairTimeStandardDeviations[i]));
    }

    return percentFunctionalVector;
}


double RFragilityFunctions::calculateMean(QVector<double> probabilities, QVector<double> states)
{
    // Check that the vectors are consistent

    double mean = 0.0;

    for (int i=0; i<probabilities.size(); i++) {

        mean += states.at(i) * probabilities.at(i);
    }

    return mean;
}


double RFragilityFunctions::getTimeUntilGivenRestorationPercentage(double desiredRestorationPercentage, QVector<double> damageProbabilities, double timeOfEvent)
{
    // Quick return if we haven't reached the time of the event
    if (timeOfEvent > theCurrentTime->getCurrentValue() || damageProbabilities.size() < 1) {
        return 0.0;
    }


    // Transform the given percentage into a ratio
    double ratio = desiredRestorationPercentage; // / 100.0;


    // Determine the time lapsed since the event
    double timeSinceEvent = theCurrentTime->getCurrentValue() - timeOfEvent;


    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);


    // Calculate the time from event until given ratio, for each damage state
    QVector<double> timeVector;
    double result;
    for (int i = 0; i < theRepairTimeMedians.count(); i++) {

        // Calculate the time until the desired restoration ratio
        result = theRepairTimeMedians[i] + theNormalDistribution.getInverseNormalCDF(ratio, theRepairTimeStandardDeviations[i]) - timeSinceEvent;

        // Check if that restoration ratio was already reached, in which case the time until the desired restoration ratio is zero
        if (result > 0.0) {
            timeVector << result;
        }
        else {
            timeVector << 0.0;
        }
    }

    // Calculate the mean
    double meanTime = 0.0;
    for (int i = 0; i < timeVector.count(); i++) {

        meanTime += timeVector[i] * damageProbabilities[i];
    }


    return meanTime;
}


