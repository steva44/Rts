#ifndef RStevesManufacturingImpactModel_H
#define RStevesManufacturingImpactModel_H
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

// This model calculates the energy, water, time, cost, and emissions from material manufacturing

#include "RModel.h"

class RBIM;
class RContinuousRandomVariable;
class RResponse;
class RInfoRichResponse;
class RStatistics;
class RCSIAssembler;

class RStevesManufacturingImpactModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)

public:
    RStevesManufacturingImpactModel(QObject *parent, QString name);
    ~RStevesManufacturingImpactModel() override;

    // Get and set the building information model
    QObject *getBIM() const;
    void setBIM(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

    int resetTime() override;

private:

    // Objects and methods used by this class
    QPointer<RBIM> theBIM;
    RStatistics* theStatsMethod;

    // The responses that this model returns
    QPointer<RResponse> theCostResponse;
    QPointer<RResponse> theWaterResponse;
    QPointer<RResponse> theTimeResponse;
    QPointer<RInfoRichResponse> theEmissionsResponse;
    QPointer<RInfoRichResponse> theEnergyResponse;

    // Vector of material quantities - used by the costing method to calculate the cost of materials
    // <CSICode, Quantity, Description>
    QVector<std::tuple<QString,double,QString>> materialQuantityCSICodes;

    // Method that provides the CSI codes for the various components
    std::unique_ptr<RCSIAssembler> theCSIAssembler;

};

#endif
