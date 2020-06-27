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

#include "RBuildingDamageModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RBuildingInformationModel.h"
#include "RProbabilityDistributions.h"

#include <math.h>

RBuildingDamageModel::RBuildingDamageModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBuildingInformationModel = 0;
    thePeakDriftRatio = 0;
    thePeakAcceleration = 0;
    theProbabilityDistributions = 0;
    theBuildingType = "Concrete frame";

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

    QString theta1Name = "BuildingDamageModelTheta1";
    theTheta1 = theDomain->findChild<RContinuousRandomVariable *>(theta1Name);
    if (!theTheta1) {
        theDomain->createObject(theta1Name, "RContinuousRandomVariable");
        theTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta1->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta1->setMean("0.0");
        theTheta1->setStandardDeviation("1.0");
        theAutoParametersList << theTheta1;
    }

    QString theta2Name = "BuildingDamageModelTheta2";
    theTheta2 = theDomain->findChild<RContinuousRandomVariable *>(theta2Name);
    if (!theTheta2) {
        theDomain->createObject(theta2Name, "RContinuousRandomVariable");
        theTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta2->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta2->setMean("0.0");
        theTheta2->setStandardDeviation("1.0");
        theAutoParametersList << theTheta2;
    }

    QString theta3Name = "BuildingDamageModelTheta3";
    theTheta3 = theDomain->findChild<RContinuousRandomVariable *>(theta3Name);
    if (!theTheta3) {
        theDomain->createObject(theta3Name, "RContinuousRandomVariable");
        theTheta3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta3->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta3->setMean("0.0");
        theTheta3->setStandardDeviation("1.0");
        theAutoParametersList << theTheta3;
    }

    QString theta4Name = "BuildingDamageModelTheta4";
    theTheta4 = theDomain->findChild<RContinuousRandomVariable *>(theta4Name);
    if (!theTheta4) {
        theDomain->createObject(theta4Name, "RContinuousRandomVariable");
        theTheta4 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta4->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta4->setMean("0.0");
        theTheta4->setStandardDeviation("1.0");
        theAutoParametersList << theTheta4;
    }

    QString theta5Name = "BuildingDamageModelTheta5";
    theTheta5 = theDomain->findChild<RContinuousRandomVariable *>(theta5Name);
    if (!theTheta5) {
        theDomain->createObject(theta5Name, "RContinuousRandomVariable");
        theTheta5 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta5->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta5->setMean("0.0");
        theTheta5->setStandardDeviation("1.0");
        theAutoParametersList << theTheta5;
    }

    QString theta6Name = "BuildingDamageModelTheta6";
    theTheta6 = theDomain->findChild<RContinuousRandomVariable *>(theta6Name);
    if (!theTheta6) {
        theDomain->createObject(theta6Name, "RContinuousRandomVariable");
        theTheta6 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta6->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta6->setMean("0.0");
        theTheta6->setStandardDeviation("1.0");
        theAutoParametersList << theTheta6;
    }

    QString theta7Name = "BuildingDamageModelTheta7";
    theTheta7 = theDomain->findChild<RContinuousRandomVariable *>(theta7Name);
    if (!theTheta7) {
        theDomain->createObject(theta7Name, "RContinuousRandomVariable");
        theTheta7 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta7->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta7->setMean("0.0");
        theTheta7->setStandardDeviation("1.0");
        theAutoParametersList << theTheta7;
    }

    QString theta8Name = "BuildingDamageModelTheta8";
    theTheta8 = theDomain->findChild<RContinuousRandomVariable *>(theta8Name);
    if (!theTheta8) {
        theDomain->createObject(theta8Name, "RContinuousRandomVariable");
        theTheta8 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta8->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta8->setMean("0.0");
        theTheta8->setStandardDeviation("1.0");
        theAutoParametersList << theTheta8;
    }

    QString theta9Name = "BuildingDamageModelTheta9";
    theTheta9 = theDomain->findChild<RContinuousRandomVariable *>(theta9Name);
    if (!theTheta9) {
        theDomain->createObject(theta9Name, "RContinuousRandomVariable");
        theTheta9 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta9->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta9->setMean("0.0");
        theTheta9->setStandardDeviation("1.0");
        theAutoParametersList << theTheta9;
    }




/*
    QString epsilonName = "BuildingDamageModelEpsilon";
    theEpsilon1 = theDomain->findChild<RContinuousRandomVariable *>(epsilonName);
    if (!theEpsilon1) {
        theDomain->createObject(epsilonName, "RContinuousRandomVariable");
        theEpsilon1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theEpsilon1->setDistributionType(QString("Normal (mean, stdv)"));
        theEpsilon1->setMean("0.0");
        theEpsilon1->setStandardDeviation("1.0");
        theAutoParametersList << theEpsilon1;
    }
*/




    QString theta10Name = "BuildingDamageModelTheta10";
    theTheta10 = theDomain->findChild<RContinuousRandomVariable *>(theta10Name);
    if (!theTheta10) {
        theDomain->createObject(theta10Name, "RContinuousRandomVariable");
        theTheta10 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta10->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta10->setMean("1.27310387789009");
        theTheta10->setCoefficientOfVariation("0.009659920321684");
        theAutoParametersList << theTheta10;
    }

    QString theta11Name = "BuildingDamageModelTheta11";
    theTheta11 = theDomain->findChild<RContinuousRandomVariable *>(theta11Name);
    if (!theTheta11) {
        theDomain->createObject(theta11Name, "RContinuousRandomVariable");
        theTheta11 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta11->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta11->setMean("4.86771763084631");
        theTheta11->setCoefficientOfVariation("0.009500499962581");
        theAutoParametersList << theTheta11;
    }

    QString theta12Name = "BuildingDamageModelTheta12";
    theTheta12 = theDomain->findChild<RContinuousRandomVariable *>(theta12Name);
    if (!theTheta12) {
        theDomain->createObject(theta12Name, "RContinuousRandomVariable");
        theTheta12 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta12->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta12->setMean("1.26757284351552");
        theTheta12->setCoefficientOfVariation("0.006798166443569");
        theAutoParametersList << theTheta12;
    }

    QString theta13Name = "BuildingDamageModelTheta13";
    theTheta13 = theDomain->findChild<RContinuousRandomVariable *>(theta13Name);
    if (!theTheta13) {
        theDomain->createObject(theta13Name, "RContinuousRandomVariable");
        theTheta13 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta13->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta13->setMean("0.193211895250043");
        theTheta13->setCoefficientOfVariation("0.00827053736024");
        theAutoParametersList << theTheta13;
    }

    QString theta14Name = "BuildingDamageModelTheta14";
    theTheta14 = theDomain->findChild<RContinuousRandomVariable *>(theta14Name);
    if (!theTheta14) {
        theDomain->createObject(theta14Name, "RContinuousRandomVariable");
        theTheta14 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta14->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta14->setMean("1.29985185840713");
        theTheta14->setCoefficientOfVariation("0.008459627480646");
        theAutoParametersList << theTheta14;
    }

    QString theta15Name = "BuildingDamageModelTheta15";
    theTheta15 = theDomain->findChild<RContinuousRandomVariable *>(theta15Name);
    if (!theTheta15) {
        theDomain->createObject(theta15Name, "RContinuousRandomVariable");
        theTheta15 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta15->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta15->setMean("0.12808792466436");
        theTheta15->setCoefficientOfVariation("0.014976535492789");
        theAutoParametersList << theTheta15;
    }



/*
    QString nonStructuralDamageEpsilonName = "BuildingDamageModelNonStructuralDamageEpsilon";
    theEpsilon2 = theDomain->findChild<RContinuousRandomVariable *>(nonStructuralDamageEpsilonName);
    if (!theEpsilon2) {
        theDomain->createObject(nonStructuralDamageEpsilonName, "RContinuousRandomVariable");
        theEpsilon2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theEpsilon2->setDistributionType(QString("Normal (mean, stdv)"));
        theEpsilon2->setMean("0.0");
        theEpsilon2->setStandardDeviation("0.036069031303692");
        theAutoParametersList << theEpsilon2;
    }

    QString nonStructuralAccSensDamageEpsilonName = "BuildingDamageModelNonStructuralAccSensDamageEpsilon";
    theEpsilon3 = theDomain->findChild<RContinuousRandomVariable *>(nonStructuralAccSensDamageEpsilonName);
    if (!theEpsilon3) {
        theDomain->createObject(nonStructuralAccSensDamageEpsilonName, "RContinuousRandomVariable");
        theEpsilon3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theEpsilon3->setDistributionType(QString("Normal (mean, stdv)"));
        theEpsilon3->setMean("0.0");
        theEpsilon3->setStandardDeviation("0.024409954404172");
        theAutoParametersList << theEpsilon3;
    }

    QString contentDamageEpsilonName = "BuildingDamageModelContentDamageEpsilon";
    theEpsilon4 = theDomain->findChild<RContinuousRandomVariable *>(contentDamageEpsilonName);
    if (!theEpsilon4) {
        theDomain->createObject(contentDamageEpsilonName, "RContinuousRandomVariable");
        theEpsilon4 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theEpsilon4->setDistributionType(QString("Normal (mean, stdv)"));
        theEpsilon4->setMean("0.0");
        theEpsilon4->setStandardDeviation("0.032479292799483");
        theAutoParametersList << theEpsilon4;
    }
*/

}

RBuildingDamageModel::~RBuildingDamageModel()
{

}




QObject * RBuildingDamageModel::getProbabilityDistributions() const
{
    return theProbabilityDistributions;
}

void RBuildingDamageModel::setProbabilityDistributions(QObject *value)
{
    theProbabilityDistributions = qobject_cast<RProbabilityDistributions *>(value);
}




QObject * RBuildingDamageModel::getBuildingInformationModel() const	
{	
    return theBuildingInformationModel;
}	

void RBuildingDamageModel::setBuildingInformationModel(QObject *value)	
{	
    theBuildingInformationModel = qobject_cast<RModel *>(value);
}	

QObject * RBuildingDamageModel::getPeakDriftRatio() const	
{	
    return thePeakDriftRatio;
}	

void RBuildingDamageModel::setPeakDriftRatio(QObject *value)	
{	
    thePeakDriftRatio = qobject_cast<RParameter *>(value);
}	

QObject * RBuildingDamageModel::getPeakAcceleration() const	
{	
    return thePeakAcceleration;
}	

void RBuildingDamageModel::setPeakAcceleration(QObject *value)	
{	
    thePeakAcceleration = qobject_cast<RParameter *>(value);
}	




QString RBuildingDamageModel::getBuildingType()
{
    return theBuildingType;
}

void RBuildingDamageModel::setBuildingType(QString value)
{
    theBuildingType = QString("empty");

    QStringList buildingTypes;
    buildingTypes << "Concrete frame"
                  << "Concrete shear wall"
                  << "Concrete frame with masonry infill"
                  << "Steel frame"
                  << "Braced steel frame"
                  << "Light steel frame"
                  << "Steel frame with concrete shear wall"
                  << "Steel frame  with masonry infill"
                  << "Precast concrete"
                  << "Reinforced masonry"
                  << "Unreinforced masonry"
                  << "Light wood frame"
                  << "Large wood frame";

    for (int i=0; i<buildingTypes.size(); i++) {
        if (value.contains(buildingTypes.at(i))) {
            theBuildingType = buildingTypes.at(i);
        }
    }

    if (theBuildingType == QString("empty")) {
        qCritical() << this->objectName() << "has an error in the building type; the options are:";
        for (int i=0; i<buildingTypes.size(); i++) {
            qDebug() << buildingTypes.at(i);
        }
        theBuildingType = buildingTypes.at(0);
    }
}



int RBuildingDamageModel::evaluateModel(bool evaluateDDM)
{
    
    
    
    // Use concrete frame values as default
    double meanTheta1 = 1.30535777188603;  double covTheta1 = 0.006851725426429;
    double meanTheta2 = 0.664820813681537; double covTheta2 = 0.010398857098557;
    double meanTheta3 = 0.286778342140848; double covTheta3 = 0.014494019754025;
    double meanTheta4 = 4.33744720988832;  double covTheta4 = 0.008498405604416;
    double meanTheta5 = 0.392129368320604; double covTheta5 = 0.048097225079779;
    double meanTheta6 = 0.246862945558682; double covTheta6 = 0.060319964252776;
    double meanTheta7 = 0.924172549839815; double covTheta7 = 0.025872110052779;
    double meanTheta8 = 0.616677026202032; double covTheta8 = 0.020086111302224;
    double meanTheta9 = 0.302756951133824; double covTheta9 = 0.034283652657823;
    double meanEpsilon = 0; double stdvEpsilon = 0.018781277985919;

    if (theBuildingType == QString("Concrete frame")) {
        // Do nothing; keep default values
    }
    if (theBuildingType == QString("Concrete shear wall")) {
        meanTheta1 = 1.21465614269761;  covTheta1 = 0.006402151535424;
        meanTheta2 = 0.626771402181758; covTheta2 = 0.011341103547784;
        meanTheta3 = 0.268108592798877; covTheta3 = 0.01614219789087;
        meanTheta4 = 4.02508724041101;  covTheta4 = 0.007763739253467;
        meanTheta5 = 0.113858697009988; covTheta5 = 0.172299182670169;
        meanTheta6 = 0.289690474445929; covTheta6 = 0.032907849837814;
        meanTheta7 = 0.838807543600913; covTheta7 = 0.02344426157169;
        meanTheta8 = 0.388327286045199; covTheta8 = 0.03917764151005;
        meanTheta9 = 0.00001;           covTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.014565878894457;
    }
    else if (theBuildingType == QString("Concrete frame with masonry infill")) {
        meanTheta1 = 1.35937261048995;  covTheta1 = 0.009766728201417;
        meanTheta2 = 0.691532934649142; covTheta2 = 0.015275094981982;
        meanTheta3 = 0.369626826372066; covTheta3 = 0.031767159618159;
        meanTheta4 = 5.05032390445079;  covTheta4 = 0.011188088842238;
        meanTheta5 = 0.650471648516973; covTheta5 = 0.024731772645831;
        meanTheta6 = 0.293351868679244; covTheta6 = 0.08376336469095;
        meanTheta7 = 0.471053380089266; covTheta7 = 0.054258266050058;
        meanTheta8 = 0.53900054321154;  covTheta8 = 0.040958726843212;
        meanTheta9 = 0.00001;           covTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.020402736295115;
    }
    else if (theBuildingType == QString("Steel frame")) {
        meanTheta1 = 1.32187575016808;  covTheta1 = 0.006293266110094;
        meanTheta2 = 0.626424186503563; covTheta2 = 0.010831473939025;
        meanTheta3 = 0.275353821300956; covTheta3 = 0.014305656910222;
        meanTheta4 = 4.34777181647321;  covTheta4 = 0.007894744131077;
        meanTheta5 = 0.267733203347781; covTheta5 = 0.045734929550636;
        meanTheta6 = 0.283722996846665; covTheta6 = 0.037877084692777;
        meanTheta7 = 0.877696156286803; covTheta7 = 0.024403638334624;
        meanTheta8 = 0.00001;           covTheta8 = 0.001;
        meanTheta9 = 0.274924901093514; covTheta9 = 0.037898568566978;
        meanEpsilon = 0; stdvEpsilon = 0.01702440236915;
    }
    else if (theBuildingType == QString("Braced steel frame")) {
        meanTheta1 = 1.28175966436694;  covTheta1 = 0.007224258672286;
        meanTheta2 = 0.646696697818393; covTheta2 = 0.01245972436738;
        meanTheta3 = 0.287756325416325; covTheta3 = 0.015739438691391;
        meanTheta4 = 4.15856391401591;  covTheta4 = 0.009000344605634;
        meanTheta5 = 0.170846423400316; covTheta5 = 0.121901726388769;
        meanTheta6 = 0.315525353511683; covTheta6 = 0.033741851519453;
        meanTheta7 = 1.10207271404515;  covTheta7 = 0.017181991540148;
        meanTheta8 = 0.00001;           covTheta8 = 0.001;
        meanTheta9 = 0.224400903886254; covTheta9 = 0.057888309643459;
        meanEpsilon = 0; stdvEpsilon = 0.018252553662291;
    }
    else if (theBuildingType == QString("Light steel frame")) {
        meanTheta1 = 1.238226897465;    covTheta1 = 0.010278083637242;
        meanTheta2 = 0.00001;           covTheta2 = 0.001;
        meanTheta3 = 0.277772796069453; covTheta3 = 0.022440394343561;
        meanTheta4 = 5.48917795737416;  covTheta4 = 0.010745397198204;
        meanTheta5 = 0.210309534269694; covTheta5 = 0.117972907024925;
        meanTheta6 = 0.329653135573926; covTheta6 = 0.05001245293764;
        meanTheta7 = 0.585308059114181; covTheta7 = 0.030939707531868;
        meanTheta8 = 0.00001;           covTheta8 = 0.001;
        meanTheta9 = 0.346517454949579; covTheta9 = 0.039669885289746;
        meanEpsilon = 0; stdvEpsilon = 0.019653455184743;
    }
    else if (theBuildingType == QString("Steel frame with concrete shear wall")) {
        meanTheta1 = 1.25124808132616;  covTheta1 = 0.007747213985819;
        meanTheta2 = 0.665015152837385; covTheta2 = 0.012231122581524;
        meanTheta3 = 0.291212721782239; covTheta3 = 0.016401621413013;
        meanTheta4 = 4.19951180032007;  covTheta4 = 0.009593830896499;
        meanTheta5 = 0.407003539055466; covTheta5 = 0.051550945079048;
        meanTheta6 = 0.30755286135054;  covTheta6 = 0.042074426847175;
        meanTheta7 = 0.997086315608624; covTheta7 = 0.021117256856772;
        meanTheta8 = 0.00001;           covTheta8 = 0.001;
        meanTheta9 = 0.229554284257269; covTheta9 = 0.055459081819921;
        meanEpsilon = 0; stdvEpsilon = 0.02003279046568;
    }
    else if (theBuildingType == QString("Steel frame  with masonry infill")) {
        meanTheta1 = 1.30486453250648;  covTheta1 = 0.00917330025994;
        meanTheta2 = 0.613542033821746; covTheta2 = 0.016060321130632;
        meanTheta3 = 0.375090675773637; covTheta3 = 0.030613277476669;
        meanTheta4 = 4.87527241783118;  covTheta4 = 0.01020491042874;
        meanTheta5 = 0.587020986003727; covTheta5 = 0.025471756822839;
        meanTheta6 = 0.26636308516701;  covTheta6 = 0.085444838942947;
        meanTheta7 = 0.43535766409691;  covTheta7 = 0.052680895764799;
        meanTheta8 = 0.00001;           covTheta8 = 0.001;
        meanTheta9 = 0.00001;           covTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.017549305835961;
    }
    else if (theBuildingType == QString("Precast concrete")) {
        meanTheta1 = 1.23459116707839;  covTheta1 = 0.005903044342454;
        meanTheta2 = 0.651442723437484; covTheta2 = 0.009470832278259;
        meanTheta3 = 0.253848520921968; covTheta3 = 0.01489125110138;
        meanTheta4 = 4.22903902777438;  covTheta4 = 0.007317698706426;
        meanTheta5 = 0.375453044529677; covTheta5 = 0.042450227727026;
        meanTheta6 = 0.338533472594216; covTheta6 = 0.047281961998664;
        meanTheta7 = 0.535140195336066; covTheta7 = 0.019920211330669;
        meanTheta8 = 0.371933817633649; covTheta8 = 0.038282494212727;
        meanTheta9 = 0.203089715598823; covTheta9 = 0.05292420248958;
        meanEpsilon = 0; stdvEpsilon = 0.016011211020035;
    }
    else if (theBuildingType == QString("Reinforced masonry")) {
        meanTheta1 = 1.18371377806936;  covTheta1 = 0.007765576834722;
        meanTheta2 = 0.6231401567677;   covTheta2 = 0.014993509845317;
        meanTheta3 = 0.268503431683156; covTheta3 = 0.019329208610842;
        meanTheta4 = 4.10957395391823;  covTheta4 = 0.009282595484385;
        meanTheta5 = 0.101850323590683; covTheta5 = 0.216197087743516;
        meanTheta6 = 0.425428224039622; covTheta6 = 0.038738323604197;
        meanTheta7 = 0.781350695240031; covTheta7 = 0.027583662324846;
        meanTheta8 = 0.00001;           covTheta8 = 0.001;
        meanTheta9 = 0.00001;           covTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.017299183891961;
    }
    else if (theBuildingType == QString("Unreinforced masonry")) {
        meanTheta1 = 1.36258655513574;  covTheta1 = 0.009806759991199;
        meanTheta2 = 0.618816027195958; covTheta2 = 0.027147851751869;
        meanTheta3 = 0.300288271512769; covTheta3 = 0.045566513034644;
        meanTheta4 = 5.29673222299837;  covTheta4 = 0.012756792701196;
        meanTheta5 = 0.713863081370799; covTheta5 = 0.029992962475781;
        meanTheta6 = 0.582610552545055; covTheta6 = 0.03420537038336;
        meanTheta7 = 0.572662916589229; covTheta7 = 0.042622084396919;
        meanTheta8 = 0.00001;           covTheta8 = 0.001;
        meanTheta9 = 0.00001;           covTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.017672913936856;
    }
    else if (theBuildingType == QString("Light wood frame")) {
        meanTheta1 = 1.154227919813;    covTheta1 = 0.005799987905193;
        meanTheta2 = 0.00001;           covTheta2 = 0.001;
        meanTheta3 = 0.170402360927627; covTheta3 = 0.021942867014654;
        meanTheta4 = 4.32829658800862;  covTheta4 = 0.006622432802063;
        meanTheta5 = 0.165926773126374; covTheta5 = 0.054946848921871;
        meanTheta6 = 0.380492787718244; covTheta6 = 0.031645015489524;
        meanTheta7 = 0.319445578338458; covTheta7 = 0.037082082344196;
        meanTheta8 = 0.00001;           covTheta8 = 0.001;
        meanTheta9 = 0.00001;           covTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.007929896467867;
    }
    else if (theBuildingType == QString("Large wood frame")) {
        meanTheta1 = 1.20647329309667;   covTheta1 = 0.007208878770065;
        meanTheta2 = 0.00001;            covTheta2 = 0.001;
        meanTheta3 = -0.178281288390707; covTheta3 = 0.028090854114829;
        meanTheta4 = 4.52424806438601;   covTheta4 = 0.008392336274958;
        meanTheta5 = 0.33270064404502;   covTheta5 = 0.029972760968811;
        meanTheta6 = 0.51043151953732;   covTheta6 = 0.029416591115207;
        meanTheta7 = 0.561856647995703;  covTheta7 = 0.022002546442442;
        meanTheta8 = 0.00001;            covTheta8 = 0.001;
        meanTheta9 = 0.00001;            covTheta9 = 0.001;
        meanEpsilon = 0; stdvEpsilon = 0.013623306613564;
    }
    else {
        qCritical() << "Something is wrong with the building type in" << this->objectName();
    }



    // Looking up the model parameters, remembering that the first nine, and one epsilon, must be transformed
    // y = (x-mu)/sigma   -->   x = y * sigma + mu = y * mu * cov + mu =  (y * cov + 1) * mu
    double theta1 = (theTheta1->getCurrentValue() * covTheta1 + 1.0) * meanTheta1;
    double theta2 = (theTheta2->getCurrentValue() * covTheta2 + 1.0) * meanTheta2;
    double theta3 = (theTheta3->getCurrentValue() * covTheta3 + 1.0) * meanTheta3;
    double theta4 = (theTheta4->getCurrentValue() * covTheta4 + 1.0) * meanTheta4;
    double theta5 = (theTheta5->getCurrentValue() * covTheta5 + 1.0) * meanTheta5;
    double theta6 = (theTheta6->getCurrentValue() * covTheta6 + 1.0) * meanTheta6;
    double theta7 = (theTheta7->getCurrentValue() * covTheta7 + 1.0) * meanTheta7;
    double theta8 = (theTheta8->getCurrentValue() * covTheta8 + 1.0) * meanTheta8;
    double theta9 = (theTheta9->getCurrentValue() * covTheta9 + 1.0) * meanTheta9;

    //double epsilon1 = theEpsilon1->getCurrentValue() * stdvEpsilon + meanEpsilon;


    double theta10 = theTheta10->getCurrentValue();
    double theta11 = theTheta11->getCurrentValue();
    double theta12 = theTheta12->getCurrentValue();
    double theta13 = theTheta13->getCurrentValue();
    double theta14 = theTheta14->getCurrentValue();
    double theta15 = theTheta15->getCurrentValue();

    // Looking up the model error parameters
    //double epsilon2 = theEpsilon2->getCurrentValue();
    //double epsilon3 = theEpsilon3->getCurrentValue();
    //double epsilon4 = theEpsilon4->getCurrentValue();

    // Looking up the drift and acceleration
    double drift = thePeakDriftRatio->getCurrentValue();
    double acceleration = thePeakAcceleration->getCurrentValue();


    // Getting the information from the BIM
    RBuildingInformationModel *theBIM = qobject_cast<RBuildingInformationModel *>(theBuildingInformationModel);
    if (!theBIM) {
        qCritical() << "Error in model" << objectName() << ":" << theBuildingInformationModel->objectName() << "is not a building information model.";
        return -1;
    }

    RBuildingInformationModel::RCodeLevel codeLevel = theBIM->getCodeLevel();
    double alpha = codeLevel + 1.0;
    double H = theBIM->getHeight();


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
    double damageRatioS = theProbabilityDistributions->getNormalCDF(theta1 * log(drift) + theta2 * log(H) - theta3 * alpha + theta4, 1.0) * exp(theta5 * Ivi + theta6 * Ipi + theta7 * Iss + theta8 * Isc + theta9 * Ip);
    theStructuralDamageRatio->setCurrentValue(damageRatioS);


    // Evaluating the model expression of the non-structural drift-sensitive damage model
    double damageRatioND = theProbabilityDistributions->getNormalCDF(theta10 * log(drift) + theta11, 1.0);
    theNonstructuralDriftDamageRatio->setCurrentValue(damageRatioND);


    // Evaluating the model expression of the non-structural acecleration-sensitive damage model
    double damageRatioNA = theProbabilityDistributions->getNormalCDF(theta12 * log(acceleration) - theta13 * alpha, 1.0);
    theNonstructuralAccelerationDamageRatio->setCurrentValue(damageRatioNA);


    // Evaluating the model expression of the content damage model
    double damageRatioC = theProbabilityDistributions->getNormalCDF(theta14 * log(acceleration) - theta15 * alpha, 1.0);
    theContentDamageRatio->setCurrentValue(damageRatioC);

    if (evaluateDDM) {

        qCritical() << "Implement DDM here...";
    }

    return 0;
}

void RBuildingDamageModel::updateHistoryVariables()
{
    if (theStructuralDamageRatio->getCurrentValue() > 0.5) {
        theBuildingInformationModel->updateHistoryVariables();
    }
}

void RBuildingDamageModel::resetHistoryVariables()
{
    theBuildingInformationModel->resetHistoryVariables();
}
