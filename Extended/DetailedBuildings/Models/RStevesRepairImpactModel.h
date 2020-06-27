#ifndef RStevesRepairImpactModel_H
#define RStevesRepairImpactModel_H
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

// This model calculates the energy, water, time, cost, and emissions from the repair of damage to building components

#include "RModel.h"
#include "RCSIAssembler.h"

class RBIM;
class RContinuousRandomVariable;
class RLinearShellElement;
class RResponse;
class RInfoRichResponse;
class RCSISchedulingMethod;
class RCSIAssembler;
class RStatistics;
class RStructuralAnalysisModel;
class RJob;
class RShellSectionDamageRegion;

class RStevesRepairImpactModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
    Q_PROPERTY(QObject *InputParameter READ getInputParameter WRITE setInputParameter)

public:
    RStevesRepairImpactModel(QObject *parent, QString name);
    ~RStevesRepairImpactModel() override;

    QList<RParameter *> getAllParametersList(bool *ok) override;

    // Get and set the building information model
    QObject *getBIM() const;
    void setBIM(QObject *value);

    // The input parameter is a structural analysis model, or visual damage model
    QObject *getInputParameter() const;
    void setInputParameter(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

    int resetTime() override;

private:

    QPointer<RBIM> theBIM;
    RStatistics* theStatsMethod;
    QPointer<RParameter> theSAModelResponse;
    std::unique_ptr<RCSISchedulingMethod> theScheduler;
    std::unique_ptr<RCSIAssembler> theCSIAssembler;

    // Create random variables that are used in this class for various calculations
    void createDatabaseOfRandomVariables();

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

    RContinuousRandomVariable *theHeavyMachineryUseIntensity;

    RContinuousRandomVariable *theWorkerTransportationIntensityAutomobil;
    RContinuousRandomVariable *theWorkerTransportationIntensityDieselbus;
    RContinuousRandomVariable *theWorkerTransportationIntensitySkytrain;

    RContinuousRandomVariable *theMaterialTransportationIntensityConcrete;
    RContinuousRandomVariable *theMaterialTransportationIntensitySteel;
    RContinuousRandomVariable *theMaterialTransportationIntensityWood;

    // Times and distances
    double hours_per_shift;

    // Vector of labour quantities - used to price the labour
    // <CSICode,Unit Quantity,Description>
    QVector<std::tuple<QString,double,QString>> labourQuantityCSICodes;

    // Vector of material quantities - used by the costing method to calculate the cost of materials
    // <CSICode, Quantity, Description>
    QVector<std::tuple<QString,double,QString>> materialQuantityCSICodes;

    // Vector of labour crew rates - used to schedule the labour
    // <CSICode,Crew Code,Unit Quantity,Crew Rate RV>
    QVector<std::tuple<QString,double,RContinuousRandomVariable*>> crewRates;

    // A function that returns vectors of damage quantities for reinforced concrete shell components
    void getRCShellDamageQuantities(RShellSectionDamageRegion* shellDamageRegion, bool&componentReplacement,
                                    double& crackedAreaTopTotal, double& spalledAreaTopTotal, double& coverLossAreaTopTotal,
                                    std::vector<double>& crackWidthVecTop,std::vector<double>& crackLengthVecTop, std::vector<double>& crackDepthVecTop, std::vector<double>& crackVolumeVecTop,
                                    std::vector<double>& spallAreaVecTop,std::vector<double>& spallDepthVecTop,std::vector<double>& coverAreaVecTop,std::vector<double>& coverDepthVecTop,
                                    double& crackedAreaBotTotal, double& spalledAreaBotTotal, double& coverLossAreaBotTotal,
                                    std::vector<double>& crackWidthVecBot,std::vector<double>& crackLengthVecBot, std::vector<double>& crackDepthVecBot, std::vector<double>& crackVolumeVecBot,
                                    std::vector<double>& spallAreaVecBot,std::vector<double>& spallDepthVecBot,std::vector<double>& coverAreaVecBot,std::vector<double>& coverDepthVecBot,
                                    size_t faceIndex1, size_t faceIndex2);

    // A function that returns vectors of repair quantities for reinforced concrete components
    void getRCComponentRepairQuantities(const std::vector<double>& crackVolumeVec, double& crackVol,
                                        const std::vector<double>& spallAreaVec, const std::vector<double>& spallDepthVec, double& spallArea, double& spallVol,
                                        const std::vector<double>& coverAreaVec, const std::vector<double>& coverDepthVec, double& coverArea, double& coverVol);

    // Commits the final repair action to the job
    void commitRepair(CSIVector* labourCSICodes , CSIVector* materialCodes, RJob* job, RJob* repairPhase, RComponent* theComponent, double repairCost);

};

#endif
