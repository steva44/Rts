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

#include "RLinearStaticStructuralAnalysis.h"
#include "RConstraintHandler.h"
#include "RDOFNumberer.h"
#include "RLinearSOE.h"
#include "RIntegrator.h"
#include "RNode.h"
#include "RElement.h"
#include "RVisualDamageModel.h"
#include "RComponent.h"

#include "Linear.h"
#include "RLinearSeries.h"
#include "EquiSolnAlgo.h"
#include "StaticAnalysis.h"
#include "Domain.h"
#include "StaticIntegrator.h"

#include <math.h>

RLinearStaticStructuralAnalysis::RLinearStaticStructuralAnalysis(QObject *parent, QString name)
    : RStructuralAnalysis(parent, name)
{
    theConstraintHandler = nullptr;
    theDOFNumberer = nullptr;
    theLinearSOESolver = nullptr;
    theIntegrator = nullptr;
    theSolnAlgo = nullptr;
}


RLinearStaticStructuralAnalysis::~RLinearStaticStructuralAnalysis()
{

}


int RLinearStaticStructuralAnalysis::conductStructuralAnalysis()
{ 
    maxDisplacement = 0.0;

    if (!theLinearStaticAnalysis)
    {
        auto res = this->createOpenSeesLinearStaticAnalysis();

        if(res != 0)
            return -1;
    }

    // Perform the analysis - the '1' is for the number of steps
    int numSteps = 1;

    theLinearStaticAnalysis->analyze(numSteps);

    if(theVisualDamageModel)
        theVisualDamageModel->evaluateModel(RGradientType::NONE);

    auto componentList = theDomain->findChildren<RComponent*>();
    for(auto&& it: componentList)
    {
        it->commitState();
    }

    auto nodeList = theDomain->findChildren<RNode*>();

    qDebug()<<"Max Displacement:";
    for(auto&& it: nodeList)
    {
        auto vecDisp = it->getDisp();
        for(auto i = 0; i< vecDisp.Size(); ++i)
        {
            if(fabs(vecDisp(i))>fabs(maxDisplacement))
            {
                maxDisplacement = vecDisp(i);
            }
        }
    }
    qDebug()<<maxDisplacement<<" m\n";


    return 0;
}


QObject *RLinearStaticStructuralAnalysis::getTheConstraintHandler() const
{
    return theConstraintHandler;
}


void RLinearStaticStructuralAnalysis::setTheConstraintHandler(QObject *value)
{
    theConstraintHandler = qobject_cast<RConstraintHandler*>(value);
}


QObject *RLinearStaticStructuralAnalysis::getTheDOFNumberer() const
{
    return theDOFNumberer;
}


void RLinearStaticStructuralAnalysis::setTheDOFNumberer(QObject *value)
{
    theDOFNumberer = qobject_cast<RDOFNumberer*>(value);
}


QObject *RLinearStaticStructuralAnalysis::getTheLinearSOESolver() const
{
    return theLinearSOESolver;
}

void RLinearStaticStructuralAnalysis::setTheLinearSOESolver(QObject *value)
{
    theLinearSOESolver = qobject_cast<RLinearSOE*>(value);
}


QObject *RLinearStaticStructuralAnalysis::getTheIntegrator() const
{
    return theIntegrator;
}


void RLinearStaticStructuralAnalysis::setTheIntegrator(QObject *value)
{
    theIntegrator = qobject_cast<RIntegrator*>(value);
}


int RLinearStaticStructuralAnalysis::createOpenSeesLinearStaticAnalysis(void)
{

    auto theOpenSeesDomain = theDomain->getTheOpenSeesDomain();

    //Run a bunch of checks to ensure that the essential classes were given
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


    //Check to make sure the property objects are not null pointers
    if(theConstraintHandler == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a constraint handler";
        return -1;
    }
    if(theDOFNumberer == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a DOF numberer";
        return -1;
    }
    if(theLinearSOESolver == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a linear solver";
        return -1;
    }
    if(theIntegrator == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a integrator";
        return -1;
    }

    auto theHandler = theConstraintHandler->getTheConstraintHandler();

    if(theHandler == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'Constraint Handler' failed to initialize in "<<__FUNCTION__;
        return -1;
    }

    auto theNumberer = theDOFNumberer->getTheDOFNumberer();
    if(theNumberer == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'DOF Numberer' failed to initialize in "<<__FUNCTION__;
        return -1;
    }

    auto theSOE = theLinearSOESolver->getTheLinearSOE();
    if(theSOE == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'SOE' failed to initialize in "<<__FUNCTION__;
        return -1;
    }

    auto theInt = dynamic_cast<StaticIntegrator*>(theIntegrator->getTheIntegrator());
    if(theInt == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'Integrator' failed to initialize in "<<__FUNCTION__;
        return -1;
    }

    //EquiSolnAlgo of type Linear
    theSolnAlgo = std::make_unique<Linear>();

    if(theSolnAlgo == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'Solution Algorithm' failed creation in "<<__FUNCTION__;
        return -1;
    }


    theLinearStaticAnalysis = std::make_unique<StaticAnalysis>(*theOpenSeesDomain,
                                                               *theHandler,
                                                               *theNumberer,
                                                               *theAnalysisModel,
                                                               *theSolnAlgo,
                                                               *theSOE,
                                                               *theInt);                                                               

    if(theLinearStaticAnalysis == nullptr)
    {
        qCritical()<<"Error: failed to create the linear static analysis in "<<__FUNCTION__;
        return -1;
    }

    return 0;
}




