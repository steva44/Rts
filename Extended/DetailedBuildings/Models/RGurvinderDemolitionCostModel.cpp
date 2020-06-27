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

#include "RGurvinderDemolitionCostModel.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RDomain.h"

RGurvinderDemolitionCostModel::RGurvinderDemolitionCostModel(QObject *parent, QString name)
    : RModel(parent, name)
{

    theResponse = nullptr;
    theJouleCoalResponse = nullptr;
    theJouleDieselResponse = nullptr;
    theJouleElectricityResponse = nullptr;
    theJouleFuelOilResponse = nullptr;
    theJouleNaturalGasResponse = nullptr;
    theBIM = nullptr;
    theDirectCostOfConcrete = nullptr;
    theDirectCostOfWood = nullptr;
    theDirectCostOfSteel = nullptr;
    theCostOfLandfilling = nullptr;
    theManufacturingIntensityConcrete = nullptr;
    theManufacturingIntensitySteel = nullptr;
    theManufacturingIntensityWood = nullptr;
    theMaterialTransportationIntensityConcrete = nullptr;
    theMaterialTransportationIntensitySteel = nullptr;
    theMaterialTransportationIntensityWood = nullptr;

    // Weight density of materials
    density_concrete = 2400.0;
    density_steel = 8050.0;
    density_wood = 500.0;

    // Fraction of material m that is recyclable, f^R
    recycling_rate_concrete = 0.85;
    recycling_rate_steel = 0.98;
    recycling_rate_wood = 0.98;

    // Fraction of energy used in the creation of construction-ready material from the recycled material relative to the production of new materials, f^ER
    percent_process_energy_after_recycling_concrete = 0.50;
    percent_process_energy_after_recycling_steel = 0.30;
    percent_process_energy_after_recycling_wood = 0.60;

    distance_to_landfill = 10e3;

    // Fraction of energy sources for extraction and manufacturing, f^EM
    fraction_fuel_manufacturing_concrete_coal = 0.471;
    fraction_fuel_manufacturing_concrete_fuel_oil = 0.154;
    fraction_fuel_manufacturing_concrete_natural_gas = 0.031;
    fraction_fuel_manufacturing_concrete_electricity = 0.344;

    fraction_fuel_manufacturing_steel_coal = 0.17;
    fraction_fuel_manufacturing_steel_fuel_oil = 0.0;
    fraction_fuel_manufacturing_steel_natural_gas = 0.33;
    fraction_fuel_manufacturing_steel_electricity = 0.50;

    fraction_fuel_manufacturing_wood_coal = 0.0;
    fraction_fuel_manufacturing_wood_fuel_oil = 0.728;
    fraction_fuel_manufacturing_wood_natural_gas = 0.03;
    fraction_fuel_manufacturing_wood_electricity = 0.242;

    createDatabaseOfRandomVariables();

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleCoalResponse", "RGenericResponse");
    theJouleCoalResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleCoalResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleDieselResponse", "RGenericResponse");
    theJouleDieselResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleDieselResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleElectricityResponse", "RGenericResponse");
    theJouleElectricityResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleElectricityResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleFuelOilResponse", "RGenericResponse");
    theJouleFuelOilResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleFuelOilResponse->setModel(this);

    theDomain->createObject(objectName() + "JouleNaturalGasResponse", "RGenericResponse");
    theJouleNaturalGasResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleNaturalGasResponse->setModel(this);

}


RGurvinderDemolitionCostModel::~RGurvinderDemolitionCostModel()
{

}


QObject * RGurvinderDemolitionCostModel::getBIM() const
{
    return theBIM;
}


void RGurvinderDemolitionCostModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


int RGurvinderDemolitionCostModel::evaluateModel(RGradientType theGradientType)
{

    if (theBIM->getTimeOfDemolitionValue() == theCurrentTime->getCurrentValue())
    {
        // Get the realizations of the random variables
        // These costs are mainly used for recycling profit; the components calculate their own costs
        double direct_cost_concrete = theDirectCostOfConcrete->getCurrentValue();
        double direct_cost_steel = theDirectCostOfSteel->getCurrentValue();
        double direct_cost_wood = theDirectCostOfWood->getCurrentValue();

        // Environmental cost of placing material m in landfills in $/kg, lm
        double cost_of_landfilling = theCostOfLandfilling->getCurrentValue();

        // Energy intensity demands for extraction and manufacturing, i^EM
        double manufacturing_intensity_concrete = theManufacturingIntensityConcrete->getCurrentValue();
        double manufacturing_intensity_steel = theManufacturingIntensitySteel->getCurrentValue();
        double manufacturing_intensity_wood = theManufacturingIntensityWood->getCurrentValue();

        // Energy intensity demands for transportation of materials, i^TM
        double material_transportation_intensity_concrete = theMaterialTransportationIntensityConcrete->getCurrentValue();
        double material_transportation_intensity_steel = theMaterialTransportationIntensitySteel->getCurrentValue();
        double material_transportation_intensity_wood = theMaterialTransportationIntensityWood->getCurrentValue();

        double total_weight_concrete = theBIM->getVolumeOfConcrete() * density_concrete;
        double total_weight_steel = theBIM->getVolumeOfSteel() * density_steel;
        double total_weight_wood = theBIM->getVolumeOfWood() * density_wood;

        // Money recovered by selling recycled materials
        double result = -1*(total_weight_concrete * recycling_rate_concrete * direct_cost_concrete +
                            total_weight_steel * recycling_rate_steel * direct_cost_steel +
                            total_weight_wood * recycling_rate_wood * direct_cost_wood);

        // Landfilling environmental cost
        result += (total_weight_concrete * (1.0-recycling_rate_concrete) +
                   total_weight_steel * (1.0-recycling_rate_steel) +
                   total_weight_wood * (1.0-recycling_rate_wood)) * cost_of_landfilling;

        theResponse->setCurrentValue(result);

        theJouleCoalResponse->setCurrentValue(total_weight_concrete * recycling_rate_concrete * (1.0-percent_process_energy_after_recycling_concrete) * manufacturing_intensity_concrete * fraction_fuel_manufacturing_concrete_coal +
                                              total_weight_steel * recycling_rate_steel * (1.0-percent_process_energy_after_recycling_steel) * manufacturing_intensity_steel * fraction_fuel_manufacturing_steel_coal +
                                              total_weight_wood * recycling_rate_wood * (1.0-percent_process_energy_after_recycling_wood) * manufacturing_intensity_wood * fraction_fuel_manufacturing_wood_coal);

        theJouleFuelOilResponse->setCurrentValue(total_weight_concrete * recycling_rate_concrete * (1.0-percent_process_energy_after_recycling_concrete) * manufacturing_intensity_concrete * fraction_fuel_manufacturing_concrete_fuel_oil +
                                                 total_weight_steel * recycling_rate_steel * (1.0-percent_process_energy_after_recycling_steel) * manufacturing_intensity_steel * fraction_fuel_manufacturing_steel_fuel_oil +
                                                 total_weight_wood * recycling_rate_wood * (1.0-percent_process_energy_after_recycling_wood) * manufacturing_intensity_wood * fraction_fuel_manufacturing_wood_fuel_oil);

        theJouleElectricityResponse->setCurrentValue(total_weight_concrete * recycling_rate_concrete * (1.0-percent_process_energy_after_recycling_concrete) * manufacturing_intensity_concrete * fraction_fuel_manufacturing_concrete_electricity +
                                                     total_weight_steel * recycling_rate_steel * (1.0-percent_process_energy_after_recycling_steel) * manufacturing_intensity_steel * fraction_fuel_manufacturing_steel_electricity +
                                                     total_weight_wood * recycling_rate_wood * (1.0-percent_process_energy_after_recycling_wood) * manufacturing_intensity_wood * fraction_fuel_manufacturing_wood_electricity);

        theJouleNaturalGasResponse->setCurrentValue(total_weight_concrete * recycling_rate_concrete * (1.0-percent_process_energy_after_recycling_concrete) * manufacturing_intensity_concrete * fraction_fuel_manufacturing_concrete_natural_gas +
                                                    total_weight_steel * recycling_rate_steel * (1.0-percent_process_energy_after_recycling_steel) * manufacturing_intensity_steel * fraction_fuel_manufacturing_steel_natural_gas +
                                                    total_weight_wood * recycling_rate_wood * (1.0-percent_process_energy_after_recycling_wood) * manufacturing_intensity_wood * fraction_fuel_manufacturing_wood_natural_gas);

        // Material transportation to landfill
        theJouleDieselResponse->setCurrentValue((total_weight_concrete * (1.0-recycling_rate_concrete) * material_transportation_intensity_concrete +
                                                 total_weight_steel * (1.0-recycling_rate_steel) * material_transportation_intensity_steel +
                                                 total_weight_wood * (1.0-recycling_rate_wood) * material_transportation_intensity_wood) * distance_to_landfill);

    }
    else {
        theResponse->setCurrentValue(0.0);

        theJouleCoalResponse->setCurrentValue(0.0);
        theJouleDieselResponse->setCurrentValue(0.0);
        theJouleElectricityResponse->setCurrentValue(0.0);
        theJouleFuelOilResponse->setCurrentValue(0.0);
        theJouleNaturalGasResponse->setCurrentValue(0.0);
    }

    return 0;
}


void RGurvinderDemolitionCostModel::createDatabaseOfRandomVariables()
{

    theDomain->createObject(objectName() + "DirectCostOfConcrete", "RContinuousRandomVariable");
    theDirectCostOfConcrete = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theDirectCostOfConcrete->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theDirectCostOfConcrete->setMean(QString("0.06"));
    theDirectCostOfConcrete->setCoefficientOfVariation("0.15");

    theDomain->createObject(objectName() + "DirectCostOfSteel", "RContinuousRandomVariable");
    theDirectCostOfSteel = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theDirectCostOfSteel->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theDirectCostOfSteel->setMean(QString("2.0"));
    theDirectCostOfSteel->setCoefficientOfVariation("0.15");

    theDomain->createObject(objectName() + "DirectCostOfWood", "RContinuousRandomVariable");
    theDirectCostOfWood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theDirectCostOfWood->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theDirectCostOfWood->setMean(QString("0.5"));
    theDirectCostOfWood->setCoefficientOfVariation("0.15");

    theDomain->createObject(objectName() + "CostOfLandfilling", "RContinuousRandomVariable");
    theCostOfLandfilling = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCostOfLandfilling->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theCostOfLandfilling->setMean(QString("0.013"));
    theCostOfLandfilling->setCoefficientOfVariation("0.2");

    theDomain->createObject(objectName() + "ManufacturingIntensityConcrete", "RContinuousRandomVariable");
    theManufacturingIntensityConcrete = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theManufacturingIntensityConcrete->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theManufacturingIntensityConcrete->setMean(QString("950000"));
    theManufacturingIntensityConcrete->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "ManufacturingIntensitySteel", "RContinuousRandomVariable");
    theManufacturingIntensitySteel = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theManufacturingIntensitySteel->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theManufacturingIntensitySteel->setMean(QString("36400000"));
    theManufacturingIntensitySteel->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "ManufacturingIntensityWood", "RContinuousRandomVariable");
    theManufacturingIntensityWood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theManufacturingIntensityWood->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theManufacturingIntensityWood->setMean(QString("8500000"));
    theManufacturingIntensityWood->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "MaterialTransportationIntensityConcrete", "RContinuousRandomVariable");
    theMaterialTransportationIntensityConcrete = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theMaterialTransportationIntensityConcrete->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theMaterialTransportationIntensityConcrete->setMean(QString("3.6"));
    theMaterialTransportationIntensityConcrete->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "MaterialTransportationIntensitySteel", "RContinuousRandomVariable");
    theMaterialTransportationIntensitySteel = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theMaterialTransportationIntensitySteel->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theMaterialTransportationIntensitySteel->setMean(QString("2.4"));
    theMaterialTransportationIntensitySteel->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "MaterialTransportationIntensityWood", "RContinuousRandomVariable");
    theMaterialTransportationIntensityWood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theMaterialTransportationIntensityWood->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theMaterialTransportationIntensityWood->setMean(QString("7.64"));
    theMaterialTransportationIntensityWood->setCoefficientOfVariation("0.1");

}
