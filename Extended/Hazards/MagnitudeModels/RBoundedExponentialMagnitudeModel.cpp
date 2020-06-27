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

#include "RBoundedExponentialMagnitudeModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RProbabilityDistributions.h"
#include "RContinuousRandomVariable.h"
#include "RNormalDistribution.h"

#include <math.h>

RBoundedExponentialMagnitudeModel::RBoundedExponentialMagnitudeModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theMinimumMagnitude = nullptr;
    theMaximumMagnitude = nullptr;
    theBeta = nullptr;
    theTheta = nullptr;
    theOccurrence = nullptr;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theMagnitude = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theMagnitude->setModel(this);

    // Create the standard normal random variable
    theDomain->createObject(objectName() + "_theta", "RContinuousRandomVariable");
    theTheta = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta->setDistributionType(QString("Normal (mean, stdv)"));
    theTheta->setMean("0.0");
    theTheta->setStandardDeviation("1.0");
    theAutoParametersList << theTheta;
}


RBoundedExponentialMagnitudeModel::~RBoundedExponentialMagnitudeModel()
{

}


QObject * RBoundedExponentialMagnitudeModel::getMinimumMagnitude() const
{
    return theMinimumMagnitude;
}


void RBoundedExponentialMagnitudeModel::setMinimumMagnitude(QObject *value)
{
    theMinimumMagnitude = qobject_cast<RParameter *>(value);
}


QObject * RBoundedExponentialMagnitudeModel::getMaximumMagnitude() const
{
    return theMaximumMagnitude;
}


void RBoundedExponentialMagnitudeModel::setMaximumMagnitude(QObject *value)
{
    theMaximumMagnitude = qobject_cast<RParameter *>(value);
}


QObject * RBoundedExponentialMagnitudeModel::getBeta() const
{
    return theBeta;
}


void RBoundedExponentialMagnitudeModel::setBeta(QObject *value)
{
    theBeta = qobject_cast<RParameter *>(value);
}


QObject * RBoundedExponentialMagnitudeModel::getOccurrence() const
{
    return theOccurrence;
}


void RBoundedExponentialMagnitudeModel::setOccurrence(QObject *value)
{
    theOccurrence = qobject_cast<RParameter *>(value);
    theAllParametersList<<theOccurrence;
}


QList<RParameter *> RBoundedExponentialMagnitudeModel::getPhysicalParameterList()
{
    QList<RParameter *> paramList;
    paramList << theTheta;
    return paramList;
}


int RBoundedExponentialMagnitudeModel::evaluateModel(RGradientType theGradientType)
{
    if (theOccurrence->getCurrentValue() == 0.0) {

        theMagnitude->setCurrentValue(0.0);

        return 0;
    }

    // Create an auxiliary normal distribution
    RNormalDistribution theNormalDistribution(theDomain);

    double Mmin = theMinimumMagnitude->getCurrentValue();
    double Mmax = theMaximumMagnitude->getCurrentValue();
    double beta = theBeta->getCurrentValue();
    double theta = theTheta->getCurrentValue();
    double cdf = theNormalDistribution.getNormalCDF(theta, 1.0);
    double AA = exp(-beta * (Mmax - Mmin));
    double BB = 1.0 - (1.0 - AA) * cdf;
    double magnitude = -1.0 / beta * log(BB) + Mmin;


    // Updating the response current value
    theMagnitude->setCurrentValue(magnitude);

    if (theGradientType >= RGradientType::DDM) {
        double dMagnitude_dMmin = -cdf * AA / BB + 1.0;
        double dMagnitude_dMmax = cdf * AA / BB ;
        double dMagnitude_dBeta = log(BB) / ((beta)*(beta)) + cdf / beta * (Mmax - Mmin) * AA / BB;

        double dMagnitude_dTheta = theNormalDistribution.getNormalPDF(theta, 1.0) / beta * (1.0 - AA) / BB;

        QMap<RParameter *, double> theMagnitudeGradientMap;
        theMagnitudeGradientMap.insertMulti(theMinimumMagnitude, dMagnitude_dMmin);
        theMagnitudeGradientMap.insertMulti(theMaximumMagnitude, dMagnitude_dMmax);
        theMagnitudeGradientMap.insertMulti(theBeta, dMagnitude_dBeta);
        theMagnitudeGradientMap.insertMulti(theTheta, dMagnitude_dTheta);

        theDDMMap.insertMulti(theMagnitude, theMagnitudeGradientMap);
    }

//    qCritical() << "**********************************************************************";
//    qCritical() << "An earthquake just happened with magnitude" << magnitude << " at time "<<theCurrentTime->getTime()<<" with the name "<<this->objectName();
//    qCritical() << "**********************************************************************";


    return 1;
}
