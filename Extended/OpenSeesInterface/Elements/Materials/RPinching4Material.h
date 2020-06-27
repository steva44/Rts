#ifndef RPinching4Material_H
#define RPinching4Material_H
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

// Note: this class is a work in progress and its implementation is still not complete

#include "RUniaxialMaterial.h"

class RMaterialKnowledge;
class RParameter;
class RMainWindow;
class QCustomPlot;
class RSteelMaterialKnowledge;

class RPinching4Material : public RUniaxialMaterial
{
    Q_OBJECT

public:
    RPinching4Material(QObject *parent, QString &name);
    ~RPinching4Material();

    void setMaterialKnowledge(RMaterialKnowledge *materialKnowledge);
    UniaxialMaterial* getUniaxialMaterial();
    int update(void);

    //PROPERTIES SET BY THE USER
    RParameter* getFyParameter() const;
    void setFyParameter(RParameter* value);

    RParameter* getE0Parameter() const;
    void setE0Parameter(RParameter* value);

    RParameter* getBParameter() const;
    void setBParameter(RParameter* value);

    RParameter* getRhoParameter() const;
    void setRhoParameter(RParameter* value);

    RParameter* getA1Parameter() const;
    void setA1Parameter(RParameter* value);

    RParameter* getA2Parameter() const;
    void setA2Parameter(RParameter* value);

    RParameter* getA3Parameter() const;
    void setA3Parameter(RParameter* value);

    RParameter* getA4Parameter() const;
    void setA4Parameter(RParameter* value);

    RParameter* getR0Parameter() const;
    void setR0Parameter(RParameter* value);

    RParameter* getCR1Parameter() const;
    void setCR1Parameter(RParameter* value);

    RParameter* getCR2Parameter() const;
    void setCR2Parameter(RParameter* value);

    RParameter* getSiginiParameter() const;
    void setSiginiParameter(RParameter* value);

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

    void createPinching4MaterialUniaxialMaterial(void);

    std::unique_ptr<RSteelMaterialKnowledge> theSteelMaterial;

    //Material parameters
    RParameter* fyParameter;    //  = matpar(1)  : yield stress
    RParameter* E0Parameter;    //  = matpar(2)  : initial stiffness
    RParameter* bParameter;     //  = matpar(3)  : hardening ratio (Esh/E0)
    RParameter* RhoParameter;

    // a1 through a4 are coefficients for isotropic hardening
    RParameter* a1Parameter;   //  = matpar(7)  : coefficient for isotropic hardening in compression
    RParameter* a2Parameter;   //  = matpar(8)  : coefficient for isotropic hardening in compression
    RParameter* a3Parameter;   //  = matpar(9)  : coefficient for isotropic hardening in tension
    RParameter* a4Parameter;   //  = matpar(10) : coefficient for isotropic hardening in tension

    RParameter* R0Parameter;  //  = matpar(4)  : exp transition elastic-plastic
    RParameter* cR1Parameter; //  = matpar(5)  : coefficient for changing R0 to R
    RParameter* cR2Parameter; //  = matpar(6)  : coefficient for changing R0 to R
    RParameter* siginiParameter;

    void createPlots(const QVector<double> &stresses,const QVector<double> &strains,const QVector<double> &steps,const QString& description);
    void outputMaterialProperties(void);

    void createTestMaterialParameters(void);

};




#endif


