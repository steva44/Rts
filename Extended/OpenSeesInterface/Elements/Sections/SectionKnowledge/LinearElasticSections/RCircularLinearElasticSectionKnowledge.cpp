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

#include "RCircularLinearElasticSectionKnowledge.h"
#include "RMeshGenerator.h"

RCircularLinearElasticSectionKnowledge::RCircularLinearElasticSectionKnowledge(RDomain *domain, QString componentName, RParameter *diameter)
    : RLinearElasticSectionKnowledge(domain, componentName)
{
    theDiameter = nullptr;
    theAreaParameter = nullptr;
    theIyParameter = nullptr;
    theIzParameter = nullptr;
    theJParameter = nullptr;

    theDiameter = diameter;
}


RCircularLinearElasticSectionKnowledge::~RCircularLinearElasticSectionKnowledge()
{
}


RParameter *RCircularLinearElasticSectionKnowledge::getArea()
{
    theDomain->createObject(theSectionName + "AreaLambdaParameter", "RConstant");
    theAreaParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

    auto AreaLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2) {
        auto d = theDiameter->getCurrentValue();
        return PI*d*d/4.0;
    };
    theAreaParameter->setCurrentValue(AreaLambda);

    return theAreaParameter;
}


RParameter *RCircularLinearElasticSectionKnowledge::getIy()
{
    theDomain->createObject(theSectionName + "IyLambdaParameter", "RConstant");
    theIyParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

    auto IyValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
    {
        auto d = theDiameter->getCurrentValue();

        return PI* d*d*d*d/ 64.0;
    };
    theIyParameter->setCurrentValue(IyValue);

    return theIyParameter;
}


RParameter *RCircularLinearElasticSectionKnowledge::getIz()
{
    theDomain->createObject(theSectionName + "IzLambdaParameter", "RConstant");
    theIzParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

    auto IzValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
    {
        auto d = theDiameter->getCurrentValue();
        return PI* d*d*d*d/ 64.0;
    };
    theIzParameter->setCurrentValue(IzValue);

    return theIzParameter;
}


RParameter *RCircularLinearElasticSectionKnowledge::getJ()
{
    theDomain->createObject(theSectionName + "JLambdaParameter", "RConstant");
    theJParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

    auto JValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
    {
        auto d = theDiameter->getCurrentValue();
        return PI* d*d*d*d/ 32.0;
    };
    theJParameter->setCurrentValue(JValue);

    return theJParameter;
}
