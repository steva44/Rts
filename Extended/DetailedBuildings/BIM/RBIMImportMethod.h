#ifndef RBIMImportMethod_H
#define RBIMImportMethod_H
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

// This class contains the methods for BIM import

#include <vector>
#include <unordered_map>
#include <QPointer>

class RBIM;
class RNode;
class RPoint;
class RComponent;
class RIFCImportData;
class RIFCtoComponentConverter;
class RDomain;
class IfcObject;
class IfcProduct;
class IfcPPEntity;
class IfcMaterial;
class IfcPlusPlusSystem;
class IfcPPModel;
class IfcProperty;
class IfcPhysicalQuantity;
class ProductShapeData;

class RBIMImportMethod
{

public:
    RBIMImportMethod(RBIM* BIM, RDomain* Domain);
    ~RBIMImportMethod();

    // Converts and IFC file to components
    int convertBIMtoRBIM();

    // Determines the connectivity of the components using mesh intersection algorithms or IFC information
    void getConnectivityRelationships(void);

    // Create the finite element mesh in the components
    void meshComponents(void);

    // Modify the geometry and optimize mesh of the connected components so that they align for cleaner meshing of finite elements
    // Also to remove/combine overlapping nodes
    void modifyGeometry(void);
    void optimizeMesh();

    // Converts an IFC product into an Rts component
    void convertIfcObjecttoComponent(const std::shared_ptr<IfcProduct>& product, const std::string& theClassType);

    // Gets the material from an IFC representation
    void getMaterial(IfcMaterial* theMaterial, RIFCImportData* theRePData);

    // Sets the map of the IFC entites from Ifc++
    void setMapOfIfcEntity(const std::map<int, std::shared_ptr<IfcPPEntity> > &value);

    // Set the IFC system and model
    void setTheIFCSystem(const QPointer<IfcPlusPlusSystem> &value);
    void setTheIFCppModel(const std::shared_ptr<IfcPPModel> &value);

    // Map containing the Rts objects created during BIM import
    std::unordered_map<IfcProduct *, std::shared_ptr<RIFCImportData> > getMapofRtsIfcObjects() const;

private:

    // Adds a connection to the vector if it does not exist already
    void addConnection(const int id1,const int id2);

    // Check if two components are connected
    bool checkConnectionExists(const int id1,const int id2);

    // Squared distance between two points - faster than using a square root to get the distance
    double sqrdDistanceBtwnPts(RNode* pt1, RNode* pt2);

    // Swaps two points
    void swapPoint(RPoint* originalPoint, RPoint* replacementPoint);

    // Find a point based on the coordinates
    RPoint* findPoint(double &x, double& y, double& z);

    // Checks if two points coincide, or are very close to coinciding
    bool arePointsCoinciding(RPoint* P1, RPoint* P2);

    // Converts a point to a slave node
    void pointToSlaveNode(RNode* masterNode, RNode* slaveNode);

    // Function to find the closest node
    RNode* findClosestNode(RComponent* theComponent, RNode* theNode);

    // Ifc++ system and model
    QPointer<IfcPlusPlusSystem> theIFCSystem;
    std::shared_ptr<IfcPPModel> theIFCppModel;

    // The BIM and domain
    RBIM* theBIM;
    RDomain* theDomain;

    // Physically connects the components
    void connectComponents(void);

    // Connects a column component to a slab
    int connectColumnPlate(RComponent* column, RComponent* plate);

    // Connects a wall component to a slab
    int connectWallPlate(RComponent* wall, RComponent* plate);

    // Modifies geometry of components for cleaner connectivity
    int modifyWallPlateGeometry(RComponent* wall, RComponent* plate);
    int modifyWallColumnGeometry(RComponent* wall, RComponent* column);

    // Vector of connected components. Each item in the vector returns a pair of "ID" numbers that specify which components are connected, e.g., Component #4454 is connected to component #9503
    // The "ID" integers correspond to the IFC step file number of each component
    std::vector<std::pair<int,int>> connectionVector;

    // Map to hold the Rts objects that are created
    std::unordered_map<IfcProduct*, std::shared_ptr<RIFCImportData>> mapofRtsIfcObjects;

    // A map of the IFC entities created in Ifc++
    std::map<int, std::shared_ptr<IfcPPEntity> > mapOfIfcEntity;

    // The method that converts the IFC objects into components, i.e, IfcColumn to RRectangularRCColumn
    std::shared_ptr<RIFCtoComponentConverter> theRepresentationConverter;

    // Vector of IFC properties of an object
    std::vector<std::shared_ptr<IfcProperty> > getVectorIFCProperties(std::shared_ptr<IfcObject>& ifcObject);

    // Vector of IFC physical quantities of a building, i.e., areas, volumes, etc.
    std::vector<std::shared_ptr<IfcPhysicalQuantity> > getVectorPhysicalQuantities(std::shared_ptr<IfcObject>& ifcObject);

};

#endif // RBIMImportMethod_H
