#ifndef RSeadonDirectConstructionCostModel_H
#define RSeadonDirectConstructionCostModel_H
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
*   - Seadon Chui                                                    *
*                                                                    *
*********************************************************************/

// Model for calculating the direct construction cost of a building "RSMeans Estimating Handbook 3rd Ed" and RSMeansOnline

#include "RModel.h"
#include "RResponse.h"
#include "RBIM.h"
#include "RContinuousRandomVariable.h"
#include "RRectangularRCColumnComponent.h"
#include "RCircularRCColumnComponent.h"
#include "RRCSlabComponent.h"
#include "RRCShearWallComponent.h"

class RSeadonDirectConstructionCostModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)

public:
    RSeadonDirectConstructionCostModel(QObject *parent, QString name);
    ~RSeadonDirectConstructionCostModel() override;

    QObject *getBIM() const;
    void setBIM(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

private:

    RBIM *theBIM;
    RResponse *theResponse;
    RResponse *theLabourResponse;
    RContinuousRandomVariable *theRandomness;

    // Functions to calculate costs for different components
    QPair<double, double> rectangularRCColumnCost(RRectangularRCColumnComponent *theComponent);
    QPair<double, double> circularRCColumnCost(RCircularRCColumnComponent *theComponent);
    QPair<double, double> RCSlabCost(RRCSlabComponent *theComponent);
    QPair<double, double> RCShearWallCost(RRCShearWallComponent *theComponent);

    // Cost rates of workers - dollars per crew per day
    double costCarpenters;
    double costRodmen;
    double costPour;
    double timeSteel;
    double costForms;

};

#endif
