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

#include "RWLinearElasticSectionKnowledge.h"
#include "RMeshGenerator.h"

RWLinearElasticSectionKnowledge::   RWLinearElasticSectionKnowledge(RDomain *domain,
                                                                    QString componentName,
                                                                    RParameter *theWebHeight,
                                                                    RParameter *theWebThickness,
                                                                    RParameter *theFlangeWidth,
                                                                    RParameter *theFlangeThickness)
    : RLinearElasticSectionKnowledge(domain, componentName),
      theWebHeightParameter(theWebHeight),
      theFlangeWidthParameter(theFlangeWidth),
      theWebThicknessParameter(theWebThickness),
      theFlangeThicknessParameter(theFlangeThickness)
{
    theAreaParameter = nullptr;
    theIyParameter = nullptr;
    theIzParameter = nullptr;
    theJParameter = nullptr;
    theAvyParameter = nullptr;
    theAvzParameter = nullptr;
}



RWLinearElasticSectionKnowledge::~RWLinearElasticSectionKnowledge()
{

}


RParameter *RWLinearElasticSectionKnowledge::getArea()
{
    if(theAreaParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "AreaLambdaParameter", "RConstant");
        theAreaParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        //Area - always initialized in component constructor since used across many processes
        auto AreaValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto webHeight = theWebHeightParameter->getCurrentValue();
            auto webThickness = theWebThicknessParameter->getCurrentValue();

            auto flangeWidth = theFlangeWidthParameter->getCurrentValue();
            auto flangeThickness = theFlangeThicknessParameter->getCurrentValue();

            return webHeight * webThickness + 2.0 * flangeWidth * flangeThickness;
        };
        theAreaParameter->setCurrentValue(AreaValue);
    }

    return theAreaParameter;
}


RParameter *RWLinearElasticSectionKnowledge::getIy()
{
    if(theIyParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "IyLambdaParameter", "RConstant");
        theIyParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto IyValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto webHeight = theWebHeightParameter->getCurrentValue();
            auto webThickness = theWebThicknessParameter->getCurrentValue();

            auto flangeWidth = theFlangeWidthParameter->getCurrentValue();
            auto flangeThickness = theFlangeThicknessParameter->getCurrentValue();

            return webThickness * webHeight * webHeight * webHeight / 12.0 + 2.0 * (flangeWidth * flangeThickness * flangeThickness * flangeThickness / 12.0 + flangeWidth * flangeThickness * 0.25 * webHeight * webHeight );
        };
        theIyParameter->setCurrentValue(IyValue);
    }

    return theIyParameter;

}


RParameter *RWLinearElasticSectionKnowledge::getIz()
{
    if(theIzParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "IzLambdaParameter", "RConstant");
        theIzParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        //For circular component Iz is equal to Iy
        auto IzValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto webHeight = theWebHeightParameter->getCurrentValue();
            auto webThickness = theWebThicknessParameter->getCurrentValue();

            auto flangeWidth = theFlangeWidthParameter->getCurrentValue();
            auto flangeThickness = theFlangeThicknessParameter->getCurrentValue();

            return  webHeight * webThickness * webThickness * webThickness / 12.0 + 2.0 * (flangeThickness * flangeWidth * flangeWidth * flangeWidth / 12.0);
        };
        theIzParameter->setCurrentValue(IzValue);
    }

    return theIzParameter;
}


RParameter *RWLinearElasticSectionKnowledge::getJ()
{
    if(theJParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "JLambdaParameter", "RConstant");
        theJParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto JValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto webHeight = theWebHeightParameter->getCurrentValue();
            auto webThickness = theWebThicknessParameter->getCurrentValue();

            auto flangeWidth = theFlangeWidthParameter->getCurrentValue();
            auto flangeThickness = theFlangeThicknessParameter->getCurrentValue();

            return webHeight * webThickness * webThickness * webThickness / 3.0 + 2.0 * (flangeWidth * flangeThickness * flangeThickness * flangeThickness / 3.0);
        };
        theJParameter->setCurrentValue(JValue);
    }

    return theJParameter;
}


RParameter *RWLinearElasticSectionKnowledge::getAvy()
{
    if(theAvyParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "AvyLambdaParameter", "RConstant");
        theAvyParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto AvyValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto d = theFlangeWidthParameter->getCurrentValue();
            auto w = theFlangeThicknessParameter->getCurrentValue();

            return 2.0*d*w*5.0/6.0;
        };
        theAvyParameter->setCurrentValue(AvyValue);
    }

    return theAvyParameter;
}


RParameter *RWLinearElasticSectionKnowledge::getAvz()
{
    if(theAvzParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "AvzLambdaParameter", "RConstant");
        theAvzParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto AvzValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto d = theWebHeightParameter->getCurrentValue();
            auto w = theWebThicknessParameter->getCurrentValue();

            return d*w*5.0/6.0;
        };
        theAvzParameter->setCurrentValue(AvzValue);
    }

    return theAvzParameter;
}
