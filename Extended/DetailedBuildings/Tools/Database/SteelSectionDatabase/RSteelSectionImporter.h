#ifndef RSTEELSECTIONIMPORTER_H
#define RSTEELSECTIONIMPORTER_H
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

// This class is used to import standard steel cross-sections and accompanying information from the AISC database

#include "RObject.h"
#include "RSmartPointer.h"
#include "RSectionStructs.h"

#include <unordered_map>
#include <QtCore>

class RSteelSectionImporter
{
public:
    RSteelSectionImporter(RObject* parent);

    inline WSection getWSection(std::string& theSection)
    {
        if(theWSectionMap == nullptr)
            initializeWSectionMap();

        return (*theWSectionMap)[theSection];
    }

    inline HSSRoundSection getHSSPipeSection(std::string& theSection)
    {
        if(theHSSPipeSectionMap == nullptr)
            initializeHSSPipeSectionMap();

        return (*theHSSPipeSectionMap)[theSection];
    }

    inline CSection getCSection(std::string& theSection)
    {
        if(theCSectionMap == nullptr)
            initializeCSectionMap();

        return (*theCSectionMap)[theSection];
    }

    inline WTSection getWTSection(std::string& theSection)
    {
        if(theWTSectionMap == nullptr)
            initializeWTSectionSectionMap();

        return (*theWTSectionMap)[theSection];
    }

    inline LSection getLSection(std::string& theSection)
    {
        if(theLSectionMap == nullptr)
            initializeLSectionSectionMap();

        return (*theLSectionMap)[theSection];
    }

    inline HSSRECTSection getHSSRectSection(std::string& theSection)
    {
        if(theHSSRectSectionMap == nullptr)
            initializeHSSRectSectionMap();

        return (*theHSSRectSectionMap)[theSection];
    }

private:

    // Only want to initialize the maps once and have them ready each time we want to initialize a component
    void initializeWSectionMap();
    void initializeHSSPipeSectionMap();
    void initializeCSectionMap();
    void initializeLSectionSectionMap();
    void initializeWTSectionSectionMap();
    void initializeHSSRectSectionMap();

    unique_ptr<std::unordered_map<std::string, WSection>>        theWSectionMap;
    unique_ptr<std::unordered_map<std::string, HSSRoundSection>> theHSSPipeSectionMap;
    unique_ptr<std::unordered_map<std::string, CSection>>        theCSectionMap;
    unique_ptr<std::unordered_map<std::string, LSection>>        theLSectionMap;
    unique_ptr<std::unordered_map<std::string, WTSection>>       theWTSectionMap;
    unique_ptr<std::unordered_map<std::string, HSSRECTSection>>  theHSSRectSectionMap;

    RObject* theParent;

};

#endif // RSTEELSECTIONIMPORTER_H
