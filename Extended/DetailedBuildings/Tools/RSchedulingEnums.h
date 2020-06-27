#ifndef RSchedulingEnums_H
#define RSchedulingEnums_H
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

// Brief class for the enums used in scheduling methods

// Skwk is skilled worker
// Carp is a carpenter
enum RLabourerType {Carpenter,CarpenterForeman,CementFinisher,ConcreteLaborer,EquipmentOperatorCrane,EquipmentOperatorLight,EquipmentOperatorMedium,EquipmentOperatorOiler,Laborer,LabourForeman,Rodman,RodmanForeman,RodmanHelper,SkilledWorker,SteelForeman,SteelWorker,TruckDriverLight,TruckDriverHeavy,NA};

enum RCrewCode {A3N,B2,B3,B8,B9,B89,B89B,C1,C2,C5,C6,C7,C8,C10,C10C,C16,C18,C20,C25,Carp1,Cefi1,CLAB1,CLAB2,E2,E4,E18,E19,E20,E24,EPXY,F3,Rodm2,Rodm4,Skwk2};

enum REquipmentType {AirCompressor250cfm,ConcretBucket1CY,ConcreteBreaker60lb,ConcreteCart10CF,ConcretePumpSmall,ConcreteSaw,ConcreteWallSawHydraulic,CrawlerLoader,DumpTruck12cy,FlatbedTruck3ton,GasEngineVibrator,GeneratorDiesel100kw,GunitePump,HydCrane12Ton,HydCrane25Ton,HydCrane55Ton,LatticeBoomCrane20Ton,LatticeBoomCrane40Ton,LatticeBoomCrane90Ton,TowerCrane,Welder300AmpGas,WalkBehindTrowel,WaterTank65gal,NS};

#endif // RSchedulingEnums
