#ifndef RElasticBilin_H
#define RElasticBilin_H
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

// Wrapper class for OpenSees Steel02
// Stevan Gavrilovic 1/2018
//     ElasticBilin(int tag, double E1, double E2, double eps2);
//     ElasticBilin(int tag, double E1P, double E2P, double epsP, double E1N, double E2N, double eps2N);

// $matTag	integer tag identifying material
// $EP1	tangent in tension for stains: 0 <= strains <= $epsP2
// $EP2	tangent when material in tension with strains > $epsP2
// $epsP2	strain at which material changes tangent in tension.
// $EN1	optional, default = $EP1. tangent in compression for stains: 0 < strains <= $epsN2
// $EN2	optional, default = $EP2. tangent in compression with strains < $epsN2
// $epsN2	optional, default = -epsP2. strain at which material changes tangent in compression.


#include "RUniaxialMaterial.h"

class RMaterialKnowledge;
class RParameter;
class RMainWindow;
class QCustomPlot;

class RElasticBilin : public RUniaxialMaterial
{
    Q_OBJECT

public:
    RElasticBilin(QObject *parent, QString &name);
    ~RElasticBilin();

    UniaxialMaterial* getUniaxialMaterial();
    int update(void);

    void setMaterialKnowledge(RMaterialKnowledge *materialKnowledge);

    //PROPERTIES SET BY THE USER
    RParameter *getEP1Parameter() const;
    void setEP1Parameter(RParameter *value);

    RParameter *getEP2Parameter() const;
    void setEP2Parameter(RParameter *value);

    RParameter *getEpsP2Parameter() const;
    void setEpsP2Parameter(RParameter *value);

    RParameter *getEN1Parameter() const;
    void setEN1Parameter(RParameter *value);

    RParameter *getEN2Parameter() const;
    void setEN2Parameter(RParameter *value);

    RParameter *getEpsN2Parameter() const;
    void setEpsN2Parameter(RParameter *value);

    RParameter *getRhoParameter() const;
    void setRhoParameter(RParameter *value);

    int generateOpenSeesInput(std::string& object, int flag);

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

    double EP1;
    double epsP;

    //Optional
    double EN1;
    double EN2;
    double epsN;
    double EP2;

    int createElasticBilinUniaxialMaterial(void);

    //Material parameters
    RParameter* EP1Parameter;    //tangent in tension for stains: 0 <= strains <= $epsP2
    RParameter* EP2Parameter;    //tangent when material in tension with strains > $epsP2
    RParameter* epsPParameter;  //strain at which material changes tangent in tension.
    RParameter* EN1Parameter;    //optional, default = $EP1. tangent in compression for stains: 0 < strains <= $epsN2
    RParameter* EN2Parameter;    //optional, default = $EP2. tangent in compression with strains < $epsN2
    RParameter* epsNParameter;  //optional, default = -epsP2. strain at which material changes tangent in compression.

    RParameter* RhoParameter;

    void createPlots(const QVector<double> &stresses,const QVector<double> &strains,const QVector<double> &steps,const QString& description);
    void outputMaterialProperties(void);

    int initialize(void);

};




#endif


