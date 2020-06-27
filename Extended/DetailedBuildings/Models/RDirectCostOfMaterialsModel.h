#ifndef RDirectCostOfMaterialsModel_H
#define RDirectCostOfMaterialsModel_H
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

// Simple model to calculate the cost of materials

#include "RBIM.h"
#include "RModel.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"


class RDirectCostOfMaterialsModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)

public:
    RDirectCostOfMaterialsModel(QObject *parent, QString name);
    ~RDirectCostOfMaterialsModel() override;

    QObject *getBIM() const;
    void setBIM(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

private:
    RResponse *theResponse;
    RBIM *theBIM;
    RContinuousRandomVariable *theRandomness;

    double getPriceOfConcrete(QString theConcreteStrength);

    double priceSteel;
    double priceTimber;
    double priceConcreteM;
    double priceConcreteB;

};

#endif
