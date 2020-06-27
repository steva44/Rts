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

#include "RNewmarkIntegrator.h"
#include "Newmark.h"

RNewmarkIntegrator::RNewmarkIntegrator(QObject *parent, QString name)
    : RIntegrator(parent, name)
{
    gamma = 0.5;
    beta = 0.25;

    theIntegrator = nullptr;
}


RNewmarkIntegrator::~RNewmarkIntegrator()
{

}


double RNewmarkIntegrator::getGamma() const
{
    return gamma;
}


void RNewmarkIntegrator::setGamma(double value)
{
    gamma = value;
}


double RNewmarkIntegrator::getBeta() const
{
    return beta;
}


void RNewmarkIntegrator::setBeta(double value)
{
    beta = value;
}


int RNewmarkIntegrator::getMaxSteps()
{
    return  0;
}


Integrator* RNewmarkIntegrator::getTheIntegrator()
{
    if(!theIntegrator)
        theIntegrator = std::make_unique<Newmark>(gamma, beta);

    return theIntegrator.get();
}

