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
*   - Stevan Gavrilovic                                              *
*   - Rodrigo Costa                                                  *
*                                                                    *
*********************************************************************/

#include "RStevesHumanImpactModel.h"
#include "RBIM.h"
#include "RNormalDistribution.h"
#include "RStructuralAnalysisModel.h"
#include "RStatistics.h"

#include "RResponse.h"

RStevesHumanImpactModel::RStevesHumanImpactModel(QObject *parent, QString name)
    : RModel(parent, name)
{

    theInputModelResponse = nullptr;
    theBIM = nullptr;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    // Do not discount human deaths and injuries
    theResponse->setDiscount(false);

    theStatsMethod = theDomain->getStatsMethod();

    runningSum = 0.0;
}


RStevesHumanImpactModel::~RStevesHumanImpactModel()
{

}


int RStevesHumanImpactModel::resetTime()
{

    theStatsMethod->update(theResponse->objectName()+"Undiscounted",runningSum);

    runningSum = 0.0;

    return 0;
}


QObject * RStevesHumanImpactModel::getBIM() const
{
    return theBIM;
}


void RStevesHumanImpactModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


QObject* RStevesHumanImpactModel::getInputParameter() const
{
    return theInputModelResponse;
}


void RStevesHumanImpactModel::setInputParameter(QObject *value)
{
    theInputModelResponse = qobject_cast<RParameter*>(value);

    auto theSAModel = qobject_cast<RStructuralAnalysisModel*>(value);

    if(theInputModelResponse == nullptr || theSAModel)
    {
        qCritical()<< this->objectName()<<" needs a response from a  structural analysis model";
        return;
    }

    this->theAllParametersList.push_back(theInputModelResponse);
}


int RStevesHumanImpactModel::evaluateModel(RGradientType theGradientType)
{

    theResponse->setCurrentValue(0.0);

    // Check that there is a BIM
    if(theBIM == nullptr)
    {
        qCritical() << this->objectName()<<" needs a BIM";
        return 0;
    }

    // Quick return if the input parameter is zero
    if(theInputModelResponse->getCurrentValue() == 0.0)
        return 0;


    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"*********************************";
        qDebug()<<"*******HUMAN IMPACT MODEL********";
        qDebug()<<"*********************************";
    }


    // Get the drift from the BIM
    auto drift = theBIM->getMaxInterstoreyDrift();

    // If the drift is zero, there might not be a structural analysis, get the drift from the input parameter
    if(drift == 0.0)
    {
        drift = theInputModelResponse->getCurrentValue();
    }


    // Cutoff drift at 0.25% - assume no casualties under this drift level
    if(drift <= 0.0025)
        return 0;

    if (theOutputLevel >= RObject::Maximum)
    {
        qDebug()<<"Building drift: "<<drift;
    }

    // Get the HAZUS building type from the BIM
    auto hazusType = theBIM->getBuildingType();

    // The time of day needs to be a fraction of 24 hours
    auto timeOfDay = theCurrentTime->getHoursDay();

    // Assume medium economic welfare
    QString economicWelfare = "Medium";

    QVector<double> theAgeDistributionXVector ={0.10,0.20,0.20,0.20,0.20,0.10};
    QVector<double> theAgeDistributionYVector ={0.10,0.20,0.20,0.20,0.20,0.10};

    // Assume 50/50 split
    double thePercentageOfMales = 0.50;

    // Get the medians and dispersion for fragility function
    QVector<double> medians(4);
    QVector<double> dispersions(4);
    this->getFragilityFunctionMediansAndDispersions(hazusType,medians,dispersions);

    // Now calculate the damage probabilities
    auto damageStateProbabilities = this->getDamageProbabilities(medians,dispersions,drift);

    // Get the injuries and deaths
    auto morbidityVector = this->calculateMorbidity(hazusType,timeOfDay,economicWelfare,damageStateProbabilities,theAgeDistributionXVector,theAgeDistributionYVector,thePercentageOfMales);

    // The morbidity vector returns the number of affected people in each severity level. According to Table 13.1: Injury Classification Scale in Hazus MH2.1
    // Severity 1
    // Injuries requiring basic medical aid that could be administered by paraprofessionals. These types of injuries would require bandages or observation.
    // Some examples are: a sprain, a severe cut requiring stitches, a minor burn (first degree or second degree on a small part of the body), or a bump on the head without loss of consciousness.
    // Injuries of lesser severity that could be self treated are not estimated by Hazus.
    // Severity 2
    // Injuries requiring a greater degree of medical care and use of medical technology such as x-rays or surgery, but not expected to progress to a life threatening status.
    // Some examples are third degree burns or second degree burns over large parts of the body, a bump on the head that causes loss of consciousness, fractured bone, dehydration or exposure.
    // Severity 3
    // Injuries that pose an immediate life threatening condition if not treated adequately and expeditiously.
    // Some examples are: uncontrolled bleeding, punctured organ, other internal injuries, spinal column injuries, or crush syndrome.
    // Severity 4
    // Instantaneously killed or mortally injured

    // Cost of death	8,100,000 $/death	(Miller, 2000)
    // Cost of injuries (fall, cut, struck, burn)	10,015	$/injury (Finkelstein et al., 2006)
    // Assume that in severity level 3, 75% do not make it, and 25% survive with injuries
    auto numHousingUnits = theBIM->getNumberOfHousingUnits();

    // Statistics canada, 2016
    auto numPeoplePerHousehold = 2.5;

    // Calculate the total occupants
    auto totalOccupants = numHousingUnits*numPeoplePerHousehold;

    auto numInjured1 = totalOccupants*morbidityVector[0];
    auto numInjured2 = totalOccupants*morbidityVector[1];
    auto numInjured3 = totalOccupants*morbidityVector[2]*0.25;
    auto numInjured = numInjured1+numInjured2+numInjured3;
    auto numDeaths = totalOccupants*(morbidityVector[2]*0.75 + morbidityVector[3]);

    auto totalCost = numInjured*10015.0 + numDeaths*8100000.0;

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"Injuries: "<<numInjured<<", Deaths: "<<numDeaths;
        qDebug()<<"Cost of Injuries: "<<numInjured*10015.0<<", Cost of Deaths: "<<numDeaths*8100000.0<<", total cost: "<<totalCost;

    }

    theResponse->setCurrentValue(totalCost);

    theStatsMethod->update(theResponse->objectName(),totalCost);
    theStatsMethod->update(theResponse->objectName()+"numInjured",numInjured);
    theStatsMethod->update(theResponse->objectName()+"numDeaths",numDeaths);

    runningSum += totalCost;

    return 0;
}


int RStevesHumanImpactModel::getFragilityFunctionMediansAndDispersions(const QString& HazusType, QVector<double>& medians, QVector<double>& dispersions)
{

    // Values taken from Table 5.9a Structural Fragility Curve Parameters ‐ High‐Code Seismic Design Level in Hazus MH2.1 with the medians of spectral displacement replaced with
    // with the values of drift given in the same table. The dispersion or beta parameters are kept the same.
    if(HazusType == "C2M")
    {
        medians[0] = 0.0027;
        medians[1] = 0.0067;
        medians[2] = 0.0200;
        medians[3] = 0.0533;

        dispersions[0] = 0.74;
        dispersions[1] = 0.77;
        dispersions[2] = 0.68;
        dispersions[3] = 0.77;
    }
    else if(HazusType == "W2")
    {
        medians[0] = 0.0040;
        medians[1] = 0.0120;
        medians[2] = 0.0400;
        medians[3] = 0.1000;

        dispersions[0] = 0.81;
        dispersions[1] = 0.88;
        dispersions[2] = 0.90;
        dispersions[3] = 0.83;
    }
    else if(HazusType == "S4M")
    {
        medians[0] = 0.0027;
        medians[1] = 0.0053;
        medians[2] = 0.0160;
        medians[3] = 0.0467;

        dispersions[0] = 0.77;
        dispersions[1] = 0.72;
        dispersions[2] = 0.70;
        dispersions[3] = 0.89;
    }
    else {
        qDebug()<<"Building type not recognized in "<<__FUNCTION__;
        return -1;
    }

    return 0;
}


QVector<double> RStevesHumanImpactModel::getDamageProbabilities(const QVector<double>& medians,const QVector<double>& dispersions, const double& drift)
{
    // Declare the vector to be returned
    QVector<double> theProbabilities(4);

    auto numDamageCurves = 4;

    if (medians.size() != numDamageCurves)
    {
        qCritical() << "Cannot define 5 damage states with only " << medians.size() << " damage medians.";
        return theProbabilities;
    }

    if (dispersions.size() != numDamageCurves)
    {
        qCritical() << "Cannot define 5 damage states with only " << dispersions.size() << " damage dispersions.";
        return theProbabilities;
    }

    for (int i = 0; i < numDamageCurves; ++i)
    {
        if (medians.at(0) > medians.at(i)){
            qCritical() << "The damage medians are not in ascending order.";
            return theProbabilities;
        }
    }

    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);

    // Calculate the value of each fragility function: P(D >= d_i)
    QVector<double> thePGAFragilityFunctionValues(4);
    for (int i = 0; i < numDamageCurves; ++i)
    {
        thePGAFragilityFunctionValues[i] = theNormalDistribution.getNormalCDF(1.0 / dispersions[i] * log(drift / medians[i]), 1.0);
    }

    // Calculate the damage probabilities
    double oldProb = 1.0;

    for (int i=0; i<numDamageCurves; ++i)
    {
        auto val = oldProb - thePGAFragilityFunctionValues.at(i);

        theProbabilities[i] = val;

        oldProb = thePGAFragilityFunctionValues.at(i);
    }

    // The last damage state gets oldprob - 0.0
    theProbabilities << oldProb;

    return theProbabilities;
}


QVector<double> RStevesHumanImpactModel::calculateMorbidity(QString buildingType, double timeOfDay, QString economicWelfare, QVector<double> damageStateProbabilities, QVector<double> theAgeDistributionXVector, QVector<double> theAgeDistributionYVector, double thePercentageOfMales)
{
    double probabilityOfSeverity1 = 0.0;
    double probabilityOfSeverity2 = 0.0;
    double probabilityOfSeverity3 = 0.0;
    double probabilityOfSeverity4 = 0.0;

    if (theAgeDistributionXVector.size() != 6 || theAgeDistributionYVector.size() != 6){
        qCritical() << "Check the size of the age distribution vectors passed to morbidity calcualtor.";
    }

    // HAZUS 2.1 - TABLE 13.3 THROUGH 13.6 - INDOOR CASUALTY RATES
    QVector<double> severity1RatesIndoor;
    QVector<double> severity2RatesIndoor;
    QVector<double> severity3RatesIndoor;
    QVector<double> severity4RatesIndoor;

    severity1RatesIndoor << 0.00;
    severity2RatesIndoor << 0.00;
    severity3RatesIndoor << 0.00;
    severity4RatesIndoor << 0.00;

    if(buildingType == QString("W1")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.10;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
    }
    else if(buildingType == QString("W2")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.31;
    }
    else if(buildingType == QString("S1L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 7.80;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.52;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.41;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.81;
    }
    else if(buildingType == QString("S1M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }
    else if(buildingType == QString("S1H")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.31;
    }
    else if(buildingType == QString("S2L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 7.80;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.52;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.41;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.81;
    }
    else if(buildingType == QString("S2M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }
    else if(buildingType == QString("S2H")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.31;
    }
    else if(buildingType == QString("S3")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.10;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
    }
    else if(buildingType == QString("S4L")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 7.80;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.52;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.41;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.81;
    }
    else if(buildingType == QString("S4M")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }

    else if(buildingType == QString("S4H")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.31;
    }
    else if(buildingType == QString("S5L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 7.80;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.52;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.41;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.81;
    }
    else if(buildingType == QString("S5M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }
    else if(buildingType == QString("S5H")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.31;
    }
    else if(buildingType == QString("C1L")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }
    else if(buildingType == QString("C1M")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }
    else if(buildingType == QString("C1H")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }
    else if(buildingType == QString("C2L")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }
    else if(buildingType == QString("C2M")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }
    else if(buildingType == QString("C2H")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }
    else if(buildingType == QString("C3L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 10.25;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.85;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.76;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.51;
    }
    else if(buildingType == QString("C3M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }
    else if(buildingType == QString("C3H")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }
    else if(buildingType == QString("PC1")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 10.25;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.85;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.76;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.51;
    }
    else if(buildingType == QString("PC2L")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 10.25;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.85;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.76;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.51;
    }
    else if(buildingType == QString("PC2M")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }
    else if(buildingType == QString("PC2H")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }
    else if(buildingType == QString("RM1L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }
    else if(buildingType == QString("RM1M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }
    else if(buildingType == QString("RM2L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }
    else if(buildingType == QString("RM2M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }
    else if(buildingType == QString("RM2H")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }
    else if(buildingType == QString("URML")){
        severity1RatesIndoor << 0.05 << 0.35 << 2.00 << 14.50;
        severity2RatesIndoor << 0.00 << 0.40 << 0.20 << 4.70;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.77;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.52;
    }
    else if(buildingType == QString("URMM")){
        severity1RatesIndoor << 0.05 << 0.35 << 2.00 << 14.50;
        severity2RatesIndoor << 0.00 << 0.40 << 0.20 << 4.70;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.77;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.52;
    }
    else if(buildingType == QString("MH")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.10;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
    }
    else {
        qCritical() << "The morbidity calculator has a problem with the property Building Type.";
    }

    // Time of day modification factor
    double occupancyOverTheDay = 0.0;

    // 0 < t < 0.25 is between 12:00am and 6:00am - night time
    if (timeOfDay < 0.25){
        occupancyOverTheDay = 0.999 * 0.99;
    }
    // 0.25 < t < 0.375 is between 6:00am and 9:00am - rush time in the morning
    else if(timeOfDay >= 0.25 && timeOfDay < 0.375) {
        occupancyOverTheDay = 0.70 * 0.50;
    }
    // 0.375 < t < 0.666 is between 9:00am and 4:00pm - calmer part of the day, everyone is at work
    else if(timeOfDay >= 0.375 && timeOfDay < 0.666){
        occupancyOverTheDay = 0.70 * 0.075;
    }
    // 0.666 < t 0.8333 is between 4:00pm and 8:00pm - rush hour, people going home
    else if(timeOfDay >= 0.666 && timeOfDay < 0.8333){
        occupancyOverTheDay = 0.70 * 0.50;
    }
    // t > 0.8333 is past 8pm, calmer times again
    else if (timeOfDay >= 0.833){
        occupancyOverTheDay = 0.999 * 0.99;
    }

    // ***************************************************************
    // * Socieconomic demographics
    // ***************************************************************

    // 1. AGE
    double ageInjuryFactor = 0.00;
    double ageFatalityFactor = 0.00;

    QVector<double> injurySEDFactorsForAge;
    QVector<double> fatalitySEDFactorsForAge;

    injurySEDFactorsForAge << 0.8838 << 1.5100 << 1.0162 << 0.6725 << 1.6712 << 2.7550;
    fatalitySEDFactorsForAge << 0.8838 << 1.5100 << 1.0162 << 0.6725 << 1.6712 << 2.7550;

    for (int i = 0; i < theAgeDistributionXVector.size(); i++){
        ageInjuryFactor += theAgeDistributionYVector.at(i) * injurySEDFactorsForAge.at(i);
        ageFatalityFactor += theAgeDistributionYVector.at(i) * fatalitySEDFactorsForAge.at(i);
    }

    // 2. GENDER
    double genderInjuryFactor   = 1.0 * thePercentageOfMales + 1.7925 * (1.0 - thePercentageOfMales);
    double genderFatalityFactor = 1.0 * thePercentageOfMales + 2.0033 * (1.0 - thePercentageOfMales);


    // 3. INCOME
    double incomeInjuryFactor;
    double incomeFatalityFactor;

    if (economicWelfare == QString("Low")){
        incomeInjuryFactor = 3.485;
        incomeFatalityFactor = 3.485;
    }
    else if (economicWelfare == QString("Medium")){
        incomeInjuryFactor = 1.850;
        incomeFatalityFactor = 1.850;
    }
    else {
        incomeInjuryFactor = 1.000;
        incomeFatalityFactor = 1.000;
    }

    // 4. BUILT ENVIRONMENT
    double builtEnvironmentInjuryFactor = 1.00;
    double builtEnvironmentFatalityFactor = 1.00;

    // Ignore age, gender, income, and built env. factors for now
    // double injuryFactor   = ageInjuryFactor   * genderInjuryFactor   * incomeInjuryFactor   * builtEnvironmentInjuryFactor;
    // double fatalityFactor = ageFatalityFactor * genderFatalityFactor * incomeFatalityFactor * builtEnvironmentFatalityFactor;

    double injuryFactor   = 1.0;
    double fatalityFactor = 1.0;

    for (int i = 0; i < damageStateProbabilities.size(); i++){
        probabilityOfSeverity1 += occupancyOverTheDay * damageStateProbabilities.at(i) * severity1RatesIndoor.at(i) / 100.0;
        probabilityOfSeverity2 += occupancyOverTheDay * damageStateProbabilities.at(i) * severity2RatesIndoor.at(i) / 100.0;
        probabilityOfSeverity3 += occupancyOverTheDay * damageStateProbabilities.at(i) * severity3RatesIndoor.at(i) / 100.0;
        probabilityOfSeverity4 += occupancyOverTheDay * damageStateProbabilities.at(i) * severity4RatesIndoor.at(i) / 100.0;
    }

    QVector<double> returnVector;
    returnVector << probabilityOfSeverity1 * injuryFactor << probabilityOfSeverity2 * injuryFactor << probabilityOfSeverity3 * injuryFactor << probabilityOfSeverity4 * fatalityFactor;

    return returnVector;
}


