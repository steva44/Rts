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
*                                                                    *
*********************************************************************/

#include "RLoadPatternModel.h"
#include "RLoadPattern.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RTime.h"
#include "RDomain.h"

#include "LoadPattern.h"
#include "GroundMotion.h"
#include "UniformExcitation.h"

#include <math.h>

RLoadPatternModel::RLoadPatternModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    scalingModelResponse = nullptr;
    theOccurrence = nullptr;

    patternApplied = false;

    //Just has a token response, 1.0 if active, 0.0 if not
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    scaleFactor = 1.0;
}


RLoadPatternModel::~RLoadPatternModel()
{

}


void RLoadPatternModel::reset(void)
{
    for(auto lpat : theLoadPatterns)
    {
        auto theLoadPat = lpat->getTheLoadPattern();

        if(theLoadPat == nullptr)
        {
            qDebug()<<"Error in getting the OpenSees load pattern in "<<this->objectName()<<__FUNCTION__;
            continue;
        }

        lpat->setCFactor(1.0);
        lpat->update();
    }
}


QList<RParameter *> RLoadPatternModel::getAllParametersList(bool *ok)
{
    theAllParametersList.push_back(theOccurrence);

    if(scalingModelResponse)
        theAllParametersList.push_back(qobject_cast<RResponse*>(scalingModelResponse));

    return theAllParametersList;
}


QString RLoadPatternModel::getTheLoadPatternsInput()
{
    QString string;
    for (int i = 0; i < theLoadPatterns.count(); i++)
    {
        if (theLoadPatterns[i]) {
            string += theLoadPatterns[i]->objectName();
            string += "; ";
        } else {
            theLoadPatterns.removeAt(i);
            i--;
        }
    }

    return string;
}


QObject * RLoadPatternModel::getScalingModelInput() const
{
    return scalingModelResponse;
}


void RLoadPatternModel::setScalingModelnput(QObject * value)
{
    scalingModelResponse = qobject_cast<RParameter*>(value);
}


void RLoadPatternModel::setTheLoadPatternsInput(QString value)
{
    QRegExp regExp = QRegExp(QString("[ ,;:\t\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator)));
    QStringList stringList = value.split(regExp, QString::SkipEmptyParts);

    for (int i = 0; i < stringList.count(); i++)
    {
        RLoadPattern *object = theDomain->findChild<RLoadPattern *>(stringList[i]);
        if (object)
        {
            theLoadPatterns.append(object);
        } else {
            qCritical() << "Error in" << objectName() << ": Could not find a load pattern named" << stringList[i] << "in the domain";
        }
    }
}


QObject* RLoadPatternModel::getOccurrence() const
{
    return theOccurrence;
}


void RLoadPatternModel::setOccurrence(QObject *value)
{
    theOccurrence = qobject_cast<RParameter *>(value);
}


bool RLoadPatternModel::isOn()
{

    // If the hazard does not occur, return false
    if (theOccurrence->getCurrentValue() == 0.0)
        return false;

    // If the hazard occurs, but its scaling intensity is zero, then return false
    if(scalingModelResponse)
    {
        auto PGAIntensity = scalingModelResponse->getCurrentValue();

        if(fabs(PGAIntensity) < 0.001)
            return false;

    }

    return true;
}


bool& RLoadPatternModel::isApplied()
{
    return patternApplied;
}


void RLoadPatternModel::setAppplied(bool value)
{
    patternApplied = value;
}


int RLoadPatternModel::evaluateModel(RGradientType theGradientType)
{
    // Exit quickly if this hazard isn't on
    if (!isOn()) {

        theResponse->setCurrentValue(0.0);

        return 0;
    }

    // Reset everything if it was previously applied so that it can be correctly rescaled
    if(patternApplied)
        reset();

    auto PGAScalingFactor = 1.0;

    // Get the max PGA of all of the load patterns, i.e., the max value of all directions; H1,H2,V
    // Scale every direction so that the max PGA equals the intensity scale factor
    auto maxPGA = 0.0;
    for(auto lpat : theLoadPatterns)
    {
        auto theLoadPat = lpat->getTheLoadPattern();

        if(theLoadPat == nullptr)
        {
            qCritical()<<"Error in getting the OpenSees load pattern in "<<this->objectName()<<__FUNCTION__;
            continue;
        }

        //check if it is an earthquake load pattern
        auto uniformExcLoadPat = dynamic_cast<UniformExcitation*>(theLoadPat);
        if(uniformExcLoadPat)
        {
            auto gm = const_cast<GroundMotion*>(uniformExcLoadPat->getGroundMotion());

            auto peakAccel = gm->getPeakAccel();

            if(peakAccel>maxPGA)
                maxPGA = peakAccel;
        }
    }

    if(scalingModelResponse)
    {
        auto PGAIntensity = scalingModelResponse->getCurrentValue();

        // qDebug()<<"The PGA from "<<scalingModelResponse->objectName()<<" is: "<<PGAIntensity;

        //The ground motion will get scaled so that the PGA matches the intensity
        if(PGAIntensity != 0.0)
        {
            PGAScalingFactor = PGAIntensity/maxPGA;
        }
        else
        {
            theResponse->setCurrentValue(0.0);
            return 0;
        }
    }


    for(auto lpat : theLoadPatterns)
    {
        auto theLoadPat = lpat->getTheLoadPattern();

        if(theLoadPat == nullptr)
        {
            qCritical()<<"Error in getting the OpenSees load pattern in "<<this->objectName()<<__FUNCTION__;
            continue;
        }

        // Check if it is an earthquake load pattern
        auto uniformExcLoadPat = dynamic_cast<UniformExcitation*>(theLoadPat);
        if(uniformExcLoadPat)
        {
            auto direction = uniformExcLoadPat->getDirection();

            // Only scale the horizontal directions for now
            // 1 - corresponds to translation along the global X axis
            // 2 - corresponds to translation along the global Y axis
            // 3 - corresponds to translation along the global Z axis
            // 4 - corresponds to rotation about the global X axis
            // 5 - corresponds to rotation about the global Y axis
            // 6 - corresponds to rotation about the global Z axis
            if(direction >= 3)
                continue;

            auto gm = const_cast<GroundMotion*>(uniformExcLoadPat->getGroundMotion());

            auto peakAccel = gm->getPeakAccel();

            if (theOutputLevel >= RObject::Maximum) {
                qDebug()<<"Scaling the ground motion "<<lpat->objectName()<<" with a peak ground acceleration of: "<<peakAccel<<", by a scale factor of: "<<PGAScalingFactor;
            }

            // auto newPGA = peakAccel*PGAScalingFactor;
            // qDebug()<<"The new PGA will be: "<<newPGA;

            // Scale it according to some other constant, i.e., gravity!
            auto finalScaling = PGAScalingFactor*scaleFactor;

            lpat->setCFactor(finalScaling);
            lpat->update();

        }
    }


    if (theOutputLevel >= RObject::Medium) {
        qDebug() << "***********************************";
        qDebug() << "***********************************";
        qDebug() << "A load pattern "<<this->objectName()<<" is being applied at time " <<theCurrentTime->getTime();
        qDebug() << "***********************************";
        qDebug() << "***********************************";

    }

    // Set response as 1.0 if the load pattern is active, otherwise set 0.0
    theResponse->setCurrentValue(1.0);

    return 0;
}


QList<QPointer<RLoadPattern> > RLoadPatternModel::getTheLoadPatterns()
{
    return theLoadPatterns;
}


double RLoadPatternModel::getScaleFactor() const
{
    return scaleFactor;
}


void RLoadPatternModel::setScaleFactor(double value)
{
    scaleFactor = value;
}
