#ifndef RShindellEnvironmentalCostOfEnergyModel_H
#define RShindellEnvironmentalCostOfEnergyModel_H
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
*   - Gurvinder Gill                                                 *
*                                                                    *
*********************************************************************/

#include "RModel.h"
#include "RResponse.h"
#include "RBIM.h"

class RContinuousRandomVariable;

class RShindellEnvironmentalCostOfEnergyModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
    Q_PROPERTY(QString JouleCoalList READ getJouleCoalList WRITE setJouleCoalList)
    Q_PROPERTY(QString JouleDieselList READ getJouleDieselList WRITE setJouleDieselList)
    Q_PROPERTY(QString JouleElectricityList READ getJouleElectricityList WRITE setJouleElectricityList)
    Q_PROPERTY(QString JouleFuelOilList READ getJouleFuelOilList WRITE setJouleFuelOilList)
    Q_PROPERTY(QString JouleGasolineList READ getJouleGasolineList WRITE setJouleGasolineList)
    Q_PROPERTY(QString JouleLPGList READ getJouleLPGList WRITE setJouleLPGList)
    Q_PROPERTY(QString JouleNaturalGasList READ getJouleNaturalGasList WRITE setJouleNaturalGasList)

public:
    RShindellEnvironmentalCostOfEnergyModel(QObject *parent, QString name);
    ~RShindellEnvironmentalCostOfEnergyModel() override;

    QObject *getBIM() const;
    void setBIM(QObject *value);

    QString getJouleCoalList();
    void setJouleCoalList(QString value);

    QString getJouleDieselList();
    void setJouleDieselList(QString value);

    QString getJouleElectricityList();
    void setJouleElectricityList(QString value);

    QString getJouleFuelOilList();
    void setJouleFuelOilList(QString value);

    QString getJouleGasolineList();
    void setJouleGasolineList(QString value);

    QString getJouleLPGList();
    void setJouleLPGList(QString value);

    QString getJouleNaturalGasList();
    void setJouleNaturalGasList(QString value);

    int evaluateModel(RGradientType theGradientType) override;

private:

    RBIM *theBIM;
    RResponse *theResponse;
    RContinuousRandomVariable *theFuelVolumeToCostConversionCoal;
    RContinuousRandomVariable *theFuelVolumeToCostConversionDiesel;
    RContinuousRandomVariable *theFuelVolumeToCostConversionElectricity;
    RContinuousRandomVariable *theFuelVolumeToCostConversionFuelOil;
    RContinuousRandomVariable *theFuelVolumeToCostConversionGasoline;
    RContinuousRandomVariable *theFuelVolumeToCostConversionLPG;
    RContinuousRandomVariable *theFuelVolumeToCostConversionNaturalGas;

    QList<QPointer<RParameter> > theJouleCoalList;
    QList<QPointer<RParameter> > theJouleDieselList;
    QList<QPointer<RParameter> > theJouleElectricityList;
    QList<QPointer<RParameter> > theJouleFuelOilList;
    QList<QPointer<RParameter> > theJouleGasolineList;
    QList<QPointer<RParameter> > theJouleLPGList;
    QList<QPointer<RParameter> > theJouleNaturalGasList;

};

#endif
