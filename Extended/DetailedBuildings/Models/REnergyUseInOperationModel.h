#ifndef REnergyUseInOperationModel_H
#define REnergyUseInOperationModel_H
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
*   - Gurvinder Gill                                                 *
*                                                                    *
*********************************************************************/

// Simple model to calculate the energy used in building operations

#include "RBIM.h"
#include "RModel.h"
#include "RResponse.h"
#include "RParameter.h"

class RContinuousRandomVariable;

class REnergyUseInOperationModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
    Q_PROPERTY(double CostOfElectricity READ getCostOfElectricity WRITE setCostOfElectricity)
    Q_PROPERTY(double CostOfNaturalGas READ getCostOfNaturalGas WRITE setCostOfNaturalGas)

public:
    REnergyUseInOperationModel(QObject *parent, QString name);
    ~REnergyUseInOperationModel() override;

    QObject *getBIM() const;
    void setBIM(QObject *value);

    double getCostOfElectricity();
    void setCostOfElectricity(double value);

    double getCostOfNaturalGas();
    void setCostOfNaturalGas(double value);

    int evaluateModel(RGradientType theGradientType) override;
    int resetTime() override;

private:

    QPointer<RBIM> theBIM = nullptr;

    RResponse *theJouleElectricityResponse = nullptr;
    RResponse *theJouleNaturalGasResponse = nullptr;
    RResponse *theResponse = nullptr;

    RContinuousRandomVariable *theDailyOperationElectricityUsage = nullptr;

    double thePreviousTime = 0.0;


    // Costs are from BC Hydro and FortisBC, per kWh and GJ respectively
    double costOfElectricity;
    double costOfNaturalGas;


    // Derived from BCEmissions; electricity and natural gas are the only significant contributors
    double fuelFractionResidentialElectricity;
    double fuelFractionResidentialNaturalGas;
    double fuelFractionCommercialElectricity;
    double fuelFractionCommercialNaturalGas;
};

#endif
