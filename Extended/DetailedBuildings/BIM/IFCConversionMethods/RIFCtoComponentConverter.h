#ifndef RIFCtoComponentConverter_H
#define RIFCtoComponentConverter_H
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

// This class contains the functions that convert IFC products (IfcColumn, IfcBeam, etc.) into components (RColumn, RBeam)

#include "carve/mesh.hpp"
#include "RSmartPointer.h"

class GeometrySettings;
class UnitConverter;
class RDomain;
class IfcRepresentation;
class RIFCImportData;
class RSolidModelImportMethod;
class ProductShapeData;

class RIFCtoComponentConverter
{

public:
    RIFCtoComponentConverter(shared_ptr<GeometrySettings> geomSettings, shared_ptr<UnitConverter> unitConverter, RDomain* domain);
    virtual ~RIFCtoComponentConverter();

    // Ifc++ unit converter
    inline shared_ptr<UnitConverter>& getUnitConverter(){ return theUnitConverter; }

    // Ifc++ geometry settings
    inline shared_ptr<GeometrySettings>& getGeomSettings(){ return theGeometrySettings; }

    // Creates a beam or column component from an IFC product
    void componentFromIFCBeamColumn(const shared_ptr<IfcRepresentation>& representation, shared_ptr<RIFCImportData>& theInputData, const std::string& theEntityType);

    // Creates a wall or slab component from an IFC product
    void componentFromIFCSlabWall(const shared_ptr<IfcRepresentation>& representation, shared_ptr<RIFCImportData>& theInputData, const std::string& theEntityType);

    // Creates a window component from an IFC product
    void componentFromIFCWindow(const shared_ptr<IfcRepresentation>& representation, shared_ptr<RIFCImportData>& theInputData, const std::string& theEntityType);

    // Functions to handle voids within the components
    void getVoidsFromIFCRepresentation(const shared_ptr<ProductShapeData>& voidShapeData, shared_ptr<RIFCImportData>& theInputData);
    void addVoidsToComponent(const shared_ptr<carve::mesh::MeshSet<3>>& meshSet, shared_ptr<RIFCImportData>& theInputData);

protected:

    // Ifc++ geometry settings
    shared_ptr<GeometrySettings> theGeometrySettings;

    // Ifc++ unit converter
    shared_ptr<UnitConverter> theUnitConverter;

    // Method to import solid model geometry from IFC, and to create components from this geometry
    shared_ptr<RSolidModelImportMethod> theSolidModelImporter;

};

#endif
