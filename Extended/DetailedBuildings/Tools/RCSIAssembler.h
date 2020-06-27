#ifndef RCSIAssembler_H
#define RCSIAssembler_H
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

#include "RMethod.h"

class RContinuousRandomVariable;
class RRectangularRCColumnComponent;
class RRCShearWallComponent;
class RRCSlabComponent;
class RWSteelBeamColumnComponent;
class RRectangularGlulamBeamColumnComponent;
class RCLTPanelComponent;
class RCompositePanelComponent;
class RBIM;
class RJob;

using CSIVector = QVector<std::tuple<QString,double,QString>>;

class RCSIAssembler : public RMethod
{
    Q_OBJECT

public:
    RCSIAssembler(QObject *parent, QString name);
    ~RCSIAssembler();

    //****COLUMN*****//
    // CSI codes for rectangular reinforced concrete column construction labour
    int getRectRCColLabourConstructionCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                            RJob* jobPhase,
                                            CSIVector* labourQuantityCSICodes);

    // CSI codes for rectangular reinforced concrete column formwork installation
    int getRectRCColFormingCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                 RJob* jobPhase,
                                 CSIVector* CSICodes,
                                 double contactArea = 0.0);

    // CSI codes for rectangular reinforced concrete column steel reinforcement
    int getRectRCColReinforcingCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                     RJob* jobPhase,
                                     CSIVector* labourQuantityCSICodes);

    // CSI codes for rectangular reinforced concrete column concrete pouring, placing, etc.
    int getRectRCColPlacingLabourCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                       RJob* jobPhase,
                                       CSIVector* labourQuantityCSICodes);

    // CSI codes for rectangular reinforced concrete column construction material
    int getRectRCColMaterialConstructionCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                              RJob* jobPhase,
                                              CSIVector* materialQuantityCSICodes);

    // CSI codes for rectangular reinforced concrete column replacement
    int getRectRCColReplacementCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                     RJob* jobPhase,
                                     CSIVector* materialQuantityCSICodes,
                                     CSIVector* labourQuantityCSICodes);

    // CSI codes for rectangular reinforced concrete column cover replacement
    // Volume in yards, area in sq. ft.
    int getRectRCColCoverReplacementCodes(double volumeRepair,
                                          double areaRepair,
                                          RRectangularRCColumnComponent* theRectConcreteColumn,
                                          RJob* jobPhase,
                                          CSIVector* materialQuantityCSICodes,
                                          CSIVector* labourQuantityCSICodes);

    //****SLAB*****//
    // CSI codes for reinforced concrete slab construction labour
    int getRCSlabLabourConstructionCodes(RRCSlabComponent* theConcreteSlab,
                                         RJob* jobPhase,
                                         RBIM* theBIM,
                                         CSIVector* labourQuantityCSICodes);

    // CSI codes for reinforced concrete slab formwork installation
    int getRCSlabFormingCodes(RRCSlabComponent* theConcreteSlab,
                              RJob* jobPhase,
                              RBIM* theBIM,
                              CSIVector* CSICodes);

    // CSI codes for reinforced concrete slab reinforcement installation
    int getRCSlabReinforcingCodes(RRCSlabComponent* theConcreteSlab,
                                  RJob* jobPhase,
                                  CSIVector* CSICodes);

    // CSI codes for reinforced concrete slab concrete pouring, placing, etc.
    int getRCSlabPlacingLabourCodes(RRCSlabComponent* theConcreteSlab,
                                    RJob* jobPhase,
                                    RBIM* theBIM,
                                    CSIVector* labourQuantityCSICodes);

    // CSI codes for reinforced concrete slab construction material
    int getRCSlabMaterialConstructionCodes(RRCSlabComponent* theConcreteSlab,
                                           RJob* jobPhase,
                                           RBIM* theBIM,
                                           CSIVector* materialQuantityCSICodes);

    // CSI codes for reinforced concrete slab spalling repair
    // Area in sq. ft., volume in cubic feet
    int getSlabSpallPatchCodes(double areaOfPatch,
                               double volumeOfPatch,
                               RComponent* theComponent,
                               RJob* repairPhase,
                               CSIVector* materialQuantityCSICodes,
                               CSIVector* labourQuantityCSICodes);

    // CSI codes for reinforced concrete slab cover repair
    // Area in sq. ft., volume in Yards
    int getSlabCoverRepairCodes(double areaOfPatch,
                                double volumeOfPatch,
                                RComponent* theComponent,
                                RJob* repairPhase,
                                CSIVector* materialQuantityCSICodes,
                                CSIVector* labourQuantityCSICodes);

    // CSI codes for reinforced concrete slab replacement
    int getRCSlabReplacementCodes(RRCSlabComponent* theConcreteSlab,
                                  RBIM* theBIM,
                                  RJob* jobPhase,
                                  CSIVector* materialQuantityCSICodes,
                                  CSIVector* labourQuantityCSICodes);

    // CSI codes for reinforced concrete slab cover replacement
    // For the top or "floor" of the slab
    // Voume in cubic feet, depth of the repair in inches, and the repair area in sq. ft.
    int getRCSlabTopCoverReplacementCodes(double volumeRepair,
                                          double depthRepair,
                                          double areaRepair,
                                          RJob* jobPhase,
                                          CSIVector* labourQuantityCSICodes,
                                          CSIVector* materialQuantityCSICodes,
                                          RRCSlabComponent* theConcreteSlab);

    // CSI codes for reinforced concrete slab cover replacement
    // For the bottom or "ceiling" of the slab
    // Voume in cubic feet, depth of the repair in inches, and the repair area in sq. ft.
    int getRCSlabBottomCoverReplacementCodes(double volumeRepair,
                                             double depthRepair,
                                             double areaRepair,
                                             RJob* jobPhase,
                                             CSIVector* labourQuantityCSICodes,
                                             CSIVector* materialQuantityCSICodes,
                                             RRCSlabComponent* theConcreteSlab);

    //****SHEAR WALL*****//
    // CSI codes for reinforced concrete shear wall construction labour
    int getRCShearWallLabourConstructionCodes(RRCShearWallComponent* theConcreteShearwall,
                                              RJob* jobPhase,
                                              RBIM* theBIM,
                                              CSIVector* labourQuantityCSICodes);

    // CSI codes for reinforced concrete shear wall formwork installation
    int getRCShearWallFormingCodes(RRCShearWallComponent* theConcreteShearwall,
                                   RJob* jobPhase,
                                   RBIM* theBIM,
                                   CSIVector* CSICodes);

    // CSI codes for reinforced concrete shear wall reinforcement installation
    int getRCShearWallReinforcingCodes(RRCShearWallComponent* theConcreteShearwall,
                                       RJob* jobPhase,
                                       CSIVector* CSICodes);

    // CSI codes for reinforced concrete shear wall concrete placing
    int getRCShearWallPlacingCodes(RRCShearWallComponent* theConcreteShearwall,
                                   RJob* jobPhase,
                                   RBIM* theBIM,
                                   CSIVector* CSICodes);

    // CSI codes for reinforced concrete shear wall construction material
    int getRCShearWallMaterialConstructionCodes(RRCShearWallComponent* theConcreteShearwall,
                                                RJob* jobPhase,
                                                RBIM* theBIM,
                                                CSIVector* materialQuantityCSICodes);

    // CSI codes for reinforced concrete shear wall replacement
    int getRCShearWallReplacementCodes(RRCShearWallComponent* theConcreteShearwall,
                                       RBIM* theBIM,
                                       RJob* jobPhase,
                                       CSIVector* materialQuantityCSICodes,
                                       CSIVector* labourQuantityCSICodes);

    // CSI codes for reinforced concrete shear wall cover replacement
    // Volume in yards, area in sq. ft., depth of the repair in inches
    int getRCWallCoverReplacementCodes(double volumeRepair,
                                       double depthRepair,
                                       double areaRepair,
                                       RJob* jobPhase,
                                       CSIVector* labourQuantityCSICodes,
                                       CSIVector* materialQuantityCSICodes,
                                       RRCShearWallComponent* theConcreteShearwall);

    // CSI codes for reinforced concrete shear wall spalling repair - smaller repairs
    // Area in sq. ft., volume in cubic feet
    int getWallPatchCoverCodes(double areaOfPatch,
                               double volumeOfPatch,
                               RComponent* theComponent,
                               RJob* repairPhase,
                               CSIVector* materialQuantityCSICodes,
                               CSIVector* labourQuantityCSICodes);

    // CSI codes for reinforced concrete shear wall spalling repair - larger repair using forms
    // Area in sq. ft., volume in cubic feet, wall height in feet
    int getWallCoverRepairFormsCodes(double areaOfPatch,
                                     double volumeOfPatch,
                                     double theWallHeight,
                                     RComponent* theShearwall,
                                     RJob* repairPhase,
                                     CSIVector* materialQuantityCSICodes,
                                     CSIVector* labourQuantityCSICodes);

    //******OTHER COMPONENTS******//
    // CSI codes for steel wideflange column construction labour
    int getSteelWColumnLabourConstructionCodes(RWSteelBeamColumnComponent* theWBeamCol,
                                               RJob* jobPhase,
                                               CSIVector* labourQuantityCSICodes);

    // CSI codes for steel wideflange column construction material
    int getSteelWColumnMaterialConstructionCodes(RWSteelBeamColumnComponent* theWBeamCol,
                                                 CSIVector* materialQuantityCSICodes);

    // CSI codes for rectangular glulam column construction labour
    int getGlulamColumnLabourConstructionCodes(RRectangularGlulamBeamColumnComponent* theGlulamCol,
                                               RJob* jobPhase,
                                               CSIVector* labourQuantityCSICodes);

    // CSI codes for rectangular glulam column construction material
    int getGlulamColumnMaterialConstructionCodes(RRectangularGlulamBeamColumnComponent* theGlulamCol,
                                                 CSIVector* materialQuantityCSICodes);

    // CSI codes for CLT panel construction
    int getCLTPanelConstructionCodes(RCLTPanelComponent* theCLTPanel,
                                     RJob* jobPhase,
                                     CSIVector* CSICodes);

    // CSI codes for composite steel pan and lightweight concrete floor construction
    int getSteelSkinSlabConstructionCodes(RCompositePanelComponent* theSteelPanel,
                                          RJob* jobPhase,
                                          CSIVector* labourQuantityCSICodes);

    //****DEMOLITION*****//
    // Two types of "refined" demolition methods - they depend on the concern for surrounding or adjacent elements:
    // 1) Selective Demolition (valid for all concrete components):
    //      Demolition of a selected building element,component, or finish, with some concern for surrounding or adjacent elements,
    //      components, or finishes (see the first Subdivision (s) at thebeginning of appropriate Divisions).
    //      This type of demolition is accomplishedby hand or pneumatic hand tools, and does not include handling, loading,storing, hauling, or disposal of the debris,
    //      scaffolding, or shoring.′′Gutting′′methods may be used in order to save time, but damage that is caused to surrounding or adjacent elements, components, or
    // 2) Cutout Demolition (special cases for slab and wall):
    //      Demolition of a small quantity of floor, wall, roof, orother assembly, with concern for the appearance and structural integrity of the surrounding materials.
    //      This type of demolition is accomplished byhand or pneumatic hand tools, and does not include saw cutting, handling,loading, hauling, or disposal of debris, scaffolding, or shoring.
    //      finishes may have to be repaired at a later time


    // 1) Selective Demolition (valid for all concrete components):
    // CSI codes for the selective demolition of concrete components
    // Volume in yards
    int getSelectiveConcreteDemolitionCodes(double reinfRatio,
                                            double volumeDemoConcrete,
                                            RJob* repairPhase,
                                            CSIVector* labourQuantityCSICodes,
                                            RComponent* parentComponent);

    // CSI codes for the selective demolition of steel components
    // Mass steel in tons
    int getSelectiveSteelDemolitionCodes(double massSteel,
                                         RJob* repairPhase,
                                         CSIVector* labourQuantityCSICodes,
                                         RComponent* theComponent);

    // CSI codes for the selective demolition of steel decking
    // Area of decking in sq. ft.
    int getSteelDeckingDemolitionCodes(double areaDecking,
                                       RJob* repairPhase,
                                       CSIVector* labourQuantityCSICodes,
                                       RComponent* theComponent);

    // 2) Cutout Demolition (special cases for slab and wall):
    // CSI codes for concrete slab "cutout" demolition
    // Volume in cubic feet
    int getConcreteSlabCutOutDemolitionCodes(double reinfRatio,
                                             double volumeDemoConcrete,
                                             RJob* repairPhase,
                                             CSIVector* labourQuantityCSICodes,
                                             RComponent* parentComponent);

    // CSI codes for concrete slab "cutout" demolition with wire mesh reinforcement
    // Volume in cubic feet
    int getConcreteSlabCutOutWireMeshDemolitionCodes(double volumeDemoConcrete,
                                                     RJob* repairPhase,
                                                     CSIVector* labourQuantityCSICodes,
                                                     RComponent* parentComponent);

    // CSI codes for concrete wall "cutout" demolition
    // Volume in cubic feet, wall length in feet
    int getConcreteWallCutOutDemolitionCodes(double volumeDemoConcrete,
                                             double wallLength,
                                             RJob* jobPhase,
                                             CSIVector* labourQuantityCSICodes,
                                             RComponent* parentComponent);

    //****REMOVAL AND DISPOSAL OF DEMOLITION MATERIAL*****//

    // CSI codes for concrete removal and disposal labour, equipment, etc.
    // Volume in cubic yards
    int getConcreteRemovalDisposalCodes(double volumeOfDemoConcrete,
                                        RJob* jobPhase,
                                        CSIVector* labourQuantityCSICodes,
                                        RComponent* theComponent);

    // CSI codes for wood removal and disposal labour, equipment, etc.
    // Volume in cubic yards
    int getWoodRemovalDisposalCodes(double volumeOfDemoWood,
                                    RJob* jobPhase,
                                    CSIVector* labourQuantityCSICodes,
                                    RComponent* theComponent);

    // CSI codes for steel removal and disposal labour, equipment, etc.
    // Volume in cubic yards
    int getSteelRemovalDisposalCodes(double volumeOfDemoSteel,
                                     RJob* jobPhase,
                                     CSIVector* labourQuantityCSICodes,
                                     RComponent* theComponent);

    //****CRACK REPAIR*****//
    // CSI codes for epoxy crack repair
    // Volume in cubic foot
    int getEpoxyCrackRepairCodes(double volumeOfCrack,
                                 CSIVector* materialQuantityCSICodes,
                                 RJob* jobPhase,
                                 CSIVector* labourQuantityCSICodes,
                                 RComponent* theComponent);

    //****MISC CONSTRUCTION ACTIVITIES*****//
    // CSI codes for shoring and scaffolding of slabs when repairinga and replacing other components
    int getShoringCSICodes(const double& numberOfFrames,
                           CSIVector* materialQuantityCSICodes);

    // CSI codes for concrete shotcrete repairs, e.g., on the underside of slab, or large wall surface area
    // Area in sq. ft., depth in inch
    int getConcreteRepairShotcreteCodes(double areaDamage,
                                        double depthDamage,
                                        RComponent* theComponent,
                                        RJob* repairPhase,
                                        CSIVector* labourQuantityCSICodes);

    // CSI codes for reinforcement dowel installation
    int getReinforcementDowelCodes(double numberDowels,
                                   RComponent* theComponent,
                                   RJob* repairPhase,
                                   CSIVector* labourQuantityCSICodes);

    // Get additional costs/savings for economy of scale
    int getAdditionalLabourCosts(RJob* jobPhase,
                                 RBIM* theBIM,
                                 CSIVector* labourQuantityCSICodes);

    // CSI codes for different types of concrete material, e.g., C15, C20, C30, etc.
    void getConcreteCSICode(QString theConcreteType,
                            double yardsOfConcrete,
                            QVector<std::tuple<QString,double,QString>>* materialQuantityCSICodes);

    // CSI codes for installation of steel components
    void getSteelFramingCSICodes(QString theSteelShape,
                                 double linearFeet,
                                 RComponent* theComponent,
                                 RJob* phase,
                                 CSIVector* CSICodes);

    // CSI codes for concrete curing activities
    void getConcreteCuringCSICodes(const double& formContactArea,
                                   RComponent* theComponent,
                                   RJob* phase,
                                   CSIVector* labourQuantityCSICodes);

private:

    // Unit conversion factors
    double convertMeterToInch;
    double convertMeterToFoot;
    double convertMeterSqToFtSq;
    double convertCubicMToCubicYd;
    double convertCubicMToCubicFt;
    double convertBfToCubicFt;
    double convertCubicFttoYrd;
    double convertInchtoFt;
    double convertYrdToCubicFt;

    // Volume of wooden concrete forms in yards
    // Thickness in inch, area in sq. ft.
    double getVolYrdWoodForms(const double& thickness,const double& area);

};

#endif
