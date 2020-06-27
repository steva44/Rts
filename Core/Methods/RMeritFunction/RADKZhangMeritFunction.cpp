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

#include "RADKZhangMeritFunction.h"
#include "RMatrixOperations.h"

#include <math.h>

RADKZhangMeritFunction::RADKZhangMeritFunction(QObject *parent, QString name)
	: RMeritFunction(parent, name)
{
    theFactorC = 2.0;
}


RADKZhangMeritFunction::~RADKZhangMeritFunction()
{

}


double RADKZhangMeritFunction::getFactorC() const
{	
    return theFactorC;
}	


void RADKZhangMeritFunction::setFactorC(double value)
{	
    theFactorC = value;
}	


double RADKZhangMeritFunction::getMeritFunctionValue(double objectiveFunction, QVector<double> *equalityConstraints, QVector<double> *inequalityConstraints)
{
    return 0.5 * objectiveFunction * objectiveFunction + theFactorC * equalityConstraints->at(0);
}
