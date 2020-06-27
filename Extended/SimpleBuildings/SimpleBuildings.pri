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
#                                                                    *
#********************************************************************/

INCLUDEPATH +=  ./Extended/Buildings \
                ./Extended/SimpleBuildings/RDomainAdd \
                ./Extended/SimpleBuildings/Models \


HEADERS +=  ./Extended/SimpleBuildings/Models/RSimpleBuildingDamageModel.h \
            ./Extended/SimpleBuildings/Models/RSimpleBuildingLossModel.h \
            ./Extended/SimpleBuildings/Models/RSimpleBuildingResponseModel.h \
            # ./Extended/SimpleBuildings/Models/RBuildingDamageModel.h \
            # ./Extended/SimpleBuildings/Models/RBuildingRepairCostModel.h \
            # ./Extended/SimpleBuildings/Models/RGenericDamageAndRestorationModel.h \
            # ./Extended/SimpleBuildings/Models/RHazusBuildingModel.h \
            # ./Extended/SimpleBuildings/Models/RMojtabasRegressionBuildingResponseModel.h \
            # ./Extended/SimpleBuildings/Models/RMojtabasRegressionBuildingDamageModel.h \
            # ./Extended/SimpleBuildings/Models/RMojtabasBuildingRepairCostModel.h \
            # ./Extended/SimpleBuildings/Models/RMojtabasCombinedBuildingModel.h \
            # ./Extended/SimpleBuildings/Models/RRegionalLossModel.h \


SOURCES +=  ./Extended/SimpleBuildings/Models/RSimpleBuildingDamageModel.cpp \
            ./Extended/SimpleBuildings/Models/RSimpleBuildingLossModel.cpp \
            ./Extended/SimpleBuildings/Models/RSimpleBuildingResponseModel.cpp \
            # ./Extended/SimpleBuildings/Models/RBuildingDamageModel.cpp \
            # ./Extended/SimpleBuildings/Models/RBuildingRepairCostModel.cpp \
            # ./Extended/SimpleBuildings/Models/RGenericDamageAndRestorationModel.cpp \
            # ./Extended/SimpleBuildings/Models/RHazusBuildingModel.cpp \
            # ./Extended/SimpleBuildings/Models/RMojtabasRegressionBuildingResponseModel.cpp \
            # ./Extended/SimpleBuildings/Models/RMojtabasRegressionBuildingDamageModel.cpp \
            # ./Extended/SimpleBuildings/Models/RMojtabasBuildingRepairCostModel.cpp \
            # ./Extended/SimpleBuildings/Models/RMojtabasCombinedBuildingModel.cpp \
            # ./Extended/SimpleBuildings/Models/RRegionalLossModel.cpp \


HEADERS += ./Extended/SimpleBuildings/RDomainAdd/SimpleBuildingIncludes.h \
           ./Extended/SimpleBuildings/RDomainAdd/SimpleBuildingMenu.h \
           ./Extended/SimpleBuildings/RDomainAdd/CreateSimpleBuildingObjects.h \


