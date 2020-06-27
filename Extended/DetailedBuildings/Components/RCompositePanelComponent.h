#ifndef RCompositePanelComponent_H
#define RCompositePanelComponent_H
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

// A composite floor/roof component with steel decking and lightweight concrete topping

#include "RPlateComponent.h"
#include "RContinuousRandomVariable.h"

class RParameter;
class RCompositeDeckingMaterialKnowledge;
class RLinearElasticSlabMesh;
class RMesh;

class RCompositePanelComponent : public RPlateComponent
{
    Q_OBJECT

public:
    RCompositePanelComponent(QObject *parent, QString &name);
    ~RCompositePanelComponent() override;

    // Create the finite element mesh
    int createMesh() override;

    int commitState() override;

    // The volume of steel in m^3
    double getVolumeOfSteel() override;

    // The volume of concrete in m^3
    double getVolumeOfConcrete() override;

    // Depth is NOT the same as thickness, it is the depth of the corrugated channels in m
    double getDeckingDepth() const;
    void setDeckingDepth(double value);

    // Lightweight concrete topping thickness in m
    // This is not the same as thickness of the entire assembly, this is the average thickness of the concrete
    // Since the decking is corrugated, assume a thickness of 0.4 of the pan depth
    double getAverageConcreteToppingThickness(void);

    // The maximum thickness of the concrete topping, i.e., from trough of deck to top surface, in m
    double getConcreteToppingThickness() const;

    // Decking guage thickness
    int getGuage() const;
    void setGuage(int value);

    // Get the finite element mesh
    RMesh* getMesh(void) override;

    // Material knowledge containing material properties
    RCompositeDeckingMaterialKnowledge* getTheCompositeMaterial(void) const;

private:

    // The gauge of the steel deck
    int guage;

    // The depth of the decking in m
    double deckingDepth;

    // The thickness of the concrete topping in m
    double concreteToppingThickness;

    // The finite element mesh
    unique_ptr<RMesh> theMesh;

    // Material knowledge containing material properties
    unique_ptr<RCompositeDeckingMaterialKnowledge> theCompositeMaterial;
};

#endif
