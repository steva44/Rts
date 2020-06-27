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
*   - Gurvinder Gill                                                 *
*                                                                    *
*********************************************************************/

#include "RWSteelBeamColumnComponent.h"
#include "RElement.h"
#include "RPoint.h"
#include "RSteelMaterialKnowledge.h"
#include "RSectionKnowledge.h"
#include "RBeamColumnMesh.h"
#include "RLinearElasticBeamColumnMesh.h"
#include "RNonlinearBeamColumnMesh.h"
#include "RWLinearElasticSectionKnowledge.h"
#include "RWFibreSectionKnowledge.h"
#include "RContinuousRandomVariable.h"
#include "RSteelSectionImporter.h"
#include "RFrameDamageMesh.h"

#include <regex>

RWSteelBeamColumnComponent::RWSteelBeamColumnComponent(QObject *parent, QString &name)
    : RColumnComponent(parent, name)
{
    applyEarthquake = true;
    evaluateDamage = true;

    theTheta1 = nullptr;
    theTheta2 = nullptr;
    theEpsilon  = nullptr;
    theWebHeightParameter = nullptr;
    theFlangeWidthParameter = nullptr;
    theWebThicknessParameter = nullptr;
    theFlangeThicknessParameter = nullptr;
    theSteelMaterial = nullptr;
    theSectionImporter = nullptr;

    // Create random variables
    theDomain->createObject(objectName() + "Theta1", "RContinuousRandomVariable");
    theTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta1->setDistributionType(QString("Normal (mean, stdv)"));
    theTheta1->setMean(QString("1.45158"));
    theTheta1->setCoefficientOfVariation("0.124036");

    theDomain->createObject(objectName() + "Theta2", "RContinuousRandomVariable");
    theTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta2->setDistributionType(QString("Normal (mean, stdv)"));
    theTheta2->setMean(QString("13.9135"));
    theTheta2->setCoefficientOfVariation("0.285169");

    theDomain->createObject(objectName() + "Epsilon", "RContinuousRandomVariable");
    theEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
    theEpsilon->setMean(QString("0.0"));
    theEpsilon->setStandardDeviation("0.489489");

    // Initialize parameters created in this component
    numElements = 4;
    numSections = 3;
    numFibresInFlange = 2;
    numFibresInWeb = 4;

    theSectionImporter = make_unique<RSteelSectionImporter>(this);

    // This overrides the global mesh option
    // Set local mesh option to zero to use global mesh option
    // Local mesh options:
    //      1) Linear-elastic mesh
    //      2) Fibre discretized displacement based elements
    //      3) Fibre discretized force based elements
    localMeshOption = 2;

}


RWSteelBeamColumnComponent::~RWSteelBeamColumnComponent()
{

}


void RWSteelBeamColumnComponent::setWebHeight(QObject *value)
{
    theWebHeightParameter = qobject_cast<RParameter *>(value);
    theWebHeightParameter->setNodalParameterFlag(true);
    addToParameterList(theWebHeightParameter);
}


void RWSteelBeamColumnComponent::setWebThickness(QObject *value)
{
    theWebThicknessParameter = qobject_cast<RParameter *>(value);
    theWebThicknessParameter->setNodalParameterFlag(true);
    addToParameterList(theWebThicknessParameter);
}


void RWSteelBeamColumnComponent::setFlangeWidth(QObject *value)
{
    theFlangeWidthParameter = qobject_cast<RParameter *>(value);
    theFlangeWidthParameter->setNodalParameterFlag(true);
    addToParameterList(theFlangeWidthParameter);
}


void RWSteelBeamColumnComponent::setFlangeThickness(QObject *value)
{
    theFlangeThicknessParameter = qobject_cast<RParameter *>(value);
    theFlangeThicknessParameter->setNodalParameterFlag(true);
    addToParameterList(theFlangeThicknessParameter);
}


int RWSteelBeamColumnComponent::createMesh()
{

    // Do not create mesh until the end-points and other geometry parameters are set
    if (thePoint1 == nullptr || thePoint2 == nullptr || theOrientationParameter->getCurrentValue()==-123.0
            || theFlangeWidthParameter->getCurrentValue()== 0.0 || theWebHeightParameter->getCurrentValue()== 0.0  || theFlangeThicknessParameter->getCurrentValue()== 0.0   || theWebThicknessParameter->getCurrentValue()== 0.0 ) {

        return 0;
    }

    // Re-meshing is too risky... (what if we delete shared nodes?)  Hence, just say:
    if (meshWasCreated == true) {
        qCritical() << "Changing the property of a component that has been meshed is risky! Instead, please reload the input file with the new values.";
        return -1;
    }

    // Check to make sure all measurements coming in are valid
    if(theFlangeWidthParameter->getCurrentValue() <=  0.0 || theWebHeightParameter->getCurrentValue()  <=  0.0  || theFlangeThicknessParameter->getCurrentValue()  <=  0.0
            || theWebThicknessParameter->getCurrentValue()  <=  0.0  )
    {
        qCritical()<<"One of the section dimensions in RWSteelBeamColumnComponent is less than or equal to zero";
        return 0;
    }


    // Create the steel material
    theSteelMaterial = make_unique<RSteelMaterialKnowledge>(theDomain, this->objectName(), "NULL");

    // 1) Linear-elastic mesh
    if (localMeshOption == 1)
    {

        // Create knowledge about the cross-section
        theCrossSection = make_unique<RWLinearElasticSectionKnowledge>(theDomain, this->objectName(), theWebHeightParameter, theWebThicknessParameter,theFlangeWidthParameter,theFlangeThicknessParameter);

        // Create the finite element mesh
        theMesh = make_unique<RLinearElasticBeamColumnMesh>(theDomain,this->objectName(),theSteelMaterial.get(),
                                                            theCrossSection.get(),thePoint1,thePoint2,theOrientationParameter,
                                                            theParameterList,theElementList);

        auto res = theMesh->createMesh();

        if(res!= 0)
        {
            qCritical()<<"Error: creating mesh in "<<this->objectName();
            return -1;
        }

        // In linear elastic mesh, the number of elements should be one
        numElements = 1;

    }
    // 2) Fibre discretized displacement based elements
    // 3) Fibre discretized force based elements
    else if (localMeshOption == 2 || localMeshOption ==3)
    {

        QString steelMaterialModel = "RSteel02";

        theCrossSection = make_unique<RWFibreSectionKnowledge>(theDomain,this->objectName(),
                                                               numSections,
                                                               theFlangeWidthParameter,
                                                               theFlangeThicknessParameter,
                                                               theWebHeightParameter,
                                                               theWebThicknessParameter,
                                                               numFibresInFlange,
                                                               numFibresInWeb,
                                                               steelMaterialModel,
                                                               theSteelMaterial.get());
        QString elementType;

        if(localMeshOption == 2)
            elementType = "RDispBeamColumn3d";
        else
            elementType = "RForceBeamColumn3d";


        theMesh = make_unique<RNonlinearBeamColumnMesh>(theDomain, this->objectName(), elementType, theElementList,theParameterList,numElements,theCrossSection.get(),
                                                        thePoint1,thePoint2,theOrientationParameter);

        auto beamColMesh = qobject_cast<RNonlinearBeamColumnMesh*>(theMesh.get());

        beamColMesh->setTheRhoParameter(theSteelMaterial->getDensity());

        //For columns set coordinate transformation as P-Delta
        beamColMesh->setCrdTransformationType("PDeltaCrdTransf3d");

        beamColMesh->setIntegrationType(this->getTheIntegrationType());

        auto res = theMesh->createMesh();

        if(res!= 0)
        {
            qCritical()<<"Error: creating mesh in "<<this->objectName();
            return -1;
        }

    }
    else
    {
        return 0;
    }

    // Create the visual damage mesh
    if(theDamageMesh)
        theDamageMesh.reset(nullptr);

    theDamageMesh = make_unique<RFrameDamageMesh>(this);

    auto res2 = theDamageMesh->createDamageMesh();
    if(res2 != 0)
    {
        qCritical() << "Failed to create damage mesh in "<<this->objectName();
        return res2;
    }

    // Approve structural analysis if a mesh is created (this is a data member of the base class RComponent)
    meshWasCreated = true;


    return 0;
}


void RWSteelBeamColumnComponent::setTheWSection(const WSection &value)
{

    theWSection = value;

    double inchToMeterConversion = 0.0254;

    // Create the parameters needed - in this case the flange height, flange thickness, web height, web thickness

    // First create standard parameters, they can be set later with custom if so desired
    theDomain->createObject(objectName() + "theWBeamFlangeWidthParameter", "RConstant");
    RConstant* theFlangeWidthParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
    theFlangeWidthParameter->setCurrentValue(theWSection.flangeWidth_bf*inchToMeterConversion);

    this->setFlangeWidth(theFlangeWidthParameter);

    theDomain->createObject(objectName() + "theWBeamFlangeThicknessParameter", "RConstant");
    RConstant*  theFlangeThicknessParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
    theFlangeThicknessParameter->setCurrentValue(theWSection.flangeThickness_tf*inchToMeterConversion);

    this->setFlangeThickness(theFlangeThicknessParameter);

    theDomain->createObject(objectName() + "theWBeamWebHeightParameter", "RConstant");
    RConstant* theWebHeightParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
    theWebHeightParameter->setCurrentValue(theWSection.webHeight_T*inchToMeterConversion);

    this->setWebHeight(theWebHeightParameter);

    theDomain->createObject(objectName() + "theWBeamWebThicknessParameter", "RConstant");
    RConstant* theWebThicknessParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
    theWebThicknessParameter->setCurrentValue(theWSection.webThickness_tw*inchToMeterConversion);

    this->setWebThickness(theWebThicknessParameter);

}


QString RWSteelBeamColumnComponent::getWSectionType(void)
{
    return QString::fromStdString(theWSection.Shape);
}


void RWSteelBeamColumnComponent::setWSectionType(QString value)
{

    // Just in case in comes in lower case and with spaces
    value.remove(" ");
    value = value.toUpper();

    auto stdCrossSection = value.toStdString();

    std::regex rxW("^(W|M|S|HP)[0-9]+");

    WSection theWSection;

    if(std::regex_search(stdCrossSection,rxW))
    {
        // Create W section
        theWSection = theSectionImporter->getWSection(stdCrossSection);

        if(theWSection.Shape == "NULL")
        {
            qCritical()<<"Error, could not get the section type "<<value<<" from database";
            return;
        }

        this->setTheWSection(theWSection);
    }

    return;
}


double RWSteelBeamColumnComponent::getConstructionCost()
{
    double theta1 = theTheta1->getCurrentValue();
    double theta2 = theTheta2->getCurrentValue();
    double epsilon = theEpsilon->getCurrentValue();

    double d = theWebHeightParameter->getCurrentValue();
    double h = theLengthParameter->getCurrentValue();

    double cost = theta1 * h + theta2 * d + epsilon;

    cost = pow(cost, 3.0);

    return cost;
}


double RWSteelBeamColumnComponent::getNumberOfWorkerHoursForConstruction()
{
    return 7.0;
}


double RWSteelBeamColumnComponent::getVolumeOfSteel()
{
    return this->getSectionArea()*getLength();
}


RSteelMaterialKnowledge* RWSteelBeamColumnComponent::getTheSteelMaterial(void) const
{
    return theSteelMaterial.get();
}


double RWSteelBeamColumnComponent::getSectionArea(void)
{
    if(theCrossSection == nullptr)
    {
        qDebug()<<"This section needs to be meshed first, returning a zero area "<<__FUNCTION__;
        return 0.0;
    }

    auto area = theCrossSection->getArea()->getCurrentValue();

    return area;
}

