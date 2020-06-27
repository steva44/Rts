#ifndef REnergyUseInConstructionModel_H
#define REnergyUseInConstructionModel_H
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

// Simple model to calculate the energy used in construction

#include "RBIM.h"
#include "RModel.h"
#include "RResponse.h"
#include "RParameter.h"

class RContinuousRandomVariable;

class REnergyUseInConstructionModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
    Q_PROPERTY(QObject *TotalWorkerHours READ getTotalWorkerHours WRITE setTotalWorkerHours)

public:
    REnergyUseInConstructionModel(QObject *parent, QString name);
    ~REnergyUseInConstructionModel() override;

    QObject *getBIM() const;
    void setBIM(QObject *value);

    QObject *getTotalWorkerHours() const;
    void setTotalWorkerHours(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

private:

    QPointer<RBIM> theBIM;
    QPointer<RParameter> theTotalWorkerHours;
    RContinuousRandomVariable *theHeavyMachineryUseIntensity;
    RResponse *theJouleDieselResponse;
};

#endif
