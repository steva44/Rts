#ifndef RRepairManagerModel_H
#define RRepairManagerModel_H
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
*   - Abbas Javaherian                                               *
*                                                                    *
*********************************************************************/

#include "RModel.h"
#include "RContinuousRandomVariable.h"

class RParameter;
class RResponse;

class RRepairManagerModel : public RModel
{
    Q_OBJECT

    Q_PROPERTY(double BuildingAge READ getBuildingAge WRITE setBuildingAge)
    Q_PROPERTY(bool Heritage READ getHeritage WRITE setHeritage)
    Q_PROPERTY(bool Insured READ getInsured WRITE setInsured)
    Q_PROPERTY(QObject *AuxiliaryParameter READ getAuxiliaryParameter WRITE setAuxiliaryParameter)

public:
    RRepairManagerModel(QObject *parent, QString name);
    ~RRepairManagerModel();

    double getBuildingAge() const;
    void setBuildingAge(double value);

    double getHeritage() const;
    void setHeritage(bool value);

    double getInsured() const;
    void setInsured(bool value);

    QObject *getAuxiliaryParameter() const;
    void setAuxiliaryParameter(QObject *value);

    int evaluateModel(RGradientType theGradientType);
    QStringList getActionList();

private:
    double theBuildingAge;
    bool theHeritage;
    bool theInsured;
    QPointer<RParameter> theAuxiliaryParameter;

    double numRepairActions;
    RResponse *theRepairCost;

    double getRepairCostPerUnitQuantity(int repairAction);
    double getEconomyOfScale(double repairQuantity, int repairAction);
    // double getRepairTimePerUnitQuantity(int repairAction);
    QList<RParameter *> theParameterList;
    bool shouldDemolish(double repairCost);
    RContinuousRandomVariable *theTheta1;
    RContinuousRandomVariable *theTheta2;
    RContinuousRandomVariable *repairActionCost1;
    RContinuousRandomVariable *repairActionCost2;
    RContinuousRandomVariable *repairActionCost3;
    RContinuousRandomVariable *repairActionCost4;
    RContinuousRandomVariable *repairActionCost5;

};

#endif // RRepairManagerModel_H
