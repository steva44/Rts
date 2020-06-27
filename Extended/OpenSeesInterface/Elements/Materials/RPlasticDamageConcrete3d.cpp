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

#include "RPlasticDamageConcrete3d.h"
#include "RParameter.h"
#include "RUniaxialMaterial.h"
#include "RConcreteMaterialKnowledge.h"

#include "PlasticDamageConcrete3d.h"
#include "PlasticDamageConcretePlaneStress.h"

RPlasticDamageConcrete3d::RPlasticDamageConcrete3d(QObject *parent, QString &name)
    : RNDMaterial(parent, name)
{
    theMatKnowledge = nullptr;
    thePlasticDamageConcrete3d = nullptr;
    EParameter= nullptr;
    nuParameter= nullptr;
    ftParameter= nullptr;
    fcParameter= nullptr;

    //Values taken from concretepanel.tcl -- the (typ) values are taken from the class constructor
    beta = 0.5;      // plastic deformation rate (0.6 typ)
    Ap = 1.0;        // damage parameter (0.5 typ)
    An = 2.0;        // damage parameter (2.0 typ)
    Bn = 0.8;       // damage parameter (0.75 typ)
}


RPlasticDamageConcrete3d::~RPlasticDamageConcrete3d()
{
}


int RPlasticDamageConcrete3d::update(void)
{
    thePlasticDamageConcrete3d.reset();

    return this->initialize();
}


NDMaterial* RPlasticDamageConcrete3d::getTheNDMaterial(void)
{
    if(thePlasticDamageConcrete3d == nullptr)
        this->createPlasticDamageConcrete3d();

    return thePlasticDamageConcrete3d.get();
}


int RPlasticDamageConcrete3d::createPlasticDamageConcrete3d(void)
{

    if(EParameter == nullptr)
    {
        qDebug()<<"The function "<<__FUNCTION__<<" needs an 'E' parameter";
        return -1;
    }
    if(nuParameter == nullptr)
    {
        qDebug()<<"The function "<<__FUNCTION__<<" needs an 'nu' parameter";
        return -1;
    }
    if(ftParameter == nullptr)
    {
        qDebug()<<"The function "<<__FUNCTION__<<" needs an 'ft' parameter";
        return -1;
    }
    if(fcParameter == nullptr)
    {
        qDebug()<<"The function "<<__FUNCTION__<<" needs an 'fc' parameter";
        return -1;
    }

    return this->initialize();
}


int RPlasticDamageConcrete3d::initialize(void)
{
    double E = EParameter->getCurrentValue()*0.000145038;
    double nu = nuParameter->getCurrentValue();
    double ft = ftParameter->getCurrentValue()*0.000145038;
    double fc = fcParameter->getCurrentValue()*0.000145038;

    //    PlasticDamageConcrete3d(int tag,
    //                double E,
    //                double nu,
    //                double ft,
    //                double fc,
    //                double beta = 0.6,
    //                double Ap = 0.5,
    //                double An = 2.0,
    //                double Bn = 0.75);
    // thePlasticDamageConcrete3d = new PlasticDamageConcrete3d(objectTag,E,nu,ft,fc);
    thePlasticDamageConcrete3d = std::make_unique<PlasticDamageConcretePlaneStress>(objectTag,E,nu,ft,fc,beta,Ap,An,Bn);

    if(thePlasticDamageConcrete3d)
        return 0;

    return -1;
}


RParameter *RPlasticDamageConcrete3d::getFc() const
{
    return fcParameter;
}


void RPlasticDamageConcrete3d::setFc(RParameter *value)
{
    fcParameter = value;
}


RConcreteMaterialKnowledge *RPlasticDamageConcrete3d::getTheMatKnowledge() const
{
    return theMatKnowledge;
}


void RPlasticDamageConcrete3d::setTheMaterialKnowledge(RConcreteMaterialKnowledge *value)
{
    theMatKnowledge = value;

    EParameter = theMatKnowledge->getYoungsModulus();
    nuParameter = theMatKnowledge->getPoissonsRatio();
    ftParameter = theMatKnowledge->getTensileStrength();
    fcParameter = theMatKnowledge->getCompressiveStrength();
}


int RPlasticDamageConcrete3d::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(2);

    auto tag = std::to_string(this->objectTag);

    std::string E      = std::to_string(EParameter->getCurrentValue());
    std::string nu    = std::to_string(nuParameter->getCurrentValue());
    std::string ft      = std::to_string(ftParameter->getCurrentValue());
    std::string fc    = std::to_string(fcParameter->getCurrentValue());

    std::string betas  = std::to_string(beta);
    std::string Aps    = std::to_string(Ap);
    std::string Ans    = std::to_string(An);
    std::string Bns    = std::to_string(Bn);

    if(flag == 1) //tcl file
    {
        //        out<<"# nDMaterial PlasticDamageConcretePlaneStress tag $E $nu $ft $fc $beta $Ap $An $Bn"<<std::endl;
        out<<"nDMaterial PlasticDamageConcretePlaneStress "<<tag<<" "<<E<<" "<<nu<<" "<<ft<<" "<<fc<<" "<<betas<<" "<<Aps<<" "<<Ans<<" "<<Bns<<std::endl;
    }
    else if(flag == 2) //Python
    {
        qDebug()<<"Object 'PlasticDamageConcretePlaneStress' not yet available in OpenSeesPy";
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


RParameter *RPlasticDamageConcrete3d::getFt() const
{
    return ftParameter;
}


void RPlasticDamageConcrete3d::setFt(RParameter *value)
{
    ftParameter = value;
}


RParameter *RPlasticDamageConcrete3d::getNu() const
{
    return nuParameter;
}


void RPlasticDamageConcrete3d::setNu(RParameter *value)
{
    nuParameter = value;
}


RParameter *RPlasticDamageConcrete3d::getE() const
{
    return EParameter;
}


void RPlasticDamageConcrete3d::setE(RParameter *value)
{
    EParameter = value;
}
