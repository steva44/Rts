#/*********************************************************************
#*                                                                    *
#* This file is posted by Dr. Stevan Gavrilovic (steva44@hotmail.com) *
#* as a result of work performed in the research group of Professor   *
#* Terje Haukaas (terje@civil.ubc.ca) at the University of British    *
#* Columbia in Vancouver. The code is part of the computer program    *
#* Rts, which is an extension of the computer program Rt developed by *
#* Dr. Mojtaba Mahsuli (mahsuli@sharif.edu) in the same group.        *
#*                                                                    *
#* The content of this file is the product of thesis-related work     *
#* performed at the University of British Columbia (UBC). Therefore,  *
#* the Rights and Ownership are held by UBC.                          *
#*                                                                    *
#* Please be cautious when using this code. It is provided “as is”    *
#* without warranty of any kind, express or implied.                  *
#*                                                                    *
#* Contributors to this file:                                         *
#*   - Stevan Gavrilovic                                              *
#*                                                                    *
#*********************************************************************/


INCLUDEPATH +=  \
    ./Extended/OpenSeesInterface \
    ./Extended/OpenSeesInterface/Elements \
    ./Extended/OpenSeesInterface/Elements/Elements \
    ./Extended/OpenSeesInterface/Elements/Meshes \
    ./Extended/OpenSeesInterface/Elements/Meshes/BeamColumns \
    ./Extended/OpenSeesInterface/Elements/Meshes/Bricks \
    ./Extended/OpenSeesInterface/Elements/Meshes/Plates \
    ./Extended/OpenSeesInterface/Elements/Meshes/Walls \
    ./Extended/OpenSeesInterface/Elements/Nodes \
    ./Extended/OpenSeesInterface/Elements/Points \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge \
    ./Extended/OpenSeesInterface/Elements/Materials \
    ./Extended/OpenSeesInterface/Elements/Sections \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LayeredShell \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections \
    ./Extended/OpenSeesInterface/RDomainAdd \
    ./Extended/OpenSeesInterface/Models \
    ./Extended/OpenSeesInterface/Methods \
    ./Extended/OpenSeesInterface/Methods/StructuralAnalysis \
    ./Extended/OpenSeesInterface/WrapperClasses \
    ./Extended/OpenSeesInterface/Tools/

HEADERS +=  \
    ./Extended/OpenSeesInterface/RSmartPointer.h \
    ./Extended/OpenSeesInterface/ROpenSeesInputFileGenerator.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/BeamColumns/RBeamColumnMesh.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/BeamColumns/RLinearElasticBeamColumnMesh.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/BeamColumns/RNonlinearBeamColumnMesh.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/Bricks/RLinearElasticBrickMesh.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/Walls/RLinearElasticWallMesh.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/Walls/RPlateFibreWallMesh.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/Walls/RLayeredShellWallMesh.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/Plates/RLinearElasticSlabMesh.h \
    ./Extended/OpenSeesInterface/Elements/Nodes/RNode.h \
    ./Extended/OpenSeesInterface/Elements/Nodes/RSlaveNode.h \
    ./Extended/OpenSeesInterface/Elements/Points/RParametricPoint.h \
    ./Extended/OpenSeesInterface/Elements/Points/RPoint.h \
    ./Extended/OpenSeesInterface/Elements/Points/R2DPoint.h \
    ./Extended/OpenSeesInterface/Elements/Points/R3DPoint.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RFiber.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RUniaxialFiber3d.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RUniaxialMaterial.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RNDMaterial.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RElasticMaterial.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RElasticIsotropicMaterial.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RPlateFiberMaterial.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RPlateRebarMaterial.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RPlateFromPlaneStress.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RPlasticDamageConcrete3d.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RConcrete01.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RConcrete02.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RConcrete04.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RSteel02.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RElasticBilin.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RPinching4Material.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RPinchingLimitStateMaterial.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RFiberSection3d.h \
    ./Extended/OpenSeesInterface/Elements/Sections/RSection.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LayeredShell/RLayeredShellFiberSection.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LayeredShell/RRCWallLayeredShellSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Models/RStructuralAnalysisModel.h  \
    ./Extended/OpenSeesInterface/Methods/StructuralAnalysis/RStructuralAnalysis.h  \
    ./Extended/OpenSeesInterface/Methods/StructuralAnalysis/RLinearStaticStructuralAnalysis.h  \
    ./Extended/OpenSeesInterface/Methods/StructuralAnalysis/RInelasticStaticStructuralAnalysis.h  \
    ./Extended/OpenSeesInterface/Methods/StructuralAnalysis/RInelasticDynamicStructuralAnalysis.h  \
    ./Extended/OpenSeesInterface/WrapperClasses/RIntegrator.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RLoadControlIntegrator.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RDisplacementControlIntegrator.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RNewmarkIntegrator.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RPenaltyConstraintHandler.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RTransformationConstraintHandler.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RConstraintHandler.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RCrdTransf.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RLinearCrdTransf3d.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RPDeltaCrdTransf3d.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RLinearEquationSolutionAlgorithm.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RDOFNumberer.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RBandSPDLinearSOE.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RBandGenLinSOE.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RSparseGenColLinSOE.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RMumpsSOE.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RProfileSPDLinSOE.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RLinearSOE.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RElasticBeam3d.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RElasticTimoshenkoBeam3d.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RRigidBeam.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RZeroLength.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RDispBeamColumn3d.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RForceBeamColumn3d.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RShellMITC4.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RBrick.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RLoadPattern.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RUniformExcitationPattern.h \
    ./Extended/OpenSeesInterface/WrapperClasses/ROpenSeesTimeSeries.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RPathTimeSeries.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RLinearSeries.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RSectionForceDeformation.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RElasticMembranePlateSection.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RMembranePlateFiberSection.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RConvergenceTest.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RCTestNormDispIncr.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RCTestNormUnbalance.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RCTestEnergyIncr.h \
    ./Extended/OpenSeesInterface/WrapperClasses/REquiSolnAlgo.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RNewtonRaphsonEquationSolutionAlgorithm.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RModifiedNewtonEquationSolutionAlgorithm.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RKrylovNewtonEquationSolutionAlgorithm.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RBFGSEquationSolutionAlgorithm.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RBroydenEquationSolutionAlgorithm.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RLimitCurve.h \
    ./Extended/OpenSeesInterface/WrapperClasses/RRotationShearCurve.h \
    ./Extended/OpenSeesInterface/Elements/Materials/RMaterial.h \
    ./Extended/OpenSeesInterface/Elements/RElement.h \
    ./Extended/OpenSeesInterface/Elements/RNonlinearBeamColumnElement.h \
    ./Extended/OpenSeesInterface/Elements/RElasticBeamColumnElement.h \
    ./Extended/OpenSeesInterface/Elements/RLinearShellElement.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/RMesh.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/Plates/RSlabMesh.h \
    ./Extended/OpenSeesInterface/Elements/Meshes/Walls/RWallMesh.h \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RMaterialKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RConcreteMaterialKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RConfinedConcreteMaterialKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RCLTMaterialKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RGlulamMaterialKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RCompositeDeckingMaterialKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RSteelMaterialKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/RSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections/RRCShearWallFibreSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RLinearElasticSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RCircularLinearElasticSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RCircularHSSLinearElasticSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RRectangularLinearElasticSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RWLinearElasticSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RGenericSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections/RFibreSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections/RWFibreSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections/RRectangularRCFibreSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections/RRectangularFibreSectionKnowledge.h \
    ./Extended/OpenSeesInterface/Tools/RMeshGenerator.h \
    ./Extended/OpenSeesInterface/Tools/RFibreSectionMeshGenerator.h \
    ./Extended/OpenSeesInterface/Tools/RMathUtils.h \
    ./Extended/OpenSeesInterface/Tools/RFixedPlaneBoundaryCondition.h \
    ./Extended/OpenSeesInterface/Tools/RCoordinateTransformation.h \


SOURCES += \
    ./Extended/OpenSeesInterface/ROpenSeesInputFileGenerator.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/BeamColumns/RBeamColumnMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/BeamColumns/RLinearElasticBeamColumnMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/BeamColumns/RNonlinearBeamColumnMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/Bricks/RLinearElasticBrickMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/Walls/RLinearElasticWallMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/Walls/RPlateFibreWallMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/Walls/RLayeredShellWallMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/Plates/RLinearElasticSlabMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/Nodes/RNode.cpp \
    ./Extended/OpenSeesInterface/Elements/Nodes/RSlaveNode.cpp \
    ./Extended/OpenSeesInterface/Elements/Points/RParametricPoint.cpp \
    ./Extended/OpenSeesInterface/Elements/Points/RPoint.cpp \
    ./Extended/OpenSeesInterface/Elements/Points/R2DPoint.cpp \
    ./Extended/OpenSeesInterface/Elements/Points/R3DPoint.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RFiber.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RUniaxialFiber3d.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RUniaxialMaterial.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RNDMaterial.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RPlateRebarMaterial.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RElasticMaterial.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RElasticIsotropicMaterial.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RPlateFiberMaterial.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RPlateFromPlaneStress.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RPlasticDamageConcrete3d.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RConcrete01.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RConcrete02.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RConcrete04.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RSteel02.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RElasticBilin.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RPinching4Material.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RPinchingLimitStateMaterial.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RFiberSection3d.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/RSection.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LayeredShell/RLayeredShellFiberSection.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LayeredShell/RRCWallLayeredShellSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Models/RStructuralAnalysisModel.cpp  \
    ./Extended/OpenSeesInterface/Methods/StructuralAnalysis/RStructuralAnalysis.cpp  \
    ./Extended/OpenSeesInterface/Methods/StructuralAnalysis/RLinearStaticStructuralAnalysis.cpp  \
    ./Extended/OpenSeesInterface/Methods/StructuralAnalysis/RInelasticStaticStructuralAnalysis.cpp  \
    ./Extended/OpenSeesInterface/Methods/StructuralAnalysis/RInelasticDynamicStructuralAnalysis.cpp  \
    ./Extended/OpenSeesInterface/WrapperClasses/RIntegrator.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RLoadControlIntegrator.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RDisplacementControlIntegrator.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RNewmarkIntegrator.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RPenaltyConstraintHandler.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RTransformationConstraintHandler.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RConstraintHandler.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RCrdTransf.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RLinearCrdTransf3d.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RPDeltaCrdTransf3d.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RLinearEquationSolutionAlgorithm.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RDOFNumberer.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RBandSPDLinearSOE.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RBandGenLinSOE.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RSparseGenColLinSOE.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RMumpsSOE.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RProfileSPDLinSOE.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RLinearSOE.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RElasticBeam3d.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RElasticTimoshenkoBeam3d.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RRigidBeam.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RZeroLength.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RDispBeamColumn3d.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RForceBeamColumn3d.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RShellMITC4.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RBrick.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RLoadPattern.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RUniformExcitationPattern.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/ROpenSeesTimeSeries.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RPathTimeSeries.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RLinearSeries.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RSectionForceDeformation.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RElasticMembranePlateSection.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RMembranePlateFiberSection.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RConvergenceTest.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RCTestNormDispIncr.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RCTestNormUnbalance.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RCTestEnergyIncr.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/REquiSolnAlgo.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RNewtonRaphsonEquationSolutionAlgorithm.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RModifiedNewtonEquationSolutionAlgorithm.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RKrylovNewtonEquationSolutionAlgorithm.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RBFGSEquationSolutionAlgorithm.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RBroydenEquationSolutionAlgorithm.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RLimitCurve.cpp \
    ./Extended/OpenSeesInterface/WrapperClasses/RRotationShearCurve.cpp \
    ./Extended/OpenSeesInterface/Elements/Materials/RMaterial.cpp \
    ./Extended/OpenSeesInterface/Elements/RElement.cpp \
    ./Extended/OpenSeesInterface/Elements/RNonlinearBeamColumnElement.cpp \
    ./Extended/OpenSeesInterface/Elements/RElasticBeamColumnElement.cpp \
    ./Extended/OpenSeesInterface/Elements/RLinearShellElement.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/RMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/Plates/RSlabMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/Meshes/Walls/RWallMesh.cpp \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RMaterialKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RConcreteMaterialKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RConfinedConcreteMaterialKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RCLTMaterialKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RGlulamMaterialKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RCompositeDeckingMaterialKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/MaterialKnowledge/RSteelMaterialKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/RSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections/RRCShearWallFibreSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RLinearElasticSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RCircularLinearElasticSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RCircularHSSLinearElasticSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RRectangularLinearElasticSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RWLinearElasticSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/LinearElasticSections/RGenericSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections/RFibreSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections/RWFibreSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections/RRectangularRCFibreSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Elements/Sections/SectionKnowledge/FibreSections/RRectangularFibreSectionKnowledge.cpp \
    ./Extended/OpenSeesInterface/Tools/RMeshGenerator.cpp \
    ./Extended/OpenSeesInterface/Tools/RFibreSectionMeshGenerator.cpp \
    ./Extended/OpenSeesInterface/Tools/RMathUtils.cpp \
    ./Extended/OpenSeesInterface/Tools/RFixedPlaneBoundaryCondition.cpp \
    ./Extended/OpenSeesInterface/Tools/RCoordinateTransformation.cpp \


#Parallel mumps needs MPI
contains (DEFINES, _OPENMPI){

HEADERS +=  \
    ./Extended/OpenSeesInterface/WrapperClasses/RMumpsParallelSOE.h \

SOURCES += \
    ./Extended/OpenSeesInterface/WrapperClasses/RMumpsParallelSOE.cpp \

}

