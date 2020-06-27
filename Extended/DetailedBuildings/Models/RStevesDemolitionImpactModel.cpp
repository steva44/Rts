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

#include "RStevesDemolitionImpactModel.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RComponent.h"
#include "RResponse.h"
#include "RInfoRichResponse.h"
#include "RBIM.h"
#include "RJob.h"
#include "RCSISchedulingMethod.h"
#include "RCSIAssembler.h"
#include "RCSICostMethod.h"
#include "RStatistics.h"

#include <QDateTime>
#include <QCoreApplication>

RStevesDemolitionImpactModel::RStevesDemolitionImpactModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = nullptr;
    theHeavyMachineryUseIntensity = nullptr;
    theWorkerTransportationIntensityAutomobil = nullptr;
    theWorkerTransportationIntensityDieselbus = nullptr;
    theWorkerTransportationIntensitySkytrain = nullptr;
    theMaterialTransportationIntensityConcrete = nullptr;
    theMaterialTransportationIntensitySteel = nullptr;
    theMaterialTransportationIntensityWood = nullptr;
    theDirectCostLandfillDisposalWood = nullptr;
    theDirectCostLandfillDisposalOther = nullptr;
    theDirectCostRecyclingConcrete = nullptr;
    theDirectCostRecyclingSteel = nullptr;

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

    // InformationRichResponses
    // Emissions
    theDomain->createObject(objectName() + "EmissionsResponse", "RInfoRichResponse");
    theEmissionsResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEmissionsResponse->setModel(this);

    // Energy
    theDomain->createObject(objectName() + "EnergyResponse", "RInfoRichResponse");
    theEnergyResponse = qobject_cast<RInfoRichResponse *>(theDomain->getLastAddedObject());
    theEnergyResponse->setModel(this);

    // Hours per worker shift
    hours_per_shift = 8.0;

    // Fraction of total worker hours allocated to the use of heavy machinery as determined by the construction type, f^WH
    heavy_machinery_ratio_high_rise_wood = 0.1;
    heavy_machinery_ratio_high_rise_steel = 0.3;
    heavy_machinery_ratio_high_rise_concrete = 0.3;
    heavy_machinery_ratio_low_rise_wood = 0.05;
    heavy_machinery_ratio_low_rise_steel = 0.1;
    heavy_machinery_ratio_low_rise_concrete = 0.1;

    // Fraction of energy source n that is employed in the transportation of workers, f^TW
    fractionWorkerTransportationAutomobil = 0.5;
    fractionWorkerTransportationDieselbus = 0.2;
    fractionWorkerTransportationSkytrain = 0.3;

    // Create the database of random variables used in this model
    createDatabaseOfRandomVariables();

    // Create the scheduler and CSI assembler
    theScheduler = std::make_unique<RCSISchedulingMethod>(theDomain,"DemolitionImpactModelScheduler");
    theCSIAssembler = std::make_unique<RCSIAssembler>(theDomain,"DemolitionImpactCSIAssembler");
    theStatsMethod = theDomain->getStatsMethod();
}


RStevesDemolitionImpactModel::~RStevesDemolitionImpactModel()
{

}


QObject * RStevesDemolitionImpactModel::getBIM() const
{
    return theBIM;
}


void RStevesDemolitionImpactModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


int RStevesDemolitionImpactModel::evaluateModel(RGradientType theGradientType)
{
    // Clear the responses
    theCostResponse->setCurrentValue(0.0);
    theTimeResponse->setCurrentValue(0.0);
    theWaterResponse->setCurrentValue(0.0);
    // Reset the informationRich responses
    theEmissionsResponse->clearResponses();
    theEnergyResponse->clearResponses();
    theScheduler->reset();
    labourQuantityCSICodes.clear();
    double accumulatedDirectCost = 0.0;

    // Check that there is a BIM (always needed)
    if(theBIM == nullptr)
    {
        qCritical() << this->objectName()<<" needs a BIM";
        // return -1;
    }

    // Check to see if it is demolition time, if not return quickly
    if (theCurrentTime->getCurrentValue() != theBIM->getTimeOfDemolitionValue())
    {
        return 0;
    }

    // Keep the GUI responsive
    QCoreApplication::processEvents();

    // Declare the energy sources used in demolition
    double JoulesDieselLightMedDuty = 0.0;
    double JoulesDieselHeavyDuty = 0.0;
    double JoulesDieselEquipment = 0.0;
    double JoulesGasolineLightMedDuty = 0.0;
    double kWhElectricity = 0.0;

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"***************************";
        qDebug()<<"********Demolition*********";
        qDebug()<<"***************************";
    }

    // Create a job
    RJob demoPhase;

    // Heading
    // Line Number: 024116130011
    // Description: Building demolition, large urban projects, rubbish handling, excludes foundation or dump fees, C.F. is volume of building standing

    // The bulk building volume in cubic feet
    auto demoVol = theBIM->getTheBuildingVolume()*35.3147;

    // Labour
    RCrew* crewB8 = demoPhase.addCrew(RCrewCode::B8);
    {
        // Line Number: 024116130020
        // Description: Building demolition, large urban projects, steel, includes 20 mile haul, excludes foundation demolition, dump fees
        // Unit: C.F.
        // Crew: Crew B8
        QString CSINumber = "024116130020";
        QString Description =  "Building demolition, large urban projects, steel, includes 20 mile haul, excludes foundation demolition, dump fees";

        labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,demoVol,Description));

        crewB8->addTask(CSINumber,demoVol,Description,nullptr);
    }

    {
        // Line Number: 024116130050
        // Description: Building demolition, large urban projects, concrete, includes 20 mile haul, excludes foundation demolition, dump fees
        // Unit: C.F.
        // Crew: Crew B8
        QString CSINumber = "024116130050";
        QString Description =  "Building demolition, large urban projects, concrete, includes 20 mile haul, excludes foundation demolition, dump fees";

        labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,demoVol,Description));

        crewB8->addTask(CSINumber,demoVol,Description,nullptr);
    }


    RCrew* crewB3 = demoPhase.addCrew(RCrewCode::B3);
    {
        // Line Number: 024116130700
        // Description: Building demolition, small buildings or single buildings, wood, elevated slabs, includes 20 mile haul, excludes salvage, foundation demolition or dump fees
        // Unit: C.F.
        // Crew: Crew B3
        QString CSINumber = "024116130700";
        QString Description =  " Building demolition, small buildings or single buildings, wood, elevated slabs, includes 20 mile haul, excludes salvage, foundation demolition or dump fees";

        labourQuantityCSICodes.push_back(std::make_tuple(CSINumber,demoVol,Description));

        crewB3->addTask(CSINumber,demoVol,Description,nullptr);
    }

    // Get the cost method, i.e., the method that translates the CSI codes into costs
    auto theCostMethod = theBIM->getTheCSICostMethod();

    // Set the cost-plus multiplier to multiply the bare costs with
    theCostMethod->setDemandSurge(1.2);

    //**************************************************
    //**************************************************
    //******Labour, disposal, and equipment costs*******
    //**************************************************
    //**************************************************

    // Calculate the labour costs
    auto labourDirectCosts = theCostMethod->getLabourCosts(labourQuantityCSICodes);

    if (theOutputLevel >= RObject::Maximum)
    {
        qDebug()<<"***LABOUR DIRECT COSTS***"<<labourDirectCosts;
    }

    accumulatedDirectCost += labourDirectCosts;

    theStatsMethod->update(theCostResponse->objectName()+"LabourCosts",labourDirectCosts);

    // Calculate the equipment costs
    auto equipmentDirectCosts = theCostMethod->getEquipmentCosts(labourQuantityCSICodes);

    if (theOutputLevel >= RObject::Maximum)
    {
        qDebug()<<"***EQUIPMENT DIRECT COSTS***"<<equipmentDirectCosts;
    }

    accumulatedDirectCost += equipmentDirectCosts;

    // Direct costs of landfill disposal and recycling
    // Cost to dispose of materials per tonne
    auto costDisposeWood = theDirectCostLandfillDisposalWood->getCurrentValue();
    auto costRecycleConct = theDirectCostRecyclingConcrete->getCurrentValue();
    auto costRecycleSteel = -1.0*theDirectCostRecyclingSteel->getCurrentValue();

    // In cubic m
    auto demoVolSteel = theBIM->getVolumeOfSteel();
    auto demoVolConc  = theBIM->getVolumeOfConcrete();
    auto demoVolWood  = theBIM->getVolumeOfWood();

    // Convert cubic m to tonne
    auto massTonneSteel = demoVolSteel*7900*0.001;
    auto massTonneWood = demoVolWood*500*0.001;
    auto massTonneConc = demoVolConc*2450.0*0.001;

    auto disposalCost = massTonneSteel*costRecycleSteel + massTonneWood*costDisposeWood + massTonneConc*costRecycleConct;

    accumulatedDirectCost += disposalCost;

    // Set the cost response
    theCostResponse->setCurrentValue(accumulatedDirectCost);
    theStatsMethod->update(theCostResponse->objectName()+"DisposalCost",disposalCost);
    theStatsMethod->update(theCostResponse->objectName()+"EquipmentCosts",equipmentDirectCosts);
    theStatsMethod->update(theCostResponse->objectName(),accumulatedDirectCost);

    //**************************************************
    //**************************************************
    //*****************Time and labour******************
    //**************************************************
    //**************************************************

    theScheduler->addJob(demoPhase);
    theScheduler->setTheCostMethod(theCostMethod);

    if (theOutputLevel >= RObject::Maximum)
    {
        qDebug()<<"******** TIME ********";
    }

    // The critical path is estimated below, i.e., the time for demolition
    auto theCriticalPath = theScheduler->getCriticalPath();

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"The critcal path [hrs]"<<theCriticalPath;
        qDebug()<<"The critcal path [day]"<<theCriticalPath/24.0;
        qDebug()<<"The critcal path [month]"<<theCriticalPath/720.0;
    }

    // The total worker hours are calculated here (different from the critical path)
    double totalWorkHours = theScheduler->getTotalWorkHours();

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"The total work done [hrs]"<<totalWorkHours;
        qDebug()<<"The total work done [day]"<<totalWorkHours/24.0;
        qDebug()<<"The total work done [month]"<<totalWorkHours/720.0;
    }

    // The number of workers
    int numWorkers = theScheduler->getNumberOfWorkers();

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"The number of workers: "<<numWorkers;
    }

    // The number of crews
    int numCrew = theScheduler->getNumberOfCrews();

    if (theOutputLevel >= RObject::Maximum) {
        qDebug()<<"The number of crews: "<<numCrew;
    }

    // Set the time response
    theTimeResponse->setCurrentValue(theCriticalPath/24.0);
    theStatsMethod->update(theTimeResponse->objectName(),theCriticalPath/24.0);
    theStatsMethod->update(this->objectName()+"NumberOfWorkers",numWorkers);
    theStatsMethod->update(this->objectName()+"NumberOfCrews",numCrew);

    //****************************************************************************
    //********************Emissions and Environmental Impacts*********************
    //****************************************************************************

    // the emissions for energy consumption are taken care of in the cost of energy model

    //**************************************************
    //*********************Energy***********************
    //**************************************************

    // Energy intensity demand for work with heavy machinery, i^H
    double heavy_machinery_use_intensity = theHeavyMachineryUseIntensity->getCurrentValue();

    QString constructionType = theBIM->getConstructionType();

    // Use of heavy machinery in demolition of building -- assumes heavy machinery uses diesel
    // Assumes that the hours of heavy machinery useage is a fraction of the total worker hours used in constructions
    // Note: this is assuming the same fraction as construction, however probably greater for demolition
    auto joulesDieselHeavyMachinery = 0.0;

    if (constructionType == QString("high-rise-concrete"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_high_rise_concrete*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }
    else if (constructionType == QString("high-rise-steel"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_high_rise_steel*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }
    else if (constructionType == QString("high-rise-wood"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_high_rise_wood*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }
    else if (constructionType == QString("low-rise-concrete"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_low_rise_concrete*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }
    else if (constructionType == QString("low-rise-steel"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_low_rise_steel*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }
    else if (constructionType == QString("low-rise-wood"))
    {
        joulesDieselHeavyMachinery = heavy_machinery_ratio_low_rise_wood*totalWorkHours*heavy_machinery_use_intensity*1.0;
    }

    JoulesDieselEquipment += joulesDieselHeavyMachinery;

    // To do: query a NETWORK MODEL to get the distance that workers have to travel
    // Otherwise, need to estimate the distance that workers have to travel
    // Average distance travelled by each worker in km to get to site and back home
    double workerDistanceTravelled = 25.0;

    // Total distance is the number of work shifts (total work hours/hours per shift) multiplied by the avg. distance travelled to get to site and back home
    auto avgDistancePerWorker = totalWorkHours/hours_per_shift*workerDistanceTravelled;

    // The energy intensity of transporting a worker, according to the transportation method
    auto workerTransportationIntensityAutomobil = theWorkerTransportationIntensityAutomobil->getCurrentValue();
    auto workerTransportationIntensityDieselbus = theWorkerTransportationIntensityDieselbus->getCurrentValue();
    auto workerTransportationIntensitySkytrain  = theWorkerTransportationIntensitySkytrain->getCurrentValue();

    // The joules of energy for worker transport, according to the energy source
    auto joulesGasolineWorkerTransport = avgDistancePerWorker*workerTransportationIntensityAutomobil*fractionWorkerTransportationAutomobil;
    auto joulesDieselWorkerTransport = avgDistancePerWorker*workerTransportationIntensityDieselbus*fractionWorkerTransportationDieselbus;
    auto joulesElectricityWorkerTransport = avgDistancePerWorker*workerTransportationIntensitySkytrain*fractionWorkerTransportationSkytrain;

    JoulesGasolineLightMedDuty += joulesGasolineWorkerTransport;
    JoulesDieselLightMedDuty += joulesDieselWorkerTransport;
    kWhElectricity += joulesElectricityWorkerTransport*2.77778e-7; //Convert joules to kWh


    // To do: query a NETWORK MODEL to get the distance that materials have to travel
    // Otherwise, need to estimate the distance that materials have to travel
    // Material transportation - assume that all materials are delivered using diesel

    // The distance of transporting concrete, steel, and wood in km to landfil
    double concreteTravelDistance = 25.0;
    double steelTravelDistance = 25.0;
    double woodTravelDistance = 25.0;

    // The energy intensity for material transpoer (number of J/kg/km) per material
    auto materialTransportationIntensityConcrete = theMaterialTransportationIntensityConcrete->getCurrentValue();
    auto materialTransportationIntensitySteel    = theMaterialTransportationIntensitySteel->getCurrentValue();
    auto materialTransportationIntensityWood     = theMaterialTransportationIntensityWood->getCurrentValue();

    // Mass of each material in Kg
    auto totalMassConcrete = demoVolConc*2450.0;
    auto totalMassSteel = demoVolSteel*8050.0;
    auto totalMassWood = demoVolWood*800.0;

    // Total joules consumed for the transport of all materials
    auto joulesDieselMaterialTransport = totalMassConcrete*materialTransportationIntensityConcrete*concreteTravelDistance +
            totalMassSteel*materialTransportationIntensitySteel*steelTravelDistance +
            totalMassWood*materialTransportationIntensityWood*woodTravelDistance;

    JoulesDieselHeavyDuty += joulesDieselMaterialTransport;

    // Save the energy responses
    theEnergyResponse->setCurrentValue("kWhElectricity",kWhElectricity);
    theEnergyResponse->setCurrentValue("JoulesDieselLightMediumDutyTransport",JoulesDieselLightMedDuty);
    theEnergyResponse->setCurrentValue("JoulesDieselHeavyDutyTransport",JoulesDieselHeavyDuty);
    theEnergyResponse->setCurrentValue("JoulesDieselEquipmentUsage",JoulesDieselEquipment);
    theEnergyResponse->setCurrentValue("JoulesGasolineLightMediumDutyTransport",JoulesGasolineLightMedDuty);
    theStatsMethod->update(theEnergyResponse->objectName()+"kWhElectricity",kWhElectricity);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesDieselLightMediumDutyTransport",JoulesDieselLightMedDuty);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesDieselHeavyDutyTransport",JoulesDieselHeavyDuty);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesDieselEquipmentUsage",JoulesDieselEquipment);
    theStatsMethod->update(theEnergyResponse->objectName()+"JoulesGasolineLightMediumDutyTransport",JoulesGasolineLightMedDuty);

    // Give output if requested
    if (theOutputLevel == RObject::Medium) {
        qDebug() << "After the demolition phase the cost is" << accumulatedDirectCost;
    }

    return 1;
}


int RStevesDemolitionImpactModel::resetTime()
{
    labourQuantityCSICodes.clear();
    theScheduler->reset();

    return 0;
}


void RStevesDemolitionImpactModel::createDatabaseOfRandomVariables()
{
    // Random variable for machinery use intensity
    theDomain->createObject(objectName() + "HeavyMachineryUseIntensity", "RContinuousRandomVariable");
    theHeavyMachineryUseIntensity = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theHeavyMachineryUseIntensity->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theHeavyMachineryUseIntensity->setMean(QString("2680000"));
    theHeavyMachineryUseIntensity->setCurrentValue(2680000);
    theHeavyMachineryUseIntensity->setCoefficientOfVariation("0.2");

    // Average of medium and heavy duty transport
    theDomain->createObject(objectName() + "MaterialTransportationIntensityConcrete", "RContinuousRandomVariable");
    theMaterialTransportationIntensityConcrete = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theMaterialTransportationIntensityConcrete->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theMaterialTransportationIntensityConcrete->setMean(QString("4855"));
    theMaterialTransportationIntensityConcrete->setCurrentValue(4855);
    theMaterialTransportationIntensityConcrete->setCoefficientOfVariation("0.1");

    // Transportation sector energy intensities for worker and material transport
    // British Columbia and Territories
    // https://oee.nrcan.gc.ca/corporate/statistics/neud/dpa/showTable.cfm?type=CP&sector=tran&juris=bct&rn=36&page=0
    theDomain->createObject(objectName() + "MaterialTransportationIntensitySteel", "RContinuousRandomVariable");
    theMaterialTransportationIntensitySteel = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theMaterialTransportationIntensitySteel->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theMaterialTransportationIntensitySteel->setMean(QString("2200"));
    theMaterialTransportationIntensitySteel->setCurrentValue(2200);
    theMaterialTransportationIntensitySteel->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "MaterialTransportationIntensityWood", "RContinuousRandomVariable");
    theMaterialTransportationIntensityWood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theMaterialTransportationIntensityWood->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theMaterialTransportationIntensityWood->setMean(QString("6110"));
    theMaterialTransportationIntensityWood->setCurrentValue(6110);
    theMaterialTransportationIntensityWood->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "WorkerTransportationIntensityAutomobil", "RContinuousRandomVariable");
    theWorkerTransportationIntensityAutomobil = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theWorkerTransportationIntensityAutomobil->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theWorkerTransportationIntensityAutomobil->setMean(QString("2730000"));
    theWorkerTransportationIntensityAutomobil->setCurrentValue(2730000);
    theWorkerTransportationIntensityAutomobil->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "WorkerTransportationIntensityDieselbus", "RContinuousRandomVariable");
    theWorkerTransportationIntensityDieselbus = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theWorkerTransportationIntensityDieselbus->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theWorkerTransportationIntensityDieselbus->setMean(QString("920000"));
    theWorkerTransportationIntensityDieselbus->setCurrentValue(920000);
    theWorkerTransportationIntensityDieselbus->setCoefficientOfVariation("0.1");

    theDomain->createObject(objectName() + "WorkerTransportationIntensitySkytrain", "RContinuousRandomVariable");
    theWorkerTransportationIntensitySkytrain = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theWorkerTransportationIntensitySkytrain->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theWorkerTransportationIntensitySkytrain->setMean(QString("390000"));
    theWorkerTransportationIntensitySkytrain->setCurrentValue(390000);
    theWorkerTransportationIntensitySkytrain->setCoefficientOfVariation("0.1");

    // Coefficient of variation for the disposal and recycling costs
    QString cov = "0.20";

    // Create the disposal and recycling costs random variables
    theDomain->createObject(objectName() + "theDirectCostLandfillDisposalWood", "RContinuousRandomVariable");
    theDirectCostLandfillDisposalWood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theDirectCostLandfillDisposalWood->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theDirectCostLandfillDisposalWood->setMean(QString("70.0"));
    theDirectCostLandfillDisposalWood->setCurrentValue(70.0);
    theDirectCostLandfillDisposalWood->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "theDirectCostLandfillDisposalOther", "RContinuousRandomVariable");
    theDirectCostLandfillDisposalOther = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theDirectCostLandfillDisposalOther->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theDirectCostLandfillDisposalOther->setMean(QString("90.0"));
    theDirectCostLandfillDisposalOther->setCurrentValue(90.0);
    theDirectCostLandfillDisposalOther->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "theDirectCostRecyclingConcrete", "RContinuousRandomVariable");
    theDirectCostRecyclingConcrete = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theDirectCostRecyclingConcrete->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theDirectCostRecyclingConcrete->setMean(QString("32.0"));
    theDirectCostRecyclingConcrete->setCurrentValue(32.0);
    theDirectCostRecyclingConcrete->setCoefficientOfVariation(cov);

    theDomain->createObject(objectName() + "theDirectCostRecyclingSteel", "RContinuousRandomVariable");
    theDirectCostRecyclingSteel = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theDirectCostRecyclingSteel->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theDirectCostRecyclingSteel->setMean(QString("110.0"));
    theDirectCostRecyclingSteel->setCurrentValue(110.0);
    theDirectCostRecyclingSteel->setCoefficientOfVariation(cov);

}
