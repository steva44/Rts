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

#include "RElasticMembranePlateSection.h"
#include "RParameter.h"

#include "ElasticMembranePlateSection.h"
#include <sstream>

RElasticMembranePlateSection::RElasticMembranePlateSection(QObject *parent, QString name)
    : RSectionForceDeformation(parent, name)
{

    theEParameter = nullptr;
    theNuParameter = nullptr;
    theThicknessParameter = nullptr;
    theRhoParameter = nullptr;

    theSectionForceDeformation = nullptr;
}


RElasticMembranePlateSection::~RElasticMembranePlateSection()
{

}


int RElasticMembranePlateSection::update(void)
{

    theSectionForceDeformation.reset();

    auto res = this->initialize();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees element in "<<this->objectName();
        return -1;
    }

    return 0;
}


SectionForceDeformation* RElasticMembranePlateSection::getTheSectionForceDeformation(void)
{

    if(!theSectionForceDeformation)
        this->createSectionForceDeformation();

    return theSectionForceDeformation.get();
}


int RElasticMembranePlateSection::createSectionForceDeformation(void)
{
    // ElasticMembranePlateSection(int tag,
    // double E,
    // double nu,
    // double h = 1.0,
    // double rho = 0.0 ) ;

    //Check to make sure the property objects are not null pointers
    if(theThicknessParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a Thickness parameter";
        return -1;
    }
    if(theRhoParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a Rho parameter";
        return -1;
    }
    if(theNuParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a Nu parameter";
        return -1;
    }

    if(theEParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a E parameter";
        return -1;
    }

    return this->initialize();
}


int RElasticMembranePlateSection::initialize(void)
{

    auto E   = theEParameter->getCurrentValue();
    auto nu  = theNuParameter->getCurrentValue();
    auto h   = theThicknessParameter->getCurrentValue();
    auto rho = theRhoParameter->getCurrentValue();

    theSectionForceDeformation = std::make_unique<ElasticMembranePlateSection>(objectTag, E, nu, h, rho);

    if(theSectionForceDeformation == nullptr)
        return -1;

    return 0;
}


int RElasticMembranePlateSection::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(2);

    auto tag = std::to_string(this->objectTag);

    std::string E   = std::to_string(theEParameter->getCurrentValue());
    std::string nu  = std::to_string(theNuParameter->getCurrentValue());
    std::string h   = std::to_string(theThicknessParameter->getCurrentValue());
    std::string rho = std::to_string(theRhoParameter->getCurrentValue());

    if(flag == 1) //tcl file
    {
        //out<<"# section ElasticMembranePlateSection $secTag $E $nu $h $rho"<<std::endl;
        out<<"section ElasticMembranePlateSection "<<tag<<" "<<E<<" "<<nu<<" "<<h<<" "<<rho<<std::endl;
    }
    else if(flag == 2) //Python
    {
        //out<<"# section('ElasticMembranePlateSection', secTag, E_mod, nu, h, rho)"<<std::endl;
        out<<"section('ElasticMembranePlateSection', "<<tag<<", "<<E<<", "<<nu<<", "<<h<<", "<<rho<<")"<<std::endl;
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


RParameter *RElasticMembranePlateSection::getTheRhoParameter() const
{
    return theRhoParameter;
}


void RElasticMembranePlateSection::setTheRhoParameter(RParameter *value)
{
    theRhoParameter = value;
}


RParameter *RElasticMembranePlateSection::getTheThicknessParameter() const
{
    return theThicknessParameter;
}


void RElasticMembranePlateSection::setTheThicknessParameter(RParameter *value)
{
    theThicknessParameter = value;
}


RParameter *RElasticMembranePlateSection::getTheNuParameter() const
{
    return theNuParameter;
}


void RElasticMembranePlateSection::setTheNuParameter(RParameter *value)
{
    theNuParameter = value;
}


RParameter *RElasticMembranePlateSection::getTheEParameter() const
{
    return theEParameter;
}


void RElasticMembranePlateSection::setTheEParameter(RParameter *value)
{
    theEParameter = value;
}





