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

#include "RJob.h"
#include "RLabourer.h"
#include "REquipment.h"
#include <memory>

RJob::RJob()
{
    tonsReinfNo3toNo7 = 0.0;
    tonsReinfNo8toNo18 = 0.0;
    YrdConcrete = 0.0;
    YrdWood = 0.0;
    cfEpoxy = 0.0;
}


RJob::~RJob()
{

}


RCrew* RJob::addCrew(const RCrewCode crewCode)
{
    for(auto& it :crewsOnJob)
    {
        if(it->getCrewCode() == crewCode)
            return it.get();
    }

    //If we got this far then we need to create a new crew
    auto crew = std::make_shared<RCrew>(this->createCrew(crewCode));

    crewsOnJob.push_back(crew);

    return crew.get();
}


void RJob::reset(void)
{
    crewsOnJob.clear();
    tonsReinfNo3toNo7 = 0.0;
    tonsReinfNo8toNo18 = 0.0;
    YrdConcrete = 0.0;
    YrdWood = 0.0;
    cfEpoxy = 0.0;
}


double& RJob::getTonsReinfNo3toNo7()
{
    return tonsReinfNo3toNo7;
}


double& RJob::getTonsReinfNo8toNo18()
{
    return tonsReinfNo8toNo18;
}


double& RJob::getYrdConcrete()
{
    return YrdConcrete;
}


double& RJob::getYrdWood()
{
    return YrdWood;
}


double& RJob::getCfEpoxy()
{
    return cfEpoxy;
}


RCrew* RJob::addCrew(const std::shared_ptr<RCrew>& crew)
{
    for(auto& it :crewsOnJob)
    {
        if(it->getCrewCode() == crew->getCrewCode())
        {
            auto taskList = crew->getTheTaskVector();

            for(auto&& task : taskList)
                it->addTask(task);

            return it.get();
        }
    }

    crewsOnJob.push_back(crew);

    return  crew.get();
}


const std::vector<std::shared_ptr<RCrew>>& RJob::getCrewsOnJob() const
{
    return crewsOnJob;
}


QString RJob:: getJobCode() const
{
    return jobCode;
}


void RJob::setJobCode(const QString &value)
{
    jobCode = value;
}


RCrew RJob::createCrew(const RCrewCode& crewCode)
{
    RCrew crew;

    switch(crewCode)
    {
    case RCrewCode::A3N :
    {
        // Labour and Equipment for crew C1
        // 1 Equip. Oper. (crane)
        // 1 Tower Crane (monthly)

        RLabourer EquipmentOperatorCrane(RLabourerType::EquipmentOperatorCrane);

        crew.addLabourer(EquipmentOperatorCrane, 1);

        REquipment TowerCrane(REquipmentType::TowerCrane);

        crew.addEquipment(TowerCrane,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::B2 :
    {
        // Crew B-2
        // 1 Labor Foreman (outside)
        // 4 Laborers


        RLabourer foreman(RLabourerType::LabourForeman);
        RLabourer labourer(RLabourerType::Laborer);

        crew.addLabourer(foreman, 1);
        crew.addLabourer(labourer, 4);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::B3 :
    {
        // Crew B-3
        // 1 Labor Foreman (outside)
        // 2 Laborers
        // 1 Equip. Oper. (medium)
        // 2 Truck Drivers (heavy)
        // 1 Crawler Loader, 3 C.Y.
        // 2 Dump Trucks, 12 C.Y., 400 H.P.

        RLabourer foreman(RLabourerType::LabourForeman);
        RLabourer labourer(RLabourerType::Laborer);
        RLabourer equipOpMed(RLabourerType::EquipmentOperatorMedium);
        RLabourer truckDriverHeavy(RLabourerType::TruckDriverHeavy);

        crew.addLabourer(foreman, 1);
        crew.addLabourer(labourer, 2);
        crew.addLabourer(equipOpMed, 1);
        crew.addLabourer(truckDriverHeavy, 2);

        crew.addEquipment(CrawlerLoader,1);
        crew.addEquipment(DumpTruck12cy,2);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::B8 :
    {
        // 1 Labor Foreman (outside)
        // 2 Laborers
        // 2 Equip. Oper. (medium)
        // 1 Equip. Oper. (oiler)
        // 2 Truck Drivers (heavy)
        // 1 Hyd. Crane, 25 Ton
        // 1 Crawler Loader, 3 C.Y.
        // 2 Dump Trucks, 12 C.Y., 400 H.P.

        RLabourer foreman(RLabourerType::LabourForeman);
        RLabourer labourer(RLabourerType::Laborer);
        RLabourer equipOpMed(RLabourerType::EquipmentOperatorMedium);
        RLabourer equipOpOiler(RLabourerType::EquipmentOperatorOiler);
        RLabourer truckDriverHeavy(RLabourerType::TruckDriverHeavy);

        crew.addLabourer(foreman, 1);
        crew.addLabourer(labourer, 2);
        crew.addLabourer(equipOpMed, 2);
        crew.addLabourer(equipOpOiler, 1);
        crew.addLabourer(truckDriverHeavy, 2);

        crew.addEquipment(HydCrane25Ton,1);
        crew.addEquipment(CrawlerLoader,1);
        crew.addEquipment(DumpTruck12cy,2);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::B9 :
    {
        // 1 Labor Foreman (outside)
        // 4 Laborers
        // 1 Air Compressor, 250 cfm
        // 2 Breakers, Pavement, 60 lb.
        // 2 50’ Air Hoses, 1.5􏰀

        RLabourer foreman(RLabourerType::LabourForeman);
        RLabourer labourer(RLabourerType::Laborer);

        crew.addLabourer(foreman, 1);
        crew.addLabourer(labourer, 4);

        crew.addEquipment(AirCompressor250cfm,1);
        crew.addEquipment(ConcreteBreaker60lb,2);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::B89 :
    {
        // Crew B-89
        // 1 Equip. Oper. (light)
        // 1 Truck Driver (light)
        // 1 Flatbed Truck, Gas, 3 Ton
        // 1 Concrete Saw
        // 1 Water Tank, 65 Gal.

        RLabourer equipOpLight(RLabourerType::EquipmentOperatorLight);
        RLabourer truckDriverLight(RLabourerType::TruckDriverLight);

        crew.addLabourer(equipOpLight, 1);
        crew.addLabourer(truckDriverLight, 1);

        crew.addEquipment(ConcreteSaw,1);
        crew.addEquipment(FlatbedTruck3ton,1);
        crew.addEquipment(WaterTank65gal,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::B89B :
    {
        // Crew B-89B
        // 1 Equip. Oper. (light)
        // 1 Truck Driver (light)
        // 1 Wall Saw, Hydraulic, 10 H.P.
        // 1 Generator, Diesel, 100 kW
        // 1 Water Tank, 65 Gal.
        // 1 Flatbed Truck, Gas, 3 Ton

        RLabourer equipOpLight(RLabourerType::EquipmentOperatorLight);
        RLabourer truckDriverLight(RLabourerType::TruckDriverLight);

        crew.addLabourer(equipOpLight, 1);
        crew.addLabourer(truckDriverLight, 1);

        crew.addEquipment(ConcreteWallSawHydraulic,1);
        crew.addEquipment(GeneratorDiesel100kw,1);
        crew.addEquipment(WaterTank65gal,1);
        crew.addEquipment(FlatbedTruck3ton,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C1 :
    {
        // Labour and Equipment for crew C1
        // 1 laborers
        // 3 Carpenters

        RLabourer carpenter(RLabourerType::Carpenter);
        RLabourer labourer(RLabourerType::Laborer);

        crew.addLabourer(carpenter, 3);
        crew.addLabourer(labourer, 1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C2 :
    {
        // The labour/equipment requirements for crew C2
        // 1 Carpenter Foreman (outside)
        // 4 Carpenters
        // 1 Laborer

        RLabourer carpenterForeman(RLabourerType::CarpenterForeman);
        RLabourer carpenter(RLabourerType::Carpenter);
        RLabourer labourer(RLabourerType::Laborer);

        crew.addLabourer(carpenterForeman, 1);
        crew.addLabourer(carpenter, 4);
        crew.addLabourer(labourer, 1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C5 :
    {
        // Labour and Equipment for crew C5
        // 1 Rodman Foreman (outside)
        // 4 Rodman (reinf.)
        // 1 Equip. Oper. (crane)
        // 1 Equip. Oper. (oiler)
        // 1 Hyd. Crane, 25 Ton

        RLabourer RodmanForeman(RLabourerType::RodmanForeman);
        RLabourer Rodman(RLabourerType::Rodman);
        RLabourer EquipmentOperatorCrane(RLabourerType::EquipmentOperatorCrane);
        RLabourer equipmentOperatorOiler(RLabourerType::EquipmentOperatorOiler);

        crew.addLabourer(RodmanForeman, 1);
        crew.addLabourer(Rodman, 4);
        crew.addLabourer(EquipmentOperatorCrane, 1);
        crew.addLabourer(equipmentOperatorOiler, 1);

        REquipment HydCrane25Ton(REquipmentType::HydCrane25Ton);

        crew.addEquipment(HydCrane25Ton,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C6 :
    {
        // Crew C-6
        // 1 Labor Foreman (outside)
        // 4 Laborers
        // 1 Cement Finisher
        // 2 Gas Engine Vibrators

        RLabourer labourForeman(RLabourerType::LabourForeman);
        RLabourer labourer(RLabourerType::Laborer);
        RLabourer cementFinisher(RLabourerType::CementFinisher);

        crew.addLabourer(labourForeman, 1);
        crew.addLabourer(labourer, 4);
        crew.addLabourer(cementFinisher, 1);

        REquipment gasEngineVibrator(REquipmentType::GasEngineVibrator);

        crew.addEquipment(gasEngineVibrator,2);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C7 :
    {
        // Labour and Equipment for crew C7
        // 1 Labor Foreman (outside)
        // 5 Laborers
        // 1 Cement Finisher
        // 1 Equip. Oper. (medium)
        // 1 Equip. Oper. (oiler)
        // 2 Gas Engine Vibrators
        // 1 Concrete Bucket, 1 C.Y.
        // 1 Hyd. Crane, 55 Ton

        RLabourer labourForeman(RLabourerType::LabourForeman);
        RLabourer labourer(RLabourerType::Laborer);
        RLabourer cementFinisher(RLabourerType::CementFinisher);
        RLabourer equipmentOperatorMed(RLabourerType::EquipmentOperatorMedium);
        RLabourer equipmentOperatorOiler(RLabourerType::EquipmentOperatorOiler);

        crew.addLabourer(labourForeman, 1);
        crew.addLabourer(labourer, 5);
        crew.addLabourer(cementFinisher, 1);
        crew.addLabourer(equipmentOperatorMed, 1);
        crew.addLabourer(equipmentOperatorOiler, 1);

        REquipment gasEngineVibrator(REquipmentType::GasEngineVibrator);
        REquipment ConcretBucket1CY(REquipmentType::ConcretBucket1CY);
        REquipment HydCrane55Ton(REquipmentType::HydCrane55Ton);

        crew.addEquipment(gasEngineVibrator,2);
        crew.addEquipment(ConcretBucket1CY,1);
        crew.addEquipment(HydCrane55Ton,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C8 :
    {
        // Labour and equipment for crew C8
        // 1 Labor Foreman (outside)
        // 3  Laborers
        // 2 Cement Finishers
        // 1 Equip. Oper. (medium)
        // 1 Concrete Pump (Small)

        RLabourer labourForeman(RLabourerType::LabourForeman);
        RLabourer labourer(RLabourerType::Laborer);
        RLabourer cementFinisher(RLabourerType::CementFinisher);
        RLabourer equipmentOperatorMed(RLabourerType::EquipmentOperatorMedium);

        crew.addLabourer(labourForeman, 1);
        crew.addLabourer(labourer, 3);
        crew.addLabourer(cementFinisher, 2);
        crew.addLabourer(equipmentOperatorMed, 1);

        REquipment concPump(REquipmentType::ConcretePumpSmall);

        crew.addEquipment(concPump,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C10 :
    {
        // Labour and equipment for crew C10
        // 1 Laborer
        // 2 Cement Finishers

        RLabourer labourer(RLabourerType::Laborer);
        RLabourer cementFinisher(RLabourerType::CementFinisher);

        crew.addLabourer(labourer, 1);
        crew.addLabourer(cementFinisher, 2);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C10C :
    {
        // Labour and equipment for crew C10C
        // 1 Laborer
        // 2 Cement Finishers
        // 1 Walk-Behind Trowel

        RLabourer labourer(RLabourerType::Laborer);
        RLabourer cementFinisher(RLabourerType::CementFinisher);

        crew.addLabourer(labourer, 1);
        crew.addLabourer(cementFinisher, 2);

        REquipment WalkBehindTrowel(REquipmentType::WalkBehindTrowel);

        crew.addEquipment(WalkBehindTrowel,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C16 :
    {
        // 1 Labor Foreman (outside)
        // 3 Laborers
        // 2 Cement Finishers
        // 1 Equip. Oper. (medium)
        // 1 Gunite Pump Rig
        // 2 -50’ Air Hoses, 3/4􏰀"
        // 2 -50’ Air Hoses, 2􏰀"

        RLabourer labourForeman(RLabourerType::LabourForeman);
        RLabourer labourer(RLabourerType::Laborer);
        RLabourer cementFinisher(RLabourerType::CementFinisher);
        RLabourer equipmentOperatorMed(RLabourerType::EquipmentOperatorMedium);

        crew.addLabourer(labourForeman, 1);
        crew.addLabourer(labourer, 3);
        crew.addLabourer(cementFinisher, 2);
        crew.addLabourer(equipmentOperatorMed, 1);

        REquipment gunitePump(REquipmentType::GunitePump);

        crew.addEquipment(gunitePump,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C18 :
    {
        // 0.125 Labor Foreman (outside) - assume 1
        // 1 Laborers
        // 1 Concrete Cart, 10 C.F.

        RLabourer labourForeman(RLabourerType::LabourForeman);
        RLabourer labourer(RLabourerType::Laborer);

        crew.addLabourer(labourForeman, 1);
        crew.addLabourer(labourer, 1);

        REquipment concreteCart(REquipmentType::ConcreteCart10CF);

        crew.addEquipment(concreteCart,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C20 :
    {
        // Labour and Equipment for crew C20
        // 1 Labor Foreman (outside)
        // 5 laborers
        // 1 Cement Finisher
        // 1 Equip. Oper. (medium)
        // 2 Gas Engine Vibrators
        // 1 Concrete Pump (Small)

        RLabourer labourForeman(RLabourerType::LabourForeman);
        RLabourer labourer(RLabourerType::Laborer);
        RLabourer cementFinisher(RLabourerType::CementFinisher);
        RLabourer equipmentOperatorMed(RLabourerType::EquipmentOperatorMedium);

        crew.addLabourer(labourForeman, 1);
        crew.addLabourer(labourer, 5);
        crew.addLabourer(cementFinisher, 1);
        crew.addLabourer(equipmentOperatorMed, 1);

        REquipment gasEngineVibrator(REquipmentType::GasEngineVibrator);
        REquipment concretePumpSmall(REquipmentType::ConcretePumpSmall);

        crew.addEquipment(gasEngineVibrator,2);
        crew.addEquipment(concretePumpSmall,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::C25 :
    {
        // Labour and Equipment for crew C25
        // 2 Rodman
        // 2 Rodman Helpers

        RLabourer rodman(RLabourerType::Rodman);
        RLabourer RodmanHelper(RLabourerType::RodmanHelper);

        crew.addLabourer(rodman, 2);
        crew.addLabourer(RodmanHelper, 2);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::Carp1 :
    {
        // Labour and Equipment
        // 1 Carpenter (Carp)

        RLabourer carpenter(RLabourerType::Carpenter);

        crew.addLabourer(carpenter, 1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::Cefi1 :
    {
        // Labour and Equipment for crew Cefi
        // 1 Cement Finisher (Cefi)

        RLabourer CementFinisher(RLabourerType::CementFinisher);

        crew.addLabourer(CementFinisher, 1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::CLAB1 :
    {
        // Labour and Equipment for crew CLAB
        // 1 Laborers (CLAB)

        RLabourer concreteLaborer(RLabourerType::ConcreteLaborer);

        crew.addLabourer(concreteLaborer, 1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::CLAB2 :
    {
        // Labour and Equipment for crew CLAB2
        // 2 Laborers (CLAB)

        RLabourer concreteLaborer(RLabourerType::ConcreteLaborer);

        crew.addLabourer(concreteLaborer, 2);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::E2 :
    {
        // Labour and Equipment for crew E2
        // 1 Struc. Steel Foreman (outside)
        // 4 Struc. Steel Workers
        // 1 Equip. Oper. (crane)
        // 1 Equip. Oper. (oiler)
        // 1 Lattice Boom Crane, 90 Ton

        RLabourer steelForeman(RLabourerType::SteelForeman);
        RLabourer steelWorker(RLabourerType::SteelWorker);
        RLabourer EquipmentOperatorCrane(RLabourerType::EquipmentOperatorCrane);
        RLabourer equipmentOperatorOiler(RLabourerType::EquipmentOperatorOiler);

        crew.addLabourer(steelForeman, 1);
        crew.addLabourer(steelWorker, 4);
        crew.addLabourer(EquipmentOperatorCrane, 1);
        crew.addLabourer(equipmentOperatorOiler, 1);

        REquipment LatticeCrane(REquipmentType::LatticeBoomCrane90Ton);

        crew.addEquipment(LatticeCrane,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::E4 :
    {
        // Labour and Equipment for crew E4
        // 1 Struc. Steel Foreman (outside)
        // 3 Struc. Steel Workers
        // 1 Welder, Gas Engine, 300 amp

        RLabourer steelForeman(RLabourerType::SteelForeman);
        RLabourer steelWorker(RLabourerType::SteelWorker);

        crew.addLabourer(steelForeman, 1);
        crew.addLabourer(steelWorker, 3);

        REquipment welder(REquipmentType::Welder300AmpGas);

        crew.addEquipment(welder,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::E18 :
    {
        // Labour and Equipment for crew E18
        // 1 Struc. Steel Foreman (outside)
        // 3 Structural Steel Workers
        // 1 Equipment Operator (med.)
        // 1 Lattice Boom Crane, 20 Ton

        RLabourer steelForeman(RLabourerType::SteelForeman);
        RLabourer steelWorker(RLabourerType::SteelWorker);
        RLabourer EquipmentOperatorMed(RLabourerType::EquipmentOperatorMedium);

        crew.addLabourer(steelForeman, 1);
        crew.addLabourer(steelWorker, 3);
        crew.addLabourer(EquipmentOperatorMed, 1);

        REquipment LatticeCrane(REquipmentType::LatticeBoomCrane20Ton);

        crew.addEquipment(LatticeCrane,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::E19 :
    {
        // Labour and Equipment for Crew E-19
        // 1 Struc. Steel Foreman (outside)
        // 1 Structural Steel Worker
        // 1 Equip. Oper. (light)
        // 1 Lattice Boom Crane, 20 Ton

        RLabourer steelForeman(RLabourerType::SteelForeman);
        RLabourer steelWorker(RLabourerType::SteelWorker);
        RLabourer EquipmentOperatorLight(RLabourerType::EquipmentOperatorLight);

        crew.addLabourer(steelForeman, 1);
        crew.addLabourer(steelWorker, 1);
        crew.addLabourer(EquipmentOperatorLight, 1);

        REquipment LatticeCrane(REquipmentType::LatticeBoomCrane20Ton);

        crew.addEquipment(LatticeCrane,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::E20 :
    {
        // Labour and Equipment for Crew E-20
        // 1 Struc. Steel Foreman (outside)
        // 5 Structural Steel Workers
        // 1 Equip. Oper. (crane)
        // 1 Equip. Oper. (oiler)
        // 1 Lattice Boom Crane, 40 Ton

        RLabourer steelForeman(RLabourerType::SteelForeman);
        RLabourer steelWorker(RLabourerType::SteelWorker);
        RLabourer EquipmentOperatorCrane(RLabourerType::EquipmentOperatorCrane);
        RLabourer equipmentOperatorOiler(RLabourerType::EquipmentOperatorOiler);

        crew.addLabourer(steelForeman, 1);
        crew.addLabourer(steelWorker, 5);
        crew.addLabourer(EquipmentOperatorCrane, 1);
        crew.addLabourer(equipmentOperatorOiler, 1);

        REquipment LatticeCrane(REquipmentType::LatticeBoomCrane40Ton);

        crew.addEquipment(LatticeCrane,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::E24 :
    {
        // Labour and Equipment for Crew E-24
        // 3 Structural Steel Workers
        // 1 Equipment Operator (med.)
        // 1 Hyd. Crane, 25 Ton

        RLabourer steelWorker(RLabourerType::SteelWorker);
        RLabourer EquipmentOperatorMed(RLabourerType::EquipmentOperatorMedium);

        crew.addLabourer(steelWorker, 3);
        crew.addLabourer(EquipmentOperatorMed, 1);

        REquipment HydCrane(REquipmentType::HydCrane25Ton);

        crew.addEquipment(HydCrane,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::EPXY :
    {
        // Labour and Equipment for crew EPXY
        // 2 Laborers

        RLabourer laborer(RLabourerType::Laborer);

        crew.addLabourer(laborer, 2);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::F3 :
    {
        // Labour and Equipment for crew F3
        // 4 Carpenters
        // Equip. Oper. (crane)
        // Hyd. Crane, 12 Ton

        RLabourer carpenter(RLabourerType::Carpenter);
        RLabourer EquipmentOperatorCrane(RLabourerType::EquipmentOperatorCrane);
        REquipment HydCrane12Ton(REquipmentType::HydCrane12Ton);

        crew.addLabourer(carpenter, 4);
        crew.addLabourer(EquipmentOperatorCrane, 1);

        crew.addEquipment(HydCrane12Ton,1);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::Rodm2 :
    {
        // Labour and Equipment for crew Rodm2
        // 2 Rodman

        RLabourer rodman(RLabourerType::Rodman);

        crew.addLabourer(rodman, 2);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::Rodm4 :
    {
        // Labour and Equipment for crew Rodm4
        // 4 Rodman

        RLabourer rodman(RLabourerType::Rodman);

        crew.addLabourer(rodman, 4);

        crew.setTheCrewCode(crewCode);

        break;
    }
    case RCrewCode::Skwk2 :
    {
        // Labour and Equipment
        // 2 Skilled workers (skwk)

        RLabourer skwk(RLabourerType::SkilledWorker);

        crew.addLabourer(skwk, 2);

        crew.setTheCrewCode(crewCode);

        break;
    }
    }

    return crew;
}


size_t RJob::getNumberOfCrews(void)
{
    return crewsOnJob.size();
}


