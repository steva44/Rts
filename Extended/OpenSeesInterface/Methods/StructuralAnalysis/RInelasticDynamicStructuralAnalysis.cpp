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

#include "RInelasticDynamicStructuralAnalysis.h"
#include "RConstraintHandler.h"
#include "RDOFNumberer.h"
#include "RLinearSOE.h"
#include "RIntegrator.h"
#include "RConvergenceTest.h"
#include "RNode.h"
#include "RElement.h"
#include "REquiSolnAlgo.h"
#include "RMainWindow.h"
#include "ROSGPlotter.h"
#include "RComponent.h"
#include "RVisualDamageModel.h"
#include "RStevesRepairImpactModel.h"
#include "RResponse.h"

//OpenSees
#include "NewtonRaphson.h"
#include "EquiSolnAlgo.h"
#include "DirectIntegrationAnalysis.h"
#include "Domain.h"
#include "TransientIntegrator.h"
#include "ConvergenceTest.h"

#include <functional>

RInelasticDynamicStructuralAnalysis::RInelasticDynamicStructuralAnalysis(QObject *parent, QString name)
    : RStructuralAnalysis(parent, name)
{
    theConstraintHandler = nullptr;
    theDOFNumberer = nullptr;
    theLinearSOESolver = nullptr;
    theIntegrator = nullptr;
    theSolutionAlgo = nullptr;
    theConvergenceTest = nullptr;

    theTimeStep = 0.00;
    theStartTime = 0.0;
    theEndTime = 10.0;
    thePlotInterval = 3;
    numSteps = -1;

    rayleighDamping = new double[4];
    rayleighDamping[0] = 0.0;
    rayleighDamping[1] = 0.0;
    rayleighDamping[2] = 0.0;
    rayleighDamping[3] = 0.0;
}


RInelasticDynamicStructuralAnalysis::~RInelasticDynamicStructuralAnalysis()
{
    delete [] rayleighDamping;
}


int RInelasticDynamicStructuralAnalysis::conductStructuralAnalysis()
{
    maxDisplacement = 0.0;

    //Create the transient analysis if it is not yet created
    if (!theTimeHistoryAnalysis)
    {
        auto res = this->createOpenSeesLinearDirectIntegrationAnalysis();

        if(res != 0)
            return -1;
    }

#ifdef OSG
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    // Create a plotter
    ROSGPlotter* thePlotter = mainWindow->getOSGPlotter();
#endif

    // Set the rayleigh damping factors
    auto alphaM = rayleighDamping[0];
    auto betaK = rayleighDamping[1];
    auto betaK0 = rayleighDamping[2];
    auto betaKc = rayleighDamping[3];

    auto theOpenSeesDomain = theDomain->getTheOpenSeesDomain();
    theOpenSeesDomain->setRayleighDampingFactors(alphaM, betaK, betaK0, betaKc);

    auto nodeList = theDomain->findChildren<RNode*>();
    auto componentList = theDomain->findChildren<RComponent*>();

    theOpenSeesDomain->setCurrentTime(theStartTime);

    int result = 0;
    for (int i=0; i<numSteps; ++i)
    {
        // Keep the GUI responsive
        QCoreApplication::processEvents();

        result = theTimeHistoryAnalysis->analyzeStep(theTimeStep);

        // Skip the last evaluation since it will automatically be evaluated in RModel
        if(theVisualDamageModel && i != numSteps-1)
            theVisualDamageModel->evaluateModel(RGradientType::NONE);

        // Evaluate the callback function, if one exists
        if(callback && i != numSteps-1 && i % 50 == 0)
        {
            double time = static_cast<double>(i)*theTimeStep;

            std::function<void(double&)>* callBackFxn = callback.get();
            (*callBackFxn)(time);
        }

        auto currentTime = theOpenSeesDomain->getCurrentTime();

        for(auto&& it: nodeList)
        {
            // qDebug()<<"Node with tag: "<<it->getObjectTag()<<"and name"<<it->objectName()<<"\n";
            auto vecDisp = it->getDisp();
            for(auto i = 0; i< vecDisp.Size(); ++i)
            {
                if(fabs(vecDisp(i))>fabs(maxDisplacement))
                {
                    maxDisplacement = vecDisp(i);
                }
            }
        }

        for(auto&& it: componentList)
        {
            it->commitState();
        }

        qDebug()<<"Iteration "<<i<<" the current time is:"<<currentTime;

        if (result != 0)
            break;

        // Test start
        // thePlotter->plotStructure(true,"Dynamic",currentTime, false);
        // thePlotter->plotTopology(true);
        // thePlotter->recordStructuralResponse("displacement",currentTime);
        //Test end

        // Keep the user interface responsive in case this takes time to process...
        QCoreApplication::processEvents();
    }

    // Check for potential solver errors and report
    if(result != 0)
    {
        qCritical()<<"Error in the transient analysis in "<<this->objectName()<<" error tag returned was: "<<result;
        qDebug()<<"The maximum displacement is:"<<maxDisplacement<<" m\n";
        return result;
    }

    auto numIt = theTimeHistoryAnalysis->getAlgorithm()->getNumIterations();

    qDebug()<<"After "<<numIt<<" iterations, the maximum displacement is:";
    qDebug()<<maxDisplacement<<" m\n";

    // Test start
    // thePlotter->plotDynamicStructure();
    // thePlotter->plotDynamicTopo();
    // auto elementList = theDomain->findChildren<RElement*>();
    // for(auto it: elementList)
    // {
    //     it->print();
    // }
    // Test end

    return 0;
}


QObject *RInelasticDynamicStructuralAnalysis::getTheConstraintHandler() const
{
    return theConstraintHandler;
}


void RInelasticDynamicStructuralAnalysis::setTheConstraintHandler(QObject *value)
{
    theConstraintHandler = qobject_cast<RConstraintHandler*>(value);
}


QObject *RInelasticDynamicStructuralAnalysis::getTheDOFNumberer() const
{
    return theDOFNumberer;
}


void RInelasticDynamicStructuralAnalysis::setTheDOFNumberer(QObject *value)
{
    theDOFNumberer = qobject_cast<RDOFNumberer*>(value);
}


QObject *RInelasticDynamicStructuralAnalysis::getTheLinearSOESolver() const
{
    return theLinearSOESolver;
}


void RInelasticDynamicStructuralAnalysis::setTheLinearSOESolver(QObject *value)
{
    theLinearSOESolver = qobject_cast<RLinearSOE*>(value);
}


QObject *RInelasticDynamicStructuralAnalysis::getTheIntegrator() const
{
    return theIntegrator;
}


void RInelasticDynamicStructuralAnalysis::setTheIntegrator(QObject *value)
{
    theIntegrator = qobject_cast<RIntegrator*>(value);
}


QObject *RInelasticDynamicStructuralAnalysis::getTheConvergenceTest() const
{
    return theConvergenceTest;
}


void RInelasticDynamicStructuralAnalysis::setTheConvergenceTest(QObject *value)
{
    theConvergenceTest = qobject_cast<RConvergenceTest*>(value);
}


QObject *RInelasticDynamicStructuralAnalysis::getTheEquiSolnAlgo() const
{
    return theSolutionAlgo;
}


void RInelasticDynamicStructuralAnalysis::setTheEquiSolnAlgo(QObject *value)
{
    theSolutionAlgo = qobject_cast<REquiSolnAlgo*>(value);
}


double RInelasticDynamicStructuralAnalysis::getTimeStep() const
{
    return theTimeStep;
}


void RInelasticDynamicStructuralAnalysis::setTimeStep(double value)
{
    if (value <= 0.0) {
        qCritical() << "The time step must be greater than zero.";
        value = 0.01;
    }
    theTimeStep = value;
}


double RInelasticDynamicStructuralAnalysis::getEndTime() const
{
    return theEndTime;
}


void RInelasticDynamicStructuralAnalysis::setEndTime(double value)
{
    if (value <= 0.0) {
        qCritical() << "The end time must be greater than zero.";
        value = 10.0;
    }
    theEndTime = value;
}


double RInelasticDynamicStructuralAnalysis::getStartTime() const
{
    return theStartTime;
}


void RInelasticDynamicStructuralAnalysis::setStartTime(double value)
{
    if (value < 0.0) {
        qCritical() << "The start time must be greater than or equal to zero.";
        value = 0.0;
    }

    theStartTime = value;
}


int RInelasticDynamicStructuralAnalysis::getPlotInterval() const
{
    return thePlotInterval;
}


void RInelasticDynamicStructuralAnalysis::setPlotInterval(int value)
{
    if (value <= 0.0) {
        qCritical() << "The plot interval must be greater than zero.";
        value = 10;
    }
    thePlotInterval = value;
}


int RInelasticDynamicStructuralAnalysis::createOpenSeesLinearDirectIntegrationAnalysis(void)
{

    //Check to make sure the core property objects are given
    auto theOpenSeesDomain = theDomain->getTheOpenSeesDomain();
    if(theOpenSeesDomain == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'domain' was not set in "<<__FUNCTION__;
        return -1;
    }
    if(theAnalysisModel == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'Analysis Model' was not set in "<<__FUNCTION__;
        return -1;
    }
    if(theConstraintHandler == nullptr)
    {
        qCritical()<<"Error: "<<this->objectName()<<" needs a 'Constraint Handler' in"<<__FUNCTION__;
        return -1;
    }
    if(theDOFNumberer == nullptr)
    {
        qCritical()<<"Error: "<<this->objectName()<<" needs a 'DOF Numberer' in"<<__FUNCTION__;
        return -1;
    }
    if(theLinearSOESolver == nullptr)
    {
        qCritical()<<"Error: "<<this->objectName()<<" needs a 'Solver' in"<<__FUNCTION__;
        return -1;
    }
    if(theIntegrator == nullptr)
    {
        qCritical()<<"Error: "<<this->objectName()<<" needs na 'Integrator' in"<<__FUNCTION__;
        return -1;
    }
    if(theConvergenceTest == nullptr)
    {
        qCritical()<<"Error: "<<this->objectName()<<" needs a 'Convergence Test' in"<<__FUNCTION__;
        return -1;
    }
    if(theSolutionAlgo == nullptr)
    {
        qCritical()<<"Error: "<<this->objectName()<<" needs a 'Solution Algorithm' in"<<__FUNCTION__;
        return -1;
    }

    //Get/initialize the OpenSees objects from the wrapper classes

    auto theHandler = theConstraintHandler->getTheConstraintHandler();
    if(theHandler == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'Constraint Handler' from "<<theConstraintHandler->objectName()<<"failed to initialize in "<<__FUNCTION__;
        return -1;
    }
    auto theNumberer = theDOFNumberer->getTheDOFNumberer();
    if(theNumberer == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'DOF Numberer' from "<<theDOFNumberer->objectName()<<"failed to initialize in "<<__FUNCTION__;
        return -1;
    }
    auto theSOE = theLinearSOESolver->getTheLinearSOE();
    if(theSOE == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'SOE' from "<<theLinearSOESolver->objectName()<<"failed to initialize in "<<__FUNCTION__;
        return -1;
    }
    auto theInt = dynamic_cast<TransientIntegrator*>(theIntegrator->getTheIntegrator());
    if(theInt == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'Integrator' from "<<theIntegrator->objectName()<<"failed to initialize in "<<__FUNCTION__;
        return -1;
    }
    auto theTest = dynamic_cast<ConvergenceTest*>(theConvergenceTest->getTheConvergenceTest());
    if(theTest == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'Convergence Test' from "<<theConvergenceTest->objectName()<<"failed to initialize in "<<__FUNCTION__;
        return -1;
    }
    auto  theSolnAlgo = dynamic_cast<EquiSolnAlgo*>(theSolutionAlgo->getTheEquationSolutionAlgo());
    if(theSolnAlgo == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'Equation Solution Algorithm' from "<<theConvergenceTest->objectName()<<"failed to initialize in "<<__FUNCTION__;
        return -1;
    }

    // DirectIntegrationAnalysis(Domain &theDomain,
    // ConstraintHandler &theHandler,
    // DOF_Numberer &theNumberer,
    // AnalysisModel &theModel,
    // EquiSolnAlgo &theSolnAlgo,
    // LinearSOE &theSOE,
    // TransientIntegrator &theIntegrator,
    // ConvergenceTest *theTest = 0,
    // int numSubLevels = 0,
    // int numSubSteps = 0);

    theTimeHistoryAnalysis = std::make_unique<DirectIntegrationAnalysis>(*theOpenSeesDomain,
                                                                       *theHandler,
                                                                       *theNumberer,
                                                                       *theAnalysisModel,
                                                                       *theSolnAlgo,
                                                                       *theSOE,
                                                                       *theInt,
                                                                       theTest);

    if(theTimeHistoryAnalysis == nullptr)
    {
        qCritical()<<"Error: failed to create the linear static analysis in "<<__FUNCTION__;
        return -1;
    }

    return 0;
}


int RInelasticDynamicStructuralAnalysis::getNumSteps() const
{
    return numSteps;
}


void RInelasticDynamicStructuralAnalysis::setNumSteps(int value)
{
    numSteps = value;
}


double *RInelasticDynamicStructuralAnalysis::getRayleighDamping() const
{
    return rayleighDamping;
}


void RInelasticDynamicStructuralAnalysis::setRayleighDamping(double *value)
{
    rayleighDamping = value;
}


