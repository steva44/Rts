#ifndef RCostOfEnergyModel_H
#define RCostOfEnergyModel_H
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

// This model calculates the cost of energy, according to its source, e.g., gasoline, diesel, etc.

#include "RModel.h"
#include "RResponse.h"
#include "RInfoRichResponse.h"

class RStatistics;
class RContinuousRandomVariable;

class RCostOfEnergyModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)

public:
    RCostOfEnergyModel(QObject *parent, QString name);
    ~RCostOfEnergyModel() override;

    // The input parameter is an information rich response of energy sources
    QString getInputParameterList();
    void setInputParameterList(QString value);

    int evaluateModel(RGradientType theGradientType) override;

private:

    QList<QPointer<RParameter> > theInputParameterList;
    QPointer<RResponse> theResponse;

    // Random variables for the energy costs
    RContinuousRandomVariable* costOFElectricityLowerTier;
    RContinuousRandomVariable* costOFElectricityUpperTier;
    RContinuousRandomVariable* costOFENaturalGasBuildingOperations;
    RContinuousRandomVariable* costOFDieselTransAndEquip;
    RContinuousRandomVariable* costOFGasolineTransAndEquip;
    RContinuousRandomVariable* costOFLPG;

    RStatistics* theStatsMethod;

    void createRandomVariables();
};

#endif
