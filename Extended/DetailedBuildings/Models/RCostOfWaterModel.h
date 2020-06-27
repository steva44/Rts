#ifndef RCostOfWaterModel_H
#define RCostOfWaterModel_H
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

// This model calculates the cost to treat and deliver water in Vancouver

#include "RModel.h"
#include "RResponse.h"
#include "RInfoRichResponse.h"

class RStatistics;

class RCostOfWaterModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)

public:
    RCostOfWaterModel(QObject *parent, QString name);
    ~RCostOfWaterModel() override;

    // The input parameter is water usage in m3
    QString getInputParameterList();
    void setInputParameterList(QString value);

    int evaluateModel(RGradientType theGradientType) override;

private:

    QList<QPointer<RParameter> > theInputParameterList;
    QPointer<RResponse> theResponse;

    // This model outputs the energy consumption involved in water treatment and delivery
    QPointer<RInfoRichResponse> theEnergyResponse;

    RStatistics* theStatsMethod;
};

#endif
