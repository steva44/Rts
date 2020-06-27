#ifndef RWSteelBeamColumnComponent_H
#define RWSteelBeamColumnComponent_H
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

//   Point1                           Point2
//           1       2       3      ..n
//       .-------.-------.-------.---->
//

#include "RColumnComponent.h"
#include "RSectionStructs.h"

class RNode;
class RSteelMaterialKnowledge;
class RWLinearElasticSectionKnowledge;
class RWFibreSectionKnowledge;
class RNonlinearBeamColumnMesh;
class RSectionKnowledge;
class RContinuousRandomVariable;
class RSteelSectionImporter;

class RWSteelBeamColumnComponent : public RColumnComponent
{
    Q_OBJECT
    Q_PROPERTY(QObject *WebHeight READ getWebHeight WRITE setWebHeight)
    Q_PROPERTY(QObject *FlangeWidth READ getFlangeWidth WRITE setFlangeWidth)
    Q_PROPERTY(QObject *WebThickness READ getWebThickness WRITE setWebThickness)
    Q_PROPERTY(QObject *FlangeThickness READ getFlangeThickness WRITE setFlangeThickness)
    Q_PROPERTY(QString  WSection READ getWSectionType WRITE setWSectionType)

public:
    RWSteelBeamColumnComponent(QObject *parent, QString &name);
    ~RWSteelBeamColumnComponent() override;

    // Create the finite element mesh
    int createMesh() override;

    // The web height in m
    inline RParameter *getWebHeight(){return theWebHeightParameter;}
    void setWebHeight(QObject *value);

    // The flange width in m
    inline RParameter *getFlangeWidth(){return theFlangeWidthParameter;}
    void setFlangeWidth(QObject *value);

    // The web thickness in m
    inline RParameter *getWebThickness(){return theWebThicknessParameter;}
    void setWebThickness(QObject *value);

    // The flange thickness in m
    inline RParameter *getFlangeThickness(){return theFlangeThicknessParameter;}
    void setFlangeThickness(QObject *value);

    // Get the construction cost of component (Gurvinder)
    double getConstructionCost() override;

    // Get the number of worker hours for construction (Gurvinder)
    double getNumberOfWorkerHoursForConstruction() override;

    // Volume of steel in m3
    double getVolumeOfSteel() override;

    // Cross-section area in m^2
    double getSectionArea(void);

    // Set the W section
    void setTheWSection(const WSection &value);

    // Get and set the section type
    void setWSectionType(QString value);
    QString getWSectionType(void);

    // Material knowledge containing material properties
    RSteelMaterialKnowledge* getTheSteelMaterial(void) const;

private:

    // Component
    QPointer<RParameter> theWebHeightParameter;
    QPointer<RParameter> theFlangeWidthParameter;
    QPointer<RParameter> theWebThicknessParameter;
    QPointer<RParameter> theFlangeThicknessParameter;

    // The cross-section
    WSection theWSection;

    // Construction cost model variables
    RContinuousRandomVariable *theTheta1;
    RContinuousRandomVariable *theTheta2;
    RContinuousRandomVariable *theEpsilon;

    // Material knowledge containing material properties
    unique_ptr<RSteelMaterialKnowledge> theSteelMaterial;

    // This class is used to import standard steel cross-sections and accompanying information from the AISC database
    unique_ptr<RSteelSectionImporter> theSectionImporter;

    // Inelastic analysis discretization
    int numElements;
    int numSections;
    int numFibresInFlange;
    int numFibresInWeb;
};

#endif
