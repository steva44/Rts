#ifndef RStevesConstructionImpactModel_H
#define RStevesConstructionImpactModel_H
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

// This model calculates the energy, water, time, cost, and emissions from construction

#include "RModel.h"

class RBIM;
class RContinuousRandomVariable;
class RResponse;
class RInfoRichResponse;
class RCSISchedulingMethod;
class RStatistics;
class RCSIAssembler;

class RStevesConstructionImpactModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)

public:
    RStevesConstructionImpactModel(QObject *parent, QString name);
    ~RStevesConstructionImpactModel() override;

    // Get and set the building information model
    QObject *getBIM() const;
    void setBIM(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

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

    // Fraction of total worker hours allocated to the use of heavy machinery as determined by the construction type, f^WH
    double heavy_machinery_ratio_high_rise_wood;
    double heavy_machinery_ratio_high_rise_steel;
    double heavy_machinery_ratio_high_rise_concrete;
    double heavy_machinery_ratio_low_rise_wood;
    double heavy_machinery_ratio_low_rise_steel;
    double heavy_machinery_ratio_low_rise_concrete;

    // Fraction of energy source n that is employed in the transportation of workers, f^TW
    double fractionWorkerTransportationAutomobil;
    double fractionWorkerTransportationDieselbus;
    double fractionWorkerTransportationSkytrain;

    // The energy intensities for heavy machinery usage, worker transport, and material transport
    RContinuousRandomVariable *theHeavyMachineryUseIntensity;
    RContinuousRandomVariable *theWorkerTransportationIntensityAutomobil;
    RContinuousRandomVariable *theWorkerTransportationIntensityDieselbus;
    RContinuousRandomVariable *theWorkerTransportationIntensitySkytrain;
    RContinuousRandomVariable *theMaterialTransportationIntensityConcrete;
    RContinuousRandomVariable *theMaterialTransportationIntensitySteel;
    RContinuousRandomVariable *theMaterialTransportationIntensityWood;

    // Methods that provides the CSI codes for the various components and to schedule the workers
    std::unique_ptr<RCSIAssembler> theCSIAssembler;
    std::unique_ptr<RCSISchedulingMethod> theScheduler;

    // Hours per worker shift
    double hours_per_shift;

    // Vector of labour quantities - used to price the labour
    // <CSICode,Unit Quantity,Description>
    QVector<std::tuple<QString,double,QString>> labourQuantityCSICodes;

    // Vector of labour crew rates - used to schedule the labour
    // <CSICode,Crew Code,Unit Quantity,Crew Rate RV>
    QVector<std::tuple<QString,double,RContinuousRandomVariable*>> crewRates;

};

#endif
