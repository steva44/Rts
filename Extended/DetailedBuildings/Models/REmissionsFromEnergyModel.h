#ifndef REmissionsFromEnergyModel_H
#define REmissionsFromEnergyModel_H
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

// This model calculates the emissions, CO2, N20, CH4, from energy usage, according to the energy consumption mode

#include "RModel.h"
#include "RInfoRichResponse.h"

class RStatistics;
class RContinuousRandomVariable;

class REmissionsFromEnergyModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)

public:
    REmissionsFromEnergyModel(QObject *parent, QString name);
    ~REmissionsFromEnergyModel() override;

    QString getInputParameterList();

    void setInputParameterList(QString value);

    int evaluateModel(RGradientType theGradientType) override;

private:

    QList<QPointer<RParameter> > theInputParameterList;
    RStatistics* theStatsMethod;
    QPointer<RInfoRichResponse> theResponse;

    // Light medium duty transport
    // CO2
    RContinuousRandomVariable* LightMedDutyGasolineCO2Emissions;
    RContinuousRandomVariable* LightMedDutyDieselCO2Emissions;
    RContinuousRandomVariable* LightMedDutyPropaneCO2Emissions;
    RContinuousRandomVariable* LightMedDutyNatGasCO2Emissions;

    // CH4
    RContinuousRandomVariable* LightMedDutyGasolineCH4Emissions;
    RContinuousRandomVariable* LightMedDutyDieselCH4Emissions;
    RContinuousRandomVariable* LightMedDutyPropaneCH4Emissions;
    RContinuousRandomVariable* LightMedDutyNatGasCH4Emissions;

    // N2O
    RContinuousRandomVariable* LightMedDutyGasolineN2OEmissions;
    RContinuousRandomVariable* LightMedDutyDieselN2OEmissions;
    RContinuousRandomVariable* LightMedDutyPropaneN2OEmissions;
    RContinuousRandomVariable* LightMedDutyNatGasN2OEmissions;


    // Heavy duty transport
    // CO2
    RContinuousRandomVariable* HeavyDutyGasolineCO2Emissions;
    RContinuousRandomVariable* HeavyDutyDieselCO2Emissions;
    RContinuousRandomVariable* HeavyDutyNatGasCO2Emissions;

    // CH4
    RContinuousRandomVariable* HeavyDutyGasolineCH4Emissions;
    RContinuousRandomVariable* HeavyDutyDieselCH4Emissions;
    RContinuousRandomVariable* HeavyDutyNatGasCH4Emissions;

    // N2O
    RContinuousRandomVariable* HeavyDutyGasolineN2OEmissions;
    RContinuousRandomVariable* HeavyDutyDieselN2OEmissions;
    RContinuousRandomVariable* HeavyDutyNatGasN2OEmissions;

    // Equipment Usage
    // CO2
    RContinuousRandomVariable* EquipmentGasolineCO2Emissions;
    RContinuousRandomVariable* EquipmentDieselCO2Emissions;
    RContinuousRandomVariable* EquipmentNatGasCO2Emissions;

    // CH4
    RContinuousRandomVariable* EquipmentGasolineCH4Emissions;
    RContinuousRandomVariable* EquipmentDieselCH4Emissions;
    RContinuousRandomVariable* EquipmentNatGasCH4Emissions;

    // N2O
    RContinuousRandomVariable* EquipmentGasolineN2OEmissions;
    RContinuousRandomVariable* EquipmentDieselN2OEmissions;
    RContinuousRandomVariable* EquipmentNatGasN2OEmissions;

    // Building Operations
    // CO2
    RContinuousRandomVariable* BuildingOperationsNatGasCO2Emissions;
    RContinuousRandomVariable* ElectricityGenerationCO2Emissions;

    // CH4
    RContinuousRandomVariable* BuildingOperationsNatGasCH4Emissions;

    // N2O
    RContinuousRandomVariable* BuildingOperationsNatGasN2OEmissions;

    // Create the random variables to convert energy usage to emissions
    void createRandomVariables(void);
};

#endif
