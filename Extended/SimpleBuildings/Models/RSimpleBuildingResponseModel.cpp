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
*   - Rodrigo Costa                                                  *
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RSimpleBuildingResponseModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RConstant.h"
#include "RNormalDistribution.h"
#include "RIntensityModel.h"
#include "RContinuousRandomVariable.h"
#include "RIntensityModel.h"
#include "RBIM.h"
#include "RStatistics.h"

#include <math.h>

RSimpleBuildingResponseModel::RSimpleBuildingResponseModel(QObject *parent, QString name)
    : RModel(parent, name)
{

    theBuildingType = QString("C2M");
    theNumberOfStoreys = 0;
    theBIM = nullptr;

    theDomain->createObject(objectName() + "PeakDriftResponse", "RGenericResponse");
    thePeakDriftResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePeakDriftResponse->setModel(this);
    thePeakDriftResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "PeakAccelerationResponse", "RGenericResponse");
    thePeakAccelerationResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePeakAccelerationResponse->setModel(this);
    thePeakAccelerationResponse->setCurrentValue(0.0);

    // Residual drift ratio
    theDomain->createObject(objectName() + "ResidualDriftRatioResponse", "RGenericResponse");
    theResidualDriftRatioResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResidualDriftRatioResponse->setModel(this);
    theResidualDriftRatioResponse->setCurrentValue(0.0);

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

    theStatsMethod = theDomain->getStatsMethod();

}


RSimpleBuildingResponseModel::~RSimpleBuildingResponseModel()
{

}


QObject * RSimpleBuildingResponseModel::getBIM() const
{
    return theBIM;
}


void RSimpleBuildingResponseModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);

    if(theBIM)
    {
        theLocation = qobject_cast<RParameter*>(theBIM->getLocation());
        theBuildingType = theBIM->getBuildingType();
        theNumberOfStoreys = theBIM->getNumberOfStoreys();
        theCodeDesignLevel = theBIM->getCodeLevel();

        // Earthquake
        RResponse *theResponseObject = nullptr;
        RIntensityModel *theIntensityModel = nullptr;

        for(auto&& theSa: theIntensityModelList)
        {
            theResponseObject = qobject_cast<RResponse *>(theSa);

            // Get the pointer to the intensity model
            theIntensityModel = qobject_cast<RIntensityModel *>(theResponseObject->getModel());

            // Tell that intensity model what we want
            theIntensityModel->setLocation(theLocation);
            theIntensityModel->setPeriod(theNumberOfStoreys / 10.0);
            theIntensityModel->setIntensityMeasure(QString("Sa"));
        }
    }
    else {
        qCritical()<<"Could not set the BIM in "<<__FUNCTION__;
    }

}


QString RSimpleBuildingResponseModel::getIntensityList()
{
    return parameterListToString(theIntensityModelList);
}


void RSimpleBuildingResponseModel::setIntensityList(QString value)
{
    theIntensityModelList = stringToParameterList(value);

    // Make sure the upstream models evaluate
    for(auto&& theSa: theIntensityModelList)
        theAllParametersList<<theSa;
}


int RSimpleBuildingResponseModel::evaluateModel(RGradientType theGradientType)
{
    thePeakDriftResponse->setCurrentValue(0.0);
    theResidualDriftRatioResponse->setCurrentValue(0.0);
    thePeakAccelerationResponse->setCurrentValue(0.0);

    if(theBIM == nullptr)
    {
        qDebug()<<"Error, the model "<<this->objectName()<<" needs a BIM";
        return 0;
    }

    // Earthquake
    RResponse *theResponseObject = nullptr;

    double Sa = 0.0;

    // The really really rare occurrence that more than one earthquake source gets activated at the same time
    for(auto&& theSa: theIntensityModelList)
    {
        theResponseObject = qobject_cast<RResponse *>(theSa);

        auto theIntensityModel = qobject_cast<RIntensityModel *>(theResponseObject->getModel());

        // Check to make sure the model is ON
        auto isOn = theIntensityModel->isOn();

        if(isOn)
        {
            // qDebug()<<"For object"<<theSa->objectName()<<" the Sa is "<<theResponseObject->getCurrentValue();
            Sa += theResponseObject->getCurrentValue();
        }
    }

    // End this quickly if there was no ground acceleration
    if (Sa < 0.0001){
        return 0;
    }

    // Use concrete frame values as default
    double meanResponseTheta1 = 2.22297339995124;    double covResponseTheta1 = 0.0243804860764949;
    double meanResponseTheta2 = 0.718510988499464;   double covResponseTheta2 = 0.0268934158637644;
    double meanResponseTheta3 = 1.35530466535807;    double covResponseTheta3 = 0.0715094797234139;
    double meanResponseTheta4 = 0.0337264910978365;  double covResponseTheta4 = 0.124478224239688;
    double meanResponseTheta5 = 2.75993850185935;    double covResponseTheta5 = 0.0374974925818043;
    double meanResponseTheta6 = 0.387338160854746;   double covResponseTheta6 = 0.0952607733361526;
    double meanResponseTheta7 = 8.71967516479302;    double covResponseTheta7 = 0.169471078047643;
    double meanResponseTheta8 = 1.53681896491041;    double covResponseTheta8 = 0.107580106792038;

    if (theBuildingType.contains(QString("C1"))) {
        // Do nothing; keep default values
    }
    else if (theBuildingType.contains(QString("C2"))) {
        meanResponseTheta1 = 2.22865272680911;    covResponseTheta1 = 0.0899593468412107;
        meanResponseTheta2 = 0.63307331740037;    covResponseTheta2 = 0.11291130874424;
        meanResponseTheta3 = 1.08587183210043;    covResponseTheta3 = 0.0022772494391199;
        meanResponseTheta4 = 0.0196409535011445;  covResponseTheta4 = 0.00545369185829817;
        meanResponseTheta5 = 2.75993850185935;    covResponseTheta5 = 0.0374974925818043;
        meanResponseTheta6 = 0.387338160854746;   covResponseTheta6 = 0.0952607733361526;
        meanResponseTheta7 = 8.71967516479302;    covResponseTheta7 = 0.169471078047643;
        meanResponseTheta8 = 1.53681896491041;    covResponseTheta8 = 0.107580106792038;
    }
    else if (theBuildingType.contains(QString("C3"))) {
        meanResponseTheta1 = 2.22865272680911;    covResponseTheta1 = 0.0899593468412107;
        meanResponseTheta2 = 0.63307331740037;    covResponseTheta2 = 0.11291130874424;
        meanResponseTheta3 = 1.08587183210043;    covResponseTheta3 = 0.0022772494391199;
        meanResponseTheta4 = 0.0196409535011445;  covResponseTheta4 = 0.00545369185829817;
        meanResponseTheta5 = 2.75993850185935;    covResponseTheta5 = 0.0374974925818043;
        meanResponseTheta6 = 0.387338160854746;   covResponseTheta6 = 0.0952607733361526;
        meanResponseTheta7 = 7.63456446565424;    covResponseTheta7 = 0.317914673990779;
        meanResponseTheta8 = 0.608894392451661;   covResponseTheta8 = 0.528206256875503;
    }
    else if (theBuildingType.contains(QString("S1"))) {
        meanResponseTheta1 = 2.25587853663162;    covResponseTheta1 = 0.0351262921029214;
        meanResponseTheta2 = 0.793644012324843;   covResponseTheta2 = 0.0331141662321456;
        meanResponseTheta3 = 1.52985207256129;    covResponseTheta3 = 0.10217462316747;
        meanResponseTheta4 = 0.0234317276101293;  covResponseTheta4 = 0.225092682240912;
        meanResponseTheta5 = 2.78965124797442;    covResponseTheta5 = 0.0482815468382483;
        meanResponseTheta6 = 0.369769659457065;   covResponseTheta6 = 0.120806795064979;
        meanResponseTheta7 = 8.71967516479302;    covResponseTheta7 = 0.169471078047643;
        meanResponseTheta8 = 1.53681896491041;    covResponseTheta8 = 0.107580106792038;
    }
    else if (theBuildingType.contains(QString("S2"))) {
        meanResponseTheta1 = 2.48206675948044;    covResponseTheta1 = 0.0280336682741502;
        meanResponseTheta2 = 0.794286774765151;   covResponseTheta2 = 0.0290541119046308;
        meanResponseTheta3 = 1.10501207504222;    covResponseTheta3 = 0.0139172430703158;
        meanResponseTheta4 = 0.0147620716200757;  covResponseTheta4 = 0.0351516836219855;
        meanResponseTheta5 = 2.78965124797442;    covResponseTheta5 = 0.0482815468382483;
        meanResponseTheta6 = 0.369769659457065;   covResponseTheta6 = 0.120806795064979;
        meanResponseTheta7 = 8.60036382515231;    covResponseTheta7 = 0.274701830467166;
        meanResponseTheta8 = 1.29789267224511;    covResponseTheta8 = 0.20383859222897;
    }
    else if (theBuildingType.contains(QString("S3"))) {
        meanResponseTheta1 = 2.48206675948044;    covResponseTheta1 = 0.0280336682741502;
        meanResponseTheta2 = 0.794286774765151;   covResponseTheta2 = 0.0290541119046308;
        meanResponseTheta3 = 1.10501207504222;    covResponseTheta3 = 0.0139172430703158;
        meanResponseTheta4 = 0.0147620716200757;  covResponseTheta4 = 0.0351516836219855;
        meanResponseTheta5 = 2.78965124797442;    covResponseTheta5 = 0.0482815468382483;
        meanResponseTheta6 = 0.369769659457065;   covResponseTheta6 = 0.120806795064979;
        meanResponseTheta7 = 8.60036382515231;    covResponseTheta7 = 0.274701830467166;
        meanResponseTheta8 = 1.29789267224511;    covResponseTheta8 = 0.20383859222897;
    }
    else if (theBuildingType.contains(QString("S4"))) {
        meanResponseTheta1 = 2.47013392673656;    covResponseTheta1 = 0.0231743422232301;
        meanResponseTheta2 = 0.709414586804169;   covResponseTheta2 = 0.0267620483233741;
        meanResponseTheta3 = 1.32815562635643;    covResponseTheta3 = 0.0115790057571682;
        meanResponseTheta4 = 0.0147620716200757;  covResponseTheta4 = 0.0351516836219851;
        meanResponseTheta5 = 2.78965124797442;    covResponseTheta5 = 0.0482815468382483;
        meanResponseTheta6 = 0.369769659457065;   covResponseTheta6 = 0.120806795064979;
        meanResponseTheta7 = 8.60036382515231;    covResponseTheta7 = 0.274701830467166;
        meanResponseTheta8 = 1.29789267224511;    covResponseTheta8 = 0.20383859222897;
    }
    else if (theBuildingType.contains(QString("S5"))) {
        meanResponseTheta1 = 2.47013392673656;    covResponseTheta1 = 0.0231743422232301;
        meanResponseTheta2 = 0.709414586804169;   covResponseTheta2 = 0.0267620483233741;
        meanResponseTheta3 = 1.10501207504222;    covResponseTheta3 = 0.0139172430703158;
        meanResponseTheta4 = 0.0147620716200757;  covResponseTheta4 = 0.0351516836219855;
        meanResponseTheta5 = 2.78965124797442;    covResponseTheta5 = 0.0482815468382483;
        meanResponseTheta6 = 0.369769659457065;   covResponseTheta6 = 0.120806795064979;
        meanResponseTheta7 = 7.63456446565424;    covResponseTheta7 = 0.317914673990779;
        meanResponseTheta8 = 0.608894392451661;   covResponseTheta8 = 0.528206256875503;
    }
    else if (theBuildingType.contains(QString("PC"))) {
        meanResponseTheta1 = 2.22865272680911;    covResponseTheta1 = 0.0899593468412107;
        meanResponseTheta2 = 0.63307331740037;    covResponseTheta2 = 0.11291130874424;
        meanResponseTheta3 = 1.08587183210043;    covResponseTheta3 = 0.0022772494391199;
        meanResponseTheta4 = 0.0196409535011445;  covResponseTheta4 = 0.00545369185829817;
        meanResponseTheta5 = 2.75993850185935;    covResponseTheta5 = 0.0374974925818043;
        meanResponseTheta6 = 0.387338160854746;   covResponseTheta6 = 0.0952607733361526;
        meanResponseTheta7 = 8.60036382515231;    covResponseTheta7 = 0.274701830467166;
        meanResponseTheta8 = 1.29789267224511;    covResponseTheta8 = 0.20383859222897;
    }
    else if (theBuildingType.contains(QString("RM"))) {
        meanResponseTheta1 = 2.22865272680911;    covResponseTheta1 = 0.0899593468412107;
        meanResponseTheta2 = 0.63307331740037;    covResponseTheta2 = 0.11291130874424;
        meanResponseTheta3 = 0.796505685393632;   covResponseTheta3 = 0.00271287656210005;
        meanResponseTheta4 = 0.0196851672124356;  covResponseTheta4 = 0.00475492638444581;
        meanResponseTheta5 = 2.75993850185935;    covResponseTheta5 = 0.0374974925818043;
        meanResponseTheta6 = 0.387338160854746;   covResponseTheta6 = 0.0952607733361526;
        meanResponseTheta7 = 8.71967516479302;    covResponseTheta7 = 0.169471078047643;
        meanResponseTheta8 = 1.53681896491041;    covResponseTheta8 = 0.107580106792038;
    }
    else if (theBuildingType.contains(QString("URM"))) {
        meanResponseTheta1 = 1.67865192997027;    covResponseTheta1 = 0.0188101325110738;
        meanResponseTheta2 = 0.417921147711188;   covResponseTheta2 = 0.0391874922447207;
        meanResponseTheta3 = 2.20569553863626;    covResponseTheta3 = 0.0388611097538929;
        meanResponseTheta4 = 0.0267093821664968;  covResponseTheta4 = 0.425106847681514;
        meanResponseTheta5 = 2.28819973855749;    covResponseTheta5 = 0.0911049523260439;
        meanResponseTheta6 = 0.470365575016951;   covResponseTheta6 = 0.229873918323201;
        meanResponseTheta7 = 7.63456446565424;    covResponseTheta7 = 0.317914673990779;
        meanResponseTheta8 = 0.608894392451661;   covResponseTheta8 = 0.528206256875503;
    }
    else if (theBuildingType.contains(QString("W1"))) {
        meanResponseTheta1 = 1.76561947568034;    covResponseTheta1 = 0.0961229837253151;
        meanResponseTheta2 = 0.462344358810064;   covResponseTheta2 = 0.162153117571403;
        meanResponseTheta3 = 1.13935020598235;    covResponseTheta3 = 0.02267186809777;
        meanResponseTheta4 = 0.0114859963307501;  covResponseTheta4 = 0.170689654272867;
        meanResponseTheta5 = 2.5362785203959;     covResponseTheta5 = 0.0856320184007843;
        meanResponseTheta6 = 0.274628629225648;   covResponseTheta6 = 0.349344433361288;
        meanResponseTheta7 = 4.60710061521646;    covResponseTheta7 = 0.19761908784006;
        meanResponseTheta8 = 1.37226580049028;    covResponseTheta8 = 0.148642746607334;
        meanResponseTheta1 = 1.76561947568034;    covResponseTheta1 = 0.0961229837253151;
    }
    else if (theBuildingType.contains(QString("W2"))) {
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
        qCritical() << "Something is wrong with the building type" << theBuildingType << "in" << this->objectName();
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


    double alpha = 0.0;
    if (theCodeDesignLevel == RBIM::RCodeLevel::PreCode){
        alpha = 1.0;
    }
    else if (theCodeDesignLevel == RBIM::RCodeLevel::LowCode){
        alpha = 2.0;
    }
    else if (theCodeDesignLevel == RBIM::RCodeLevel::ModerateCode){
        alpha = 3.0;
    }
    else if (theCodeDesignLevel == RBIM::RCodeLevel::HighCode){
        alpha = 4.0;
    }

    double H = theBIM->getBuildingHeight();

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
    thePeakDriftResponse->setCurrentValue(peakDrift);


    // Evaluating the acceleration model expression
    double logAcc = -responseTheta16 * log(deltaY) + responseTheta17 * log(V) - responseTheta18 * log(Mu) + responseTheta19 * log(Kappa) + responseTheta20 * log(Sa) - responseTheta21 + epsilon2;
    double peakAcc = exp(logAcc);
    thePeakAccelerationResponse->setCurrentValue(peakAcc);


    // Save some stats
    theStatsMethod->update(thePeakDriftResponse->objectName(),peakDrift);
    theStatsMethod->update(thePeakAccelerationResponse->objectName(),peakAcc);

    return 0;
}
