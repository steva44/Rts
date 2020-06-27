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
*   - Rodrigo Costa                                                  *
*                                                                    *
*********************************************************************/

#include "RSimpleBuildingLossModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include "RStatistics.h"
#include "RBIM.h"

#include <math.h>

RSimpleBuildingLossModel::RSimpleBuildingLossModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = nullptr;
    theStructuralDamageRatioResponse = nullptr;
    theNDDamageRatioResponse = nullptr;
    theNADamageRatioResponse = nullptr;
    theContentDamageRatiosResponse = nullptr;
    thePeakDrift = nullptr;

    theBuildingValue = 0.0;

    // Initializing the pointer data members
    theDomain->createObject(objectName() + "BuildingLossResponse", "RGenericResponse");
    theBuildingLossResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theBuildingLossResponse->setModel(this);
    theBuildingLossResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "ContentLossResponse", "RGenericResponse");
    theContentLossResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theContentLossResponse->setModel(this);
    theContentLossResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "DisruptionCostResponse", "RGenericResponse");
    theDisruptionCostResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theDisruptionCostResponse->setModel(this);
    theDisruptionCostResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theTotalLossResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theTotalLossResponse->setModel(this);
    theTotalLossResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "RepairTime", "RGenericResponse");
    theRepairTimeResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theRepairTimeResponse->setModel(this);
    theRepairTimeResponse->setCurrentValue(0.0);

    theStatsMethod = theDomain->getStatsMethod();

}


RSimpleBuildingLossModel::~RSimpleBuildingLossModel()
{

}


int RSimpleBuildingLossModel::resetTime()
{
    return 0;
}


double RSimpleBuildingLossModel::getBuildingValue() const
{
    return theBuildingValue;
}


void RSimpleBuildingLossModel::setBuildingValue(double value)
{
    theBuildingValue = value;
}


QObject * RSimpleBuildingLossModel::getPeakDrift() const
{
    return thePeakDrift;
}


void RSimpleBuildingLossModel::setPeakDrift(QObject *value)
{
    thePeakDrift = qobject_cast<RResponse*>(value);
}


QObject* RSimpleBuildingLossModel::getStructuralDamage() const
{
    return theStructuralDamageRatioResponse;
}


void RSimpleBuildingLossModel::setStructuralDamage(QObject* value)
{
    theStructuralDamageRatioResponse = qobject_cast<RResponse *>(value);
}


QObject* RSimpleBuildingLossModel::getNSDriftDamage() const
{
    return theNDDamageRatioResponse;
}


void RSimpleBuildingLossModel::setNSDriftDamage(QObject* value)
{
    theNDDamageRatioResponse = qobject_cast<RResponse *>(value);
}


QObject * RSimpleBuildingLossModel::getBIM() const
{
    return theBIM;
}


void RSimpleBuildingLossModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);

    if(!theBIM)
    {
        qCritical()<<"Could not set the BIM in "<<__FUNCTION__;
    }

}


QObject* RSimpleBuildingLossModel::getNSAccelerationDamage() const
{
    return theNADamageRatioResponse;
}


void RSimpleBuildingLossModel::setNSAccelerationDamage(QObject* value)
{
    theNADamageRatioResponse = qobject_cast<RResponse *>(value);
}


QObject* RSimpleBuildingLossModel::getContentDamageRatios() const
{
    return theContentDamageRatiosResponse;
}


void RSimpleBuildingLossModel::setContentDamageRatios(QObject* value)
{
    theContentDamageRatiosResponse = qobject_cast<RResponse *>(value);
}


int RSimpleBuildingLossModel::evaluateModel(RGradientType theGradientType)
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    // Reset responses
    theBuildingLossResponse->setCurrentValue(0.0);
    theContentLossResponse->setCurrentValue(0.0);
    theDisruptionCostResponse->setCurrentValue(0.0);
    theTotalLossResponse->setCurrentValue(0.0);
    theRepairTimeResponse->setCurrentValue(0.0);

    if(theBIM == nullptr)
    {
        qCritical()<<"Error, this model "<<this->objectName()<<" needs a BIM";
        return -1;
    }

    // End this quickly if there is no damage
    if (theStructuralDamageRatioResponse->getCurrentValue() < 0.001){
        return 0;
    }

    theSRepairCost.clear();
    theNSARepairCost.clear();
    theNSDRepairCost.clear();
    theContentReplacementCost.clear();
    theRecoveryTime.clear();

    // Get the information from the BIM
    auto theBuildingType = theBIM->getBuildingType();
    auto theOccupancyClass = theBIM->getBuildingOccupancyClass();

    if (theOccupancyClass == QString("RES1")) {
        theSRepairCost << 0.0 << 0.5 << 2.3 << 11.7 << 23.4;
        theNSARepairCost << 0.0 << 0.5 << 2.7 << 8.0 << 26.6;
        theNSDRepairCost << 0.0 << 1.0 << 5.5 << 25.0 << 50.0;
        theContentReplacementCost << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;
        theRecoveryTime << 0.0 << 5.0 << 120.0 << 360.0 << 720.0;  // Table 5.10 Hazus MH2.1 TM - given in days

    } else if (theOccupancyClass == QString("RES2")) {
        theSRepairCost << 0.0 << 0.4 << 2.4 << 7.3 << 24.4;
        theNSARepairCost << 0.0 << 0.8 << 3.8 << 11.3 << 37.8;
        theNSDRepairCost << 0.0 << 0.8 << 3.8 << 18.9 << 37.8;
        theContentReplacementCost << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;
        theRecoveryTime << 0.0 << 5.0 << 20.0 << 120.0 << 240.0; // Table 5.10 Hazus MH2.1 TM - given in days

    } else if (theOccupancyClass == QString("RES3")) {
        theSRepairCost << 0.0 << 0.3 << 1.4 << 6.9 << 13.8;
        theNSARepairCost << 0.0 << 0.8 << 4.3 << 13.1 << 43.7;
        theNSDRepairCost << 0.0 << 0.9 << 4.3 << 21.3 << 42.5;
        theContentReplacementCost << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;
        theRecoveryTime << 0.0 << 10.0 << 120.0 << 480.0 << 960.0; // Table 5.10 Hazus MH2.1 TM - given in days

    } else if (theOccupancyClass == QString("RES4")) {
        theSRepairCost << 0.0 << 0.2 << 1.4 << 6.8 << 13.6;
        theNSARepairCost << 0.0 << 0.9 << 4.3 << 13 << 43.2;
        theNSDRepairCost << 0.0 << 0.9 << 4.3 << 21.6 << 43.2;
        theContentReplacementCost << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;
        theRecoveryTime << 0.0 << 10.0 << 90.0 << 360.0 << 480.0; // Table 5.10 Hazus MH2.1 TM - given in days

    } else if (theOccupancyClass == QString("RES5")) {
        theSRepairCost << 0.0  << 0.4 << 1.9 << 9.4 << 18.8;
        theNSARepairCost << 0.0 << 0.8 << 4.1 << 12.4 << 41.2;
        theNSDRepairCost << 0.0 << 0.8 << 4 << 20 << 40;
        theContentReplacementCost << 0.0 << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;
        theRecoveryTime << 0.0 << 10.0 << 90.0 << 360.0 << 480.0; // Table 5.10 Hazus MH2.1 TM - given in days

    } else if (theOccupancyClass == QString("RES6")) {
        theSRepairCost << 0.0 << 0.4 << 1.8 << 9.2 << 18.4;
        theNSARepairCost << 0.0 << 0.8 << 4.1 << 12.2 << 40.8;
        theNSDRepairCost << 0.0 << 0.8 << 4.1 << 20.4 << 40.8;
        theContentReplacementCost << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;
        theRecoveryTime << 0.0 << 10.0 << 120.0 << 480.0 << 960.0; // Table 5.10 Hazus MH2.1 TM - given in days
    }


    double theDamageRatio = 0.0;

    double theStructuralDamageRatio = theStructuralDamageRatioResponse->getCurrentValue() * theSRepairCost.at(4) / 100.0;
    double theDriftDamageRatio = theNDDamageRatioResponse->getCurrentValue() * theNSDRepairCost.at(4) / 100.0;
    double theAccelerationDamageRatio = theNADamageRatioResponse->getCurrentValue() * theNSARepairCost.at(4) / 100.0;
    double theContentDamageRatio = theContentDamageRatiosResponse->getCurrentValue() * theContentReplacementCost.at(4) / 100.0;
    theDamageRatio = theStructuralDamageRatio + theDriftDamageRatio + theAccelerationDamageRatio;

    auto buildingLoss = theDamageRatio * theBuildingValue;
    auto contentLoss = theContentDamageRatio * theBuildingValue;
    auto totalLoss = (theDamageRatio + theContentDamageRatio) * theBuildingValue;

    // Get the damage state probabilities
    QVector<double> medians(4);
    QVector<double> dispersions(4);
    this->getFragilityFunctionMediansAndDispersions(theBuildingType,medians,dispersions);

    auto drift = thePeakDrift->getCurrentValue();

    auto damageStateProbabilities = this->getDamageProbabilities(medians,dispersions,drift);

    // Calculate the repair/recovery time (in days)
    double repairTime = 0.0;
    for(int i = 0; i<5; ++i)
    {
        repairTime += damageStateProbabilities[i]*theRecoveryTime[i];
    }

    theRepairTimeResponse->setCurrentValue(repairTime);
    theBuildingLossResponse->setCurrentValue(buildingLoss);
    theContentLossResponse->setCurrentValue(contentLoss);
    theTotalLossResponse->setCurrentValue(totalLoss);

    if (theOutputLevel == RDomain::Medium) {
        qDebug()<<"***************************************";
        qDebug()<< "Building loss [$]:" << theBuildingLossResponse->getCurrentValue() << "(" << 100.0 * buildingLoss / theBuildingValue << "% )";
        qDebug()<< "Content loss [$]:" << theContentLossResponse->getCurrentValue() << "(" << 100.0 * contentLoss / theBuildingValue << "% )";
        qDebug()<< "Total loss [$]:" << theTotalLossResponse->getCurrentValue() << "(" << 100.0 * totalLoss / theBuildingValue << "% )";
        qDebug()<< "Recovery time [days]:" << theRepairTimeResponse->getCurrentValue();
        qDebug()<< "Current time: "<<theCurrentTime->getTime();
        qDebug()<<"***************************************";
    }

    // Save the stats
    theStatsMethod->update(theRepairTimeResponse->objectName(),repairTime);
    theStatsMethod->update(theBuildingLossResponse->objectName(),buildingLoss);
    theStatsMethod->update(theContentLossResponse->objectName(),contentLoss);
    theStatsMethod->update(theTotalLossResponse->objectName(),totalLoss);

    return 0;
}


int RSimpleBuildingLossModel::getFragilityFunctionMediansAndDispersions(const QString& HazusType, QVector<double>& medians, QVector<double>& dispersions)
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


QVector<double> RSimpleBuildingLossModel::getDamageProbabilities(const QVector<double>& medians,const QVector<double>& dispersions, const double& drift)
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

    // The last damage state gets oldprob
    theProbabilities << oldProb;

    return theProbabilities;
}



