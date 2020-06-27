#ifndef RStevesOperationImpactModel_H
#define RStevesOperationImpactModel_H
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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

// This model calculates the energy, water, time, cost, and emissions from building operations

#include "RBIM.h"
#include "RModel.h"
#include "RResponse.h"
#include "RInfoRichResponse.h"

class RBIM;
class RContinuousRandomVariable;
class RResponse;
class RInfoRichResponse;
class RStatistics;

class RStevesOperationImpactModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)

public:
    RStevesOperationImpactModel(QObject *parent, QString name);
    ~RStevesOperationImpactModel() override;

    // Get and set the building information model
    QObject *getBIM() const;
    void setBIM(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

    // Get the trigger times, i.e., the times when the operations model will evaluate, e.g., every three months
    QList<QPair<double, QString> > getTriggerTimes(double startTime, double endTime) override;

    int resetTime() override;

private:

    // Create random variables that are used in this class
    void createDatabaseOfRandomVariables();

    // Objects and methods used by this class
    QPointer<RBIM> theBIM;
    RStatistics* theStatsMethod;

    // The responses that this model returns
    QPointer<RResponse> theCostResponse;
    QPointer<RResponse> theWaterResponse;
    QPointer<RResponse> theTimeResponse;
    QPointer<RInfoRichResponse> theEmissionsResponse;
    QPointer<RInfoRichResponse> theEnergyResponse;

    // The number of days that define an operations period
    int daysPeriod;

    // The indoor ambient temp
    double ambientTemp;

    // The last "time" in the buildings life that the operations model was run
    double thePreviousTime;

    // A list of times that the operations model was run
    QList<double> timeList;

    // This function returns the difference between the inside and outside temp of the building
    double deltaTemperature(int month);

    // Random variables for heating and cooling
    RContinuousRandomVariable *heatingCoolingEfficiency;
    RContinuousRandomVariable *airChangesPerHour;

    // The type of heating system, e.g., natural gas, electric
    QString heatingSystemType;
};

#endif
