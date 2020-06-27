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

#include "RGenericDamageAndRestorationModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include <algorithm>
#include <math.h>


RGenericDamageAndRestorationModel::RGenericDamageAndRestorationModel(QObject *parent, QString name)
    : RModel(parent, name)
{

    theIntensity = 0;

    theDomain->createObject(objectName() + "PercentFunctionalResponse", "RGenericResponse");
    thePercentFunctional = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePercentFunctional->setModel(this);

    theDomain->createObject(objectName() + "DamageFactorResponse", "RGenericResponse");
    theDamageFactor = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theDamageFactor->setModel(this);

    theDomain->createObject(objectName() + "_theta", "RContinuousRandomVariable");
    theTheta = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta->setDistributionType(QString("Normal (mean, stdv)"));
    theTheta->setMean("0.0");
    theTheta->setStandardDeviation("1.0");
    theAutoParametersList << theTheta;

}

RGenericDamageAndRestorationModel::~RGenericDamageAndRestorationModel()
{

}


QObject * RGenericDamageAndRestorationModel::getIntensity() const
{	
    return theIntensity;
}	

void RGenericDamageAndRestorationModel::setIntensity(QObject *value)
{	
    theIntensity = qobject_cast<RParameter *>(value);
}

QObject * RGenericDamageAndRestorationModel::getTime() const	
{	
    return theTime;
}	

void RGenericDamageAndRestorationModel::setTime(QObject *value)	
{	
    theTime = qobject_cast<RParameter *>(value);
}	


int RGenericDamageAndRestorationModel::evaluateModel(RGradientType theGradientType)
{	
    // ************************************************************
    //  INPUT
    // ************************************************************


    // Get time and intensity
    double time = theTime->getCurrentValue();
    double intensity = theIntensity->getCurrentValue();


    // Set the damageMedians and damageDispersions for the lognormal "fragility curves"
    QVector<double> damageMedians;
    QVector<double> damageDispersions;
    damageMedians << 0.4 << 0.5 << 0.7 << 0.9;
    damageDispersions << 0.2 << 0.2 << 0.2 << 0.2;


    // Set the mean and standard deviation of the restoration curves
    QVector<double> restorationMeans;
    QVector<double> theRestorationStdvs;
    restorationMeans << 0.6 << 2.5 << 75.0 << 230.0;
    theRestorationStdvs << 0.6 << 2.7 << 42.0 << 110.0;


    // Set the damage ratios, i.e., the percentage of total loss for each damage state
    QVector<double> damageRatios;
    damageRatios << 0.0 << 3.0 << 8.0 << 25.0 << 100.0;



    // ************************************************************
    //  CALCULATION OF MOMENTS FOR DAMAGE AND RESTORATION
    // ************************************************************


    // Calculate the probability of being in different damage states
    QVector<double> probabilityVector;
    double prob = 1.0;
    RNormalDistribution theNormalDistribution(theDomain);
    for (int i = 0; i < damageMedians.count(); i++) {
        double oldProb = prob;
        prob = theNormalDistribution.getNormalCDF(1.0 / damageDispersions[i] * log(intensity / damageMedians[i]), 1);
        probabilityVector << oldProb - prob;
    }
    probabilityVector << prob - 0.0;



    // Caluclate the mean and standard deviation of the damage factor (ratio)
    double MDF = 0;
    double sigma2DF = 0;
    for (int i = 0; i < damageRatios.count(); i++) {
        MDF += damageRatios[i] * probabilityVector[i];
        sigma2DF += damageRatios[i] * damageRatios[i] * probabilityVector[i];
    }
    double sigmaDF = sqrt(sigma2DF - MDF * MDF);



    // Calculate the percent functional for each damage state at this time after the earthquake
    QVector<double> percentFunctionalVector;
    percentFunctionalVector << 1.0;
    for (int i = 0; i < restorationMeans.count(); i++) {
        percentFunctionalVector << theNormalDistribution.getNormalCDF(time - restorationMeans[i], theRestorationStdvs[i]);
    }


    // Caluclate the mean and standard deviation of the percent functional
    double MPF = 0;
    double sigma2PF = 0;
    for (int i = 0; i < percentFunctionalVector.count(); i++) {
        MPF += percentFunctionalVector[i] * probabilityVector[i];
        sigma2PF += percentFunctionalVector[i] * percentFunctionalVector[i] * probabilityVector[i];
    }
    double sigmaPF = sqrt(sigma2PF - MPF * MPF);
    if (qAbs(sigma2PF - MPF * MPF) < 1e-8) {
        sigmaPF = 0.0;
    }



    // ************************************************************
    //  NOW USE THOSE MOMENTS TO TRANSFORM THETA INTO DAMAGE AND RESTORATION
    // ************************************************************


    // Now use the quantities computed above only as statistical moments of two random variables, whose realization is the input variable "theta"
    double theta = theTheta->getCurrentValue();
    double thetaProb = theNormalDistribution.getNormalCDF(theta, 1.0);


    // Considering DF = LN(MDF, sigmaDF)
    double lognormalZetaDF = log(MDF)-0.5*log(1+pow((sigmaDF/MDF), 2.0));
    double lognormalSigmaDF = sqrt(log(pow((sigmaDF/MDF), 2.0) + 1));
    double z = theNormalDistribution.getInverseNormalCDF(thetaProb, 1.0);
    double damageFactor = exp(lognormalSigmaDF*z) / exp(lognormalZetaDF);
    //double damageFactor = theProbabilityDistributions->getInverseLognormalCDF(thetaProb, lognormalZetaDF, lognormalSigmaDF);
    damageFactor /= 100.0;


    // Considering PF = LN(MPF, sigmaPF)
    double lognormalZetaPF = log(MPF)-0.5*log(1+pow((sigmaPF/MPF), 2.0));
    double lognormalSigmaPF = sqrt(log(pow((sigmaPF/MPF), 2.0) + 1));
    double percentFunctional = exp(lognormalSigmaPF*z) / exp(lognormalZetaPF);
    //double percentFunctional = theProbabilityDistributions->getInverseLognormalCDF(thetaProb, lognormalZetaPF, lognormalSigmaPF);


    // Give a couple of warnings now in the start
    if (damageFactor < 0) {
        qCritical() << "The damage in" << this->objectName() << "is" << damageFactor << "i.e., less than unity.";
    }
    if (damageFactor > 1.0) {
        qCritical() << "The damage in" << this->objectName() << "is" << damageFactor << "i.e., greater than unity.";
    }

    
    // Adjusting percent functional
    if (percentFunctional < 0) {
        qCritical() << "The functionality of" << this->objectName() << "is" << percentFunctional << "i.e., less than unity.";
    }
    if (percentFunctional > 1.0) {
        qCritical() << "The functionality of" << this->objectName() << "is" << percentFunctional << "i.e., greater than unity.";
    }


    // ************************************************************
    //  SET RESPONSES
    // ************************************************************

    // Updating the response current value
    thePercentFunctional->setCurrentValue(percentFunctional);
    theDamageFactor->setCurrentValue(damageFactor);

    return 0;
}





