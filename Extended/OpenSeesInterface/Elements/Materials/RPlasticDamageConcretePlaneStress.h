#ifndef RPlasticDamageConcretePlaneStress_H
#define RPlasticDamageConcretePlaneStress_H

#include "RNDMaterial.h"

class RParameter;
class RConcreteMaterialKnowledge;
class NDMaterial;
class PlasticDamageConcrete3d;

class RPlasticDamageConcretePlaneStress : public RNDMaterial
{
    Q_OBJECT

public:
    RPlasticDamageConcretePlaneStress(QObject *parent, QString &name);
    ~RPlasticDamageConcretePlaneStress();

    NDMaterial* getTheNDMaterial(void);

    RParameter *getE() const;
    void setE(RParameter *value);

    RParameter *getNu() const;
    void setNu(RParameter *value);

    RParameter *getFt() const;
    void setFt(RParameter *value);

    RParameter *getFc() const;
    void setFc(RParameter *value);

    RConcreteMaterialKnowledge *getTheMatKnowledge() const;
    void setTheMatKnowledge(RConcreteMaterialKnowledge *value);

private:

    RConcreteMaterialKnowledge* theMatKnowledge;
    PlasticDamageConcrete3d* thePlasticDamageConcrete3d;
    void createPlasticDamageConcrete3d(void);

    RParameter* EParameter;
    RParameter* nuParameter;
    RParameter* ftParameter;
    RParameter* fcParameter;
};

#endif


