#ifndef RSimpleBuildingLossModel_H
#define RSimpleBuildingLossModel_H
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

class RParameter;
class RResponse;
class RStatistics;
class RBIM;

class RSimpleBuildingLossModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
    Q_PROPERTY(QObject *PeakDrift READ getPeakDrift WRITE setPeakDrift)
    Q_PROPERTY(QObject *StructuralDamage READ getStructuralDamage WRITE setStructuralDamage)
    Q_PROPERTY(QObject *NSDriftDamage READ getNSDriftDamage WRITE setNSDriftDamage)
    Q_PROPERTY(QObject *NSAccelerationDamage READ getNSAccelerationDamage WRITE setNSAccelerationDamage)
    Q_PROPERTY(QObject *ContentDamageRatios READ getContentDamageRatios WRITE setContentDamageRatios)
    Q_PROPERTY(double BuildingValue READ getBuildingValue WRITE setBuildingValue)  

public:
    RSimpleBuildingLossModel(QObject *parent, QString name);
    ~RSimpleBuildingLossModel();

    int resetTime();

    QObject *getPeakDrift() const;
    void setPeakDrift(QObject *value);

    QObject* getStructuralDamage() const;
    void setStructuralDamage(QObject* value);

    QObject* getNSDriftDamage() const;
    void setNSDriftDamage(QObject* value);

    QObject* getNSAccelerationDamage() const;
    void setNSAccelerationDamage(QObject* value);

    QObject* getBuildingDamageRatios() const;
    void setBuildingDamageRatios(QObject* value);

    QObject* getContentDamageRatios() const;
    void setContentDamageRatios(QObject* value);

    double getBuildingValue() const;
    void setBuildingValue(double value);

    QObject *getBIM() const;
    void setBIM(QObject *value);

    // Define the method where the actual model is coded
    int evaluateModel(RGradientType theGradientType);

    int getFragilityFunctionMediansAndDispersions(const QString& HazusType, QVector<double>& medians, QVector<double>& dispersions);

    QVector<double> getDamageProbabilities(const QVector<double>& medians,const QVector<double>& dispersions, const double& drift);

private:

    // Responses input to this model
    QPointer<RResponse> theStructuralDamageRatioResponse;
    QPointer<RResponse> theNDDamageRatioResponse;
    QPointer<RResponse> theNADamageRatioResponse;
    QPointer<RResponse> theContentDamageRatiosResponse;
    QPointer<RResponse> thePeakDrift;

    RStatistics* theStatsMethod;

    // Building attributes
    RBIM* theBIM;
    double theBuildingValue;   
    QVector<double> theSRepairCost;
    QVector<double> theNSARepairCost;
    QVector<double> theNSDRepairCost;
    QVector<double> theContentReplacementCost;
    QVector<double> theRecoveryTime;

    // Responses from this model
    QPointer<RResponse> theBuildingLossResponse;
    QPointer<RResponse> theDisruptionCostResponse;
    QPointer<RResponse> theContentLossResponse;
    QPointer<RResponse> theTotalLossResponse;
    QPointer<RResponse> theRepairTimeResponse;
};

#endif // RSimpleBuildingLossModel_H
