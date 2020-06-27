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
*   - Mojtaba Mahsuli                                                *
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RContinuousDecisionVariable.h"
#include "RMainWindow.h"

RContinuousDecisionVariable::RContinuousDecisionVariable(QObject *parent, QString name)
	: RDecisionVariable(parent, name)
{
    theInitialValueParameter = nullptr;
    theUpperBoundParameter = nullptr;
    theLowerBoundParameter = nullptr;
    theIncrementalCostParameter = nullptr;
	theInitialValue = 0;
	theUpperBound = 0;
	theLowerBound = 0;
	theIncrementalCost = 1;
	theBetaSensitivityWrtCost = 0;
	thePfSensitivityWrtCost = 0;
}


RContinuousDecisionVariable::~RContinuousDecisionVariable()
{

}


QStringList RContinuousDecisionVariable::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    return actionList;
}


QString RContinuousDecisionVariable::getInitialValue()
{
	return getStringParameter(&theInitialValue, theInitialValueParameter);
}


void RContinuousDecisionVariable::setInitialValue(QString value)
{
	setStringParameter(value, &theInitialValue, &theInitialValueParameter);
	theCurrentValue = getInitialValueValue();
}


QString RContinuousDecisionVariable::getUpperBound()
{
	return getStringParameter(&theUpperBound, theUpperBoundParameter);
}


void RContinuousDecisionVariable::setUpperBound(QString value)
{
	setStringParameter(value, &theUpperBound, &theUpperBoundParameter);
}


QString RContinuousDecisionVariable::getLowerBound()
{
	return getStringParameter(&theLowerBound, theLowerBoundParameter);
}


void RContinuousDecisionVariable::setLowerBound(QString value)
{
	setStringParameter(value, &theLowerBound, &theLowerBoundParameter);
}


QString RContinuousDecisionVariable::getIncrementalCost()
{
	return getStringParameter(&theIncrementalCost, theIncrementalCostParameter);
}


void RContinuousDecisionVariable::setIncrementalCost(QString value)
{
	setStringParameter(value, &theIncrementalCost, &theIncrementalCostParameter);
}


double RContinuousDecisionVariable::getInitialValueValue()
{
	return getDoubleParameter(&theInitialValue, theInitialValueParameter);
}


double RContinuousDecisionVariable::getUpperBoundValue()
{
	return getDoubleParameter(&theUpperBound, theUpperBoundParameter);
}


double RContinuousDecisionVariable::getLowerBoundValue()
{
	return getDoubleParameter(&theLowerBound, theLowerBoundParameter);
}


double RContinuousDecisionVariable::getIncrementalCostValue()
{
	return getDoubleParameter(&theIncrementalCost, theIncrementalCostParameter);
}


double RContinuousDecisionVariable::getBetaSensitivityWrtCost()
{
	return theBetaSensitivityWrtCost;
}


void RContinuousDecisionVariable::setBetaSensitivityWrtCost(double value)
{
	theBetaSensitivityWrtCost = value;
}


double RContinuousDecisionVariable::getPfSensitivityWrtCost()
{
	return thePfSensitivityWrtCost;
}


void RContinuousDecisionVariable::setPfSensitivityWrtCost(double value)
{
	thePfSensitivityWrtCost = value;
}


QList<RParameter *> RContinuousDecisionVariable::getParameterList()
{
	QList<RParameter *> theParameterList;
	if (theInitialValueParameter) {
		theParameterList << theInitialValueParameter;
	}
	if (theUpperBoundParameter) {
		theParameterList << theUpperBoundParameter;
	}
	if (theLowerBoundParameter) {
		theParameterList << theLowerBoundParameter;
	}
	if (theIncrementalCostParameter) {
		theParameterList << theIncrementalCostParameter;
	}
	return theParameterList;
}


void RContinuousDecisionVariable::setCurrentValue(double value)
{
    auto valueToSet = 0.0;

    //Check to see if the value is within the upper and lower bounds; use constraints if not.

    //The first case where both of the bounds are zero IE unconstrained optimization
    if(theLowerBound == 0.0 && theUpperBound == 0.0)
        valueToSet = value;
    else if(value <= theLowerBound)
        valueToSet = theLowerBound;
    else if(value >= theUpperBound)
        valueToSet = theUpperBound;
    else
        valueToSet = value;

    // Set this variables current value
    this->theCurrentValue = valueToSet;

}


void RContinuousDecisionVariable::setCurrentValue(std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> LambdaValue)
{
    qCritical()<<"Error: only double values should be set as RDecisionVariables";
}


