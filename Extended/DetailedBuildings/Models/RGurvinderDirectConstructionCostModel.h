#ifndef RGurvinderDirectConstructionCostModel_H
#define RGurvinderDirectConstructionCostModel_H
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

// Simple model to calculate the cost of consruction of a building, values based off RSMeans 2010

#include "RBIM.h"
#include "RModel.h"
#include "RTimeSeries.h"
#include "RResponse.h"

#include "RCircularRCColumnComponent.h"
#include "RCLTPanelComponent.h"
#include "RCompositePanelComponent.h"
#include "RHSSRoundBeamColumnComponent.h"
#include "RRCSlabComponent.h"
#include "RRectangularGlulamBeamColumnComponent.h"
#include "RWSteelBeamColumnComponent.h"

class RGurvinderDirectConstructionCostModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)

public:
    RGurvinderDirectConstructionCostModel(QObject *parent, QString name);
    ~RGurvinderDirectConstructionCostModel() override;

    QObject *getBIM() const;
    void setBIM(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

private:

    // Random variables for numbers in our database
    RResponse *theResponse;

    RBIM *theBIM;

    void createDatabaseOfRandomVariables();

    RContinuousRandomVariable *theCircularRCColumnTheta1;
    RContinuousRandomVariable *theCircularRCColumnTheta2;
    RContinuousRandomVariable *theCircularRCColumnTheta3;
    RContinuousRandomVariable *theCircularRCColumnTheta4;
    RContinuousRandomVariable *theCircularRCColumnEpsilon;

    RContinuousRandomVariable *theCLTPanelTheta1;
    RContinuousRandomVariable *theCLTPanelTheta2;
    RContinuousRandomVariable *theCLTPanelTheta3;
    RContinuousRandomVariable *theCLTPanelEpsilon;

    RContinuousRandomVariable *theCorrugatedRoofTheta1;
    RContinuousRandomVariable *theCorrugatedRoofTheta2;
    RContinuousRandomVariable *theCorrugatedRoofTheta3;
    RContinuousRandomVariable *theCorrugatedRoofEpsilon;

    RContinuousRandomVariable *theRCSlabTheta1;
    RContinuousRandomVariable *theRCSlabTheta2;
    RContinuousRandomVariable *theRCSlabTheta3;
    RContinuousRandomVariable *theRCSlabTheta4;
    RContinuousRandomVariable *theRCSlabTheta5;
    RContinuousRandomVariable *theRCSlabEpsilon;

    RContinuousRandomVariable *theRectangularTimberBeamColumnTheta1;
    RContinuousRandomVariable *theRectangularTimberBeamColumnTheta2;
    RContinuousRandomVariable *theRectangularTimberBeamColumnEpsilon;

    RContinuousRandomVariable *theWSteelBeamColumnTheta1;
    RContinuousRandomVariable *theWSteelBeamColumnTheta2;
    RContinuousRandomVariable *theWSteelBeamColumnEpsilon;

    double circularRCColumnCost(RCircularRCColumnComponent *theComponent);
    double CLTPanelCost(RCLTPanelComponent *theComponent);
    double corrugatedRoofCost(RCompositePanelComponent *theComponent);
    double RCSlabCost(RRCSlabComponent *theComponent);
    double rectangularTimberBeamColumnCost(RRectangularGlulamBeamColumnComponent *theComponent);
    double WSteelBeamColumnCost(RWSteelBeamColumnComponent *theComponent);

};

#endif
