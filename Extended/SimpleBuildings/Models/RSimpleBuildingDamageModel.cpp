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
*   - Stevan Gavrilovic                                              *
*   - Rodrigo Costa                                                  *
*                                                                    *
*********************************************************************/

#include "RSimpleBuildingDamageModel.h"
#include "RContinuousRandomVariable.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RConstant.h"
#include "RNormalDistribution.h"
#include "RBIM.h"

#include <math.h>

RSimpleBuildingDamageModel::RSimpleBuildingDamageModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = nullptr;
    thePeakDrift = nullptr;
    thePeakAcceleration = nullptr;
    theResidualDriftResponse = nullptr;
    theLocation = nullptr;
    theBuildingType = QString("C2M");
    theCodeDesignLevel = QString("High");
    theNumberOfStoreys = 1;
    theOccupancyClass = QString("RES3");
    theTenure = QString("Owner");

    theDomain->createObject(objectName() + "StructuralDamageRatioResponse", "RGenericResponse");
    theStructuralDamageRatio = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theStructuralDamageRatio->setModel(this);

    theDomain->createObject(objectName() + "NonstructuralDriftDamageRatioResponse", "RGenericResponse");
    theNonstructuralDriftDamageRatio = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theNonstructuralDriftDamageRatio->setModel(this);

    theDomain->createObject(objectName() + "NonstructuralAccelerationDamageRatioResponse", "RGenericResponse");
    theNonstructuralAccelerationDamageRatio = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theNonstructuralAccelerationDamageRatio->setModel(this);

    theDomain->createObject(objectName() + "EffectiveDamageRatio", "RGenericResponse");
    theEffectiveDamageRatio = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theEffectiveDamageRatio->setModel(this);

    theDomain->createObject(objectName() + "ContentDamageRatioResponse", "RGenericResponse");
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


}


RSimpleBuildingDamageModel::~RSimpleBuildingDamageModel()
{


}


QObject * RSimpleBuildingDamageModel::getPeakDrift() const
{
    return thePeakDrift;
}


void RSimpleBuildingDamageModel::setPeakDrift(QObject *value)
{
    thePeakDrift = qobject_cast<RResponse*>(value);
}


QObject * RSimpleBuildingDamageModel::getPeakAcceleration() const
{
    return thePeakAcceleration;
}


void RSimpleBuildingDamageModel::setPeakAcceleration(QObject *value)
{
    thePeakAcceleration = qobject_cast<RResponse*>(value);
}


QObject* RSimpleBuildingDamageModel::getResidualDrift() const
{
    return theResidualDriftResponse;
}


void RSimpleBuildingDamageModel::setResidualDrift(QObject *value)
{
    theResidualDriftResponse = qobject_cast<RResponse*>(value);
}


QObject * RSimpleBuildingDamageModel::getBIM() const
{
    return theBIM;
}


void RSimpleBuildingDamageModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);

    if(theBIM)
    {
        theLocation = qobject_cast<RParameter*>(theBIM->getLocation());
        theBuildingType = theBIM->getBuildingType();
        theNumberOfStoreys = theBIM->getNumberOfStoreys();
        theCodeDesignLevel = theBIM->getCodeLevel();
        theOccupancyClass = theBIM->getBuildingOccupancyClass();
    }
    else {
        qCritical()<<"Could not set the BIM in "<<__FUNCTION__;
    }

}


QString RSimpleBuildingDamageModel::getTenure() const
{
    return theTenure;
}


void RSimpleBuildingDamageModel::setTenure(QString value)
{
    theTenure = value;
}


double RSimpleBuildingDamageModel::getEffectiveDamageRatio()
{
    return theEffectiveDamageRatio->getCurrentValue();
}


double RSimpleBuildingDamageModel::getStrDamageRatio()
{
    return theStructuralDamageRatio->getCurrentValue();
}


double RSimpleBuildingDamageModel::getNSDDamageRatio()
{
    return theNonstructuralDriftDamageRatio->getCurrentValue();
}


double RSimpleBuildingDamageModel::getNSADamageRatio()
{
    return theNonstructuralAccelerationDamageRatio->getCurrentValue();
}


double RSimpleBuildingDamageModel::getContentDamageRatio()
{
    return theContentDamageRatio->getCurrentValue();
}


int RSimpleBuildingDamageModel::evaluateModel(RGradientType theGradientType)
{
    theStructuralDamageRatio->setCurrentValue(0.0);
    theNonstructuralDriftDamageRatio->setCurrentValue(0.0);
    theNonstructuralAccelerationDamageRatio->setCurrentValue(0.0);
    theContentDamageRatio->setCurrentValue(0.0);
    theEffectiveDamageRatio->setCurrentValue(0.0);

    if(theBIM == nullptr)
    {
        qDebug()<<"Error, the model "<<this->objectName()<<" needs a BIM";
        return 0;
    }

    // Looking up the drift and acceleration
    double drift = thePeakDrift->getCurrentValue();
    double acceleration = thePeakAcceleration->getCurrentValue();

    // End quickly if there is no acceleration
    if (drift <= 0.0 && acceleration <= 0.0){
        return 0;
    }

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

    if (theBuildingType.contains(QString("C1"))) {
        // Do nothing; keep default values
    }
    else if (theBuildingType.contains(QString("C2"))) {
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
    else if (theBuildingType.contains(QString("C3"))) {
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
    else if (theBuildingType.contains(QString("S1"))) {
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
    else if (theBuildingType.contains(QString("S2"))) {
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
    else if (theBuildingType.contains(QString("S3"))) {
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
    else if (theBuildingType.contains(QString("S4"))) {
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
    else if (theBuildingType.contains(QString("S5"))) {
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
    else if (theBuildingType.contains(QString("PC"))) {
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
    else if (theBuildingType.contains(QString("RM"))) {
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
    else if (theBuildingType.contains(QString("URM"))) {
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
    else if (theBuildingType.contains(QString("W1"))) {
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
    else if (theBuildingType.contains(QString("W2"))) {
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

    // Look up for irregularities
    double Ivi = 0;
    double Ipi = 0;
    double Iss = 0;
    double Isc = 0;
    double Ip = 0;

    // if (theBIM->getVerticalIrregularity()) {
    //     Ivi = 1.0;
    // }
    // if (theBIM->getVerticalIrregularity()) {
    //     Ipi = 1.0;
    // }
    // if (theBIM->getSoftStorey()) {
    //     Iss = 1.0;
    // }
    // if (theBIM->getShortColumn()) {
    //     Isc = 1.0;
    // }
    // if (theBIM->getPounding()) {
    //     Ip = 1.0;
    // }

    // This is also in the response model:
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

    // Evaluating the model expression of the structural damage model
    RNormalDistribution theNormalDistribution(theDomain);
    double damageRatioS = theNormalDistribution.getNormalCDF(damageTheta1 * log(drift) + damageTheta2 * log(H) - damageTheta3 * alpha + damageTheta4, 1.0) * exp(damageTheta5 * Ivi + damageTheta6 * Ipi + damageTheta7 * Iss + damageTheta8 * Isc + damageTheta9 * Ip);
    // double damageRatioS = theNormalDistribution.getNormalCDF(meanDamageTheta1 * log(drift) + meanDamageTheta2 * log(H) - meanDamageTheta3 * alpha + meanDamageTheta4, 1.0) * exp(meanDamageTheta5 * Ivi + meanDamageTheta6 * Ipi + meanDamageTheta7 * Iss + meanDamageTheta8 * Isc + meanDamageTheta9 * Ip);

    theStructuralDamageRatio->setCurrentValue(damageRatioS);
    // qDebug() << "The structural damage ratio is:" << damageRatioS << "for a drift of:" << drift;

    // Evaluating the model expression of the non-structural drift-sensitive damage model
    double damageRatioND = theNormalDistribution.getNormalCDF(damageTheta10 * log(drift) + damageTheta11, 1.0);
    theNonstructuralDriftDamageRatio->setCurrentValue(damageRatioND);

    // Evaluating the model expression of the non-structural acecleration-sensitive damage model
    double damageRatioNA = theNormalDistribution.getNormalCDF(damageTheta12 * log(acceleration) - damageTheta13 * alpha, 1.0);
    theNonstructuralAccelerationDamageRatio->setCurrentValue(damageRatioNA);

    // Evaluating the model expression of the content damage model
    double damageRatioC = theNormalDistribution.getNormalCDF(damageTheta14 * log(acceleration) - damageTheta15 * alpha, 1.0);
    theContentDamageRatio->setCurrentValue(damageRatioC);

    // Irreparability
    RNormalDistribution theProbabilityDistributions(theDomain);
    double pIrreparability = 0.0;
    double Dr = theResidualDriftResponse->getCurrentValue();


    // 'Total' damage state
    double dijk = (damageRatioS + damageRatioND + damageRatioNA)/3.0;
    theEffectiveDamageRatio->setCurrentValue(damageRatioS);


    // If too much loss
    if (dijk > 0.60){
        pIrreparability = 1.0;
    }
    // If old building
    else if (dijk > 0.45 * 1.25 && (theCodeDesignLevel == QString("Pre") || theCodeDesignLevel == QString("Low"))){
        pIrreparability = 1.0;
    }
    // If damage and rented
    else if (dijk > 0.45 * 1.25 && theTenure == QString("Renter") && theOccupancyClass != QString("RES3")) {
        pIrreparability = 1.0;
    }
    // If this is a concrete building consider the residual drift limit state
    else if (theBuildingType.contains("C")) {
        pIrreparability = theProbabilityDistributions.getNormalCDF((1.0 / 0.3) * log(Dr / 1.0), 1);
    }


    double  rngReparability = (double) rand()/RAND_MAX;
    if (rngReparability <= pIrreparability){

        // If the building is irreparable it has to be demolished AND rebuilt
        // Demolition costs are 20%-30% of the cost of the building (FEMA-P58)
        theEffectiveDamageRatio->setCurrentValue(1.25);
    }


    return 0;
}
