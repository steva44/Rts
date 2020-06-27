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
#   - Mojtaba Mahsuli                                                *
#   - Stevan Gavrilovic                                              *
#   - Terje Haukaas                                                  *
#   - Rodrigo Costa                                                  *
#                                                                    *
#********************************************************************/

####################################################################
#                                                                  #
#                     RTS CORE PROJECT FILE                        #
#                                                                  #
####################################################################

FORMS += ./Core/GUI/RDialogs/RAboutDialog.ui \
    ./Core/GUI/RDialogs/RModelInferenceDialog.ui \
    ./Core/GUI/RDialogs/RVariableInferenceDialog.ui \
    ./Core/GUI/RDialogs/RTimeSeriesDialog.ui \
    ./Core/GUI/RFlowchartWidget/RFlowchartWidget.ui \
    ./Core/GUI/RObjectWidget/RObjectWidget.ui \
    ./Core/GUI/RPropertyWidget/RPropertyWidget.ui


INCLUDEPATH += . \
        ./Core/GUI \
        ./Core/GUI/RObjectWidget \
        ./Core/GUI/RFlowchartWidget \
        ./Core/GUI/RMapWidget \
        ./Core/GUI/RPropertyWidget \
        ./Core/GUI/RCustomPlotWidget \
        ./Core/GUI/RPropertyWidget/RDelegate \
        ./Core/GUI/RDialogs \
        ./Core/Models \
        ./Core/Models/MuParser \
        ./Core/Parameters \
        ./Core/Methods \
        ./Core/Tools \
        ./Core/Methods/RLinearSolver \
        ./Core/Methods/RProbabilityDistributions \
        ./Core/Methods/RProbabilityTransformation \
        ./Core/Methods/RRandomNumberGenerator \
        ./Core/Methods/RSearchDirection \
        ./Core/Methods/RMeritFunction \
        ./Core/Methods/ROptimizationAlgorithm \
        ./Core/Methods/RConvergenceCheck \
        ./Core/Methods/RCholeskyDecomposition \
        ./Core/Methods/RLineSearchAlgorithm \
        ./Core/Methods/RNonlinearSolver \
        ./GeneratedFiles


HEADERS += ./Core/GUI/RPropertyWidget/RDelegate/QtColorComboBox.h \
    ./Core/GUI/RDialogs/RAboutDialog.h \
    ./Core/GUI/RDialogs/RVariableInferenceDialog.h \
    ./Core/GUI/RDialogs/RModelInferenceDialog.h \
    ./Core/GUI/RDialogs/RTimeSeriesDialog.h \
    ./Core/GUI/RObjectWidget/RAggregationObjectWidget.h \
    ./Core/GUI/RObjectWidget/RObjectWidget.h \
    ./Core/GUI/RPropertyWidget/RDelegate/RCheckBoxTableDelegate.h \
    ./Core/GUI/RPropertyWidget/RDelegate/RColorComboBoxTableDelegate.h \
    ./Core/GUI/RPropertyWidget/RDelegate/RComboBoxTableDelegate.h \
    ./Core/GUI/RPropertyWidget/RDelegate/RDateTimeDelegate.h \
    ./Core/GUI/RFlowchartWidget/RFlowchartWidget.h \
    ./Core/GUI/RCustomPlotWidget/qcustomplot.h \
    ./Core/GUI/RMainWindow.h \
    ./Core/GUI/RPropertyWidget/RPropertyWidget.h \
    ./Core/GUI/RObject.h \
    ./Core/GUI/RDomain.h \
    ./Core/Tools/RMatrixOperations.h \
    ./Core/Tools/RNormalDistribution.h \
    ./Core/Tools/RModelManager.h \
    ./Core/Tools/REuclidsAlgorithm.h \
    ./Core/Tools/RFragilityFunctions.h \
    ./Core/Methods/RMethod.h \
    ./Core/Methods/RLinearSolver/RLinearSolver.h \
    ./Core/Methods/RLinearSolver/RCholeskyLinearSolver.h \
    ./Core/Methods/RLinearSolver/RGaussJordanLinearSolver.h \
    ./Core/Methods/RLinearSolver/RLULinearSolver.h \
    ./Core/Methods/RLinearSolver/RQRLinearSolver.h \
    ./Core/Methods/RLinearSolver/RMatrixInverseLinearSolver.h \
    ./Core/Methods/RLinearSolver/RInHouseLinearSolver.h \
    ./Core/Methods/RProbabilityDistributions/RProbabilityDistributions.h \
    ./Core/Methods/RProbabilityDistributions/RInHouseProbabilityDistributions.h \
    ./Core/Methods/RProbabilityTransformation/RProbabilityTransformation.h \
    ./Core/Methods/RProbabilityTransformation/RNatafProbabilityTransformation.h \
    ./Core/Methods/RProbabilityTransformation/RIndependenceProbabilityTransformation.h \
    ./Core/Methods/RRandomNumberGenerator/RRandomNumberGenerator.h \
    ./Core/Methods/RRandomNumberGenerator/RInHouseRandomNumberGenerator.h \
    ./Core/Methods/RSearchDirection/RSearchDirection.h \
    ./Core/Methods/RSearchDirection/RHLRFSearchDirection.h \
    ./Core/Methods/RSearchDirection/RSteepestDescentSearchDirection.h \
    ./Core/Methods/RSearchDirection/RQuasiNewtonSearchDirection.h \
    ./Core/Methods/RSearchDirection/RComboSearchDirection.h \
    ./Core/Methods/RSearchDirection/RConjugateGradientSearchDirection.h \
    ./Core/Methods/RMeritFunction/RMeritFunction.h \
    ./Core/Methods/RMeritFunction/RADKZhangMeritFunction.h \
    ./Core/Methods/RMeritFunction/RBasicMeritFunction.h \
    ./Core/Methods/RMeritFunction/RDummyMeritFunction.h \
    ./Core/Methods/RMeritFunction/RObjectiveMeritFunction.h \
    ./Core/Methods/RConvergenceCheck/RConvergenceCheck.h \
    ./Core/Methods/RConvergenceCheck/RStandardFORMConvergenceCheck.h \
    ./Core/Methods/RConvergenceCheck/RGradientNormConvergenceCheck.h \
    ./Core/Methods/RCholeskyDecomposition/RCholeskyDecomposition.h \
    ./Core/Methods/RCholeskyDecomposition/RBasicCholeskyDecomposition.h \
    ./Core/Methods/RLineSearchAlgorithm/RLineSearchAlgorithm.h \
    ./Core/Methods/RLineSearchAlgorithm/RFixedLineSearchAlgorithm.h \
    ./Core/Methods/RLineSearchAlgorithm/RBisectionLineSearchAlgorithm.h \
    ./Core/Methods/RLineSearchAlgorithm/RNewtonLineSearchAlgorithm.h \
    ./Core/Methods/RLineSearchAlgorithm/RSecantLineSearchAlgorithm.h \
    ./Core/Methods/RLineSearchAlgorithm/RGoldenSectionLineSearchAlgorithm.h \
    ./Core/Methods/RLineSearchAlgorithm/RArmijoLineSearchAlgorithm.h \
    ./Core/Methods/RLineSearchAlgorithm/RPlottingLineSearchAlgorithm.h \
    ./Core/Methods/ROptimizationAlgorithm/ROptimizationAlgorithm.h \
    ./Core/Methods/ROptimizationAlgorithm/RDirectionalLineSearchOptimizationAlgorithm.h \
    ./Core/Methods/ROptimizationAlgorithm/RDownhillSimplexOptimizationAlgorithm.h \
    ./Core/Parameters/RCommandResponse.h \
    ./Core/Parameters/RConstant.h \
    ./Core/Parameters/RContinuousDecisionVariable.h \
    ./Core/Parameters/RContinuousRandomVariable.h \
    ./Core/Parameters/RCorrelation.h \
    ./Core/Parameters/RDecisionVariable.h \
    ./Core/Parameters/RFileResponse.h \
    ./Core/Parameters/RGenericResponse.h \
    ./Core/Parameters/RLocation.h \
    ./Core/Parameters/RLocationResponse.h \
    ./Core/Parameters/RTimeSeriesResponse.h \
    ./Core/Parameters/RParameter.h \
    ./Core/Parameters/RRandomVariable.h \
    ./Core/Parameters/RResponse.h \
    ./Core/Parameters/RTime.h \
    ./Core/Parameters/RTimeSeries.h \
    ./Core/Parameters/RInfoRichResponse.h \
    ./Core/Models/RModel.h \
    ./Core/Models/MuParser/muParser.h \
    ./Core/Models/MuParser/muParserBase.h \
    ./Core/Models/MuParser/muParserBytecode.h \
    ./Core/Models/MuParser/muParserCallback.h \
    ./Core/Models/MuParser/muParserTemplateMagic.h \
    ./Core/Models/MuParser/muParserDef.h \
    ./Core/Models/MuParser/muParserDLL.h \
    ./Core/Models/MuParser/muParserError.h \
    ./Core/Models/MuParser/muParserFixes.h \
    ./Core/Models/MuParser/muParserInt.h \
    ./Core/Models/MuParser/muParserStack.h \
    ./Core/Models/MuParser/muParserToken.h \
    ./Core/Models/MuParser/muParserTokenReader.h \
    ./Core/Models/RAlgebraicExpressionModel.h \
    ./Core/Models/RHardcodedExpressionModel.h \
    ./Core/Models/RFORMModel.h \
    ./Core/Models/RFOSMModel.h \
    ./Core/Models/RSOSMModel.h \
    ./Core/Models/ROptimizationModel.h \
    ./Core/Tools/RLineSearchCallBackAssistant.h \
    ./Core/Models/R1DOptimizationModel.h \
    ./Core/Models/RRootFindingModel.h \
    ./Core/Models/RRandomVariableWithRandomParametersModel.h \
    ./Core/Models/RRiskModel.h \
    ./Core/Models/RSamplingModel.h \
    ./Core/Models/RScriptModel.h \
    ./Core/Models/RScenarioModel.h \
    ./Core/Tools/RStatistics.h \
    ./Core/Tools/REmpiricalProbabilityDistribution.h


SOURCES += ./Core/GUI/RMain.cpp \
    ./Core/GUI/RDialogs/RAboutDialog.cpp \
    ./Core/GUI/RDialogs/RVariableInferenceDialog.cpp \
    ./Core/GUI/RDialogs/RModelInferenceDialog.cpp \
    ./Core/GUI/RDialogs/RTimeSeriesDialog.cpp \
    ./Core/GUI/RObjectWidget/RAggregationObjectWidget.cpp \
    ./Core/GUI/RObjectWidget/RObjectWidget.cpp \
    ./Core/GUI/RPropertyWidget/RDelegate/QtColorComboBox.cpp \
    ./Core/GUI/RPropertyWidget/RDelegate/RCheckBoxTableDelegate.cpp \
    ./Core/GUI/RPropertyWidget/RDelegate/RColorComboBoxTableDelegate.cpp \
    ./Core/GUI/RPropertyWidget/RDelegate/RComboBoxTableDelegate.cpp \
    ./Core/GUI/RPropertyWidget/RDelegate/RDateTimeDelegate.cpp \
    ./Core/GUI/RPropertyWidget/RPropertyWidget.cpp \
    ./Core/GUI/RFlowchartWidget/RFlowchartWidget.cpp \
    ./Core/GUI/RCustomPlotWidget/qcustomplot.cpp \
    ./Core/GUI/RMainWindow.cpp \
    ./Core/GUI/RObject.cpp \
    ./Core/GUI/RDomain.cpp \
    ./Core/Tools/RMatrixOperations.cpp \
    ./Core/Tools/RNormalDistribution.cpp \
    ./Core/Tools/RModelManager.cpp \
    ./Core/Tools/REuclidsAlgorithm.cpp \
    ./Core/Tools/RFragilityFunctions.cpp \
    ./Core/Methods/RMethod.cpp \
    ./Core/Methods/RLinearSolver/RLinearSolver.cpp \
    ./Core/Methods/RLinearSolver/RCholeskyLinearSolver.cpp \
    ./Core/Methods/RLinearSolver/RGaussJordanLinearSolver.cpp \
    ./Core/Methods/RLinearSolver/RLULinearSolver.cpp \
    ./Core/Methods/RLinearSolver/RQRLinearSolver.cpp \
    ./Core/Methods/RLinearSolver/RMatrixInverseLinearSolver.cpp \
    ./Core/Methods/RLinearSolver/RInHouseLinearSolver.cpp \
    ./Core/Methods/RProbabilityDistributions/RProbabilityDistributions.cpp \
    ./Core/Methods/RProbabilityDistributions/RInHouseProbabilityDistributions.cpp \
    ./Core/Methods/RProbabilityTransformation/RProbabilityTransformation.cpp \
    ./Core/Methods/RProbabilityTransformation/RNatafProbabilityTransformation.cpp \
    ./Core/Methods/RProbabilityTransformation/RIndependenceProbabilityTransformation.cpp \
    ./Core/Methods/RRandomNumberGenerator/RRandomNumberGenerator.cpp \
    ./Core/Methods/RRandomNumberGenerator/RInHouseRandomNumberGenerator.cpp \
    ./Core/Methods/RSearchDirection/RSearchDirection.cpp \
    ./Core/Methods/RSearchDirection/RHLRFSearchDirection.cpp \
    ./Core/Methods/RSearchDirection/RSteepestDescentSearchDirection.cpp \
    ./Core/Methods/RSearchDirection/RQuasiNewtonSearchDirection.cpp \
    ./Core/Methods/RSearchDirection/RComboSearchDirection.cpp \
    ./Core/Methods/RSearchDirection/RConjugateGradientSearchDirection.cpp \
    ./Core/Methods/RMeritFunction/RMeritFunction.cpp \
    ./Core/Methods/RMeritFunction/RADKZhangMeritFunction.cpp \
    ./Core/Methods/RMeritFunction/RBasicMeritFunction.cpp \
    ./Core/Methods/RMeritFunction/RDummyMeritFunction.cpp \
    ./Core/Methods/RMeritFunction/RObjectiveMeritFunction.cpp \
    ./Core/Methods/RConvergenceCheck/RConvergenceCheck.cpp \
    ./Core/Methods/RConvergenceCheck/RStandardFORMConvergenceCheck.cpp \
    ./Core/Methods/RConvergenceCheck/RGradientNormConvergenceCheck.cpp \
    ./Core/Methods/RCholeskyDecomposition/RCholeskyDecomposition.cpp \
    ./Core/Methods/RCholeskyDecomposition/RBasicCholeskyDecomposition.cpp \
    ./Core/Methods/RLineSearchAlgorithm/RLineSearchAlgorithm.cpp \
    ./Core/Methods/RLineSearchAlgorithm/RFixedLineSearchAlgorithm.cpp \
    ./Core/Methods/RLineSearchAlgorithm/RBisectionLineSearchAlgorithm.cpp \
    ./Core/Methods/RLineSearchAlgorithm/RNewtonLineSearchAlgorithm.cpp \
    ./Core/Methods/RLineSearchAlgorithm/RSecantLineSearchAlgorithm.cpp \
    ./Core/Methods/RLineSearchAlgorithm/RGoldenSectionLineSearchAlgorithm.cpp \
    ./Core/Methods/RLineSearchAlgorithm/RArmijoLineSearchAlgorithm.cpp \
    ./Core/Methods/RLineSearchAlgorithm/RPlottingLineSearchAlgorithm.cpp \
    ./Core/Methods/ROptimizationAlgorithm/ROptimizationAlgorithm.cpp \
    ./Core/Methods/ROptimizationAlgorithm/RDirectionalLineSearchOptimizationAlgorithm.cpp \
    ./Core/Methods/ROptimizationAlgorithm/RDownhillSimplexOptimizationAlgorithm.cpp \
    ./Core/Parameters/RCommandResponse.cpp \
    ./Core/Parameters/RConstant.cpp \
    ./Core/Parameters/RContinuousDecisionVariable.cpp \
    ./Core/Parameters/RContinuousRandomVariable.cpp \
    ./Core/Parameters/RCorrelation.cpp \
    ./Core/Parameters/RDecisionVariable.cpp \
    ./Core/Parameters/RFileResponse.cpp \
    ./Core/Parameters/RGenericResponse.cpp \
    ./Core/Parameters/RLocation.cpp \
    ./Core/Parameters/RLocationResponse.cpp \
    ./Core/Parameters/RTimeSeriesResponse.cpp \
    ./Core/Parameters/RParameter.cpp \
    ./Core/Parameters/RRandomVariable.cpp \
    ./Core/Parameters/RResponse.cpp \
    ./Core/Parameters/RTime.cpp \
    ./Core/Parameters/RTimeSeries.cpp \
    ./Core/Parameters/RInfoRichResponse.cpp \
    ./Core/Models/RModel.cpp \
    ./Core/Models/MuParser/muParser.cpp \
    ./Core/Models/MuParser/muParserBase.cpp \
    ./Core/Models/MuParser/muParserBytecode.cpp \
    ./Core/Models/MuParser/muParserCallback.cpp \
    ./Core/Models/MuParser/muParserDLL.cpp \
    ./Core/Models/MuParser/muParserError.cpp \
    ./Core/Models/MuParser/muParserInt.cpp \
    ./Core/Models/MuParser/muParserTokenReader.cpp \
    ./Core/Models/RAlgebraicExpressionModel.cpp \
    ./Core/Models/RHardcodedExpressionModel.cpp \
    ./Core/Models/RFORMModel.cpp \
    ./Core/Models/RFOSMModel.cpp \
    ./Core/Models/RSOSMModel.cpp \
    ./Core/Models/ROptimizationModel.cpp \
    ./Core/Tools/RLineSearchCallBackAssistant.cpp \
    ./Core/Models/R1DOptimizationModel.cpp \
    ./Core/Models/RRootFindingModel.cpp \
    ./Core/Models/RRandomVariableWithRandomParametersModel.cpp \
    ./Core/Models/RRiskModel.cpp \
    ./Core/Models/RSamplingModel.cpp \
    ./Core/Models/RScriptModel.cpp \
    ./Core/Models/RScenarioModel.cpp \
    ./Core/Tools/RStatistics.cpp \
    ./Core/Tools/REmpiricalProbabilityDistribution.cpp


contains (DEFINES, GIS)   {
#FORMS += ./Core/GUI/RMapWidget/RGISWidget.ui \
HEADERS +=  ./Core/GUI/RMapWidget/RGISWidget.h
SOURCES +=  ./Core/GUI/RMapWidget/RGISWidget.cpp
 }
