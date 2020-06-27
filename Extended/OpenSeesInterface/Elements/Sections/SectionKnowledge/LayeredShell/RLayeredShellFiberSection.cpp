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

#include "RLayeredShellFiberSection.h"
#include "RNDMaterial.h"
#include "RParameter.h"

#include "NDMaterial.h"
#include "LayeredShellFiberSection.h"

#include <array>
#include <sstream>

RLayeredShellFiberSection::RLayeredShellFiberSection(QObject *parent, QString name)
    : RSectionForceDeformation(parent, name)
{     
    numCoverLayers = 1;
    numCoreLayers = 2;

    theCoverThickness = nullptr;
    theShellThickness = nullptr;
    theTransReinfLayerThickness = nullptr;
    theLongReinfLayerThickness = nullptr;

    theCoverLayerConcreteMaterial = nullptr;
    theCoreLayerConcreteMaterial = nullptr;
    theTransLayerReinfMaterial = nullptr;
    theLongLayerReinfMaterial = nullptr;
    theSectionForceDeformation = nullptr;
}


RLayeredShellFiberSection::~RLayeredShellFiberSection()
{
}


int RLayeredShellFiberSection::update(void)
{
    theSectionForceDeformation.reset();
    return 0;
}


SectionForceDeformation* RLayeredShellFiberSection::getTheSectionForceDeformation(void)
{
    if(theSectionForceDeformation == nullptr)
    {
        this->createLayeredShellFiberSection();
    }

    return theSectionForceDeformation.get();
}


int RLayeredShellFiberSection::createLayeredShellFiberSection(void)
{
    if(theCoverLayerConcreteMaterial == nullptr)
    {
        qDebug()<<__FUNCTION__<<" needs a cover concrete layer material in"<<this->objectName();
        return -1;
    }
    if(theCoreLayerConcreteMaterial == nullptr)
    {
        qDebug()<<__FUNCTION__<<" needs a core concrete layer material in"<<this->objectName();
        return -1;
    }
    if(theTransLayerReinfMaterial == nullptr)
    {
        qDebug()<<__FUNCTION__<<" needs a transverse reinforcement layer material in"<<this->objectName();
        return -1;
    }
    if(theLongLayerReinfMaterial == nullptr)
    {
        qDebug()<<__FUNCTION__<<" needs a longitudinal reinforcement layer material in"<<this->objectName();
        return -1;
    }
    if(theCoverThickness == nullptr)
    {
        qDebug()<<__FUNCTION__<<" needs a concrete cover thickness parameter in"<<this->objectName();
        return -1;
    }
    if(theShellThickness == nullptr)
    {
        qDebug()<<__FUNCTION__<<" needs a total shell thickness parameter in"<<this->objectName();
        return -1;
    }
    if(theTransReinfLayerThickness == nullptr)
    {
        qDebug()<<__FUNCTION__<<" needs a transverse reinforcement layer thickness parameter in"<<this->objectName();
        return -1;
    }
    if(theLongReinfLayerThickness == nullptr)
    {
        qDebug()<<__FUNCTION__<<" needs a longitudinal reinforcement layer thickness parameter in"<<this->objectName();
        return -1;
    }

    return this->intialize();
}


int RLayeredShellFiberSection::intialize(void)
{

    //Get the OpenSees Materials
    NDMaterial* longMat = theLongLayerReinfMaterial->getTheNDMaterial();
    if(longMat == nullptr)
    {
        qDebug()<<"Could not create an OpenSees material layer for"<<theLongLayerReinfMaterial->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    NDMaterial* transMat = theTransLayerReinfMaterial->getTheNDMaterial();
    if(transMat == nullptr)
    {
        qDebug()<<"Could not create an OpenSees material layer for"<<theTransLayerReinfMaterial->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    NDMaterial* coreMat = theCoreLayerConcreteMaterial->getTheNDMaterial();
    if(coreMat == nullptr)
    {
        qDebug()<<"Could not create an OpenSees material layer for"<<theCoreLayerConcreteMaterial->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    NDMaterial* coverMat = theCoverLayerConcreteMaterial->getTheNDMaterial();
    if(coverMat == nullptr)
    {
        qDebug()<<"Could not create an OpenSees material layer for"<<theCoverLayerConcreteMaterial->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    auto longReinfLayerThickness = theLongReinfLayerThickness->getCurrentValue();
    auto TransReinfLayerThickness =  theTransReinfLayerThickness->getCurrentValue();

    //Calculate the thicknesses of the various layers
    auto coverLayerThickness = theCoverThickness->getCurrentValue()/(double)numCoverLayers;
    auto coreLayerThickness = (theShellThickness->getCurrentValue()-(theCoverThickness->getCurrentValue() + longReinfLayerThickness + TransReinfLayerThickness)*2.0)/(double)numCoreLayers;

    //    qDebug()<<"coverLayerThickness"<<coverLayerThickness;
    //    qDebug()<<"coreLayerThickness"<<coreLayerThickness;
    //    qDebug()<<"longReinfLayerThickness"<<longReinfLayerThickness;
    //    qDebug()<<"TransReinfLayerThickness"<<TransReinfLayerThickness;

    //********* Cover Concrete Layer i *********
    //********* Cover Concrete Layer : *********
    //********* Cover Concrete Layer n *********
    //********* Longitudinal Reinf Layer *********
    //********* Transverse Reinf Layer *********
    //********* Core Concrete Layer i *********
    //********* Core Concrete Layer i+1 *********
    //********* Core Concrete Layer  : *********
    //********* Core Concrete Layer  m *********
    //********* Longitudinal Reinf Layer *********
    //********* Transverse Reinf Layer *********
    //********* Cover Concrete Layer i *********
    //********* Cover Concrete Layer : *********
    //********* Cover Concrete Layer n *********

    //Two times the cover layers, two rebar layers (trans. and long.) times two, and number of core layers gives total layer count
    auto nLayers = numCoverLayers*2 + 2*2 + numCoreLayers;

    NDMaterial **theMatFibers   = new NDMaterial*[nLayers];
    double *thickness = new double[nLayers];

    //Create the cover concrete layer(s)
    int count = 0;
    for(int i = count; i<numCoverLayers; ++i, ++count)
    {
        thickness[i] = coverLayerThickness;
        theMatFibers[i] = coverMat;
    }

    //Create the reinforcement layers
    thickness[count] = longReinfLayerThickness;
    theMatFibers[count] = longMat;
    ++count;

    thickness[count] = TransReinfLayerThickness;
    theMatFibers[count] = transMat;
    ++count;

    //Create the confined concrete layer(s)
    for(int i = count; i<numCoverLayers+2+numCoreLayers; ++i, ++count)
    {
        thickness[i] = coreLayerThickness;
        theMatFibers[i] = coreMat;
    }

    //Create the reinforcement layer on other side
    thickness[count] = longReinfLayerThickness;
    theMatFibers[count] = longMat;
    ++count;

    thickness[count] = TransReinfLayerThickness;
    theMatFibers[count] = transMat;
    ++count;

    //Create the cover layer on other side
    for(int i = count; i< nLayers; ++i, ++count)
    {
        thickness[i] = coverLayerThickness;
        theMatFibers[i] = coverMat;
    }

    //    LayeredShellFiberSection(   int tag,
    //                                int iLayers,
    //                                double *thickness,
    //                                NDMaterial **fibers );
    theSectionForceDeformation = std::make_unique<LayeredShellFiberSection>(objectTag,nLayers,thickness,theMatFibers);

    //    auto thickCheck = 0.0;
    //    for(int i = 0; i< nLayers; ++i)
    //    {
    //        thickCheck += thickness[i];
    //    }
    //    qDebug()<<"Calculated thickness: "<<thickCheck<<" should be close to: "<< theShellThickness->getCurrentValue();

    delete [] thickness;
    delete [] theMatFibers;

    return 0;
}


int RLayeredShellFiberSection::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(2);

    auto tag = std::to_string(this->objectTag);

    //Two times the cover layers, two rebar layers (trans. and long.) times two, and number of core layers gives total layer count
    auto numLayers = numCoverLayers*2 + 2*2 + numCoreLayers;

    std::string nLayers   = std::to_string(numLayers);

    //Get the OpenSees Materials
    NDMaterial* longMat = theLongLayerReinfMaterial->getTheNDMaterial();
    if(longMat == nullptr)
    {
        qDebug()<<"Could not get the OpenSees material layer for "<<theLongLayerReinfMaterial->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    NDMaterial* transMat = theTransLayerReinfMaterial->getTheNDMaterial();
    if(transMat == nullptr)
    {
        qDebug()<<"Could not get the OpenSees material layer for "<<theTransLayerReinfMaterial->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    NDMaterial* coreMat = theCoreLayerConcreteMaterial->getTheNDMaterial();
    if(coreMat == nullptr)
    {
        qDebug()<<"Could not get the OpenSees material layer for "<<theCoreLayerConcreteMaterial->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    NDMaterial* coverMat = theCoverLayerConcreteMaterial->getTheNDMaterial();
    if(coverMat == nullptr)
    {
        qDebug()<<"Could not get the OpenSees material layer for "<<theCoverLayerConcreteMaterial->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    auto longReinfLayerThickness = theLongReinfLayerThickness->getCurrentValue();
    auto TransReinfLayerThickness =  theTransReinfLayerThickness->getCurrentValue();

    //Calculate the thicknesses of the various layers
    auto coverLayerThickness = theCoverThickness->getCurrentValue()/static_cast<double>(numCoverLayers);
    auto coreLayerThickness = (theShellThickness->getCurrentValue()-(theCoverThickness->getCurrentValue() + longReinfLayerThickness + TransReinfLayerThickness)*2.0)/static_cast<double>(numCoreLayers);

    //Strings
    std::string coverMatTag  = std::to_string(coverMat->getTag());
    std::string coreMatTag   = std::to_string(coreMat->getTag());
    std::string longMatTag   = std::to_string(longMat->getTag());
    std::string transMatTag  = std::to_string(transMat->getTag());

    auto longRefThickStr    = std::to_string(longReinfLayerThickness);
    auto transRefThickStr   = std::to_string(TransReinfLayerThickness);
    auto coverThickStr      = std::to_string(coverLayerThickness);
    auto coreThickStr       = std::to_string(coreLayerThickness);

    if(flag == 1) //tcl file
    {
        //out<<"# section LayeredShell $sectionTag $nLayers $matTag1 $thickness1...$matTagn $thicknessn"<<std::endl;
        out<<"section LayeredShell "<<tag<<" "<<nLayers<<" ";

        //Create the cover concrete layer(s) on the first side
        for(int i = 0; i<numCoverLayers; ++i)
        {
            out<<coverMatTag<<" "<<coverThickStr<<" ";
        }

        //Create the reinforcement layer on one side
        out<<longMatTag<<" "<<longRefThickStr<<" "<<transMatTag<<" "<<transRefThickStr<<" ";

        //Create the confined concrete layer(s)
        for(int i = 0; i<numCoreLayers; ++i)
        {
            out<<coreMatTag<<" "<<coreThickStr<<" ";
        }

        //Create the reinforcement layer on other side
        out<<longMatTag<<" "<<longRefThickStr<<" "<<transMatTag<<" "<<transRefThickStr<<" ";

        //Create the cover layer on the other side
        for(int i = 0; i<numCoverLayers; ++i)
        {
            out<<coverMatTag<<" "<<coverThickStr<<" ";
        }

        out<<std::endl;

    }
    else if(flag == 2) //Python
    {
        out<<"Not supported in Python "<<__FUNCTION__<<std::endl;
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


RParameter *RLayeredShellFiberSection::getTheLongReinfLayerThickness() const
{
    return theLongReinfLayerThickness;
}


void RLayeredShellFiberSection::setTheLongReinfLayerThickness(RParameter *value)
{
    theLongReinfLayerThickness = value;
}


RParameter *RLayeredShellFiberSection::getTheTransReinfLayerThickness() const
{
    return theTransReinfLayerThickness;
}


void RLayeredShellFiberSection::setTheTransReinfLayerThickness(RParameter *value)
{
    theTransReinfLayerThickness = value;
}

RParameter *RLayeredShellFiberSection::getTheShellThickness() const
{
    return theShellThickness;
}


void RLayeredShellFiberSection::setTheShellThickness(RParameter *value)
{
    theShellThickness = value;
}


RParameter *RLayeredShellFiberSection::getTheCoverThickness() const
{
    return theCoverThickness;
}

void RLayeredShellFiberSection::setTheCoverThickness(RParameter *value)
{
    theCoverThickness = value;
}


int RLayeredShellFiberSection::getNumCoreLayers() const
{
    return numCoreLayers;
}


void RLayeredShellFiberSection::setNumCoreLayers(int value)
{
    numCoreLayers = value;
}

int RLayeredShellFiberSection::getNumCoverLayers() const
{
    return numCoverLayers;
}


void RLayeredShellFiberSection::setNumCoverLayers(int value)
{
    numCoverLayers = value;
}


void RLayeredShellFiberSection::setTheLongLayerReinfMaterial(RNDMaterial *value)
{
    theLongLayerReinfMaterial = value;
}


RNDMaterial *RLayeredShellFiberSection::getTheLongLayerReinfMaterial() const
{
    return theLongLayerReinfMaterial;
}

RNDMaterial *RLayeredShellFiberSection::getTheTransLayerReinfMaterial() const
{
    return theTransLayerReinfMaterial;
}


void RLayeredShellFiberSection::setTheTransLayerReinfMaterial(RNDMaterial *value)
{
    theTransLayerReinfMaterial = value;
}


RNDMaterial *RLayeredShellFiberSection::getTheCoreLayerConcreteMaterial() const
{
    return theCoreLayerConcreteMaterial;
}

void RLayeredShellFiberSection::setTheCoreLayerConcreteMaterial(RNDMaterial *value)
{
    theCoreLayerConcreteMaterial = value;
}


RNDMaterial *RLayeredShellFiberSection::getTheCoverLayerConcreteMaterial() const
{
    return theCoverLayerConcreteMaterial;
}


void RLayeredShellFiberSection::setTheCoverLayerConcreteMaterial(RNDMaterial *value)
{
    theCoverLayerConcreteMaterial = value;
}




