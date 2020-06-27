#ifndef RCostOfEmissionsModel_H
#define RCostOfEmissionsModel_H
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

// This classc calculates the cost of emissions from material manufacturing, energy usage, etc.
// The cost of emissions stems from environmental damage and damge to human health

#include "RModel.h"

class RResponse;
class RContinuousRandomVariable;
class RStatistics;
class RBIM;

class RCostOfEmissionsModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)

public:
    RCostOfEmissionsModel(QObject *parent, QString name);
    ~RCostOfEmissionsModel() override;

    // The input parameter is an information rich response of emissions
    QString getInputParameterList();
    void setInputParameterList(QString value);

    int evaluateModel(RGradientType theGradientType) override;

private:

    QList<QPointer<RParameter> > theInputParameterList;
    QPointer<RResponse> theResponse;
    QPointer<RResponse> theCostOfDamageToHumanHealthResponse;
    QPointer<RResponse> theCostOfEnvironmentalDamageResponse;
    QPointer<RResponse> theCostAtMaxDriftResponse;
    QPointer<RResponse> theMaxDriftResponse;

    QString discountRate;

    // Human health costs of aerosol Emissions
    RContinuousRandomVariable* theEmissionsToHumanHealthCostConversionCO2;
    RContinuousRandomVariable* theEmissionsToHumanHealthCostConversionCH4;
    RContinuousRandomVariable* theEmissionsToHumanHealthCostConversionN2O;
    RContinuousRandomVariable* theEmissionsToHumanHealthCostConversionHFC_134a;
    RContinuousRandomVariable* theEmissionsToHumanHealthCostConversionBC;
    RContinuousRandomVariable* theEmissionsToHumanHealthCostConversionSO2;
    RContinuousRandomVariable* theEmissionsToHumanHealthCostConversionCO;
    RContinuousRandomVariable* theEmissionsToHumanHealthCostConversionOC;
    RContinuousRandomVariable* theEmissionsToHumanHealthCostConversionNOx;
    RContinuousRandomVariable* theEmissionsToHumanHealthCostConversionNH3;

    // Environmetal costs of aerosol emissions
    RContinuousRandomVariable* theEmissionsToEnvironmentalCostConversionCO2;
    RContinuousRandomVariable* theEmissionsToEnvironmentalCostConversionCH4;
    RContinuousRandomVariable* theEmissionsToEnvironmentalCostConversionN2O;
    RContinuousRandomVariable* theEmissionsToEnvironmentalCostConversionSO2;
    RContinuousRandomVariable* theEmissionsToEnvironmentalCostConversionCO;
    RContinuousRandomVariable* theEmissionsToEnvironmentalCostConversionNOx;
    RContinuousRandomVariable* theEmissionsToEnvironmentalCostConversionNH3;

    // Environmental Costs of Midpoint Impacts
    RContinuousRandomVariable* AgriculturalLandOccupationToCost;
    RContinuousRandomVariable* ClimateChangeToCost;
    RContinuousRandomVariable* FreshwaterEcotoxicityToCost;
    RContinuousRandomVariable* FreshwaterEutrophicationToCost;
    RContinuousRandomVariable* HumanToxicityToCost;
    RContinuousRandomVariable* IonisingRadiationToCost;
    RContinuousRandomVariable* MarineEcotoxicityToCost;
    RContinuousRandomVariable* MarineEutrophicationToCost;
    RContinuousRandomVariable* OzoneDepletionToCost;
    RContinuousRandomVariable* ParticulateMatterFormationToCost;
    RContinuousRandomVariable* PhotochemicalOxidantToCost;
    RContinuousRandomVariable* TerrestrialAcidificationToCost;
    RContinuousRandomVariable* TerrestrialEcotoxicityToCost;
    RContinuousRandomVariable* UrbanLandOccupationToCost ;

    RStatistics* theStatsMethod;

    void createHumanHealthCostRandomVariables(void);
    void createEnvironmentalCostRandomVariables(void)  ;
    void createMidpointCostRandomVariables(void);
};

#endif
