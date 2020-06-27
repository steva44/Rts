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

#include "RCircularHSSLinearElasticSectionKnowledge.h"
#include "RMeshGenerator.h"

RCircularHSSLinearElasticSectionKnowledge::RCircularHSSLinearElasticSectionKnowledge(RDomain *domain, QString componentName, RParameter *outerDiameterParameter, RParameter *wallThicknessParameter)
    : RLinearElasticSectionKnowledge(domain, componentName)
{
    theOuterDiameterParameter = nullptr;
    theWallThicknessParameter = nullptr;
    theAreaParameter = nullptr;
    theIyParameter = nullptr;
    theIzParameter = nullptr;
    theJParameter = nullptr;

    theOuterDiameterParameter = outerDiameterParameter;
    theWallThicknessParameter = wallThicknessParameter;
}


RCircularHSSLinearElasticSectionKnowledge::~RCircularHSSLinearElasticSectionKnowledge()
{

}


RParameter *RCircularHSSLinearElasticSectionKnowledge::getArea()
{
    if(theAreaParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "AreaLambdaParameter", "RConstant");
        theAreaParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto AreaValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto Do = theOuterDiameterParameter->getCurrentValue();
            auto t = theWallThicknessParameter->getCurrentValue();

            if(theDDMParameter == nullptr && theDDMParameter2 == nullptr)
            {
                return Do*PI*t-PI*t*t;
            }
            else if (theDDMParameter == theOuterDiameterParameter)
            {
                return PI*t;  // dAreadD = pi*t
            }
            else if (theDDMParameter == theWallThicknessParameter)
            {
                return Do*PI-2.0*PI*t;  // dAreadt
            }
            else if (theDDMParameter2 == theWallThicknessParameter)
            {
                return -2.0*PI;  // d2Areadt
            }
            else
                return 0.0;

        };
        theAreaParameter->setCurrentValue(AreaValue);
    }

    return theAreaParameter;
}


RParameter *RCircularHSSLinearElasticSectionKnowledge::getIy()
{
    if(theIyParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "IyLambdaParameter", "RConstant");
        theIyParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        //Calculate Iy
        auto IyValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto Do = theOuterDiameterParameter->getCurrentValue();
            auto diameterPowFour = Do*Do*Do*Do;
            auto t = theWallThicknessParameter->getCurrentValue();
            auto innerDiameter = Do-2*t;

            if(theDDMParameter == nullptr && theDDMParameter2 == nullptr)
            {
                return 0.015625*PI*(diameterPowFour - innerDiameter*innerDiameter*innerDiameter*innerDiameter);
            }
            else if (theDDMParameter == theOuterDiameterParameter)
            {
                return 3.0/8.0*Do*Do*PI*t-3.0/4.0*Do*PI*t*t+PI*t*t*t/2.0; // dIydD
            }
            else if (theDDMParameter2 == theOuterDiameterParameter)
            {
                return 3.0/4.0*Do*PI*t-3.0/4.0*PI*t*t; //d2IydD
            }
            else if (theDDMParameter == theWallThicknessParameter)
            {
                return Do*Do*Do*PI/8.0-3.0/4.0*Do*Do*PI*t+3.0/2.0*Do*PI*t*t-PI*t*t*t; // dIydt
            }
            else if (theDDMParameter2 == theWallThicknessParameter)
            {
                return -3.0/4.0*Do*Do*PI+3.0*Do*PI*t-3.0*PI*t*t; // d2Iydt
            }
            else
                return 0.0;

        };
        theIyParameter->setCurrentValue(IyValue);
    }

    return theIyParameter;

}


RParameter *RCircularHSSLinearElasticSectionKnowledge::getIz()
{
    if(theIzParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "IzLambdaParameter", "RConstant");
        theIzParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        auto IzValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto Do = theOuterDiameterParameter->getCurrentValue();
            auto t = theWallThicknessParameter->getCurrentValue();
            auto diameterPowFour = Do*Do*Do*Do;
            auto innerDiameter = Do-2*t;

            if(theDDMParameter == nullptr && theDDMParameter2 == nullptr)
            {
                return 0.015625*PI*(diameterPowFour - innerDiameter*innerDiameter*innerDiameter*innerDiameter);
            }
            else if (theDDMParameter == theOuterDiameterParameter)
            {
                return 3.0/8.0*Do*Do*PI*t-3.0/4.0*Do*PI*t*t+PI*t*t*t/2.0;  // dIzdD
            }
            else if (theDDMParameter2 == theOuterDiameterParameter)
            {
                return 3.0/4.0*Do*PI*t-3.0/4.0*PI*t*t; // d2IzdD
            }
            else if (theDDMParameter == theWallThicknessParameter)
            {
                return Do*Do*Do*PI/8.0-3.0/4.0*Do*Do*PI*t+3.0/2.0*Do*PI*t*t-PI*t*t*t; // dIzdt
            }
            else if (theDDMParameter2 == theWallThicknessParameter)
            {
                return -3.0/4.0*Do*Do*PI+3.0*Do*PI*t-3.0*PI*t*t; // d2Izdt
            }
            else
                return 0.0;
        };
        theIzParameter->setCurrentValue(IzValue);
    }

    return theIzParameter;
}


RParameter *RCircularHSSLinearElasticSectionKnowledge::getJ()
{
    if(theJParameter == nullptr)
    {
        theDomain->createObject(theSectionName + "JLambdaParameter", "RConstant");
        theJParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

        //Calculate St Venants Torsional Constant
        auto JValue = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto Do = theOuterDiameterParameter->getCurrentValue();
            auto t = theWallThicknessParameter->getCurrentValue();
            auto diameterSquared = Do*Do;
            auto diameterCubed = Do*Do*Do;

            if(theDDMParameter == nullptr && theDDMParameter2 == nullptr)
            {
                return 0.25*PI*t*(diameterCubed-3.0*diameterSquared*t+4.0*Do*t*t-2.0*t*t*t);
            }
            else if (theDDMParameter == theOuterDiameterParameter)
            {
                return PI*t*(3.0*Do*Do-6.0*Do*t+4.0*t*t)/4.0; // dJydD
            }
            else if (theDDMParameter2 == theOuterDiameterParameter)
            {
                return 3.0/2.0*PI*t*(-t+Do); // d2JydD
            }
            else if (theDDMParameter == theWallThicknessParameter)
            {
                return PI*(Do*Do*Do-3.0*Do*Do*t+4.0*Do*t*t-2.0*t*t*t)/4.0+PI*t*(-3.0*Do*Do+8.0*Do*t-6.0*t*t)/4.0; // dJydt
            }
            else if (theDDMParameter2 == theWallThicknessParameter)
            {
                return -3.0/2.0*Do*Do*PI+6.0*Do*PI*t-6.0*PI*t*t; // d2Jydt
            }
            else
                return 0.0;

        };
        theJParameter->setCurrentValue(JValue);
    }

    return theJParameter;
}



