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
*   - Mojtaba Mahsuli                                                *
*                                                                    *
*********************************************************************/

#include "RMojtabasBuildingRepairCostModel.h"
#include "RBIM.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RConstant.h"
#include "RNormalDistribution.h"
#include <math.h>

RMojtabasBuildingRepairCostModel::RMojtabasBuildingRepairCostModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    // Create ONE model variable for ALL model instances
    QString lossEpsilonName = "BuildingResponseModelDriftEpsilon";
    theLossEpsilon = theDomain->findChild<RContinuousRandomVariable *>(lossEpsilonName);
    if (!theLossEpsilon) {
        theDomain->createObject(lossEpsilonName, "RContinuousRandomVariable");
        theLossEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theLossEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
        theLossEpsilon->setMean("1.0");
        theLossEpsilon->setStandardDeviation("0.1");
    }
}

RMojtabasBuildingRepairCostModel::~RMojtabasBuildingRepairCostModel()
{

}

QObject * RMojtabasBuildingRepairCostModel::getBIM() const
{
    return theBIM;
}

void RMojtabasBuildingRepairCostModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


RParameter * RMojtabasBuildingRepairCostModel::getStructuralDamageRatio() const
{
    return theStructuralDamageRatio;
}

void RMojtabasBuildingRepairCostModel::setStructuralDamageRatio(RParameter *value)
{
    theStructuralDamageRatio = value;
}


RParameter * RMojtabasBuildingRepairCostModel::getNonstructuralDriftDamageRatio() const
{
    return theNonstructuralDriftDamageRatio;
}

void RMojtabasBuildingRepairCostModel::setNonstructuralDriftDamageRatio(RParameter *value)
{
    theNonstructuralDriftDamageRatio = value;
}


RParameter * RMojtabasBuildingRepairCostModel::getNonstructuralAccelerationDamageRatio() const
{
    return theNonstructuralAccelerationDamageRatio;
}

void RMojtabasBuildingRepairCostModel::setNonstructuralAccelerationDamageRatio(RParameter *value)
{
    theNonstructuralAccelerationDamageRatio = value;
}


RParameter * RMojtabasBuildingRepairCostModel::getContentDamageRatio() const
{
    return theContentDamageRatio;
}

void RMojtabasBuildingRepairCostModel::setContentDamageRatio(RParameter *value)
{
    theContentDamageRatio = value;
}




int RMojtabasBuildingRepairCostModel::evaluateModel(RGradientType theGradientType)
{
    double DFs = theStructuralDamageRatio->getCurrentValue();
    double DFnd = theNonstructuralDriftDamageRatio->getCurrentValue();
    double DFna = theNonstructuralAccelerationDamageRatio->getCurrentValue();
    double DFc = theContentDamageRatio->getCurrentValue();

    double epsilon = theLossEpsilon->getCurrentValue();
    double totalArea = theBIM->getTotalArea();

    double theStructuralUnitAreaCost = 266;
    double theNonstructuralDriftUnitAreaCost = 833;
    double theNonstructuralAccelerationUnitAreaCost = 844;
    double theContentUnitAreaCost = 0;

    double repairCost = (DFs * theStructuralUnitAreaCost + DFnd * theNonstructuralDriftUnitAreaCost + DFna * theNonstructuralAccelerationUnitAreaCost + DFc * theContentUnitAreaCost) * totalArea * epsilon;

    theResponse->setCurrentValue(repairCost);


    return 0;
}
