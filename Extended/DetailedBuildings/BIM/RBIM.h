#ifndef RBIM_H
#define RBIM_H
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
*   - Mojtaba Mahsuli                                                *
*                                                                    *
*********************************************************************/

#include "RObject.h"

#ifdef IFC
class IfcPlusPlusSystem;
class IfcPPModel;
#endif
class RBIMImportMethod;
class RCSICostMethod;
class RParameter;
class RResponse;
class RLocation;
class RComponent;
class RTime;
class RMainWindow;
class RNode;
class ROSGPlotter;
class RStatistics;
class ProductShapeData;

class RBIM : public RObject
{
    Q_OBJECT
    Q_PROPERTY(QString IfcFile READ getIfcFile WRITE setIfcFile)
    Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation)
    Q_PROPERTY(QObject *TimeOfConstruction READ getTimeOfConstruction WRITE setTimeOfConstruction)
    Q_PROPERTY(QObject *TimeOfDemolition READ getTimeOfDemolition WRITE setTimeOfDemolition)
    Q_PROPERTY(RLoadBearingSystem LoadBearingSystem READ getLoadBearingSystem WRITE setLoadBearingSystem)
    Q_PROPERTY(RConstructionMaterial ConstructionMaterial READ getConstructionMaterial WRITE setConstructionMaterial)
    Q_PROPERTY(int NumberOfStoreys READ getNumberOfStoreys WRITE setNumberOfStoreys)
    Q_PROPERTY(double BuildingHeight READ getBuildingHeight WRITE setBuildingHeight)
    Q_PROPERTY(QString FootprintArea READ getFootprintArea WRITE setFootprintArea)
    Q_PROPERTY(QString BuildingOccupancyClass READ getBuildingOccupancyClass WRITE setBuildingOccupancyClass)
    Q_PROPERTY(ROccupancyType OccupancyType READ getOccupancyType WRITE setOccupancyType)
    Q_PROPERTY(RPlanShape PlanShape READ getPlanShape WRITE setPlanShape)
    Q_PROPERTY(bool PlanIrregularity READ getPlanIrregularity WRITE setPlanIrregularity)
    Q_PROPERTY(bool VerticalIrregularity READ getVerticalIrregularity WRITE setVerticalIrregularity)
    Q_PROPERTY(bool SoftStorey READ getSoftStorey WRITE setSoftStorey)
    Q_PROPERTY(bool Opening READ getOpening WRITE setOpening)
    Q_PROPERTY(bool ShortColumn READ getShortColumn WRITE setShortColumn)
    Q_PROPERTY(bool Pounding READ getPounding WRITE setPounding)
    Q_PROPERTY(bool Retrofitted READ getRetrofitted WRITE setRetrofitted)
    Q_PROPERTY(bool Basement READ getBasement WRITE setBasement)
    Q_PROPERTY(bool Balcony READ getBalcony WRITE setBalcony)
    Q_PROPERTY(int  NumberOfHouseHolds READ getNumberOfHousingUnits WRITE setNumberOfHousingUnits)
    Q_PROPERTY(QString BuildingType READ getBuildingType WRITE setBuildingType)

    Q_ENUMS(RLoadBearingSystem)
    Q_ENUMS(RConstructionMaterial)
    Q_ENUMS(ROccupancyType)
    Q_ENUMS(RPlanShape)
    Q_ENUMS(RCodeLevel)
    Q_ENUMS(RPrototype)

public:
    RBIM(QObject *parent, QString name);
    ~RBIM();

    enum RLoadBearingSystem{ShearWall, MomentFrame, BracedFrame, LightFrame, MasonryWall};
    enum RConstructionMaterial{ReinforcedConcrete, PrecastConcrete, Steel, Wood, ReinforcedMasonry, UnreinforcedMasonry, MobileHome};
    enum ROccupancyType{SingleResidence, MultiResidence, InstitutionalResidence, Trade, Services, Medical, Recreation, Parking, School, University, Government, Industry, Agriculture};
    enum RPlanShape{Rectangular, LShape, TShape, EShape, CShape, ZShape};
    enum RCodeLevel{PreCode, LowCode, ModerateCode, HighCode};
    enum RPrototype{ConcreteShearWall, ConcreteMomentFrame, ConcreteFrameWithMasonryInfillWall, PrecastFrameWithShearWall, SteelFrameWithConcreteShearWall, SteelMomentFrame, SteelBracedFrame, SteelLightFrame, SteelFrameWithMasonryInfillWall, WoodLargeFrame, WoodLightFrame, ReinforcedMasonryBearingWall, UnreinforcedMasonryBearingWall, Mobile};

    // The IFC file corresponding to this BIM
    QString getIfcFile() const;
    void setIfcFile(QString value);

    // The location of the building
    QObject *getLocation() const;
    void setLocation(QObject *value);

    // Time of construction (RTime)
    QObject *getTimeOfConstruction() const;
    void setTimeOfConstruction(QObject *value);
    double getTimeOfConstructionValue();

    // Time of demolition (RTime)
    QObject *getTimeOfDemolition() const;
    void setTimeOfDemolition(QObject *value);
    double getTimeOfDemolitionValue();

    // Type of structural system
    RLoadBearingSystem getLoadBearingSystem() const;
    void setLoadBearingSystem(RLoadBearingSystem value);

    // Predominant strucutural construction material
    RConstructionMaterial getConstructionMaterial() const;
    void setConstructionMaterial(RConstructionMaterial value);

    // HAZUS building type
    // Refer to table 3.1 in Hazus2.1 for the various building types
    QString getBuildingType();
    void setBuildingType(QString value);

    // The number of building storeys
    int getNumberOfStoreys() const;
    void setNumberOfStoreys(int value);

    // The building footprint area in m2
    QString getFootprintArea();
    void setFootprintArea(QString value);

    // The HAZUS building occupancy class
    QString getBuildingOccupancyClass() const;
    void setBuildingOccupancyClass(QString value);

    // The occupancy type, residential, commercial, medical, etc.
    ROccupancyType getOccupancyType() const;
    void setOccupancyType(ROccupancyType value);

    // The shape of the building, Rectangular, LShape, TShape, etc.
    RPlanShape getPlanShape() const;
    void setPlanShape(RPlanShape value);

    // True if building has a plan irregularity
    bool getPlanIrregularity() const;
    void setPlanIrregularity(bool value);

    // True if building has a vertical irregularity
    bool getVerticalIrregularity() const;
    void setVerticalIrregularity(bool value);

    // True if building has a soft storey
    bool getSoftStorey() const;
    void setSoftStorey(bool value);

    // True if building has a large opening
    bool getOpening() const;
    void setOpening(bool value);

    // True if building has short columns
    bool getShortColumn() const;
    void setShortColumn(bool value);

    // True if there is possibility of building pounding
    bool getPounding() const;
    void setPounding(bool value);

    // True if building has been retrofitted
    bool getRetrofitted() const;
    void setRetrofitted(bool value);

    // True if building has a basement
    bool getBasement() const;
    void setBasement(bool value);

    // True if building has a balcony
    bool getBalcony() const;
    void setBalcony(bool value);

    // List of components within the BIM
    QList<RComponent *> getComponentList();

    // Building costs at various phases of its life
    double getManufacturingCost();
    double getConstructionCost();
    double getRepairCost();
    double getPreventativeRepairCost();

    // Volume of concrete in m3
    double getVolumeOfConcrete();

    // Volume of steel in m3
    double getVolumeOfSteel();

    // Volume of wood in m3
    double getVolumeOfWood();

    // Number of people in the building
    double getOccupancyNumber();

    // Type of construction, midrise, highrise, etc.
    QString getConstructionType();

    //HAZUS damage state
    QString getBuildingDamageState();

    // Total floor area in m2
    double getTotalFloorlArea() const;

    // Residential floor area in m2
    double getResidentialFloorArea() const;

    // Commercial floor area in m2
    double getCommercialFloorArea() const;

    // Maximum interstorey drift within the building
    double getMaxInterstoreyDrift(void);

    // Returns the building drift at the roof
    double getBuildingDrift(void);

    // Building volume in m3
    double getTheBuildingVolume() const;
    void setTheBuildingVolume(double value);

    // Height of the ceilings within each storey in m
    double getBuildingStoreyHeight(int storey);

    // Elevation of the floor, i.e., how high above the ground in m
    double getBuildingStoreyElevation(int storey);

    // Method to calculate the cost of CSI codes with RSMeans
    RCSICostMethod* getTheCSICostMethod();

    // Fundamental period of the building
    double getBuildingPeriod(void);

    // Number of residential housing units
    int getNumberOfHousingUnits() const;
    void setNumberOfHousingUnits(int value);

    // Building height in m
    double getBuildingHeight();
    void setBuildingHeight(double value);

    // Occupancy type, i.e., single, multi, commercial, etc.
    QString getOccupancyClass() const;

    // Code design level, i.e., high-code, low-code, etc.
    RCodeLevel getCodeLevel();

    // Building prototype, ConcreteShearWall, ConcreteMomentFrame, ConcreteFrameWithMasonryInfillWall, etc.
    RPrototype getPrototype();

    // Trigger times for preventative maintenance
    QList<QPair<double, QString> > getTriggerTimes(double startTime, double endTime);

#ifdef IFC
    // Set the Ifc++ system to the BIM
    void setIfcPPSystem(QPointer<IfcPlusPlusSystem> IfcPPSystem);

    // Creates the Rts components from a BIM
    void createComponentsFromBIM();

    // Loads the BIM and creates the finite element mesh of the structure
    void createMesh();

    // Creates the surface or visualization mesh of the building components
    void createTopoLogicalMesh(void);

    // Data members that describe the building
    double theBuildingHeight;
    QVector<double> buildingStoreyElevation;
    QVector<double> buildingStoreyHeight;
    int theNumberOfStoreys;
    double theTotalFloorlArea;
    double theBuildingVolume;
    double residentialFloorArea;
    double commercialFloorArea;
    double footprintArea;
    int numberOfHousingUnits;
    bool isMeshed;
    bool isTopoMeshed;
#endif

private:

    void computeCodeLevel(bool retrofitted);
    void computePrototype();
    QString theBuildingType;
    QString theOccupancyClass;
    QString theIfcFile;
    QPointer<RParameter> theLocation;
    QPointer<RTime> theTimeOfConstruction;
    QPointer<RTime> theTimeOfDemolition;
    RLoadBearingSystem theLoadBearingSystem;
    RConstructionMaterial theConstructionMaterial;
    double theFootPrintArea;
    ROccupancyType theOccupancyType;
    RPlanShape thePlanShape;
    bool thePlanIrregularity;
    bool theVerticalIrregularity;
    bool theSoftStorey;
    bool theOpening;
    bool theShortColumn;
    bool thePounding;
    bool theRetrofitted;
    bool theBasement;
    bool theBalcony;
    RCodeLevel theCodeLevel;
    RPrototype thePrototype;

    // Method to calculate the cost of CSI codes
    std::unique_ptr<RCSICostMethod> theCSICostMethod;

#ifdef IFC
    // Initializes the BIM import classes and extracts the building location
    void initializeRBIM();
    std::unique_ptr<RBIMImportMethod> theBIMImportMethod;
    std::shared_ptr<IfcPPModel>  theIFCppModel;
    QPointer<IfcPlusPlusSystem> theIFCSystem;
    std::shared_ptr<RLocation>   theBIMLocation;
    ROSGPlotter* thePlotter;
    QString IFCFilePath;

    RMainWindow* theMainWindow;
    bool ifcFileLoaded;
#endif

};

#endif
