#ifndef RStevesDemolitionImpactModel_H
#define RStevesDemolitionImpactModel_H
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

// This model calculates the energy, water, time, cost, and emissions from building demolition

#include "RModel.h"

class RBIM;
class RContinuousRandomVariable;
class RResponse;
class RInfoRichResponse;
class RResponse;
class RCSISchedulingMethod;
class RStatistics;
class RCSIAssembler;

class RStevesDemolitionImpactModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)

public:
    RStevesDemolitionImpactModel(QObject *parent, QString name);
    ~RStevesDemolitionImpactModel() override;

    // The building information model
    QObject *getBIM() const;
    void setBIM(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

    int resetTime() override;

private:

    QPointer<RBIM> theBIM;
    RStatistics* theStatsMethod;
    void createDatabaseOfRandomVariables();

    // Methods to assemble the CSI codes and to schedule the workers
    std::unique_ptr<RCSISchedulingMethod> theScheduler;
    std::unique_ptr<RCSIAssembler> theCSIAssembler;

    // The responses returned by this model
    QPointer<RResponse> theCostResponse;
    QPointer<RResponse> theWaterResponse;
    QPointer<RResponse> theTimeResponse;
    QPointer<RInfoRichResponse> theEmissionsResponse;
    QPointer<RInfoRichResponse> theEnergyResponse;

    // Hours worked per shift
    double hours_per_shift;

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

    // Vector of labour quantities - used to price the labour
    // <CSICode,Unit Quantity,Description>
    QVector<std::tuple<QString,double,QString>> labourQuantityCSICodes;

    // The energy intensities for heavy machinery usage, worker transport, and material transport
    RContinuousRandomVariable *theHeavyMachineryUseIntensity;
    RContinuousRandomVariable *theWorkerTransportationIntensityAutomobil;
    RContinuousRandomVariable *theWorkerTransportationIntensityDieselbus;
    RContinuousRandomVariable *theWorkerTransportationIntensitySkytrain;
    RContinuousRandomVariable *theMaterialTransportationIntensityConcrete;
    RContinuousRandomVariable *theMaterialTransportationIntensitySteel;
    RContinuousRandomVariable *theMaterialTransportationIntensityWood;

    // Costs for disposal and recycling
    RContinuousRandomVariable *theDirectCostLandfillDisposalWood;
    RContinuousRandomVariable *theDirectCostLandfillDisposalOther;
    RContinuousRandomVariable *theDirectCostRecyclingConcrete;
    RContinuousRandomVariable *theDirectCostRecyclingSteel;

};

#endif
