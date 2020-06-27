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

#include "RHardcodedExpressionModel.h"
#include "RDomain.h"
#include "RParameter.h"
#include "RResponse.h"

#include <math.h>

RHardcodedExpressionModel::RHardcodedExpressionModel(QObject *parent, QString name)
	: RModel(parent, name)
{
    theA = nullptr;
    theB = nullptr;
    theC = nullptr;
    theEvaluationCount = 0;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RHardcodedExpressionModel::~RHardcodedExpressionModel()
{

}


QObject * RHardcodedExpressionModel::getA() const
{
    return theA;
}


void RHardcodedExpressionModel::setA(QObject *value)
{
    theA = qobject_cast<RParameter *>(value);
}


QObject * RHardcodedExpressionModel::getB() const
{
    return theB;
}


void RHardcodedExpressionModel::setB(QObject *value)
{
    theB = qobject_cast<RParameter *>(value);
}


QObject * RHardcodedExpressionModel::getC() const
{
    return theC;
}


void RHardcodedExpressionModel::setC(QObject *value)
{
    theC = qobject_cast<RParameter *>(value);
}


int RHardcodedExpressionModel::getEvaluationCount() const
{
    return theEvaluationCount;
}


void RHardcodedExpressionModel::setEvaluationCount(int value)
{
    theEvaluationCount = value;
}


QList<RParameter *> RHardcodedExpressionModel::getPhysicalParameterList()
{
	QList<RParameter *> paramList;
    paramList << theA << theB << theC;
	return paramList;
}


int RHardcodedExpressionModel::evaluateModel(RGradientType theGradientType)
{	
    theEvaluationCount++;

    double RV1 = theA->getCurrentValue();
    double RV2 = theB->getCurrentValue();
    double RV3 = theC->getCurrentValue();

    double response = RV2/(1000.0*RV3) + (RV1/(200.0*RV3)) * (RV1/(200.0*RV3));

    theResponse->setCurrentValue(response);

    if (theGradientType >= RGradientType::DDM) {

        double dResponse_dRV1 = RV1/(20000.0*RV3*RV3);
        double dResponse_dRV2 = 1.0/(1000.0*RV3);
        double dResponse_dRV3 = -(RV1*RV1/(20000.0*RV3*RV3*RV3) + RV2/(1000.0*RV3*RV3));

        QMap<RParameter *, double> theResponseGradientMap;
        theResponseGradientMap.insertMulti(theA, dResponse_dRV1);
        theResponseGradientMap.insertMulti(theB, dResponse_dRV2);
        theResponseGradientMap.insertMulti(theC, dResponse_dRV3);

        theDDMMap.insertMulti(theResponse, theResponseGradientMap);
	}

	return 1;
}	
