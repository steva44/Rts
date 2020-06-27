#ifndef RSimpleBuildingDamageModel_H
#define RSimpleBuildingDamageModel_H
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
*   - Stevan Gavrilovic                                              *
*   - Rodrigo Costa                                                  *
*                                                                    *
*********************************************************************/

#include "RModel.h"

class RContinuousRandomVariable;
class RParameter;
class RResponse;
class RBIM;

class RSimpleBuildingDamageModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
    Q_PROPERTY(QObject *PeakDrift READ getPeakDrift WRITE setPeakDrift)
    Q_PROPERTY(QObject *PeakAcceleration READ getPeakAcceleration WRITE setPeakAcceleration)
    Q_PROPERTY(QObject *ResidualDrift READ getResidualDrift WRITE setResidualDrift)

public:
    RSimpleBuildingDamageModel(QObject *parent, QString name);
    ~RSimpleBuildingDamageModel();

    QObject *getPeakDrift() const;
    void setPeakDrift(QObject *value);

    QObject *getPeakAcceleration() const;
    void setPeakAcceleration(QObject *value);

    QObject *getResidualDrift() const;
    void setResidualDrift(QObject *value);

    QObject *getBIM() const;
    void setBIM(QObject *value);

    QString getTenure() const;
    void setTenure(QString value);

    double getEffectiveDamageRatio();
    double getStrDamageRatio();
    double getNSDDamageRatio();
    double getNSADamageRatio();
    double getContentDamageRatio();

    int evaluateModel(RGradientType theGradientType);

private:

    RBIM* theBIM;

    RResponse *thePeakDrift;
    RResponse *thePeakAcceleration;
    RResponse *theResidualDriftResponse;
    RParameter *theLocation;
    QString theBuildingType;
    QString theOccupancyClass;
    QString theTenure;
    QString theCodeDesignLevel;
    int theNumberOfStoreys;

    QPointer<RContinuousRandomVariable> theDamageTheta1;
    QPointer<RContinuousRandomVariable> theDamageTheta2;
    QPointer<RContinuousRandomVariable> theDamageTheta3;
    QPointer<RContinuousRandomVariable> theDamageTheta4;
    QPointer<RContinuousRandomVariable> theDamageTheta5;
    QPointer<RContinuousRandomVariable> theDamageTheta6;
    QPointer<RContinuousRandomVariable> theDamageTheta7;
    QPointer<RContinuousRandomVariable> theDamageTheta8;
    QPointer<RContinuousRandomVariable> theDamageTheta9;
    QPointer<RContinuousRandomVariable> theDamageTheta10;
    QPointer<RContinuousRandomVariable> theDamageTheta11;
    QPointer<RContinuousRandomVariable> theDamageTheta12;
    QPointer<RContinuousRandomVariable> theDamageTheta13;
    QPointer<RContinuousRandomVariable> theDamageTheta14;
    QPointer<RContinuousRandomVariable> theDamageTheta15;
    QPointer<RContinuousRandomVariable> theDamageEpsilon1;
    QPointer<RContinuousRandomVariable> theDamageEpsilon2;
    QPointer<RContinuousRandomVariable> theDamageEpsilon3;
    QPointer<RContinuousRandomVariable> theDamageEpsilon4;

    RResponse *theStructuralDamageRatio;
    RResponse *theNonstructuralDriftDamageRatio;
    RResponse *theNonstructuralAccelerationDamageRatio;
    RResponse *theEffectiveDamageRatio;
    RResponse *theContentDamageRatio;
};

#endif
