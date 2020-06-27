#ifndef RConcrete04_H
#define RConcrete04_H
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

//Wrapper class for OpenSees Concrete04

#include "RUniaxialMaterial.h"

class RMaterialKnowledge;
class RParameter;
class RMainWindow;
class QCustomPlot;

class RConcrete04 : public RUniaxialMaterial
{
    Q_OBJECT

public:
    RConcrete04(QObject *parent, QString &name);
    ~RConcrete04();

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

    // concrete tensile strength
    void setFt(RParameter* theParameter);

    // tension stiffening slope
    void setEts(RParameter* theParameter);

    // the concrete density
    void setDensity(RParameter* theParameter);
    double getDensityValue();

    // value defining the exponential curve parameter to define the residual stress (as a factor of ft) at etu
    void setBeta(RParameter *value);

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

    int createConcrete04UniaxialMaterial(void);

    //Concrete FIXED PROPERTIES
    RParameter*  fc;      // concrete compression strength
    RParameter*  epsc0;   // strain at compression strength
    RParameter*  fcu;     // stress at ultimate (crushing) strain
    RParameter*  epscu;   // ultimate (crushing) strain
    RParameter*  ft;      // concrete tensile strength
    RParameter*  Ets;     // tension stiffening slope
    RParameter*  beta;    // value defining the exponential curve parameter to define the residual stress (as a factor of ft) at etu

    //Rts stuff
    QPointer<RParameter> theDensityParameter;
    void createPlots(const QVector<double> &stresses,const QVector<double> &strains,const QVector<double> &steps,const QString& description);
    void outputMaterialProperties(void);

    int initialize(void);

};

#endif








