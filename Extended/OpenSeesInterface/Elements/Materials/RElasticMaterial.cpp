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

#include "RElasticMaterial.h"
#include "RUniaxialMaterial.h"
#include "RDomain.h"

#include "ElasticMaterial.h"

RElasticMaterial::RElasticMaterial(QObject *parent, QString &name)
    : RUniaxialMaterial(parent, name)
{
    theElasticMaterial = nullptr;
    theEParameter = nullptr;
    theEtaParameter = nullptr;
    theEnegParameter = nullptr;
}


RElasticMaterial::~RElasticMaterial()
{
}


int RElasticMaterial::update(void)
{
    auto res = this->createElasticMaterial();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees 'ElasticMaterial' in "<<this->objectName();
        theElasticMaterial.reset();
        return -1;
    }

    return 0;

}


UniaxialMaterial* RElasticMaterial::getUniaxialMaterial()
{
    if(theElasticMaterial == nullptr)
        this->createElasticMaterial();

    return theElasticMaterial.get();
}


RParameter *RElasticMaterial::getTheEParameter() const
{
    return theEParameter;
}


void RElasticMaterial::setTheEParameter(RParameter *value)
{
    theEParameter = value;
}


RParameter *RElasticMaterial::getTheEtaParameter() const
{
    return theEtaParameter;
}

void RElasticMaterial::setTheEtaParameter(RParameter *value)
{
    theEtaParameter = value;
}


RParameter *RElasticMaterial::getTheEnegParameter() const
{
    return theEnegParameter;
}

void RElasticMaterial::setTheEnegParameter(RParameter *value)
{
    theEnegParameter = value;
}


int RElasticMaterial::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(4);

    auto tag = std::to_string(this->objectTag);

    std::string E     = std::to_string(theEParameter->getCurrentValue());
    std::string Eta = "0.0";
    std::string Eneg = E;

    if(theEtaParameter)
        Eta    = std::to_string(theEtaParameter->getCurrentValue());

    if(theEnegParameter)
        Eneg      = std::to_string(theEnegParameter->getCurrentValue());

    if(flag == 1) //tcl file
    {
        //        out<<"# uniaxialMaterial Elastic $matTag $E <$eta> <$Eneg>"<<std::endl;
        out<<"uniaxialMaterial Elastic "<<tag<<" "<<E<<" "<<Eta<<" "<<Eneg<<std::endl;
    }
    else if(flag == 2) //Python
    {
        //        out<<"# uniaxialMaterial('Elastic', matTag, E, eta=0.0, Eneg=E)"<<std::endl;
        out<<"uniaxialMaterial('Elastic', "<<tag<<", "<<E<<", "<<Eta<<", "<<Eneg<<std::endl;
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


int RElasticMaterial::createElasticMaterial(void)
{
    if(theEParameter == nullptr)
    {
        qDebug()<<"The E parameter is not set in "<<__FUNCTION__;
        return -1;
    }

    auto E = theEParameter->getCurrentValue();

    //Optional
    auto Eta = 0.0;
    auto Eneg = E;

    if(theEtaParameter)
        Eta = theEtaParameter->getCurrentValue();

    if(theEnegParameter)
        Eneg = theEnegParameter->getCurrentValue();

    //    ElasticMaterial(int tag, double E, double eta = 0.0);
    //    ElasticMaterial(int tag, double Epos, double eta, double Eneg);
    theElasticMaterial = std::make_unique<ElasticMaterial>(ElasticMaterial(objectTag,E,Eta,Eneg));

    if(theElasticMaterial == nullptr)
        return -1;

    return 0;
}



