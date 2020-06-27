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

#include "RStevesOperationImpactModel.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RComponent.h"
#include "RWallComponent.h"
#include "RPlateComponent.h"
#include "RWindowComponent.h"
#include "RStatistics.h"

#include <QCoreApplication>

RStevesOperationImpactModel::RStevesOperationImpactModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = nullptr;
    theStatsMethod = nullptr;
    theCostResponse = nullptr;
    theWaterResponse = nullptr;
    theTimeResponse = nullptr;
    theEmissionsResponse = nullptr;
    theEnergyResponse = nullptr;
    heatingCoolingEfficiency = nullptr;
    airChangesPerHour = nullptr;

    // Create the response objects
    // Cost
    theDomain->createObject(objectName() + "CostResponse", "RGenericResponse");
    theCostResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theCostResponse->setModel(this);

    // Time
    theDomain->createObject(objectName() + "TimeResponse", "RGenericResponse");
    theTimeResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theTimeResponse->setModel(this);

    // Water
    theDomain->createObject(objectName() + "WaterResponse", "RGenericResponse");
    theWaterResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theWaterResponse->setModel(this);

    // Information rich responses
    // Emissions
    theDomain->createObject(objectName() + "EmissionsResponse", "RInfoRichResponse");
    theEmissionsResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEmissionsResponse->setModel(this);

    // Energy
    theDomain->createObject(objectName() + "EnergyResponse", "RInfoRichResponse");
    theEnergyResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEnergyResponse->setModel(this);

    // The last "time" in the buildings life that the operations model was run
    thePreviousTime = 0.0;

    // The ambient internal temperature of the building
    ambientTemp  = 23.0;

    // The number of days that define an operations period
    // Roughly every three months
    daysPeriod = 120;

    // The type of heating system, e.g., natural gas, electric
    heatingSystemType = "NaturalGas";

    theStatsMethod = theDomain->getStatsMethod();

    createDatabaseOfRandomVariables();
}


RStevesOperationImpactModel::~RStevesOperationImpactModel()
{

}


QObject * RStevesOperationImpactModel::getBIM() const
{
    return theBIM;
}


void RStevesOperationImpactModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


QList<QPair<double, QString> > RStevesOperationImpactModel::getTriggerTimes(double startTime, double endTime)
{
    QList<QPair<double, QString> > times;

    // First get the end of the construction time from the BIM -- assume that building operation starts at the time of construction and goes until demolition, every month
    auto operationStart = theBIM->getTimeOfConstructionValue();
    auto operationEnd = theBIM->getTimeOfDemolitionValue();

    double time = operationStart;

    // The first trigger time should be
    int day = daysPeriod;

    while (time < operationEnd)
    {

        QString thisName  = this->objectName() + "OperationalPeriod" + QString::number(time);

        time = trunc(operationStart) + day;

        times.append(qMakePair(time, thisName));

        timeList.append(time);

        day += daysPeriod;
    }

    // qDebug()<<"The total number of trigger points in "<<this->objectName()<<" is "<<times.size();

    return times;
}


int RStevesOperationImpactModel::evaluateModel(RGradientType theGradientType)
{
    if (theOutputLevel >= RObject::Maximum) {

        qDebug()<<"***************************";
        qDebug()<<"*****Operations Phase*****";
        qDebug()<<"***************************";
    }

    // Clear the responses
    theCostResponse->setCurrentValue(0.0);
    theTimeResponse->setCurrentValue(0.0);
    theWaterResponse->setCurrentValue(0.0);
    // Reset the informationRich responses
    theEmissionsResponse->clearResponses();
    theEnergyResponse->clearResponses();


    // Check that there is a BIM (always needed)
    if(theBIM == nullptr)
    {
        qCritical() << this->objectName()<<" needs a BIM";
        return 0;
    }

    // Check if it is time to evaluate this model
    if (!timeList.contains(theCurrentTime->getCurrentValue()))
    {
        return 0;
    }

    // Create the mesh if not already done
    theBIM->createMesh();

    // Check if the components were created
    auto theComponentList = theBIM->getComponentList();
    if(theComponentList.isEmpty())
    {
        qDebug()<<"There are no components in "<<__FUNCTION__;
        return -1;
    }

    // Keep the GUI responsive
    QCoreApplication::processEvents();

    //Get the commercial floor area in m2
    auto areaCommercial = theBIM->getCommercialFloorArea();

    //**************************************************
    //**************************************************
    //***********Direct Cost of Operation***************
    //**************************************************
    //**************************************************

    auto directCost = 0.0;

    // To do: Insurance
    // auto monthlyInsuranceCost = 15000.0;
    // auto monthlyInsuranceCost = 500.0;

    auto monthlyInsuranceCost = 0.0;

    directCost+= monthlyInsuranceCost*3.0;

    // To do: Financing i.e., mortgage or interest costs
    // auto monthlyInterestCost = 9000.0;
    // auto monthlyInterestCost = 2000.0;

    auto monthlyInterestCost = 0.0;

    directCost+= monthlyInterestCost*3.0;

    // To do: Maintenance costs

    // To do: Commerical rental income

    // avg 63.10  	$/ft2/yr	(CBRE, 2018)
    // 1 ft2 = 0.092903 m2

    // auto yearlyRentSqM = 62.10/0.092903;
    // auto monthlyIncome = areaCommercial*yearlyRentSqM/12.0;

    // To do: Residential rental income

    auto monthlyIncome = 0.0;

    // Note the negative sign, this is profit!
    directCost -= monthlyIncome*3.0;

    // qDebug()<<"Monthly Income"<<monthlyIncome;

    theCostResponse->setCurrentValue(directCost);

    //**************************************************
    //**************************************************
    //*********************Energy***********************
    //**************************************************
    //**************************************************

    auto JoulesNaturalGas = 0.0;

    auto kWhElectricity   = 0.0;

    auto month = theCurrentTime->getMonth();

    // Calculate the average temperature differential over the preceding time period
    int monthsPeriod = daysPeriod/30;

    auto sumDelT = 0.0;
    for(int i = 0; i < monthsPeriod; ++i)
    {
        // Get the average temperature differential over the previous months
        sumDelT += this->deltaTemperature(month-i);
    }

    auto delT = fabs(sumDelT/static_cast<double>(monthsPeriod));

    // qDebug()<<"Delta temp is "<<delT;

    // Total watts heat loss
    auto wattsTotal = 0.0;

    // Iterate through the components to calculate energy loss rate through the walls, windows, doors
    for(auto&& it : theComponentList)
    {

        // Check if it is an exterior component
        if(!it->isExterior())
            continue;

        // The slabs (roof) and exterior walls can contribute to energy loss; as will windows, doors, etc.
        auto slab = qobject_cast<RPlateComponent*>(it);
        auto wall = qobject_cast<RWallComponent*>(it);
        auto window = qobject_cast<RWindowComponent*>(it);

        // Get the R-Value of the component - R Units = (m^2 * degree C) / Watts
        auto RValue = it->getRSIValue();

        if(RValue<0.0)
        {
            qDebug()<<"R-value is not implemented for component "<<it->objectName();
            continue;
        }

        if(window)
        {
            // Get the elevation area of the window in m2
            auto area = window->getElevationArea();

            // Heat transfer formula -> Ht =  (A * deltaTemperature) / R
            wattsTotal += (area * delT) / RValue;
        }
        else if(wall)
        {
            // Get the elevation area of the window in m2
            auto area = wall->getElevationArea();

            // Heat transfer formula -> Ht =  (A * deltaTemperature) / R
            // R Units = (m^2 * degree C) / Watts

            wattsTotal += (area * delT) / RValue;
        }
        else if(slab)
        {
            // Get the plan area of the slab in m2
            auto area = slab->getPlanArea()  ;

            // Heat transfer formula -> Ht =  (A * deltaTemperature) / R
            // R Units = (m^2 * degree C) / Watts

            wattsTotal += (area * delT) / RValue;
        }
    }

    // Calculate the number of hours in daysPeriod
    auto numHrs = static_cast<double>(daysPeriod)*24.0;

    // Get the heating and cooling efficiency ratio
    auto heatCoolEff = heatingCoolingEfficiency->getCurrentValue();

    // Just in case the efficiency exceeds the theoretical limit
    if(heatCoolEff>0.95)
        heatCoolEff=0.95;

    // Calculate the joules of energy lost due to transmission - 1 Watt = 1 Joule / Second
    auto joulesTransmission = wattsTotal*3600.0*numHrs/heatCoolEff;

    // Next calculate the energy lost by ventilation
    // Q = cp x rho × n × V × ΔT [watt-hour]
    // where
    // cp is its heat capacity -> Specific heat capacity (Cp) air at 0°C and 1 bara:  1.006 kJ/kgK = 0.24028 Btu(IT)/(lbm °F) or kcal/(kg K) https://www.engineeringtoolbox.com/air-properties-d_156.html
    // rho is the density -> Density  (at 0°C and 1 bara): 1.276 kg/m3 = 0.00248 slug/ft3 = 0.0797 lb/ft3 https://www.engineeringtoolbox.com/air-properties-d_156.html
    // n is the number of ACH - complete air changes that take place per hour
    // V is the volume of the building
    // ΔT is the temperature differential

    // Assume n is 3.0 for ventilation and about 0.5 for infiltration - https://www.engineeringtoolbox.com/air-change-rate-room-d_867.html
    // RV with mean 6.0 and cov 0.30
    auto ACH = airChangesPerHour->getCurrentValue(); //hour
    auto cp = 1006.0; // J/kg/°C
    auto rho = 1.276; // kg/m3

    // Assume 80% of the building is heated
    auto vol = 0.80*theBIM->getTheBuildingVolume();

    // Calculate the joules of energy lost due to ventilation and infiltration
    auto joulesVentInf = numHrs*cp*rho*ACH*vol*delT/heatCoolEff;

    // Calculate the total joules of energy lost due to transmission + ventilation + infiltration
    auto totalJoulesReqd = joulesTransmission + joulesVentInf;

    // Get the heating system type
    if(heatingSystemType == "NaturalGas")
    {
        JoulesNaturalGas += totalJoulesReqd;
    }
    else if(heatingSystemType == "AirSourceHeatPump")
    {
        //Taking heat from the air...https://www.cleanenergybc.org/wp-content/uploads/2019/03/electrification-bc-CEBC-webinar-March2019.pdf
        auto COP = 3.5;

        auto joulesElecHeat= totalJoulesReqd/COP;

        kWhElectricity += joulesElecHeat/3600000.0;
    }
    else
    {
        qDebug()<<"Error: The heating system type "<<heatingSystemType<<" is not supported in "<<__FUNCTION__;
    }

    // For commercial buildings
    // Estimation of Electricity Consumption in Commercial Buildings, Stephen M. Frank and Pankaj K. (PK) Sen
    // Average power per-unit-area is 1.7 (W/ft2 )
    // Power per-unit-area in (W/m2 )
    auto powPerUnitArea = 1.7*10.7639104;

    // Get the area of the building in m2
    auto buildingSize = theBIM->getCommercialFloorArea();

    // Power consumption kWh for commercial
    kWhElectricity +=  numHrs*buildingSize*powPerUnitArea/1000.0;

    // Power consumption kWh for residential
    auto numHouseholds = theBIM->getNumberOfHousingUnits();

    // qDebug()<<"NUM HOUSEHOLDS "<<numHouseholds;

    // Average electricity consumption per electrified household Canada 11,135 kWh/year per household
    // 2014 averages Enerdata via World Energy Council
    // https://wec-indicators. enerdata.net/household-electricity-use.html.
    kWhElectricity +=  static_cast<double>(numHouseholds)*static_cast<double>(numHrs)*11135.0/8760.0;

    // Set the responses
    theEnergyResponse->setCurrentValue("kWhElectricityBuildingOperations",kWhElectricity);
    theEnergyResponse->setCurrentValue("JoulesNaturalGasBuildingOperations",JoulesNaturalGas);
    theStatsMethod->update(theEnergyResponse->objectName()+"kWhElectricityBuildingOperations",kWhElectricity);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesNaturalGasBuildingOperations",JoulesNaturalGas);

    //**************************************************
    //**************************************************
    //**********************WATER***********************
    //**************************************************
    //**************************************************

    // Calculate water usage per person, per day
    // Calculate number of people from number of households

    // Residential water consumption is 0.438 m3/person/day (Metro Vancouver, 2018)
    auto waterUsagePersonDay = 0.438;

    //  Number of people per household
    //  Census Profile, 2016 Census
    //  Average household size 	2.5
    //  https://www12.statcan.gc.ca/census-recensement/2016/dp-pd/prof/details/page.cfm?Lang=E&Geo1=CMACA&Code1=933&Geo2=PR&Code2=59&Data=Count&SearchText=Vancouver&SearchType=Begins&SearchPR=01&B1=All&TABID=1
    auto AvgPeoplePerHousehold = 2.5;

    auto totalWaterUsage = waterUsagePersonDay*AvgPeoplePerHousehold*numHouseholds*static_cast<double>(daysPeriod);

    // Commercial water consumption - 0.07 	gal/sq ft/day of heated area	(Dziegielewski et al. 2000)
    // 1 sq ft = 0.092903 m2
    // 1 gal = 0.00378541 m3

    auto usagePerSqM = 0.07*0.00378541*0.092903;

    totalWaterUsage += usagePerSqM*areaCommercial*static_cast<double>(daysPeriod);

    // Give output if requested
    if (theOutputLevel == RObject::Maximum) {
        qDebug() << "The total water usage is" << totalWaterUsage;
    }

    // Set the responses
    theWaterResponse->setCurrentValue(totalWaterUsage);
    theStatsMethod->update(theWaterResponse->objectName()+"totalWaterUsage",totalWaterUsage);

    // Store the current time as a history variable
    thePreviousTime = theCurrentTime->getCurrentValue();

    return 1;
}


int RStevesOperationImpactModel::resetTime()
{
    thePreviousTime = 0.0;
    timeList.clear();

    return 0;
}


void RStevesOperationImpactModel::createDatabaseOfRandomVariables()
{ 
    theDomain->createObject(objectName() + "heatingCoolingEfficiency", "RContinuousRandomVariable");
    heatingCoolingEfficiency = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    heatingCoolingEfficiency->setDistributionType(QString("Lognormal (zeta, sigma)"));
    heatingCoolingEfficiency->setMean(QString("0.85"));
    heatingCoolingEfficiency->setCoefficientOfVariation("0.1");


    theDomain->createObject(objectName() + "airChangesPerHour", "RContinuousRandomVariable");
    airChangesPerHour = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    airChangesPerHour->setDistributionType(QString("Lognormal (zeta, sigma)"));
    airChangesPerHour->setMean(QString("3.5"));
    airChangesPerHour->setCoefficientOfVariation("0.3");
}


double RStevesOperationImpactModel::deltaTemperature(int month)
{
    // https://www.theweathernetwork.com/ca/api/sitewrapper/index?b=%2Fstatistics%2F&p=%2Fforecasts%2Fstatistics%2Findex&url=%2Fstatistics%2Fcabc0308%2F%2F%2F%2F%3F
    // Average monthly temperature for Vancouver in degrees celsius
    //          JAN	FEB	MAR	APR	MAY	  JUN	JUL	  AUG	SEP	 OCT	NOV	 DEC
    // Average	4.8	5.9	7.6	10	13.2  15.9	18.1  18.3	15.4 11.1	7.1	 4.8

    auto deltaT =  0.0;

    if(month > 12)
        month -= 12;
    else if(month <= 0)
        month += 12;

    switch (month) {
    case 1:
        deltaT = ambientTemp - 4.8;
        break;
    case 2:
        deltaT = ambientTemp - 5.9;
        break;
    case 3:
        deltaT = ambientTemp - 	7.6;
        break;
    case 4:
        deltaT = ambientTemp - 10.0;
        break;
    case 5:
        deltaT = ambientTemp - 	13.2;
        break;
    case 6:
        deltaT = ambientTemp - 15.9;
        break;
    case 7:
        deltaT = ambientTemp - 18.1;
        break;
    case 8:
        deltaT = ambientTemp -  18.3;
        break;
    case 9:
        deltaT = ambientTemp - 15.4;
        break;
    case 10:
        deltaT = ambientTemp -  11.1;
        break;
    case 11:
        deltaT = ambientTemp - 7.1;
        break;
    case 12:
        deltaT = ambientTemp - 4.8;
        break;
    default:
        qDebug()<<"Warning: month values of "<<month<<" not recognized in "<<__FUNCTION__;
        break;
    }

    return deltaT;
}

