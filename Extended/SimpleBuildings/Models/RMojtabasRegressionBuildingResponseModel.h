#ifndef RMojtabasRegressionBuildingResponseModel_H
#define RMojtabasRegressionBuildingResponseModel_H
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
#include "RContinuousRandomVariable.h"
#include "RBIM.h"

class RParameter;
class RResponse;

class RMojtabasRegressionBuildingResponseModel : public RModel
{
	Q_OBJECT	
		
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
	Q_PROPERTY(QObject *Sa READ getSa WRITE setSa)

public:

	RMojtabasRegressionBuildingResponseModel(QObject *parent, QString name);
	~RMojtabasRegressionBuildingResponseModel();

    QObject *getBIM() const;
    void setBIM(QObject *value);

	QObject *getSa() const;
	void setSa(QObject *value);

    int evaluateModel(RGradientType theGradientType);

private:

    QPointer<RBIM> theBIM = nullptr;
    QPointer<RParameter> theSa = nullptr;

    QPointer<RContinuousRandomVariable> theResponseTheta1;
    QPointer<RContinuousRandomVariable> theResponseTheta2;
    QPointer<RContinuousRandomVariable> theResponseTheta3;
    QPointer<RContinuousRandomVariable> theResponseTheta4;
    QPointer<RContinuousRandomVariable> theResponseTheta5;
    QPointer<RContinuousRandomVariable> theResponseTheta6;
    QPointer<RContinuousRandomVariable> theResponseTheta7;
    QPointer<RContinuousRandomVariable> theResponseTheta8;
    QPointer<RContinuousRandomVariable> theResponseTheta9;
    QPointer<RContinuousRandomVariable> theResponseTheta10;
    QPointer<RContinuousRandomVariable> theResponseTheta11;
    QPointer<RContinuousRandomVariable> theResponseTheta12;
    QPointer<RContinuousRandomVariable> theResponseTheta13;
    QPointer<RContinuousRandomVariable> theResponseTheta14;
    QPointer<RContinuousRandomVariable> theResponseTheta15;
    QPointer<RContinuousRandomVariable> theResponseTheta16;
    QPointer<RContinuousRandomVariable> theResponseTheta17;
    QPointer<RContinuousRandomVariable> theResponseTheta18;
    QPointer<RContinuousRandomVariable> theResponseTheta19;
    QPointer<RContinuousRandomVariable> theResponseTheta20;
    QPointer<RContinuousRandomVariable> theResponseTheta21;
    QPointer<RContinuousRandomVariable> theResponseSigma1;
    QPointer<RContinuousRandomVariable> theResponseSigma2;
    QPointer<RContinuousRandomVariable> theResponseEpsilon1;
    QPointer<RContinuousRandomVariable> theResponseEpsilon2;

    RResponse *thePeakDriftResponse;
    RResponse *thePeakAccelerationResponse;
};

#endif
