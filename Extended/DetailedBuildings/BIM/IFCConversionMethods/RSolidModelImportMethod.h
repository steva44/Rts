#ifndef RSolidModelImportMethod_H
#define RSolidModelImportMethod_H
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

#include "RObject.h"
#include "RSmartPointer.h"
#include "RMathUtils.h"

// Ifc++ includes
#include <IFC4/include/IfcExtrudedAreaSolid.h>
#include <IFC4/include/IfcSolidModel.h>
#include <IFC4/include/IfcMappedItem.h>
#include <IFC4/include/IfcFace.h>
#include "Carve/SolidModelConverter.h"

class RDomain;
class RPoint;
class GeometrySettings;
class UnitConverter;
class FaceConverter;
class CurveConverter;
class ProfileCache;
class Sweeper;
class PointConverter;
class RIFCImportData;
class IfcBooleanResult;
class RSteelSectionImporter;

class RSolidModelImportMethod : public RObject
{
    Q_OBJECT

public:
    RSolidModelImportMethod(shared_ptr<GeometrySettings>& gs,
                            shared_ptr<UnitConverter>& uc,
                            RDomain* domain);
    virtual ~RSolidModelImportMethod();

    // Converts an IfcSolidModel geometry representation object into a Rts component
    // IfcSolidModel is a base class of one of IfcCsgSolid, IfcManifoldSolidBrep, IfcSweptAreaSolid, or IfcSweptDiskSolid
    void createComponentFromIfcSolidModel(const shared_ptr<IfcSolidModel>& solid_model, shared_ptr<RIFCImportData>& inputItemData, const std::string& theEntityType);

    // Converts an IfcBooleanResult geometry representation object into a Rts component
    void createComponentFromIfcBooleanResult(const shared_ptr<IfcBooleanResult>& bool_result, shared_ptr<RIFCImportData>& inputItemData);

    // Unit converter to convert the geometry from the units used in the BIM to m, here units always in m
    shared_ptr<UnitConverter> unitConverter;

private:

    // Tools from Ifc++
    shared_ptr<GeometrySettings> geometrySettings;
    shared_ptr<PointConverter> pointConverter;
    shared_ptr<CurveConverter> curveConverter;
    shared_ptr<FaceConverter> faceConverter;
    shared_ptr<ProfileCache> profileCache;
    shared_ptr<PlacementConverter> placementConverter;
    shared_ptr<SplineConverter>	splineConverter;
    shared_ptr<Sweeper>	geometrySweeper;
    unique_ptr<SolidModelConverter> IfcPPSolidModelConverter;

    // Tool that provides common math operations
    RMathUtils mathOps;

    // Returns a vector that contains the coordinates of the centroid of an IFCFace
    vec3 getCentroidFromFace(shared_ptr<IfcFace>& ifcFace);

    // Returns a vector that contains the coordinates of the centroid of an IFCFace that is given as a vector of points
    vec3 getCentroidFromFaceVector(std::vector<vec3>& Facevector);

    // Function that merges coplanar faces into one face
    int mergeCoplanarFaces(std::vector<std::vector<vec3> >& faceVector);

    // A function to check if a point is coplanar with three other points, from Shewchuk_predicates
    // If returns a zero then the point pd is coplanar with points pa, pb, and pc
    inline double orient3dfast(const vec3& pa, const vec3& pb, const vec3& pc, const vec3& pd);

    // Creates beam or column component from two cross-section centroid points and an extrusion vector, the "profilePaths" vector contains the cross-section
    template<typename T>
    void createBeamColumnGeometryFromSolid(const vec3& faceCentroid1,
                                           const vec3& faceCentroid2,
                                           const vec3& extrusionVector,
                                           IfcPPEntity* ifcEntity,
                                           shared_ptr<RIFCImportData>& inputItemData,
                                           const double& lengthFactor,
                                           const std::string& theEntityType,
                                           const std::vector<std::vector<T> >& profilePaths);

    // Creates and slab component by extruding the "profilePaths" vector containing the cross-section
    void createSlabGeometryFromSolid(const std::vector<std::vector<vec2> >& profilePaths, const vec3 extrusionVector, IfcPPEntity* ifcEntity, shared_ptr<RIFCImportData>& inputItemData, const double& lengthFactor);

    // Creates a window component from the "profilePaths" vector containing the cross-section
    void createWindowGeometryFromSolid(const std::vector<std::vector<vec3> >& profilePaths, IfcPPEntity* ifcEntity, shared_ptr<RIFCImportData>& inputItemData, const double& lengthFactor);

    // Creates a wall component by extruding the "profilePaths" vector containing the cross-section
    void createWallGeometryFromSolid(const std::vector<std::vector<vec2> >& profilePaths, const vec3 extrusionVector, IfcPPEntity* ifcEntity, shared_ptr<RIFCImportData>& inputItemData, const double& lengthFactor);

    // Creates a wall component by extruding the "profilePaths" vector containing the cross-section
    // The IfcWallStandardCase handles all cases of walls that are extruded vertically along the positive z axis of the wall in local coordinate system
    void createStdCaseWallGeometryFromSolid(const std::vector<std::vector<vec2> >& profilePaths, const vec3 extrusionVector, IfcPPEntity* ifcEntity, shared_ptr<RIFCImportData>& inputItemData, const double& lengthFactor);

    // Calculates the IFC object placement within the global coordinates of the building, this function is taken from ifc++ library
    void convertIfcObjectPlacement(const shared_ptr<IfcObjectPlacement>& ifcObjectPlacement,
                                   const double lengthFactor,
                                   carve::math::Matrix& resultingMatrix,
                                   std::unordered_set<IfcObjectPlacement*>& placementAlreadyApplied,
                                   bool onlyRotation = false );

    // Creates a point in the domain
    RPoint* createAPoint(const QString& objectName, std::vector<double>& coords);

    // The steel section importer reads cross-section information from a database of standard steel components
    // e.g., given the steel component tag W10x120, the section importer provides the cross-section parameters like flange thickness, web height, etc.
    unique_ptr<RSteelSectionImporter> theSectionImporter;

    // Multiplies a vector with a matrix and returns a vector
    std::vector<double> preMultiplyVectorMatrix(const carve::math::Matrix & _mat, const std::vector<double>& v );

    // Plots points for debugging purposes
    void plotPointForDebug(const vec3& point, shared_ptr<RIFCImportData>& inputItemData, const double& lengthFactor );
};

#endif
