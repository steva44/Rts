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

#include "RInelasticStaticStructuralAnalysis.h"
#include "RConstraintHandler.h"
#include "RDOFNumberer.h"
#include "RLinearSOE.h"
#include "RIntegrator.h"
#include "RNode.h"
#include "RElement.h"
#include "RConvergenceTest.h"
#include "REquiSolnAlgo.h"
#include "RLinearSeries.h"
#include "RDisplacementControlIntegrator.h"
#include "RVisualDamageModel.h"
#include "RComponent.h"

// OpenSees
#include "Linear.h"
#include "EquiSolnAlgo.h"
#include "StaticAnalysis.h"
#include "Domain.h"
#include "StaticIntegrator.h"
#include "ConvergenceTest.h"
#include "EquiSolnAlgo.h"

#include <math.h>

RInelasticStaticStructuralAnalysis::RInelasticStaticStructuralAnalysis(QObject *parent, QString name)
    : RStructuralAnalysis(parent, name)
{
    theConstraintHandler = nullptr;
    theDOFNumberer = nullptr;
    theLinearSOESolver = nullptr;
    theIntegrator = nullptr;
    theSolutionAlgo = nullptr;
    theConvergenceTest = nullptr;

    maxSteps = -1;
}


RInelasticStaticStructuralAnalysis::~RInelasticStaticStructuralAnalysis()
{

}


QObject *RInelasticStaticStructuralAnalysis::getTheConstraintHandler() const
{
    return theConstraintHandler;
}


void RInelasticStaticStructuralAnalysis::reset()
{
    theLinearStaticAnalysis.reset(nullptr);

    return;
}


void RInelasticStaticStructuralAnalysis::setTheConstraintHandler(QObject *value)
{
    auto valueCast = qobject_cast<RConstraintHandler*>(value);
    theConstraintHandler = valueCast;
}


QObject *RInelasticStaticStructuralAnalysis::getTheDOFNumberer() const
{
    return theDOFNumberer;
}


void RInelasticStaticStructuralAnalysis::setTheDOFNumberer(QObject *value)
{
    auto valueCast = qobject_cast<RDOFNumberer*>(value);
    theDOFNumberer = valueCast;
}


QObject *RInelasticStaticStructuralAnalysis::getTheLinearSOESolver() const
{
    return theLinearSOESolver;
}


void RInelasticStaticStructuralAnalysis::setTheLinearSOESolver(QObject *value)
{
    auto valueCast = qobject_cast<RLinearSOE*>(value);
    theLinearSOESolver = valueCast;
}


QObject *RInelasticStaticStructuralAnalysis::getTheIntegrator() const
{
    return theIntegrator;
}


void RInelasticStaticStructuralAnalysis::setTheIntegrator(QObject *value)
{
    auto valueCast = qobject_cast<RIntegrator*>(value);
    theIntegrator = valueCast;
}


QObject *RInelasticStaticStructuralAnalysis::getTheConvergenceTest() const
{
    return theConvergenceTest;
}


void RInelasticStaticStructuralAnalysis::setTheConvergenceTest(QObject *value)
{
    auto valueCast = qobject_cast<RConvergenceTest*>(value);
    theConvergenceTest = valueCast;
}


int RInelasticStaticStructuralAnalysis::getMaxSteps() const
{
    return maxSteps;
}


void RInelasticStaticStructuralAnalysis::setMaxSteps(int value)
{
    maxSteps = value;
}


QObject *RInelasticStaticStructuralAnalysis::getTheEquiSolnAlgo() const
{
    return theSolutionAlgo;
}


void RInelasticStaticStructuralAnalysis::setTheEquiSolnAlgo(QObject *value)
{
    auto valueCast = qobject_cast<REquiSolnAlgo*>(value);
    theSolutionAlgo = valueCast;
}


int RInelasticStaticStructuralAnalysis::conductStructuralAnalysis()
{
    maxDisplacement = 0.0;

    if (!theLinearStaticAnalysis)
    {
        auto res = this->createOpenSeesLinearStaticAnalysis();

        if(res != 0)
            return -1;
    }

    if(maxSteps == -1)
    {
        maxSteps = theIntegrator->getMaxSteps();
    }

    if(maxSteps <= 0)
    {
        qCritical()<<"Error in getting the max steps in "<<this->objectName();
        return -1;
    }

    auto result = theLinearStaticAnalysis->analyze(maxSteps);

    // Check for potential solver errors
    if(result != 0)
    {
        qCritical()<<"Error in the inlestic static analysis in "<<this->objectName();
        return result;
    }

    auto numIt = theLinearStaticAnalysis->getAlgorithm()->getNumIterations();

    qDebug()<<"The number of iterations at convergence: "<<numIt;

    if(theVisualDamageModel)
        theVisualDamageModel->evaluateModel(RGradientType::NONE);

    auto nodeList = theDomain->findChildren<RNode*>();
    for(auto&& it: nodeList)
    {
        // qDebug()<<"Node with tag: "<<it->getObjectTag()<<"and name"<<it->objectName()<<"\n";
        auto vecDisp = it->getDisp();
        for(auto i = 0; i< vecDisp.Size(); ++i)
        {
            if(fabs(vecDisp(i))>fabs(maxDisplacement))
            {
                maxDisplacement = vecDisp(i);
                // qDebug()<<"xDisp: "<<it->getXDisplacement();
                // qDebug()<<"yDisp: "<<it->getYDisplacement();
                // qDebug()<<"zDisp: "<<it->getZDisplacement();
            }
        }
    }

    auto componentList = theDomain->findChildren<RComponent*>();
    for(auto&& it: componentList)
    {
        it->commitState();
    }

    qDebug()<<"After "<<numIt<<" iterations, the maximum displacement is:";
    qDebug()<<maxDisplacement<<" m\n";

    return 0;
}


int RInelasticStaticStructuralAnalysis::createOpenSeesLinearStaticAnalysis(void)
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
    auto theInt = dynamic_cast<StaticIntegrator*>(theIntegrator->getTheIntegrator());
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
        qCritical()<<"Error: the OpenSees 'Equation Solution Algorithm' failed to initialize in "<<__FUNCTION__;
        return -1;
    }


    theLinearStaticAnalysis = std::make_unique<StaticAnalysis>(*theOpenSeesDomain,
                                                               *theHandler,
                                                               *theNumberer,
                                                               *theAnalysisModel,
                                                               *theSolnAlgo,
                                                               *theSOE,
                                                               *theInt,
                                                               theTest);

    if(theLinearStaticAnalysis == nullptr)
    {
        qCritical()<<"Error: failed to create the linear static analysis in "<<__FUNCTION__;
        return -1;
    }

    return 0;
}


