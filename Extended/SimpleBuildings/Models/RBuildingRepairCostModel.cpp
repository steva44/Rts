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

#include "RBuildingRepairCostModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RBuildingInformationModel.h"

#include <math.h>

RBuildingRepairCostModel::RBuildingRepairCostModel(QObject *parent, QString name)
	: RModel(parent, name)
{
	theBuildingInformationModel = 0;
	theStructuralDamageRatio = 0;
	theNonstructuralDriftDamageRatio = 0;
	theNonstructuralAccelerationDamageRatio = 0;
	theContentDamageRatio = 0;
	theStructuralUnitAreaCost = 0;
	theNonstructuralDriftUnitAreaCost = 0;
	theNonstructuralAccelerationUnitAreaCost = 0;
	theContentUnitAreaCost = 0;
	theTime = 0;
	theAnnualInterestRate = 0;
	theEpsilon = 0;
	
	theDomain->createObject(objectName() + "Response", "RGenericResponse");
	theRepairCost = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
	theRepairCost->setModel(this);
}

RBuildingRepairCostModel::~RBuildingRepairCostModel()
{

}

QObject * RBuildingRepairCostModel::getBuildingInformationModel() const	
{	
	return theBuildingInformationModel;
}	
	
void RBuildingRepairCostModel::setBuildingInformationModel(QObject *value)	
{	
	theBuildingInformationModel = qobject_cast<RModel *>(value);	// Getting the information from the BIM
}	

QObject * RBuildingRepairCostModel::getStructuralDamageRatio() const	
{	
	return theStructuralDamageRatio;
}	
	
void RBuildingRepairCostModel::setStructuralDamageRatio(QObject *value)	
{	
	theStructuralDamageRatio = qobject_cast<RParameter *>(value);
}	
	
QObject * RBuildingRepairCostModel::getNonstructuralDriftDamageRatio() const	
{	
	return theNonstructuralDriftDamageRatio;
}	
	
void RBuildingRepairCostModel::setNonstructuralDriftDamageRatio(QObject *value)	
{	
	theNonstructuralDriftDamageRatio = qobject_cast<RParameter *>(value);
}	
	
QObject * RBuildingRepairCostModel::getNonstructuralAccelerationDamageRatio() const	
{	
	return theNonstructuralAccelerationDamageRatio;
}	
	
void RBuildingRepairCostModel::setNonstructuralAccelerationDamageRatio(QObject *value)	
{	
	theNonstructuralAccelerationDamageRatio = qobject_cast<RParameter *>(value);
}	
	
QObject * RBuildingRepairCostModel::getContentDamageRatio() const	
{	
	return theContentDamageRatio;
}	
	
void RBuildingRepairCostModel::setContentDamageRatio(QObject *value)	
{	
	theContentDamageRatio = qobject_cast<RParameter *>(value);
}

double RBuildingRepairCostModel::getStructuralUnitAreaCost() const	
{	
	return theStructuralUnitAreaCost;
}	
	
void RBuildingRepairCostModel::setStructuralUnitAreaCost(double value)	
{	
	theStructuralUnitAreaCost = value;
}	
	
double RBuildingRepairCostModel::getNonstructuralDriftUnitAreaCost() const	
{	
	return theNonstructuralDriftUnitAreaCost;
}	
	
void RBuildingRepairCostModel::setNonstructuralDriftUnitAreaCost(double value)	
{	
	theNonstructuralDriftUnitAreaCost = value;
}	
	
double RBuildingRepairCostModel::getNonstructuralAccelerationUnitAreaCost() const	
{	
	return theNonstructuralAccelerationUnitAreaCost;
}	
	
void RBuildingRepairCostModel::setNonstructuralAccelerationUnitAreaCost(double value)	
{	
	theNonstructuralAccelerationUnitAreaCost = value;
}	
	
double RBuildingRepairCostModel::getContentUnitAreaCost() const	
{	
	return theContentUnitAreaCost;
}	
	
void RBuildingRepairCostModel::setContentUnitAreaCost(double value)	
{	
	theContentUnitAreaCost = value;
}	
QObject * RBuildingRepairCostModel::getTime() const	
{	
	return theTime;
}	
	
void RBuildingRepairCostModel::setTime(QObject *value)	
{	
	theTime = qobject_cast<RParameter *>(value);
}	

QObject * RBuildingRepairCostModel::getAnnualInterestRate() const	
{	
	return theAnnualInterestRate;
}	
	
void RBuildingRepairCostModel::setAnnualInterestRate(QObject *value)	
{	
	theAnnualInterestRate = qobject_cast<RParameter *>(value);
}	
	
QObject * RBuildingRepairCostModel::getEpsilon() const	
{	
	return theEpsilon;
}	
	
void RBuildingRepairCostModel::setEpsilon(QObject *value)	
{	
	theEpsilon = qobject_cast<RParameter *>(value);
}	

int RBuildingRepairCostModel::evaluateModel(RGradientType theGradientType)
{	
	// Looking up structural damage model parameters
	double DFs = theStructuralDamageRatio->getCurrentValue();
	double DFnd = theNonstructuralDriftDamageRatio->getCurrentValue();
	double DFna = theNonstructuralAccelerationDamageRatio->getCurrentValue();
	double DFc = theContentDamageRatio->getCurrentValue();
	double currentTime = theTime->getCurrentValue();
	double interestRate = theAnnualInterestRate->getCurrentValue();
	double epsilon = theEpsilon->getCurrentValue();
	
	// Getting the information from the BIM
	RBuildingInformationModel *theBIM = qobject_cast<RBuildingInformationModel *>(theBuildingInformationModel);
	if (!theBIM) {
		qCritical() << "Error in model" << objectName() << ":" << theBuildingInformationModel->objectName() << "is not a building information model.";
		return -1;
	}
	double totalArea = theBIM->getTotalArea();

	double repairCost = (DFs * theStructuralUnitAreaCost + DFnd * theNonstructuralDriftUnitAreaCost + DFna * theNonstructuralAccelerationUnitAreaCost + DFc * theContentUnitAreaCost) * totalArea * epsilon;

	double futureDiscountFactor = 1.0;
	if (currentTime > theInitialTime) {
		//futureDiscountFactor = pow(1 + interestRate, -(currentTime - theInitialTime) / 365.0);
		futureDiscountFactor = exp(-interestRate * (currentTime - theInitialTime) / 365.0);
	}

	repairCost *= futureDiscountFactor;

	theRepairCost->setCurrentValue(repairCost);

    if (theGradientType == RGradientType::DDM) {
		// IMPLEMENT
	}

	return 0;
}

void RBuildingRepairCostModel::reset()
{
	theInitialTime = theTime->getCurrentValue();
}
