#ifndef RLayeredShellFiberSection_H
#define RLayeredShellFiberSection_H
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

#include "RSectionForceDeformation.h"

class RNDMaterial;
class LayeredShellFiberSection;

class RLayeredShellFiberSection : public RSectionForceDeformation
{
    Q_OBJECT

public:
    
    RLayeredShellFiberSection(QObject *parent, QString name);
    ~RLayeredShellFiberSection();
    
    SectionForceDeformation* getTheSectionForceDeformation(void);
    int update(void);

    RNDMaterial *getTheCoverLayerConcreteMaterial() const;
    void setTheCoverLayerConcreteMaterial(RNDMaterial *value);

    RNDMaterial *getTheCoreLayerConcreteMaterial() const;
    void setTheCoreLayerConcreteMaterial(RNDMaterial *value);

    RNDMaterial *getTheTransLayerReinfMaterial() const;
    void setTheTransLayerReinfMaterial(RNDMaterial *value);

    RNDMaterial *getTheLongLayerReinfMaterial() const;
    void setTheLongLayerReinfMaterial(RNDMaterial *value);

    int getNumCoverLayers() const;
    void setNumCoverLayers(int value);

    int getNumCoreLayers() const;
    void setNumCoreLayers(int value);

    RParameter *getTheCoverThickness() const;
    void setTheCoverThickness(RParameter *value);

    RParameter *getTheShellThickness() const;
    void setTheShellThickness(RParameter *value);

    RParameter *getTheTransReinfLayerThickness() const;
    void setTheTransReinfLayerThickness(RParameter *value);

    RParameter *getTheLongReinfLayerThickness() const;
    void setTheLongReinfLayerThickness(RParameter *value);

    int generateOpenSeesInput(std::string& object, int flag);

protected:

    int createLayeredShellFiberSection(void);
    int intialize(void);

    int numCoverLayers;
    int numCoreLayers;

    RParameter* theCoverThickness;
    RParameter* theShellThickness;
    RParameter* theTransReinfLayerThickness;
    RParameter* theLongReinfLayerThickness;

    RNDMaterial* theCoverLayerConcreteMaterial;
    RNDMaterial* theCoreLayerConcreteMaterial;
    RNDMaterial* theTransLayerReinfMaterial;
    RNDMaterial* theLongLayerReinfMaterial;

    std::unique_ptr<LayeredShellFiberSection> theSectionForceDeformation;

};

#endif
