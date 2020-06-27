#ifndef RBuildingRepairCostModel_H
#define RBuildingRepairCostModel_H
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

#include "RModel.h"

class RParameter;
class RResponse;

class RBuildingRepairCostModel : public RModel
{
	Q_OBJECT	
		
	Q_PROPERTY(QObject *BuildingInformationModel READ getBuildingInformationModel WRITE setBuildingInformationModel)
	Q_PROPERTY(QObject *StructuralDamageRatio READ getStructuralDamageRatio WRITE setStructuralDamageRatio)
	Q_PROPERTY(QObject *NonstructuralDriftDamageRatio READ getNonstructuralDriftDamageRatio WRITE setNonstructuralDriftDamageRatio)
	Q_PROPERTY(QObject *NonstructuralAccelerationDamageRatio READ getNonstructuralAccelerationDamageRatio WRITE setNonstructuralAccelerationDamageRatio)
	Q_PROPERTY(QObject *ContentDamageRatio READ getContentDamageRatio WRITE setContentDamageRatio)
	Q_PROPERTY(double StructuralUnitAreaCost READ getStructuralUnitAreaCost WRITE setStructuralUnitAreaCost)
	Q_PROPERTY(double NonstructuralDriftUnitAreaCost READ getNonstructuralDriftUnitAreaCost WRITE setNonstructuralDriftUnitAreaCost)
	Q_PROPERTY(double NonstructuralAccelerationUnitAreaCost READ getNonstructuralAccelerationUnitAreaCost WRITE setNonstructuralAccelerationUnitAreaCost)
	Q_PROPERTY(double ContentUnitAreaCost READ getContentUnitAreaCost WRITE setContentUnitAreaCost)
	Q_PROPERTY(QObject *Time READ getTime WRITE setTime)
	Q_PROPERTY(QObject *AnnualInterestRate READ getAnnualInterestRate WRITE setAnnualInterestRate)
	Q_PROPERTY(QObject *Epsilon READ getEpsilon WRITE setEpsilon)

public:
	RBuildingRepairCostModel(QObject *parent, QString name);
	~RBuildingRepairCostModel();

	QObject *getBuildingInformationModel() const;
	void setBuildingInformationModel(QObject *value);
	QObject *getStructuralDamageRatio() const;
	void setStructuralDamageRatio(QObject *value);
	QObject *getNonstructuralDriftDamageRatio() const;
	void setNonstructuralDriftDamageRatio(QObject *value);
	QObject *getNonstructuralAccelerationDamageRatio() const;
	void setNonstructuralAccelerationDamageRatio(QObject *value);
	QObject *getContentDamageRatio() const;
	void setContentDamageRatio(QObject *value);
	double getStructuralUnitAreaCost() const;
	void setStructuralUnitAreaCost(double value);
	double getNonstructuralDriftUnitAreaCost() const;
	void setNonstructuralDriftUnitAreaCost(double value);
	double getNonstructuralAccelerationUnitAreaCost() const;
	void setNonstructuralAccelerationUnitAreaCost(double value);
	double getContentUnitAreaCost() const;
	void setContentUnitAreaCost(double value);
	QObject *getTime() const;
	void setTime(QObject *value);
	QObject *getAnnualInterestRate() const;
	void setAnnualInterestRate(QObject *value);
	QObject *getEpsilon() const;
	void setEpsilon(QObject *value);

	int evaluateModel(RGradientType theGradientType);
	void reset();

private:
	QPointer<RModel> theBuildingInformationModel;
	QPointer<RParameter> theStructuralDamageRatio;
	QPointer<RParameter> theNonstructuralDriftDamageRatio;
	QPointer<RParameter> theNonstructuralAccelerationDamageRatio;
	QPointer<RParameter> theContentDamageRatio;
	double theStructuralUnitAreaCost;
	double theNonstructuralDriftUnitAreaCost;
	double theNonstructuralAccelerationUnitAreaCost;
	double theContentUnitAreaCost;
	QPointer<RParameter> theTime;
	QPointer<RParameter> theAnnualInterestRate;
	QPointer<RParameter> theEpsilon;

	double theInitialTime;

	RResponse *theRepairCost;

};

#endif // RBuildingRepairCostModel_H
