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

#include "RSeadonDirectConstructionCostModel.h"
#include "RLocation.h"
#include "math.h"
#include "RContinuousRandomVariable.h"

RSeadonDirectConstructionCostModel::RSeadonDirectConstructionCostModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = nullptr;
    theResponse = nullptr;
    theLabourResponse = nullptr;
    theRandomness = nullptr;

    // Cost rates of workers - dollars per crew per day
    costCarpenters = 71.59;
    costRodmen = 83.7;
    costPour = 79.04;
    timeSteel = 189.3;
    costForms = 370.0;

    // Create the response objects
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    theDomain->createObject(objectName() + "LabourResponse", "RGenericResponse");
    theLabourResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theLabourResponse->setModel(this);

    // This random variable lumps all the uncertainty in the model
    theDomain->createObject(objectName() + "RandomVariable", "RContinuousRandomVariable");
    theRandomness = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRandomness->setDistributionType(QString("Normal (mean, stdv)"));
    theRandomness->setMean(QString("1"));
    theRandomness->setCoefficientOfVariation("0.2");
}


RSeadonDirectConstructionCostModel::~RSeadonDirectConstructionCostModel()
{
}


QObject * RSeadonDirectConstructionCostModel::getBIM() const
{
    return theBIM;
}


void RSeadonDirectConstructionCostModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


int RSeadonDirectConstructionCostModel::evaluateModel(RGradientType theGradientType)
{
    if (theBIM->getTimeOfConstructionValue() == theCurrentTime->getCurrentValue())
    {
        // Initiate the pairs to accumulate cost and labour responses
        QPair<double, double> thisPair;
        QPair<double, double> theCostLabour;
        theCostLabour.first = 0.0;
        theCostLabour.second = 0.0;

        QList<RComponent *> theComponentList = theBIM->getComponentList();

        // Loop over components, calculate costs
        for (auto&& it: theComponentList)
        {
            if (qobject_cast<RRectangularRCColumnComponent *>(it))
            {
                thisPair = rectangularRCColumnCost(qobject_cast<RRectangularRCColumnComponent *>(it));
            }
            else if (qobject_cast<RCircularRCColumnComponent *>(it))
            {
                thisPair = circularRCColumnCost(qobject_cast<RCircularRCColumnComponent *>(it));
            }
            else if (qobject_cast<RRCSlabComponent *>(it))
            {
                thisPair = RCSlabCost(qobject_cast<RRCSlabComponent *>(it));
            }
            else if (qobject_cast<RRCShearWallComponent *>(it))
            {
                thisPair = RCShearWallCost(qobject_cast<RRCShearWallComponent *>(it));
            }
            else {
                qCritical() << this->objectName() << " is missing a construction cost model for " << it->objectName();
                thisPair.first = 0.0;
                thisPair.second = 0.0;
            }

            theCostLabour.first += thisPair.first;
            theCostLabour.second += thisPair.second;
        }

        theResponse->setCurrentValue(theCostLabour.first * theRandomness->getCurrentValue());
        theLabourResponse->setCurrentValue(theCostLabour.second);

    }
    else {
        theResponse->setCurrentValue(0.0);
        theLabourResponse->setCurrentValue(0.0);
    }

    return 0;

}


QPair<double, double> RSeadonDirectConstructionCostModel::rectangularRCColumnCost(RRectangularRCColumnComponent *theComponent)
{
    // Initialization
    QPair<double, double> theCostLabour;
    theCostLabour.first = 0.0;
    theCostLabour.second = 0.0;
    double labour;

    // Costing parameters
    double formsRatio = 0.0762;
    double timeForms = 1.9;
    double timePour = 1/(qMax(theComponent->getCrossSectionDepth()->getCurrentValue(),theComponent->getCrossSectionWidth()->getCurrentValue())*12+2.35);

    // Formwork - no re-uses
    labour = timeForms*(theComponent->getSurfaceArea());
    theCostLabour.second += labour;
    theCostLabour.first += 1.1*costForms*formsRatio*(theComponent->getSurfaceArea());
    theCostLabour.first += costCarpenters*labour;

    // Reinforcement
    labour = timeSteel*(theComponent->getVolumeOfSteel());
    theCostLabour.second += labour;
    theCostLabour.first += costRodmen*labour;

    // Pouring
    labour = (theComponent->getVolumeOfConcrete())*timePour;
    theCostLabour.second += labour;
    theCostLabour.first += labour*costPour;

    return theCostLabour;
}


QPair<double, double> RSeadonDirectConstructionCostModel::circularRCColumnCost(RCircularRCColumnComponent *theComponent)
{
    // Initialization
    QPair<double, double> theCostLabour;
    theCostLabour.first = 0.0;
    theCostLabour.second = 0.0;
    double labour;

    // Costing parameters
    double costForms = pow(((theComponent->getDiameter())->getCurrentValue()),2)*71.84+6.446;
    double timeForms = (theComponent->getDiameter())->getCurrentValue()*0.335+0.6;
    double timePour = 1/((theComponent->getDiameter())->getCurrentValue()*12+2.35);

    // Formwork - no re-uses
    labour = timeForms*(theComponent->getLength());
    theCostLabour.second += labour;
    theCostLabour.first += 1.1*costForms*(theComponent->getLength());
    theCostLabour.first += costCarpenters*labour;

    // Reinforcement
    labour = timeSteel*(theComponent->getVolumeOfSteel());
    theCostLabour.second += labour;
    theCostLabour.first += costRodmen*labour;

    // Pouring
    labour = (theComponent->getVolumeOfConcrete())*timePour;
    theCostLabour.second += labour;
    theCostLabour.first += labour*costPour;

    return theCostLabour;
}


QPair<double, double> RSeadonDirectConstructionCostModel::RCSlabCost(RRCSlabComponent *theComponent)
{
    // Initialization
    QPair<double, double> theCostLabour;
    theCostLabour.first = 0.0;
    theCostLabour.second = 0.0;
    double labour;

    // Costing parameters
    double formsRatio = 0.0508;
    double timeForms = 1.0;
    double timePour = 0.5;

    // Formwork - no re-uses
    labour = timeForms*(theComponent->getPlanArea());
    theCostLabour.second += labour;
    theCostLabour.first += 1.1*costForms*formsRatio*(theComponent->getPlanArea());
    theCostLabour.first += costCarpenters*labour;

    // Reinforcement
    labour = timeSteel*(theComponent->getVolumeOfSteel());
    theCostLabour.second += labour;
    theCostLabour.first += costRodmen*labour;

    // Pouring
    labour = (theComponent->getVolumeOfConcrete())*timePour;
    theCostLabour.second += labour;
    theCostLabour.first += labour*costPour;

    return theCostLabour;
}


QPair<double, double> RSeadonDirectConstructionCostModel::RCShearWallCost(RRCShearWallComponent *theComponent)
{
    // Initialization
    QPair<double, double> theCostLabour;
    theCostLabour.first = 0.0;
    theCostLabour.second = 0.0;
    double labour;

    // Costing parameters
    double formsRatio = 0.0635;
    double timeForms = 1.83;
    double timePour = 0.65;

    // Formwork - no re-uses
    labour = timeForms*(theComponent->getElevationArea());
    theCostLabour.second += labour;
    theCostLabour.first += 1.1*costForms*formsRatio*(theComponent->getElevationArea());
    theCostLabour.first += costCarpenters*labour;

    // Reinforcement
    labour = timeSteel*(theComponent->getVolumeOfSteel());
    theCostLabour.second += labour;
    theCostLabour.first += costRodmen*labour;

    // Pouring
    labour = (theComponent->getVolumeOfConcrete())*timePour;
    theCostLabour.second += labour;
    theCostLabour.first += labour*costPour;

    return theCostLabour;
}
