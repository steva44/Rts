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

#include "RElasticIsotropicMaterial.h"
#include "RParameter.h"
#include "RUniaxialMaterial.h"
#include "RConcreteMaterialKnowledge.h"

#include "ElasticIsotropicThreeDimensional.h"

RElasticIsotropicMaterial::RElasticIsotropicMaterial(QObject *parent, QString &name)
    : RNDMaterial(parent, name)
{
    theMatKnowledge = nullptr;
    theElasticIsotropic3DMaterial = nullptr;
    EParameter = nullptr;
    nuParameter = nullptr;
    rhoParameter = nullptr;
}


RElasticIsotropicMaterial::~RElasticIsotropicMaterial()
{
}


int RElasticIsotropicMaterial::update(void)
{
    theElasticIsotropic3DMaterial.reset();

    return this->createElasticIsotropicMaterial();
}


NDMaterial* RElasticIsotropicMaterial::getTheNDMaterial(void)
{
    if(theElasticIsotropic3DMaterial == nullptr)
        this->createElasticIsotropicMaterial();

    return theElasticIsotropic3DMaterial.get();
}


int RElasticIsotropicMaterial::createElasticIsotropicMaterial(void)
{
    if(theMatKnowledge == nullptr)
    {
        qDebug()<<"The function "<<__FUNCTION__<<" needs an material knowledge";
        return -1;
    }

    double E = EParameter->getCurrentValue();
    double nu = nuParameter->getCurrentValue();
    double rho = rhoParameter->getCurrentValue();

    //ElasticIsotropicMaterial (int tag, double E, double nu, double rho = 0.0);
    theElasticIsotropic3DMaterial = std::make_unique<ElasticIsotropicThreeDimensional>(objectTag,E,nu,rho);

    return 0;
}

int RElasticIsotropicMaterial::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(4);

    auto tag = std::to_string(this->objectTag);

    std::string E      = std::to_string(EParameter->getCurrentValue());
    std::string nu    = std::to_string(nuParameter->getCurrentValue());
    std::string rho      = std::to_string(rhoParameter->getCurrentValue());

    if(flag == 1) //tcl file
    {
        //        out<<"# nDMaterial ElasticIsotropic $matTag $E $v <$rho>"<<std::endl;
        out<<"nDMaterial ElasticIsotropic "<<tag<<" "<<E<<" "<<nu<<" "<<rho<<std::endl;
    }
    else if(flag == 2) //Python
    {
        //        out<<"# nDMaterial('ElasticIsotropic', matTag, E, nu, rho=0.0)"<<std::endl;
        out<<"nDMaterial('ElasticIsotropic', "<<tag<<", "<<E<<", "<<nu<<", "<<rho<<")"<<std::endl;

        qDebug()<<"Object 'PlasticDamageConcretePlaneStress' not yet available in OpenSeesPy";
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


RMaterialKnowledge *RElasticIsotropicMaterial::getTheMaterialKnowledge() const
{
    return theMatKnowledge;
}


void RElasticIsotropicMaterial::setTheMaterialKnowledge(RMaterialKnowledge *value)
{
    theMatKnowledge = value;

    EParameter = theMatKnowledge->getYoungsModulus();
    nuParameter = theMatKnowledge->getPoissonsRatio();
    rhoParameter = theMatKnowledge->getDensity();
}


