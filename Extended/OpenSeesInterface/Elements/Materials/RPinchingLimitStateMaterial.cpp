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
*                                                                    *
*********************************************************************/

#include "RPinchingLimitStateMaterial.h"
#include "qcustomplot.h"
#include "RMainWindow.h"
#include "RParameter.h"
#include "RLimitCurve.h"
#include "RDomain.h"

#include "Domain.h"
#include "PinchingLimitStateMaterial.h"

RPinchingLimitStateMaterial::RPinchingLimitStateMaterial(QObject *parent, QString &name)
    : RUniaxialMaterial(parent, name)
{
    theMaterial = nullptr;
    KelasParameter = nullptr;
    theLimitCurve = nullptr;

    ndT   =-1;
    ndB   =-1;
    driftAxis =-1;
    crvTyp = -1;

    theOSDomain = nullptr;
    theNodeT = nullptr;
    theNodeB = nullptr;

    //The values below are from example: http://opensees.berkeley.edu/wiki/index.php/PinchingLimitStateMaterial_Example

    YpinchUPN = 0.3;	//floating point unloading force pinching factor for loading in the negative direction
    YpinchRPN = 0.2;	//floating point reloading force pinching factor for loading in the negative direction
    XpinchRPN = 0.0;   //floating point reloading displacement pinching factor for loading in the negative direction
    YpinchUNP = 0.3;	//floating point unloading force pinching factor for loading in the positive direction
    YpinchRNP = 0.2;	//floating point reloading force pinching factor for loading in the positive direction
    XpinchRNP = 0.0;	//floating point reloading displacement pinching factor for loading in the positive direction
    dmgStrsLimE = 0.0;	//floating point force limit for elastic stiffness damage (typically defined as the lowest of shear strength or shear at flexrual yielding).
    dmgDispMax = 1.0;	//floating point for ultimate drift at failure (δmax Eq. 1) and is used for strength and stiffness damage.

    //floating point elastic stiffness damage factors α1,α2,α3,α4 shown in Eq. 1
    dmgE1 = 0.0;
    dmgE2 = 0.0;
    dmgE3 = 0.0;
    dmgE4 = 0.0;

    dmgELim = 0.0;	//floating point elastic stiffness damage limit Dlim shown in Eq. 1 = 0.0; Note: This value must be between zero and unity

    //floating point reloading stiffness damage factors α1,α2,α3,α4 shown in Eq. 1
    dmgR1 = 0.0;
    dmgR2 = 0.0;
    dmgR3 = 0.0;
    dmgR4 = 0.0;

    dmgRLim = 0.0;	//floating point reloading stiffness damage limit Dlim shown in Eq. 1 = 0.0; Note: This value must be between zero and unity
    dmgRCyc = 0.0;	//floating point cyclic reloading stiffness damage index = 0.0; Note: This value must be between zero and unity
    //floating point backbone strength damage factors α1,α2,α3,α4 shown in Eq. 1
    dmgS1 = 0.0;
    dmgS2 = 0.0;
    dmgS3 = 0.0;
    dmgS4 = 0.0;

    dmgSLim = 0.0;	//floating point backbone strength damage limit Dlim shown in Eq. 1 = 0.0; Note: This value must be between zero and unity
    dmgSCyc = 0.3;	//floating point cyclic backbone strength damage index; Note: This value must be between zero and unity

}


RPinchingLimitStateMaterial::~RPinchingLimitStateMaterial()
{
}


int RPinchingLimitStateMaterial::update(void)
{
    //    auto res = this->initialize();

    //    if(res!=0)
    //    {
    //        qCritical()<<"Error updating OpenSees 'Pinching4Material' in "<<this->objectName();
    //        return -1;
    //    }
    return 0;
}


//************ MATERIAL SPECIFIC CODE

UniaxialMaterial* RPinchingLimitStateMaterial::getUniaxialMaterial()
{

    qCritical()<<"This material still needs to be set up and tested!";


    if (theMaterial == nullptr)
    {
        this->createPinching4MaterialUniaxialMaterial();
    }


    return theMaterial;
}


int RPinchingLimitStateMaterial::createPinching4MaterialUniaxialMaterial(void)
{

    if(KelasParameter == nullptr)
    {
        qDebug()<<"The material "<<this->objectName()<<" needs a 'Kelastic' parameter";
        return -1;
    }


    if(theLimitCurve == nullptr)
    {
        qDebug()<<"The material "<<this->objectName()<<" needs a 'Limit Curve' assigned to it";
        return -1;
    }


    if(crvTyp == -1)
    {
        qDebug()<<"The material "<<this->objectName()<<" needs a 'Limit Curve Type' assigned to it";
        return -1;
    }

    if(driftAxis == -1)
    {
        qDebug()<<"The material "<<this->objectName()<<" needs a 'Drift Axis' assigned to it";
        return -1;
    }

    if(theOSDomain == nullptr)
        theOSDomain = theDomain->getTheOpenSeesDomain();

    theNodeT = theOSDomain->getNode(ndT);
    if (theNodeT == nullptr)
    {
        qDebug()<<"Error, getting OpenSees node in "<<__FUNCTION__;
        return -1;
    }

    theNodeB = theOSDomain->getNode(ndB);
    if (theNodeB == nullptr)
    {
        qDebug()<<"Error, getting OpenSees node in "<<__FUNCTION__;
        return -1;
    }


    auto theCurve = theLimitCurve->getTheLimitCurve();
    auto crvTag = theLimitCurve->getObjectTag();
    auto Kelas =  KelasParameter->getCurrentValue();

    auto dmgU1 = 0.0;
    auto dmgU2 = 0.0;
    auto dmgU3 = 0.0;
    auto dmgU4 = 0.0;
    auto dmgULim = 0.0;


    //     opserr << "WARNING PinchingLimitStateMaterial -- insufficient arguments\n";
    //     opserr << "For direct input of limit state material want:\n\n";
    //     opserr << "uniaxialMaterial PinchingLimitStateMaterial matTag?\n";
    //     opserr << "nodeT? nodeB? driftAxis? Kelas? crvTyp? crvTag?\n";
    //     opserr << "YpinchUPN? YpinchRPN? XpinchRPN?\n";
    //     opserr << "YpinchUNP? YpinchRNP? XpinchRNP?\n";
    //     opserr << "dmgStrsLimE? dmgDispMax?\n?";
    //     opserr << "dmgE1? dmgE2? dmgE3? dmgE4? dmgELim?\n";
    //     opserr << "dmgR1? dmgR2? dmgR3? dmgR4? dmgRLim? dmgRCyc?\n";
    //     opserr << "dmgS1? dmgS2? dmgS3? dmgS4? dmgSLim? dmgSCyc?\n" << endln;


    //    PinchingLimitStateMaterial(int matTag,
    //    int nodeT, int nodeB, int drftAx, double Kelas, int crvTyp, int crvTag,
    //    double YpinchUPN, double YpinchRPN,    double XpinchRPN,
    //    double YpinchUNP, double YpinchRNP,    double XpinchRNP,
    //    double dmgStrsLimE, double dmgDispMax,
    //    double dmgE1, double dmgE2, double dmgE3, double dmgE4, double dmgELim,
    //    double dmgU1, double dmgU2, double dmgU3, double dmgU4, double dmgULim,
    //    double dmgR1, double dmgR2, double dmgR3, double dmgR4,	double dmgRLim, double dmgRCyc,
    //    double dmgS1, double dmgS2, double dmgS3, double dmgS4, double dmgSLim, double dmgSCyc,
    //    int eleTag,	double b, double d, double h, double a, double st, double As, double Acc,
    //    double ld, double db, double rhot, double fc,double fy, double fyt,
    //    Domain *theDomain, Node *theNodeT, Node *theNodeB, LimitCurve &theCurve, Element *theElement);
    theMaterial = new PinchingLimitStateMaterial( objectTag,
                                                  ndT, ndB,  driftAxis,  Kelas,  crvTyp, crvTag,
                                                  YpinchUPN,  YpinchRPN,     XpinchRPN,
                                                  YpinchUNP,  YpinchRNP,     XpinchRNP,
                                                  dmgStrsLimE,  dmgDispMax,
                                                  dmgE1,  dmgE2,  dmgE3,  dmgE4,  dmgELim,
                                                  dmgU1,  dmgU2,  dmgU3,  dmgU4,  dmgULim,
                                                  dmgR1,  dmgR2,  dmgR3,  dmgR4,  dmgRLim,  dmgRCyc,
                                                  dmgS1,  dmgS2,  dmgS3,  dmgS4,  dmgSLim,  dmgSCyc,
                                                  0,	0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
                                                  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
                                                  theOSDomain,  theNodeT,  theNodeB,  *theCurve,  0);
}


int RPinchingLimitStateMaterial::getDriftAxis() const
{
    return driftAxis;
}


void RPinchingLimitStateMaterial::setDriftAxis(int value)
{
    driftAxis = value;
}


int RPinchingLimitStateMaterial::getLimitCurveType() const
{
    return crvTyp;
}


void RPinchingLimitStateMaterial::setLimitCurveType(int value)
{
    crvTyp = value;
}

int RPinchingLimitStateMaterial::getNdB() const
{
    return ndB;
}


void RPinchingLimitStateMaterial::setNdB(int value)
{
    ndB = value;
}


int RPinchingLimitStateMaterial::getNdT() const
{
    return ndT;
}


void RPinchingLimitStateMaterial::setNdT(int value)
{
    ndT = value;
}


RParameter *RPinchingLimitStateMaterial::getKelas() const
{
    return KelasParameter;
}


void RPinchingLimitStateMaterial::setKelas(RParameter *value)
{
    KelasParameter = value;
}


RLimitCurve *RPinchingLimitStateMaterial::getTheLimitCurve() const
{
    return theLimitCurve;
}


void RPinchingLimitStateMaterial::setTheLimitCurve(RLimitCurve *value)
{
    theLimitCurve = value;
}


//************ RTS STUFF FOR PLOTTING

void RPinchingLimitStateMaterial::outputMaterialProperties(void)
{
    //    qDebug()<<"The parameter vqalues in "<<this->objectName();
    //    qDebug()<<"fy (yield stress) : "<<fyParameter->getCurrentValue();
    //    qDebug()<<"E0 (initial stiffness): "<<E0Parameter->getCurrentValue();
    //    qDebug()<<"b (hardening ratio (Esh/E0)): "<<bParameter->getCurrentValue();
    //    qDebug()<<"R0 (exp transition elastic-plastic): "<<R0Parameter->getCurrentValue();
    //    qDebug()<<"R1 (coefficient for changing R0 to R): "<<cR1Parameter->getCurrentValue();
    //    qDebug()<<"cR2 (coefficient for changing R0 to R): "<<cR2Parameter->getCurrentValue();
    //    qDebug()<<"a1  (coefficient for isotropic hardening in compression): "<<a1Parameter->getCurrentValue();
    //    qDebug()<<"a2  (coefficient for isotropic hardening in compression): "<<a2Parameter->getCurrentValue();
    //    qDebug()<<"a3  (coefficient for isotropic hardening in tension): "<<a3Parameter->getCurrentValue();
    //    qDebug()<<"a4  (coefficient for isotropic hardening in tension): "<<a4Parameter->getCurrentValue();
    //    qDebug()<<"sigini (initial strain): "<<siginiParameter->getCurrentValue();
}


void RPinchingLimitStateMaterial::plotStressStrainBackbone(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    auto numPoints = 100;

    QVector<double> stresses;
    QVector<double> strains;

    auto rangeLow = -0.2;
    auto rangeHigh = 0.2;

    auto range = rangeHigh-rangeLow;
    auto step = range/numPoints;

    for(double i =rangeLow; i<=rangeHigh; i+=step)
        strains.push_back(i);

    for(auto&& it: strains)
    {
        theMaterial->setTrialStrain(it);
        // stresses.push_back( theMaterial->getStress()/fy);
        stresses.push_back( theMaterial->getStress());
    }


    //    qDebug()<<"Stress: ";
    //    for(int i =0; i<stresses.size();++i)
    //    {
    //        qDebug()<<stresses.at(i);
    //    }

    //    qDebug()<<"Strains: ";
    //    for(int i =0; i<strains.size();++i)
    //    {
    //        qDebug()<<strains.at(i);
    //    }


    this->outputMaterialProperties();

    QPointer<QCustomPlot> customPlot = nullptr;

    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    customPlot = new QCustomPlot();
    mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Backbone of Pinching4Material", true);

    // Create graph for the backbone
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
    customPlot->xAxis->setLabel("Strain");
    customPlot->yAxis->setLabel("Stress/fy");
    customPlot->yAxis->setLabelColor(QColor(0, 0, 255, 255));
    customPlot->graph(0)->addData(strains,stresses);
    customPlot->rescaleAxes();
    customPlot->replot();

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::plotStrainHistoryRunOne(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    //Run 1. Unidirectional incremental strain in tension.
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Unidirectional Incremental Strain in Tension";
    auto numSteps = 50;
    auto amplitudeHigh = 0.20;

    for(double i =0.0; i<=numSteps; ++i)
        stepHistory.push_back(i);

    for(double i =0; i<amplitudeHigh ; i+=amplitudeHigh/numSteps)
        strainHistory.push_back(i);

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back( theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::plotStrainHistoryRunTwo(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    //Run 2. Unidirectional incremental strain in compression only.
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Unidirectional incremental strain in compression only";
    auto numSteps = 50;
    auto amplitudeLow = -0.20;

    for(double i =0.0; i<=numSteps; ++i)
        stepHistory.push_back(i);

    for(double i = 0.0; i>amplitudeLow ; i+=amplitudeLow/numSteps)
        strainHistory.push_back(i);

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back( theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::plotStrainHistoryRunThree(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    //    Run 3. Three cycles of tension and compression to a low maximum-strain level (Tension first).
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Three Cycles of Tension and Compression to a Low Maximum-Strain Level +-0.005 (Tension first)";
    auto numSteps = 50;
    auto amplitude = 0.02;
    auto numCycles =2;

    for(double i =0.0; i<=numSteps*numCycles*2; ++i)
        stepHistory.push_back(i);

    for(int i = 0; i<=numCycles*2; ++i)
    {
        for(int j = 0; j<numSteps*2; ++j)
        {
            auto P = numSteps;
            strainHistory.push_back(((i%2)?-1.0:1.0)*(double)(amplitude/P) * (P - fabs(j % (2*P)- P) ));
        }
    }

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back( theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::plotStrainHistoryRunFour(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    //    Run 4. Three cycles of tension and compression to a low maximum-strain level (Tension first).
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Three cycles of compression and tension to a low maximum-strain level +-0.005 (Compression first).";
    auto numSteps = 50;
    auto amplitude = 0.02;
    auto numCycles =2;

    for(double i =0.0; i<=numSteps*numCycles*2; ++i)
        stepHistory.push_back(i);

    for(int i = 0; i<=numCycles*2; ++i)
    {
        for(int j = 0; j<numSteps*2; ++j)
        {
            auto P = numSteps;
            strainHistory.push_back(((i%2)?1.0:-1.0)*(double)(amplitude/P) * (P - fabs(j % (2*P)- P) ));
        }
    }

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back( theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::plotStrainHistoryRunFive(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    //    Run 5. Three cycles of tension and compression to a low maximum-strain level (Tension first).
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Three Cycles of Tension and Compression to a Low Maximum-Strain Level +-0.005 (Tension first)";
    auto numSteps = 50;
    auto amplitude = 0.2;
    auto numCycles =2;

    for(double i =0.0; i<=numSteps*numCycles*2; ++i)
        stepHistory.push_back(i);

    for(int i = 0; i<=numCycles*2; ++i)
    {
        for(int j = 0; j<numSteps*2; ++j)
        {
            auto P = numSteps;
            strainHistory.push_back(((i%2)?-1.0:1.0)*(double)(amplitude/P) * (P - fabs(j % (2*P)- P) ));
        }
    }

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back( theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::plotStrainHistoryRunSix(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    //    Run 6. Three cycles of tension and compression to a low maximum-strain level (Tension first).
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Three cycles of compression and tension to a low maximum-strain level +-0.005 (Compression first).";
    auto numSteps = 50;
    auto amplitude = 0.2;
    auto numCycles =2;

    for(double i =0.0; i<=numSteps*numCycles*2; ++i)
        stepHistory.push_back(i);

    for(int i = 0; i<=numCycles*2; ++i)
    {
        for(int j = 0; j<numSteps*2; ++j)
        {
            auto P = numSteps;
            strainHistory.push_back(((i%2)?1.0:-1.0)*(double)(amplitude/P) * (P - fabs(j % (2*P)- P) ));
        }
    }

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back( theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::plotStrainHistoryRunSeven(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    //Run 7. Numerous cycles of increasing amplitude (3 cycles per amplitude) to a high maximum-strain level in Tension only.
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Numerous cycles of increasing amplitude (3 cycles per amplitude) to a high maximum-strain level in Tension only.";
    auto numSteps = 50;
    auto numCycles =18;

    for(double i =0.0; i<=numSteps*numCycles*2; ++i)
        stepHistory.push_back(i);

    for(int i = 0; i<=numCycles*2; ++i)
    {
        auto amplitude = 0.0;
        if(i<3)
            amplitude = 0.0005;
        else if (i<6)
            amplitude = 0.001;
        else if (i<9)
            amplitude = 0.005;
        else if (i<12)
            amplitude = 0.05;
        else if (i<15)
            amplitude = 0.1;
        else if (i<18)
            amplitude = 0.15;
        else if (i<21)
            amplitude = 0.2;

        for(int j = 0; j<numSteps*2; ++j)
        {
            auto P = numSteps;
            strainHistory.push_back((double)(amplitude/P) * (P - fabs(j % (2*P)- P) ));
        }
    }

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back( theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::plotStrainHistoryRunEight(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    //    Run 8. Numerous cycles of increasing amplitude (3 cycles per amplitude) to a high maximum-strain level in Tension only.
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Numerous cycles of increasing amplitude (3 cycles per amplitude) to a high maximum-strain level in Compression only";
    auto numSteps = 50;
    auto numCycles =18;

    for(double i =0.0; i<=numSteps*numCycles*2; ++i)
        stepHistory.push_back(i);

    for(int i = 0; i<=numCycles*2; ++i)
    {
        auto amplitude = 0.0;
        if(i<3)
            amplitude = 0.0005;
        else if (i<6)
            amplitude = 0.001;
        else if (i<9)
            amplitude = 0.005;
        else if (i<12)
            amplitude = 0.05;
        else if (i<15)
            amplitude = 0.1;
        else if (i<18)
            amplitude = 0.15;
        else if (i<21)
            amplitude = 0.2;

        for(int j = 0; j<numSteps*2; ++j)
        {
            auto P = numSteps;
            strainHistory.push_back(-1.0*(double)(amplitude/P) * (P - fabs(j % (2*P)- P) ));
        }
    }

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back( theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::plotStrainHistoryRunNine(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    //    Run 9. Numerous cycles of increasing amplitude (3 cycles per amplitude) to a low maximum-strain level in Tension & Compression.
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Numerous Cycles of Increasing Amplitude (3 cycles per amplitude) to a High Maximum-strain Level in Tension & Compression";
    auto numSteps = 50;
    auto numCycles =8;

    for(double i =0.0; i<=numSteps*numCycles*2; ++i)
        stepHistory.push_back(i);

    for(int i = 0; i<=numCycles*2; ++i)
    {
        auto amplitude = 0.0;
        if(i<2)
            amplitude = 0.001;
        else if (i<4)
            amplitude = 0.005;
        else if (i<6)
            amplitude = 0.01;
        else if (i<8)
            amplitude = 0.02;

        for(int j = 0; j<numSteps*2; ++j)
        {
            auto P = numSteps;
            strainHistory.push_back(((i%2)?-1.0:1.0)*(double)(amplitude/P) * (P - fabs(j % (2*P)- P) ));
        }
    }


    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back( theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::plotStrainHistoryRunTen(void)
{
    if(theMaterial == nullptr)
        return;

    theMaterial->revertToStart();

    //    Run 10. Numerous cycles of increasing amplitude (3 cycles per amplitude) to a high maximum-strain level in Tension & Compression.
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Numerous Cycles of Increasing Amplitude (3 cycles per amplitude) to a High Maximum-strain Level in Tension & Compression";
    auto numSteps = 50;
    auto numCycles =14;

    for(double i =0.0; i<=numSteps*numCycles*2; ++i)
        stepHistory.push_back(i);

    for(int i = 0; i<=numCycles*2; ++i)
    {
        auto amplitude = 0.0;
        if(i<2)
            amplitude = 0.001;
        else if (i<4)
            amplitude = 0.005;
        else if (i<6)
            amplitude = 0.01;
        else if (i<8)
            amplitude = 0.02;
        else if (i<10)
            amplitude = 0.05;
        else if (i<12)
            amplitude = 0.1;
        else if (i<14)
            amplitude = 0.15;

        for(int j = 0; j<numSteps*2; ++j)
        {
            auto P = numSteps;
            strainHistory.push_back(((i%2)?-1.0:1.0)*(double)(amplitude/P) * (P - fabs(j % (2*P)- P) ));
        }
    }

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back( theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);

    theMaterial->revertToStart();
}


void RPinchingLimitStateMaterial::createPlots(const QVector<double> &stresses,const QVector<double> &strains,const QVector<double> &steps,const QString& description)
{
    QPointer<QCustomPlot> customPlot = nullptr;

    QVector<double> normalizedStress;

    for(auto&& it : stresses)
    {
        normalizedStress.push_back(it);
    }

    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    customPlot = new QCustomPlot();
    //mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Envelope of Pinching4Material"+ description, true);
    mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Envelope of Pinching4Material", true);

    auto newCurve = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(newCurve);

    // Create graph for the backbone
    customPlot->xAxis->setLabel("Strain");
    customPlot->yAxis->setLabel("Stress/fy");
    customPlot->yAxis->setLabelColor(QColor(0, 0, 255, 255));
    newCurve->setData(strains,normalizedStress);
    customPlot->rescaleAxes();
    customPlot->replot();

    QPointer<QCustomPlot> customPlot2 = nullptr;
    customPlot2 = new QCustomPlot();
    mainWindow->addSubWindow(customPlot2.data(), "Strain History -"+ description, true);

    // Create graph for the imposed strain and stress history
    customPlot2->addGraph();
    customPlot2->graph(0)->setPen(QColor(0, 0, 255, 255));
    customPlot2->xAxis->setLabel("Steps");
    customPlot2->yAxis->setLabel("Imposed Strain");
    customPlot2->yAxis->setLabelColor(QColor(0, 0, 255, 255));
    customPlot2->graph(0)->addData(steps,strains);
    customPlot2->rescaleAxes();
    customPlot2->replot();
}

