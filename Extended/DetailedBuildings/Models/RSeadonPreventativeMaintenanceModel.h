#ifndef RSeadonPreventativeMaintenanceModel_H
#define RSeadonPreventativeMaintenanceModel_H
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

// Model that provides the cost of preventative maintenance of building components

#include "RModel.h"
#include "RResponse.h"
#include "RBIM.h"
#include "RRectangularRCColumnComponent.h"
#include "RCircularRCColumnComponent.h"
#include "RRCSlabComponent.h"
#include "RRCShearWallComponent.h"

class RSeadonPreventativeMaintenanceModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)

public:

    RSeadonPreventativeMaintenanceModel(QObject *parent, QString name);
    ~RSeadonPreventativeMaintenanceModel() override;

    QObject *getBIM() const;
    void setBIM(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;
    QList<QPair<double, QString> > getTriggerTimes(double startTime, double endTime) override;

private:

    RBIM *theBIM;
    RResponse *theResponse;

    QList<QPair<double, RComponent *> > repairTimes;

    double getConcreteMaintenancePeriod(QString theConcreteStrength, double theConcreteCover);

    double getPriceOfConcrete(QString theConcreteStrength);
    double priceConcreteM;
    double priceConcreteB;
};

#endif
