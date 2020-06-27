#*********************************************************************
#                                                                    *
# This file is posted by Dr. Stevan Gavrilovic (steva44@hotmail.com) *
# as a result of work performed in the research group of Professor   *
# Terje Haukaas (terje@civil.ubc.ca) at the University of British    *
# Columbia in Vancouver. The code is part of the computer program    *
# Rts, which is an extension of the computer program Rt developed by *
# Dr. Mojtaba Mahsuli (mahsuli@sharif.edu) in the same group.        *
#                                                                    *
# The content of this file is the product of thesis-related work     *
# performed at the University of British Columbia (UBC). Therefore,  *
# the Rights and Ownership are held by UBC.                          *
#                                                                    *
# Please be cautious when using this code. It is provided “as is”    *
# without warranty of any kind, express or implied.                  *
#                                                                    *
# Contributors to this file:                                         *
#   - Stevan Gavrilovic                                              *
#   - Terje Haukaas                                                  *
#                                                                    *
#********************************************************************/

####################################################################
#                                                                  #
#                DETAILED BUILDINGS PROJECT FILE                   #
#                                                                  #
####################################################################

INCLUDEPATH +=  ./Extended/DetailedBuildings \
                ./Extended/DetailedBuildings/Components \
                ./Extended/DetailedBuildings/Components/Components \
                ./Extended/DetailedBuildings/RDomainAdd \
                ./Extended/DetailedBuildings/Models \
                ./Extended/DetailedBuildings/Methods \
                ./Extended/DetailedBuildings/Tools \
                ./Extended/DetailedBuildings/VisualDamage \
                ./Extended/DetailedBuildings/VisualDamage/Topological \
                ./Extended/DetailedBuildings/Tools/Database \
                ./Extended/DetailedBuildings/BIM \
                ./Extended/DetailedBuildings/BIM/IFCConversionMethods \
                ./Extended/DetailedBuildings/Tools/Database/SteelSectionDatabase \
	

HEADERS +=  ./Extended/DetailedBuildings/Components/RComponent.h \
            ./Extended/DetailedBuildings/Components/RConcreteComponent.h \
            ./Extended/DetailedBuildings/Components/RBeamColumnComponent.h \
            ./Extended/DetailedBuildings/Components/RColumnComponent.h \
            ./Extended/DetailedBuildings/Components/RTrussComponent.h \
            ./Extended/DetailedBuildings/Components/RCLTPanelComponent.h \
            ./Extended/DetailedBuildings/Components/RCompositePanelComponent.h \
            ./Extended/DetailedBuildings/Components/RHSSRoundBeamColumnComponent.h \
            ./Extended/DetailedBuildings/Components/RRectangularGlulamBeamColumnComponent.h \
            ./Extended/DetailedBuildings/Components/RPlateComponent.h \
            ./Extended/DetailedBuildings/Components/RWallComponent.h \
            ./Extended/DetailedBuildings/Components/RWindowComponent.h \
            ./Extended/DetailedBuildings/Components/RRectangularRCColumnComponent.h \
            ./Extended/DetailedBuildings/Components/RCircularRCColumnComponent.h \
            ./Extended/DetailedBuildings/Components/RNonLoadBearingWallComponent.h \
            ./Extended/DetailedBuildings/Components/RRCShearWallComponent.h \
            ./Extended/DetailedBuildings/VisualDamage/RShellSectionDamageRegion.h \
            ./Extended/DetailedBuildings/VisualDamage/RFrameDamageMesh.h \
            ./Extended/DetailedBuildings/VisualDamage/RRCFibreContainer.h \
            ./Extended/DetailedBuildings/VisualDamage/RWSectionDamageRegion.h \
            ./Extended/DetailedBuildings/VisualDamage/RQuadRCSectionDamageRegion.h \
            ./Extended/DetailedBuildings/VisualDamage/RQuadSectionDamageRegion.h \
            ./Extended/DetailedBuildings/VisualDamage/RGenericFibreContainer.h \
            ./Extended/DetailedBuildings/VisualDamage/RFibreDamageRegion.h \
            ./Extended/DetailedBuildings/VisualDamage/RDamageRegion.h \
            ./Extended/DetailedBuildings/VisualDamage/RShellDamageMesh.h \
            ./Extended/DetailedBuildings/VisualDamage/RDamageMesh.h \
            ./Extended/DetailedBuildings/VisualDamage/RRCVisualDamage.h \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoVertex.h \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoMesh.h \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoEdge.h \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoFace.h \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoPlotter.h \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoUtils.h \
            ./Extended/DetailedBuildings/Components/RRCSlabComponent.h \
            ./Extended/DetailedBuildings/Components/RStandardSteelComponent.h \
            ./Extended/DetailedBuildings/Components/RWSteelBeamColumnComponent.h \
            ./Extended/DetailedBuildings/Models/RGurvinderDirectConstructionCostModel.h  \
            ./Extended/DetailedBuildings/Models/REnergyUseInOperationModel.h  \
            ./Extended/DetailedBuildings/Models/RGurvinderDemolitionCostModel.h  \
            ./Extended/DetailedBuildings/Models/RShindellEnvironmentalCostOfEnergyModel.h  \
            ./Extended/DetailedBuildings/Models/RCostOfEmissionsModel.h  \
            ./Extended/DetailedBuildings/Models/REmissionsFromEnergyModel.h  \
            ./Extended/DetailedBuildings/Models/RCostOfEnergyModel.h  \
            ./Extended/DetailedBuildings/Models/RCostOfWaterModel.h  \
            ./Extended/DetailedBuildings/Models/RCostOfDowntimeModel.h  \
            ./Extended/DetailedBuildings/Models/REnergyUseInManufacturingModel.h  \
            ./Extended/DetailedBuildings/Models/REnergyUseInTransportationModel.h  \
            ./Extended/DetailedBuildings/Models/REnergyUseInConstructionModel.h  \
            ./Extended/DetailedBuildings/Models/RSeadonDirectConstructionCostModel.h  \
            ./Extended/DetailedBuildings/Models/RSeadonPreventativeMaintenanceModel.h  \
            ./Extended/DetailedBuildings/Models/RDirectCostOfMaterialsModel.h  \
            ./Extended/DetailedBuildings/Models/RRepairManagerModel.h \
            ./Extended/DetailedBuildings/Models/RStevesManufacturingImpactModel.h \
            ./Extended/DetailedBuildings/Models/RStevesConstructionImpactModel.h \
            ./Extended/DetailedBuildings/Models/RStevesRepairImpactModel.h \
            ./Extended/DetailedBuildings/Models/RStevesOperationImpactModel.h \
            ./Extended/DetailedBuildings/Models/RStevesDemolitionImpactModel.h \
            ./Extended/DetailedBuildings/Models/RStevesHumanImpactModel.h \
            ./Extended/DetailedBuildings/Models/RVisualDamageModel.h \
            ./Extended/DetailedBuildings/Models/RBuildingPushoverModel.h \
            ./Extended/DetailedBuildings/Models/RDisplacementControlledPushoverModel.h \
            ./Extended/DetailedBuildings/Models/RTimeHistoryScalingPlotModel.h \
            ./Extended/DetailedBuildings/Models/RTimeHistoryEvolutionPlotModel.h \
            ./Extended/DetailedBuildings/Tools/RCSICostMethod.h \
            ./Extended/DetailedBuildings/Tools/RCSISchedulingMethod.h \
            ./Extended/DetailedBuildings/Tools/RCSIAssembler.h \
            ./Extended/DetailedBuildings/Tools/RCrew.h \
            ./Extended/DetailedBuildings/Tools/REquipment.h \
            ./Extended/DetailedBuildings/Tools/RJob.h \
            ./Extended/DetailedBuildings/Tools/RLabourer.h \
            ./Extended/DetailedBuildings/Tools/RTask.h \
            ./Extended/DetailedBuildings/Tools/Database/SteelSectionDatabase/RSteelSectionImporter.h \
            ./Extended/DetailedBuildings/BIM/IFCConversionMethods/RIFCtoComponentConverter.h \
            ./Extended/DetailedBuildings/BIM/IFCConversionMethods/RSolidModelImportMethod.h \
            ./Extended/DetailedBuildings/BIM/IFCConversionMethods/RIFCImportData.h \
            ./Extended/DetailedBuildings/BIM/RBIM.h  \
            ./Extended/DetailedBuildings/BIM/RBIMImportMethod.h \


SOURCES +=  ./Extended/DetailedBuildings/Components/RComponent.cpp \
            ./Extended/DetailedBuildings/Components/RConcreteComponent.cpp \
            ./Extended/DetailedBuildings/Components/RBeamColumnComponent.cpp \
            ./Extended/DetailedBuildings/Components/RColumnComponent.cpp \
            ./Extended/DetailedBuildings/Components/RTrussComponent.cpp \
            ./Extended/DetailedBuildings/Components/RCLTPanelComponent.cpp \
            ./Extended/DetailedBuildings/Components/RCompositePanelComponent.cpp \
            ./Extended/DetailedBuildings/Components/RHSSRoundBeamColumnComponent.cpp \
            ./Extended/DetailedBuildings/Components/RRectangularGlulamBeamColumnComponent.cpp \
            ./Extended/DetailedBuildings/Components/RPlateComponent.cpp \
            ./Extended/DetailedBuildings/Components/RWallComponent.cpp \
            ./Extended/DetailedBuildings/Components/RWindowComponent.cpp \
            ./Extended/DetailedBuildings/Components/RRectangularRCColumnComponent.cpp \
            ./Extended/DetailedBuildings/Components/RCircularRCColumnComponent.cpp \
            ./Extended/DetailedBuildings/Components/RNonLoadBearingWallComponent.cpp \
            ./Extended/DetailedBuildings/Components/RRCShearWallComponent.cpp \
            ./Extended/DetailedBuildings/Components/RRCSlabComponent.cpp \
            ./Extended/DetailedBuildings/Components/RStandardSteelComponent.cpp \
            ./Extended/DetailedBuildings/Components/RWSteelBeamColumnComponent.cpp \
            ./Extended/DetailedBuildings/Models/RGurvinderDirectConstructionCostModel.cpp  \
            ./Extended/DetailedBuildings/Models/REnergyUseInOperationModel.cpp  \
            ./Extended/DetailedBuildings/Models/RGurvinderDemolitionCostModel.cpp  \
            ./Extended/DetailedBuildings/Models/RShindellEnvironmentalCostOfEnergyModel.cpp  \
            ./Extended/DetailedBuildings/Models/RCostOfEmissionsModel.cpp  \
            ./Extended/DetailedBuildings/Models/REmissionsFromEnergyModel.cpp  \
            ./Extended/DetailedBuildings/Models/RCostOfEnergyModel.cpp  \
            ./Extended/DetailedBuildings/Models/RCostOfWaterModel.cpp  \
            ./Extended/DetailedBuildings/Models/RCostOfDowntimeModel.cpp  \
            ./Extended/DetailedBuildings/Models/REnergyUseInManufacturingModel.cpp  \
            ./Extended/DetailedBuildings/Models/REnergyUseInTransportationModel.cpp  \
            ./Extended/DetailedBuildings/Models/REnergyUseInConstructionModel.cpp  \
            ./Extended/DetailedBuildings/Models/RSeadonDirectConstructionCostModel.cpp  \
            ./Extended/DetailedBuildings/Models/RSeadonPreventativeMaintenanceModel.cpp  \
            ./Extended/DetailedBuildings/Models/RDirectCostOfMaterialsModel.cpp  \
            ./Extended/DetailedBuildings/Models/RRepairManagerModel.cpp \
            ./Extended/DetailedBuildings/Models/RStevesManufacturingImpactModel.cpp \
            ./Extended/DetailedBuildings/Models/RStevesConstructionImpactModel.cpp \
            ./Extended/DetailedBuildings/Models/RStevesRepairImpactModel.cpp \
            ./Extended/DetailedBuildings/Models/RStevesOperationImpactModel.cpp \
            ./Extended/DetailedBuildings/Models/RStevesDemolitionImpactModel.cpp \
            ./Extended/DetailedBuildings/Models/RStevesHumanImpactModel.cpp \
            ./Extended/DetailedBuildings/Models/RVisualDamageModel.cpp \
            ./Extended/DetailedBuildings/Models/RBuildingPushoverModel.cpp \
            ./Extended/DetailedBuildings/Models/RDisplacementControlledPushoverModel.cpp \
            ./Extended/DetailedBuildings/Models/RTimeHistoryScalingPlotModel.cpp \
            ./Extended/DetailedBuildings/Models/RTimeHistoryEvolutionPlotModel.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RShellSectionDamageRegion.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RFrameDamageMesh.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RRCFibreContainer.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RWSectionDamageRegion.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RQuadRCSectionDamageRegion.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RQuadSectionDamageRegion.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RGenericFibreContainer.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RFibreDamageRegion.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RDamageRegion.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RShellDamageMesh.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RDamageMesh.cpp \
            ./Extended/DetailedBuildings/VisualDamage/RRCVisualDamage.cpp \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoVertex.cpp \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoMesh.cpp \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoEdge.cpp \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoFace.cpp \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoPlotter.cpp \
            ./Extended/DetailedBuildings/VisualDamage/Topological/RTopoUtils.cpp \
            ./Extended/DetailedBuildings/Tools/RCSICostMethod.cpp \
            ./Extended/DetailedBuildings/Tools/RCSISchedulingMethod.cpp \
            ./Extended/DetailedBuildings/Tools/RCSIAssembler.cpp \
            ./Extended/DetailedBuildings/Tools/RCrew.cpp \
            ./Extended/DetailedBuildings/Tools/REquipment.cpp \
            ./Extended/DetailedBuildings/Tools/RJob.cpp \
            ./Extended/DetailedBuildings/Tools/RLabourer.cpp \
            ./Extended/DetailedBuildings/Tools/RTask.cpp \
            ./Extended/DetailedBuildings/Tools/Database/SteelSectionDatabase/RSteelSectionImporter.cpp \
            ./Extended/DetailedBuildings/BIM/RBIM.cpp  \
            ./Extended/DetailedBuildings/BIM/RBIMImportMethod.cpp \
            ./Extended/DetailedBuildings/BIM/IFCConversionMethods/RIFCtoComponentConverter.cpp \
            ./Extended/DetailedBuildings/BIM/IFCConversionMethods/RSolidModelImportMethod.cpp \


HEADERS += ./Extended/DetailedBuildings/RDomainAdd/DetailedBuildingsIncludes.h \
           ./Extended/DetailedBuildings/RDomainAdd/DetailedBuildingsMenu.h \
           ./Extended/DetailedBuildings/RDomainAdd/CreateDetailedBuildingsObjects.h \


RESOURCES += ./Extended/DetailedBuildings/RtsDetailedBuildings.qrc \





