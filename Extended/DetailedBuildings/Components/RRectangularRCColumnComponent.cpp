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
*   - Abbas Javaherian                                               *
*                                                                    *
*********************************************************************/

#include "RRectangularRCColumnComponent.h"
#include "RContinuousRandomVariable.h"
#include "RSectionForceDeformation.h"
#include "RResponse.h"
#include "RParameter.h"
#include "RMeshGenerator.h"
#include "RLinearElasticBeamColumnMesh.h"
#include "RNonlinearBeamColumnMesh.h"
#include "RRectangularLinearElasticSectionKnowledge.h"
#include "RRectangularRCFibreSectionKnowledge.h"
#include "RDispBeamColumn3d.h"
#include "RLinearElasticBrickMesh.h"
#include "RQuadRCSectionDamageRegion.h"
#include "RFrameDamageMesh.h"

#ifdef IFC
#include "RTopoMesh.h"
#include "RTopoEdge.h"
#include "RTopoFace.h"
#endif

RRectangularRCColumnComponent::RRectangularRCColumnComponent(QObject *parent, QString &name)
    : RColumnComponent(parent, name), RConcreteComponent()
{  
    applyEarthquake = true;
    evaluateDamage = true;

    theCrossSectionDepthParameter = nullptr;
    theCrossSectionWidthParameter = nullptr;
    theTheta1 = nullptr;
    theTheta2 = nullptr;
    theTheta3 = nullptr;
    theEpsilon = nullptr;
    theSigma = nullptr;

    // Initialize some default parameters for this component
    theConcreteStrength = "C25";
    longitudinalRebarDesignation = "15M";
    transverseRebarDesignation  = "10M";

    transverseReinforcementSpacing = 0.2;
    typeTransverseReinforcement = "S1";
    
    // Number of elements and sections
    numElements = 3;
    numSections = 2;

    // Number of fibres along the core and cover
    numFibresInCoreAlong12 = 2;
    numFibresInCoreAlong23 = 2;
    numFibresInCoverAlong12 = 1;
    numFibresInCoverAlong23 = 1;

    // Number of rebar along the directions
    rebarAlong12 = 4;
    rebarAlong23 = 4;
    rebarAlong34 = 4;
    rebarAlong41 = 4;

    // This overrides the global mesh option
    // Set local mesh option to zero to use global mesh option
    // Local mesh options:
    //      1) Linear-elastic mesh
    //      2) Fibre discretized displacement based elements
    //      3) Fibre discretized force based elements
    //      4) Brick elements
    localMeshOption = 2;

}


RRectangularRCColumnComponent::~RRectangularRCColumnComponent()
{

}


void RRectangularRCColumnComponent::setCrossSectionDepth(QObject* value)
{
    theCrossSectionDepthParameter = qobject_cast<RParameter*>(value);
    theCrossSectionDepthParameter->setNodalParameterFlag(true);
    addToParameterList(theCrossSectionDepthParameter);
}


void RRectangularRCColumnComponent::setCrossSectionWidth(QObject* value)
{
    theCrossSectionWidthParameter = qobject_cast<RParameter*>(value);
    theCrossSectionWidthParameter->setNodalParameterFlag(true);
    addToParameterList(theCrossSectionWidthParameter);
}


int RRectangularRCColumnComponent::createComponentSpecificParameters(void)
{
    if( longitudinalRebarDesignation == "NULL" || transverseRebarDesignation == "NULL")
    {
        qCritical() << "Need to specify the type of reinforcement in"<<this->objectName();
        return -1;
    }

    if(!theTransverseReinforcementRatioParameter && transverseReinforcementSpacing == 0.0)
    {
        qCritical()<<"Either a transverse reinforcement ratio must be given or the transverse reinforcement spacing must be specified ";
        return -1;
    }


    //For transverse reinforcement -- could specify the spacing and designation or could give a ratio and designation
    if(!theTransverseReinforcementRatioParameter && transverseReinforcementSpacing != 0.0)
    {
        theDomain->createObject(objectName() + "theTransverseReinforcementRatioParameter", "RConstant");
        theTransverseReinforcementRatioParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
        auto theTransverseReinforcementRatioLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {

            auto columnVolume = this->getVolume();

            auto columnLength = this->getLength();
            auto numHoops = columnLength/transverseReinforcementSpacing;

            auto hoopLength = 2.0*theCrossSectionWidthParameter->getCurrentValue() +  2.0*theCrossSectionDepthParameter->getCurrentValue() + 0.1;

            auto transverseRebarArea =  this->getRebarArea(transverseRebarDesignation);

            auto volumeOfTransRebar = numHoops*hoopLength*transverseRebarArea;

            return volumeOfTransRebar/columnVolume;
        };
        theTransverseReinforcementRatioParameter->setCurrentValue(theTransverseReinforcementRatioLambda);
    }


    if(!theLongitudinalReinforcementRatioParameter)
    {
        theDomain->createObject(objectName() + "theLongitudinalRatioParameter", "RConstant");
        theLongitudinalReinforcementRatioParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
        theLongitudinalReinforcementRatioParameter->setCurrentValue(0.02);
    }


    // Set the concrete cover thickness
    if(!concreteCoverThickness)
    {
        theDomain->createObject(objectName() + "CoverConcreteThickness", "RContinuousRandomVariable");
        auto theConcreteCoverThicknessRV = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theConcreteCoverThicknessRV->setDistributionType(QString("Lognormal (zeta, sigma)"));
        theConcreteCoverThicknessRV->setMean(QString("0.05"));
        theConcreteCoverThicknessRV->setCoefficientOfVariation("0.10");
        concreteCoverThickness = qobject_cast<RParameter *>(theConcreteCoverThicknessRV);
    }


    if(!theTheta1)
    {
        theDomain->createObject(objectName() + "CrackLengthModelParameter1", "RContinuousRandomVariable");
        theTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta1->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta1->setMean(QString("7.10"));
        theTheta1->setCoefficientOfVariation("0.016");

        // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
        theParameterList << theTheta1;
    }


    if(!theTheta2)
    {
        theDomain->createObject(objectName() + "CrackLengthModelParameter2", "RContinuousRandomVariable");
        theTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta2->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta2->setMean(QString("-0.31"));
        theTheta2->setCoefficientOfVariation("0.19");

        // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
        theParameterList << theTheta2;
    }


    if(!theTheta3)
    {
        theDomain->createObject(objectName() + "CrackLengthModelParameter3", "RContinuousRandomVariable");
        theTheta3 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theTheta3->setDistributionType(QString("Normal (mean, stdv)"));
        theTheta3->setMean(QString("0.86"));
        theTheta3->setCoefficientOfVariation("0.21");

        // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
        theParameterList << theTheta3;
    }


    if(!theSigma)
    {
        theDomain->createObject(objectName() + "CrackLengthModelSigma", "RContinuousRandomVariable");
        theSigma = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theSigma->setDistributionType(QString("Normal (mean, stdv)"));
        theSigma->setMean(QString("0.95"));
        theSigma->setCoefficientOfVariation("0.06");

        // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
        theParameterList << theSigma;
    }


    if(!theEpsilon)
    {
        theDomain->createObject(objectName() + "CrackLengthModelEpsilon", "RContinuousRandomVariable");
        theEpsilon = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theEpsilon->setDistributionType(QString("Normal (mean, stdv)"));
        theEpsilon->setMean(QString("0.0"));
        theEpsilon->setStandardDeviationValue(theSigma->getCurrentValue());

        // IMPORTANT (add the parameter to the list, otherwise the analysis won't include it)
        theParameterList << theEpsilon;
    }

    return 0;

}


int RRectangularRCColumnComponent::createMesh()
{

    // Do not create mesh until the end-points and other geometry parameters are set
    if (thePoint1==nullptr || thePoint2==nullptr || theOrientationParameter->getCurrentValue()==-123.0 || theCrossSectionWidthParameter->getCurrentValue()==0.0 || theCrossSectionDepthParameter->getCurrentValue()==0.0
            || getConcreteStrengthValue()==0.0)
        return 0;

    // Re-meshing is too risky... (what if we delete shared nodes?)  Hence, just say:
    if (meshWasCreated == true) {
        qCritical() << "Changing the property of a component that has been meshed is risky! Instead, please reload the input file with the new values.";
        return -1;
    }

    // Create the parameters that are specific to this component
    auto result = createComponentSpecificParameters();

    if(result != 0)
        return -1;

    // Create the rebar steel material
    theReinforcementMaterialKnowledge = make_unique<RSteelMaterialKnowledge>(theDomain, this->objectName(), "NULL");

    // Create knowledge for the unconfined concrete material
    theConcreteMaterialKnowledge = make_unique<RConcreteMaterialKnowledge>(theDomain, this->objectName(), theConcreteStrength);

    // Create knowledge for the unconfined concrete material
    theConfinedConcreteMaterialKnowledge = make_unique<RConfinedConcreteMaterialKnowledge>(theDomain, this->objectName()+"Confined", theConcreteStrength);

    // Set the confinement parameters
    theConfinedConcreteMaterialKnowledge->setFyh(theReinforcementMaterialKnowledge->getYieldStress());
    theConfinedConcreteMaterialKnowledge->setRhoTrans(this->getTheTransverseReinforcementRatioParameter());

    // When the cover fibres fail, the strength in those fibres reduces rapidly, and the analysis fails to converge
    // Therefore, use an elastic bilinear model for the cover fibres
    QString coverConcreteMaterialModel = "RElasticBilin";
    // QString coverConcreteMaterialModel = "RConcrete02";

    QString coreConcreteMaterialModel = "RConcrete02";
    QString steelMaterialModel = "RSteel02";

    if(localMeshOption == 0)
        localMeshOption = (int)theGlobalMeshOption->getCurrentValue();

    //Now create the mesh
    // 1) Linear-elastic mesh
    if (localMeshOption == 1)
    {
        // Create knowledge about the cross-section
        theCrossSection = make_unique<RRectangularLinearElasticSectionKnowledge>(theDomain, this->objectName(), theCrossSectionDepthParameter, theCrossSectionWidthParameter);

        // Create the finite element mesh
        theMesh = make_unique<RLinearElasticBeamColumnMesh>(theDomain,this->objectName(),theConcreteMaterialKnowledge.get(),
                                                            theCrossSection.get(),thePoint1,thePoint2,theOrientationParameter,
                                                            theParameterList,theElementList);

        auto res = theMesh->createMesh();

        if(res!= 0)
        {
            qCritical()<<"Error: creating mesh in "<<this->objectName();
            return -1;
        }

        //In linear elastic mesh, the number of elements should be one
        numElements = 1;

    }
    // 2) Fibre discretized displacement based elements
    // 3) Fibre discretized force based elements
    else if (localMeshOption == 2 || localMeshOption == 3)
    {
        theConcreteMaterialKnowledge->setTheConcreteMaterialModel(coverConcreteMaterialModel);
        theConfinedConcreteMaterialKnowledge->setTheConcreteMaterialModel(coreConcreteMaterialModel);
        theReinforcementMaterialKnowledge->setTheSteelMaterialModel(steelMaterialModel);

        theCrossSection = make_unique<RRectangularRCFibreSectionKnowledge>(theDomain,this,numSections,numFibresInCoreAlong12,numFibresInCoreAlong23,numFibresInCoverAlong12, numFibresInCoverAlong23,
                                                                           rebarAlong12,rebarAlong23,rebarAlong34,rebarAlong41);

        QString elementType;

        if(localMeshOption == 2)
            elementType = "RDispBeamColumn3d";
        else
            elementType = "RForceBeamColumn3d";

        theMesh = make_unique<RNonlinearBeamColumnMesh>(theDomain, this->objectName(), elementType, theElementList,theParameterList,numElements,theCrossSection.get(),
                                                        thePoint1,thePoint2,theOrientationParameter);

        auto beamColMesh = qobject_cast<RNonlinearBeamColumnMesh*>(theMesh.get());

        beamColMesh->setTheRhoParameter(theConcreteMaterialKnowledge->getDensity());

        //For columns set coordinate transformation as P-Delta
        beamColMesh->setCrdTransformationType("PDeltaCrdTransf3d");

        beamColMesh->setIntegrationType(this->getTheIntegrationType());

        auto res = theMesh->createMesh();

        if(res!= 0 || theElementList.isEmpty())
        {
            qCritical()<<"Error: creating mesh in "<<this->objectName();
            return -1;
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

    }
    // 4) Brick elements
    else if (localMeshOption == 4)
    {

        // The number of elements along the various directions
        int numElementsU = 2;
        int numElementsV = 2;
        int numElementsW = 10;

        // Create knowledge about the cross-section
        theCrossSection = make_unique<RRectangularLinearElasticSectionKnowledge>(theDomain, this->objectName(), theCrossSectionDepthParameter, theCrossSectionWidthParameter);

        // Create the finite element mesh
        theMesh = make_unique<RLinearElasticBrickMesh>(theDomain,this->objectName(),theConcreteMaterialKnowledge.get(),
                                                       theCrossSection.get(),thePoint1,thePoint2,numElementsU,numElementsV,
                                                       numElementsW,theParameterList,theElementList);

        auto res = theMesh->createMesh();

        if(res!= 0)
        {
            qCritical()<<"Error: creating mesh in "<<this->objectName();
            return -1;
        }
    }
    else
    {
        qCritical()<<"Error: the mesh option "<<localMeshOption<<" is not supported in "<<this->objectName();
        return 0;
    }


    // Approve structural analysis if a mesh is created (this is a data member of the base class RComponent)
    meshWasCreated = true;

    //Add the parameters that are sitting in the concrete component base-class
    addToParameterList(theConcreteMaterialKnowledge->getParameterList());
    addToParameterList(theReinforcementMaterialKnowledge->getParameterList());

    return 0;
}


double  RRectangularRCColumnComponent::getAxialLoad()
{
    //Get the axial load of the first element
    return theMesh->getAxialLoad(0);
}


double RRectangularRCColumnComponent::getCrackLength()
{
    // Do not calculate crack length until the cross section dimensions are set
    if (theCrossSectionWidthParameter->getCurrentValue()==0.0 || theCrossSectionDepthParameter->getCurrentValue()==0.0 ||
            getConcreteStrengthValue()==0.0 || concreteCoverThickness == 0) {

        return 0;
    }

    if (theTheta1 == nullptr || theTheta2 == nullptr || theTheta3 == nullptr || theEpsilon == nullptr || theSigma == nullptr)
        return 0;

    double crackLengthRatio = 0.0;

    auto drift = this->getDrift();
    auto axialLoad = this->getAxialLoad();

    double theta1 = theTheta1->getCurrentValue();
    double theta2 = theTheta2->getCurrentValue();
    double theta3 = theTheta3->getCurrentValue();
    double epsilon = theEpsilon->getCurrentValue();
    double sigma = theSigma->getCurrentValue();

    auto crossSectionDepth = theCrossSectionDepthParameter->getCurrentValue();
    auto crossSectionWidth = theCrossSectionWidthParameter->getCurrentValue();
    crackLengthRatio = theta1 * drift * 100.0 + theta2 * fabs(axialLoad)/getConcreteStrengthValue()/(crossSectionDepth-concreteCoverThickness->getCurrentValue())/(crossSectionWidth-concreteCoverThickness->getCurrentValue()) * 100.0 + theta3 * theLongitudinalReinforcementRatioParameter->getCurrentValue() * 100.0 + sigma * epsilon;

    return crossSectionDepth*crackLengthRatio;
}


QString RRectangularRCColumnComponent::getTypeTransverseReinforcement(void) const
{
    return typeTransverseReinforcement;
}


double RRectangularRCColumnComponent::getVolume()
{
    return theLengthParameter->getCurrentValue()*this->getSurfaceAreaCrossSection();
}


double RRectangularRCColumnComponent::getSurfaceAreaWidth()
{
    return theCrossSectionWidthParameter->getCurrentValue()*theLengthParameter->getCurrentValue();
}


double RRectangularRCColumnComponent::getSurfaceAreaDepth()
{
    return theCrossSectionDepthParameter->getCurrentValue()*theLengthParameter->getCurrentValue();
}


double RRectangularRCColumnComponent::getSurfaceAreaCrossSection()
{
    return theCrossSectionDepthParameter->getCurrentValue()*theCrossSectionWidthParameter->getCurrentValue();
}


double RRectangularRCColumnComponent::getTotalVolumeOfReinforcement()
{
    return this->getVolumeOfLongitudinalReinforcement()+this->getVolumeOfTransverseReinforcement();
}


double RRectangularRCColumnComponent::getVolumeOfConcrete()
{
    return this->getVolume()-getTotalVolumeOfReinforcement();
}


double RRectangularRCColumnComponent::getVolumeOfSteel()
{
    return this->getTotalVolumeOfReinforcement();
}


double RRectangularRCColumnComponent::getSurfaceArea(void)
{
    return getSurfaceAreaWidth()*2.0+getSurfaceAreaDepth()*2.0+getSurfaceAreaCrossSection()*2.0;
}


int RRectangularRCColumnComponent::createTopologicalMesh(void)
{

    // Get the nonlinear beam column mesh
    auto nonLinMesh = qobject_cast<RNonlinearBeamColumnMesh*>(theMesh.get());

    if(nonLinMesh == nullptr)
        return  -1;

    auto theTopoMesh = make_shared<RTopoMesh>(theMesh.get());
    theMesh->setTopoMesh(theTopoMesh);


    QVector<RNode*> theNodeVector(numElements+1);

    for (int i=0; i<theElementList.size(); ++i)
    {
        // Element name
        auto theElement = theElementList.at(i);

        auto theNode1 = theElement->getNode(0);
        auto theNode2 = theElement->getNode(1);

        theNodeVector[i] = theNode1;
        theNodeVector[i+1] = theNode2;

        auto nonlinElem = qobject_cast<RNonlinearBeamColumnElement*>(theElement);

        if(nonlinElem == nullptr)
        {
            qCritical()<<"Error: element in "<<this->objectName() <<" must be a nonlinear element";
            return -1;
        }

        auto sections = nonlinElem->getTheSectionForceDeformation();
        auto sectionLoc = nonlinElem->getSectionLocations();


        if(sections->empty())
        {
            qDebug()<<"Sections are empty in "<<__FUNCTION__;
            return -1;
        }

        auto numberSections = sections->size();

        auto length = nonlinElem->getLength();

        for(size_t i = 0; i < numberSections; ++i)
        {
            auto section = sections->at(i);
            auto vertexLoop = section->getSectionVertexLoop();

            double xi1 = static_cast<double>(i)/static_cast<double>(numberSections);
            double xi2 = static_cast<double>(i+1)/static_cast<double>(numberSections);

            //For each section will have to create the 4 faces. i.e., a box. Each corner vertex should be linked to a node or section.
            auto l1 = vertexLoop.at(0);
            auto l2 = vertexLoop.at(1);
            auto l3 = vertexLoop.at(2);
            auto l4 = vertexLoop.at(3);

            auto vertex1 = theTopoMesh->addVertex(theNode1,theNode2, xi1, l1.getYCoordinateValue(), l1.getZCoordinateValue(), 0.0);
            auto vertex2 = theTopoMesh->addVertex(theNode1,theNode2, xi1, l2.getYCoordinateValue(), l2.getZCoordinateValue(), 0.0);
            auto vertex3 = theTopoMesh->addVertex(theNode1,theNode2, xi1, l3.getYCoordinateValue(), l3.getZCoordinateValue(), 0.0);
            auto vertex4 = theTopoMesh->addVertex(theNode1,theNode2, xi1, l4.getYCoordinateValue(), l4.getZCoordinateValue(), 0.0);

            auto vertex5 = theTopoMesh->addVertex(theNode1,theNode2, xi2, l1.getYCoordinateValue(), l1.getZCoordinateValue(), 0.0);
            auto vertex6 = theTopoMesh->addVertex(theNode1,theNode2, xi2, l2.getYCoordinateValue(), l2.getZCoordinateValue(), 0.0);
            auto vertex7 = theTopoMesh->addVertex(theNode1,theNode2, xi2, l3.getYCoordinateValue(), l3.getZCoordinateValue(), 0.0);
            auto vertex8 = theTopoMesh->addVertex(theNode1,theNode2, xi2, l4.getYCoordinateValue(), l4.getZCoordinateValue(), 0.0);

            auto edge1 = theTopoMesh->addEdge(vertex1,vertex2);
            auto edge2 = theTopoMesh->addEdge(vertex2,vertex6);
            auto edge3 = theTopoMesh->addEdge(vertex6,vertex7);
            auto edge4 = theTopoMesh->addEdge(vertex5,vertex1);
            auto edge5 = theTopoMesh->addEdge(vertex3,vertex2);
            auto edge6 = theTopoMesh->addEdge(vertex7,vertex3);
            auto edge7 = theTopoMesh->addEdge(vertex6,vertex7);
            auto edge8 = theTopoMesh->addEdge(vertex3,vertex4);
            auto edge9 = theTopoMesh->addEdge(vertex4,vertex8);
            auto edge10 = theTopoMesh->addEdge(vertex8,vertex7);
            auto edge11 = theTopoMesh->addEdge(vertex1,vertex4);
            auto edge12 = theTopoMesh->addEdge(vertex8,vertex5);

            RTopoFace face1(edge1,edge2,edge3,edge4);
            RTopoFace face2(edge5,edge2,edge7,edge6);
            RTopoFace face3(edge8,edge9,edge10,edge6);
            RTopoFace face4(edge11,edge9,edge12,edge4);

            auto rcdamagecontainer = dynamic_cast<RQuadRCSectionDamageRegion*>(section->getSegmentDamageRegion().get());

            face1.setTheDamageRegion(&rcdamagecontainer->quadrantI);
            face2.setTheDamageRegion(&rcdamagecontainer->quadrantII);
            face3.setTheDamageRegion(&rcdamagecontainer->quadrantIII);
            face4.setTheDamageRegion(&rcdamagecontainer->quadrantIV);

            theTopoMesh->addFace(face1);
            theTopoMesh->addFace(face2);
            theTopoMesh->addFace(face3);
            theTopoMesh->addFace(face4);
        }

    }

    auto numNodes = theNodeVector.size();

    //Check to make sure the number elements is equal to numberOfElementsAlongU
    if(numNodes-1 != numElements)
    {
        qCritical()<<"Error in topo meshing, vectors are not consistent in RRectangularRCColumnComponent";
        return -1;
    }

    auto dD = 0.5*theCrossSectionDepthParameter->getCurrentValue();
    auto dW = 0.5*theCrossSectionWidthParameter->getCurrentValue();

    // Mesh out the top and bottom caps
    {
        RNode* bottomNode = theNodeVector[0];
        auto vertex1 = theTopoMesh->addVertex(bottomNode,-dD,-dW,0.0);
        auto vertex2 = theTopoMesh->addVertex(bottomNode, dD,-dW,0.0);
        auto vertex3 = theTopoMesh->addVertex(bottomNode, dD, dW,0.0);
        auto vertex4 = theTopoMesh->addVertex(bottomNode,-dD, dW,0.0);

        auto edge1 = theTopoMesh->addEdge(vertex1,vertex2);
        auto edge2 = theTopoMesh->addEdge(vertex2,vertex3);
        auto edge3 = theTopoMesh->addEdge(vertex3,vertex4);
        auto edge4 = theTopoMesh->addEdge(vertex4,vertex1);

        RTopoFace face(edge1,edge2,edge3,edge4);

        theTopoMesh->addFace(face);
    }

    {
        RNode* topNode = theNodeVector.back();
        auto vertex1 = theTopoMesh->addVertex(topNode,-dD,-dW,0.0);
        auto vertex2 = theTopoMesh->addVertex(topNode, dD,-dW,0.0);
        auto vertex3 = theTopoMesh->addVertex(topNode, dD, dW,0.0);
        auto vertex4 = theTopoMesh->addVertex(topNode,-dD, dW,0.0);

        auto edge1 = theTopoMesh->addEdge(vertex1,vertex2);
        auto edge2 = theTopoMesh->addEdge(vertex2,vertex3);
        auto edge3 = theTopoMesh->addEdge(vertex3,vertex4);
        auto edge4 = theTopoMesh->addEdge(vertex4,vertex1);

        RTopoFace face(edge1,edge2,edge3,edge4);

        theTopoMesh->addFace(face);
    }

    return 0;
}


double RRectangularRCColumnComponent::getVolumeOfTransverseReinforcement()
{
    return theTransverseReinforcementRatioParameter->getCurrentValue()*this->getVolume();
}


double RRectangularRCColumnComponent::getVolumeOfLongitudinalReinforcement()
{
    return theLongitudinalReinforcementRatioParameter->getCurrentValue()*this->getVolume();
}


int RRectangularRCColumnComponent::getNumberOfLongitudinalRebar()
{
    // If fibre discretized, the number of rebar is specified
    if(localMeshOption == 2 || localMeshOption == 3)
    {
        return rebarAlong12 + rebarAlong23 + rebarAlong34 + rebarAlong41;
    }

    // Else estimate from the reinforcement ratio
    auto totalAreaOfLongRebar = this->getSurfaceAreaCrossSection()*theLongitudinalReinforcementRatioParameter->getCurrentValue();

    auto rebarArea = this->getRebarArea(longitudinalRebarDesignation);

    return std::ceil(totalAreaOfLongRebar/rebarArea);

}



