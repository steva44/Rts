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

#include "RBroydenEquationSolutionAlgorithm.h"
#include "Broyden.h"
#include "ConvergenceTest.h"
#include "RConvergenceTest.h"

RBroydenEquationSolutionAlgorithm::RBroydenEquationSolutionAlgorithm(QObject *parent, QString name)
    : REquiSolnAlgo(parent, name)
{
    theEquiSolutionAlgo = nullptr;
    theConvergenceTest = nullptr;

    numStepsNewTangent = 10;
}


RBroydenEquationSolutionAlgorithm::~RBroydenEquationSolutionAlgorithm()
{

}


QObject *RBroydenEquationSolutionAlgorithm::getTheConvergenceTest() const
{
    return theConvergenceTest;
}


void RBroydenEquationSolutionAlgorithm::setTheConvergenceTest(QObject *value)
{
    auto valueCast = qobject_cast<RConvergenceTest*>(value);
    theConvergenceTest = valueCast;
}


EquiSolnAlgo* RBroydenEquationSolutionAlgorithm::getTheEquationSolutionAlgo()
{
    if(!theEquiSolutionAlgo)
    {

        if(theConvergenceTest == nullptr)
        {
            qDebug()<<"The "<<__FUNCTION__<<" needs a convergence test";
            return nullptr;
        }

        auto theTest = dynamic_cast<ConvergenceTest*>(theConvergenceTest->getTheConvergenceTest());
        if(theTest == nullptr)
        {
            qCritical()<<"Error: the OpenSees 'Convergence Test' failed to initialize in "<<__FUNCTION__;
            return nullptr;
        }

        // Broyden(int tangent = CURRENT_TANGENT, int n = 10);
        theEquiSolutionAlgo = std::make_unique<Broyden>(*theTest,CURRENT_TANGENT,numStepsNewTangent);
    }

    return theEquiSolutionAlgo.get();
}


int RBroydenEquationSolutionAlgorithm::getNumStepsNewTangent() const
{
    return numStepsNewTangent;
}


void RBroydenEquationSolutionAlgorithm::setNumStepsNewTangent(int value)
{
    numStepsNewTangent = value;
}








