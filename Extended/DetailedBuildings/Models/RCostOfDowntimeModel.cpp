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

#include "RCostOfDowntimeModel.h"
#include "RDomain.h"
#include "RContinuousRandomVariable.h"
#include "RStatistics.h"
#include "RBIM.h"

RCostOfDowntimeModel::RCostOfDowntimeModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theResponse = nullptr;
    theInputParameter = nullptr;
    theBIM = nullptr;

    // Create the response object
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    theStatsMethod = theDomain->getStatsMethod();
}


RCostOfDowntimeModel::~RCostOfDowntimeModel()
{

}


QObject * RCostOfDowntimeModel::getInputParameter() const
{
    return theInputParameter;
}


void RCostOfDowntimeModel::setInputParameter(QObject *value)
{
    theInputParameter = qobject_cast<RParameter *>(value);
}


QObject * RCostOfDowntimeModel::getBIM() const
{
    return theBIM;
}


void RCostOfDowntimeModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


int RCostOfDowntimeModel::evaluateModel(RGradientType theGradientType)
{

    theResponse->setCurrentValue(0.0);

    if(theBIM == nullptr)
    {
        qCritical()<<"Error this model "<<this->objectName()<<" needs a BIM in "<<__FUNCTION__;
        return -1;
    }

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"***************************";
        qDebug()<<"*****Cost of Downtime******";
        qDebug()<<"***************************";
    }

    auto downTimeDays = theInputParameter->getCurrentValue();

    if(downTimeDays == 0.0)
        return 0;

    auto areaCommercial = theBIM->getCommercialFloorArea();

    // Commerical rental avg 63.10 $/ft2/yr for vancouver (CBRE, 2018)
    // 1 ft2 = 0.092903 m2

    auto yearlyRentSqM = 62.10/0.092903;

    auto dailyRentSqM = yearlyRentSqM/365.0;

    auto totalCostOfDowntime = areaCommercial*downTimeDays*dailyRentSqM;

    // To do: residential

    if (theOutputLevel >= RObject::Medium)
    {
        qDebug()<<"The total cost of downtime is:"<<totalCostOfDowntime;
    }

    theStatsMethod->update(this->objectName()+"totalDirectCostOfDowntime",totalCostOfDowntime);

    theResponse->setCurrentValue(totalCostOfDowntime);

    return 0;
}



