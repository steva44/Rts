#include "RPlasticDamageConcretePlaneStress.h"
#include "RParameter.h"
#include "RUniaxialMaterial.h"
#include "RConcreteMaterialKnowledge.h"

#include "PlasticDamageConcrete3d.h"

RPlasticDamageConcretePlaneStress::RPlasticDamageConcretePlaneStress(QObject *parent, QString &name)
    : RNDMaterial(parent, name)
{
    theMatKnowledge = nullptr;
    thePlasticDamageConcrete3d = nullptr;
    EParameter= nullptr;
    nuParameter= nullptr;
    ftParameter= nullptr;
    fcParameter= nullptr;
}

RPlasticDamageConcretePlaneStress::~RPlasticDamageConcretePlaneStress()
{
}


NDMaterial* RPlasticDamageConcretePlaneStress::getTheNDMaterial(void)
{
    if(thePlasticDamageConcrete3d == nullptr)
        this->createPlasticDamageConcrete3d();

    return thePlasticDamageConcrete3d;
}



void RPlasticDamageConcretePlaneStress::createPlasticDamageConcrete3d(void)
{

    if(EParameter == nullptr)
    {
        qDebug()<<"The function "<<__FUNCTION__<<" needs an 'E' parameter";
        return;
    }
    if(nuParameter == nullptr)
    {
        qDebug()<<"The function "<<__FUNCTION__<<" needs an 'nu' parameter";
        return;
    }
    if(ftParameter == nullptr)
    {
        qDebug()<<"The function "<<__FUNCTION__<<" needs an 'ft' parameter";
        return;
    }
    if(fcParameter == nullptr)
    {
        qDebug()<<"The function "<<__FUNCTION__<<" needs an 'fc' parameter";
        return;
    }

    double E = EParameter->getCurrentValue();
    double nu = nuParameter->getCurrentValue();
    double ft = ftParameter->getCurrentValue();
    double fc = fcParameter->getCurrentValue();

    //    PlasticDamageConcrete3d(int tag,
    //                double E,
    //                double nu,
    //                double ft,
    //                double fc,
    //                double beta = 0.6,
    //                double Ap = 0.5,
    //                double An = 2.0,
    //                double Bn = 0.75);
    thePlasticDamageConcrete3d = new PlasticDamageConcrete3d(objectTag,E,nu,ft,fc);

}

RParameter *RPlasticDamageConcretePlaneStress::getFc() const
{
    return fcParameter;
}

void RPlasticDamageConcretePlaneStress::setFc(RParameter *value)
{
    fcParameter = value;
}

RConcreteMaterialKnowledge *RPlasticDamageConcretePlaneStress::getTheMatKnowledge() const
{
    return theMatKnowledge;
}

void RPlasticDamageConcretePlaneStress::setTheMatKnowledge(RConcreteMaterialKnowledge *value)
{
    theMatKnowledge = value;

    EParameter = theMatKnowledge->getYoungsModulus();
    nuParameter = theMatKnowledge->getPoissonsRatio();
    ftParameter = theMatKnowledge->getTensileStrength();
    fcParameter = theMatKnowledge->getCompressiveStrengthfc();
}

RParameter *RPlasticDamageConcretePlaneStress::getFt() const
{
    return ftParameter;
}

void RPlasticDamageConcretePlaneStress::setFt(RParameter *value)
{
    ftParameter = value;
}

RParameter *RPlasticDamageConcretePlaneStress::getNu() const
{
    return nuParameter;
}

void RPlasticDamageConcretePlaneStress::setNu(RParameter *value)
{
    nuParameter = value;
}

RParameter *RPlasticDamageConcretePlaneStress::getE() const
{
    return EParameter;
}

void RPlasticDamageConcretePlaneStress::setE(RParameter *value)
{
    EParameter = value;
}
