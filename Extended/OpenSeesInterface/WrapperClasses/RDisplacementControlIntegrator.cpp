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

#include "RDisplacementControlIntegrator.h"
#include "RPoint.h"
#include "RDomain.h"

#include "DisplacementControl.h"
#include "Integrator.h"

RDisplacementControlIntegrator::RDisplacementControlIntegrator(QObject *parent, QString name)
    : RIntegrator(parent, name)
{
    theIntegrator = nullptr;
    thePoint = nullptr;

    minDelta = 0.0;
    maxDelta = 0.0;
    dispIncrement = -1.0;
    numIncrements = 1;
    dof = -1;
}


RDisplacementControlIntegrator::~RDisplacementControlIntegrator()
{

}


void RDisplacementControlIntegrator::reset()
{
    theIntegrator.reset(nullptr);
}


QObject* RDisplacementControlIntegrator::getPoint() const
{
    return thePoint;
}


void RDisplacementControlIntegrator::setPoint(QObject *value)
{
    auto point = qobject_cast<RPoint*>(value);

    thePoint = point;
}


double RDisplacementControlIntegrator::getDispIncr() const
{
    return dispIncrement;
}


void RDisplacementControlIntegrator::setDispIncr(double value)
{
    dispIncrement = value;
}


int RDisplacementControlIntegrator::getNumIncr() const
{  
    return numIncrements;
}


void RDisplacementControlIntegrator::setNumIncr(int value)
{
    numIncrements = value;
}


int RDisplacementControlIntegrator::getDof() const
{
    return dof;
}


void RDisplacementControlIntegrator::setDof(int value)
{
    dof = value;
}


double RDisplacementControlIntegrator::getMinDelta() const
{
    return minDelta;
}


void RDisplacementControlIntegrator::setMinDelta(double value)
{
    minDelta = value;
}


double RDisplacementControlIntegrator::getMaxDelta() const
{
    return maxDelta;
}


int RDisplacementControlIntegrator::getMaxSteps()
{
    return  numIncrements;
}


void RDisplacementControlIntegrator::setMaxDelta(double value)
{
    maxDelta = value;
}


Integrator* RDisplacementControlIntegrator::getTheIntegrator()
{

    if(thePoint == nullptr)
    {
        qCritical()<<"Need a point as an input in "<<__FUNCTION__;
        return nullptr;
    }

    if(dispIncrement <= 0.0)
    {
        qCritical()<<"The displacement increment must be greater than or equal to zero "<<__FUNCTION__;
        return nullptr;
    }

    if(dof < 1)
    {
        qCritical()<<"The dof number is not valid in "<<__FUNCTION__;
        return nullptr;
    }

    // The defaults
    if(minDelta == 0.0)
     minDelta = dispIncrement;

    if(maxDelta == 0.0)
        maxDelta = dispIncrement;

    auto node = thePoint->getAssociatedNode()->getObjectTag();

    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    if(theOSDomain == nullptr)
    {
        qCritical()<<"Could not get the OpenSees domain in "<<__FUNCTION__;
        return nullptr;
    }


    // DisplacementControl(int node, int dof, double increment, Domain *theDomain, int numIncrStep, double minIncrement, double maxIncrement);
    // Subtract -1 from the dof because 0=x, 1=y, 2=z, etc.
    if(!theIntegrator)
        theIntegrator = std::make_unique<DisplacementControl>(node, dof-1, dispIncrement, theOSDomain, numIncrements, minDelta, maxDelta);

    return theIntegrator.get();
}


