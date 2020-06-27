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

#include "RRectangularLinearElasticSectionKnowledge.h"
#include "RMeshGenerator.h"

RRectangularLinearElasticSectionKnowledge::RRectangularLinearElasticSectionKnowledge(RDomain *domain,
                                                                                     QString componentName,
                                                                                     RParameter *depth,
                                                                                     RParameter *width)
    : RLinearElasticSectionKnowledge(domain, componentName)
{
    theDepth = depth;
    theWidth = width;

    theAreaParameter = nullptr;
    theIyParameter = nullptr;
    theIzParameter = nullptr;
    theJParameter = nullptr;
    theAvyParameter = nullptr;
    theAvzParameter = nullptr;
}



RRectangularLinearElasticSectionKnowledge::~RRectangularLinearElasticSectionKnowledge()
{

}


RParameter *RRectangularLinearElasticSectionKnowledge::getArea()
{
    if(theAreaParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "AreaLambdaParameter", "RConstant");
        theAreaParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto AreaLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2) {
            auto h = theDepth->getCurrentValue();
            auto b = theWidth->getCurrentValue();

            return b*h;
        };
        theAreaParameter->setCurrentValue(AreaLambda);
    }

    return theAreaParameter;
}


RParameter *RRectangularLinearElasticSectionKnowledge::getIy()
{
    if(theIyParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "IyLambdaParameter", "RConstant");
        theIyParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto IyValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto h = theDepth->getCurrentValue();
            auto b = theWidth->getCurrentValue();

            return b*h*h*h/12.0;
        };
        theIyParameter->setCurrentValue(IyValue);
    }

    return theIyParameter;
}


RParameter *RRectangularLinearElasticSectionKnowledge::getIz()
{
    if(theIzParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "IzLambdaParameter", "RConstant");
        theIzParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto IzValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto h = theDepth->getCurrentValue();
            auto b = theWidth->getCurrentValue();

            return h*b*b*b/12.0;
        };
        theIzParameter->setCurrentValue(IzValue);
    }

    return theIzParameter;
}


RParameter *RRectangularLinearElasticSectionKnowledge::getJ()
{
    if(theJParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "JLambdaParameter", "RConstant");
        theJParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto JValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto a = theDepth->getCurrentValue();
            auto b = theWidth->getCurrentValue();

            return a*b*b*b*(0.333-0.21*b/a*(1.0-b*b*b*b/(12*a*a*a*a)));
        };
        theJParameter->setCurrentValue(JValue);
    }

    return theJParameter;
}


RParameter *RRectangularLinearElasticSectionKnowledge::getAvy()
{
    if(theAvyParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "AvyLambdaParameter", "RConstant");
        theAvyParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto AvyValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto d = theDepth->getCurrentValue();
            auto w = theWidth->getCurrentValue();

            return d*w*5.0/6.0;
        };
        theAvyParameter->setCurrentValue(AvyValue);
    }

    return theAvyParameter;
}


RParameter *RRectangularLinearElasticSectionKnowledge::getAvz()
{
    if(theAvzParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "AvzLambdaParameter", "RConstant");
        theAvzParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto AvzValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto d = theDepth->getCurrentValue();
            auto w = theWidth->getCurrentValue();

            return d*w*5.0/6.0;
        };
        theAvzParameter->setCurrentValue(AvzValue);
    }

    return theAvzParameter;
}


QPointer<RParameter> RRectangularLinearElasticSectionKnowledge::getTheDepth() const
{
    return theDepth;
}


void RRectangularLinearElasticSectionKnowledge::setTheDepth(const QPointer<RParameter> &value)
{
    theDepth = value;
}


QPointer<RParameter> RRectangularLinearElasticSectionKnowledge::getTheWidth() const
{
    return theWidth;
}


void RRectangularLinearElasticSectionKnowledge::setTheWidth(const QPointer<RParameter> &value)
{
    theWidth = value;
}
