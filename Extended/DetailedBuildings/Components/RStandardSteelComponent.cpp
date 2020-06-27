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

#include "RStandardSteelComponent.h"
#include "RSectionStructs.h"
#include "RWSteelBeamColumnComponent.h"
#include "RHSSRoundBeamColumnComponent.h"
#include "RConstant.h"

#include <regex>

RStandardSteelComponent::RStandardSteelComponent(QObject *parent, QString &name)
    : RComponent(parent, name)
{
    thePoint1 = nullptr;
    thePoint2 = nullptr;
    theSectionImporter = nullptr;
    theOrientationParameter = nullptr;

    theCrossSectionType = "NULL";
    theSectionImporter = make_unique<RSteelSectionImporter>(this);
    theOrientationDouble = -123.0;
}


RStandardSteelComponent::~RStandardSteelComponent()
{

}


int RStandardSteelComponent::createComponent()
{
    // Check to ensure that all objects are given
    if( thePoint1 != nullptr && thePoint2 != nullptr && theCrossSectionType != "NULL" && theOrientationDouble != -123.0)
    {
        // Just in case in comes in lower case and with spaces
        theCrossSectionType.remove(" ");
        theCrossSectionType.toUpper();

        auto stdCrossSection = theCrossSectionType.toStdString();

        std::regex rxW("^(W|M|S|HP)[0-9]+");
        std::regex rxWT("^(WT|MT|ST)[0-9]+");
        std::regex rxC("^(C|MC)[0-9]+");
        std::regex rxL("^L[0-9]+");
        std::regex rxHSSPIPE("(^HSS([0-9]|\.)+X([0-9]|\.)+$|^Pipe[0-9]+)");
        std::regex rxHSSRECT("^HSS([0-9]|-|\/)+X([0-9]|\/|-)+X([0-9]|\/|-)+");

        if(std::regex_search(stdCrossSection,rxW))  // Create W section
        {
            WSection theWSection = theSectionImporter->getWSection(stdCrossSection);

            if(theWSection.Shape == "NULL")
            {
                qCritical()<<"Error, could not get the type "<<theCrossSectionType<<" from database";
                return -1;
            }

            // If no error create a Steel IBeam component
            theDomain->createObject(this->objectName()+theCrossSectionType,"RWSteelBeamColumnComponent");
            RWSteelBeamColumnComponent* theSteelIBeam = qobject_cast<RWSteelBeamColumnComponent*>(theDomain->getLastAddedObject());
            if(theSteelIBeam == nullptr)
            {
                qCritical()<<"Could not create a "<<this->objectName()+theCrossSectionType<<" in RDomain";
                return -1;
            }

            // Set the standard things like points, mesh option, and orientation
            theSteelIBeam->setPoint1(thePoint1);
            theSteelIBeam->setPoint2(thePoint2);
            theSteelIBeam->setOrientation(getOrientation());

            // Create the parameters needed - in this case the flange height, flange thickness, web height, web thickness

            // First create standard parameters, they can be set later with custom if so desired
            theDomain->createObject(objectName() + "theWBeamFlangeWidthParameter", "RConstant");
            RConstant* theFlangeWidthParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
            theFlangeWidthParameter->setCurrentValue(theWSection.flangeWidth_bf*inchToMeterConversion);

            theDomain->createObject(objectName() + "theWBeamFlangeThicknessParameter", "RConstant");
            RConstant*  theFlangeThicknessParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
            theFlangeThicknessParameter->setCurrentValue(theWSection.flangeThickness_tf*inchToMeterConversion);

            theDomain->createObject(objectName() + "theWBeamWebHeightParameter", "RConstant");
            RConstant* theWebHeightParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
            theWebHeightParameter->setCurrentValue(theWSection.webHeight_T*inchToMeterConversion);

            theDomain->createObject(objectName() + "theWBeamWebThicknessParameter", "RConstant");
            RConstant* theWebThicknessParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
            theWebThicknessParameter->setCurrentValue(theWSection.webThickness_tw*inchToMeterConversion);

            //Populate values from table, make sure to convert from inch to meter!
            theSteelIBeam->setFlangeWidth(theFlangeWidthParameter);
            theSteelIBeam->setFlangeThickness(theFlangeThicknessParameter);
            theSteelIBeam->setWebHeight(theWebHeightParameter);
            theSteelIBeam->setWebThickness(theWebThicknessParameter);

            //Set the section to the component so we can get the standard properties if so desired
            theSteelIBeam->setTheWSection(theWSection);

        }
        else if(std::regex_search(stdCrossSection,rxWT))  // Create WT section
        {
            WTSection theWTSection = theSectionImporter->getWTSection(stdCrossSection);

            if(theWTSection.Shape == "NULL")
            {
                qCritical()<<"Error, could not get the type "<<theCrossSectionType<<" from database";
                return -1;
            }

            // To do: to implement
            qCritical()<<"Need to implement the "<<theCrossSectionType<<" in "<<__FUNCTION__;
        }
        else if(std::regex_search(stdCrossSection,rxC))  // Create C section
        {
            CSection theCSection = theSectionImporter->getCSection(stdCrossSection);

            if(theCSection.Shape == "NULL")
            {
                qCritical()<<"Error, could not get the type "<<theCrossSectionType<<" from database";
                return -1;
            }

            // To do: to implement
            qCritical()<<"Need to implement the "<<theCrossSectionType<<" in "<<__FUNCTION__;
        }
        else if(std::regex_search(stdCrossSection,rxL))  // Create L section
        {
            LSection theLSection = theSectionImporter->getLSection(stdCrossSection);

            if(theLSection.Shape == "NULL")
            {
                qCritical()<<"Error, could not get the type "<<theCrossSectionType<<" from database";
                return -1;
            }

            // To do: to implement
            qCritical()<<"Need to implement the "<<theCrossSectionType<<" in "<<__FUNCTION__;
        }
        else if(std::regex_search(stdCrossSection,rxHSSRECT))  // Create HSS Rect section
        {
            HSSRECTSection theHSSRectSection = theSectionImporter->getHSSRectSection(stdCrossSection);

            if(theHSSRectSection.Shape == "NULL")
            {
                qCritical()<<"Error, could not get the type "<<theCrossSectionType<<" from database";
                return -1;
            }

            // To do: to implement
            qCritical()<<"Need to implement the "<<theCrossSectionType<<" in "<<__FUNCTION__;
        }
        else if(std::regex_search(stdCrossSection,rxHSSPIPE))   // Create HSSPipe section
        {
            HSSRoundSection theHSSPipeSection = theSectionImporter->getHSSPipeSection(stdCrossSection);

            if(theHSSPipeSection.Shape == "NULL")
            {
                qCritical()<<"Error, could not get the type "<<theCrossSectionType<<" from database";
                return -1;
            }

            // If no error create a Steel IBeam component
            theDomain->createObject(this->objectName()+theCrossSectionType,"RHSSRoundBeamColumnComponent");
            RHSSRoundBeamColumnComponent* theHSSRoundMember = qobject_cast<RHSSRoundBeamColumnComponent*>(theDomain->getLastAddedObject());
            if(theHSSRoundMember == nullptr)
            {
                qCritical()<<"Could not create a "<<this->objectName()+theCrossSectionType<<" in RDomain";
                return -1;
            }

            // Set the standard things like points, mesh option, and orientation
            theHSSRoundMember->setPoint1(thePoint1);
            theHSSRoundMember->setPoint2(thePoint2);
            theHSSRoundMember->setOrientation(getOrientation());

            // Create the parameters needed - in this case the thickness and diameter
            theDomain->createObject(objectName() + "theHSSRoundDiameterParameter", "RConstant");
            RConstant* theHSSRoundDiameterParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
            theHSSRoundDiameterParameter->setCurrentValue(theHSSPipeSection.outerDiameter_OD*inchToMeterConversion);

            theDomain->createObject(objectName() + "theHSSRoundThicknessParameter", "RConstant");
            RConstant* theHSSRoundThicknessParameter = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
            theHSSRoundThicknessParameter->setCurrentValue(theHSSPipeSection.thicknessDesign_tdes*inchToMeterConversion);

            // Populate values from table, make sure to convert from inch to meter!
            theHSSRoundMember->setDiameter(theHSSRoundDiameterParameter->objectName());
            theHSSRoundMember->setWallThickness(theHSSRoundThicknessParameter->objectName());

            // Set the section to the component so we can get the standard properties if so desired
            theHSSRoundMember->setStandardSection(theHSSPipeSection);
        }
        else
        {
            qCritical()<<"The standard steel section of type :"<<theCrossSectionType<<" is not found.";
            return -1;
        }
    }

    return 0;
}




