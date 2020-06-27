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

#include "RSteelSectionImporter.h"

#include <QFile>
#include <QDebug>

RSteelSectionImporter::RSteelSectionImporter(RObject* parent) : theParent(parent)
{
    theWSectionMap = nullptr;
    theHSSPipeSectionMap = nullptr;
    theCSectionMap = nullptr;
    theLSectionMap = nullptr;
    theWTSectionMap = nullptr;
    theHSSRectSectionMap = nullptr;
}


void RSteelSectionImporter::initializeWSectionMap()
{    
    // Files are added in RtsBuilding.qrc and can be updated on the fly without messing with the program
    QFile file(":Tools/Database/SteelSectionDatabase/WSECTTABLE.csv");
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }
    
    // Create new map if it does not exist
    if(theWSectionMap == nullptr)
        theWSectionMap = make_unique<std::unordered_map<std::string, WSection>>();
    
    QString data = file.readAll();
    data.remove( QRegExp("\r"));
    
    //First get each line, each line is a section except for the first
    QStringList sectionList;
    
    sectionList.append(data.split('\n'));
    
    QString firstLine = sectionList.front();
    
    QStringList sectionProperties;
    sectionProperties.append(firstLine.split(','));
    
    if(sectionProperties.size() != 30)
    {
        qCritical()<<"There are 30 parameters for importing a W section, only have "<<sectionProperties.size();
        return;
    }
    
    // Pop the front off, that only describes the various properties
    sectionList.pop_front();
    
    for(auto& it : sectionList)
    {
        WSection newWSection;
        
        QStringList sectionPropertiesList;
        sectionPropertiesList.append(it.split(','));
        
        newWSection.Shape    = sectionPropertiesList[0].toStdString();
        newWSection.Area_a        = sectionPropertiesList[1].toDouble();
        newWSection.depth_d        = sectionPropertiesList[2].toDouble();
        newWSection.webThickness_tw       = sectionPropertiesList[3].toDouble();
        newWSection.flangeWidth_bf       = sectionPropertiesList[4].toDouble();
        newWSection.flangeThickness_tf       = sectionPropertiesList[5].toDouble();
        newWSection.kdes     = sectionPropertiesList[6].toDouble();
        newWSection.kdet     = sectionPropertiesList[7].toDouble();
        newWSection.k1       = sectionPropertiesList[8].toDouble();
        newWSection.webHeight_T        = sectionPropertiesList[9].toDouble();
        newWSection.gage     = sectionPropertiesList[10].toDouble();
        newWSection.wtperft  = sectionPropertiesList[11].toDouble();
        newWSection.bfdiv2tf = sectionPropertiesList[12].toDouble();
        newWSection.hdivtw   = sectionPropertiesList[13].toDouble();
        newWSection.Ix       = sectionPropertiesList[14].toDouble();
        newWSection.Sx       = sectionPropertiesList[15].toDouble();
        newWSection.rx       = sectionPropertiesList[16].toDouble();
        newWSection.Zx       = sectionPropertiesList[17].toDouble();
        newWSection.Iy       = sectionPropertiesList[18].toDouble();
        newWSection.Sy       = sectionPropertiesList[19].toDouble();
        newWSection.ry       = sectionPropertiesList[20].toDouble();
        newWSection.Zy       = sectionPropertiesList[21].toDouble();
        newWSection.rts      = sectionPropertiesList[22].toDouble();
        newWSection.ho       = sectionPropertiesList[23].toDouble();
        newWSection.J        = sectionPropertiesList[24].toDouble();
        newWSection.Cw       = sectionPropertiesList[25].toDouble();
        newWSection.Wno      = sectionPropertiesList[26].toDouble();
        newWSection.Sw1      = sectionPropertiesList[27].toDouble();
        newWSection.Qf       = sectionPropertiesList[28].toDouble();
        newWSection.Qw       = sectionPropertiesList[29].toDouble();
        
        theWSectionMap->emplace(sectionPropertiesList[0].toStdString(),newWSection);
    }
}


void RSteelSectionImporter::initializeHSSPipeSectionMap()
{
    // Files are added in RtsBuilding.qrc and can be updated on the fly without messing with the program
    QFile file(":Tools/Database/SteelSectionDatabase/HSSPIPESECTTABLE.csv");
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }
    
    // Create new map if it does not exist
    if(theHSSPipeSectionMap == nullptr)
        theHSSPipeSectionMap = make_unique<std::unordered_map<std::string, HSSRoundSection>>();
    
    QString data = file.readAll();
    data.remove( QRegExp("\r"));
    
    //First get each line, each line is a section except for the first
    QStringList sectionList;
    
    sectionList.append(data.split('\n'));
    
    QString firstLine = sectionList.front();
    
    QStringList sectionProperties;
    sectionProperties.append(firstLine.split(','));
    
    if(sectionProperties.size() != 14)
    {
        qCritical()<<"There are 14 parameters for importing a HSS Pipe section, only have "<<sectionProperties.size();
        return;
    }
    
    // Pop the front off, that only describes the various properties
    sectionList.pop_front();
    
    for(auto& it : sectionList)
    {
        HSSRoundSection newHSSPipeSection;
        
        QStringList sectionPropertiesList;
        sectionPropertiesList.append(it.split(','));
        
        newHSSPipeSection.Shape   = sectionPropertiesList[0].toStdString();
        newHSSPipeSection.A       = sectionPropertiesList[1].toDouble();
        newHSSPipeSection.outerDiameter_OD      = sectionPropertiesList[2].toDouble();
        newHSSPipeSection.ID      = sectionPropertiesList[3].toDouble();
        newHSSPipeSection.thicknessNominal_tnom    = sectionPropertiesList[4].toDouble();
        newHSSPipeSection.thicknessDesign_tdes    = sectionPropertiesList[5].toDouble();
        newHSSPipeSection.wtperft = sectionPropertiesList[6].toDouble();
        newHSSPipeSection.Ddivt   = sectionPropertiesList[7].toDouble();
        newHSSPipeSection.I       = sectionPropertiesList[8].toDouble();
        newHSSPipeSection.S       = sectionPropertiesList[9].toDouble();
        newHSSPipeSection.r       = sectionPropertiesList[10].toDouble();
        newHSSPipeSection.Z       = sectionPropertiesList[11].toDouble();
        newHSSPipeSection.J       = sectionPropertiesList[12].toDouble();
        newHSSPipeSection.C       = sectionPropertiesList[13].toDouble();
        
        theHSSPipeSectionMap->emplace(sectionPropertiesList[0].toStdString(),newHSSPipeSection);
    }
}


void RSteelSectionImporter::initializeCSectionMap()
{
    // Files are added in RtsBuilding.qrc and can be updated on the fly without messing with the program
    QFile file(":Tools/Database/SteelSectionDatabase/CSECTTABLE.csv");
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }
    
    // Create new map if it does not exist
    if(theCSectionMap == nullptr)
        theCSectionMap = make_unique<std::unordered_map<std::string, CSection>>();
    
    QString data = file.readAll();
    data.remove( QRegExp("\r"));
    
    // First get each line, each line is a section except for the first
    QStringList sectionList;
    
    sectionList.append(data.split('\n'));
    
    QString firstLine = sectionList.front();
    
    QStringList sectionProperties;
    sectionProperties.append(firstLine.split(','));
    
    if(sectionProperties.size() != 28)
    {
        qCritical()<<"There are 28 parameters for importing a Channel section, only have "<<sectionProperties.size();
        return;
    }
    
    // Pop the front off, that only describes the various properties
    sectionList.pop_front();
    
    for(auto& it : sectionList)
    {
        CSection newCSection;
        
        QStringList sectionPropertiesList;
        sectionPropertiesList.append(it.split(','));
        
        newCSection.Shape     = sectionPropertiesList[0].toStdString();
        newCSection.A         = sectionPropertiesList[1].toDouble();
        newCSection.d         = sectionPropertiesList[2].toDouble();
        newCSection.tw        = sectionPropertiesList[3].toDouble();
        newCSection.bf        = sectionPropertiesList[4].toDouble();
        newCSection.tf        = sectionPropertiesList[5].toDouble();
        newCSection.kdes      = sectionPropertiesList[6].toDouble();
        newCSection.kdet      = sectionPropertiesList[7].toDouble();
        newCSection.T         = sectionPropertiesList[8].toDouble();
        newCSection.gage      = sectionPropertiesList[9].toDouble();
        newCSection.rts       = sectionPropertiesList[10].toDouble();
        newCSection.ho        = sectionPropertiesList[11].toDouble();
        newCSection.wtperft   = sectionPropertiesList[12].toDouble();
        newCSection.eo        = sectionPropertiesList[13].toDouble();
        newCSection.Ix        = sectionPropertiesList[14].toDouble();
        newCSection.Sx        = sectionPropertiesList[15].toDouble();
        newCSection.rx        = sectionPropertiesList[16].toDouble();
        newCSection.Zx        = sectionPropertiesList[17].toDouble();
        newCSection.Iy        = sectionPropertiesList[18].toDouble();
        newCSection.Sy        = sectionPropertiesList[19].toDouble();
        newCSection.ry        = sectionPropertiesList[20].toDouble();
        newCSection.xbar      = sectionPropertiesList[21].toDouble();
        newCSection.Zy        = sectionPropertiesList[22].toDouble();
        newCSection.xp        = sectionPropertiesList[23].toDouble();
        newCSection.J         = sectionPropertiesList[24].toDouble();
        newCSection.Cw        = sectionPropertiesList[25].toDouble();
        newCSection.robar     = sectionPropertiesList[26].toDouble();
        newCSection.H         = sectionPropertiesList[27].toDouble();
        
        theCSectionMap->emplace(sectionPropertiesList[0].toStdString(),newCSection);
    }
    
}


void RSteelSectionImporter::initializeLSectionSectionMap()
{
    // Files are added in RtsBuilding.qrc and can be updated on the fly without messing with the program
    QFile file(":Tools/Database/SteelSectionDatabase/LSECTTABLE.csv");
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }
    
    // Create new map if it does not exist
    if(theLSectionMap == nullptr)
        theLSectionMap = make_unique<std::unordered_map<std::string, LSection>>();
    
    QString data = file.readAll();
    data.remove( QRegExp("\r"));
    
    //First get each line, each line is a section except for the first
    QStringList sectionList;
    
    sectionList.append(data.split('\n'));
    
    QString firstLine = sectionList.front();
    
    QStringList sectionProperties;
    sectionProperties.append(firstLine.split(','));
    
    if(sectionProperties.size() != 29)
    {
        qCritical()<<"There are 29 parameters for importing a Angled section, only have "<<sectionProperties.size();
        return;
    }
    
    // Pop the front off, that only describes the various properties
    sectionList.pop_front();
    
    for(auto& it : sectionList)
    {
        LSection newLSection;
        
        QStringList sectionPropertiesList;
        sectionPropertiesList.append(it.split(','));
        
        newLSection.Shape   = sectionPropertiesList[0].toStdString();
        newLSection.A       = sectionPropertiesList[1].toDouble();
        newLSection.d       = sectionPropertiesList[2].toDouble();
        newLSection.b       = sectionPropertiesList[3].toDouble();
        newLSection.t       = sectionPropertiesList[4].toDouble();
        newLSection.kdes    = sectionPropertiesList[5].toDouble();
        newLSection.kdet    = sectionPropertiesList[6].toDouble();
        newLSection.wtperft = sectionPropertiesList[7].toDouble();
        newLSection.Ix      = sectionPropertiesList[8].toDouble();
        newLSection.Sx      = sectionPropertiesList[9].toDouble();
        newLSection.rx      = sectionPropertiesList[10].toDouble();
        newLSection.ybar    = sectionPropertiesList[11].toDouble();
        newLSection.Zx      = sectionPropertiesList[12].toDouble();
        newLSection.yp      = sectionPropertiesList[13].toDouble();
        newLSection.Iy      = sectionPropertiesList[14].toDouble();
        newLSection.Sy      = sectionPropertiesList[15].toDouble();
        newLSection.ry      = sectionPropertiesList[16].toDouble();
        newLSection.xbar    = sectionPropertiesList[17].toDouble();
        newLSection.Zy      = sectionPropertiesList[18].toDouble();
        newLSection.xp      = sectionPropertiesList[19].toDouble();
        newLSection.Iz      = sectionPropertiesList[20].toDouble();
        newLSection.Sz      = sectionPropertiesList[21].toDouble();
        newLSection.rz      = sectionPropertiesList[22].toDouble();
        newLSection.TANa    = sectionPropertiesList[23].toDouble();
        newLSection.Qs      = sectionPropertiesList[24].toDouble();
        newLSection.J       = sectionPropertiesList[25].toDouble();
        newLSection.Cw      = sectionPropertiesList[26].toDouble();
        newLSection.robar   = sectionPropertiesList[27].toDouble();
        newLSection.H       = sectionPropertiesList[28].toDouble();
        
        theLSectionMap->emplace(sectionPropertiesList[0].toStdString(),newLSection);
    }
        
}


void RSteelSectionImporter::initializeWTSectionSectionMap()
{
    // Files are added in RtsBuilding.qrc and can be updated on the fly without messing with the program
    QFile file(":Tools/Database/SteelSectionDatabase/WTSECTTABLE.csv");
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }
    
    // Create new map if it does not exist
    if(theWTSectionMap == nullptr)
        theWTSectionMap = make_unique<std::unordered_map<std::string, WTSection>>();
    
    QString data = file.readAll();
    data.remove( QRegExp("\r"));
    
    // First get each line, each line is a section except for the first
    QStringList sectionList;
    
    sectionList.append(data.split('\n'));
    
    QString firstLine = sectionList.front();
    
    QStringList sectionProperties;
    sectionProperties.append(firstLine.split(','));
    
    if(sectionProperties.size() != 27)
    {
        qCritical()<<"There are 27 parameters for importing a WT section, only have "<<sectionProperties.size();
        return;
    }
    
    // Pop the front off, that only describes the various properties
    sectionList.pop_front();
    
    for(auto& it : sectionList)
    {
        WTSection newWTSection;
        
        QStringList sectionPropertiesList;
        sectionPropertiesList.append(it.split(','));
        
        newWTSection.Shape    = sectionPropertiesList[0].toStdString();
        newWTSection.A        = sectionPropertiesList[1].toDouble();
        newWTSection.d        = sectionPropertiesList[2].toDouble();
        newWTSection.tw       = sectionPropertiesList[3].toDouble();
        newWTSection.bf       = sectionPropertiesList[4].toDouble();
        newWTSection.tf       = sectionPropertiesList[5].toDouble();
        newWTSection.kdes     = sectionPropertiesList[6].toDouble();
        newWTSection.kdet     = sectionPropertiesList[7].toDouble();
        newWTSection.gage     = sectionPropertiesList[8].toDouble();
        newWTSection.wtperft  = sectionPropertiesList[9].toDouble();
        newWTSection.bfdiv2tf = sectionPropertiesList[10].toDouble();
        newWTSection.ddivtw   = sectionPropertiesList[11].toDouble();
        newWTSection.Ix       = sectionPropertiesList[12].toDouble();
        newWTSection.Sx       = sectionPropertiesList[13].toDouble();
        newWTSection.rx       = sectionPropertiesList[14].toDouble();
        newWTSection.ybar     = sectionPropertiesList[15].toDouble();
        newWTSection.Zx       = sectionPropertiesList[16].toDouble();
        newWTSection.yp       = sectionPropertiesList[17].toDouble();
        newWTSection.Iy       = sectionPropertiesList[18].toDouble();
        newWTSection.Sy       = sectionPropertiesList[19].toDouble();
        newWTSection.ry       = sectionPropertiesList[20].toDouble();
        newWTSection.Zy       = sectionPropertiesList[21].toDouble();
        newWTSection.Qs       = sectionPropertiesList[22].toDouble();
        newWTSection.J        = sectionPropertiesList[23].toDouble();
        newWTSection.Cw       = sectionPropertiesList[24].toDouble();
        newWTSection.robar    = sectionPropertiesList[25].toDouble();
        newWTSection.H        = sectionPropertiesList[26].toDouble();
        
        theWTSectionMap->emplace(sectionPropertiesList[0].toStdString(),newWTSection);
    }
        
}


void RSteelSectionImporter::initializeHSSRectSectionMap()
{
    // Files are added in RtsBuilding.qrc and can be updated on the fly without messing with the program
    QFile file(":Tools/Database/SteelSectionDatabase/HSSRECTSECTTABLE.csv");
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }
    
    // Create new map if it does not exist
    if(theHSSRectSectionMap == nullptr)
        theHSSRectSectionMap = make_unique<std::unordered_map<std::string, HSSRECTSection>>();
    
    QString data = file.readAll();
    data.remove( QRegExp("\r"));
    
    // First get each line, each line is a section except for the first
    QStringList sectionList;
    
    sectionList.append(data.split('\n'));
    
    QString firstLine = sectionList.front();
    
    QStringList sectionProperties;
    sectionProperties.append(firstLine.split(','));
    
    if(sectionProperties.size() != 22)
    {
        qCritical()<<"There are 22 parameters for importing a Rectangular HSS section, only have "<<sectionProperties.size();
        return;
    }
    
    // Pop the front off, that only describes the various properties
    sectionList.pop_front();
    
    for(auto& it : sectionList)
    {
        HSSRECTSection newHSSRectSection;
        
        QStringList sectionPropertiesList;
        sectionPropertiesList.append(it.split(','));
        
        newHSSRectSection.Shape     = sectionPropertiesList[0].toStdString();
        newHSSRectSection.A         = sectionPropertiesList[1].toDouble();
        newHSSRectSection.h         = sectionPropertiesList[2].toDouble();
        newHSSRectSection.b         = sectionPropertiesList[3].toDouble();
        newHSSRectSection.tnom      = sectionPropertiesList[4].toDouble();
        newHSSRectSection.tdes      = sectionPropertiesList[5].toDouble();
        newHSSRectSection.wtperft   = sectionPropertiesList[6].toDouble();
        newHSSRectSection.bdivt     = sectionPropertiesList[7].toDouble();
        newHSSRectSection.hdivt     = sectionPropertiesList[8].toDouble();
        newHSSRectSection.Ix        = sectionPropertiesList[9].toDouble();
        newHSSRectSection.Sx        = sectionPropertiesList[10].toDouble();
        newHSSRectSection.rx        = sectionPropertiesList[11].toDouble();
        newHSSRectSection.Zx        = sectionPropertiesList[12].toDouble();
        newHSSRectSection.Iy        = sectionPropertiesList[13].toDouble();
        newHSSRectSection.Sy        = sectionPropertiesList[14].toDouble();
        newHSSRectSection.ry        = sectionPropertiesList[15].toDouble();
        newHSSRectSection.Zy        = sectionPropertiesList[16].toDouble();
        newHSSRectSection.hflat     = sectionPropertiesList[17].toDouble();
        newHSSRectSection.bflat     = sectionPropertiesList[18].toDouble();
        newHSSRectSection.J         = sectionPropertiesList[19].toDouble();
        newHSSRectSection.C         = sectionPropertiesList[20].toDouble();
        newHSSRectSection.Asurf     = sectionPropertiesList[21].toDouble();
        
        theHSSRectSectionMap->emplace(sectionPropertiesList[0].toStdString(),newHSSRectSection);
    }
    
}

