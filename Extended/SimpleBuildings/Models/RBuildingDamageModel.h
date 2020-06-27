#ifndef RBuildingDamageModel_H
#define RBuildingDamageModel_H
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
#include "RProbabilityDistributions.h"
#include "RContinuousRandomVariable.h"

class RParameter;
class RResponse;

class RBuildingDamageModel : public RModel
{
	Q_OBJECT	
		
	Q_PROPERTY(QObject *BuildingInformationModel READ getBuildingInformationModel WRITE setBuildingInformationModel)
	Q_PROPERTY(QObject *PeakDriftRatio READ getPeakDriftRatio WRITE setPeakDriftRatio)
	Q_PROPERTY(QObject *PeakAcceleration READ getPeakAcceleration WRITE setPeakAcceleration)
    Q_PROPERTY(QObject *ProbabilityDistributions READ getProbabilityDistributions WRITE setProbabilityDistributions)
    Q_PROPERTY(QString BuildingType READ getBuildingType WRITE setBuildingType)


public:
	RBuildingDamageModel(QObject *parent, QString name);
	~RBuildingDamageModel();

	QObject *getBuildingInformationModel() const;
	void setBuildingInformationModel(QObject *value);

	QObject *getPeakDriftRatio() const;
	void setPeakDriftRatio(QObject *value);

	QObject *getPeakAcceleration() const;
	void setPeakAcceleration(QObject *value);

    QObject *getProbabilityDistributions() const;
    void setProbabilityDistributions(QObject *value);

    QString getBuildingType();
    void setBuildingType(QString value);

	int evaluateModel(bool evaluateDDM);
	void updateHistoryVariables();
	void resetHistoryVariables();

private:
	QPointer<RModel> theBuildingInformationModel;
	QPointer<RParameter> thePeakDriftRatio;
	QPointer<RParameter> thePeakAcceleration;
    QPointer<RProbabilityDistributions> theProbabilityDistributions;
    QString theBuildingType;

    QPointer<RContinuousRandomVariable> theTheta1;
    QPointer<RContinuousRandomVariable> theTheta2;
    QPointer<RContinuousRandomVariable> theTheta3;
    QPointer<RContinuousRandomVariable> theTheta4;
    QPointer<RContinuousRandomVariable> theTheta5;
    QPointer<RContinuousRandomVariable> theTheta6;
    QPointer<RContinuousRandomVariable> theTheta7;
    QPointer<RContinuousRandomVariable> theTheta8;
    QPointer<RContinuousRandomVariable> theTheta9;
    QPointer<RContinuousRandomVariable> theTheta10;
    QPointer<RContinuousRandomVariable> theTheta11;
    QPointer<RContinuousRandomVariable> theTheta12;
    QPointer<RContinuousRandomVariable> theTheta13;
    QPointer<RContinuousRandomVariable> theTheta14;
    QPointer<RContinuousRandomVariable> theTheta15;
    QPointer<RContinuousRandomVariable> theEpsilon1;
    QPointer<RContinuousRandomVariable> theEpsilon2;
    QPointer<RContinuousRandomVariable> theEpsilon3;
    QPointer<RContinuousRandomVariable> theEpsilon4;

	RResponse *theStructuralDamageRatio;
	RResponse *theNonstructuralDriftDamageRatio;
	RResponse *theNonstructuralAccelerationDamageRatio;
	RResponse *theContentDamageRatio;

};

#endif // RBuildingDamageModel_H
