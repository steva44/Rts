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

#include "RFibreDamageRegion.h"
#include "RFiberSection3d.h"
#include "RRCFibreContainer.h"
#include "RFiber.h"
#include "RNonlinearBeamColumnElement.h"

RFibreDamageRegion::RFibreDamageRegion() : RDamageRegion()
{
    theElement = nullptr;
    theSection = nullptr;

    yBar = 0;
    zBar = 0;
}


RFibreDamageRegion::~RFibreDamageRegion()
{

}


void RFibreDamageRegion::reset(void)
{

}


std::unordered_map<int, fibreInfo>* RFibreDamageRegion::getFiberMap()
{
    return &fiberMap;
}


double RFibreDamageRegion::getFibreStrain(int tag)
{
    auto theFibreInfo  = fiberMap.find(tag)->second;

    if(theFibreInfo.fibreTag != tag)
        qDebug()<<"ERROR"<<__FUNCTION__;

    auto strain = theFibreInfo.strain;

    return strain;
}


double RFibreDamageRegion::getFibreStress(int tag)
{
    auto theFibreInfo  = fiberMap.find(tag)->second;

    if(theFibreInfo.fibreTag != tag)
        qDebug()<<"ERROR"<<__FUNCTION__;

    auto stress = theFibreInfo.stress;

    return stress;
}


int RFibreDamageRegion::updateFibreResponses(void)
{
    yBar = 0;
    zBar = 0;

    if(!theSection || !theElement)
    {
        qCritical()<<"The damage region needs a section in "<<__FUNCTION__;
        return -1;
    }

    // Get the vector of fibres fibres
    auto fibreVec = theSection->getFibers();

    // Get the section number - need this to query the OpenSees element for the proper section responses
    auto sectionNumber = theSection->getSectionNumber();

    // Get the section responses from the element
    QVector<double> elementResponse;

    theElement->getLocalResponse(&elementResponse,"Strain",sectionNumber);

    auto numFibers = elementResponse.size()/5;

    if(numFibers != fibreVec.size())
    {
        qDebug()<<"Error getting the output from "<<theElement->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    fiberMap.clear();

    // Update the fibres
    // elementResponse(count)   = yLoc
    // elementResponse(count+1) = zLoc
    // elementResponse(count+2) = A
    // elementResponse(count+3) = stress
    // elementResponse(count+4) = strain
    // Compute the centroid while we are at it
    auto QzBar = 0.0;
    auto QyBar = 0.0;
    auto Abar = 0.0;
    for(int j = 0, count = 0 ; j<numFibers; ++j, count += 5)
    {
        auto fiber = fibreVec[j];

        auto yLoc = elementResponse[count];
        auto zLoc = elementResponse[count+1];
        auto area = elementResponse[count+2];
        auto stress = elementResponse[count+3];
        auto strain = elementResponse[count+4];

        QzBar += yLoc*area;
        QyBar += zLoc*area;
        Abar  += area;

        // Debug test
        // qDebug()<<"Stress:"<<stress;
        // qDebug()<<"Strain"<<strain;
        // if(yLoc != fiber->getFibreYCoordinate())
        //     qDebug()<<"Error in Yloc "<<__FUNCTION__<<"OS:"<<yLoc<<"Rts:"<<fiber->getFibreYCoordinate();
        // if(zLoc != fiber->getFibreZCoordinate())
        //     qDebug()<<"Error in Zloc"<<__FUNCTION__<<"OS:"<<zLoc<<"Rts:"<<fiber->getFibreZCoordinate();
        // if(area != fiber->getArea())
        //     qDebug()<<"Error in Area"<<__FUNCTION__<<"OS:"<<area<<"Rts:"<<fiber->getArea();
        // End test

        fibreInfo info;

        auto fibreTag = fiber->getObjectTag();

        info.fibreTag = fibreTag;
        info.associatedFiber = fiber;
        info.yLoc = yLoc;
        info.zLoc = zLoc;
        info.area = area;
        info.stress = stress;
        info.strain = strain;
        info.length = fibreLength;

        fiberMap.insert({fibreTag,info});
    }

     yBar = QzBar/Abar;
     zBar = QyBar/Abar;

    return 0;
}


double RFibreDamageRegion::getYBar() const
{
    return yBar;
}


double RFibreDamageRegion::getZBar() const
{
    return zBar;
}


void RFibreDamageRegion::setFibreLength(double value)
{
    fibreLength = value;
}


double RFibreDamageRegion::getFibreLength() const
{
    return fibreLength;
}


RFiberSection3d *RFibreDamageRegion::getTheSection() const
{
    return theSection;
}


void RFibreDamageRegion::setTheSection(RFiberSection3d *value)
{
    theSection = value;
}


RNonlinearBeamColumnElement *RFibreDamageRegion::getTheElement() const
{
    return theElement;
}


void RFibreDamageRegion::setTheElement(RNonlinearBeamColumnElement *value)
{
    theElement = value;
}






