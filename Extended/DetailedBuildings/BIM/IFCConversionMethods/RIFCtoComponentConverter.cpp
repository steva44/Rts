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

#include "RIFCtoComponentConverter.h"
#include "RIFCImportData.h"
#include "RDomain.h"
#include "RSolidModelImportMethod.h"
#include "UnitConverter.h"
#include "GeometrySettings.h"
#include "StylesConverter.h"
#include "IfcRepresentation.h"
#include "IfcRepresentationMap.h"
#include "IfcPolyline.h"
#include "IfcShapeModel.h"
#include "IfcShapeRepresentation.h"
#include "IfcTopologyRepresentation.h"
#include "IncludeCarveHeaders.h"
#include "Carve/ProfileCache.h"
#include "Carve/Sweeper.h"
#include "Carve/SplineConverter.h"
#include "Carve/PointConverter.h"
#include "Carve/CurveConverter.h"
#include "Carve/FaceConverter.h"
#include "Carve/PlacementConverter.h"
#include "geom2d.hpp"

RIFCtoComponentConverter::RIFCtoComponentConverter(shared_ptr<GeometrySettings> geomSettings, shared_ptr<UnitConverter> unitConverter, RDomain* domain)
    : theGeometrySettings(geomSettings), theUnitConverter(unitConverter)
{

    theSolidModelImporter = std::make_shared<RSolidModelImportMethod>(theGeometrySettings, theUnitConverter, domain);
}


RIFCtoComponentConverter::~RIFCtoComponentConverter()
{

}


void RIFCtoComponentConverter::componentFromIFCBeamColumn(const shared_ptr<IfcRepresentation>& representation, shared_ptr<RIFCImportData>& theInputData, const std::string& theEntityType)
{
    if(representation->m_RepresentationIdentifier)
    {
        theInputData->representationIdentifier = representation->m_RepresentationIdentifier->m_value;
    }

    if(representation->m_RepresentationType)
    {
        theInputData->representationIdentifier = representation->m_RepresentationType->m_value;
    }

    theInputData->theIfcRepresentationContext = representation->m_ContextOfItems;

    // IfcRepresentationItem is an abstract base class for IfcGeometricRepresentationItem, IfcMappedItem, IfcStyledItem, IfcTopologicalRepresentationItem;
    for(auto&& it : representation->m_Items)
    {

        // IfcGeometricRepresentationItem is an abstract base class for:
        // IfcAnnotationFillArea, IfcBooleanResult, IfcBoundingBox, IfcCartesianTransformationOperator, IfcCompositeCurveSegment, IfcCsgPrimitive3D,
        // IfcCurve, IfcDefinedSymbol, IfcDirection, IfcFaceBasedSurfaceModel, IfcFillAreaStyleHatching, IfcFillAreaStyleTiles, IfcFillAreaStyleTileSymbolWithStyle,
        // IfcGeometricSet, IfcHalfSpaceSolid, IfcLightSource, IfcOneDirectionRepeatFactor, IfcPlacement, IfcPlanarExtent, IfcPoint, IfcSectionedSpine,
        // IfcShellBasedSurfaceModel, IfcSolidModel, IfcSurface, IfcTextLiteral, IfcTextureCoordinate, IfcTextureVertex, IfcVector))
        shared_ptr<IfcGeometricRepresentationItem> theIfcGeomItem = dynamic_pointer_cast<IfcGeometricRepresentationItem>( it );
        if( theIfcGeomItem )
        {

            //  For now only IfcSolidModel is supported for columns
            shared_ptr<IfcSolidModel> theIfcSolidModel = dynamic_pointer_cast<IfcSolidModel>( theIfcGeomItem );
            if( theIfcSolidModel )
            {
                theSolidModelImporter->createComponentFromIfcSolidModel(theIfcSolidModel, theInputData, theEntityType);
                return;
            }

            shared_ptr<IfcBooleanResult> theIfcBooleanResult = dynamic_pointer_cast<IfcBooleanResult>( theIfcGeomItem );
            if( theIfcBooleanResult )
            {
                theSolidModelImporter->createComponentFromIfcBooleanResult( theIfcBooleanResult, theInputData );
                return;
            }

            qDebug()<<"Unhandled IfcGeometricRepresentationItem IFC Representation";

            continue;
        }

        shared_ptr<IfcMappedItem> theIfcMappedItem = dynamic_pointer_cast<IfcMappedItem>( it );
        if(theIfcMappedItem)
        {
            shared_ptr<IfcRepresentationMap> map_source = theIfcMappedItem->m_MappingSource;
            if( !map_source )
            {
                qDebug()<<"MappingSource not valid";
                continue;
            }
            shared_ptr<IfcRepresentation> mapped_representation = map_source->m_MappedRepresentation;
            if( !mapped_representation )
            {
                qDebug()<<"MappingSource MappedRepresentation not valid";
                continue;
            }

            componentFromIFCBeamColumn( mapped_representation, theInputData, theEntityType);

            continue;
        }

        qDebug()<<"unhandled IfcRepresentationItem representation";
    }
}


void RIFCtoComponentConverter::componentFromIFCWindow( const shared_ptr<IfcRepresentation>& representation, shared_ptr<RIFCImportData>& theInputData, const std::string& theEntityType )
{
    if( representation->m_RepresentationIdentifier )
    {
        theInputData->representationIdentifier = representation->m_RepresentationIdentifier->m_value;
    }

    if( representation->m_RepresentationType )
    {
        theInputData->representationIdentifier = representation->m_RepresentationType->m_value;
    }

    theInputData->theIfcRepresentationContext = representation->m_ContextOfItems;

    // IfcRepresentationItem is an abstract base class for IfcGeometricRepresentationItem, IfcMappedItem, IfcStyledItem, IfcTopologicalRepresentationItem;
    for( auto&& it : representation->m_Items)
    {

        // IfcGeometricRepresentationItem is an abstract base class for:
        // IfcAnnotationFillArea, IfcBooleanResult, IfcBoundingBox, IfcCartesianTransformationOperator, IfcCompositeCurveSegment, IfcCsgPrimitive3D,
        // IfcCurve, IfcDefinedSymbol, IfcDirection, IfcFaceBasedSurfaceModel, IfcFillAreaStyleHatching, IfcFillAreaStyleTiles, IfcFillAreaStyleTileSymbolWithStyle,
        // IfcGeometricSet, IfcHalfSpaceSolid, IfcLightSource, IfcOneDirectionRepeatFactor, IfcPlacement, IfcPlanarExtent, IfcPoint, IfcSectionedSpine,
        // IfcShellBasedSurfaceModel, IfcSolidModel, IfcSurface, IfcTextLiteral, IfcTextureCoordinate, IfcTextureVertex, IfcVector))
        shared_ptr<IfcGeometricRepresentationItem> theIfcGeomItem = dynamic_pointer_cast<IfcGeometricRepresentationItem>( it );
        if( theIfcGeomItem )
        {

            shared_ptr<IfcSolidModel> theIfcSolidModel = dynamic_pointer_cast<IfcSolidModel>( theIfcGeomItem );
            if( theIfcSolidModel )
            {
                theSolidModelImporter->createComponentFromIfcSolidModel(theIfcSolidModel, theInputData, theEntityType);
                return;
            }

            qDebug()<<"Unhandled IfcGeometricRepresentationItem of type "<<it->className()<<" of object "<<QString::fromStdWString(theInputData->objectName);

            continue;
        }


        shared_ptr<IfcMappedItem> theIfcMappedItem = dynamic_pointer_cast<IfcMappedItem>( it );
        if(theIfcMappedItem)
        {
            shared_ptr<IfcRepresentationMap> map_source = theIfcMappedItem->m_MappingSource;
            if(!map_source)
            {
                qDebug()<<"MappingSource not valid";
                continue;
            }
            shared_ptr<IfcRepresentation> mapped_representation = map_source->m_MappedRepresentation;
            if(!mapped_representation)
            {
                qDebug()<<"MappingSource MappedRepresentation not valid";
                continue;
            }

            this->componentFromIFCWindow( mapped_representation, theInputData, theEntityType);

            continue;
        }

        qDebug()<<"Unhandled IfcRepresentationItem of type "<<it->className()<<" of object "<<QString::fromStdWString(theInputData->objectName);
    }
}


void RIFCtoComponentConverter::componentFromIFCSlabWall( const shared_ptr<IfcRepresentation>& representation, shared_ptr<RIFCImportData>& theInputData, const std::string& theEntityType )
{
    if( representation->m_RepresentationIdentifier )
    {
        theInputData->representationIdentifier = representation->m_RepresentationIdentifier->m_value;
    }

    if( representation->m_RepresentationType )
    {
        theInputData->representationIdentifier = representation->m_RepresentationType->m_value;
    }

    theInputData->theIfcRepresentationContext = representation->m_ContextOfItems;

    // IfcRepresentationItem is an abstract base class for IfcGeometricRepresentationItem, IfcMappedItem, IfcStyledItem, IfcTopologicalRepresentationItem;
    for( auto&& it : representation->m_Items)
    {
        // IfcGeometricRepresentationItem is an abstract base class for:
        // IfcAnnotationFillArea, IfcBooleanResult, IfcBoundingBox, IfcCartesianTransformationOperator, IfcCompositeCurveSegment, IfcCsgPrimitive3D,
        // IfcCurve, IfcDefinedSymbol, IfcDirection, IfcFaceBasedSurfaceModel, IfcFillAreaStyleHatching, IfcFillAreaStyleTiles, IfcFillAreaStyleTileSymbolWithStyle,
        // IfcGeometricSet, IfcHalfSpaceSolid, IfcLightSource, IfcOneDirectionRepeatFactor, IfcPlacement, IfcPlanarExtent, IfcPoint, IfcSectionedSpine,
        // IfcShellBasedSurfaceModel, IfcSolidModel, IfcSurface, IfcTextLiteral, IfcTextureCoordinate, IfcTextureVertex, IfcVector))
        shared_ptr<IfcGeometricRepresentationItem> theIfcGeomItem = dynamic_pointer_cast<IfcGeometricRepresentationItem>( it );
        if( theIfcGeomItem )
        {

            shared_ptr<IfcSolidModel> theIfcSolidModel = dynamic_pointer_cast<IfcSolidModel>( theIfcGeomItem );
            if( theIfcSolidModel )
            {
                theSolidModelImporter->createComponentFromIfcSolidModel( theIfcSolidModel, theInputData, theEntityType);
                return;
            }

            shared_ptr<IfcPolyline> theIfcPolyLineModel = dynamic_pointer_cast<IfcPolyline>( theIfcGeomItem );
            if(theIfcPolyLineModel)
            {
                return;
            }

            qDebug()<<"Unhandled IfcGeometricRepresentationItem of type "<<it->className()<<" of object "<<QString::fromStdWString(theInputData->objectName);
        }
    }
}


void RIFCtoComponentConverter::getVoidsFromIFCRepresentation( const shared_ptr<ProductShapeData>& voidShapeData, shared_ptr<RIFCImportData>& theInputData)
{
    // Get the representation data from the product shape data
    std::vector<shared_ptr<RepresentationData> > theRepVec = voidShapeData->m_vec_representations;
    for(auto repData : theRepVec)
    {
        // Get the item shape data from the representation data
        std::vector<shared_ptr<ItemShapeData> >	theItemShapeVec = repData->m_vec_item_data;
        for(auto itemShape : theItemShapeVec)
        {
            // Get the mesh-sets from the voids vector (mesh openings)
            // These were saved previously in the method subtractOpenings - thus we do not have to convert the representations into mesh again
            std::vector<shared_ptr<carve::mesh::MeshSet<3> > > vecOfMeshSets = itemShape->m_meshsets_of_openings;
            for(auto openingMeshSet : vecOfMeshSets)
            {
                this->addVoidsToComponent(openingMeshSet,theInputData);
            }
        }
    }
}


void RIFCtoComponentConverter::addVoidsToComponent( const shared_ptr<carve::mesh::MeshSet<3>>& meshSet, shared_ptr<RIFCImportData>& theInputData)
{
    // Get the individual meshes from the mesh set
    std::vector<carve::mesh::Mesh<3>*> vecOfMeshes = meshSet->meshes;
    if(vecOfMeshes.size()!= 1)
    {
        qDebug()<<"Only mesh sets of size one are supported in convertVoidMesh";
        return;
    }

    // Get the component
    auto theComponent = theInputData->theRComponent;

    carve::mesh::Mesh<3>* theMesh = vecOfMeshes.at(0);

    // Get the volume of the void
    auto volume = theMesh->volume();

    // Calculate the area of the void
    auto area = 0.0;
    std::vector<carve::mesh::Face<3>*> facesOfMesh = theMesh->faces;
    for(auto it: facesOfMesh)
    {

        std::vector<carve::mesh::Vertex<3>*> vecVerts;

        it->getVertices(vecVerts);

        double A = carve::geom2d::signedArea(vecVerts, it->projector());
        area += fabs(A)*0.5;
    }

    // Save the void characteristics to the component
    theComponent->addVoid(area,volume);

    return;
}

