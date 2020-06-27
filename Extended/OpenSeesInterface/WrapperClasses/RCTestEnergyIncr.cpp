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

#include "RCTestEnergyIncr.h"
#include "CTestEnergyIncr.h"
#include "ConvergenceTest.h"

RCTestEnergyIncr::RCTestEnergyIncr(QObject *parent, QString name)
    : RConvergenceTest(parent, name)
{
    tol = 1.0e-12;
    maxNumIter = 10;
    printFlag = 0;
    normType = 2;

    theConvergenceTest = nullptr;
}


RCTestEnergyIncr::~RCTestEnergyIncr()
{

}


double RCTestEnergyIncr::getTol() const
{
    return tol;
}


void RCTestEnergyIncr::setTol(double value)
{
    tol = value;
}


int RCTestEnergyIncr::getMaxNumIter() const
{  
    return maxNumIter;
}


void RCTestEnergyIncr::setMaxNumIter(int value)
{
    maxNumIter = value;
}


ConvergenceTest* RCTestEnergyIncr::getTheConvergenceTest()
{
    //CTestEnergyIncr(double tol, int maxNumIter, int printFlag, int normType =2, double maxTol = OPS_MAXTOL);

    if(!theConvergenceTest)
        theConvergenceTest = std::make_unique<CTestEnergyIncr>(tol, maxNumIter, printFlag, normType);

    return theConvergenceTest.get();
}


int RCTestEnergyIncr::getPrintFlag() const
{
    return printFlag;
}


void RCTestEnergyIncr::setPrintFlag(int value)
{
    printFlag = value;
}


int RCTestEnergyIncr::getNormType() const
{
    return normType;
}


void RCTestEnergyIncr::setNormType(int value)
{
    normType = value;
}





