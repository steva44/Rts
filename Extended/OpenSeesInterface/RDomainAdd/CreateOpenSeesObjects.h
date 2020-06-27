

// *******************************************************************************
// *******************************************************************************
//                                NODES/POINTS
// *******************************************************************************
// *******************************************************************************


factory.registerClass<RPoint>("RPoint");
factory.registerClass<RPoint>("COMPONENTS Points");

factory.registerClass<RParametricPoint>("RParametricPoint");
factory.registerClass<RParametricPoint>("OTHER OBJECTS Parametric Points");

factory.registerClass<RNode>("RNode");
factory.registerClass<RNode>("COMPONENTS Nodes");

factory.registerClass<RSlaveNode>("RSlaveNode");
factory.registerClass<RSlaveNode>("OTHER OBJECTS Slave Nodes");



// *******************************************************************************
// *******************************************************************************
//                                     MATERIALS
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RConcrete01>("RConcrete01");
factory.registerClass<RConcrete01>("MATERIALS OpenSees Concrete01");
factory.registerClass<RConcrete02>("RConcrete02");
factory.registerClass<RConcrete02>("MATERIALS OpenSees Concrete02");
factory.registerClass<RConcrete04>("RConcrete04");
factory.registerClass<RConcrete04>("MATERIALS OpenSees Concrete04");
factory.registerClass<RSteel02>("RSteel02");
factory.registerClass<RSteel02>("MATERIALS OpenSees Steel02");
factory.registerClass<RElasticBilin>("RElasticBilin");
factory.registerClass<RElasticBilin>("MATERIALS OpenSees ElasticBilin");
factory.registerClass<RUniaxialFiber3d>("RUniaxialFiber3d");
factory.registerClass<RPlateRebarMaterial>("RPlateRebarMaterial");
factory.registerClass<RPlateFromPlaneStress>("RPlateFromPlaneStress");
factory.registerClass<RPlasticDamageConcrete3d>("RPlasticDamageConcrete3d");
factory.registerClass<RElasticMaterial>("RElasticMaterial");
factory.registerClass<RElasticIsotropicMaterial>("RElasticIsotropicMaterial");
factory.registerClass<RPlateFiberMaterial>("RPlateFiberMaterial");

// *******************************************************************************
// *******************************************************************************
//                                      SECTIONS
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RElasticMembranePlateSection>("RElasticMembranePlateSection");
factory.registerClass<RFiberSection3d>("RFiberSection3d");
factory.registerClass<RLayeredShellFiberSection>("RLayeredShellFiberSection");
factory.registerClass<RMembranePlateFiberSection>("RMembranePlateFiberSection");

// *******************************************************************************
// *******************************************************************************
//                                     METHODS
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RLinearStaticStructuralAnalysis>("RLinearStaticStructuralAnalysis");
factory.registerClass<RInelasticStaticStructuralAnalysis>("RInelasticStaticStructuralAnalysis");
factory.registerClass<RInelasticDynamicStructuralAnalysis>("RInelasticDynamicStructuralAnalysis");


// *******************************************************************************
// *******************************************************************************
//                                   MODELS
// *******************************************************************************
// *******************************************************************************


factory.registerClass<RStructuralAnalysisModel>("RStructuralAnalysisModel");
factory.registerClass<RStructuralAnalysisModel>("MODELS Structural Analysis Models");


// *******************************************************************************
// *******************************************************************************
//                                LOAD PATTERNS
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RPointLoad>("RPointLoad");
factory.registerClass<RPointLoad>("LOADS Point Load");
factory.registerClass<RLoadPattern>("RLoadPattern");
factory.registerClass<RUniformExcitationPattern>("RUniformExcitationPattern");


// *******************************************************************************
// *******************************************************************************
//                                ELEMENTS
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RElasticBeam3d>("RElasticBeam3d");
factory.registerClass<RElasticBeam3d>("ELEMENTS OpenSees ElasticBeam3d");

factory.registerClass<RShellMITC4>("RShellMITC4");
factory.registerClass<RShellMITC4>("ELEMENTS OpenSees ShellMITC4");

factory.registerClass<RDispBeamColumn3d>("RDispBeamColumn3d");
factory.registerClass<RDispBeamColumn3d>("ELEMENTS OpenSees DispBeamColumn3d");

factory.registerClass<RForceBeamColumn3d>("RForceBeamColumn3d");
factory.registerClass<RForceBeamColumn3d>("ELEMENTS OpenSees ForceBeamColumn3d");

factory.registerClass<RBrick>("RBrick");
factory.registerClass<RBrick>("ELEMENTS OpenSees Brick");

factory.registerClass<RElasticTimoshenkoBeam3d>("RElasticTimoshenkoBeam3d");
factory.registerClass<RElasticTimoshenkoBeam3d>("ELEMENTS OpenSees ElasticTimoshenkoBeam3d");


// *******************************************************************************
// *******************************************************************************
//                                OTHER OBJECTS
// *******************************************************************************
// *******************************************************************************


factory.registerClass<RFixedPlaneBoundaryCondition>("RFixedPlaneBoundaryCondition");
factory.registerClass<RFixedPlaneBoundaryCondition>("OTHER Fixed Planes");


// *******************************************************************************
// *******************************************************************************
//                               INTEGRATOR
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RLoadControlIntegrator>("RLoadControlIntegrator");
factory.registerClass<RDisplacementControlIntegrator>("RDisplacementControlIntegrator");
factory.registerClass<RNewmarkIntegrator>("RNewmarkIntegrator");


// *******************************************************************************
// *******************************************************************************
//                               CONSTRAINT HANDLERS
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RPenaltyConstraintHandler>("RPenaltyConstraintHandler");
factory.registerClass<RTransformationConstraintHandler>("RTransformationConstraintHandler");


// *******************************************************************************
// *******************************************************************************
//                               DOF NUMBERER
// *******************************************************************************
// *******************************************************************************
factory.registerClass<RDOFNumberer>("RDOFNumberer");


// *******************************************************************************
// *******************************************************************************
//                              SOLVERS
// *******************************************************************************
// *******************************************************************************
factory.registerClass<RLinearEquationSolutionAlgorithm>("RLinearEquationSolutionAlgorithm");
factory.registerClass<RBandSPDLinearSOE>("RBandSPDLinearSOE");
factory.registerClass<RProfileSPDLinSOE>("RProfileSPDLinSOE");
factory.registerClass<RBandGenLinSOE>("RBandGenLinSOE");
factory.registerClass<RMumpsParallelSOE>("RMumpsParallelSOE");
factory.registerClass<RMumpsSOE>("RMumpsSOE");
factory.registerClass<RSparseGenColLinSOE>("RSparseGenColLinSOE");


// *******************************************************************************
// *******************************************************************************
//                           COORDINATE TRANSFORMATIONS
// *******************************************************************************
// *******************************************************************************
factory.registerClass<RLinearCrdTransf3d>("RLinearCrdTransf3d");
factory.registerClass<RPDeltaCrdTransf3d>("RPDeltaCrdTransf3d");


// *******************************************************************************
// *******************************************************************************
//                             TIME SERIES
// *******************************************************************************
// *******************************************************************************
factory.registerClass<RLinearSeries>("RLinearSeries");
factory.registerClass<RPathTimeSeries>("RPathTimeSeries");


// *******************************************************************************
// *******************************************************************************
//                             CONVERGENCE TEST
// *******************************************************************************
// *******************************************************************************
factory.registerClass<RCTestEnergyIncr>("RCTestEnergyIncr");
factory.registerClass<RCTestNormDispIncr>("RCTestNormDispIncr");
factory.registerClass<RCTestNormUnbalance>("RCTestNormUnbalance");

// *******************************************************************************
// *******************************************************************************
//                       EQUATION SOLUTION ALGORITHM
// *******************************************************************************
// *******************************************************************************
factory.registerClass<RNewtonRaphsonEquationSolutionAlgorithm>("RNewtonRaphsonEquationSolutionAlgorithm");
factory.registerClass<RModifiedNewtonEquationSolutionAlgorithm>("RModifiedNewtonEquationSolutionAlgorithm");
factory.registerClass<RKrylovNewtonEquationSolutionAlgorithm>("RKrylovNewtonEquationSolutionAlgorithm");
factory.registerClass<RBFGSEquationSolutionAlgorithm>("RBFGSEquationSolutionAlgorithm");
factory.registerClass<RBroydenEquationSolutionAlgorithm>("RBroydenEquationSolutionAlgorithm");
factory.registerClass<RLinearEquationSolutionAlgorithm>("RLinearEquationSolutionAlgorithm");

// *******************************************************************************
// *******************************************************************************
//                       Rigid Links
// *******************************************************************************
// *******************************************************************************
factory.registerClass<RRigidBeam>("RRigidBeam");

