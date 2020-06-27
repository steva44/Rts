#ifndef RPlasticDamageConcrete3d_H
#define RPlasticDamageConcrete3d_H
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

#include "RNDMaterial.h"

class RParameter;
class RConcreteMaterialKnowledge;
class NDMaterial;
class PlasticDamageConcrete3d;
class PlasticDamageConcretePlaneStress;

class RPlasticDamageConcrete3d : public RNDMaterial
{
    Q_OBJECT

public:
    RPlasticDamageConcrete3d(QObject *parent, QString &name);
    ~RPlasticDamageConcrete3d();

    NDMaterial* getTheNDMaterial(void);
    int update(void);

    RParameter *getE() const;
    void setE(RParameter *value);

    RParameter *getNu() const;
    void setNu(RParameter *value);

    RParameter *getFt() const;
    void setFt(RParameter *value);

    RParameter *getFc() const;
    void setFc(RParameter *value);

    RConcreteMaterialKnowledge *getTheMatKnowledge() const;
    void setTheMaterialKnowledge(RConcreteMaterialKnowledge *value);

    int generateOpenSeesInput(std::string& object, int flag);

private:

    RConcreteMaterialKnowledge* theMatKnowledge;
    std::unique_ptr<PlasticDamageConcretePlaneStress> thePlasticDamageConcrete3d;
    int createPlasticDamageConcrete3d(void);
    int initialize(void);

    RParameter* EParameter;
    RParameter* nuParameter;
    RParameter* ftParameter;
    RParameter* fcParameter;

    double beta;
    double Ap;
    double An;
    double Bn;
};

#endif


