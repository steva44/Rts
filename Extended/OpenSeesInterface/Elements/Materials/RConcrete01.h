#ifndef RConcrete01_H
#define RConcrete01_H
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

//Wrapper class for OpenSees Concrete01
//Stevan Gavrilovic 2/2019

#include "RUniaxialMaterial.h"

class RMaterialKnowledge;
class RParameter;
class RMainWindow;
class QCustomPlot;

class RConcrete01 : public RUniaxialMaterial
{
    Q_OBJECT

public:
    RConcrete01(QObject *parent, QString &name);
    ~RConcrete01();

    void setMaterialKnowledge(RMaterialKnowledge *materialKnowledge);
    UniaxialMaterial* getUniaxialMaterial();
    int update(void);

    //PROPERTIES SET BY THE USER
    // concrete compression strength
    void setfc(RParameter* theParameter);

    // strain at compression strength
    void setEpsc0(RParameter* theParameter);

    // stress at ultimate (crushing) strain
    void setFcu(RParameter* theParameter);

    // ultimate (crushing) strain
    void setEpscu(RParameter* theParameter);

    // the concrete density
    void setDensity(RParameter* theParameter);
    double getDensityValue();

protected slots:

    void plotStressStrainBackbone(void);
    void plotStrainHistoryRunOne(void);
    void plotStrainHistoryRunTwo(void);
    void plotStrainHistoryRunThree(void);
    void plotStrainHistoryRunFour(void);
    void plotStrainHistoryRunFive(void);
    void plotStrainHistoryRunSix(void);
    void plotStrainHistoryRunSeven(void);
    void plotStrainHistoryRunEight(void);
    void plotStrainHistoryRunNine(void);
    void plotStrainHistoryRunTen(void);

private:

    int createConcrete01UniaxialMaterial(void);

    //Concrete FIXED PROPERTIES
    RParameter*  fpc;      // concrete compression strength
    RParameter*  epsc0;   // strain at compression strength
    RParameter*  fpcu;     // stress at ultimate (crushing) strain
    RParameter*  epsu;   // ultimate (crushing) strain

    //Rts stuff
    QPointer<RParameter> theDensityParameter;
    void createPlots(const QVector<double> &stresses,const QVector<double> &strains,const QVector<double> &steps,const QString& description);
    void outputMaterialProperties(void);

    int initialize(void);

};

#endif








