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

#include "RElasticBilin.h"
#include "RMaterialKnowledge.h"
#include "RSteelMaterialKnowledge.h"
#include "RConcreteMaterialKnowledge.h"
#include "RGlulamMaterialKnowledge.h"
#include "qcustomplot.h"
#include "RMainWindow.h"
#include "RParameter.h"

#include "ElasticBilin.h"

RElasticBilin::RElasticBilin(QObject *parent, QString &name)
    : RUniaxialMaterial(parent, name)
{
    EP1Parameter = nullptr;
    EP2Parameter  = nullptr;
    epsPParameter  = nullptr;
    EN1Parameter  = nullptr;
    EN2Parameter  = nullptr;
    epsNParameter  = nullptr;
    RhoParameter = nullptr;

    EP1   = 0.0;
    epsP  = 0.0;
    EN1   = 0.0;
    EN2   = 0.0;
    epsN = 0.0;
    EP2 = 0.0;
}


RElasticBilin::~RElasticBilin()
{
}


int RElasticBilin::update(void)
{
    auto res = this->initialize();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees 'ElasticBilin' in "<<this->objectName();
        theMaterial.reset();
        return -1;
    }
    return 0;
}


void RElasticBilin::setMaterialKnowledge(RMaterialKnowledge *materialKnowledge)
{
    auto concreteMaterial = qobject_cast<RConcreteMaterialKnowledge*>(materialKnowledge);
    auto glulamMaterial = qobject_cast<RGlulamMaterialKnowledge*>(materialKnowledge);

    if(concreteMaterial)
    {
        //Compression
        EP1Parameter  = concreteMaterial->getYoungsModulus();
        epsPParameter = concreteMaterial->getStrainAtfc();
        //        EP2Parameter = concreteMaterial->getTensionStiffeningSlope(); //If comment this out, then default will be zero
    }
    else if(glulamMaterial)
    {

        EP1Parameter = glulamMaterial->getYoungsModulus();
        epsPParameter = glulamMaterial->getUltimateTensileStrain();
        epsNParameter = glulamMaterial->getUltimateCompressiveStrain();

        EN2Parameter = glulamMaterial->getmEParameter();
        //        EP2Parameter = glulamMaterial->getmEParameter(); //Comment this out for a zero slope on the tensile side
    }

}


//************ MATERIAL SPECIFIC CODE

UniaxialMaterial* RElasticBilin::getUniaxialMaterial()
{
    if (theMaterial == nullptr)
    {
        this->createElasticBilinUniaxialMaterial();
    }

    return theMaterial.get();
}


int RElasticBilin::createElasticBilinUniaxialMaterial(void)
{

    if(EP1Parameter == nullptr)
    {
        qDebug()<<"The material needs a 'EP1Parameter' in "<<__FUNCTION__;
        return -1;
    }
    //    if(EP2Parameter == nullptr)
    //    {
    //        qDebug()<<"The material needs a 'EP2Parameter' in "<<__FUNCTION__;
    //        return -1;
    //    }
    if(epsPParameter == nullptr)
    {
        qDebug()<<"The material needs a 'epsParameter' in "<<__FUNCTION__;
        return -1;
    }

    return this->initialize();
}


int RElasticBilin::initialize(void)
{
    EP1 =  EP1Parameter->getCurrentValue();
    epsP = epsPParameter->getCurrentValue();

    if(EP2Parameter == nullptr)
        EP2 = 0.0;
    else
        EP2 = EP2Parameter->getCurrentValue();

    //Optional
    EN1   = 0.0;
    EN2   = 0.0;
    epsN = 0.0;

    if(EN1Parameter == nullptr)
        EN1 = EP1;
    else
        EN1 = EN1Parameter->getCurrentValue();

    if(EN2Parameter == nullptr)
        EN2 = EP2;
    else
        EN2 = EN2Parameter->getCurrentValue();

    if(epsNParameter == nullptr)
        epsN = -epsP;
    else
        epsN = epsNParameter->getCurrentValue();


    //    ElasticBilin(int tag, double E1, double E2, double eps2);
    //    ElasticBilin(int tag, double E1P, double E2P, double epsP, double E1N, double E2N, double eps2N);
    theMaterial = std::make_unique<ElasticBilin>(objectTag,EP1,EP2,epsP,EN1,EN2,epsN);

    if(theMaterial == nullptr)
        return -1;

    return 0;
}


//************ RTS STUFF FOR PLOTTING

void RElasticBilin::outputMaterialProperties(void)
{
    qDebug()<<"The parameter values in "<<this->objectName();

    qDebug()<<"EP1 : " <<EP1;
    qDebug()<<"epsP : "<<epsP;
    qDebug()<<"EP2 : " <<EP2;

    qDebug()<<"EN1 : " <<EN1;
    qDebug()<<"epsN : "<<epsN;
    qDebug()<<"EN2 : " <<EN2;
}


int RElasticBilin::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(4);

    auto tag = std::to_string(this->objectTag);

    std::string sEP1    = std::to_string(EP1);
    std::string sepsP   = std::to_string(epsP);
    std::string sEP2    = std::to_string(EP2);
    std::string sEN1    = std::to_string(EN1);
    std::string sepsN   = std::to_string(epsN);
    std::string sEN2    = std::to_string(EN2);

    if(flag == 1) //tcl file
    {
        //        out<<"# uniaxialMaterial ElasticBilin $matTag $EP1 $EP2 $epsP2 <$EN1 $EN2 $epsN2>"<<std::endl;
        out<<"uniaxialMaterial ElasticBilin "<<tag<<" "<<sEP1<<" "<<sEP2<<" "<<sepsP<<" "<<sEN1<<" "<<sEN2<<" "<<sepsN<<std::endl;
    }
    else if(flag == 2) //Python
    {
        //        out<<"# uniaxialMaterial('ElasticBilin', matTag, EP1, EP2, epsP2, EN1=EP1, EN2=EP2, epsN2=-epsP2)"<<std::endl;
        out<<"uniaxialMaterial('ElasticBilin', "<<tag<<", "<<sEP1<<", "<<sEP2<<", "<<sepsP<<", "<<sEN1<<", "<<sEN2<<", "<<sepsN<<std::endl;
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


void RElasticBilin::plotStressStrainBackbone(void)
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
    mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Backbone of ElasticBilin", true);

    // Create graph for the backbone
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
    customPlot->xAxis->setLabel("Strain");
    customPlot->yAxis->setLabel("Stress");
    customPlot->yAxis->setLabelColor(QColor(0, 0, 255, 255));
    customPlot->graph(0)->addData(strains,stresses);
    customPlot->rescaleAxes();
    customPlot->replot();

    theMaterial->revertToStart();
}


void RElasticBilin::plotStrainHistoryRunOne(void)
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


void RElasticBilin::plotStrainHistoryRunTwo(void)
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


void RElasticBilin::plotStrainHistoryRunThree(void)
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


void RElasticBilin::plotStrainHistoryRunFour(void)
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


void RElasticBilin::plotStrainHistoryRunFive(void)
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


void RElasticBilin::plotStrainHistoryRunSix(void)
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


void RElasticBilin::plotStrainHistoryRunSeven(void)
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


void RElasticBilin::plotStrainHistoryRunEight(void)
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


void RElasticBilin::plotStrainHistoryRunNine(void)
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


void RElasticBilin::plotStrainHistoryRunTen(void)
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


RParameter *RElasticBilin::getRhoParameter() const
{
    return RhoParameter;
}


void RElasticBilin::setRhoParameter(RParameter *value)
{
    RhoParameter = value;
}


RParameter *RElasticBilin::getEpsN2Parameter() const
{
    return epsNParameter;
}


void RElasticBilin::setEpsN2Parameter(RParameter *value)
{
    epsNParameter = value;
}


RParameter *RElasticBilin::getEN2Parameter() const
{
    return EN2Parameter;
}


void RElasticBilin::setEN2Parameter(RParameter *value)
{
    EN2Parameter = value;
}


RParameter *RElasticBilin::getEN1Parameter() const
{
    return EN1Parameter;
}


void RElasticBilin::setEN1Parameter(RParameter *value)
{
    EN1Parameter = value;
}


RParameter *RElasticBilin::getEpsP2Parameter() const
{
    return epsPParameter;
}


void RElasticBilin::setEpsP2Parameter(RParameter *value)
{
    epsPParameter = value;
}


RParameter *RElasticBilin::getEP2Parameter() const
{
    return EP2Parameter;
}


void RElasticBilin::setEP2Parameter(RParameter *value)
{
    EP2Parameter = value;
}


RParameter *RElasticBilin::getEP1Parameter() const
{
    return EP1Parameter;
}


void RElasticBilin::setEP1Parameter(RParameter *value)
{
    EP1Parameter = value;
}


void RElasticBilin::createPlots(const QVector<double> &stresses,const QVector<double> &strains,const QVector<double> &steps,const QString& description)
{
    QPointer<QCustomPlot> customPlot = nullptr;

    QVector<double> normalizedStress;

    for(auto&& it : stresses)
    {
        normalizedStress.push_back(it);
    }

    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    customPlot = new QCustomPlot();
    //mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Envelope of ElasticBilin"+ description, true);
    mainWindow->addSubWindow(customPlot.data(), "Stress-Strain Envelope of ElasticBilin", true);

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

