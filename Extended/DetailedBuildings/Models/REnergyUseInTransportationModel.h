#ifndef REnergyUseInTransportationModel_H
#define REnergyUseInTransportationModel_H
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

// Simple model to calculate the energy used in transportation of materials and workers

#include "RBIM.h"
#include "RModel.h"
#include "RResponse.h"
#include "RParameter.h"
#include "RLocation.h"

class RContinuousRandomVariable;

class REnergyUseInTransportationModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
    Q_PROPERTY(QObject *TotalWorkerHours READ getTotalWorkerHours WRITE setTotalWorkerHours)
    Q_PROPERTY(QObject *HoursPerShift READ getHoursPerShift WRITE setHoursPerShift)
    Q_PROPERTY(QObject *ConcreteSource READ getConcreteSource WRITE setConcreteSource)
    Q_PROPERTY(QObject *SteelSource READ getSteelSource WRITE setSteelSource)
    Q_PROPERTY(QObject *TimberSource READ getTimberSource WRITE setTimberSource)

public:

    REnergyUseInTransportationModel(QObject *parent, QString name);
    ~REnergyUseInTransportationModel() override;

    QObject *getBIM() const;
    void setBIM(QObject *value);

    RParameter *getTotalWorkerHours() const;
    void setTotalWorkerHours(QObject *value);

    RParameter *getHoursPerShift() const;
    void setHoursPerShift(QObject *value);

    RLocation *getConcreteSource() const;
    void setConcreteSource(QObject *value);

    RLocation *getSteelSource() const;
    void setSteelSource(QObject *value);

    RLocation *getTimberSource() const;
    void setTimberSource(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

private:

    QPointer<RBIM> theBIM;
    QPointer<RParameter> theTotalWorkerHours;
    QPointer<RParameter> theHoursPerShift;

    QPointer<RLocation> theConcreteSource;
    QPointer<RLocation> theSteelSource;
    QPointer<RLocation> theTimberSource;

    RContinuousRandomVariable *theJoulePerMetreCar;
    RContinuousRandomVariable *theJoulePerMetreBus;
    RContinuousRandomVariable *theJoulePerMetreSkytrain;
    RContinuousRandomVariable *theJoulePerMetreHeavyTruck;

    RResponse *theJouleCoalResponse;
    RResponse *theJouleDieselResponse;
    RResponse *theJouleElectricityResponse;
    RResponse *theJouleFuelOilResponse;
    RResponse *theJouleGasolineResponse;
    RResponse *theJouleLPGResponse;
    RResponse *theJouleNaturalGasResponse;
};

#endif
