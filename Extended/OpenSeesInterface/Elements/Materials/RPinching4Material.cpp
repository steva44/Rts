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

#include "RPinching4Material.h"
#include "RMaterialKnowledge.h"
#include "RSteelMaterialKnowledge.h"
#include "qcustomplot.h"
#include "RMainWindow.h"
#include "RParameter.h"

#include "Pinching4Material.h"

RPinching4Material::RPinching4Material(QObject *parent, QString &name)
    : RUniaxialMaterial(parent, name)
{
    theSteelMaterial = nullptr;

    fyParameter = nullptr;
    E0Parameter = nullptr;
    bParameter = nullptr;
    RhoParameter = nullptr;

    a1Parameter = nullptr;
    a2Parameter = nullptr;
    a3Parameter = nullptr;
    a4Parameter = nullptr;

    R0Parameter = nullptr;
    cR1Parameter = nullptr;
    cR2Parameter = nullptr;
    siginiParameter = nullptr;
}


RPinching4Material::~RPinching4Material()
{
}


int RPinching4Material::update(void)
{
    //    auto res = this->initialize();

    //    if(res!=0)
    //    {
    //        qCritical()<<"Error updating OpenSees 'Pinching4Material' in "<<this->objectName();
    //        return -1;
    //    }
    return 0;
}


void RPinching4Material::setMaterialKnowledge(RMaterialKnowledge *materialKnowledge)
{
    auto theSteelKnowledge= dynamic_cast<RSteelMaterialKnowledge*>(materialKnowledge);
    if(theSteelKnowledge)
    {
        E0Parameter  = theSteelKnowledge->getYoungsModulus();
        fyParameter  = theSteelKnowledge->getYieldStress();
        bParameter  = theSteelKnowledge->getb();
        R0Parameter  = theSteelKnowledge->getRo();
        cR1Parameter = theSteelKnowledge->getcR1();
        cR2Parameter = theSteelKnowledge->getcR2();
        a1Parameter  = theSteelKnowledge->geta1();
        a2Parameter  = theSteelKnowledge->geta2();
        a3Parameter  = theSteelKnowledge->geta3();
        a4Parameter  = theSteelKnowledge->geta4();
        siginiParameter = theSteelKnowledge->getsiginit();
        RhoParameter = theSteelKnowledge->getDensity();
    }
    else
        qCritical()<<"Warning: an incompatible material "<<materialKnowledge->objectName()<<" was passed into "<<this->objectName();
}


RParameter* RPinching4Material::getFyParameter() const
{
    return fyParameter;
}


void RPinching4Material::setFyParameter(RParameter* value)
{
    fyParameter = value;
}


RParameter* RPinching4Material::getSiginiParameter() const
{
    return siginiParameter;
}


void RPinching4Material::setSiginiParameter(RParameter* value)
{
    siginiParameter = value;
}


RParameter* RPinching4Material::getCR2Parameter() const
{
    return cR2Parameter;
}


void RPinching4Material::setCR2Parameter(RParameter* value)
{
    cR2Parameter = value;
}


RParameter* RPinching4Material::getCR1Parameter() const
{
    return cR1Parameter;
}


void RPinching4Material::setCR1Parameter(RParameter* value)
{
    cR1Parameter = value;
}

RParameter* RPinching4Material::getR0Parameter() const
{
    return R0Parameter;
}


void RPinching4Material::setR0Parameter(RParameter* value)
{
    R0Parameter = value;
}

RParameter* RPinching4Material::getA4Parameter() const
{
    return a4Parameter;
}


void RPinching4Material::setA4Parameter(RParameter* value)
{
    a4Parameter = value;
}

RParameter* RPinching4Material::getA3Parameter() const
{
    return a3Parameter;
}


void RPinching4Material::setA3Parameter(RParameter* value)
{
    a3Parameter = value;
}


RParameter* RPinching4Material::getA2Parameter() const
{
    return a2Parameter;
}

void RPinching4Material::setA2Parameter(RParameter* value)
{
    a2Parameter = value;
}


RParameter* RPinching4Material::getA1Parameter() const
{
    return a1Parameter;
}


void RPinching4Material::setA1Parameter(RParameter* value)
{
    a1Parameter = value;
}

RParameter* RPinching4Material::getRhoParameter() const
{
    return RhoParameter;
}

void RPinching4Material::setRhoParameter(RParameter* value)
{
    RhoParameter = value;
}


RParameter* RPinching4Material::getBParameter() const
{
    return bParameter;
}

void RPinching4Material::setBParameter(RParameter* value)
{
    bParameter = value;
}


RParameter* RPinching4Material::getE0Parameter() const
{
    return E0Parameter;
}


void RPinching4Material::setE0Parameter(RParameter* value)
{
    E0Parameter = value;
}

//************ MATERIAL SPECIFIC CODE

UniaxialMaterial* RPinching4Material::getUniaxialMaterial()
{

    qCritical()<<"This material still needs to be set up and tested!";


    if (theMaterial == nullptr)
    {
        this->createPinching4MaterialUniaxialMaterial();
    }


    return theMaterial.get();
}


void RPinching4Material::createPinching4MaterialUniaxialMaterial(void)
{

    if(fyParameter == nullptr)
    {
        qDebug()<<"The material needs a 'fyParameter' parameter in "<<__FUNCTION__;
        return;
    }
    if(E0Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'E0Parameter' parameter in "<<__FUNCTION__;
        return;
    }
    if(bParameter == nullptr)
    {
        qDebug()<<"The material needs a 'bParameter' in "<<__FUNCTION__;
        return;
    }
    if(RhoParameter == nullptr)
    {
        qDebug()<<"The material needs a 'RhoParameter' in "<<__FUNCTION__;
        return;
    }
    if(a1Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'a1Parameter' in "<<__FUNCTION__;
        return;
    }
    if(a2Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'a2Parameter' in "<<__FUNCTION__;
        return;
    }
    if(a3Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'a3Parameter' in "<<__FUNCTION__;
        return;
    }
    if(a4Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'a4Parameter' in "<<__FUNCTION__;
        return;
    }
    if(cR1Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'cR1Parameter' in "<<__FUNCTION__;
        return;
    }
    if(cR2Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'cR2Parameter' in "<<__FUNCTION__;
        return;
    }

    double fy =  fyParameter->getCurrentValue();
    double E0 =  E0Parameter->getCurrentValue();
    double b  =  bParameter->getCurrentValue();
    double R0 =  R0Parameter->getCurrentValue();
    double cR1 = cR1Parameter->getCurrentValue();
    double cR2 = cR2Parameter->getCurrentValue();
    double a1 =  a1Parameter->getCurrentValue();
    double a2 =  a2Parameter->getCurrentValue();
    double a3 =  a3Parameter->getCurrentValue();
    double a4 =  a4Parameter->getCurrentValue();
    double sigInit = siginiParameter->getCurrentValue();

    qCritical()<<"This material still needs to be set up and tested!";

    //    Pinching4Material(int tag,
    //		double stress1p, double strain1p, double stress2p, double strain2p,
    //		double stress3p, double strain3p, double stress4p, double strain4p,
    //		double stress1n, double strain1n, double stress2n, double strain2n,
    //		double stress3n, double strain3n, double stress4n, double strain4n,
    //		double rDispP, double rForceP, double uForceP,
    //		double rDispN, double rForceN, double uForceN,
    //		double gammaK1, double gammaK2, double gammaK3,
    //		double gammaK4, double gammaKLimit,
    //		double gammaD1, double gammaD2, double gammaD3,
    //		double gammaD4, double gammaDLimit,
    //		double gammaF1, double gammaF2, double gammaF3,
    //		double gammaF4, double gammaFLimit, double gammaE, int DmgCyc);

    //	Pinching4Material(int tag,
    //		double stress1p, double strain1p, double stress2p, double strain2p,
    //		double stress3p, double strain3p, double stress4p, double strain4p,
    //		double rDispP, double rForceP, double uForceP,
    //		double gammaK1, double gammaK2, double gammaK3,
    //		double gammaK4, double gammaKLimit,
    //		double gammaD1, double gammaD2, double gammaD3,
    //		double gammaD4, double gammaDLimit,
    //		double gammaF1, double gammaF2, double gammaF3,
    //		double gammaF4, double gammaFLimit, double gammaE, int DmgCyc);
    //    theMaterial = new Pinching4Material(objectTag,fy,E0,b,R0,cR1,cR2,a1 ,a2,a3,a4,sigInit);
}

//************ RTS STUFF FOR PLOTTING

void RPinching4Material::outputMaterialProperties(void)
{
    qDebug()<<"The parameter vqalues in "<<this->objectName();
    qDebug()<<"fy (yield stress) : "<<fyParameter->getCurrentValue();
    qDebug()<<"E0 (initial stiffness): "<<E0Parameter->getCurrentValue();
    qDebug()<<"b (hardening ratio (Esh/E0)): "<<bParameter->getCurrentValue();
    qDebug()<<"R0 (exp transition elastic-plastic): "<<R0Parameter->getCurrentValue();
    qDebug()<<"R1 (coefficient for changing R0 to R): "<<cR1Parameter->getCurrentValue();
    qDebug()<<"cR2 (coefficient for changing R0 to R): "<<cR2Parameter->getCurrentValue();
    qDebug()<<"a1  (coefficient for isotropic hardening in compression): "<<a1Parameter->getCurrentValue();
    qDebug()<<"a2  (coefficient for isotropic hardening in compression): "<<a2Parameter->getCurrentValue();
    qDebug()<<"a3  (coefficient for isotropic hardening in tension): "<<a3Parameter->getCurrentValue();
    qDebug()<<"a4  (coefficient for isotropic hardening in tension): "<<a4Parameter->getCurrentValue();
    qDebug()<<"sigini (initial strain): "<<siginiParameter->getCurrentValue();
}

void RPinching4Material::createTestMaterialParameters(void)
{
    // Create knowledge for the concrete material
    auto theSteelMaterial = std::make_unique<RSteelMaterialKnowledge>(theDomain, this->objectName()+"Pinching4Material", "A36");

    this->setMaterialKnowledge(theSteelMaterial.get());
}

void RPinching4Material::plotStressStrainBackbone(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::plotStrainHistoryRunOne(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::plotStrainHistoryRunTwo(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::plotStrainHistoryRunThree(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::plotStrainHistoryRunFour(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::plotStrainHistoryRunFive(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::plotStrainHistoryRunSix(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::plotStrainHistoryRunSeven(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::plotStrainHistoryRunEight(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::plotStrainHistoryRunNine(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::plotStrainHistoryRunTen(void)
{
    if(theSteelMaterial == nullptr)
        this->createTestMaterialParameters();

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

void RPinching4Material::createPlots(const QVector<double> &stresses,const QVector<double> &strains,const QVector<double> &steps,const QString& description)
{
    QPointer<QCustomPlot> customPlot = nullptr;

    QVector<double> normalizedStress;

    for(auto&& it : stresses)
    {
        normalizedStress.push_back(it/fyParameter->getCurrentValue());
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

