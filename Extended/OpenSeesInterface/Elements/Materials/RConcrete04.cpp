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

#include "RConcrete04.h"
#include "RMaterialKnowledge.h"
#include "RConcreteMaterialKnowledge.h"
#include "qcustomplot.h"
#include "RMainWindow.h"
#include "RParameter.h"

#include "Concrete04.h"

RConcrete04::RConcrete04(QObject *parent, QString &name)
    : RUniaxialMaterial(parent, name)
{
    fc  = nullptr;     // concrete compression strength
    epsc0  = nullptr;  // strain at compression strength
    fcu  = nullptr;    // stress at ultimate (crushing) strain
    epscu  = nullptr;  // ultimate (crushing) strain
    ft  = nullptr;     // concrete tensile strength
    Ets  = nullptr;    // tension stiffening slope
    beta = nullptr;    // value defining the exponential curve parameter to define the residual stress (as a factor of ft) at etu
}


RConcrete04::~RConcrete04(){}


int RConcrete04::update(void)
{
    auto res = this->initialize();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees 'Concrete04' in "<<this->objectName();
        theMaterial.reset();
        return -1;
    }
    return 0;
}


void RConcrete04::setMaterialKnowledge(RMaterialKnowledge *materialKnowledge)
{

    auto theConcreteMaterialKnowledge = dynamic_cast<RConcreteMaterialKnowledge*>(materialKnowledge);
    if(theConcreteMaterialKnowledge)
    {
        this->setDensity(theConcreteMaterialKnowledge->getDensity());
        this->setEpsc0(theConcreteMaterialKnowledge->getStrainAtfc());
        this->setEpscu(theConcreteMaterialKnowledge->getUltimateCrushingStrain());
        this->setEts(theConcreteMaterialKnowledge->getTensionStiffeningSlope());
        this->setfc(theConcreteMaterialKnowledge->getCompressiveStrength());
        this->setFcu(theConcreteMaterialKnowledge->getUltimateCrushingStress());
        this->setFt(theConcreteMaterialKnowledge->getTensileStrength());
        this->setBeta(theConcreteMaterialKnowledge->getBeta());
    }
    else
        qCritical()<<"Warning: an incompatible material "<<materialKnowledge->objectName()<<" was passed into "<<this->objectName();
}


void RConcrete04::setfc(RParameter* theParameter)
{
    //WARNING: compression should be negative in this model
    fc = theParameter;
}


void RConcrete04::setEpsc0(RParameter* theParameter)
{
    epsc0 = theParameter;
}


void RConcrete04::setFcu(RParameter* theParameter)
{
    fcu = theParameter;
}


void RConcrete04::setEpscu(RParameter* theParameter)
{
    epscu = theParameter;
}


void RConcrete04::setFt(RParameter* theParameter)
{
    ft = theParameter;
}


void RConcrete04::setEts(RParameter* theParameter)
{
    Ets = theParameter;
}


void RConcrete04::setDensity(RParameter* theParameter)
{
    theDensityParameter = theParameter;
}


double RConcrete04::getDensityValue()
{
    return theDensityParameter->getCurrentValue();
}


UniaxialMaterial* RConcrete04::getUniaxialMaterial()
{

    if (theMaterial == nullptr)
    {
        this->createConcrete04UniaxialMaterial();
    }

    return theMaterial.get();
}


int RConcrete04::createConcrete04UniaxialMaterial(void)
{

    if(fc == nullptr)
    {
        qDebug()<<"The material needs a 'fc' parameter in "<<__FUNCTION__;
        return -1;
    }
    if(epsc0 == nullptr)
    {
        qDebug()<<"The material needs a 'epsc0' parameter in "<<__FUNCTION__;
        return -1;
    }
    if(fcu == nullptr)
    {
        qDebug()<<"The material needs a 'fcu' in "<<__FUNCTION__;
        return -1;
    }
    if(epscu == nullptr)
    {
        qDebug()<<"The material needs a 'epscu' in "<<__FUNCTION__;
        return -1;
    }
    if(ft == nullptr)
    {
        qDebug()<<"The material needs a 'ft' in "<<__FUNCTION__;
        return -1;
    }
    if(Ets == nullptr)
    {
        qDebug()<<"The material needs a 'Ets' in "<<__FUNCTION__;
        return -1;
    }
    if(beta == nullptr)
    {
        qDebug()<<"The material needs a 'beta' in "<<__FUNCTION__;
        return -1;
    }

    return this->initialize();
}


void RConcrete04::setBeta(RParameter *value)
{
    beta = value;
}


int RConcrete04::initialize(void)
{
    auto _fc = -1.0*fc->getCurrentValue();
    auto _epsc0 =  -1.0*epsc0->getCurrentValue();
    auto _fcu = -1.0*fcu->getCurrentValue();
    auto _epscu =  -1.0*epscu->getCurrentValue();
    auto _ft = ft->getCurrentValue();
    auto _Ets = Ets->getCurrentValue();
    auto _beta = beta->getCurrentValue();

    //value defining ultimate tensile strain of concrete
    //Take it as the x-intercept point of concrete 02
    auto etu =    _ft*(_epsc0/(2.0 * _fc) + 1.0/_Ets);

    //Initial stiffness - taken from constructor of concrete 02
    auto Eco =  _fc * 2.0 / _epsc0;

    // Concrete04 (int tag, double fpc, double eco, double ecu, double Ec0, double fct, double etu, double beta);
    theMaterial = std::make_unique<Concrete04>(objectTag,_fc,_epsc0,_epscu,Eco,_ft,etu,_beta);

    if(theMaterial == nullptr)
        return -1;

    return 0;
}


//************ RTS STUFF FOR PLOTTING

void RConcrete04::outputMaterialProperties(void)
{
    auto _fc = -1.0*fc->getCurrentValue();
    auto _epsc0 =  -1.0*epsc0->getCurrentValue();
    auto _ft = ft->getCurrentValue();
    auto _Ets = Ets->getCurrentValue();

    //value defining ultimate tensile strain of concrete
    //Take it as the x-intercept point of concrete 02
    auto etu =    _ft*(_epsc0/(2.0 * _fc) + 1.0/_Ets);

    //Initial stiffness - taken from constructor of concrete 02
    auto Eco =  _fc * 2.0 / _epsc0;

    qDebug()<<"These are the fixed properties used in the Concrete04 material "<<this->objectName();
    qDebug()<<"fc: "<<fc->getCurrentValue()/1.0e6<<"MPa";
    qDebug()<<"ec: "<<epsc0->getCurrentValue();
    qDebug()<<"ecu: "<<epscu->getCurrentValue();\
    qDebug()<<"Ec: "<<Eco/1.0e6<<"MPa";
    qDebug()<<"fct: "<<ft->getCurrentValue()/1.0e6<<"MPa";
    qDebug()<<"et: "<<etu;
    qDebug()<<"beta: "<<beta->getCurrentValue();
}


void RConcrete04::plotStressStrainBackbone(void)
{
    this->createConcrete04UniaxialMaterial();

    this->outputMaterialProperties();

    theMaterial->revertToStart();

    auto numPoints = 100;

    QVector<double> stresses;
    QVector<double> strains;

    auto rangeLow = epscu->getCurrentValue()*-1.2;

    auto rangeHigh = 1.2*ft->getCurrentValue()*(epsc0->getCurrentValue()/(2.0 * fc->getCurrentValue()) + 1.0/Ets->getCurrentValue());

    auto range = rangeHigh-rangeLow;
    auto step = range/numPoints;

    for(double i =rangeLow; i<=rangeHigh; i+=step)
        strains.push_back(i);

    for(auto&& it: strains)
    {
        theMaterial->setTrialStrain(it);
        //stresses.push_back(theMaterial->getStress()/fabs(fc));
        stresses.push_back(theMaterial->getStress());
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

    QPointer<QCustomPlot> customPlot = nullptr;

    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    customPlot = new QCustomPlot();
    mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Backbone of Concrete04", true);

    // Create graph for the backbone
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
    customPlot->xAxis->setLabel("Strain");
    customPlot->yAxis->setLabel("Stress");
    customPlot->yAxis->setLabelColor(QColor(0, 0, 255, 255));
    customPlot->graph(0)->addData(strains,stresses);
    customPlot->rescaleAxes();
    customPlot->replot();
}


void RConcrete04::plotStrainHistoryRunOne(void)
{
    theMaterial->revertToStart();

    //Run 1. Unidirectional incremental strain in tension.
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Unidirectional Incremental Strain in Tension";
    auto numSteps = 50;
    auto amplitudeHigh = 0.020;

    for(double i =0.0; i<=numSteps; ++i)
        stepHistory.push_back(i);

    for(double i =0; i<amplitudeHigh ; i+=amplitudeHigh/numSteps)
        strainHistory.push_back(i);

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back(theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);
}


void RConcrete04::plotStrainHistoryRunTwo(void)
{
    theMaterial->revertToStart();

    //Run 2. Unidirectional incremental strain in compression only.
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Unidirectional incremental strain in compression only";
    auto numSteps = 50;
    auto amplitudeLow = -0.020;

    for(double i =0.0; i<=numSteps; ++i)
        stepHistory.push_back(i);

    for(double i = 0.0; i>amplitudeLow ; i+=amplitudeLow/numSteps)
        strainHistory.push_back(i);

    for(auto&& it: strainHistory)
    {
        theMaterial->setTrialStrain(it);
        theMaterial->commitState();
        stresses.push_back(theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);
}


void RConcrete04::plotStrainHistoryRunThree(void)
{
    theMaterial->revertToStart();

    //    Run 3. Three cycles of tension and compression to a low maximum-strain level (Tension first).
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Three Cycles of Tension and Compression to a Low Maximum-Strain Level +-0.005 (Tension first)";
    auto numSteps = 50;
    auto amplitude = 0.002;
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
        stresses.push_back(theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);
}


void RConcrete04::plotStrainHistoryRunFour(void)
{
    theMaterial->revertToStart();

    //    Run 4. Three cycles of tension and compression to a low maximum-strain level (Tension first).
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Three cycles of compression and tension to a low maximum-strain level +-0.005 (Compression first).";
    auto numSteps = 50;
    auto amplitude = 0.002;
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
        stresses.push_back(theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);
}


void RConcrete04::plotStrainHistoryRunFive(void)
{
    theMaterial->revertToStart();

    //    Run 5. Three cycles of tension and compression to a low maximum-strain level (Tension first).
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Three Cycles of Tension and Compression to a Low Maximum-Strain Level +-0.005 (Tension first)";
    auto numSteps = 50;
    auto amplitude = 0.05;
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
        stresses.push_back(theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);
}


void RConcrete04::plotStrainHistoryRunSix(void)
{
    theMaterial->revertToStart();

    //    Run 6. Three cycles of tension and compression to a low maximum-strain level (Tension first).
    QVector<double> strainHistory;
    QVector<double> stepHistory;
    QVector<double> stresses;

    QString description ="Three cycles of compression and tension to a low maximum-strain level +-0.005 (Compression first).";
    auto numSteps = 50;
    auto amplitude = 0.05;
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
        stresses.push_back(theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);
}


void RConcrete04::plotStrainHistoryRunSeven(void)
{
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
            amplitude = 0.00005;
        else if (i<6)
            amplitude = 0.00025;
        else if (i<9)
            amplitude = 0.0005;
        else if (i<12)
            amplitude = 0.0025;
        else if (i<15)
            amplitude = 0.005;
        else if (i<18)
            amplitude = 0.0075;
        else if (i<21)
            amplitude = 0.01;

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
        stresses.push_back(theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);
}


void RConcrete04::plotStrainHistoryRunEight(void)
{
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
            amplitude = 0.00005;
        else if (i<6)
            amplitude = 0.00025;
        else if (i<9)
            amplitude = 0.00075;
        else if (i<12)
            amplitude = 0.0025;
        else if (i<15)
            amplitude = 0.005;
        else if (i<18)
            amplitude = 0.0125;
        else if (i<21)
            amplitude = 0.02;

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
        stresses.push_back(theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);
}


void RConcrete04::plotStrainHistoryRunNine(void)
{
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
            amplitude = 0.00025;
        else if (i<4)
            amplitude = 0.00075;
        else if (i<6)
            amplitude = 0.00125;
        else if (i<8)
            amplitude = 0.002;

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
        stresses.push_back(theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);
}


void RConcrete04::plotStrainHistoryRunTen(void)
{
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
            amplitude = 0.00025;
        else if (i<4)
            amplitude = 0.00075;
        else if (i<6)
            amplitude = 0.00125;
        else if (i<8)
            amplitude = 0.002;
        else if (i<10)
            amplitude = 0.0075;
        else if (i<12)
            amplitude = 0.0125;
        else if (i<14)
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
        stresses.push_back(theMaterial->getStress());
    }

    this->outputMaterialProperties();

    createPlots(stresses,strainHistory, stepHistory,description);
}


void RConcrete04::createPlots(const QVector<double> &stresses,const QVector<double> &strains,const QVector<double> &steps,const QString& description)
{
    QPointer<QCustomPlot> customPlot = nullptr;

    QVector<double> normalizedStress;

    for(auto&& it : stresses)
    {
        // normalizedStress.push_back(it/fabs(fc));
        normalizedStress.push_back(it);

    }

    //    qDebug()<<"Stress: ";
    //    for(int i =0; i<normalizedStress.size();++i)
    //    {
    //        qDebug()<<normalizedStress.at(i);
    //    }

    //    qDebug()<<"Strains: ";
    //    for(int i =0; i<strains.size();++i)
    //    {
    //        qDebug()<<strains.at(i);
    //    }

    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    customPlot = new QCustomPlot();
    //mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Backbone of 1D Concrete04"+ description, true);
    mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Envelope for Concrete04", true);

    auto newCurve = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(newCurve);

    // Create graph for the backbone
    customPlot->xAxis->setLabel("Strain");
    customPlot->yAxis->setLabel("Stress");
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

