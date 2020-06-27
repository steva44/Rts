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

#include "RMojtabasCombinedBuildingModel.h"
#include "RBIM.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RConstant.h"
#include "RNormalDistribution.h"

#include <math.h>

RMojtabasCombinedBuildingModel::RMojtabasCombinedBuildingModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = 0;
    theSa = 0;

    theDomain->createObject(objectName() + "DriftResponse", "RGenericResponse");
    thePeakDrift = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePeakDrift->setModel(this);

    theDomain->createObject(objectName() + "AccelerationResponse", "RGenericResponse");
    thePeakAcc = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePeakAcc->setModel(this);

    theDomain->createObject(objectName() + "RepairCostResponse", "RGenericResponse");
    theRepairCost = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theRepairCost->setModel(this);


    // *************************
    //    RESPONSE
    // *************************


    // Create the model parameters, which must be the SAME for all instances of this model, but DIFFERENT mean/stdv for different building types... :)

    QString responseTheta1Name = "BuildingResponseModelTheta1";
    theResponseTheta1 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta1Name);
    if (!theResponseTheta1) {
        theDomain->createObject(responseTheta1Name, "RContinuousRandomVariable");
        theResponseTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta1->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta1->setMean("0.0");
        theResponseTheta1->setStandardDeviation("1.0");
        theAutoParametersList << theResponseTheta1;
    }

    QString responseTheta2Name = "BuildingResponseModelTheta2";
    theResponseTheta2 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta2Name);
    if (!theResponseTheta2) {
        theDomain->createObject(responseTheta2Name, "RContinuousRandomVariable");
        theResponseTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta2->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta2->setMean("0.0");
        theResponseTheta2->setStandardDeviation("1.0");
        theAutoParametersList << theResponseTheta2;
    }

    QString responseTheta3Name = "BuildingResponseModelTheta3";
    theResponseTheta3 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta3Name);
    if (!theResponseTheta3) {
        theDomain->createObject(responseTheta3Name, "RContinuousRandomVariable");
        theResponseTheta3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta3->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta3->setMean("0.0");
        theResponseTheta3->setStandardDeviation("1.0");
        theAutoParametersList << theResponseTheta3;
    }

    QString responseTheta4Name = "BuildingResponseModelTheta4";
    theResponseTheta4 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta4Name);
    if (!theResponseTheta4) {
        theDomain->createObject(responseTheta4Name, "RContinuousRandomVariable");
        theResponseTheta4 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta4->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta4->setMean("0.0");
        theResponseTheta4->setStandardDeviation("1.0");
        theAutoParametersList << theResponseTheta4;
    }

    QString responseTheta5Name = "BuildingResponseModelTheta5";
    theResponseTheta5 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta5Name);
    if (!theResponseTheta5) {
        theDomain->createObject(responseTheta5Name, "RContinuousRandomVariable");
        theResponseTheta5 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta5->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta5->setMean("0.0");
        theResponseTheta5->setStandardDeviation("1.0");
        theAutoParametersList << theResponseTheta5;
    }

    QString responseTheta6Name = "BuildingResponseModelTheta6";
    theResponseTheta6 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta6Name);
    if (!theResponseTheta6) {
        theDomain->createObject(responseTheta6Name, "RContinuousRandomVariable");
        theResponseTheta6 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta6->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta6->setMean("0.0");
        theResponseTheta6->setStandardDeviation("1.0");
        theAutoParametersList << theResponseTheta6;
    }

    QString responseTheta7Name = "BuildingResponseModelTheta7";
    theResponseTheta7 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta7Name);
    if (!theResponseTheta7) {
        theDomain->createObject(responseTheta7Name, "RContinuousRandomVariable");
        theResponseTheta7 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta7->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta7->setMean("0.0");
        theResponseTheta7->setStandardDeviation("1.0");
        theAutoParametersList << theResponseTheta7;
    }

    QString responseTheta8Name = "BuildingResponseModelTheta8";
    theResponseTheta8 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta8Name);
    if (!theResponseTheta8) {
        theDomain->createObject(responseTheta8Name, "RContinuousRandomVariable");
        theResponseTheta8 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta8->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta8->setMean("0.0");
        theResponseTheta8->setStandardDeviation("1.0");
        theAutoParametersList << theResponseTheta8;
    }

    QString responseTheta9Name = "BuildingResponseModelTheta9";
    theResponseTheta9 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta9Name);
    if (!theResponseTheta9) {
        theDomain->createObject(responseTheta9Name, "RContinuousRandomVariable");
        theResponseTheta9 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta9->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta9->setMean("0.599505");
        theResponseTheta9->setCoefficientOfVariation("0.0449346");
        theAutoParametersList << theResponseTheta9;
    }

    QString responseTheta10Name = "BuildingResponseModelTheta10";
    theResponseTheta10 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta10Name);
    if (!theResponseTheta10) {
        theDomain->createObject(responseTheta10Name, "RContinuousRandomVariable");
        theResponseTheta10 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta10->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta10->setMean("0.30647");
        theResponseTheta10->setCoefficientOfVariation("0.0854928");
        theAutoParametersList << theResponseTheta10;
    }

    QString responseTheta11Name = "BuildingResponseModelTheta11";
    theResponseTheta11 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta11Name);
    if (!theResponseTheta11) {
        theDomain->createObject(responseTheta11Name, "RContinuousRandomVariable");
        theResponseTheta11 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta11->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta11->setMean("1.11074");
        theResponseTheta11->setCoefficientOfVariation("0.0167152");
        theAutoParametersList << theResponseTheta11;
    }

    QString responseTheta12Name = "BuildingResponseModelTheta12";
    theResponseTheta12 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta12Name);
    if (!theResponseTheta12) {
        theDomain->createObject(responseTheta12Name, "RContinuousRandomVariable");
        theResponseTheta12 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta12->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta12->setMean("0.138935");
        theResponseTheta12->setCoefficientOfVariation("0.0359842");
        theAutoParametersList << theResponseTheta12;
    }

    QString responseTheta13Name = "BuildingResponseModelTheta13";
    theResponseTheta13 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta13Name);
    if (!theResponseTheta13) {
        theDomain->createObject(responseTheta13Name, "RContinuousRandomVariable");
        theResponseTheta13 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta13->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta13->setMean("1.02909");
        theResponseTheta13->setCoefficientOfVariation("0.00716849");
        theAutoParametersList << theResponseTheta13;
    }

    QString responseTheta14Name = "BuildingResponseModelTheta14";
    theResponseTheta14 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta14Name);
    if (!theResponseTheta14) {
        theDomain->createObject(responseTheta14Name, "RContinuousRandomVariable");
        theResponseTheta14 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta14->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta14->setMean("0.200544");
        theResponseTheta14->setCoefficientOfVariation("0.175224");
        theAutoParametersList << theResponseTheta14;
    }

    QString responseTheta15Name = "BuildingResponseModelTheta15";
    theResponseTheta15 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta15Name);
    if (!theResponseTheta15) {
        theDomain->createObject(responseTheta15Name, "RContinuousRandomVariable");
        theResponseTheta15 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta15->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta15->setMean("1.38183");
        theResponseTheta15->setCoefficientOfVariation("0.0725668");
        theAutoParametersList << theResponseTheta15;
    }

    QString responseTheta16Name = "BuildingResponseModelTheta16";
    theResponseTheta16 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta16Name);
    if (!theResponseTheta16) {
        theDomain->createObject(responseTheta16Name, "RContinuousRandomVariable");
        theResponseTheta16 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta16->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta16->setMean("0.114734");
        theResponseTheta16->setCoefficientOfVariation("0.0726954");
        theAutoParametersList << theResponseTheta16;
    }

    QString responseTheta17Name = "BuildingResponseModelTheta17";
    theResponseTheta17 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta17Name);
    if (!theResponseTheta17) {
        theDomain->createObject(responseTheta17Name, "RContinuousRandomVariable");
        theResponseTheta17 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta17->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta17->setMean("0.284588");
        theResponseTheta17->setCoefficientOfVariation("0.0374309");
        theAutoParametersList << theResponseTheta17;
    }

    QString responseTheta18Name = "BuildingResponseModelTheta18";
    theResponseTheta18 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta18Name);
    if (!theResponseTheta18) {
        theDomain->createObject(responseTheta18Name, "RContinuousRandomVariable");
        theResponseTheta18 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta18->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta18->setMean("0.0888824");
        theResponseTheta18->setCoefficientOfVariation("0.169121");
        theAutoParametersList << theResponseTheta18;
    }

    QString responseTheta19Name = "BuildingResponseModelTheta19";
    theResponseTheta19 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta19Name);
    if (!theResponseTheta19) {
        theDomain->createObject(responseTheta19Name, "RContinuousRandomVariable");
        theResponseTheta19 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta19->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta19->setMean("0.109799");
        theResponseTheta19->setCoefficientOfVariation("0.0208415");
        theAutoParametersList << theResponseTheta19;
    }

    QString responseTheta20Name = "BuildingResponseModelTheta20";
    theResponseTheta20 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta20Name);
    if (!theResponseTheta20) {
        theDomain->createObject(responseTheta20Name, "RContinuousRandomVariable");
        theResponseTheta20 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta20->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta20->setMean("0.797943");
        theResponseTheta20->setCoefficientOfVariation("0.00416237");
        theAutoParametersList << theResponseTheta20;
    }

    QString responseTheta21Name = "BuildingResponseModelTheta21";
    theResponseTheta21 = theDomain->findChild<RContinuousRandomVariable *>(responseTheta21Name);
    if (!theResponseTheta21) {
        theDomain->createObject(responseTheta21Name, "RContinuousRandomVariable");
        theResponseTheta21 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseTheta21->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseTheta21->setMean("0.710859");
        theResponseTheta21->setCoefficientOfVariation("0.0760852");
        theAutoParametersList << theResponseTheta21;
    }

    QString driftSigmaName = "BuildingResponseModelDriftSigma";
    theResponseSigma1 = theDomain->findChild<RContinuousRandomVariable *>(driftSigmaName);
    if (!theResponseSigma1) {
        theDomain->createObject(driftSigmaName, "RContinuousRandomVariable");
        theResponseSigma1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseSigma1->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseSigma1->setMean("0.390434");
        theResponseSigma1->setCoefficientOfVariation("0.00965384");
        theAutoParametersList << theResponseSigma1;
    }

    QString accSigmaName = "BuildingResponseModelAccSigma";
    theResponseSigma2 = theDomain->findChild<RContinuousRandomVariable *>(accSigmaName);
    if (!theResponseSigma2) {
        theDomain->createObject(accSigmaName, "RContinuousRandomVariable");
        theResponseSigma2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseSigma2->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseSigma2->setMean("0.224021");
        theResponseSigma2->setCoefficientOfVariation("0.00965294");
        theAutoParametersList << theResponseSigma2;
    }

    QString driftEpsilonName = "BuildingResponseModelDriftEpsilon";
    theResponseEpsilon1 = theDomain->findChild<RContinuousRandomVariable *>(driftEpsilonName);
    if (!theResponseEpsilon1) {
        theDomain->createObject(driftEpsilonName, "RContinuousRandomVariable");
        theResponseEpsilon1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseEpsilon1->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseEpsilon1->setMean("0.0");
        theResponseEpsilon1->setStandardDeviation("1.0");
        theAutoParametersList << theResponseEpsilon1;
    }

    QString accEpsilonName = "BuildingResponseModelAccEpsilon";
    theResponseEpsilon2 = theDomain->findChild<RContinuousRandomVariable *>(accEpsilonName);
    if (!theResponseEpsilon2) {
        theDomain->createObject(accEpsilonName, "RContinuousRandomVariable");
        theResponseEpsilon2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theResponseEpsilon2->setDistributionType(QString("Normal (mean, stdv)"));
        theResponseEpsilon2->setMean("0.0");
        theResponseEpsilon2->setStandardDeviation("1.0");
        theAutoParametersList << theResponseEpsilon2;
    }



    // ***********************
    //     DAMAGE
    // ***********************

    theBIM->getConstructionType() = "Concrete frame";

    theDomain->createObject(objectName() + "StructuralResponse", "RGenericResponse");
    theStructuralDamageRatio = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theStructuralDamageRatio->setModel(this);

    theDomain->createObject(objectName() + "NonstructuralDriftResponse", "RGenericResponse");
    theNonstructuralDriftDamageRatio = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theNonstructuralDriftDamageRatio->setModel(this);

    theDomain->createObject(objectName() + "NonstructuralAccelerationResponse", "RGenericResponse");
    theNonstructuralAccelerationDamageRatio = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theNonstructuralAccelerationDamageRatio->setModel(this);

    theDomain->createObject(objectName() + "ContentResponse", "RGenericResponse");
    theContentDamageRatio = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theContentDamageRatio->setModel(this);


    // Create the model parameters, which must be the SAME for all instances of this model, but DIFFERENT mean/stdv for different building types... :)

    QString damageTheta1Name = "BuildingDamageModelTheta1";
    theDamageTheta1 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta1Name);
    if (!theDamageTheta1) {
        theDomain->createObject(damageTheta1Name, "RContinuousRandomVariable");
        theDamageTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta1->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta1->setMean("0.0");
        theDamageTheta1->setStandardDeviation("1.0");
        theAutoParametersList << theDamageTheta1;
    }

    QString damageTheta2Name = "BuildingDamageModelTheta2";
    theDamageTheta2 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta2Name);
    if (!theDamageTheta2) {
        theDomain->createObject(damageTheta2Name, "RContinuousRandomVariable");
        theDamageTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta2->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta2->setMean("0.0");
        theDamageTheta2->setStandardDeviation("1.0");
        theAutoParametersList << theDamageTheta2;
    }

    QString damageTheta3Name = "BuildingDamageModelTheta3";
    theDamageTheta3 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta3Name);
    if (!theDamageTheta3) {
        theDomain->createObject(damageTheta3Name, "RContinuousRandomVariable");
        theDamageTheta3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta3->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta3->setMean("0.0");
        theDamageTheta3->setStandardDeviation("1.0");
        theAutoParametersList << theDamageTheta3;
    }

    QString damageTheta4Name = "BuildingDamageModelTheta4";
    theDamageTheta4 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta4Name);
    if (!theDamageTheta4) {
        theDomain->createObject(damageTheta4Name, "RContinuousRandomVariable");
        theDamageTheta4 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta4->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta4->setMean("0.0");
        theDamageTheta4->setStandardDeviation("1.0");
        theAutoParametersList << theDamageTheta4;
    }

    QString damageTheta5Name = "BuildingDamageModelTheta5";
    theDamageTheta5 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta5Name);
    if (!theDamageTheta5) {
        theDomain->createObject(damageTheta5Name, "RContinuousRandomVariable");
        theDamageTheta5 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta5->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta5->setMean("0.0");
        theDamageTheta5->setStandardDeviation("1.0");
        theAutoParametersList << theDamageTheta5;
    }

    QString damageTheta6Name = "BuildingDamageModelTheta6";
    theDamageTheta6 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta6Name);
    if (!theDamageTheta6) {
        theDomain->createObject(damageTheta6Name, "RContinuousRandomVariable");
        theDamageTheta6 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta6->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta6->setMean("0.0");
        theDamageTheta6->setStandardDeviation("1.0");
        theAutoParametersList << theDamageTheta6;
    }

    QString damageTheta7Name = "BuildingDamageModelTheta7";
    theDamageTheta7 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta7Name);
    if (!theDamageTheta7) {
        theDomain->createObject(damageTheta7Name, "RContinuousRandomVariable");
        theDamageTheta7 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta7->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta7->setMean("0.0");
        theDamageTheta7->setStandardDeviation("1.0");
        theAutoParametersList << theDamageTheta7;
    }

    QString damageTheta8Name = "BuildingDamageModelTheta8";
    theDamageTheta8 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta8Name);
    if (!theDamageTheta8) {
        theDomain->createObject(damageTheta8Name, "RContinuousRandomVariable");
        theDamageTheta8 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta8->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta8->setMean("0.0");
        theDamageTheta8->setStandardDeviation("1.0");
        theAutoParametersList << theDamageTheta8;
    }

    QString damageTheta9Name = "BuildingDamageModelTheta9";
    theDamageTheta9 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta9Name);
    if (!theDamageTheta9) {
        theDomain->createObject(damageTheta9Name, "RContinuousRandomVariable");
        theDamageTheta9 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta9->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta9->setMean("0.0");
        theDamageTheta9->setStandardDeviation("1.0");
        theAutoParametersList << theDamageTheta9;
    }




/*
    QString epsilonName = "BuildingDamageModelEpsilon";
    theDamageEpsilon1 = theDomain->findChild<RContinuousRandomVariable *>(epsilonName);
    if (!theDamageEpsilon1) {
        theDomain->createObject(epsilonName, "RContinuousRandomVariable");
        theDamageEpsilon1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageEpsilon1->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageEpsilon1->setMean("0.0");
        theDamageEpsilon1->setStandardDeviation("1.0");
        theAutoParametersList << theDamageEpsilon1;
    }
*/




    QString damageTheta10Name = "BuildingDamageModelTheta10";
    theDamageTheta10 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta10Name);
    if (!theDamageTheta10) {
        theDomain->createObject(damageTheta10Name, "RContinuousRandomVariable");
        theDamageTheta10 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta10->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta10->setMean("1.27310387789009");
        theDamageTheta10->setCoefficientOfVariation("0.009659920321684");
        theAutoParametersList << theDamageTheta10;
    }

    QString damageTheta11Name = "BuildingDamageModelTheta11";
    theDamageTheta11 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta11Name);
    if (!theDamageTheta11) {
        theDomain->createObject(damageTheta11Name, "RContinuousRandomVariable");
        theDamageTheta11 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta11->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta11->setMean("4.86771763084631");
        theDamageTheta11->setCoefficientOfVariation("0.009500499962581");
        theAutoParametersList << theDamageTheta11;
    }

    QString damageTheta12Name = "BuildingDamageModelTheta12";
    theDamageTheta12 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta12Name);
    if (!theDamageTheta12) {
        theDomain->createObject(damageTheta12Name, "RContinuousRandomVariable");
        theDamageTheta12 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta12->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta12->setMean("1.26757284351552");
        theDamageTheta12->setCoefficientOfVariation("0.006798166443569");
        theAutoParametersList << theDamageTheta12;
    }

    QString damageTheta13Name = "BuildingDamageModelTheta13";
    theDamageTheta13 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta13Name);
    if (!theDamageTheta13) {
        theDomain->createObject(damageTheta13Name, "RContinuousRandomVariable");
        theDamageTheta13 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta13->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta13->setMean("0.193211895250043");
        theDamageTheta13->setCoefficientOfVariation("0.00827053736024");
        theAutoParametersList << theDamageTheta13;
    }

    QString damageTheta14Name = "BuildingDamageModelTheta14";
    theDamageTheta14 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta14Name);
    if (!theDamageTheta14) {
        theDomain->createObject(damageTheta14Name, "RContinuousRandomVariable");
        theDamageTheta14 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta14->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta14->setMean("1.29985185840713");
        theDamageTheta14->setCoefficientOfVariation("0.008459627480646");
        theAutoParametersList << theDamageTheta14;
    }

    QString damageTheta15Name = "BuildingDamageModelTheta15";
    theDamageTheta15 = theDomain->findChild<RContinuousRandomVariable *>(damageTheta15Name);
    if (!theDamageTheta15) {
        theDomain->createObject(damageTheta15Name, "RContinuousRandomVariable");
        theDamageTheta15 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageTheta15->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageTheta15->setMean("0.12808792466436");
        theDamageTheta15->setCoefficientOfVariation("0.014976535492789");
        theAutoParametersList << theDamageTheta15;
    }



/*
    QString nonStructuralDamageEpsilonName = "BuildingDamageModelNonStructuralDamageEpsilon";
    theDamageEpsilon2 = theDomain->findChild<RContinuousRandomVariable *>(nonStructuralDamageEpsilonName);
    if (!theDamageEpsilon2) {
        theDomain->createObject(nonStructuralDamageEpsilonName, "RContinuousRandomVariable");
        theDamageEpsilon2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageEpsilon2->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageEpsilon2->setMean("0.0");
        theDamageEpsilon2->setStandardDeviation("0.036069031303692");
        theAutoParametersList << theDamageEpsilon2;
    }

    QString nonStructuralAccSensDamageEpsilonName = "BuildingDamageModelNonStructuralAccSensDamageEpsilon";
    theDamageEpsilon3 = theDomain->findChild<RContinuousRandomVariable *>(nonStructuralAccSensDamageEpsilonName);
    if (!theDamageEpsilon3) {
        theDomain->createObject(nonStructuralAccSensDamageEpsilonName, "RContinuousRandomVariable");
        theDamageEpsilon3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageEpsilon3->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageEpsilon3->setMean("0.0");
        theDamageEpsilon3->setStandardDeviation("0.024409954404172");
        theAutoParametersList << theDamageEpsilon3;
    }

    QString contentDamageEpsilonName = "BuildingDamageModelContentDamageEpsilon";
    theDamageEpsilon4 = theDomain->findChild<RContinuousRandomVariable *>(contentDamageEpsilonName);
    if (!theDamageEpsilon4) {
        theDomain->createObject(contentDamageEpsilonName, "RContinuousRandomVariable");
        theDamageEpsilon4 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theDamageEpsilon4->setDistributionType(QString("Normal (mean, stdv)"));
        theDamageEpsilon4->setMean("0.0");
        theDamageEpsilon4->setStandardDeviation("0.032479292799483");
        theAutoParametersList << theDamageEpsilon4;
    }
*/




    // ***************************************
    //    COST
    // ***************************************

    QString lossEpsilonName = "BuildingResponseModelDriftEpsilon";
    theLossEpsilon = theDomain->findChild<RContinuousRandomVariable *>(lossEpsilonName);
    if (!theLossEpsilon) {
        theDomain->createObject(lossEpsilonName, "RContinuousRandomVariable");
        theLossEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theLossEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
        theLossEpsilon->setMean("1.0");
        theLossEpsilon->setStandardDeviation("0.1");
        theAutoParametersList << theLossEpsilon;
    }


}

RMojtabasCombinedBuildingModel::~RMojtabasCombinedBuildingModel()
{

}

QObject * RMojtabasCombinedBuildingModel::getBIM() const
{
    return theBIM;
}

void RMojtabasCombinedBuildingModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


QObject * RMojtabasCombinedBuildingModel::getSa() const
{
    return theSa;
}

void RMojtabasCombinedBuildingModel::setSa(QObject *value)
{
    theSa = qobject_cast<RParameter *>(value);
}










int RMojtabasCombinedBuildingModel::evaluateModel(RGradientType theGradientType)
{

    // **************************************
    //    RESPONSE
    // **************************************

    // Use concrete frame values as default
    double meanResponseTheta1 = 2.22297339995124;    double covResponseTheta1 = 0.0243804860764949;
    double meanResponseTheta2 = 0.718510988499464;   double covResponseTheta2 = 0.0268934158637644;
    double meanResponseTheta3 = 1.35530466535807;    double covResponseTheta3 = 0.0715094797234139;
    double meanResponseTheta4 = 0.0337264910978365;  double covResponseTheta4 = 0.124478224239688;
    double meanResponseTheta5 = 2.75993850185935;    double covResponseTheta5 = 0.0374974925818043;
    double meanResponseTheta6 = 0.387338160854746;   double covResponseTheta6 = 0.0952607733361526;
    double meanResponseTheta7 = 8.71967516479302;    double covResponseTheta7 = 0.169471078047643;
    double meanResponseTheta8 = 1.53681896491041;    double covResponseTheta8 = 0.107580106792038;

    if (theBIM->getConstructionType() == QString("Concrete frame")) {
        // Do nothing; keep default values
    }
    if (theBIM->getConstructionType() == QString("Concrete shear wall")) {
        meanResponseTheta1 = 2.22865272680911;    covResponseTheta1 = 0.0899593468412107;
        meanResponseTheta2 = 0.63307331740037;    covResponseTheta2 = 0.11291130874424;
        meanResponseTheta3 = 1.08587183210043;    covResponseTheta3 = 0.0022772494391199;
        meanResponseTheta4 = 0.0196409535011445;  covResponseTheta4 = 0.00545369185829817;
        meanResponseTheta5 = 2.75993850185935;    covResponseTheta5 = 0.0374974925818043;
        meanResponseTheta6 = 0.387338160854746;   covResponseTheta6 = 0.0952607733361526;
        meanResponseTheta7 = 8.71967516479302;    covResponseTheta7 = 0.169471078047643;
        meanResponseTheta8 = 1.53681896491041;    covResponseTheta8 = 0.107580106792038;
    }
    else if (theBIM->getConstructionType() == QString("Concrete frame with masonry infill")) {
        meanResponseTheta1 = 2.22865272680911;    covResponseTheta1 = 0.0899593468412107;
        meanResponseTheta2 = 0.63307331740037;    covResponseTheta2 = 0.11291130874424;
        meanResponseTheta3 = 1.08587183210043;    covResponseTheta3 = 0.0022772494391199;
        meanResponseTheta4 = 0.0196409535011445;  covResponseTheta4 = 0.00545369185829817;
        meanResponseTheta5 = 2.75993850185935;    covResponseTheta5 = 0.0374974925818043;
        meanResponseTheta6 = 0.387338160854746;   covResponseTheta6 = 0.0952607733361526;
        meanResponseTheta7 = 7.63456446565424;    covResponseTheta7 = 0.317914673990779;
        meanResponseTheta8 = 0.608894392451661;   covResponseTheta8 = 0.528206256875503;
    }
    else if (theBIM->getConstructionType() == QString("Steel frame")) {
        meanResponseTheta1 = 2.25587853663162;    covResponseTheta1 = 0.0351262921029214;
        meanResponseTheta2 = 0.793644012324843;   covResponseTheta2 = 0.0331141662321456;
        meanResponseTheta3 = 1.52985207256129;    covResponseTheta3 = 0.10217462316747;
        meanResponseTheta4 = 0.0234317276101293;  covResponseTheta4 = 0.225092682240912;
        meanResponseTheta5 = 2.78965124797442;    covResponseTheta5 = 0.0482815468382483;
        meanResponseTheta6 = 0.369769659457065;   covResponseTheta6 = 0.120806795064979;
        meanResponseTheta7 = 8.71967516479302;    covResponseTheta7 = 0.169471078047643;
        meanResponseTheta8 = 1.53681896491041;    covResponseTheta8 = 0.107580106792038;
    }
    else if (theBIM->getConstructionType() == QString("Braced steel frame")) {
        meanResponseTheta1 = 2.48206675948044;    covResponseTheta1 = 0.0280336682741502;
        meanResponseTheta2 = 0.794286774765151;   covResponseTheta2 = 0.0290541119046308;
        meanResponseTheta3 = 1.10501207504222;    covResponseTheta3 = 0.0139172430703158;
        meanResponseTheta4 = 0.0147620716200757;  covResponseTheta4 = 0.0351516836219855;
        meanResponseTheta5 = 2.78965124797442;    covResponseTheta5 = 0.0482815468382483;
        meanResponseTheta6 = 0.369769659457065;   covResponseTheta6 = 0.120806795064979;
        meanResponseTheta7 = 8.60036382515231;    covResponseTheta7 = 0.274701830467166;
        meanResponseTheta8 = 1.29789267224511;    covResponseTheta8 = 0.20383859222897;
    }
    else if (theBIM->getConstructionType() == QString("Light steel frame")) {
        meanResponseTheta1 = 2.48206675948044;    covResponseTheta1 = 0.0280336682741502;
        meanResponseTheta2 = 0.794286774765151;   covResponseTheta2 = 0.0290541119046308;
        meanResponseTheta3 = 1.10501207504222;    covResponseTheta3 = 0.0139172430703158;
        meanResponseTheta4 = 0.0147620716200757;  covResponseTheta4 = 0.0351516836219855;
        meanResponseTheta5 = 2.78965124797442;    covResponseTheta5 = 0.0482815468382483;
        meanResponseTheta6 = 0.369769659457065;   covResponseTheta6 = 0.120806795064979;
        meanResponseTheta7 = 8.60036382515231;    covResponseTheta7 = 0.274701830467166;
        meanResponseTheta8 = 1.29789267224511;    covResponseTheta8 = 0.20383859222897;
    }
    else if (theBIM->getConstructionType() == QString("Steel frame with concrete shear wall")) {
        meanResponseTheta1 = 2.47013392673656;    covResponseTheta1 = 0.0231743422232301;
        meanResponseTheta2 = 0.709414586804169;   covResponseTheta2 = 0.0267620483233741;
        meanResponseTheta3 = 1.32815562635643;    covResponseTheta3 = 0.0115790057571682;
        meanResponseTheta4 = 0.0147620716200757;  covResponseTheta4 = 0.0351516836219851;
        meanResponseTheta5 = 2.78965124797442;    covResponseTheta5 = 0.0482815468382483;
        meanResponseTheta6 = 0.369769659457065;   covResponseTheta6 = 0.120806795064979;
        meanResponseTheta7 = 8.60036382515231;    covResponseTheta7 = 0.274701830467166;
        meanResponseTheta8 = 1.29789267224511;    covResponseTheta8 = 0.20383859222897;
    }
    else if (theBIM->getConstructionType() == QString("Steel frame  with masonry infill")) {
        meanResponseTheta1 = 2.47013392673656;    covResponseTheta1 = 0.0231743422232301;
        meanResponseTheta2 = 0.709414586804169;   covResponseTheta2 = 0.0267620483233741;
        meanResponseTheta3 = 1.10501207504222;    covResponseTheta3 = 0.0139172430703158;
        meanResponseTheta4 = 0.0147620716200757;  covResponseTheta4 = 0.0351516836219855;
        meanResponseTheta5 = 2.78965124797442;    covResponseTheta5 = 0.0482815468382483;
        meanResponseTheta6 = 0.369769659457065;   covResponseTheta6 = 0.120806795064979;
        meanResponseTheta7 = 7.63456446565424;    covResponseTheta7 = 0.317914673990779;
        meanResponseTheta8 = 0.608894392451661;   covResponseTheta8 = 0.528206256875503;
    }
    else if (theBIM->getConstructionType() == QString("Precast concrete")) {
        meanResponseTheta1 = 2.22865272680911;    covResponseTheta1 = 0.0899593468412107;
        meanResponseTheta2 = 0.63307331740037;    covResponseTheta2 = 0.11291130874424;
        meanResponseTheta3 = 1.08587183210043;    covResponseTheta3 = 0.0022772494391199;
        meanResponseTheta4 = 0.0196409535011445;  covResponseTheta4 = 0.00545369185829817;
        meanResponseTheta5 = 2.75993850185935;    covResponseTheta5 = 0.0374974925818043;
        meanResponseTheta6 = 0.387338160854746;   covResponseTheta6 = 0.0952607733361526;
        meanResponseTheta7 = 8.60036382515231;    covResponseTheta7 = 0.274701830467166;
        meanResponseTheta8 = 1.29789267224511;    covResponseTheta8 = 0.20383859222897;
    }
    else if (theBIM->getConstructionType() == QString("Reinforced masonry")) {
        meanResponseTheta1 = 2.22865272680911;    covResponseTheta1 = 0.0899593468412107;
        meanResponseTheta2 = 0.63307331740037;    covResponseTheta2 = 0.11291130874424;
        meanResponseTheta3 = 0.796505685393632;   covResponseTheta3 = 0.00271287656210005;
        meanResponseTheta4 = 0.0196851672124356;  covResponseTheta4 = 0.00475492638444581;
        meanResponseTheta5 = 2.75993850185935;    covResponseTheta5 = 0.0374974925818043;
        meanResponseTheta6 = 0.387338160854746;   covResponseTheta6 = 0.0952607733361526;
        meanResponseTheta7 = 8.71967516479302;    covResponseTheta7 = 0.169471078047643;
        meanResponseTheta8 = 1.53681896491041;    covResponseTheta8 = 0.107580106792038;
    }
    else if (theBIM->getConstructionType() == QString("Unreinforced masonry")) {
        meanResponseTheta1 = 1.67865192997027;    covResponseTheta1 = 0.0188101325110738;
        meanResponseTheta2 = 0.417921147711188;   covResponseTheta2 = 0.0391874922447207;
        meanResponseTheta3 = 2.20569553863626;    covResponseTheta3 = 0.0388611097538929;
        meanResponseTheta4 = 0.0267093821664968;  covResponseTheta4 = 0.425106847681514;
        meanResponseTheta5 = 2.28819973855749;    covResponseTheta5 = 0.0911049523260439;
        meanResponseTheta6 = 0.470365575016951;   covResponseTheta6 = 0.229873918323201;
        meanResponseTheta7 = 7.63456446565424;    covResponseTheta7 = 0.317914673990779;
        meanResponseTheta8 = 0.608894392451661;   covResponseTheta8 = 0.528206256875503;
    }
    else if (theBIM->getConstructionType() == QString("Light wood frame")) {
        meanResponseTheta1 = 1.76561947568034;    covResponseTheta1 = 0.0961229837253151;
        meanResponseTheta2 = 0.462344358810064;   covResponseTheta2 = 0.162153117571403;
        meanResponseTheta3 = 1.13935020598235;    covResponseTheta3 = 0.02267186809777;
        meanResponseTheta4 = 0.0114859963307501;  covResponseTheta4 = 0.170689654272867;
        meanResponseTheta5 = 2.5362785203959;     covResponseTheta5 = 0.0856320184007843;
        meanResponseTheta6 = 0.274628629225648;   covResponseTheta6 = 0.349344433361288;
        meanResponseTheta7 = 4.60710061521646;    covResponseTheta7 = 0.19761908784006;
        meanResponseTheta8 = 1.37226580049028;    covResponseTheta8 = 0.148642746607334;
    }
    else if (theBIM->getConstructionType() == QString("Large wood frame")) {
        meanResponseTheta1 = 1.76561947568034;    covResponseTheta1 = 0.096122983725315;
        meanResponseTheta2 = 0.462344358810064;   covResponseTheta2 = 0.162153117571403;
        meanResponseTheta3 = 1.13935020598235;    covResponseTheta3 = 0.02267186809777;
        meanResponseTheta4 = 0.0114859963307501;  covResponseTheta4 = 0.170689654272867;
        meanResponseTheta5 = 2.5362785203959;     covResponseTheta5 = 0.0856320184007843;
        meanResponseTheta6 = 0.274628629225648;   covResponseTheta6 = 0.349344433361287;
        meanResponseTheta7 = 8.71967516479302;    covResponseTheta7 = 0.169471078047643;
        meanResponseTheta8 = 1.53681896491041;    covResponseTheta8 = 0.107580106792038;
    }
    else {
        qCritical() << "Something is wrong with the building type in" << this->objectName();
    }




    // Looking up the model parameters, remembering that the first eight must be transformed
    // y = (x-mu)/sigma   -->   x = y * sigma + mu = y * mu * cov + mu =  (y * cov + 1) * mu
    double responseTheta1 = (theResponseTheta1->getCurrentValue() * covResponseTheta1 + 1.0) * meanResponseTheta1;
    double responseTheta2 = (theResponseTheta2->getCurrentValue() * covResponseTheta2 + 1.0) * meanResponseTheta2;
    double responseTheta3 = (theResponseTheta3->getCurrentValue() * covResponseTheta3 + 1.0) * meanResponseTheta3;
    double responseTheta4 = (theResponseTheta4->getCurrentValue() * covResponseTheta4 + 1.0) * meanResponseTheta4;
    double responseTheta5 = (theResponseTheta5->getCurrentValue() * covResponseTheta5 + 1.0) * meanResponseTheta5;
    double responseTheta6 = (theResponseTheta6->getCurrentValue() * covResponseTheta6 + 1.0) * meanResponseTheta6;
    double responseTheta7 = (theResponseTheta7->getCurrentValue() * covResponseTheta7 + 1.0) * meanResponseTheta7;
    double responseTheta8 = (theResponseTheta8->getCurrentValue() * covResponseTheta8 + 1.0) * meanResponseTheta8;


    double responseTheta9 = theResponseTheta9->getCurrentValue();
    double responseTheta10 = theResponseTheta10->getCurrentValue();
    double responseTheta11 = theResponseTheta11->getCurrentValue();
    double responseTheta12 = theResponseTheta12->getCurrentValue();
    double responseTheta13 = theResponseTheta13->getCurrentValue();
    double responseTheta14 = theResponseTheta14->getCurrentValue();
    double responseTheta15 = theResponseTheta15->getCurrentValue();
    double responseTheta16 = theResponseTheta16->getCurrentValue();
    double responseTheta17 = theResponseTheta17->getCurrentValue();
    double responseTheta18 = theResponseTheta18->getCurrentValue();
    double responseTheta19 = theResponseTheta19->getCurrentValue();
    double responseTheta20 = theResponseTheta20->getCurrentValue();
    double responseTheta21 = theResponseTheta21->getCurrentValue();
    double sigma1 = theResponseSigma1->getCurrentValue();
    double sigma2 = theResponseSigma2->getCurrentValue();
    double epsilon1 = theResponseEpsilon1->getCurrentValue() * sigma1;
    double epsilon2 = theResponseEpsilon2->getCurrentValue() * sigma2;




    //Looking up the demand parameter
    qCritical() << "Here I think we forget to give the location and natural period to the intensity model...";
    double Sa = theSa->getCurrentValue();


    // Getting the information from the BIM
    RBIM *theBIM = qobject_cast<RBIM *>(theBIM);
    if (!theBIM) {
        qCritical() << "Error in model" << objectName() << ":" << theBIM->objectName() << "is not a building information model.";
        return -1;
    }

    RBIM::RCodeLevel codeLevel = theBIM->getCodeLevel();
    double alpha = codeLevel + 1.0;
    double H = theBIM->getBuildingHeight();


    // Evaluating the sub-models
    double T = exp(-responseTheta1) * pow(H, responseTheta2);
    double V = exp(-responseTheta3) * exp(-responseTheta4 * H) * (2.0 + (alpha - 2.0) * (alpha - 1.0)) / 8.0;
    double gg = 9.81; //Gravity
    double deltaY = (T / 2.0 / 3.14159265359)*(T / 2.0 / 3.14159265359) * V * gg / H;
    double Mu = exp(responseTheta5) * pow(H, -responseTheta6) * (10.0 + (alpha - 2.0) * (alpha - 1.0)) / 16.0;
    double deltaU = Mu * deltaY;
    double Kappa = exp(-responseTheta7 * Sa * pow(alpha, -responseTheta8));


    // Evaluating the drift model expression
    double logDrift = responseTheta9 * log(deltaY) + responseTheta10 * log(deltaU) - responseTheta11 * log(V) - responseTheta12 * log(Kappa) + responseTheta13 * log(Sa) + responseTheta14 * Sa - responseTheta15 + epsilon1;
    double peakDrift = exp(logDrift);
    thePeakDrift->setCurrentValue(peakDrift);


    // Evaluating the acceleration model expression
    double logAcc = -responseTheta16 * log(deltaY) + responseTheta17 * log(V) - responseTheta18 * log(Mu) + responseTheta19 * log(Kappa) + responseTheta20 * log(Sa) - responseTheta21 + epsilon2;
    double peakAcc = exp(logAcc);
    thePeakAcc->setCurrentValue(peakAcc);




    // ******************************
    //    DAMAGE
    // ******************************


    // Use concrete frame values as default
    double meanDamageTheta1 = 1.30535777188603;  double covDamageTheta1 = 0.006851725426429;
    double meanDamageTheta2 = 0.664820813681537; double covDamageTheta2 = 0.010398857098557;
    double meanDamageTheta3 = 0.286778342140848; double covDamageTheta3 = 0.014494019754025;
    double meanDamageTheta4 = 4.33744720988832;  double covDamageTheta4 = 0.008498405604416;
    double meanDamageTheta5 = 0.392129368320604; double covDamageTheta5 = 0.048097225079779;
    double meanDamageTheta6 = 0.246862945558682; double covDamageTheta6 = 0.060319964252776;
    double meanDamageTheta7 = 0.924172549839815; double covDamageTheta7 = 0.025872110052779;
    double meanDamageTheta8 = 0.616677026202032; double covDamageTheta8 = 0.020086111302224;
    double meanDamageTheta9 = 0.302756951133824; double covDamageTheta9 = 0.034283652657823;
    double meanEpsilon = 0; double stdvEpsilon = 0.018781277985919;

    if (theBIM->getConstructionType() == QString("Concrete frame")) {
        // Do nothing; keep default values
    }
    if (theBIM->getConstructionType() == QString("Concrete shear wall")) {
        meanDamageTheta1 = 1.21465614269761;  covDamageTheta1 = 0.006402151535424;
        meanDamageTheta2 = 0.626771402181758; covDamageTheta2 = 0.011341103547784;
        meanDamageTheta3 = 0.268108592798877; covDamageTheta3 = 0.01614219789087;
        meanDamageTheta4 = 4.02508724041101;  covDamageTheta4 = 0.007763739253467;
        meanDamageTheta5 = 0.113858697009988; covDamageTheta5 = 0.172299182670169;
        meanDamageTheta6 = 0.289690474445929; covDamageTheta6 = 0.032907849837814;
        meanDamageTheta7 = 0.838807543600913; covDamageTheta7 = 0.02344426157169;
        meanDamageTheta8 = 0.388327286045199; covDamageTheta8 = 0.03917764151005;
        meanDamageTheta9 = 0.00001;           covDamageTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.014565878894457;
    }
    else if (theBIM->getConstructionType() == QString("Concrete frame with masonry infill")) {
        meanDamageTheta1 = 1.35937261048995;  covDamageTheta1 = 0.009766728201417;
        meanDamageTheta2 = 0.691532934649142; covDamageTheta2 = 0.015275094981982;
        meanDamageTheta3 = 0.369626826372066; covDamageTheta3 = 0.031767159618159;
        meanDamageTheta4 = 5.05032390445079;  covDamageTheta4 = 0.011188088842238;
        meanDamageTheta5 = 0.650471648516973; covDamageTheta5 = 0.024731772645831;
        meanDamageTheta6 = 0.293351868679244; covDamageTheta6 = 0.08376336469095;
        meanDamageTheta7 = 0.471053380089266; covDamageTheta7 = 0.054258266050058;
        meanDamageTheta8 = 0.53900054321154;  covDamageTheta8 = 0.040958726843212;
        meanDamageTheta9 = 0.00001;           covDamageTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.020402736295115;
    }
    else if (theBIM->getConstructionType() == QString("Steel frame")) {
        meanDamageTheta1 = 1.32187575016808;  covDamageTheta1 = 0.006293266110094;
        meanDamageTheta2 = 0.626424186503563; covDamageTheta2 = 0.010831473939025;
        meanDamageTheta3 = 0.275353821300956; covDamageTheta3 = 0.014305656910222;
        meanDamageTheta4 = 4.34777181647321;  covDamageTheta4 = 0.007894744131077;
        meanDamageTheta5 = 0.267733203347781; covDamageTheta5 = 0.045734929550636;
        meanDamageTheta6 = 0.283722996846665; covDamageTheta6 = 0.037877084692777;
        meanDamageTheta7 = 0.877696156286803; covDamageTheta7 = 0.024403638334624;
        meanDamageTheta8 = 0.00001;           covDamageTheta8 = 0.001;
        meanDamageTheta9 = 0.274924901093514; covDamageTheta9 = 0.037898568566978;
        meanEpsilon = 0; stdvEpsilon = 0.01702440236915;
    }
    else if (theBIM->getConstructionType() == QString("Braced steel frame")) {
        meanDamageTheta1 = 1.28175966436694;  covDamageTheta1 = 0.007224258672286;
        meanDamageTheta2 = 0.646696697818393; covDamageTheta2 = 0.01245972436738;
        meanDamageTheta3 = 0.287756325416325; covDamageTheta3 = 0.015739438691391;
        meanDamageTheta4 = 4.15856391401591;  covDamageTheta4 = 0.009000344605634;
        meanDamageTheta5 = 0.170846423400316; covDamageTheta5 = 0.121901726388769;
        meanDamageTheta6 = 0.315525353511683; covDamageTheta6 = 0.033741851519453;
        meanDamageTheta7 = 1.10207271404515;  covDamageTheta7 = 0.017181991540148;
        meanDamageTheta8 = 0.00001;           covDamageTheta8 = 0.001;
        meanDamageTheta9 = 0.224400903886254; covDamageTheta9 = 0.057888309643459;
        meanEpsilon = 0; stdvEpsilon = 0.018252553662291;
    }
    else if (theBIM->getConstructionType() == QString("Light steel frame")) {
        meanDamageTheta1 = 1.238226897465;    covDamageTheta1 = 0.010278083637242;
        meanDamageTheta2 = 0.00001;           covDamageTheta2 = 0.001;
        meanDamageTheta3 = 0.277772796069453; covDamageTheta3 = 0.022440394343561;
        meanDamageTheta4 = 5.48917795737416;  covDamageTheta4 = 0.010745397198204;
        meanDamageTheta5 = 0.210309534269694; covDamageTheta5 = 0.117972907024925;
        meanDamageTheta6 = 0.329653135573926; covDamageTheta6 = 0.05001245293764;
        meanDamageTheta7 = 0.585308059114181; covDamageTheta7 = 0.030939707531868;
        meanDamageTheta8 = 0.00001;           covDamageTheta8 = 0.001;
        meanDamageTheta9 = 0.346517454949579; covDamageTheta9 = 0.039669885289746;
        meanEpsilon = 0; stdvEpsilon = 0.019653455184743;
    }
    else if (theBIM->getConstructionType() == QString("Steel frame with concrete shear wall")) {
        meanDamageTheta1 = 1.25124808132616;  covDamageTheta1 = 0.007747213985819;
        meanDamageTheta2 = 0.665015152837385; covDamageTheta2 = 0.012231122581524;
        meanDamageTheta3 = 0.291212721782239; covDamageTheta3 = 0.016401621413013;
        meanDamageTheta4 = 4.19951180032007;  covDamageTheta4 = 0.009593830896499;
        meanDamageTheta5 = 0.407003539055466; covDamageTheta5 = 0.051550945079048;
        meanDamageTheta6 = 0.30755286135054;  covDamageTheta6 = 0.042074426847175;
        meanDamageTheta7 = 0.997086315608624; covDamageTheta7 = 0.021117256856772;
        meanDamageTheta8 = 0.00001;           covDamageTheta8 = 0.001;
        meanDamageTheta9 = 0.229554284257269; covDamageTheta9 = 0.055459081819921;
        meanEpsilon = 0; stdvEpsilon = 0.02003279046568;
    }
    else if (theBIM->getConstructionType() == QString("Steel frame  with masonry infill")) {
        meanDamageTheta1 = 1.30486453250648;  covDamageTheta1 = 0.00917330025994;
        meanDamageTheta2 = 0.613542033821746; covDamageTheta2 = 0.016060321130632;
        meanDamageTheta3 = 0.375090675773637; covDamageTheta3 = 0.030613277476669;
        meanDamageTheta4 = 4.87527241783118;  covDamageTheta4 = 0.01020491042874;
        meanDamageTheta5 = 0.587020986003727; covDamageTheta5 = 0.025471756822839;
        meanDamageTheta6 = 0.26636308516701;  covDamageTheta6 = 0.085444838942947;
        meanDamageTheta7 = 0.43535766409691;  covDamageTheta7 = 0.052680895764799;
        meanDamageTheta8 = 0.00001;           covDamageTheta8 = 0.001;
        meanDamageTheta9 = 0.00001;           covDamageTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.017549305835961;
    }
    else if (theBIM->getConstructionType() == QString("Precast concrete")) {
        meanDamageTheta1 = 1.23459116707839;  covDamageTheta1 = 0.005903044342454;
        meanDamageTheta2 = 0.651442723437484; covDamageTheta2 = 0.009470832278259;
        meanDamageTheta3 = 0.253848520921968; covDamageTheta3 = 0.01489125110138;
        meanDamageTheta4 = 4.22903902777438;  covDamageTheta4 = 0.007317698706426;
        meanDamageTheta5 = 0.375453044529677; covDamageTheta5 = 0.042450227727026;
        meanDamageTheta6 = 0.338533472594216; covDamageTheta6 = 0.047281961998664;
        meanDamageTheta7 = 0.535140195336066; covDamageTheta7 = 0.019920211330669;
        meanDamageTheta8 = 0.371933817633649; covDamageTheta8 = 0.038282494212727;
        meanDamageTheta9 = 0.203089715598823; covDamageTheta9 = 0.05292420248958;
        meanEpsilon = 0; stdvEpsilon = 0.016011211020035;
    }
    else if (theBIM->getConstructionType() == QString("Reinforced masonry")) {
        meanDamageTheta1 = 1.18371377806936;  covDamageTheta1 = 0.007765576834722;
        meanDamageTheta2 = 0.6231401567677;   covDamageTheta2 = 0.014993509845317;
        meanDamageTheta3 = 0.268503431683156; covDamageTheta3 = 0.019329208610842;
        meanDamageTheta4 = 4.10957395391823;  covDamageTheta4 = 0.009282595484385;
        meanDamageTheta5 = 0.101850323590683; covDamageTheta5 = 0.216197087743516;
        meanDamageTheta6 = 0.425428224039622; covDamageTheta6 = 0.038738323604197;
        meanDamageTheta7 = 0.781350695240031; covDamageTheta7 = 0.027583662324846;
        meanDamageTheta8 = 0.00001;           covDamageTheta8 = 0.001;
        meanDamageTheta9 = 0.00001;           covDamageTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.017299183891961;
    }
    else if (theBIM->getConstructionType() == QString("Unreinforced masonry")) {
        meanDamageTheta1 = 1.36258655513574;  covDamageTheta1 = 0.009806759991199;
        meanDamageTheta2 = 0.618816027195958; covDamageTheta2 = 0.027147851751869;
        meanDamageTheta3 = 0.300288271512769; covDamageTheta3 = 0.045566513034644;
        meanDamageTheta4 = 5.29673222299837;  covDamageTheta4 = 0.012756792701196;
        meanDamageTheta5 = 0.713863081370799; covDamageTheta5 = 0.029992962475781;
        meanDamageTheta6 = 0.582610552545055; covDamageTheta6 = 0.03420537038336;
        meanDamageTheta7 = 0.572662916589229; covDamageTheta7 = 0.042622084396919;
        meanDamageTheta8 = 0.00001;           covDamageTheta8 = 0.001;
        meanDamageTheta9 = 0.00001;           covDamageTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.017672913936856;
    }
    else if (theBIM->getConstructionType() == QString("Light wood frame")) {
        meanDamageTheta1 = 1.154227919813;    covDamageTheta1 = 0.005799987905193;
        meanDamageTheta2 = 0.00001;           covDamageTheta2 = 0.001;
        meanDamageTheta3 = 0.170402360927627; covDamageTheta3 = 0.021942867014654;
        meanDamageTheta4 = 4.32829658800862;  covDamageTheta4 = 0.006622432802063;
        meanDamageTheta5 = 0.165926773126374; covDamageTheta5 = 0.054946848921871;
        meanDamageTheta6 = 0.380492787718244; covDamageTheta6 = 0.031645015489524;
        meanDamageTheta7 = 0.319445578338458; covDamageTheta7 = 0.037082082344196;
        meanDamageTheta8 = 0.00001;           covDamageTheta8 = 0.001;
        meanDamageTheta9 = 0.00001;           covDamageTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.007929896467867;
    }
    else if (theBIM->getConstructionType() == QString("Large wood frame")) {
        meanDamageTheta1 = 1.20647329309667;   covDamageTheta1 = 0.007208878770065;
        meanDamageTheta2 = 0.00001;            covDamageTheta2 = 0.001;
        meanDamageTheta3 = -0.178281288390707; covDamageTheta3 = 0.028090854114829;
        meanDamageTheta4 = 4.52424806438601;   covDamageTheta4 = 0.008392336274958;
        meanDamageTheta5 = 0.33270064404502;   covDamageTheta5 = 0.029972760968811;
        meanDamageTheta6 = 0.51043151953732;   covDamageTheta6 = 0.029416591115207;
        meanDamageTheta7 = 0.561856647995703;  covDamageTheta7 = 0.022002546442442;
        meanDamageTheta8 = 0.00001;            covDamageTheta8 = 0.001;
        meanDamageTheta9 = 0.00001;            covDamageTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.013623306613564;
    }
    else {
        qCritical() << "Something is wrong with the building type in" << this->objectName();
    }



    // Looking up the model parameters, remembering that the first nine, and one epsilon, must be transformed
    // y = (x-mu)/sigma   -->   x = y * sigma + mu = y * mu * cov + mu =  (y * cov + 1) * mu
    double damageTheta1 = (theDamageTheta1->getCurrentValue() * covDamageTheta1 + 1.0) * meanDamageTheta1;
    double damageTheta2 = (theDamageTheta2->getCurrentValue() * covDamageTheta2 + 1.0) * meanDamageTheta2;
    double damageTheta3 = (theDamageTheta3->getCurrentValue() * covDamageTheta3 + 1.0) * meanDamageTheta3;
    double damageTheta4 = (theDamageTheta4->getCurrentValue() * covDamageTheta4 + 1.0) * meanDamageTheta4;
    double damageTheta5 = (theDamageTheta5->getCurrentValue() * covDamageTheta5 + 1.0) * meanDamageTheta5;
    double damageTheta6 = (theDamageTheta6->getCurrentValue() * covDamageTheta6 + 1.0) * meanDamageTheta6;
    double damageTheta7 = (theDamageTheta7->getCurrentValue() * covDamageTheta7 + 1.0) * meanDamageTheta7;
    double damageTheta8 = (theDamageTheta8->getCurrentValue() * covDamageTheta8 + 1.0) * meanDamageTheta8;
    double damageTheta9 = (theDamageTheta9->getCurrentValue() * covDamageTheta9 + 1.0) * meanDamageTheta9;

    //double epsilon1 = theDamageEpsilon1->getCurrentValue() * stdvEpsilon + meanEpsilon;


    double damageTheta10 = theDamageTheta10->getCurrentValue();
    double damageTheta11 = theDamageTheta11->getCurrentValue();
    double damageTheta12 = theDamageTheta12->getCurrentValue();
    double damageTheta13 = theDamageTheta13->getCurrentValue();
    double damageTheta14 = theDamageTheta14->getCurrentValue();
    double damageTheta15 = theDamageTheta15->getCurrentValue();

    // Looking up the model error parameters
    //double epsilon2 = theDamageEpsilon2->getCurrentValue();
    //double epsilon3 = theDamageEpsilon3->getCurrentValue();
    //double epsilon4 = theDamageEpsilon4->getCurrentValue();

    // Looking up the drift and acceleration
    double drift = peakDrift;
    double acceleration = peakAcc;
    //    double drift = thePeakDriftRatio->getCurrentValue();
    //    double acceleration = thePeakAcceleration->getCurrentValue();


    // Getting the information from the BIM
/*    RBIM *theBIM = qobject_cast<RBIM *>(theBuildingInformationModel);
    if (!theBIM) {
        qCritical() << "Error in model" << objectName() << ":" << theBuildingInformationModel->objectName() << "is not a building information model.";
        return -1;
    }*/

    //RBIM::RCodeLevel codeLevel = theBIM->getCodeLevel();
    //double alpha = codeLevel + 1.0;
    //double H = theBIM->getHeight();


    // Look up for irregularities
    double Ivi = 0;
    double Ipi = 0;
    double Iss = 0;
    double Isc = 0;
    double Ip = 0;
    if (theBIM->getVerticalIrregularity()) {
        Ivi = 1.0;
    }
    if (theBIM->getVerticalIrregularity()) {
        Ipi = 1.0;
    }
    if (theBIM->getSoftStorey()) {
        Iss = 1.0;
    }
    if (theBIM->getShortColumn()) {
        Isc = 1.0;
    }
    if (theBIM->getPounding()) {
        Ip = 1.0;
    }


    // Evaluating the model expression of the structural damage model
    RNormalDistribution theNormalDistribution(theDomain);
    double damageRatioS = theNormalDistribution.getNormalCDF(damageTheta1 * log(drift) + damageTheta2 * log(H) - damageTheta3 * alpha + damageTheta4, 1.0) * exp(damageTheta5 * Ivi + damageTheta6 * Ipi + damageTheta7 * Iss + damageTheta8 * Isc + damageTheta9 * Ip);
    //double damageRatioS = theProbabilityDistributions->getNormalCDF(damageTheta1 * log(drift) + damageTheta2 * log(H) - damageTheta3 * alpha + damageTheta4, 1.0) * exp(damageTheta5 * Ivi + damageTheta6 * Ipi + damageTheta7 * Iss + damageTheta8 * Isc + damageTheta9 * Ip);
    theStructuralDamageRatio->setCurrentValue(damageRatioS);


    // Evaluating the model expression of the non-structural drift-sensitive damage model
    double damageRatioND = theNormalDistribution.getNormalCDF(damageTheta10 * log(drift) + damageTheta11, 1.0);
    //double damageRatioND = theProbabilityDistributions->getNormalCDF(damageTheta10 * log(drift) + damageTheta11, 1.0);
    theNonstructuralDriftDamageRatio->setCurrentValue(damageRatioND);


    // Evaluating the model expression of the non-structural acecleration-sensitive damage model
    double damageRatioNA = theNormalDistribution.getNormalCDF(damageTheta12 * log(acceleration) - damageTheta13 * alpha, 1.0);
    //double damageRatioNA = theProbabilityDistributions->getNormalCDF(damageTheta12 * log(acceleration) - damageTheta13 * alpha, 1.0);
    theNonstructuralAccelerationDamageRatio->setCurrentValue(damageRatioNA);


    // Evaluating the model expression of the content damage model
    double damageRatioC = theNormalDistribution.getNormalCDF(damageTheta14 * log(acceleration) - damageTheta15 * alpha, 1.0);
    //double damageRatioC = theProbabilityDistributions->getNormalCDF(damageTheta14 * log(acceleration) - damageTheta15 * alpha, 1.0);
    theContentDamageRatio->setCurrentValue(damageRatioC);



    // *********************************
    //    COST
    // *********************************

    double DFs = damageRatioS;
    double DFnd = damageRatioND;
    double DFna = damageRatioNA;
    double DFc = damageRatioC;

    double epsilon = theLossEpsilon->getCurrentValue();
    double totalArea = theBIM->getTotalArea();

    double theStructuralUnitAreaCost = 266;
    double theNonstructuralDriftUnitAreaCost = 833;
    double theNonstructuralAccelerationUnitAreaCost = 844;
    double theContentUnitAreaCost = 0;


    double repairCost = (DFs * theStructuralUnitAreaCost + DFnd * theNonstructuralDriftUnitAreaCost + DFna * theNonstructuralAccelerationUnitAreaCost + DFc * theContentUnitAreaCost) * totalArea * epsilon;

/*    double currentTime = theCurrentTime->getCurrentValue();
    double interestRate = 0.03;
    double futureDiscountFactor = 1.0;
    if (currentTime > theInitialTime) {
        //futureDiscountFactor = pow(1 + interestRate, -(currentTime - theInitialTime) / 365.0);
        futureDiscountFactor = exp(-interestRate * (currentTime - theInitialTime) / 365.0);
    }

    repairCost *= futureDiscountFactor;*/

    theRepairCost->setCurrentValue(repairCost);


    return 0;
}

int RMojtabasCombinedBuildingModel::setResponsesThatFlowUpstream()
{
    // Getting the information from the BIM
    RBIM *theBIM = qobject_cast<RBIM *>(theBIM);
    if (!theBIM) {
        qCritical() << "Error in model" << objectName() << ":" << theBIM->objectName() << "is not a building information model.";
        return -1;
    }

    RBIM::RCodeLevel codeLevel = theBIM->getCodeLevel();
    double responseTheta1 = theResponseTheta1->getCurrentValue();
    double responseTheta2 = theResponseTheta2->getCurrentValue();
    double H = theBIM->getBuildingHeight();
    double T = exp(-responseTheta1) * pow(H, responseTheta2);

    // Setting the value of the constant parameter (thePeriodInformation) to the calculated T. This constant will serve as input to upstream models such as the attenuation model
    /*thePeriodInformation->setCurrentValue(T);

    QList<RParameter *> parameterList;
    parameterList << theResponseTheta1 << theResponseTheta2;
    RConstant *constObject = qobject_cast<RConstant *>(thePeriodInformation);
    if (!constObject) {
        qCritical() << "Error in" << objectName() << ": The parameter" << thePeriodInformation->objectName() << "that carries a rexponse to an upstream model must be a Cosntant object";
        return -1;
    }
    constObject->setUpstreamParameterList(parameterList);*/

    return 0;
}
