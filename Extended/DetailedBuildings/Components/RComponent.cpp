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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RComponent.h"
#include "RParameter.h"
#include "RMesh.h"
#include "RDamageMesh.h"
#include "RContinuousRandomVariable.h"

RComponent::RComponent(QObject *parent, QString &name)
    : RObject(parent, name)
{
    theDamageMesh = nullptr;

    applyEarthquake = false;
    applySnow = false;
    applyWind = false;
    applyLiveLoad = false;
    evaluateDamage = false;
    meshWasCreated = false;
    shouldBeReplaced = false;
    isStructural = true;
    localMeshOption = 0;
    buildingStoreyNumber = 0;
    componentElevation = 0.0;
    RSIValue = -1.0;
    theComponentExposure = RInteriorOrExteriorComponent::NA;
    replacementCost = 0.0;
    constructionCost = 0.0;
    manufacturingCost = 0.0;
    repairCost = 0.0;
}


RComponent::~RComponent()
{

}


void RComponent::addToParameterList(RParameter* theParameter)
{
    if(theParameter)
    {
        if(theParameter->objectName().contains("connectingMember") || theParameter->objectName().contains("holdDown"))
            if(!qobject_cast<RContinuousRandomVariable*>(theParameter))
                return;

        if(!theParameterList.contains(theParameter))
        {
            theParameterList.append(theParameter);
            theDomain->addToDDMList(theParameter);
        }
    }
    else
        qCritical()<<"Null parameter came into RComponent add to list function";

    return;
}


void RComponent::addToParameterList(QList<RParameter*> parameterList)
{

    for(auto&& theParameter : parameterList)
    {

        if(theParameter->objectName().contains("connectingMember") || theParameter->objectName().contains("holdDown"))
            if(!qobject_cast<RContinuousRandomVariable*>(theParameter))
                continue;

        if(!theParameterList.contains(theParameter))
        {
            theParameterList.append(theParameter);
            theDomain->addToDDMList(theParameter);
        }
    }

    return;
}


void RComponent::setRepairCost(double value)
{
    repairCost = value;
}


bool RComponent::getShouldBeReplaced() const
{
    return shouldBeReplaced;
}


void RComponent::setComponentReplaced(bool value)
{
    shouldBeReplaced = value;
}


void RComponent::setManufacturingCost(double value)
{
    manufacturingCost = value;

    return;
}


void RComponent::setConstructionCost(double value)
{
    constructionCost = value;

    return;
}


RJob& RComponent::getConstructionJob()
{
    return constructionJob;
}


void RComponent::setConstructionJob(const RJob &value)
{
    constructionJob = value;

    return;
}


CSIvec& RComponent::getManufacturingMaterialQuantityCSICodes()
{
    return constructionMaterialQuantityCSICodes;
}


void RComponent::setManufacturingMaterialQuantityCSICodes(const QVector<std::tuple<QString, double, QString> > &value)
{
    constructionMaterialQuantityCSICodes = value;

    return;
}


CSIvec& RComponent::getConstructionLabourQuantityCSICodes()
{
    return constructionLabourQuantityCSICodes;
}


void RComponent::setConstructionLabourQuantityCSICodes(const CSIvec &value)
{
    constructionLabourQuantityCSICodes = value;

    return;
}


double RComponent::getReplacementCost() const
{
    return replacementCost;
}


void RComponent::setReplacementCost(double value)
{
    replacementCost = value;

    return;
}


RJob& RComponent::getReplacementJob()
{
    return replacementJob;
}


void RComponent::setReplacementJob(const RJob &value)
{
    replacementJob = value;

    return;
}


CSIvec& RComponent::getReplacementMaterialQuantityCSICodes()
{
    return replacementMaterialQuantityCSICodes;
}


void RComponent::setReplacementMaterialQuantityCSICodes(const CSIvec &value)
{
    replacementMaterialQuantityCSICodes = value;
}


CSIvec& RComponent::getReplacementLabourQuantityCSICodes()
{
    return replacementLabourQuantityCSICodes;
}


void RComponent::setReplacementLabourQuantityCSICodes(const CSIvec &value)
{
    replacementLabourQuantityCSICodes = value;

    return;
}


double RComponent::getRepairCost()
{
    return repairCost;
}


RJob& RComponent::getRepairJob()
{
    return repairJob;
}


CSIvec& RComponent::getRepairMaterialQuantityCSICodes()
{
    return repairMaterialQuantityCSICodes;
}


CSIvec& RComponent::getRepairLabourQuantityCSICodes()
{
    return repairLabourQuantityCSICodes;
}


void RComponent::commitRepair(CSIvec* labourCSICodes , CSIvec* materialCodes, RJob* job, double repCost)
{

    for(auto&& csi: *labourCSICodes)
    {
        repairLabourQuantityCSICodes.push_back(csi);
    }

    for(auto&& csi: *materialCodes)
    {
        repairMaterialQuantityCSICodes.push_back(csi);
    }


    auto crewsVec = job->getCrewsOnJob();

    // At the building, crews are being shared amongst multiple tasks, and multiple components
    // Need to do things a little different here. The job here should be for repairing the component only
    // This means that every task should correspond to this component.
    for(auto&& it : crewsVec)
    {
        auto crewCode = it->getCrewCode();

        auto crew = repairJob.addCrew(crewCode);

        auto taskVec = it->getTheTaskVector();

        for(auto&& task : taskVec)
            crew->addTask(task);

    }

    repairCost += repCost;

    return;
}


bool RComponent::getIsStructural() const
{
    return isStructural;
}


bool RComponent::getEvaluateDamage() const
{
    return evaluateDamage;
}


int RComponent::getBuildingStoreyNumber()
{
    return buildingStoreyNumber;
}


QList<RParameter *> RComponent::getParameterList()
{
    return theParameterList;
}


QList<RElement *> RComponent::getElementList()
{
    return theElementList;
}


QList<RPoint *> RComponent::getPointList()
{
    QList<RPoint*> emptyPointList;

    return emptyPointList;
}


int RComponent::createMesh()
{
    return 0;
}


void RComponent::resetCSI(void)
{
    constructionLabourQuantityCSICodes.clear();
    constructionMaterialQuantityCSICodes.clear();
    constructionJob.reset();

    repairLabourQuantityCSICodes.clear();
    repairMaterialQuantityCSICodes.clear();
    repairJob.reset();

    shouldBeReplaced = false;
}


void RComponent::reset(void)
{
    if(isStructural == false)
        return;

    auto theMesh = this->getMesh();

    if(theMesh)
        theMesh->reset();

    constructionCost =0.0;
    manufacturingCost =0.0;
    repairCost = 0.0;
    replacementCost =0.0;

    replacementLabourQuantityCSICodes.clear();
    replacementMaterialQuantityCSICodes.clear();
    replacementJob.reset();

    this->resetCSI();
}


void RComponent::resetVisualDamage(void)
{
    if(theDamageMesh)
        theDamageMesh->resetDamageMesh();

    return;
}


int RComponent::createTopologicalMesh()
{
    return 0;
}


RMesh* RComponent::getMesh()
{
    return nullptr;
}


RDamageMesh* RComponent::getDamageMesh()
{
    return theDamageMesh.get();
}


int RComponent::commitState()
{
    return 0;
}


void RComponent::clearElementList()
{
    theElementList.clear();
}


double RComponent::getRepairTime()
{
    return 0.0;
}


double RComponent::getManufacturingCost()
{
    return manufacturingCost;
}


double RComponent::getConstructionCost()
{
    return constructionCost;
}


double RComponent::getNumberOfWorkerHoursForConstruction()
{
    return 0.0;
}


double RComponent::getNumberOfWorkerHoursForRepair()
{
    return 0.0;
}


double RComponent::getVolumeOfConcrete()
{
    return 0.0;
}


double RComponent::getVolumeOfSteel()
{
    return 0.0;
}


double RComponent::getVolumeOfWood()
{
    return 0.0;
}


double RComponent::getServiceabilityCost()
{
    return 0.0;
}


QList<QPair<double, QString> > RComponent::getTriggerTimes(double startTime, double endTime)
{
    return repairTimes;
}


double RComponent::getPreventativeRepairCost()
{
    return 0.0;
}


void RComponent::setStoreyNumber(int storeyNumber)
{
    buildingStoreyNumber = storeyNumber;
}


void RComponent::setElevation(double elevation)
{
    componentElevation = elevation;
}


double RComponent::getElevation(void)
{
    return componentElevation;
}


void RComponent::setInteriorExterior(RInteriorOrExteriorComponent componentLocation)
{
    theComponentExposure = componentLocation;
}


bool RComponent::isExterior(void)
{
    if(theComponentExposure == RInteriorOrExteriorComponent::EXTERIOR)
        return true;
    else
        return false;
}


double RComponent::getRSIValue()
{
    return RSIValue;
}


void RComponent::setRSIValue(double value)
{
    RSIValue = value;

    return;
}


void RComponent::addVoid(double area, double volume)
{
    vectorOfVoids.push_back(qMakePair(area,volume));

    return;
}

