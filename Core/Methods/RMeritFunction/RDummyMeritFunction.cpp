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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RDummyMeritFunction.h"
#include "RMatrixOperations.h"
#include <math.h>

RDummyMeritFunction::RDummyMeritFunction(QObject *parent, QString name)
	: RMeritFunction(parent, name)
{
    theObjectiveMultiplier = 1.0;
    theObjectivePower = 1.0;
    theConstraintMultiplier = 1.0;
    theConstraintPower = 1.0;
}


RDummyMeritFunction::~RDummyMeritFunction()
{

}


double RDummyMeritFunction::getObjectiveMultiplier() const
{	
    return theObjectiveMultiplier;
}	


void RDummyMeritFunction::setObjectiveMultiplier(double value)
{	
    theObjectiveMultiplier = value;
}	


double RDummyMeritFunction::getObjectivePower() const
{
    return theObjectivePower;
}


void RDummyMeritFunction::setObjectivePower(double value)
{
    theObjectivePower = value;
}


double RDummyMeritFunction::getConstraintMultiplier() const
{
    return theConstraintMultiplier;
}


void RDummyMeritFunction::setConstraintMultiplier(double value)
{
    theConstraintMultiplier = value;
}


double RDummyMeritFunction::getConstraintPower() const
{
    return theConstraintPower;
}


void RDummyMeritFunction::setConstraintPower(double value)
{
    theConstraintPower = value;
}


double RDummyMeritFunction::getMeritFunctionValue(double objectiveFunction, QVector<double> *equalityConstraints, QVector<double> *inequalityConstraints)
{
    double poweredConstraintSum = 0.0;

    if (equalityConstraints) {

        for (int i=0; i<equalityConstraints->size(); i++) {

            poweredConstraintSum += pow(equalityConstraints->at(i), theConstraintPower);
        }
    }

    if (inequalityConstraints) {

        for (int i=0; i<inequalityConstraints->size(); i++) {

            poweredConstraintSum += pow(inequalityConstraints->at(i), theConstraintPower);
        }
    }

    return theObjectiveMultiplier * pow(objectiveFunction, theObjectivePower) + theConstraintMultiplier * poweredConstraintSum;
}
