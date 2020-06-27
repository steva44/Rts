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

#include "RLoadControlIntegrator.h"
#include "LoadControl.h"
#include "Integrator.h"

RLoadControlIntegrator::RLoadControlIntegrator(QObject *parent, QString name)
    : RIntegrator(parent, name)
{
    minLambda = nullptr;
    maxlambda = nullptr;
    theIntegrator = nullptr;

    deltaLambda = 1.0;
    numIncrements = -1;
}


RLoadControlIntegrator::~RLoadControlIntegrator()
{

}


double RLoadControlIntegrator::getDeltaLambda() const
{
    return deltaLambda;
}


void RLoadControlIntegrator::setDeltaLambda(double value)
{
    deltaLambda = value;
}


int RLoadControlIntegrator::getNumIncr() const
{  
    return numIncrements;
}


void RLoadControlIntegrator::setNumIncr(int value)
{
    numIncrements = value;
}


double RLoadControlIntegrator::getMinLambda() const
{
    return *minLambda;
}


void RLoadControlIntegrator::setMinLambda(double value)
{
    minLambda = std::make_unique<double>(value);
}


double RLoadControlIntegrator::getMaxlambda() const
{
    return *maxlambda;
}


void RLoadControlIntegrator::setMaxlambda(double value)
{
    maxlambda = std::make_unique<double>(value);
}


int RLoadControlIntegrator::getMaxSteps()
{
    return  numIncrements;
}


Integrator* RLoadControlIntegrator::getTheIntegrator()
{
    // The defaults
    auto minLamVal = deltaLambda;
    auto maxLamVal = deltaLambda;

    //Set the optional arguments if they exist
    if(minLambda)
        minLamVal = *minLambda;

    if(maxlambda)
        maxLamVal = *maxlambda;

    if(numIncrements == -1)
    {
        qCritical()<<"Error, need to set the number of increments parameter in "<<this->objectName();
        return nullptr;
    }

    // LoadControl(double deltaLambda, int numIncr, double minLambda, double maxlambda)
    if(!theIntegrator)
        theIntegrator = std::make_unique<LoadControl>(deltaLambda, numIncrements, minLamVal, maxLamVal);

    return theIntegrator.get();
}


