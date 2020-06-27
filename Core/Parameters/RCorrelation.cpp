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
*                                                                    *
*********************************************************************/

#include "RCorrelation.h"
#include "RRandomVariable.h"
#include "RMainWindow.h"

RCorrelation::RCorrelation(QObject *parent, QString name)
	: RParameter(parent, name)
{
    theRandomVariable1 = nullptr;
    theRandomVariable2 = nullptr;
    theCoefficient = 0.0;
}


RCorrelation::~RCorrelation()
{

}


QStringList RCorrelation::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    return actionList;
}


QObject * RCorrelation::getRandomVariable1() const	
{	
	return theRandomVariable1;
}	
	

void RCorrelation::setRandomVariable1(QObject *value)	
{	
	theRandomVariable1 = qobject_cast<RRandomVariable *>(value);
	
    if (theRandomVariable1 == theRandomVariable2) {
        qCritical() << "Warning: The first and second random variables in the correlation object" << objectName() << "are the same.";
    }
}	
	

QObject * RCorrelation::getRandomVariable2() const	
{	
	return theRandomVariable2;
}	
	

void RCorrelation::setRandomVariable2(QObject *value)	
{	
	theRandomVariable2 = qobject_cast<RRandomVariable *>(value);
	
    if (theRandomVariable1 == theRandomVariable2) {
        qCritical() << "Warning: The first and second random variables in the correlation object" << objectName() << "are the same.";
    }
}	


double RCorrelation::getCoefficient() const	
{	
	return theCoefficient;
}	


void RCorrelation::setCoefficient(double value)	
{	
	if ((value < -1.0) || (value > 1.0)) {
		qCritical() << "Error: The correlation coeffcient given to object" << objectName() << "must be between -1 and 1.";
		return;
	}
	theCoefficient = value;
}	
