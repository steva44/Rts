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

#include "RRepairManagerModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RComponent.h"
#include <math.h>

RRepairManagerModel::RRepairManagerModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theTheta1 = nullptr;
    theTheta2 = nullptr;
    repairActionCost1 = nullptr;
    repairActionCost2 = nullptr;
    repairActionCost3 = nullptr;
    repairActionCost4 = nullptr;
    repairActionCost5 = nullptr;
    theAuxiliaryParameter = nullptr;

    numRepairActions = 5;

    theBuildingAge = 0.0;
    theHeritage = false;
    theInsured = false ;

    theDomain->createObject(objectName() + "CostResponse", "RGenericResponse");
    theRepairCost = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theRepairCost->setModel(this);
}


RRepairManagerModel::~RRepairManagerModel()
{

}


QStringList RRepairManagerModel::getActionList()
{
    QStringList actionList;

    actionList << "&Run"
               << "&Help";

    return actionList;
}


double RRepairManagerModel::getBuildingAge() const
{	
    return theBuildingAge;
}	


void RRepairManagerModel::setBuildingAge(double value)
{	
    theBuildingAge = value;
}	


double RRepairManagerModel::getHeritage() const
{
    return theHeritage;
}


void RRepairManagerModel::setHeritage(bool value)
{
    theHeritage = value;
}


double RRepairManagerModel::getInsured() const
{
    return theInsured;

}


void RRepairManagerModel::setInsured(bool value)
{
    theInsured = value;

}


QObject * RRepairManagerModel::getAuxiliaryParameter() const
{
    return theAuxiliaryParameter;
}


void RRepairManagerModel::setAuxiliaryParameter(QObject *value)
{
    theAuxiliaryParameter = qobject_cast<RParameter *>(value);
}


int RRepairManagerModel::evaluateModel(RGradientType theGradientType)
{	
    // Create a list of all components
    QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();


    // Some declarations ...
    QVector<QString> componentRepairAction;
    QVector<double> componentRepairQuantity;
    QVector<double> buildingRepairQuantity(numRepairActions);
    bool repairFound;
    double repairCost;


    // Initialization of the repair quantity vector
    for (int i = 0; i < numRepairActions; i++)  {
        buildingRepairQuantity[i] = 0.0;

    }


    //Sum up the repair quantities from components
    for (int i = 0; i < theComponentList.count(); i++) {


        componentRepairAction = QVector<QString>(theComponentList[i]->numRepairActions);
        componentRepairQuantity = QVector<double>(theComponentList[i]->numRepairActions);
        //theComponentList[i]->getRepairActionList(&componentRepairAction, &componentRepairQuantity,nullptr);
        //qDebug()<<theComponentList[i]->objectName();
        //qDebug()<<componentRepairAction;

        for (int j = 0; j < theComponentList[i]->numRepairActions; j++) {

            if (componentRepairAction[j] == QString("None")) {
                buildingRepairQuantity[0] = buildingRepairQuantity[0] + componentRepairQuantity[0];

            }
            else if (componentRepairAction[j] == QString("EpoxyInjection")) {
                buildingRepairQuantity[1] = buildingRepairQuantity[1] + componentRepairQuantity[1];

            }
            else if (componentRepairAction[j] == QString("PatchingCoverConcrete")) {
                buildingRepairQuantity[2] = buildingRepairQuantity[2] + componentRepairQuantity[2];

            }
            else if (componentRepairAction[j] == QString("CoverConcreteReplacement")) {
                buildingRepairQuantity[3] = buildingRepairQuantity[3] + componentRepairQuantity[3];

            }
            else if (componentRepairAction[j] == QString("SectionReplacement")) {
                buildingRepairQuantity[4] = buildingRepairQuantity[4] + componentRepairQuantity[4];

            }
        }
    }

    // Print the repair quantity
    qDebug()<<"Building repair quantity = " << buildingRepairQuantity;


    // Initialization
    repairCost = 0.0;



    //Sum up the reapir cost for all repair actions
    for (int i = 0; i < numRepairActions; i++) {
        repairCost = repairCost + getEconomyOfScale(buildingRepairQuantity[i], i+1) * buildingRepairQuantity[i] * getRepairCostPerUnitQuantity(i+1);

    }


    double constructionCost = 10.0e10;

    if( shouldDemolish(repairCost) ) {
        repairCost = constructionCost;
    }


    double auxiliary = theAuxiliaryParameter->getCurrentValue();

    // Print the repair cost
    qDebug()<< "Building repair cost = " << repairCost;


    theRepairCost->setCurrentValue(repairCost);



    if (theGradientType == RGradientType::DDM) {
        // IMPLEMENT
    }

    return 0;
}


double RRepairManagerModel::getRepairCostPerUnitQuantity(int repairAction) {

    if (repairAction == 1) {

        if (repairActionCost1 != 0) {
            theDomain->deleteObject(repairActionCost1->objectName());
            repairActionCost1 = 0;
        }

        theDomain->createObject(objectName() + "CostOfNonePerUnitQuantity", "RContinuousRandomVariable");
        repairActionCost1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        repairActionCost1->setDistributionType(QString("Lognormal (zeta, sigma)"));

        //Set the mean using an online data base
        repairActionCost1->setMean(QString("0.0"));
        repairActionCost1->setCoefficientOfVariation("0.15");
        // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
        theParameterList << repairActionCost1;

        return repairActionCost1->getCurrentValue();

    }


    else if (repairAction==2) {

        if (repairActionCost2 != 0) {
            theDomain->deleteObject(repairActionCost2->objectName());
            repairActionCost2 = 0;
        }

        theDomain->createObject(objectName() + "CostOfEpoxyInjectionPerUnitQuantity", "RContinuousRandomVariable");
        repairActionCost2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        repairActionCost2->setDistributionType(QString("Lognormal (zeta, sigma)"));

        //Set the mean using an online data base
        repairActionCost2->setMean(QString("1000.0"));
        repairActionCost2->setCoefficientOfVariation("0.15");
        // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
        theParameterList << repairActionCost2;
        return repairActionCost2->getCurrentValue();
    }


    else if (repairAction==3) {

        if (repairActionCost3 != 0) {
            theDomain->deleteObject(repairActionCost3->objectName());
            repairActionCost3 = 0;
        }

        theDomain->createObject(objectName() + "CostOfPatchingPerUnitQuantity", "RContinuousRandomVariable");
        repairActionCost3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        repairActionCost3->setDistributionType(QString("Lognormal (zeta, sigma)"));

        //Set the mean using an online data base
        repairActionCost3->setMean(QString("2000.0"));
        repairActionCost3->setCoefficientOfVariation("0.15");
        // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
        theParameterList << repairActionCost3;

        return repairActionCost3->getCurrentValue();
    }


    else if (repairAction==4) {

        if (repairActionCost4 != 0) {
            theDomain->deleteObject(repairActionCost4->objectName());
            repairActionCost4 = 0;
        }

        theDomain->createObject(objectName() + "CostOfReplacingCoverConcretePerUnitQuantity", "RContinuousRandomVariable");
        repairActionCost4 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        repairActionCost4->setDistributionType(QString("Lognormal (zeta, sigma)"));

        //Set the mean using an online data base
        repairActionCost4->setMean(QString("3000.0"));
        repairActionCost4->setCoefficientOfVariation("0.15");
        // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
        theParameterList << repairActionCost4;

        return repairActionCost4->getCurrentValue();
    }



    else if (repairAction==5) {

        if (repairActionCost5 != 0) {
            theDomain->deleteObject(repairActionCost5->objectName());
            repairActionCost5 = 0;
        }

        theDomain->createObject(objectName() + "CostOfRCComponentReplacementPerUnitQuantity", "RContinuousRandomVariable");
        repairActionCost5 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        repairActionCost5->setDistributionType(QString("Lognormal (zeta, sigma)"));

        //Set the mean using an online data base
        repairActionCost5->setMean(QString("10000.0"));
        repairActionCost5->setCoefficientOfVariation("0.15");
        // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
        theParameterList << repairActionCost5;

        return repairActionCost5->getCurrentValue();

    }


    else {

        // Issue error message
        qCritical() << "The repair action is not defined to the repair manager" << this->objectName();
        return -1;
    }


}


double RRepairManagerModel::getEconomyOfScale(double repairQuantity, int repairAction)
{
    if (repairQuantity == 0) {
        return 0.0;
    }

    else if (repairAction <= 4) {
        return (repairQuantity + 5) / repairQuantity;

    }

    else if (repairAction == 5) {
        return (repairQuantity + 1) / repairQuantity;

    }
}


bool RRepairManagerModel::shouldDemolish(double repairCost)
{
    double ratio;
    double constructionCost = 10.0e100;
    double Age = getBuildingAge();
    int insured;

    if (getInsured()) {
        insured = 1;
    }
    else {
        insured = 0;
    }



    if (theTheta1 != 0) {
        theDomain->deleteObject(theTheta1->objectName());
        theTheta1 = 0;
    }

    theDomain->createObject(objectName() + "DemolitionModelParameter1", "RContinuousRandomVariable");
    theTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta1->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theTheta1->setMean(QString("1.0"));
    theTheta1->setCoefficientOfVariation("0.15");
    // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
    theParameterList << theTheta1;


    if (theTheta2 != 0) {
        theDomain->deleteObject(theTheta2->objectName());
        theTheta2 = 0;
    }

    theDomain->createObject(objectName() + "DemolitionModelParameter2", "RContinuousRandomVariable");
    theTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta2->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theTheta2->setMean(QString("0.5"));
    theTheta2->setCoefficientOfVariation("0.15");
    // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
    theParameterList << theTheta2;


    double theta1 = theTheta1->getCurrentValue();
    double theta2 = theTheta2->getCurrentValue();

    // The model for demolishing
    ratio = theta1 * repairCost/constructionCost + theta2 * Age/60.0;

    if (!getHeritage()) {
        if (ratio > 0.6 - insured * 0.1) {
            return true;

        }
        else {
            return false;

        }
    }
    else {
        if (ratio > 0.95 - insured * 0.1) {
            return true;

        }
        else {
            return false;

        }
    }
}
