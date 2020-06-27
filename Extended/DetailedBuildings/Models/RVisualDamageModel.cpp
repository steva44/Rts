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

#include "RVisualDamageModel.h"
#include "RResponse.h"
#include "RBIM.h"
#include "RStructuralAnalysisModel.h"
#include "RFiber.h"
#include "RShellDamageMesh.h"
#include "RGenericFibreContainer.h"
#include "RQuadSectionDamageRegion.h"
#include "RRCFibreContainer.h"
#include "RQuadRCSectionDamageRegion.h"
#include "RWSectionDamageRegion.h"
#include "RConcreteMaterialKnowledge.h"
#include "RSteelMaterialKnowledge.h"
#include "RCompositeDeckingMaterialKnowledge.h"
#include "RGlulamMaterialKnowledge.h"
#include "RCLTMaterialKnowledge.h"
#include "RRCShearWallComponent.h"
#include "RRCSlabComponent.h"
#include "RRectangularRCColumnComponent.h"
#include "RWSteelBeamColumnComponent.h"
#include "RCompositePanelComponent.h"
#include "RRectangularGlulamBeamColumnComponent.h"
#include "RCLTPanelComponent.h"

#include "Element.h"
#include "DummyStream.h"
#include "Response.h"

#include <QCoreApplication>

RVisualDamageModel::RVisualDamageModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theInputParameter = nullptr;
    theBIM = nullptr;
    theResponse = nullptr;
    theCrackWidthRequiresRepair = nullptr;
    theCrackLengthRequiresRepair = nullptr;

    // Create the response object
    // Cost
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    // Create parameters used in this model
    createDatabaseOfRandomVariables();
}


RVisualDamageModel::~RVisualDamageModel()
{

}


QObject * RVisualDamageModel::getBIM() const
{
    return theBIM;
}


void RVisualDamageModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


QObject* RVisualDamageModel::getInputParameter() const
{
    return theInputParameter;
}


void RVisualDamageModel::setInputParameter(QObject *value)
{
    theInputParameter = qobject_cast<RParameter *>(value);

    if(theInputParameter)
    {
        auto theSAmodel = qobject_cast<RStructuralAnalysisModel*>(theInputParameter->parent());

        if(theSAmodel == nullptr)
        {
            qCritical()<< this->objectName()<<" needs an input from a structural analysis model";
        }
    }
}


int RVisualDamageModel::resetTime()
{
    return 0;
}


int RVisualDamageModel::evaluateModel(RGradientType theGradientType)
{
    // Keep the GUI responsive
    QCoreApplication::processEvents();

    theResponse->setCurrentValue(0.0);

    // Get the component list from the BIM or the domain
    QList<RComponent*> theComponentList;
    if(theBIM == nullptr)
    {
        theComponentList = theDomain->findChildren<RComponent*>();
    }
    else
    {
        theComponentList = theBIM->getComponentList();
    }

    // Iterate through the components
    for(auto&& component : theComponentList)
    {
        if(component->getEvaluateDamage() == false)
            continue;

        auto rectConcreteCol = qobject_cast<RRectangularRCColumnComponent*>(component);
        auto concreteSlab = qobject_cast<RRCSlabComponent*>(component);
        auto concreteShearWall = qobject_cast<RRCShearWallComponent*>(component);
        auto steelWBeamCol = qobject_cast<RWSteelBeamColumnComponent*>(component);
        auto compositeSlab = qobject_cast<RCompositePanelComponent*>(component);
        auto rectGlulamBeamCol = qobject_cast<RRectangularGlulamBeamColumnComponent*>(component);
        auto CLTpanelComponent  = qobject_cast<RCLTPanelComponent*>(component);

        // Predict the visual damage
        if(rectConcreteCol)
        {
            // Damage types are:
            //      0) undamaged
            //      1) cracking - flexural, shear, torsion
            //      2) cover spalling
            //      3) cover loss
            //      4) core crushing due to axial/flexural
            //      5) reinforcement fracture
            //      6) reinforcement buckling

            std::vector<std::string> damageTypes{"UNDAMAGED","CRACKING","SPALLING","COVERLOSS","CORECRUSHING","REBARBUCKLE","REBARFRACTURE"};

            // Get the concrete and reinforcement materials
            auto concreteMat = rectConcreteCol->getTheConcreteMaterialKnowledge();
            auto reinfMat = rectConcreteCol->getTheReinforcementMaterialKnowledge();

            // Get the concrete cover thickness parameter
            auto concreteCoverThickness = rectConcreteCol->getConcreteCoverThickness();

            // Get the damage strain limits
            auto reinfBucklingStrainLimit = reinfMat->getBucklingStrainLimit()->getCurrentValue();
            auto reinfFractureStrainLimit = reinfMat->getFractureStrainLimit()->getCurrentValue();
            auto coreCrushingStrainLimit = concreteMat->getCoreCrushingStrainLimit()->getCurrentValue();
            auto coverLossStrainLimit = concreteMat->getCoverLossStrainLimit()->getCurrentValue();
            auto spallingStrainLimit = concreteMat->getSpallingStrainLimit()->getCurrentValue();

            // Repair quantity for spalling - assume a linear relationship between the spalling strain and the cover loss strain
            // As the strain approaches the cover loss strain, the amount of cover lost will approach 100%. i.e., at the coverlossstrainlimit, all of the cover will have delaminated
            auto deltaSpall = coverLossStrainLimit-spallingStrainLimit;

            // The minimum damage area that will be considered when the strain in the cover reaches the spalling limit. In other words, at the spalling strain, assume that 20% of the cover is spalled
            auto minAreaRatio = 0.20;

            // Get the allowable crack width in m
            auto crackWidthAllow = theCrackWidthRequiresRepair->getCurrentValue();

            // Get the concrete cover thickness in m
            auto coverThickness = concreteCoverThickness->getCurrentValue();

            // Assume that the crack depth is  equal to the cover thickess
            auto crackDepth = coverThickness;

            // Get the depth and width of the column in m
            auto depth = rectConcreteCol->getCrossSectionDepth()->getCurrentValue();
            auto width = rectConcreteCol->getCrossSectionWidth()->getCurrentValue();

            // Get the damage mesh from the component
            auto theDamageMesh = component->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region, get the visual damage
            for(auto&& damageRegion : damageRegionVec)
            {

                auto beamColDamageRegion = dynamic_cast<RQuadRCSectionDamageRegion*>(damageRegion.get());

                if(!beamColDamageRegion)
                {
                    qCritical()<<"Error, requires a RC section damage region in "<<__FUNCTION__;
                    return -1;
                }

                // Update the fibre responses from the finite elements
                beamColDamageRegion->updateFibreResponses();

                // Now that the fibers are updated, onto the damage for this section
                auto fibreContainers = beamColDamageRegion->getAllFibreContainers();

                // Get the centroid of the section
                auto yBar = beamColDamageRegion->getYBar();
                auto zBar = beamColDamageRegion->getZBar();

                for(auto&& fiberContainer : fibreContainers)
                {
                    auto damageMode = fiberContainer->getGlobalDamageType();

                    if(damageMode >= 4)
                        continue;

                    // Get the fibers from the container
                    auto theReinforcementFibres = fiberContainer->theReinforcementFibres;
                    auto theCoreFibres = fiberContainer->theCoreFibres;
                    auto theCoverFibres = fiberContainer->theCoverFibres;

                    // Assume that  cracks will only occur along the reinforcement
                    // auto numCracks = static_cast<int>(fibreLength/transverseReinfSpacing);
                    auto numCracks = static_cast<int>(theReinforcementFibres.size());

                    // Resize to update the size for the cracking for the cracking vectors for the fiber container
                    fiberContainer->setNumCracks(numCracks);

                    // The visible surface area of the fiber container
                    auto surfaceArea = fiberContainer->getSurfaceArea();

                    //*********CRACKING*************
                    if(damageMode <= 1)
                    {
                        auto numReinf = theReinforcementFibres.size();

                        // Cracking is controlled by the strain in the reinforcement
                        for(int i = 0; i<numReinf; ++i)
                        {
                            auto rebarFibre = theReinforcementFibres.at(i);

                            auto rebarStrain = beamColDamageRegion->getFibreStrain(rebarFibre->getObjectTag());
                            // qDebug()<<"Rebar strain:"<<rebarStrain;

                            if(rebarStrain < 0.0)
                                continue;

                            auto areaRebarFibre = rebarFibre->getArea();

                            // Calculate the parameters for the crack width
                            // Assume that Aeff is 15 times the rebar diameter
                            auto Aeff = 15.0*areaRebarFibre;
                            auto de = coverThickness;

                            auto h1y = rebarFibre->getFibreYCoordinate()-yBar;
                            auto h1z = rebarFibre->getFibreZCoordinate()-zBar;

                            auto h2y = depth - yBar;
                            auto h2z = width - zBar;

                            auto By = fabs(h2y/h1y);
                            auto Bz = fabs(h2z/h1z);

                            // Be conservative, take the larger of the two betas
                            auto Beta = By > Bz ? By : Bz;

                            auto crackWidth = this->getColumnCrackWidth(Beta,de,rebarStrain,Aeff);
                            // qDebug()<<"Crack Width:"<<crackWidth;

                            auto maxCrackWidth = fiberContainer->getMaxCrackWidth(i);

                            if(crackWidth > crackWidthAllow && crackWidth > maxCrackWidth)
                            {
                                fiberContainer->setMaxCrackWidth(i,crackWidth);

                                if (theOutputLevel >= RObject::Maximum) {
                                    if(maxCrackWidth != 0.0)
                                        qDebug()<<"column cracking; crack width: "<<crackWidth<<" which is greater than "<<maxCrackWidth<<"; rebar strain: "<<rebarStrain<<"; rebar stress:"<< beamColDamageRegion->getFibreStress(rebarFibre->getObjectTag())/1.0e6;
                                }

                                for(auto&& coverFibre: theCoverFibres)
                                {
                                    coverFibre->setDamageType("CRACKING");
                                }
                                fiberContainer->setGlobalDamageType(1);
                                damageMode = 1;

                                // qDebug()<<"Crack width"<<crackWidth<<"crack depth"<<crackDepth<<"crack length"<<crackLength;

                                // Since we are using a fibre discretized elements, we only account for flexural and axial failure
                                // Assume the crack orientation is zero for now (flexure cracks only) - although vertical splitting cracks can still occur due to axial load
                                auto crackOrientation = 0.0;

                                // Get the crack length from the column in m
                                auto crackLength = rectConcreteCol->getCrackLength();

                                // Here we are saving the maximum crack width
                                fiberContainer->setCrackParameters(i,maxCrackWidth,crackDepth,crackLength,crackOrientation);

                                fiberContainer->setSurfaceAreaOfDamage(surfaceArea);

                                // Dont include a break here, need to check all reinforcement fibres since there could be cracking at more than one - therefore need to account for all of the cracks in that quadrant of the segment
                            }
                        }
                    }

                    //*********SPALLING*************
                    if(damageMode < 2)
                    {
                        auto prevStrainSpall = fiberContainer->getMaxStrainSpall();

                        for(auto&& coverFibre : theCoverFibres)
                        {
                            auto strain = beamColDamageRegion->getFibreStrain(coverFibre->getObjectTag());
                            // auto stress = theSectionFiberContainer->getFibreStress(coverFibre->getObjectTag());
                            // qDebug()<<"Cover strain:"<<strain;
                            // qDebug()<<"Cover stress:"<<stress;

                            // First check to see if max compressive strain in the cover is greater than the strain capacity for spalling
                            auto absStrain = fabs(strain);

                            // Check if compressive (negative)
                            if (strain < 0.0 && absStrain >= spallingStrainLimit && absStrain > prevStrainSpall)
                            {
                                // Set the new max strain
                                prevStrainSpall = strain;

                                if (theOutputLevel >= RObject::Medium) {
                                    qDebug()<<"*****Spalling***** at strain:"<<strain;
                                }

                                for(auto&& coverFibre : theCoverFibres)
                                {
                                    coverFibre->setDamageType("SPALLING");
                                }
                                fiberContainer->setGlobalDamageType(2);
                                damageMode = 2;

                                auto damageRatio = (absStrain-spallingStrainLimit)*((1.0-minAreaRatio)/deltaSpall)+ minAreaRatio;

                                if(damageRatio > 1.0)
                                {
                                    qDebug()<<"Warning: damage ratio should not be greater than 1.0 in"<<__FUNCTION__;
                                    damageRatio = 1.0;
                                }

                                auto damageArea = surfaceArea*damageRatio;

                                fiberContainer->setSurfaceAreaOfDamage(damageArea);
                                fiberContainer->setDepthOfDamage(coverThickness);

                                auto volDamage = damageArea*coverThickness;
                                fiberContainer->setVolumeOfDamage(volDamage);

                                break;
                            }
                        }
                    }

                    //*********COVER LOSS*************
                    if(damageMode < 3)
                    {
                        for(auto&& coreFibre : theCoreFibres)
                        {
                            auto strain = beamColDamageRegion->getFibreStrain(coreFibre->getObjectTag());
                            // auto stress = theSectionFiberContainer->getFibreStress(coreFibre->getObjectTag());
                            // qDebug()<<"Core strain:"<<strain;
                            // qDebug()<<"Core stress:"<<stress;

                            // Check if max compressive strain in the core is greater than the strain capacity for cover loss
                            if (strain < 0.0 && fabs(strain) > coverLossStrainLimit)
                            {
                                fiberContainer->setGlobalDamageType(3);
                                damageMode = 3;

                                if (theOutputLevel >= RObject::Medium) {
                                    qDebug()<<"*********COVER LOSS*************; strain:"<<strain;
                                }

                                for(auto&& coverFibre : theCoverFibres)
                                {
                                    coverFibre->setDamageType("COVERLOSS");
                                }

                                // The cover on the entire surface is lost
                                auto damageArea = surfaceArea;

                                fiberContainer->setSurfaceAreaOfDamage(damageArea);
                                fiberContainer->setDepthOfDamage(coverThickness);

                                auto volDamage = damageArea*coverThickness;
                                fiberContainer->setVolumeOfDamage(volDamage);

                                break;
                            }
                        }
                    }

                    //*********CORE CRUSHING*************
                    if(damageMode < 4)
                    {
                        for(auto&& coreFibre : theCoreFibres)
                        {
                            auto strain = beamColDamageRegion->getFibreStrain(coreFibre->getObjectTag());

                            // Check if max compressive strain in the core is greater than the strain capacity for cover loss
                            // Core crush limit: 0.007 - crush the core just before it goes downhill and the analysis fails to converge
                            if (strain < 0.0 && fabs(strain) > coreCrushingStrainLimit - 0.0005)
                            {
                                if (theOutputLevel >= RObject::Medium) {
                                    qDebug()<<"*********CORE CRUSHING************* LIMIT:"<<coreCrushingStrainLimit-0.0005<<" STRAIN:"<<strain;
                                }

                                for(auto&& it : theCoreFibres)
                                {
                                    it->setDamageType("CORECRUSHING");
                                }
                                for(auto&& it : fibreContainers)
                                {
                                    it->setGlobalDamageType(4); // Core crushing = 4
                                }
                                damageMode = 4;

                                break;
                            }
                        }
                    }

                    //*********REBAR FRACTURE AND BUCKLING*************
                    if(damageMode < 5)
                    {
                        // Check the reinforcement fibres for buckling
                        for(auto&& rebarFibre : theReinforcementFibres)
                        {
                            auto strain = beamColDamageRegion->getFibreStrain(rebarFibre->getObjectTag());

                            if(strain < 0.0  && fabs(strain) > reinfBucklingStrainLimit)
                            {
                                if (theOutputLevel >= RObject::Medium) {
                                    qDebug()<<"*********Rebar buckling********";
                                }

                                rebarFibre->setDamageType("REBARBUCKLE");
                                // Iterate through all fibre containers and set to failure IE total failure
                                for(auto&& it : fibreContainers)
                                {
                                    // Reinforcement buckling = 5
                                    it->setGlobalDamageType(5);
                                }
                                damageMode = 5;

                                break;
                            }
                        }
                    }

                    // Column could be in pure tension and rebar fracture is last resort, if it did not crush, then need to check for fracture
                    if(damageMode < 6)
                    {
                        // Check the reinforcement fibres for fracture
                        for(auto&& rebarFibre : theReinforcementFibres)
                        {
                            auto strain = beamColDamageRegion->getFibreStrain(rebarFibre->getObjectTag());

                            if(strain > 0.0  &&  fabs(strain) > reinfFractureStrainLimit)
                            {
                                // Iterate through all fibre containers and set to failure IE total failure
                                for(auto&& it : fibreContainers)
                                {
                                    // Reinforcement fracture = 6
                                    it->setGlobalDamageType(6);
                                }

                                rebarFibre->setDamageType("REBARFRACTURE");

                                if (theOutputLevel >= RObject::Medium) {

                                    qDebug()<<"Rebar fracture";
                                }
                                damageMode = 6;
                                break;
                            }
                        }

                    }
                }
            }
        }
        else if(concreteSlab)
        {
            // Damage types are:
            //      0) Undamaged
            //      1) Cracking - flexural, shear, torsion
            //      2) Cover spalling
            //      3) Cover loss
            //      4) Crushing due to axial/flexural
            //      5) Reinforcement fracture
            //      6) Reinforcement buckling
            //      7) Shear failure

            // Get the concrete and reinforcement materials
            auto concreteMat = concreteSlab->getTheConcreteMaterialKnowledge();
            auto reinfMat = concreteSlab->getTheReinforcementMaterialKnowledge();

            // Get the required component parameters and their values
            auto coverThicknessParameter = concreteSlab->getConcreteCoverThickness();
            auto slabThicknessParameter = concreteSlab->getThickness();
            auto slabThickness = slabThicknessParameter->getCurrentValue();
            auto coverThickness = coverThicknessParameter->getCurrentValue();
            auto longReinfDiameter = concreteSlab->getLongitudinalReinforcementDiameter();
            auto transReinfDiameter = concreteSlab->getTransverseReinforcementDiameter();
            auto longReinfSpacing = concreteSlab->getLongitudinalReinforcementSpacing();
            auto transReinfSpacing = concreteSlab->getTransverseReinforcementSpacing();

            // Area in the component "x" direction
            auto areaSx = concreteSlab->getWidth()*slabThickness;

            // Area in the component "y" direction
            auto areaSy = concreteSlab->getDepth()*slabThickness;

            // Visual damage strain limits
            auto reinfBucklingStrainLimit = reinfMat->getBucklingStrainLimit();
            auto reinfFractureStrainLimit = reinfMat->getFractureStrainLimit();
            auto coreCrushingStrainLimit = concreteMat->getCoreCrushingStrainLimit();
            auto coverLossStrainLimit = concreteMat->getCoverLossStrainLimit();
            auto spallingStrainLimit = concreteMat->getSpallingStrainLimit();
            auto spallingLimit = spallingStrainLimit->getCurrentValue();
            auto coverLimit = coverLossStrainLimit->getCurrentValue();
            auto crushingLimit = coreCrushingStrainLimit->getCurrentValue();
            auto bucklingLimit = reinfBucklingStrainLimit->getCurrentValue();
            auto fractureLimit = reinfFractureStrainLimit->getCurrentValue();

            // Get the damage mesh from the component
            auto theDamageMesh = component->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region, get the visual damage
            for(auto&& damageRegion : damageRegionVec)
            {
                auto shellDamageRegion = dynamic_cast<RShellSectionDamageRegion*>(damageRegion.get());

                if(!shellDamageRegion)
                {
                    qCritical()<<"Error, requires a shell damage region in "<<__FUNCTION__;
                    return -1;
                }

                // Get the visual damage for the RC shell
                this->getRCShellDamage(shellDamageRegion,spallingLimit,coverLimit,crushingLimit,
                                       bucklingLimit,fractureLimit,slabThickness,coverThickness,longReinfDiameter,
                                       transReinfDiameter,longReinfSpacing,transReinfSpacing,areaSx,areaSy,2,5);

            }
        }
        else if(concreteShearWall)
        {
            // Damage types are:
            //      0) Undamaged
            //      1) Cracking - flexural, shear, torsion
            //      2) Cover spalling
            //      3) Cover loss
            //      4) Crushing due to axial/flexural
            //      5) Reinforcement fracture
            //      6) Reinforcement buckling
            //      7) Shear failure

            // Get the concrete and reinforcement materials
            auto concreteMat = concreteShearWall->getTheConcreteMaterialKnowledge();
            auto reinfMat = concreteShearWall->getTheReinforcementMaterialKnowledge();

            // Get the required component parameters and their values
            auto coverThicknessParameter = concreteShearWall->getConcreteCoverThickness();
            auto wallThicknessParameter = concreteShearWall->getThickness();
            auto wallThickness = wallThicknessParameter->getCurrentValue();
            auto coverThickness = coverThicknessParameter->getCurrentValue();
            auto longReinfDiameter = concreteShearWall->getLongitudinalReinforcementDiameter();
            auto transReinfDiameter = concreteShearWall->getTransverseReinforcementDiameter();
            auto longReinfSpacing = concreteShearWall->getLongitudinalReinforcementSpacing();
            auto transReinfSpacing = concreteShearWall->getTransverseReinforcementSpacing();
            auto areaSx = concreteShearWall->getLengthValue()*wallThickness;
            auto areaSy = concreteShearWall->getHeightValue()*wallThickness;

            // Visual damage strain limits
            auto reinfBucklingStrainLimit = reinfMat->getBucklingStrainLimit();
            auto reinfFractureStrainLimit = reinfMat->getFractureStrainLimit();
            auto coreCrushingStrainLimit = concreteMat->getCoreCrushingStrainLimit();
            auto coverLossStrainLimit = concreteMat->getCoverLossStrainLimit();
            auto spallingStrainLimit = concreteMat->getSpallingStrainLimit();
            auto spallingLimit = spallingStrainLimit->getCurrentValue();
            auto coverLimit = coverLossStrainLimit->getCurrentValue();
            auto crushingLimit = coreCrushingStrainLimit->getCurrentValue();
            auto bucklingLimit = reinfBucklingStrainLimit->getCurrentValue();
            auto fractureLimit = reinfFractureStrainLimit->getCurrentValue();

            // Get the damage mesh from the component
            auto theDamageMesh = component->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region
            for(auto&& damageRegion : damageRegionVec)
            {
                auto shellDamageRegion = dynamic_cast<RShellSectionDamageRegion*>(damageRegion.get());

                if(!shellDamageRegion)
                {
                    qCritical()<<"Error, should be a shell damage region in "<<__FUNCTION__;
                    return -1;
                }

                // Get the visual damage for the RC shell
                this->getRCShellDamage(shellDamageRegion,spallingLimit,coverLimit,crushingLimit,
                                       bucklingLimit,fractureLimit,wallThickness,coverThickness,longReinfDiameter,
                                       transReinfDiameter,longReinfSpacing,transReinfSpacing,areaSx,areaSy,4,1);

            }
        }
        else if(steelWBeamCol)
        {
            // Damage types are:
            //      0) Undamaged
            //      1) Yielding

            // Get the steel material
            auto steelMat = steelWBeamCol->getTheSteelMaterial();

            // Visual damage strain limits
            auto steelYieldStress = steelMat->getYieldStress()->getCurrentValue();

            // Get the damage mesh from the component
            auto theDamageMesh = component->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region, get the visual damage
            for(auto&& damageRegion : damageRegionVec)
            {
                auto beamColDamageRegion = dynamic_cast<RWSectionDamageRegion*>(damageRegion.get());

                if(!beamColDamageRegion)
                {
                    qCritical()<<"Error, requires a W-section damage region in "<<__FUNCTION__;
                    return -1;
                }

                // Update the fibre responses from the finite elements
                beamColDamageRegion->updateFibreResponses();

                // Now that the fibers are updated, onto the damage for this section
                auto fibreContainers = beamColDamageRegion->getAllFibreContainers();

                for(auto&& fiberContainer : fibreContainers)
                {
                    auto damageState = fiberContainer->getGlobalDamageType();

                    if(damageState >= 1)
                        continue;

                    // Get the fibers from the container
                    auto theFibres = fiberContainer->getAllFibres();

                    auto numFibre = fiberContainer->numFibres();

                    //*********YIELDING*************
                    for(int i = 0; i<numFibre; ++i)
                    {
                        auto theSteelFibre = theFibres.at(i);

                        auto theSteelStress = beamColDamageRegion->getFibreStress(theSteelFibre->getObjectTag());

                        if(fabs(theSteelStress) > steelYieldStress)
                        {
                            if (theOutputLevel >= RObject::Medium) {
                                qDebug()<<"******W_COLUMN_YIELDING; Steel stress: "<<theSteelStress<<", and yield stress: "<<steelYieldStress;
                            }

                            fiberContainer->setDamageTypeString(0,"YIELDING");

                            theSteelFibre->setDamageType("YIELDING");

                            fiberContainer->setGlobalDamageType(1);
                        }
                    }
                }
            }

        }
        else if(compositeSlab)
        {
            // Damage types are:
            //      0) Undamaged
            //      1) Concrete crushes at the top surface
            //      2) Steel pan yields at the bottom

            // Get the composite material knowledge
            auto theCompositeMaterial = compositeSlab->getTheCompositeMaterial();

            // Thickness of the concrete in the decking, not the same as total thickness...
            auto concThickness = compositeSlab->getAverageConcreteToppingThickness();

            // Concrete crushing stress limit for the lightweight concrete
            auto concCrushingStressLimit = theCompositeMaterial->getLWCfc()->getCurrentValue();

            // Steel decking yield strain limit
            auto steelYieldStressLimit = theCompositeMaterial->getSteelYieldStress()->getCurrentValue();

            // Get the damage mesh from the component
            auto theDamageMesh = component->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region, get the visual damage
            for(auto&& damageRegion : damageRegionVec)
            {
                // Get the shell damage region
                auto shellDamageRegion = dynamic_cast<RShellSectionDamageRegion*>(damageRegion.get());

                if(!shellDamageRegion)
                {
                    qCritical()<<"Error, should be a shell damage region in "<<__FUNCTION__;
                    return -1;
                }

                // Get the global damage mode
                auto globalDamageMode = shellDamageRegion->getGlobalDamageType();

                // The surface area of the region in m^2
                auto surfaceArea = shellDamageRegion->getSurfaceArea();

                // Get the stress response
                auto stressResponse = shellDamageRegion->getStressResponse();

                // Calculate the stresses at the extreme fibres, in the cover, in the two x-y directions, for the top and bottom of the shell
                auto sigmaxx = stressResponse[0] + stressResponse[3];
                auto sigmayy = stressResponse[1] + stressResponse[4];

                // qDebug()<<"Stress xx2"<<stressResponse[0];
                // qDebug()<<"Stress yy2"<<stressResponse[1];
                // qDebug()<<"Stress xx"<<sigmaxx;
                // qDebug()<<"Stress yy"<<sigmayy;

                //*********CONCRETE CRUSHING*************
                if(globalDamageMode == 0)
                {
                    if(sigmaxx < 0.0 && fabs(sigmaxx) > concCrushingStressLimit )
                    {
                        if (theOutputLevel >= RObject::Medium)
                        {
                            qDebug()<<"*********Composite Slab Concrete Crushing Top XX*********";
                        }

                        shellDamageRegion->setDamageTypeString(2,"CRUSHING");
                        shellDamageRegion->setGlobalDamageType(1);

                        auto repairVolume = surfaceArea*concThickness;

                        shellDamageRegion->setSurfaceAreaOfDamage(2,surfaceArea);
                        shellDamageRegion->setVolumeOfDamage(2,repairVolume);
                        shellDamageRegion->setDepthOfDamage(2,concThickness);
                    }
                    if(sigmayy < 0.0 && fabs(sigmayy) > concCrushingStressLimit)
                    {
                        if (theOutputLevel >= RObject::Medium)
                        {
                            qDebug()<<"*********Composite Slab Concrete Crushing Top YY*********";
                        }

                        shellDamageRegion->setDamageTypeString(2,"CRUSHING");
                        shellDamageRegion->setGlobalDamageType(1);

                        auto repairVolume = surfaceArea*concThickness;

                        shellDamageRegion->setSurfaceAreaOfDamage(2,surfaceArea);
                        shellDamageRegion->setVolumeOfDamage(2,repairVolume);
                        shellDamageRegion->setDepthOfDamage(2,concThickness);
                    }

                    //*********STEEL PAN YIELDING*************
                    if(sigmaxx > 0.0 && fabs(sigmaxx) > steelYieldStressLimit)
                    {
                        if (theOutputLevel >= RObject::Medium) {
                            qDebug()<<"*********Pan Yielding Bottom XX*********";

                            qDebug()<<"Stress: "<<sigmaxx<<", should be greater than ultimate value of: "<<steelYieldStressLimit;
                        }

                        shellDamageRegion->setDamageTypeString(5,"YIELDING");
                        shellDamageRegion->setGlobalDamageType(2);

                        auto repairVolume = surfaceArea*concThickness;

                        shellDamageRegion->setSurfaceAreaOfDamage(5,surfaceArea);
                        shellDamageRegion->setVolumeOfDamage(5,repairVolume);
                        shellDamageRegion->setDepthOfDamage(5,concThickness);
                    }
                    if(sigmayy > 0.0 && fabs(sigmayy) > steelYieldStressLimit)
                    {
                        if (theOutputLevel >= RObject::Medium) {
                            qDebug()<<"*********Pan Yielding Bottom YY*********";
                        }

                        shellDamageRegion->setDamageTypeString(5,"YIELDING");
                        shellDamageRegion->setGlobalDamageType(2);

                        auto repairVolume = surfaceArea*concThickness;

                        shellDamageRegion->setSurfaceAreaOfDamage(5,surfaceArea);
                        shellDamageRegion->setVolumeOfDamage(5,repairVolume);
                        shellDamageRegion->setDepthOfDamage(5,concThickness);
                    }
                }
            }
        }
        else if(rectGlulamBeamCol)
        {
            // Damage types are:
            //      0) Undamaged
            //      1) Crushing in compression
            //      2) Fracture in tension


            // Get the glulam material knowledge
            auto glulamMat = rectGlulamBeamCol->getTheGlulamMaterial();

            // Visual damage strain limits
            // Assume that damage happens at 80% of ultimate
            auto maxCompStress = glulamMat->getUltimateCompressiveStress()->getCurrentValue()*0.80;
            auto maxTensStress = glulamMat->getUltimateTensileStress()->getCurrentValue()*0.80;

            // Get the damage mesh from the component
            auto theDamageMesh = component->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region, get the visual damage
            for(auto&& damageRegion : damageRegionVec)
            {

                auto beamColDamageRegion = dynamic_cast<RQuadSectionDamageRegion*>(damageRegion.get());

                if(!beamColDamageRegion)
                {
                    qCritical()<<"Error, requires a rectangular section damage region in "<<__FUNCTION__;
                    return -1;
                }

                // Update the fibre responses from the finite elements
                beamColDamageRegion->updateFibreResponses();

                // Now that the fibers are updated, onto the damage for this component
                auto fibreContainers = beamColDamageRegion->getAllFibreContainers();

                for(auto&& fiberContainer : fibreContainers)
                {
                    auto damageType = fiberContainer->getGlobalDamageType();

                    //Get the fibers from the container
                    auto theFibres = fiberContainer->getAllFibres();

                    auto numFibre = fiberContainer->numFibres();

                    //*********SPLITTING OR TENSILE RUPTURE*************
                    if(damageType < 1)
                    {
                        for(int i = 0; i<numFibre; ++i)
                        {
                            auto theWoodFibre = theFibres.at(i);

                            if(theWoodFibre->getDamageType() != "NONE")
                                continue;

                            auto theWoodStress = beamColDamageRegion->getFibreStress(theWoodFibre->getObjectTag());
                            // qDebug()<<"Wood Stress:"<<theWoodStress<<", and the max. compressive stress is: "<<maxCompStress<<", and the max. tensile stress is: "<<maxTensStress;

                            if(theWoodStress < 0.0)
                            {
                                if(fabs(theWoodStress) > maxCompStress)
                                {
                                    if (theOutputLevel >= RObject::Medium) {
                                        qDebug()<<"****COMPRESSIVE SPLITTING - Wood stress: "<<theWoodStress<<", and the max. compressive stress is: "<<maxCompStress;
                                    }

                                    fiberContainer->setDamageTypeString(0,"SPLITTING");

                                    theWoodFibre->setDamageType("SPLITTING");

                                    fiberContainer->setGlobalDamageType(1);
                                    damageType = 1;
                                }
                            }
                            else
                            {
                                if(fabs(theWoodStress) > maxTensStress)
                                {
                                    if (theOutputLevel >= RObject::Medium) {
                                        qDebug()<<"****TENSILE RUPTURE - Wood stress: "<<theWoodStress<<", and the max. tensile stress is: "<<maxTensStress;
                                    }

                                    fiberContainer->setDamageTypeString(0,"RUPTURE");

                                    theWoodFibre->setDamageType("RUPTURE");

                                    fiberContainer->setGlobalDamageType(1);
                                    damageType = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(CLTpanelComponent)
        {
            // Damage types are:
            //      0) Undamaged
            //      1) Compressive failure
            //      2) Tensile failure
            //      3) Shear failure

            // Get the CLT material knowledge
            auto theCLTMaterial = CLTpanelComponent->getTheMaterialKnowledge();

            // Panel thickness in m
            auto panelThickness = CLTpanelComponent->getThickness()->getCurrentValue();

            // Visual damage strain limits
            auto compressionStressLimit = theCLTMaterial->getFc0()->getCurrentValue();
            auto tensionStressLimit = theCLTMaterial->getFt0()->getCurrentValue();
            auto shearStressLimit = theCLTMaterial->getFv0()->getCurrentValue();

            // Get the damage mesh from the component
            auto theDamageMesh = component->getDamageMesh();

            if(!theDamageMesh)
            {
                qCritical()<<"Error, need a damage mesh in "<<__FUNCTION__;
                return -1;
            }

            // Then get the damage regions from the damage mesh
            auto damageRegionVec = theDamageMesh->getDamageRegionVector();

            if(damageRegionVec.empty())
            {
                qCritical()<<"Error, damage mesh has no damage regions in "<<__FUNCTION__;
                return -1;
            }

            // For each damage region, get the visual damage
            for(auto&& damageRegion : damageRegionVec)
            {
                // Get the shell damage region
                auto shellDamageRegion = dynamic_cast<RShellSectionDamageRegion*>(damageRegion.get());

                if(!shellDamageRegion)
                {
                    qCritical()<<"Error, should be a shell damage region in "<<__FUNCTION__;
                    return -1;
                }

                //The larger of the two damage types
                auto globalDamageMode = shellDamageRegion->getGlobalDamageType();

                //The surface damage is the area of the element
                auto surfaceArea = shellDamageRegion->getSurfaceArea();

                // Get the stress response from the damage region
                auto stressResponse = shellDamageRegion->getStressResponse();

                //calculate the strain at the extreme fibres, in the cover, in the two x-y directions, for the top and bottom of the shell
                auto sigmaxx = stressResponse[0] + stressResponse[3];
                auto sigmayy = stressResponse[1] + stressResponse[4];

                auto tauxy = stressResponse[5];
                auto tayxz = stressResponse[6];
                auto tauyz = stressResponse[7];

                // qDebug()<<"Stress xx2"<<stressResponse[0];
                // qDebug()<<"Stress yy2"<<stressResponse[1];
                // qDebug()<<"Stress xx"<<sigmaxx;
                // qDebug()<<"Stress yy"<<sigmayy;

                if(globalDamageMode == 0)
                {
                    //*********COMPRESSIVE FAILURE*************
                    if(sigmaxx < 0.0 && abs(sigmaxx) > compressionStressLimit)
                    {
                        if (theOutputLevel >= RObject::Medium) {
                            qDebug()<<"*********Compressive Failure XX*********";
                        }

                        shellDamageRegion->setGlobalDamageTypeString("CLTCompression");
                        shellDamageRegion->setGlobalDamageType(1);

                        auto repairVolume = surfaceArea*panelThickness;

                        shellDamageRegion->setSurfaceAreaOfDamage(3,surfaceArea);
                        shellDamageRegion->setVolumeOfDamage(3,repairVolume);
                        shellDamageRegion->setDepthOfDamage(3,panelThickness);
                    }

                    if(sigmayy < 0.0 && fabs(sigmayy) > compressionStressLimit)
                    {
                        if (theOutputLevel >= RObject::Medium) {
                            qDebug()<<"*********Compressive Failure YY*********";
                        }

                        shellDamageRegion->setGlobalDamageTypeString("CLTCompression");
                        shellDamageRegion->setGlobalDamageType(1);

                        auto repairVolume = surfaceArea*panelThickness;

                        shellDamageRegion->setSurfaceAreaOfDamage(3,surfaceArea);
                        shellDamageRegion->setVolumeOfDamage(3,repairVolume);
                        shellDamageRegion->setDepthOfDamage(3,panelThickness);
                    }

                    //*********TENSILE FAILURE*************
                    if(sigmaxx > 0.0 && abs(sigmaxx) > tensionStressLimit)
                    {
                        if (theOutputLevel >= RObject::Medium)
                        {
                            qDebug()<<"*********Tensile Failure XX*********";
                        }

                        shellDamageRegion->setGlobalDamageTypeString("CLTTension");
                        shellDamageRegion->setGlobalDamageType(2);

                        auto repairVolume = surfaceArea*panelThickness;

                        shellDamageRegion->setSurfaceAreaOfDamage(3,surfaceArea);
                        shellDamageRegion->setVolumeOfDamage(3,repairVolume);
                        shellDamageRegion->setDepthOfDamage(3,panelThickness);
                    }

                    if(sigmayy > 0.0 && fabs(sigmayy) > tensionStressLimit)
                    {
                        if (theOutputLevel >= RObject::Medium)
                        {
                            qDebug()<<"*********Tensile Failure YY*********";
                        }

                        shellDamageRegion->setGlobalDamageTypeString("CLTTension");
                        shellDamageRegion->setGlobalDamageType(2);

                        auto repairVolume = surfaceArea*panelThickness;

                        shellDamageRegion->setSurfaceAreaOfDamage(3,surfaceArea);
                        shellDamageRegion->setVolumeOfDamage(3,repairVolume);
                        shellDamageRegion->setDepthOfDamage(3,panelThickness);
                    }

                    //*********SHEAR FAILURE*************
                    if(fabs(tauxy) > shearStressLimit)
                    {
                        if (theOutputLevel >= RObject::Medium)
                        {
                            qDebug()<<"*********Shear Failure*********";
                            qDebug()<<"TauXZ: "<<tayxz<<"TauYZz: "<<tauyz<<"TauUlt: "<<shearStressLimit;
                        }

                        shellDamageRegion->setGlobalDamageTypeString("CLTShear");
                        shellDamageRegion->setGlobalDamageType(3);

                        auto repairVolume = surfaceArea*panelThickness;

                        shellDamageRegion->setSurfaceAreaOfDamage(3,surfaceArea);
                        shellDamageRegion->setVolumeOfDamage(3,repairVolume);
                        shellDamageRegion->setDepthOfDamage(3,panelThickness);
                    }
                }
            }
        }
    }

    // Set the response from this model, in this case it is just a token response
    theResponse->setCurrentValue(1.0);

    // Test
    // auto theSAmodel = qobject_cast<RStructuralAnalysisModel*>(theInputParameter->parent());
    // theSAmodel->plotDeformed();

    return 1;
}


// Reinforced concrete shell cracking parameters in a vector of size 3 where:
// Crack Width [0]
// Crack Spacing [1]
// Crack Orientation (theta) [2]
std::vector<double> RVisualDamageModel::getShellCrackParameters(const double& epsilonxx, const double& epsilonyy, const double& gammaxy, const double& reinfDiamaterxx, const double& reinfDiamateryy,
                                                                const double& reinfSpacingxx, const double& reinfSpacingyy, const double& areaSx, const double& areaSy, const double& coverThickness)
{
    // Calculate the principal strains
    auto c1 = 0.5*(epsilonxx+epsilonyy);
    auto c2 = 0.5*(epsilonxx-epsilonyy);
    auto c3 = 0.5*gammaxy;
    auto c4 = sqrt(c2*c2 + c3*c3);

    auto epsilon11 = c1 + c4;
    auto epsilon22 = c1 - c4;

    // Surface crack spacing S = 2c + Sb/5.0 + k1*k2*db/(4*pef)
    // According to CEB-FIP code
    // Where c=concrete cover, Sb = reinforcement spacing, db = reinforcement bar diameter, k1 = 0.4 for deformed bars and 0.8 for plain bars, k2 = 0.25*(eps1 + eps2)/(2*eps1)
    // pef = Area of section / area of effective embedment zone where reinforcing bars can influence the crack widths = Areas/Acef
    auto k1 = 0.4;

    // k2 = coefficient to account for strain gradient (e1 and e2 = the largest and the smallest tensile strains in the effective embedment zone, respectively)

    auto et1 = (epsilon11 > 0.0 ? epsilon11 : 0.0);
    auto et2 = (epsilon22 > 0.0 ? epsilon22 : 0.0);

    auto e1 = (et1 > et2 ? et1 : et2);    //Larger
    auto e2 = (et1 < et2 ? et1 : et2);    //Smaller

    auto k2 = 0.0;

    // Filter out the noise where e1 is very close to zero.. that means e2 is even smaller. In that case k2 should be zero
    if(fabs(e1) > 1.0e-5)
    {
        k2 = 0.25*(e1 + e2)/(2.0*e1);
    }

    // Assume Acef = 15*db
    auto pefx = areaSx/(15.0*reinfDiamaterxx);
    auto pefy = areaSy/(15.0*reinfDiamateryy);

    auto Smx = 2.0*coverThickness + reinfSpacingxx/5.0 + k1*k2*reinfDiamaterxx/(4.0*pefx);
    auto Smy = 2.0*coverThickness + reinfSpacingyy/5.0 + k1*k2*reinfDiamateryy/(4.0*pefy);

    // As a simplified approach, Smx could be taken as the spacing of the reinforcement in the x-direction and Smy as the spacing of the reinforcement bars in the y-direction
    // auto Smx = longReinfSpacing;
    // auto Smy = transReinfSpacing;

    // Surface crack width w = eps1*sm(theta) -> this assumes that cracks occur perpendicular to the tensile strain direction
    // From VECTOR2 users manual
    auto theta = 0.5*atan2(gammaxy,(epsilonxx-epsilonyy));

    // Sm(theta) = 1/(sin(theta)/Smx + cos(theta)/Smy) where theta is the angle between the principal strain axis and the line formed by the x-y strains on a Mohr's circle
    auto SmTheta = 1.0/(sin(theta)/Smx + cos(theta)/Smy);

    // Put a ceiling on Sm(theta) -- make sure it is not larger than Smx or Smy otherwise it does not make physical sense
    if(SmTheta > Smx || SmTheta > Smy)
        SmTheta = (Smx>Smy ? Smx : Smy); //Cap Sm(theta) to be the smaller of the two

    // Calculate the crack width
    auto crackWidth = epsilon11*SmTheta;

    // Return the crack width parameter vector where:
    // Crack Width [0]
    // Crack Spacing [1]
    // Crack Orientation (theta) [2]
    std::vector<double> crackParameters {crackWidth,SmTheta, theta};

    return crackParameters;
}


// Beta factor accounting for strain gradient:
double RVisualDamageModel::getColumnCrackWidth(const double& beta, const double& de, const double& epsSteel, const double& Aeff)
{
    // *1.0 for uniform strains
    // *h2/h1 for varying strains. Where h1 is the distance from the tension steel to the neutral axis and h2 is the distance from the extreme tension fiber to the neutral axis
    // de = distance from the extreme tension fiber to the center of the closest bar
    // epsSteel = strain in the reinforcing bar at crack location
    // Aeff = Effective area of concrete surrounding each bar taken as the total area of concrete in tension which as the same centroid as the tension reinforcement, divided by the number of bars.
    // When the reinforcement consists of different sizes of bars, the number of bars used to compute A is found by dividing the total area of reinforcement by the area of the largest bar

    auto Wmax = 2.2*beta*epsSteel*pow(de*Aeff,1.0/3.0);

    return Wmax;
}


// Calculates the damage of a reinforced concrete shell element (slab, wall, etc.) according to the modified compression field theory
int RVisualDamageModel::getRCShellDamage(RShellSectionDamageRegion* theDamageRegion, double& spallingLimit,
                                         double& coverLimit, double& crushingLimit, double& bucklingLimit,double& fractureLimit,
                                         double& shellThickness, double& coverThickness, double& longReinfDiameter, double& transReinfDiameter,
                                         double& longReinfSpacing,double& transReinfSpacing,double& areaSx,double& areaSy,size_t faceIndex1, size_t faceIndex2)
{

    // During a time history analysis, the cracks will open and close, and the maximum calculated crack width will be larger than the crack width at the end, i.e., the residual crack width
    // Therefore, multiply the maximum calculated crack width with a factor to estimate the residual crack width
    double residualCrackWidth = 0.4;

    // Get the current damage types - the top and bottom
    auto damageMode1 = theDamageRegion->getDamageType(faceIndex1);
    auto damageMode2 = theDamageRegion->getDamageType(faceIndex2);

    // The larger of the two damage types
    auto globalDamageMode = theDamageRegion->getGlobalDamageType();

    auto maxReinfDiameter = (longReinfDiameter>transReinfDiameter ? longReinfDiameter : transReinfDiameter);

    // Crack width is in m
    auto crackWidthAllow = theCrackWidthRequiresRepair->getCurrentValue();

    // Surface area in m2
    auto surfaceArea = theDamageRegion->getSurfaceArea();

    // Get the strain response
    auto strainResponse = theDamageRegion->getStrainResponse();

    auto epsilonxx = strainResponse[0];
    auto epsilonyy = strainResponse[1];
    auto gammaxy = strainResponse[2];
    auto kappaxx = strainResponse[3];
    auto kappayy = strainResponse[4];


    // Calculate the strain at the extreme fibres, in the cover, in the x and y directions, for the top and bottom of the shell
    auto strainCoverTop_xx = epsilonxx - kappayy*shellThickness*0.5;
    auto strainCoverBot_xx = epsilonxx + kappayy*shellThickness*0.5;

    auto strainCoverTop_yy = epsilonyy + kappaxx*shellThickness*0.5;
    auto strainCoverBot_yy = epsilonyy - kappaxx*shellThickness*0.5;

    //  qDebug()<<"Cover Strain xx"<<strainCover_xx;
    //  qDebug()<<"Cover Strain yy"<<strainCover_yy;

    //calculate the strain at rebar; in the two directions
    auto strainReinfTop_xx = epsilonxx - kappayy*(shellThickness-coverThickness)*0.5;
    auto strainReinfBot_xx = epsilonxx + kappayy*(shellThickness-coverThickness)*0.5;

    auto strainReinfTop_yy = epsilonyy + kappaxx*(shellThickness-coverThickness)*0.5;
    auto strainReinfBot_yy = epsilonyy - kappaxx*(shellThickness-coverThickness)*0.5;

    // qDebug()<<"Reinf Strain xx"<<strainReinf_xx;
    // qDebug()<<"Reinf Strain yy"<<strainReinf_yy;

    //*********CRACKING*************
    if(damageMode1 <= 1)
    {
        auto maxCrackWidthTop = theDamageRegion->getMaxCrackWidth(faceIndex1);

        auto crackParamsTop = this->getShellCrackParameters(strainCoverTop_xx,strainCoverTop_yy,gammaxy,longReinfDiameter,transReinfDiameter,longReinfSpacing,transReinfSpacing,areaSx,areaSy,coverThickness);

        auto crackWidthTop = crackParamsTop[0];

         crackWidthTop *= residualCrackWidth;
        // qDebug()<<"Crack width top"<<crackWidthTop;

        if(crackWidthTop > crackWidthAllow && crackWidthTop > maxCrackWidthTop)
        {

            if (theOutputLevel >= RObject::Maximum) {
                qDebug()<<"*********TOP CRACKING: old crack width:"<<maxCrackWidthTop<<", new crack width: "<<crackWidthTop<<"*********";
            }

            // Set the new max crack width
            maxCrackWidthTop = crackWidthTop;

            // Crack depth is taken as the cover thickness
            auto crackDepth = coverThickness;

            auto crackSpacingTop = crackParamsTop[1];

            auto crackOrientationTop = -crackParamsTop[2];

            //Crack length is down the entire length of the element
            auto lengthx = theDamageRegion->getLength11();
            auto lengthy = theDamageRegion->getLength22();

            // Take the average as the crack length
            auto crackLength = 0.5*(lengthx*lengthy);

            // Get the quantity of cracks
            auto repairVolume = crackWidthTop*crackDepth*crackLength*(crackLength/crackSpacingTop);

            theDamageRegion->setDamageTypeString(faceIndex1,"CRACKING");
            theDamageRegion->setDamageType(faceIndex1,1);
            damageMode1 = 1;

            theDamageRegion->setCrackParameters(faceIndex1,crackWidthTop,crackDepth,crackLength,crackOrientationTop);
            theDamageRegion->setSurfaceAreaOfDamage(faceIndex1, surfaceArea);
            theDamageRegion->setVolumeOfDamage(faceIndex1, repairVolume);

        }
    }

    if(damageMode2 <= 1)
    {
        auto maxCrackWidthBot = theDamageRegion->getMaxCrackWidth(faceIndex2);

        auto crackParamsBot = this->getShellCrackParameters(strainCoverBot_xx,strainCoverBot_yy,gammaxy,longReinfDiameter,transReinfDiameter,longReinfSpacing,transReinfSpacing,areaSx,areaSy,coverThickness);

        auto crackWidthBot = crackParamsBot[0];

         crackWidthBot *= residualCrackWidth;
        // qDebug()<<"Crack width bot"<<crackWidthBot;

        if(crackWidthBot > crackWidthAllow && crackWidthBot > maxCrackWidthBot)
        {
            if (theOutputLevel >= RObject::Maximum) {
                qDebug()<<"*********BOTTOM CRACKING: old crack width:"<<maxCrackWidthBot<<", new crack width: "<<crackWidthBot<<"*********";
            }

            maxCrackWidthBot = crackWidthBot;

            // Crack depth is taken as the cover thickness
            auto crackDepth = coverThickness;

            auto crackSpacingBot = crackParamsBot[1];
            auto crackOrientationBot = crackParamsBot[2];

            // Crack length is down the entire length of the element
            auto lengthx = theDamageRegion->getLength11();
            auto lengthy = theDamageRegion->getLength22();

            // Lets take the average as the crack length
            auto crackLength = 0.5*(lengthx*lengthy);

            // Get the quantity of cracks
            auto repairVolume = crackWidthBot*crackDepth*crackLength*(crackLength/crackSpacingBot);

            theDamageRegion->setDamageTypeString(faceIndex2,"CRACKING");
            theDamageRegion->setDamageType(faceIndex2,1);
            damageMode2 = 1;

            theDamageRegion->setCrackParameters(faceIndex2,crackWidthBot,crackDepth,crackLength,crackOrientationBot);
            theDamageRegion->setSurfaceAreaOfDamage(faceIndex2,surfaceArea);
            theDamageRegion->setVolumeOfDamage(faceIndex2,repairVolume);
        }
    }


    //*********SPALLING*************
    // Spalling only occurs in compression with mild buckling of rebar
    // Transverse refinforcement is exposed with potential for longitudinal reinforcement to be exposed as well
    // If the strain is positive, then compression at the bottom surface, tension at the top; spalling will happen at the bottom (compression only)
    // If the strain is negative, then tension at the bottom, compression at the top; spalling will happen at the top (compression only)
    if(damageMode1 < 2)
    {
        auto damageTop = false;

        auto maxSpallStrainTop = theDamageRegion->getMaxStrainSpall(faceIndex1);

        auto maxStrainTop = ( fabs(strainCoverTop_xx) > fabs(strainCoverTop_yy) ? fabs(strainCoverTop_xx) : fabs(strainCoverTop_yy));

        if(strainCoverTop_xx < 0.0 && fabs(strainCoverTop_xx) > spallingLimit)
            damageTop = true;

        if(strainCoverTop_yy < 0.0 && fabs(strainCoverTop_yy) > spallingLimit)
            damageTop = true;


        if(damageTop && maxStrainTop > maxSpallStrainTop)
        {
            if (theOutputLevel >= RObject::Medium) {
                qDebug()<<"*********SPALLING Top*********";
            }

            maxSpallStrainTop = maxStrainTop;

            theDamageRegion->setDamageTypeString(faceIndex1,"SPALLING");
            damageMode1 = 2;
            theDamageRegion->setDamageType(faceIndex1,2);

            // Assume the depth of the repair is the depth of cover
            auto depth = coverThickness;

            auto repairVolume = surfaceArea*depth;

            theDamageRegion->setSurfaceAreaOfDamage(faceIndex1,surfaceArea);
            theDamageRegion->setVolumeOfDamage(faceIndex1,repairVolume);
            theDamageRegion->setDepthOfDamage(faceIndex1,depth);
        }
    }

    if(damageMode2 < 2)
    {
        auto damageBot = false;

        auto maxSpallStrainBot = theDamageRegion->getMaxStrainSpall(faceIndex2);

        auto currentMaxStrainBot = ( fabs(strainCoverBot_xx) > fabs(strainCoverBot_yy) ? fabs(strainCoverBot_xx) : fabs(strainCoverBot_yy));

        if(strainCoverBot_xx < 0.0 && fabs(strainCoverBot_xx) > spallingLimit)
            damageBot = true;

        if(strainCoverBot_yy < 0.0 && fabs(strainCoverBot_yy) > spallingLimit)
            damageBot = true;

        if(damageBot && currentMaxStrainBot > maxSpallStrainBot)
        {
            if (theOutputLevel >= RObject::Medium) {
                qDebug()<<"*********SPALLING Bottom*********";
            }

            maxSpallStrainBot = currentMaxStrainBot;

            theDamageRegion->setDamageTypeString(faceIndex2,"SPALLING");
            damageMode2 = 2;
            theDamageRegion->setDamageType(faceIndex2,2);

            // Assume the depth of the repair is 0.75 of cover
            auto depth = coverThickness*0.75;

            auto repairVolume = surfaceArea*depth;

            theDamageRegion->setSurfaceAreaOfDamage(faceIndex2,surfaceArea);
            theDamageRegion->setVolumeOfDamage(faceIndex2,repairVolume);
            theDamageRegion->setDepthOfDamage(faceIndex2,depth);
        }
    }


    //*********COVER LOSS*************
    //  For cover loss, use the reinforcement strain; assume perfect bonding
    //  Assume total cover delamination exposing longitudinal reinforcement
    //  If the strain is positive, then compression at the bottom surface, tension at the top; cover loss will happen at the bottom (compression only)
    //  If the strain is negative, then tension at the bottom, compression at the top; cover loss will happen at the top (compression only)
    if(damageMode1 < 3)
    {
        auto damageTop = false;

        if(strainReinfTop_xx < 0.0 && fabs(strainReinfTop_xx) > coverLimit)
            damageTop = true;

        if(strainReinfTop_yy < 0.0 && fabs(strainReinfTop_yy) > coverLimit)
            damageTop = true;

        if(damageTop)
        {
            if (theOutputLevel >= RObject::Medium) {
                qDebug()<<"*********COVER LOSS Top*********";
            }

            theDamageRegion->setDamageTypeString(faceIndex1,"COVERLOSS");
            damageMode1 = 3;
            theDamageRegion->setDamageType(faceIndex1,3);

            // Assume that the depth of repair is the full cover thickness + space behind so that the aggregate can pass
            auto depth = coverThickness + 3.0*maxReinfDiameter + 0.01;

            auto repairVolume = surfaceArea*depth;

            theDamageRegion->setSurfaceAreaOfDamage(faceIndex1,surfaceArea);
            theDamageRegion->setVolumeOfDamage(faceIndex1,repairVolume);
            theDamageRegion->setDepthOfDamage(faceIndex1,depth);
        }
    }

    if(damageMode2 < 3)
    {
        auto damageBot = false;

        if(strainReinfBot_xx < 0.0 && fabs(strainReinfBot_xx) > coverLimit)
            damageBot = true;

        if(strainReinfBot_yy < 0.0 && fabs(strainReinfBot_yy) > coverLimit)
            damageBot = true;

        if(damageBot)
        {
            if (theOutputLevel >= RObject::Medium) {
                qDebug()<<"*********COVER LOSS Bottom*********";
            }

            theDamageRegion->setDamageTypeString(faceIndex2,"COVERLOSS");
            damageMode2 = 3;
            theDamageRegion->setDamageType(faceIndex2,3);

            // Now assume that the depth of repair is the full cover thickness + space behind so that the aggregate can pass
            auto depth = coverThickness + 3.0*maxReinfDiameter + 0.01;

            auto repairVolume = surfaceArea*depth;

            theDamageRegion->setSurfaceAreaOfDamage(faceIndex2,surfaceArea);
            theDamageRegion->setVolumeOfDamage(faceIndex2,repairVolume);
            theDamageRegion->setDepthOfDamage(faceIndex2,depth);
        }
    }


    //*********CORE CRUSHING*************
    if(globalDamageMode < 4)
    {
        // Check the strain in the core for crushing
        auto strainCore_xx = epsilonxx;
        auto strainCore_yy = epsilonyy;
        // qDebug()<<"Core Strain xx"<<strainCore_xx;
        // qDebug()<<"Core Strain yy"<<strainCore_yy;

        if((strainCore_xx < 0.0 && fabs(strainCore_xx) > crushingLimit) || (strainCore_yy < 0.0 && fabs(strainCore_yy) > crushingLimit))
        {
            if (theOutputLevel >= RObject::Medium) {
                qDebug()<<"*********CORE CRUSHING*********";
            }

            theDamageRegion->setGlobalDamageType(4);

            theDamageRegion->setGlobalDamageTypeString("CORECRUSHING");
            globalDamageMode = 4;
        }
    }


    //*********REBAR FRACTURE AND BUCKLING*************
    if(globalDamageMode < 5)
    {
        // Check the strain in the reinforcement at the top and bottom for buckling (compression)
        auto damageTop = false;
        auto damageBot = false;

        if(strainReinfTop_xx < 0.0 && fabs(strainReinfTop_xx) > bucklingLimit)
            damageTop = true;

        if(strainReinfTop_yy < 0.0 && fabs(strainReinfTop_yy) > bucklingLimit)
            damageTop = true;

        if(strainReinfBot_xx < 0.0 && fabs(strainReinfBot_xx) > bucklingLimit)
            damageBot = true;

        if(strainReinfBot_yy < 0.0 && fabs(strainReinfBot_yy) > bucklingLimit)
            damageBot = true;

        if(damageTop || damageBot)
        {
            if (theOutputLevel >= RObject::Medium) {
                qDebug()<<"*********REBAR BUCKLING*********";
            }

            theDamageRegion->setGlobalDamageType(5);

            theDamageRegion->setGlobalDamageTypeString("REBARBUCKLE");
            globalDamageMode = 5;
        }
    }

    if(globalDamageMode < 6)
    {
        // Check the strain in the reinforcement at the top and bottom for fracture (tension)
        auto damageTop = false;
        auto damageBot = false;

        if(strainReinfTop_xx > 0.0 && fabs(strainReinfTop_xx) > fractureLimit)
            damageTop = true;

        if(strainReinfTop_yy > 0.0 && fabs(strainReinfTop_yy) > fractureLimit)
            damageTop = true;

        if(strainReinfBot_xx > 0.0 && fabs(strainReinfBot_xx) > fractureLimit)
            damageBot = true;

        if(strainReinfBot_yy > 0.0 && fabs(strainReinfBot_yy) > fractureLimit)
            damageBot = true;

        if(damageTop || damageBot)
        {
            if (theOutputLevel >= RObject::Medium) {
                qDebug()<<"*********REBAR FRACTURE*********";
            }

            theDamageRegion->setGlobalDamageType(6);

            theDamageRegion->setGlobalDamageTypeString("REBARFRACTURE");
            globalDamageMode = 6;
        }
    }

    //*********SHEAR*************
    // To do: account for shear failure
    //  if(globalDamageMode < 7)
    //  {
    //      auto rho = theLongReinfRatioParameter->getCurrentValue() + theTransReinfRatioParameter->getCurrentValue();

    //      auto fc = fpc->getCurrentValue();
    //      auto d = slabThickness - coverThickness;

    //      //The concrete resistance to  shear
    //      auto Vr = 0.18*(1.0+100.0/d)*pow((100.0*rho*fc),1.0/3.0);

    //      //Get the shear stress from the element
    //      // membrane stresses at each integration point (4) total points:
    //      //
    //      //  response(0) =   sigma_xx                                                       i.e.   (11)-axial strain
    //      //  response(1) =   sigma_yy                                                       i.e.   (22)-axial strain
    //      //  response(2) =   tau_xy                                                         i.e.   (12)-in-plane shear strain
    //      //  response(3) =   sigma_xx = -z * kappa00  + eps00_membrane                      i.e.   (11)-bending curvature PLUS axial strain
    //      //  response(4) =   sigma_yy = -z * kappa11  + eps11_membrane                      i.e.   (22)-bending curvature PLUS axial strain
    //      //  response(5) =   tau_xy   = 2*epsilon01 = -z * (2*kappa01) + gamma01_membrane   i.e. 2*(12)-bending curvature PLUS in-plane shear strain
    //      //  response(6) =   tau_xz                                                         i.e.   (13)-out-of-plane shear
    //      //  response(7) =   tau_yz                                                         i.e.   (23)-out-of-plane shear
    //      //
    //      //  Out-of-plane shear constant through cross section
    //      //
    //      QVector<double> stressResponse(8);
    //      linearShellElem->getLocalResponse(&stressResponse,"Stress");
    //      auto tauXZ = stressResponse[6];
    //      auto tauYZ = stressResponse[7];

    //      if(fabs(tauXZ) > Vr || fabs(tauYZ) > Vr)
    //      {
    //          //Then the slab fails in shear, replace slab
    //          if(fabs(tauYZ) > Vr)
    //              qDebug()<<"Failed in shear. Shear capacity: "<<Vr<<" and shear stress YZ: "<<tauYZ;
    //          else
    //              qDebug()<<"Failed in shear. Shear capacity: "<<Vr<<" and shear stress XZ: "<<tauXZ;

    //          element->setDamageType("SHEARFAILURE");
    //          theDamageContainer->setCurrentDamageState(7);
    //      }
    //  }

    return 0;
}


void RVisualDamageModel::createDatabaseOfRandomVariables()
{
    // Set the crack width beyond which the repair is necessary
    theDomain->createObject(objectName() + "CrackWidthRequireRepair", "RContinuousRandomVariable");
    theCrackWidthRequiresRepair = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCrackWidthRequiresRepair->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theCrackWidthRequiresRepair->setMean(QString("0.0003")); // 0.3mm from FIB-code 2013
    theCrackWidthRequiresRepair->setCoefficientOfVariation("0.25");

    // Set the crack length beyond which the repair is necessary
    theDomain->createObject(objectName() + "CrackLengthRequireRepair", "RContinuousRandomVariable");
    theCrackLengthRequiresRepair = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theCrackLengthRequiresRepair->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theCrackLengthRequiresRepair->setMean("0.5");
    theCrackLengthRequiresRepair->setCoefficientOfVariation("0.5");

}
