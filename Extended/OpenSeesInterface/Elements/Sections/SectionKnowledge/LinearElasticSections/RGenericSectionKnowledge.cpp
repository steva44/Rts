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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RGenericSectionKnowledge.h"

RGenericSectionKnowledge::RGenericSectionKnowledge(QObject *parent, QString name)
    : RLinearElasticSectionKnowledge(parent, name)
{
    theAreaParameter = nullptr;
    theIyParameter = nullptr;
    theIzParameter = nullptr;
    theJParameter = nullptr;
}


RGenericSectionKnowledge::~RGenericSectionKnowledge()
{

}


RParameter *RGenericSectionKnowledge::getArea()
{
    return theAreaParameter;
}


RParameter *RGenericSectionKnowledge::getIy()
{
    return theIyParameter;
}


RParameter *RGenericSectionKnowledge::getIz()
{
    return theIzParameter;
}


RParameter *RGenericSectionKnowledge::getJ()
{
    return theJParameter;
}


void RGenericSectionKnowledge::setA(RParameter* parameter)
{
    theAreaParameter = parameter;
}


void RGenericSectionKnowledge::setIy(RParameter* parameter)
{
    theIyParameter = parameter;
}


void RGenericSectionKnowledge::setIz(RParameter* parameter)
{
    theIzParameter = parameter;
}


void RGenericSectionKnowledge::setJ(RParameter* parameter)
{
    theJParameter = parameter;
}
