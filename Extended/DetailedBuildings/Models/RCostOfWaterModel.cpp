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

#include "RCostOfWaterModel.h"
#include "RContinuousRandomVariable.h"
#include "RStatistics.h"
#include "RDomain.h"

RCostOfWaterModel::RCostOfWaterModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theResponse = nullptr;
    theEnergyResponse = nullptr;

    // Cost of water response object
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    // Energy response - energy consumption involved in water treatment and delivery
    theDomain->createObject(objectName() + "EnergyResponse", "RInfoRichResponse");
    theEnergyResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEnergyResponse->setModel(this);

    theStatsMethod = theDomain->getStatsMethod();
}


RCostOfWaterModel::~RCostOfWaterModel()
{

}


QString RCostOfWaterModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}


void RCostOfWaterModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


int RCostOfWaterModel::evaluateModel(RGradientType theGradientType)
{
    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"****************************";
        qDebug()<<"*******Cost of Water********";
        qDebug()<<"****************************";
    }

    theResponse->setCurrentValue(0.0);

    // Get the models that serve input to this model
    QList<RResponse *> inputResponselList;
    for (auto it : theInputParameterList)
    {
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(it);

        if(theInputCastAsResponse)
            inputResponselList.push_back(theInputCastAsResponse);
    }

    auto totalCostOfWater = 0.0;
    auto kWhElectricity   = 0.0;

    // For each response
    for(auto it : inputResponselList)
    {
        // Get the water useage from the response
        auto m3OfWater = it->getCurrentValue();

        if(m3OfWater == 0.0)
            continue;

        auto costOfWaterAtThisStage = 0.0;

        // First calculate the energy to deliver water in metro vancouver 0.123	kWh/m3	(Metro Vancouver, 2014)
        auto kWhPerm3 =  0.123;

        auto kWhElectricityAtThisStage = kWhPerm3*m3OfWater;

        // Then the direct cost of water
        // https://vancouver.ca/home-property-development/metered-rates.aspx
        // City of Vancouver water meter rates
        // 1 unit is 2.83168466 cubic metres
        // Rate per unit 	2018
        // October 1 – May 31 	$2.849
        // June 1 – September 30 $3.571
        auto Costm3Water =  (5.0*3.571+7.0*2.849)/12.0/2.83168466;

        costOfWaterAtThisStage = Costm3Water*m3OfWater;

        if (theOutputLevel >= RObject::Maximum)
        {
            qDebug()<<"For the response "<<it->objectName();
            qDebug()<<"Cost of water: "<<costOfWaterAtThisStage;
            qDebug()<<"kWh of electricity for water supply: "<<kWhElectricityAtThisStage;
        }

        totalCostOfWater += costOfWaterAtThisStage;
        kWhElectricity += kWhElectricityAtThisStage;

        theStatsMethod->update(it->objectName()+"DirectCostOfWater",costOfWaterAtThisStage);
        theStatsMethod->update(it->objectName()+"ElectricityForWaterGeneration",kWhElectricityAtThisStage);

        theStatsMethod->accumulate(it->objectName()+"DirectCostOfWater",costOfWaterAtThisStage);

    }

    // Set the response value
    theEnergyResponse->setCurrentValue("kWhElectricity",kWhElectricity);

    if (theOutputLevel >= RObject::Maximum) {

        qDebug()<<"The total cost of water is:"<<totalCostOfWater;
        qDebug()<<"The total kWh for supplying water is:"<<kWhElectricity;

    }

    theStatsMethod->update(this->objectName()+"totalDirectCostOfWater",totalCostOfWater);
    theStatsMethod->update(this->objectName()+"totalElectricityForWaterGeneration",kWhElectricity);

    theResponse->setCurrentValue(totalCostOfWater);

    return 0;
}


