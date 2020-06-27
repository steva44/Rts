#ifndef RGurvinderDemolitionCostModel_H
#define RGurvinderDemolitionCostModel_H
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
*   - Gurvinder Gill                                                 *
*                                                                    *
*********************************************************************/

// Simple cost model to calculate the cost of building demolition

#include "RBIM.h"
#include "RModel.h"
#include "RTimeSeries.h"
#include "RResponse.h"

class RContinuousRandomVariable;

class RGurvinderDemolitionCostModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)

public:
    RGurvinderDemolitionCostModel(QObject *parent, QString name);
    ~RGurvinderDemolitionCostModel() override;

    QObject *getBIM() const;
    void setBIM(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

private:

    RResponse *theResponse;
    RResponse *theJouleCoalResponse;
    RResponse *theJouleDieselResponse;
    RResponse *theJouleElectricityResponse;
    RResponse *theJouleFuelOilResponse;
    RResponse *theJouleNaturalGasResponse;

    RBIM *theBIM;

    void createDatabaseOfRandomVariables();

    // Declare the random variables
    RContinuousRandomVariable *theDirectCostOfConcrete;
    RContinuousRandomVariable *theDirectCostOfWood;
    RContinuousRandomVariable *theDirectCostOfSteel;
    RContinuousRandomVariable *theCostOfLandfilling;

    RContinuousRandomVariable *theManufacturingIntensityConcrete;
    RContinuousRandomVariable *theManufacturingIntensitySteel;
    RContinuousRandomVariable *theManufacturingIntensityWood;
    RContinuousRandomVariable *theMaterialTransportationIntensityConcrete;
    RContinuousRandomVariable *theMaterialTransportationIntensitySteel;
    RContinuousRandomVariable *theMaterialTransportationIntensityWood;

    // Weight density of materials
    double density_concrete;
    double density_steel;
    double density_wood;

    // Fraction of material m that is recyclable, f^R
    double recycling_rate_concrete;
    double recycling_rate_steel;
    double recycling_rate_wood;

    // Fraction of energy used in the creation of construction-ready material from the recycled material relative to the production of new materials, f^ER
    double percent_process_energy_after_recycling_concrete;
    double percent_process_energy_after_recycling_steel;
    double percent_process_energy_after_recycling_wood;

    double distance_to_landfill = 10e3;

    // Fraction of energy sources for extraction and manufacturing, f^EM
    double fraction_fuel_manufacturing_concrete_coal;
    double fraction_fuel_manufacturing_concrete_fuel_oil;
    double fraction_fuel_manufacturing_concrete_natural_gas;
    double fraction_fuel_manufacturing_concrete_electricity;

    double fraction_fuel_manufacturing_steel_coal;
    double fraction_fuel_manufacturing_steel_fuel_oil;
    double fraction_fuel_manufacturing_steel_natural_gas;
    double fraction_fuel_manufacturing_steel_electricity;

    double fraction_fuel_manufacturing_wood_coal;
    double fraction_fuel_manufacturing_wood_fuel_oil;
    double fraction_fuel_manufacturing_wood_natural_gas;
    double fraction_fuel_manufacturing_wood_electricity;

};

#endif
