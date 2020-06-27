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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/


// *******************************************************************************
// *******************************************************************************
//                                     BIM
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RBIM>("RBIM");

// *******************************************************************************
// *******************************************************************************
//                                     MODELS
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RStevesManufacturingImpactModel>("RStevesManufacturingImpactModel");
factory.registerClass<RStevesManufacturingImpactModel>("MODELS Steves Manufacturing");

factory.registerClass<RStevesConstructionImpactModel>("RStevesConstructionImpactModel");
factory.registerClass<RStevesConstructionImpactModel>("MODELS Steves Construction");

factory.registerClass<RVisualDamageModel>("RVisualDamageModel");
factory.registerClass<RVisualDamageModel>("MODELS Visual Damage");

factory.registerClass<RStevesRepairImpactModel>("RStevesRepairImpactModel");
factory.registerClass<RStevesRepairImpactModel>("MODELS Repair Impact");

factory.registerClass<RStevesOperationImpactModel>("RStevesOperationImpactModel");
factory.registerClass<RStevesOperationImpactModel>("MODELS Steves Operation");

factory.registerClass<RStevesDemolitionImpactModel>("RStevesDemolitionImpactModel");
factory.registerClass<RStevesDemolitionImpactModel>("MODELS Steves Demolition");

factory.registerClass<RStevesHumanImpactModel>("RStevesHumanImpactModel");
factory.registerClass<RStevesHumanImpactModel>("MODELS Steves Human Impact");

factory.registerClass<RCostOfEmissionsModel>("RCostOfEmissionsModel");
factory.registerClass<RCostOfEmissionsModel>("MODELS Cost of Emissions");

factory.registerClass<REmissionsFromEnergyModel>("REmissionsFromEnergyModel");
factory.registerClass<REmissionsFromEnergyModel>("MODELS Energy from Emissions");

factory.registerClass<RCostOfWaterModel>("RCostOfWaterModel");
factory.registerClass<RCostOfWaterModel>("MODELS Cost Water");

factory.registerClass<RCostOfDowntimeModel>("RCostOfDowntimeModel");
factory.registerClass<RCostOfDowntimeModel>("MODELS Cost Downtime");

factory.registerClass<RCostOfEnergyModel>("RCostOfEnergyModel");
factory.registerClass<RCostOfEnergyModel>("MODELS Cost Energy");

factory.registerClass<RBuildingPushoverModel>("RBuildingPushoverModel");
factory.registerClass<RBuildingPushoverModel>("MODELS Building Pushover");

factory.registerClass<RDisplacementControlledPushoverModel>("RDisplacementControlledPushoverModel");
factory.registerClass<RDisplacementControlledPushoverModel>("MODELS Displacement Controlled Pushover");

factory.registerClass<RTimeHistoryScalingPlotModel>("RTimeHistoryScalingPlotModel");
factory.registerClass<RTimeHistoryScalingPlotModel>("MODELS Time History Scaling Plot");

factory.registerClass<RTimeHistoryEvolutionPlotModel>("RTimeHistoryEvolutionPlotModel");
factory.registerClass<RTimeHistoryEvolutionPlotModel>("MODELS Time History Evolution Plot");

// *******************************************************************************
// *******************************************************************************
//                                     COMPONENTS
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RNonLoadBearingWallComponent>("RNonLoadBearingWallComponent");
factory.registerClass<RNonLoadBearingWallComponent>("COMPONENTS Non Load Bearing Walls");

factory.registerClass<RCircularRCColumnComponent>("RCircularRCColumnComponent");
factory.registerClass<RCircularRCColumnComponent>("COMPONENTS Circular RC Columns");

factory.registerClass<RRCSlabComponent>("RRCSlabComponent");
factory.registerClass<RRCSlabComponent>("COMPONENTS RC Slabs");

factory.registerClass<RRectangularGlulamBeamColumnComponent>("RRectangularGlulamBeamColumnComponent");
factory.registerClass<RRectangularGlulamBeamColumnComponent>("COMPONENTS Rectangular Timber Beam Columns");

factory.registerClass<RCLTPanelComponent>("RCLTPanelComponent");
factory.registerClass<RCLTPanelComponent>("COMPONENTS CLT Panels");

factory.registerClass<RCompositePanelComponent>("RCompositePanelComponent");
factory.registerClass<RCompositePanelComponent>("COMPONENTS Corrugated Roofs");

factory.registerClass<RWSteelBeamColumnComponent>("RWSteelBeamColumnComponent");
factory.registerClass<RWSteelBeamColumnComponent>("COMPONENTS Steel I Beams");

factory.registerClass<RStandardSteelComponent>("RStandardSteelComponent");
factory.registerClass<RStandardSteelComponent>("COMPONENTS Standard Steel Beams");

factory.registerClass<RRCShearWallComponent>("RRCShearWallComponent");
factory.registerClass<RRCShearWallComponent>("COMPONENTS RC Shear Walls");

factory.registerClass<RWindowComponent>("RWindowComponent");
factory.registerClass<RWindowComponent>("COMPONENTS Window");

factory.registerClass<RRectangularRCColumnComponent>("RRectangularRCColumnComponent");
factory.registerClass<RRectangularRCColumnComponent>("COMPONENTS Rectangular RC Columns");

factory.registerClass<RHSSRoundBeamColumnComponent>("RHSSRoundBeamColumnComponent");
factory.registerClass<RHSSRoundBeamColumnComponent>("COMPONENTS HSS Round Frame Components");

