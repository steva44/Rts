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

#include "RKrylovNewtonEquationSolutionAlgorithm.h"
#include "KrylovNewton.h"
#include "ConvergenceTest.h"
#include "RConvergenceTest.h"

RKrylovNewtonEquationSolutionAlgorithm::RKrylovNewtonEquationSolutionAlgorithm(QObject *parent, QString name)
    : REquiSolnAlgo(parent, name)
{
    theEquiSolutionAlgo = nullptr;
    theConvergenceTest = nullptr;
}


RKrylovNewtonEquationSolutionAlgorithm::~RKrylovNewtonEquationSolutionAlgorithm()
{

}


QObject *RKrylovNewtonEquationSolutionAlgorithm::getTheConvergenceTest() const
{
    return theConvergenceTest;
}


void RKrylovNewtonEquationSolutionAlgorithm::setTheConvergenceTest(QObject *value)
{
    auto valueCast = qobject_cast<RConvergenceTest*>(value);
    theConvergenceTest = valueCast;
}


EquiSolnAlgo* RKrylovNewtonEquationSolutionAlgorithm::getTheEquationSolutionAlgo()
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

        //        KrylovNewton(ConvergenceTest &theTest, int tangent = CURRENT_TANGENT, double iFactor = 0.0, double cFactor = 1.0);
        theEquiSolutionAlgo = std::make_unique<KrylovNewton>(*theTest);
    }

    return theEquiSolutionAlgo.get();
}


