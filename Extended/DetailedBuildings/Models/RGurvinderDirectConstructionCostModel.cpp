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

#include "RGurvinderDirectConstructionCostModel.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RComponent.h"
#include "RCircularRCColumnComponent.h"
#include "RCLTPanelComponent.h"
#include "RCompositePanelComponent.h"
#include "RHSSRoundBeamColumnComponent.h"
#include "RRCSlabComponent.h"
#include "RRectangularGlulamBeamColumnComponent.h"
#include "RWSteelBeamColumnComponent.h"

RGurvinderDirectConstructionCostModel::RGurvinderDirectConstructionCostModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    // Random variables for numbers in our database
    theResponse = nullptr;
    theBIM = nullptr;
    theCircularRCColumnTheta1 = nullptr;
    theCircularRCColumnTheta2 = nullptr;
    theCircularRCColumnTheta3 = nullptr;
    theCircularRCColumnTheta4 = nullptr;
    theCircularRCColumnEpsilon = nullptr;
    theCLTPanelTheta1 = nullptr;
    theCLTPanelTheta2 = nullptr;
    theCLTPanelTheta3 = nullptr;
    theCLTPanelEpsilon = nullptr;
    theCorrugatedRoofTheta1 = nullptr;
    theCorrugatedRoofTheta2 = nullptr;
    theCorrugatedRoofTheta3 = nullptr;
    theCorrugatedRoofEpsilon = nullptr;
    theRCSlabTheta1 = nullptr;
    theRCSlabTheta2 = nullptr;
    theRCSlabTheta3 = nullptr;
    theRCSlabTheta4 = nullptr;
    theRCSlabTheta5 = nullptr;
    theRCSlabEpsilon = nullptr;
    theRectangularTimberBeamColumnTheta1 = nullptr;
    theRectangularTimberBeamColumnTheta2 = nullptr;
    theRectangularTimberBeamColumnEpsilon = nullptr;
    theWSteelBeamColumnTheta1 = nullptr;
    theWSteelBeamColumnTheta2 = nullptr;
    theWSteelBeamColumnEpsilon = nullptr;

    // consider splitting this into multiple databases
    createDatabaseOfRandomVariables();

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

}


RGurvinderDirectConstructionCostModel::~RGurvinderDirectConstructionCostModel()
{

}


QObject * RGurvinderDirectConstructionCostModel::getBIM() const
{
    return theBIM;
}


void RGurvinderDirectConstructionCostModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


int RGurvinderDirectConstructionCostModel::evaluateModel(RGradientType theGradientType)
{

    if (theBIM->getTimeOfConstructionValue() == theCurrentTime->getCurrentValue())
    {
        double cost = 0.0;
        QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();

        // Loop over components, calculate costs
        for (auto&& it: theComponentList)
        {
            if (qobject_cast<RCircularRCColumnComponent *>(it))
            {
                cost += circularRCColumnCost(qobject_cast<RCircularRCColumnComponent *>(it));
            }
            else if(qobject_cast<RCLTPanelComponent *>(it))
            {
                cost += CLTPanelCost(qobject_cast<RCLTPanelComponent *>(it));
            }
            else if(qobject_cast<RCompositePanelComponent *>(it))
            {
                cost += corrugatedRoofCost(qobject_cast<RCompositePanelComponent *>(it));
            }
            else if (qobject_cast<RRCSlabComponent *>(it))
            {
                cost += RCSlabCost(qobject_cast<RRCSlabComponent *>(it));
            }
            else if(qobject_cast<RRectangularGlulamBeamColumnComponent *>(it))
            {
                cost += rectangularTimberBeamColumnCost(qobject_cast<RRectangularGlulamBeamColumnComponent *>(it));
            }
            else if (qobject_cast<RWSteelBeamColumnComponent *>(it))
            {
                cost += WSteelBeamColumnCost(qobject_cast<RWSteelBeamColumnComponent *>(it));
            }
            else {
                qCritical() << this->objectName() << " is missing a construction cost model";
            }
        }

        theResponse->setCurrentValue(cost);
    }
    else {
        theResponse->setCurrentValue(0.0);
    }

    return 0;
}


void RGurvinderDirectConstructionCostModel::createDatabaseOfRandomVariables()
{
    theDomain->createObject(objectName() + "CircularRCColumnTheta1", "RContinuousRandomVariable");
    theCircularRCColumnTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCircularRCColumnTheta1->setDistributionType(QString("Normal (mean, stdv)"));
    theCircularRCColumnTheta1->setMean(QString("3.01"));
    theCircularRCColumnTheta1->setCoefficientOfVariation("0.0064");

    theDomain->createObject(objectName() + "CircularRCColumnTheta2", "RContinuousRandomVariable");
    theCircularRCColumnTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCircularRCColumnTheta2->setDistributionType(QString("Normal (mean, stdv)"));
    theCircularRCColumnTheta2->setMean(QString("2.67"));
    theCircularRCColumnTheta2->setCoefficientOfVariation("0.007");

    theDomain->createObject(objectName() + "CircularRCColumnTheta3", "RContinuousRandomVariable");
    theCircularRCColumnTheta3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCircularRCColumnTheta3->setDistributionType(QString("Normal (mean, stdv)"));
    theCircularRCColumnTheta3->setMean(QString("0.76"));
    theCircularRCColumnTheta3->setCoefficientOfVariation("0.0324");

    theDomain->createObject(objectName() + "CircularRCColumnTheta4", "RContinuousRandomVariable");
    theCircularRCColumnTheta4 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCircularRCColumnTheta4->setDistributionType(QString("Normal (mean, stdv)"));
    theCircularRCColumnTheta4->setMean(QString("33.06"));
    theCircularRCColumnTheta4->setCoefficientOfVariation("0.01287");

    theDomain->createObject(objectName() + "CircularRCColumnEpsilon", "RContinuousRandomVariable");
    theCircularRCColumnEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCircularRCColumnEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
    theCircularRCColumnEpsilon->setMean(QString("0.0"));
    theCircularRCColumnEpsilon->setStandardDeviation("0.42");


    theDomain->createObject(objectName() + "CLTPanelTheta1", "RContinuousRandomVariable");
    theCLTPanelTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCLTPanelTheta1->setDistributionType(QString("Normal (mean, stdv)"));
    theCLTPanelTheta1->setMean(QString("1.34585"));
    theCLTPanelTheta1->setCoefficientOfVariation("0.104525");

    theDomain->createObject(objectName() + "CLTPanelTheta2", "RContinuousRandomVariable");
    theCLTPanelTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCLTPanelTheta2->setDistributionType(QString("Normal (mean, stdv)"));
    theCLTPanelTheta2->setMean(QString("0.104166"));
    theCLTPanelTheta2->setCoefficientOfVariation("0.007");

    theDomain->createObject(objectName() + "CLTPanelTheta3", "RContinuousRandomVariable");
    theCLTPanelTheta3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCLTPanelTheta3->setDistributionType(QString("Normal (mean, stdv)"));
    theCLTPanelTheta3->setMean(QString("17.2714"));
    theCLTPanelTheta3->setCoefficientOfVariation("0.0845901");

    theDomain->createObject(objectName() + "CLTPanelEpsilon", "RContinuousRandomVariable");
    theCLTPanelEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCLTPanelEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
    theCLTPanelEpsilon->setMean(QString("0.0"));
    theCLTPanelEpsilon->setStandardDeviation("0.431411");

    theDomain->createObject(objectName() + "CorrugatedRoofTheta1", "RContinuousRandomVariable");
    theCorrugatedRoofTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCorrugatedRoofTheta1->setDistributionType(QString("Normal (mean, stdv)"));
    theCorrugatedRoofTheta1->setMean(QString("0.613584"));
    theCorrugatedRoofTheta1->setCoefficientOfVariation("0.742129");

    theDomain->createObject(objectName() + "CorrugatedRoofTheta2", "RContinuousRandomVariable");
    theCorrugatedRoofTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCorrugatedRoofTheta2->setDistributionType(QString("Normal (mean, stdv)"));
    theCorrugatedRoofTheta2->setMean(QString("0.74133"));
    theCorrugatedRoofTheta2->setCoefficientOfVariation("0.458416 ");

    theDomain->createObject(objectName() + "CorrugatedRoofTheta3", "RContinuousRandomVariable");
    theCorrugatedRoofTheta3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCorrugatedRoofTheta3->setDistributionType(QString("Normal (mean, stdv)"));
    theCorrugatedRoofTheta3->setMean(QString("36.0187"));
    theCorrugatedRoofTheta3->setCoefficientOfVariation("0.131296");

    theDomain->createObject(objectName() + "CorrugatedRoofEpsilon", "RContinuousRandomVariable");
    theCorrugatedRoofEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCorrugatedRoofEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
    theCorrugatedRoofEpsilon->setMean(QString("0.0"));
    theCorrugatedRoofEpsilon->setStandardDeviation("1.39645");


    theDomain->createObject(objectName() + "RCSlabTheta1", "RContinuousRandomVariable");
    theRCSlabTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRCSlabTheta1->setDistributionType(QString("Normal (mean, stdv)"));
    theRCSlabTheta1->setMean(QString("1.14725"));
    theRCSlabTheta1->setCoefficientOfVariation("0.0951018 ");

    theDomain->createObject(objectName() + "RCSlabTheta2", "RContinuousRandomVariable");
    theRCSlabTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRCSlabTheta2->setDistributionType(QString("Normal (mean, stdv)"));
    theRCSlabTheta2->setMean(QString("0.924642"));
    theRCSlabTheta2->setCoefficientOfVariation("0.114329");

    theDomain->createObject(objectName() + "RCSlabTheta3", "RContinuousRandomVariable");
    theRCSlabTheta3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRCSlabTheta3->setDistributionType(QString("Normal (mean, stdv)"));
    theRCSlabTheta3->setMean(QString("11.5121"));
    theRCSlabTheta3->setCoefficientOfVariation("0.0905687");

    theDomain->createObject(objectName() + "RCSlabTheta4", "RContinuousRandomVariable");
    theRCSlabTheta4 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRCSlabTheta4->setDistributionType(QString("Normal (mean, stdv)"));
    theRCSlabTheta4->setMean(QString("-0.0447446"));
    theRCSlabTheta4->setCoefficientOfVariation("0.326021");

    theDomain->createObject(objectName() + "RCSlabTheta5", "RContinuousRandomVariable");
    theRCSlabTheta5 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRCSlabTheta5->setDistributionType(QString("Normal (mean, stdv)"));
    theRCSlabTheta5->setMean(QString("359.772"));
    theRCSlabTheta5->setCoefficientOfVariation("0.0869725");

    theDomain->createObject(objectName() + "RCSlabEpsilon", "RContinuousRandomVariable");
    theRCSlabEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRCSlabEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
    theRCSlabEpsilon->setMean(QString("0.0"));
    theRCSlabEpsilon->setStandardDeviation("0.801987");


    theDomain->createObject(objectName() + "RectangularTimberBeamColumnTheta1", "RContinuousRandomVariable");
    theRectangularTimberBeamColumnTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRectangularTimberBeamColumnTheta1->setDistributionType(QString("Normal (mean, stdv)"));
    theRectangularTimberBeamColumnTheta1->setMean(QString("1.12185"));
    theRectangularTimberBeamColumnTheta1->setCoefficientOfVariation("0.0212455 ");

    theDomain->createObject(objectName() + "RectangularTimberBeamColumnTheta2", "RContinuousRandomVariable");
    theRectangularTimberBeamColumnTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRectangularTimberBeamColumnTheta2->setDistributionType(QString("Normal (mean, stdv)"));
    theRectangularTimberBeamColumnTheta2->setMean(QString("13.4026"));
    theRectangularTimberBeamColumnTheta2->setCoefficientOfVariation("0.010852");

    theDomain->createObject(objectName() + "RectangularTimberBeamColumnEpsilon", "RContinuousRandomVariable");
    theRectangularTimberBeamColumnEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRectangularTimberBeamColumnEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
    theRectangularTimberBeamColumnEpsilon->setMean(QString("0.0"));
    theRectangularTimberBeamColumnEpsilon->setStandardDeviation("0.161562");

    theDomain->createObject(objectName() + "WSteelBeamColumnTheta1", "RContinuousRandomVariable");
    theWSteelBeamColumnTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theWSteelBeamColumnTheta1->setDistributionType(QString("Normal (mean, stdv)"));
    theWSteelBeamColumnTheta1->setMean(QString("1.45158"));
    theWSteelBeamColumnTheta1->setCoefficientOfVariation("0.124036");

    theDomain->createObject(objectName() + "WSteelBeamColumnTheta2", "RContinuousRandomVariable");
    theWSteelBeamColumnTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theWSteelBeamColumnTheta2->setDistributionType(QString("Normal (mean, stdv)"));
    theWSteelBeamColumnTheta2->setMean(QString("13.9135"));
    theWSteelBeamColumnTheta2->setCoefficientOfVariation("0.285169");

    theDomain->createObject(objectName() + "WSteelBeamColumnEpsilon", "RContinuousRandomVariable");
    theWSteelBeamColumnEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theWSteelBeamColumnEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
    theWSteelBeamColumnEpsilon->setMean(QString("0.0"));
    theWSteelBeamColumnEpsilon->setStandardDeviation("0.489489");
}


double RGurvinderDirectConstructionCostModel::circularRCColumnCost(RCircularRCColumnComponent *theComponent)
{
    double theta1 = theCircularRCColumnTheta1->getCurrentValue();
    double theta2 = theCircularRCColumnTheta2->getCurrentValue();
    double theta3 = theCircularRCColumnTheta3->getCurrentValue();
    double theta4 = theCircularRCColumnTheta4->getCurrentValue();
    double epsilon = theCircularRCColumnEpsilon->getCurrentValue();

    QString theConcreteStrength = theComponent->getConcreteStrength();
    double fc = 20.0e6;
    if (theConcreteStrength == "C20")
        fc = 20.0e6;
    else if (theConcreteStrength == "C25")
        fc = 25.0e6;
    else if (theConcreteStrength == "C30")
        fc = 30.0e6;

    double rho = (theComponent->getTheLongitudinalReinforcementRatioParameter())->getCurrentValue();

    double d = (theComponent->getDiameter())->getCurrentValue();

    double height = theComponent->getLength();

    double cost = theta1 * height/3.048 + theta2 * d/0.41 + theta3 * fc/27.58e6 + theta4 * rho * d/0.41 + epsilon;

    cost = pow(cost, 3.0);

    return cost;
}


double RGurvinderDirectConstructionCostModel::CLTPanelCost(RCLTPanelComponent *theComponent)
{
    double theta1 = theCLTPanelTheta1->getCurrentValue();
    double theta2 = theCLTPanelTheta2->getCurrentValue();
    double theta3 = theCLTPanelTheta3->getCurrentValue();
    double epsilon = theCLTPanelEpsilon->getCurrentValue();

    double l = theComponent->getDepth();
    double w = theComponent->getWidth();
    double t = (theComponent->getThickness())->getCurrentValue();

    double cost = theta1 * l + theta2 * w + theta3 * t + epsilon;

    return cost;
}


double RGurvinderDirectConstructionCostModel::corrugatedRoofCost(RCompositePanelComponent *theComponent)
{
    double theta1 = theCorrugatedRoofTheta1->getCurrentValue();
    double theta2 = theCorrugatedRoofTheta2->getCurrentValue();
    double theta3 = theCorrugatedRoofTheta3->getCurrentValue();
    double epsilon = theCorrugatedRoofEpsilon->getCurrentValue();

    double l = theComponent->getDeckingDepth();
    double w = theComponent->getWidth();
    double t = (theComponent->getThickness())->getCurrentValue();

    double cost = theta1 * l + theta2 * w + theta3 * t + epsilon;

    cost = pow(cost, 3.0);

    return cost;
}


double RGurvinderDirectConstructionCostModel::RCSlabCost(RRCSlabComponent *theComponent)
{
    double theta1 = theRCSlabTheta1->getCurrentValue();
    double theta2 = theRCSlabTheta2->getCurrentValue();
    double theta3 = theRCSlabTheta3->getCurrentValue();
    double theta4 = theRCSlabTheta4->getCurrentValue();
    double theta5 = theRCSlabTheta5->getCurrentValue();
    double epsilon = theRCSlabEpsilon->getCurrentValue();

    double l = theComponent->getDepth();
    double w = theComponent->getWidth();
    double t = (theComponent->getThickness())->getCurrentValue();
    //double fc = theMaterialKnowledge->getCompressiveStrengthfc();
    // material knowledge pointers not fully implemented
    double fc = 20.0e6;
    double rho = theComponent->getTheLongitudinalReinforcementRatioParameter()->getCurrentValue();

    //double cost = theta1 * l + theta2 * w + theta3 * t + theta4 * fc + theta5 * rho + epsilon; //Original

    double cost = theta1 * l + theta2 * w + theta3 * t + theta4 * fc/1.0e6 + theta5 * rho + epsilon; //SG change

    cost = pow(cost, 3.0);

    return cost;
}


double RGurvinderDirectConstructionCostModel::rectangularTimberBeamColumnCost(RRectangularGlulamBeamColumnComponent *theComponent)
{
    double theta1 = theRectangularTimberBeamColumnTheta1->getCurrentValue();
    double theta2 = theRectangularTimberBeamColumnTheta2->getCurrentValue();
    double epsilon = theRectangularTimberBeamColumnEpsilon->getCurrentValue();

    double w = (theComponent->getCrossSectionWidth())->getCurrentValue();
    double d = (theComponent->getCrossSectionDepth())->getCurrentValue();
    double h = theComponent->getLength();

    // Gurvinder developed this model for circular timber columns, with diameter d, hence this temp fix:
    double cost = theta1 * h + theta2 * 0.5*(w+d) + epsilon;

    cost = pow(cost, 3.0);

    return cost;
}


double RGurvinderDirectConstructionCostModel::WSteelBeamColumnCost(RWSteelBeamColumnComponent *theComponent)
{

    double theta1 = theWSteelBeamColumnTheta1->getCurrentValue();
    double theta2 = theWSteelBeamColumnTheta2->getCurrentValue();
    double epsilon = theWSteelBeamColumnEpsilon->getCurrentValue();

    double d = (theComponent->getWebHeight())->getCurrentValue();
    double h = theComponent->getLength();

    double cost = theta1 * h + theta2 * d + epsilon;

    cost = pow(cost, 3.0);

    return cost;
}

