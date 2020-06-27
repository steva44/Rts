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

#include "RMembranePlateFiberSection.h"
#include "RParameter.h"
#include "RNDMaterial.h"

#include "MembranePlateFiberSection.h"

RMembranePlateFiberSection::RMembranePlateFiberSection(QObject *parent, QString name)
    : RSectionForceDeformation(parent, name)
{

    theThicknessParameter = nullptr;
    theNDMaterial = nullptr;

    theSectionForceDeformation = nullptr;
}


RMembranePlateFiberSection::~RMembranePlateFiberSection()
{

}


int RMembranePlateFiberSection::update(void)
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


RNDMaterial* RMembranePlateFiberSection::getTheNDMaterial() const
{
    return theNDMaterial;
}


void RMembranePlateFiberSection::setTheNDMaterial(RNDMaterial *value)
{
    theNDMaterial = value;
}


SectionForceDeformation* RMembranePlateFiberSection::getTheSectionForceDeformation(void)
{
    if(!theSectionForceDeformation)
        this->createSectionForceDeformation();

    return theSectionForceDeformation.get();
}

int RMembranePlateFiberSection::createSectionForceDeformation(void)
{

    // MembranePlateFiberSection(
    // int tag,
    // double thickness,
    // NDMaterial &Afiber ) ;

    //Check to make sure the property objects are not null pointers
    if(theThicknessParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a Thickness parameter";
        return -1;
    }

    if(theNDMaterial == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs an ND material";
        return -1;
    }

    return this->initialize();
}


int RMembranePlateFiberSection::initialize(void)
{
    auto h =  theThicknessParameter->getCurrentValue();

    NDMaterial* ndMat = theNDMaterial->getTheNDMaterial();

    theSectionForceDeformation = std::make_unique<MembranePlateFiberSection>(objectTag, h, *ndMat);

    if(theSectionForceDeformation == nullptr)
        return -1;

    return 0;
}


RParameter *RMembranePlateFiberSection::getTheThicknessParameter() const
{
    return theThicknessParameter;
}


void RMembranePlateFiberSection::setTheThicknessParameter(RParameter *value)
{
    theThicknessParameter = value;
}




