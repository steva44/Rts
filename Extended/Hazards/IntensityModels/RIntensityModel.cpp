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

#include "RIntensityModel.h"
#include "RParameter.h"
#include "RLocation.h"
#include "RBIM.h"

RIntensityModel::RIntensityModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    thePeriod = 0;
    theLocation = nullptr;
}


RIntensityModel::~RIntensityModel()
{

}


int RIntensityModel::resetTime()
{
    theRjb = 0.0;
    return 0;
}


void RIntensityModel::setIntensityMeasure(QString theIntensityMeasure)
{
    if (theIntensityMeasure == QString("PGA")) {

        theResponseType = PGA;
    }
    else if (theIntensityMeasure == QString("Sa")) {

        theResponseType = Sa;
    }
    else if (theIntensityMeasure == QString("PGV")) {

        theResponseType = PGV;
    }
    else if (theIntensityMeasure == QString("PGD")) {

        theResponseType = PGD;
    }
    else {

        qCritical() << this->objectName() << "does not understand the intensity measure" << theIntensityMeasure;
    }
}


QString RIntensityModel::getResponseType()
{
    if (theResponseType == RResponseType::PGA) {

        return "PGA";
    }
    else if (theResponseType == RResponseType::Sa) {

        return "Sa";
    }
    else if (theResponseType == RResponseType::PGV) {

        return "PGV";
    }
    else if (theResponseType == RResponseType::PGD) {

        return "PGD";
    }
    else {

        qCritical() << this->objectName() << "does not understand the intensity measure" << theResponseType;
    }

    return "NULL";
}


void RIntensityModel::setResponseType(QString value)
{
    this->setIntensityMeasure(value);
}


#ifdef DETAILEDBUILDINGS
QObject* RIntensityModel::getBIM() const
{
    return theBIM;
}


void RIntensityModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);


    auto buildingLocation = theBIM->getLocation();

    auto buildingPeriod = theBIM->getBuildingPeriod();

    if(buildingLocation == nullptr)
    {
        qDebug()<<"Warning, the BIM could not return a location. Without a location, no intensity will be applied!";
        return;
    }

    this->setLocation(buildingLocation);
    this->setPeriod(buildingPeriod);

}
#endif


void RIntensityModel::setLocation(QObject *value)
{
    // First we need to cast from QObject to RParameter...
    RParameter *theParameter = qobject_cast<RParameter *>(value);

    if (!theParameter) {
        qCritical() << "setLocation in" << this->objectName() << "could not cast the input to an RParameter";
        return;
    }

    // Next, set it as a map location
    theLocation = parameterToLocation(theParameter);
}


double RIntensityModel::getRjb()
{
    return theRjb;
}


void RIntensityModel::setPeriod(double value)
{
    thePeriod = value;
}
