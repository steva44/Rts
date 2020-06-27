#ifndef RComponent_H
#define RComponent_H
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

// The base class for all building components

#include "RNode.h"
#include "RElement.h"
#include "RObject.h"
#include "RJob.h"
#include "RPoint.h"

class RParameter;
class RDamageMesh;
class RMesh;

using CSIvec = QVector<std::tuple<QString, double, QString> >;

// A data structure that contains the connectivity information, i.e., what components are connected to this component
struct componentConnectivityStruct{

public:

    void addComponentForConnection(RComponent* component)
    {
        theConnectivityList.push_back(component);
    }

    void addConnectedComponent(RComponent* component)
    {
        if(isConnectedToComponent(component))
            return;
        else
            theConnectivityList.push_back(component);

    }

    bool isConnectedToComponent(RComponent* component)
    {
        auto it = std::find(std::begin(theConnectivityList), std::end(theConnectivityList), component);

        if ( theConnectivityList.end() == it )
        {
            return false;
        }
        else
            return true;
    }

    // Vector of all nodes for this component - connectivity only occurs at the nodes
    std::vector<RNode*> theNodeList;

    // Vector of all components currently connected to this one
    std::vector<RComponent*> theConnectivityList;
};

class RComponent : public RObject
{
    Q_OBJECT
    Q_PROPERTY(QString MeshOption READ getMeshOption WRITE setMeshOption)

public:
    RComponent(QObject *parent, QString &name);
    ~RComponent();

    // A tag to describe whether this component is an interior component, or is part of the exterior building envelope
    // For energy loss calculations
    enum RInteriorOrExteriorComponent{INTERIOR,EXTERIOR,NA};

    // Gets the parameters of this component
    QList<RParameter *> getParameterList();

    // Gets the finite elements of this component
    QList<RElement *> getElementList();
    void clearElementList();

    // Flag to ensure that a finite element mesh was created for this component
    bool meshWasCreated;

    // The storey number where this component is located
    void setStoreyNumber(int storeyNumber);

    // The elevation (m), i.e., height from the ground, of this component
    void setElevation(double componentElevation);
    double getElevation(void);

    // Set whether this component is part of the exterior building envelope
    void setInteriorExterior(RInteriorOrExteriorComponent componentLocation);

    // Returns true if this component is part of the exterior building envelope
    bool isExterior(void);

    // This component may have a void
    void addVoid(double area, double volume);

    // Methods to create finite elements & loads and get finite element responses (non-structural components MAY not have finite elements)
    virtual int createMesh();

    // May need to commit the state of the component in an analysis
    virtual int commitState();

    // Returns the points list that describes the geometry of the component
    virtual QList<RPoint *> getPointList();

    // Reset parameters in a new analysis
    virtual void reset(void);

    // Reset damage in a new analysis
    virtual void resetVisualDamage(void);

    // Method to create the topo mesh
    virtual int createTopologicalMesh();

    // Gets and sets the RSI value for energy loss calculations
    virtual double getRSIValue();
    virtual void setRSIValue(double value);

    // Sets and gets the manufacturing cost of the component
    virtual double getManufacturingCost();
    void setManufacturingCost(double value);

    // Sets and gets the construction cost of the component
    virtual double getConstructionCost();
    void setConstructionCost(double value);

    // Sets and gets the repair cost of the component
    void setRepairCost(double value);
    virtual double getRepairCost();

    // Gets and sets the replacement cost of this component - different from the repair cost
    double getReplacementCost() const;
    void setReplacementCost(double value);

    // Quantities of materials
    virtual double getVolumeOfConcrete();
    virtual double getVolumeOfSteel();
    virtual double getVolumeOfWood();

    // Finite element mesh
    virtual RMesh* getMesh();

    // Damage mesh
    virtual RDamageMesh* getDamageMesh();

    // Get trigger times for maintenance actions of component
    virtual QList<QPair<double, QString> > getTriggerTimes(double startTime, double endTime);

    // Provides the location of this component in terms of the building story it belongs in
    // Assume 0 = ground floor, -1 first basement floor, 1 first floor above ground, etc.
    virtual int getBuildingStoreyNumber();

    // The datamember that stores component connectivity information
    componentConnectivityStruct  theConnectivityStruct;

    // Gets and sets the local mesh option
    QString getMeshOption(){return QString::number(localMeshOption);}
    void setMeshOption(QString value){localMeshOption = value.toDouble();}

    // Flags to check if damage should be evaluated for this component
    bool getEvaluateDamage() const;

    // Flags to check if the component is part of the structural system, and say not a window
    bool getIsStructural() const;

    // Data members to store the repair related CSI codes for this component
    CSIvec& getRepairLabourQuantityCSICodes();
    CSIvec& getRepairMaterialQuantityCSICodes();
    RJob& getRepairJob();
    void commitRepair(CSIvec* labourCSICodes , CSIvec* materialCodes, RJob* job, double repCost);

    // Data members to store the CSI codes related to the replacement of this component
    CSIvec& getReplacementLabourQuantityCSICodes();
    void setReplacementLabourQuantityCSICodes(const CSIvec &value);
    CSIvec& getReplacementMaterialQuantityCSICodes();
    void setReplacementMaterialQuantityCSICodes(const CSIvec &value);
    RJob& getReplacementJob();
    void setReplacementJob(const RJob &value);

    // Data members to store the construction related CSI codes for this component
    CSIvec& getConstructionLabourQuantityCSICodes();
    void setConstructionLabourQuantityCSICodes(const CSIvec &value);
    RJob& getConstructionJob();
    void setConstructionJob(const RJob &value);

    // Data members to store the manufacturing related CSI codes for this component
    CSIvec& getManufacturingMaterialQuantityCSICodes();
    void setManufacturingMaterialQuantityCSICodes(const CSIvec &value);

    // Resets the CSI codes for this component
    void resetCSI(void);

    // A flag to check if this component is being replaced
    bool getShouldBeReplaced() const;
    void setComponentReplaced(bool value);

    // Older deprecated methods to compute various costs, now these costs are computed in models
    int numRepairActions;
    virtual double getNumberOfWorkerHoursForConstruction();
    virtual double getNumberOfWorkerHoursForRepair();
    virtual double getServiceabilityCost();
    virtual double getRepairTime();
    virtual double getPreventativeRepairCost();

protected:

    QList<QPair<double, QString> > repairTimes;
    QList<RElement *> theElementList;
    QList<RParameter *> theParameterList;
    void addToParameterList(RParameter* theParameter);
    void addToParameterList(QList<RParameter*> theParameter);

    // Vector to store the voids, if any, of this component
    QVector<QPair<double,double>> vectorOfVoids;

    bool applyEarthquake;
    bool applySnow;
    bool applyWind;
    bool applyLiveLoad;
    bool evaluateDamage;
    bool isStructural;
    bool shouldBeReplaced;

    // The local mesh option
    int localMeshOption;

    //Assume 0 = ground floor, -1 is the first floor in the basement, 1 is the first floor above ground, etc.
    int buildingStoreyNumber;

    // Component elevation in m
    double componentElevation;

    RInteriorOrExteriorComponent theComponentExposure;

     // The R-Value of the component- R Units = (m^2 * degree C) / Watts
    double RSIValue;

    // Various costs returned by the component
    double manufacturingCost;
    double constructionCost;
    double repairCost;
    double replacementCost;

    // CSI code data members
    // Note that equipment is part of the labour codes
    // CSI codes for replacing the component
    CSIvec replacementLabourQuantityCSICodes;
    CSIvec replacementMaterialQuantityCSICodes;
    RJob   replacementJob;

    // CSI codes for constructing the component
    CSIvec constructionLabourQuantityCSICodes;
    CSIvec constructionMaterialQuantityCSICodes;
    RJob   constructionJob;

    //CSI codes for reparing the component
    CSIvec repairLabourQuantityCSICodes;
    CSIvec repairMaterialQuantityCSICodes;
    RJob   repairJob;

    // The visual damage mesh
    unique_ptr<RDamageMesh> theDamageMesh;

private:

};

#endif
