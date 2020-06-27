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

#include "RSteel02.h"
#include "RMaterialKnowledge.h"
#include "RSteelMaterialKnowledge.h"
#include "qcustomplot.h"
#include "RMainWindow.h"
#include "RParameter.h"

#include "Steel02.h"

RSteel02::RSteel02(QObject *parent, QString &name)
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


RSteel02::~RSteel02()
{
}


int RSteel02::update(void)
{
    auto res = this->initialize();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees 'Steel02' in "<<this->objectName();
        theMaterial.reset();
        return -1;
    }
    return 0;
}


void RSteel02::setMaterialKnowledge(RMaterialKnowledge *materialKnowledge)
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


RParameter* RSteel02::getFyParameter() const
{
    return fyParameter;
}


void RSteel02::setFyParameter(RParameter* value)
{
    fyParameter = value;
}


RParameter* RSteel02::getSiginiParameter() const
{
    return siginiParameter;
}


void RSteel02::setSiginiParameter(RParameter* value)
{
    siginiParameter = value;
}


RParameter* RSteel02::getCR2Parameter() const
{
    return cR2Parameter;
}


void RSteel02::setCR2Parameter(RParameter* value)
{
    cR2Parameter = value;
}


RParameter* RSteel02::getCR1Parameter() const
{
    return cR1Parameter;
}


void RSteel02::setCR1Parameter(RParameter* value)
{
    cR1Parameter = value;
}


RParameter* RSteel02::getR0Parameter() const
{
    return R0Parameter;
}

void RSteel02::setR0Parameter(RParameter* value)
{
    R0Parameter = value;
}


RParameter* RSteel02::getA4Parameter() const
{
    return a4Parameter;
}


void RSteel02::setA4Parameter(RParameter* value)
{
    a4Parameter = value;
}

RParameter* RSteel02::getA3Parameter() const
{
    return a3Parameter;
}


void RSteel02::setA3Parameter(RParameter* value)
{
    a3Parameter = value;
}


RParameter* RSteel02::getA2Parameter() const
{
    return a2Parameter;
}


void RSteel02::setA2Parameter(RParameter* value)
{
    a2Parameter = value;
}


RParameter* RSteel02::getA1Parameter() const
{
    return a1Parameter;
}


void RSteel02::setA1Parameter(RParameter* value)
{
    a1Parameter = value;
}

RParameter* RSteel02::getRhoParameter() const
{
    return RhoParameter;
}


void RSteel02::setRhoParameter(RParameter* value)
{
    RhoParameter = value;
}


RParameter* RSteel02::getBParameter() const
{
    return bParameter;
}

void RSteel02::setBParameter(RParameter* value)
{
    bParameter = value;
}


RParameter* RSteel02::getE0Parameter() const
{
    return E0Parameter;
}


void RSteel02::setE0Parameter(RParameter* value)
{
    E0Parameter = value;
}


//************ MATERIAL SPECIFIC CODE

UniaxialMaterial* RSteel02::getUniaxialMaterial()
{
    if (theMaterial == nullptr)
    {
        this->createSteel02UniaxialMaterial();
    }

    return theMaterial.get();
}


int RSteel02::createSteel02UniaxialMaterial(void)
{

    if(fyParameter == nullptr)
    {
        qDebug()<<"The material needs a 'fyParameter' parameter in "<<__FUNCTION__;
        return -1;
    }
    if(E0Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'E0Parameter' parameter in "<<__FUNCTION__;
        return -1;
    }
    if(bParameter == nullptr)
    {
        qDebug()<<"The material needs a 'bParameter' in "<<__FUNCTION__;
        return -1;
    }
    if(RhoParameter == nullptr)
    {
        qDebug()<<"The material needs a 'RhoParameter' in "<<__FUNCTION__;
        return -1;
    }
    if(a1Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'a1Parameter' in "<<__FUNCTION__;
        return -1;
    }
    if(a2Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'a2Parameter' in "<<__FUNCTION__;
        return -1;
    }
    if(a3Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'a3Parameter' in "<<__FUNCTION__;
        return -1;
    }
    if(a4Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'a4Parameter' in "<<__FUNCTION__;
        return -1;
    }
    if(cR1Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'cR1Parameter' in "<<__FUNCTION__;
        return -1;
    }
    if(cR2Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'cR2Parameter' in "<<__FUNCTION__;
        return -1;
    }

    return this->initialize();
}


int RSteel02::initialize(void)
{
    auto fy =  fyParameter->getCurrentValue();
    auto E0 =  E0Parameter->getCurrentValue();
    auto b  =  bParameter->getCurrentValue();
    auto R0 =  R0Parameter->getCurrentValue();
    auto cR1 = cR1Parameter->getCurrentValue();
    auto cR2 = cR2Parameter->getCurrentValue();
    auto a1 =  a1Parameter->getCurrentValue();
    auto a2 =  a2Parameter->getCurrentValue();
    auto a3 =  a3Parameter->getCurrentValue();
    auto a4 =  a4Parameter->getCurrentValue();
    auto sigInit = siginiParameter->getCurrentValue();

    // steel02(int tag,
    // double fy, double E0, double b,
    // double R0, double cR1, double cR2,
    // double a1, double a2, double a3, double a4, double sigInit =0.0);
    theMaterial = std::make_unique<Steel02>(objectTag,fy,E0,b,R0,cR1,cR2,a1 ,a2,a3,a4,sigInit);

    // Constructor for no isotropic hardening
    // Steel02(int tag,
    // double fy, double E0, double b,
    // double R0, double cR1, double cR2);
    // theMaterial = std::make_unique<Steel02>(objectTag,fy,E0,b,R0,cR1,cR2);

    if(theMaterial == nullptr)
        return -1;

    return 0;
}


int RSteel02::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(2);

    auto tag = std::to_string(this->objectTag);

    auto fy =  std::to_string(fyParameter->getCurrentValue());
    auto E0 =  std::to_string(E0Parameter->getCurrentValue());
    auto b  =  std::to_string(bParameter->getCurrentValue());
    auto R0 =  std::to_string(R0Parameter->getCurrentValue());
    auto cR1 = std::to_string(cR1Parameter->getCurrentValue());
    auto cR2 = std::to_string(cR2Parameter->getCurrentValue());
    auto a1 =  std::to_string(a1Parameter->getCurrentValue());
    auto a2 =  std::to_string(a2Parameter->getCurrentValue());
    auto a3 =  std::to_string(a3Parameter->getCurrentValue());
    auto a4 =  std::to_string(a4Parameter->getCurrentValue());
    auto sig0 =std::to_string(siginiParameter->getCurrentValue());

    if(flag == 1) //tcl file
    {
        //        out<<"# uniaxialMaterial Steel02 $matTag $Fy $E $b $R0 $cR1 $cR2 <$a1 $a2 $a3 $a4 $sigInit>"<<std::endl;
        out<<"uniaxialMaterial Steel02 "<<tag<<" "<<fy<<" "<<E0<<" "<<b<<" "<<R0<<" "<<cR1<<" "<<cR2<<" "<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<" "<<sig0<<std::endl;
    }
    else if(flag == 2) //Python
    {
        //        out<<"# uniaxialMaterial('Steel02', matTag, Fy, E0, b, *params, a1=a2*Fy/E0, a2=1.0, a3=a4*Fy/E0, a4=1.0, sigInit=0.0)"<<std::endl;
        out<<"uniaxialMaterial('Steel02', "<<tag<<", "<<fy<<", "<<E0<<", "<<b<<", "<<R0<<", "<<cR1<<", "<<cR2<<", "<<a1<<", "<<a2<<", "<<a3<<", "<<a4<<", "<<sig0<<")"<<std::endl;
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


//************ RTS STUFF FOR PLOTTING

void RSteel02::outputMaterialProperties(void)
{
    qDebug()<<"The parameter values in "<<this->objectName();
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


void RSteel02::plotStressStrainBackbone(void)
{

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
    mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Backbone of steel02", true);

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


void RSteel02::plotStrainHistoryRunOne(void)
{

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


void RSteel02::plotStrainHistoryRunTwo(void)
{
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


void RSteel02::plotStrainHistoryRunThree(void)
{

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


void RSteel02::plotStrainHistoryRunFour(void)
{

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


void RSteel02::plotStrainHistoryRunFive(void)
{

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


void RSteel02::plotStrainHistoryRunSix(void)
{

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


void RSteel02::plotStrainHistoryRunSeven(void)
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


void RSteel02::plotStrainHistoryRunEight(void)
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


void RSteel02::plotStrainHistoryRunNine(void)
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


void RSteel02::plotStrainHistoryRunTen(void)
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


void RSteel02::createPlots(const QVector<double> &stresses,const QVector<double> &strains,const QVector<double> &steps,const QString& description)
{
    QPointer<QCustomPlot> customPlot = nullptr;

    QVector<double> normalizedStress;

    for(auto&& it : stresses)
    {
        normalizedStress.push_back(it/fyParameter->getCurrentValue());
    }

    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    customPlot = new QCustomPlot();
    //mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Envelope of steel02"+ description, true);
    mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Envelope of steel02", true);

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

