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

#include "RBeamColumnComponent.h"
#include "RConstant.h"
#include "RSectionKnowledge.h"
#include "RBeamColumnMesh.h"
#include "RMesh.h"

RBeamColumnComponent::RBeamColumnComponent(QObject *parent, QString &name)
    : RComponent(parent, name)
{

    thePoint1 = nullptr;
    thePoint2 = nullptr;
    theOrientationParameter = nullptr;
    theLengthParameter = nullptr;
    theAreaParameter = nullptr;
    theMesh = nullptr;
    theCrossSection = nullptr;

    // Integration type for nonlinear analysis
    theIntegrationType = "Lobatto";

    // Create the default orientation parameter
    theDomain->createObject(objectName() + "theOrientationParameter", "RConstant");
    theOrientationParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());

    auto theOrientationLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){
        return -123.0;
    };
    theOrientationParameter->setCurrentValue(theOrientationLambda);

    // Create the length parameter
    theDomain->createObject(objectName() + "theLengthParameter", "RConstant");
    theLengthParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());  
    auto theLengthLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2){
        auto xcoord1 = thePoint1->getXCoordinateValue();
        auto xcoord2 = thePoint2->getXCoordinateValue();
        auto ycoord1 = thePoint1->getYCoordinateValue();
        auto ycoord2 = thePoint2->getYCoordinateValue();
        auto zcoord1 = thePoint1->getZCoordinateValue();
        auto zcoord2 = thePoint2->getZCoordinateValue();

        auto dx = xcoord2-xcoord1;
        auto dy = ycoord2-ycoord1;
        auto dz = zcoord2-zcoord1;

        return sqrt(dx*dx + dy*dy + dz*dz);
    };
    theLengthParameter->setCurrentValue(theLengthLambda);

}


RBeamColumnComponent::~RBeamColumnComponent()
{

}


RMesh*  RBeamColumnComponent::getMesh(void)
{
    return theMesh.get();
}


void RBeamColumnComponent::setOrientation(const QString value)
{
    auto valueDbl = value.toDouble();

    auto theOrientationLambda = [valueDbl](RParameter* theDDMParameter, RParameter* theDDMParameter2){
        return valueDbl;
    };
    theOrientationParameter->setCurrentValue(theOrientationLambda);
}


QString RBeamColumnComponent::getOrientation()
{
    auto dummyDouble = 0.0;
    return getStringParameter(&dummyDouble, theOrientationParameter);
}


double RBeamColumnComponent::getVolume()
{
    if(theAreaParameter == nullptr)
    {
        qCritical()<<"Area parameter for component "<<this->objectName()<<" is a null pointer";
        return 0.0;
    }
    return theLengthParameter->getCurrentValue()*theAreaParameter->getCurrentValue();
}


double RBeamColumnComponent::getLength()
{
    return theLengthParameter->getCurrentValue();
}


double RBeamColumnComponent::getCrossSectionArea(void)
{
    if(theAreaParameter == nullptr)
    {
        qCritical()<<"Area parameter for component "<<this->objectName()<<" is a null pointer";
        return 0.0;
    }
    return theAreaParameter->getCurrentValue();
}


RParameter* RBeamColumnComponent::getTheLengthParameter() const
{
    return theLengthParameter;
}


void RBeamColumnComponent::setPoint1(QObject *value)
{
    thePoint1 = qobject_cast<RPoint *>(value);

    // Get the nodal parameters for DDM calculations
    QList<RParameter*> theXNodalParameterList = thePoint1->getXCoordinateParameters();
    QList<RParameter*> theYNodalParameterList = thePoint1->getYCoordinateParameters();
    QList<RParameter*> theZNodalParameterList = thePoint1->getZCoordinateParameters();

    for(auto& itx : theXNodalParameterList)
    {
        itx->setNodalParameterFlag(true);
        theXNodalParameterMap.insertMulti(itx,thePoint1);
        addToParameterList(itx);
    }

    for(auto& ity : theYNodalParameterList)
    {
        ity->setNodalParameterFlag(true);
        theYNodalParameterMap.insertMulti(ity,thePoint1);
        addToParameterList(ity);
    }

    for(auto& itz : theZNodalParameterList)
    {
        itz->setNodalParameterFlag(true);
        theZNodalParameterMap.insertMulti(itz,thePoint1);
        addToParameterList(itz);
    }

}


void RBeamColumnComponent::setPoint2(QObject *value)
{
    thePoint2 = qobject_cast<RPoint *>(value);

    // Get the nodal parameters for DDM calculations
    QList<RParameter*> theXNodalParameterList = thePoint2->getXCoordinateParameters();
    QList<RParameter*> theYNodalParameterList = thePoint2->getYCoordinateParameters();
    QList<RParameter*> theZNodalParameterList = thePoint2->getZCoordinateParameters();

    for(auto& itx : theXNodalParameterList)
    {
        itx->setNodalParameterFlag(true);
        theXNodalParameterMap.insertMulti(itx,thePoint2);
        addToParameterList(itx);
    }

    for(auto& ity : theYNodalParameterList)
    {
        ity->setNodalParameterFlag(true);
        theYNodalParameterMap.insertMulti(ity,thePoint2);
        addToParameterList(ity);
    }

    for(auto& itz : theZNodalParameterList)
    {
        itz->setNodalParameterFlag(true);
        theZNodalParameterMap.insertMulti(itz,thePoint2);
        addToParameterList(itz);
    }
}


QString RBeamColumnComponent::getTheIntegrationType() const
{
    return theIntegrationType;
}


void RBeamColumnComponent::setTheIntegrationType(const QString &value)
{
    theIntegrationType = value;
}


QList<RPoint*> RBeamColumnComponent::getPointList()
{
    QList<RPoint*> pointsList{thePoint1,thePoint2};

    return pointsList;
}
