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

#include "RSolidModelImportMethod.h"
#include "RDomain.h"
#include "RPoint.h"
#include "ROsgDrawUtils.h"
#include "RMathUtils.h"
#include "RIFCImportData.h"
#include "RSteelSectionImporter.h"
#include "RRectangularRCColumnComponent.h"
#include "RRCSlabComponent.h"
#include "RNonLoadBearingWallComponent.h"
#include "RCLTPanelComponent.h"
#include "RRectangularGlulamBeamColumnComponent.h"
#include "RWSteelBeamColumnComponent.h"
#include "RHSSRoundBeamColumnComponent.h"
#include "RStandardSteelComponent.h"
#include "RCircularRCColumnComponent.h"
#include "RCompositePanelComponent.h"
#include "RWallComponent.h"
#include "RRCShearWallComponent.h"
#include "RWindowComponent.h"

// Ifc++ includes
#include "GeometrySettings.h"
#include "PointConverter.h"
#include "ProfileCache.h"
#include "FaceConverter.h"
#include "CurveConverter.h"
#include "Sweeper.h"
#include "CSG_Adapter.h"
#include "IfcObject.h"
#include "IfcLabel.h"
#include "IfcBooleanResult.h"
#include "UnitConverter.h"

#include <regex>

typedef std::vector<std::vector<vec3> > vecLoop;

RSolidModelImportMethod::RSolidModelImportMethod(shared_ptr<GeometrySettings>& gs, shared_ptr<UnitConverter>& uc, RDomain* domain)
    : unitConverter(uc), geometrySettings(gs)
{
    theDomain = domain;

    theSectionImporter = make_unique<RSteelSectionImporter>(this);
    geometrySweeper = shared_ptr<Sweeper>(new Sweeper(geometrySettings, unitConverter));
    pointConverter = shared_ptr<PointConverter>(new PointConverter(unitConverter));
    splineConverter = shared_ptr<SplineConverter>(new SplineConverter(pointConverter));
    placementConverter = shared_ptr<PlacementConverter>(new PlacementConverter(unitConverter));
    curveConverter = shared_ptr<CurveConverter>(new CurveConverter(geometrySettings, placementConverter, pointConverter, splineConverter));
    faceConverter = shared_ptr<FaceConverter>(new FaceConverter(geometrySettings, unitConverter, curveConverter, splineConverter, geometrySweeper));
    profileCache = shared_ptr<ProfileCache>(new ProfileCache(curveConverter, splineConverter));
    IfcPPSolidModelConverter = make_unique<SolidModelConverter>(geometrySettings, pointConverter, curveConverter, faceConverter, profileCache, geometrySweeper);

}


RSolidModelImportMethod::~RSolidModelImportMethod()
{

}


void RSolidModelImportMethod::createComponentFromIfcSolidModel( const shared_ptr<IfcSolidModel>& solid_model, shared_ptr<RIFCImportData>& inputItemData, const std::string& theEntityType)
{

    // Get the conversion factor from the unit conveter
    auto lengthConversionFactor = unitConverter->getLengthInMeterFactor();

    // IFC class Heirarchy for IfcSweptAreaSolid
    // --------------------------IfcRepresentationItem
    // -------------------IfcGeometricRepresentationItem
    // ------------IfcSolidModel
    // -----IfcSweptAreaSolid
    shared_ptr<IfcSweptAreaSolid> swept_area_solid = dynamic_pointer_cast<IfcSweptAreaSolid>( solid_model );
    if( swept_area_solid )
    {
        shared_ptr<IfcProfileDef>& swept_area = swept_area_solid->m_SweptArea;
        if(!swept_area)
        {
            qCritical()<<"SweptArea not valid in "<<__FUNCTION__;
            return;
        }

        shared_ptr<ItemShapeData> item_data_solid(new ItemShapeData());
        if(!item_data_solid)
        {
            qCritical()<<"Out of memory in "<<__FUNCTION__;
            return;
        }

        // Check if local coordinate system is specified for extrusion
        shared_ptr<TransformData> primitive_placement_transform;
        if(swept_area_solid->m_Position)
        {
            shared_ptr<IfcAxis2Placement3D> swept_area_position = swept_area_solid->m_Position;
            placementConverter->convertIfcAxis2Placement3D(swept_area_position, primitive_placement_transform);
        }

        shared_ptr<IfcExtrudedAreaSolid> extrudedArea = dynamic_pointer_cast<IfcExtrudedAreaSolid>(swept_area_solid);
        if(extrudedArea)
        {
            if(!extrudedArea->m_ExtrudedDirection)
            {
                qCritical()<<"Invalid extruded direction in "<<__FUNCTION__;
                return;
            }

            if(!extrudedArea->m_Depth)
            {
                qCritical()<<"Invalid Depth in "<<__FUNCTION__;
                return;
            }

            // Get the length of extrusion
            const auto depth = extrudedArea->m_Depth->m_value*lengthConversionFactor;

            // Get the direction of the member
            vec3 extrusionVector;
            std::vector<shared_ptr<IfcReal> >& vec_direction = extrudedArea->m_ExtrudedDirection->m_DirectionRatios;
            if(GeomUtils::allPointersValid(vec_direction))
            {
                if(vec_direction.size() > 2)
                {
                    extrusionVector = carve::geom::VECTOR(vec_direction[0]->m_value * depth, vec_direction[1]->m_value * depth, vec_direction[2]->m_value * depth);
                }
                else if(vec_direction.size() > 1)
                {
                    extrusionVector = carve::geom::VECTOR(vec_direction[0]->m_value * depth, vec_direction[1]->m_value * depth, 0);
                }
            }

            // Get the "swept area" from the Ifc profile definition
            shared_ptr<IfcProfileDef> sweptArea = extrudedArea->m_SweptArea;
            if(!sweptArea)
            {
                qDebug()<<"Invalid SweptArea in "<<__FUNCTION__;
                return;
            }

            shared_ptr<ProfileConverter> profileConverter = profileCache->getProfileConverter(sweptArea);
            profileConverter->simplifyPaths();

            // Get the vertices of the profile path after conversion
            const std::vector<std::vector<vec2> >& paths = profileConverter->getCoordinates();

            if(paths.size() == 0)
            {
                qCritical()<<"The path size is zero in sweptAreaSolid in "<<__FUNCTION__;
                return;
            }
            else if(paths.size() > 1)
                qCritical()<<"Warning, more than one path came into the RSolidModelImportMethod, only one path is supported in "<<__FUNCTION__;

            // Get the centroid of the paths
            auto theProfileCentroid = mathOps.computePolygonCentroid2D(paths[0]);

            // To do: Here we need to figure out the type of shape from the paths is it a quad? circle? c/L-section? An I beam?

            if(theEntityType == "IfcBeam")
            {
                // Since we work in 2D for the swept area solid - need to convert to global, for a beam this should be
                // The local x direction is the global z direction
                // The local y direction is the global x direction
                // The local z direction is the global y direction

                // Calculate the end points of the column, i.e., the centroids
                auto centroid1(carve::geom::VECTOR(theProfileCentroid[1], 0.0,theProfileCentroid[0]));
                auto centroid2(carve::geom::VECTOR(theProfileCentroid[1]+ extrusionVector[1], extrusionVector[2], theProfileCentroid[0] + extrusionVector[0]));

                // Create the Rts geometry
                createBeamColumnGeometryFromSolid(centroid1, centroid2, extrusionVector,
                                                  solid_model.get(), inputItemData,
                                                  lengthConversionFactor, theEntityType, paths);
            }
            else if(theEntityType == "IfcColumn")
            {
                // Since we work in 2D for the swept area solid - need to convert to global, for a column this should be
                // The local x direction is the global x direction
                // The local y direction is the global y direction
                // The local z direction is the global z direction

                // Calculate the end points of the column, i.e., the centroids
                auto centroid1(carve::geom::VECTOR(theProfileCentroid[1], 0.0,theProfileCentroid[0]));
                auto centroid2(carve::geom::VECTOR(theProfileCentroid[1]+ extrusionVector[0], extrusionVector[1], theProfileCentroid[0] + extrusionVector[2]));

                // Create the beam column in Rts
                createBeamColumnGeometryFromSolid(centroid1,centroid2,extrusionVector,solid_model.get(),inputItemData, lengthConversionFactor, theEntityType,paths);
            }
            else if(theEntityType == "IfcSlab")
            {
                // Create the slab in Rts
                createSlabGeometryFromSolid(paths, extrusionVector, extrudedArea.get(),inputItemData, lengthConversionFactor);
            }
            else if(theEntityType == "IfcWall")
            {
                // Create the wall in Rts
                createWallGeometryFromSolid(paths, extrusionVector, extrudedArea.get(),inputItemData, lengthConversionFactor);
            }
            else if(theEntityType == "IfcWallStandardCase")
            {
                // Create the wall in Rts
                createStdCaseWallGeometryFromSolid(paths, extrusionVector, extrudedArea.get(),inputItemData, lengthConversionFactor);
            }
            else
                qCritical()<<"Error, the IFC entity type is not supported in "<<__FUNCTION__;

            return;
        }

        qDebug()<<"Unhandled IfcSweptAreaSolid IFC Representation in "<<__FUNCTION__;
    }


    // IFC class Heirarchy for IfcManifoldSolidBrep -- Brep stands for "Boundary REPresentation"
    // ----------------------IfcRepresentationItem
    // -----------------IfcGeometricRepresentationItem
    // -------------IfcSolidModel
    // --------IfcManifoldSolidBrep
    // ----IfcFacetedBrep
    shared_ptr<IfcManifoldSolidBrep> manifoldBrepSolid = dynamic_pointer_cast<IfcManifoldSolidBrep>(solid_model);
    if(manifoldBrepSolid)
    {
        // IfcManifoldSolidBrep is a base class of one of IfcAdvancedBrep, or IfcFacetedBrep
        shared_ptr<IfcClosedShell>& outer_shell = manifoldBrepSolid->m_Outer;
        if(outer_shell)
        {
            // Convert outer shell into face loops
            std::vector<shared_ptr<IfcFace> >& facesOuterShell = outer_shell->m_CfsFaces;

            vecLoop theFaceLoopVector;

            // Iterate through the face looop
            for(auto&& ifcFace : facesOuterShell)
            {
                if(!ifcFace)
                {
                    continue;
                }
                const std::vector<shared_ptr<IfcFaceBound> >& vecBounds = ifcFace->m_Bounds;
                vecLoop faceLoops;

                for(auto&& it : vecBounds)
                {
                    const shared_ptr<IfcFaceBound>& faceBound = it;

                    if(!faceBound)
                        continue;

                    // IfcLoop is a base class of one of IfcEdgeLoop, IfcPolyLoop, or IfcVertexLoop
                    const shared_ptr<IfcLoop>& loop = faceBound->m_Bound;
                    if( !loop )
                    {
                        if(faceBound == (*vecBounds.begin()))
                            break;
                        else
                            continue;
                    }

                    faceLoops.push_back(std::vector<vec3>());
                    std::vector<vec3>& loopPoints = faceLoops.back();
                    curveConverter->convertIfcLoop(loop, loopPoints);

                    if(loopPoints.size() < 3)
                    {
                        if(faceBound == (*vecBounds.begin()))
                            break;
                        else
                            continue;
                    }

                    bool orientation = true;
                    if(faceBound->m_Orientation)
                        orientation = faceBound->m_Orientation->m_value;

                    if(!orientation)
                        std::reverse(loopPoints.begin(), loopPoints.end());

                    theFaceLoopVector.push_back(loopPoints);
                }
            }

            int numRemoved = 0;

            // Merge the coplanar faces, some BIM programs add extraneous faces on a surface
            numRemoved = mergeCoplanarFaces(theFaceLoopVector);

            // Compare and sort the vectors according to size
            auto comPareVectors = [](const std::vector<vec3>& a,const std::vector<vec3>& b){ return a.size() > b.size();};
            std::sort(theFaceLoopVector.begin(),theFaceLoopVector.end(),comPareVectors);

            // Get the ifc product
            auto theIfcProduct = inputItemData->theIfcProduct.lock();

            if(theEntityType == "IfcBeam")
            {
                // Find the faces that are the furthest apart, those will be the beam or column ends in the majority of cases
                // The only way this will not be the case will be for very short columns and very deep beams
                // Start with the first sorted point, as usually the most complex face (aka most vertices) is that of a cross-section
                // Calculate the centroid from all of the faces
                std::vector<vec3> theCentroidFaceVector;
                for (auto&& it : theFaceLoopVector)
                    theCentroidFaceVector.push_back(getCentroidFromFaceVector(it));

                vec3 point1 = theCentroidFaceVector.front();
                vec3 point2;
                vec3 previousPoint;

                // This function returns the squared distance between two points; square root operations are expensive so should avoid!
                auto distanceBtwnPts = [](const vec3& pt1, const vec3& pt2)
                {
                    return (pt2[0]-pt1[0])*(pt2[0]-pt1[0])+(pt2[1]-pt1[1])*(pt2[1]-pt1[1])+(pt2[2]-pt1[2])*(pt2[2]-pt1[2]);
                };

                double distancePrev = 0.0;
                size_t count = 0;
                while(count < theCentroidFaceVector.size())
                {
                    for(auto&& it : theCentroidFaceVector)
                    {
                        if (point1 == it)
                            continue;

                        double distanceNow = distanceBtwnPts(point1,it);

                        if(distanceNow >= distancePrev)
                        {
                            point2 = it;
                            distancePrev = distanceNow;
                        }
                    }

                    if(point2 == previousPoint)
                        break;

                    else
                    {
                        previousPoint = point1;
                        point1 = point2;
                        ++count;
                    }
                }

                // Calculate the extrusion vector
                auto extrusionVector = point1-point2;

                // Create the beam or column geometry
                createBeamColumnGeometryFromSolid(point1,point2,extrusionVector,solid_model.get(),inputItemData, lengthConversionFactor, theEntityType,theFaceLoopVector);
            }
            else if(theEntityType == "IfcWindow")
            {
                // Create the window component
                this->createWindowGeometryFromSolid(theFaceLoopVector,solid_model.get(),inputItemData, lengthConversionFactor);
            }
            else
            {
                qCritical()<<"Unhandled entity type in "<<__FUNCTION__;
                return;
            }
        }

        shared_ptr<IfcFacetedBrep> faceted_brep = dynamic_pointer_cast<IfcFacetedBrep>(manifoldBrepSolid);
        if(faceted_brep)
        {
            // The more complex case is handled above
            return;
        }

        // The outer shell has the potential to have voids.
        shared_ptr<IfcAdvancedBrep> advanced_brep = dynamic_pointer_cast<IfcAdvancedBrep>(manifoldBrepSolid);
        if(advanced_brep)
        {
            // IfcAdvancedBrep is a base class of IfcAdvancedBrepWithVoids
            if(dynamic_pointer_cast<IfcAdvancedBrepWithVoids>(advanced_brep))
            {
                // shared_ptr<IfcAdvancedBrepWithVoids> advanced_brep_with_voids = dynamic_pointer_cast<IfcAdvancedBrepWithVoids>( solid_model );
                // std::vector<shared_ptr<IfcClosedShell> >& vec_voids = advanced_brep_with_voids->m_Voids;
                // To do: implement this, subtract voids from outer shell

                qCritical()<<"IfcAdvancedBrep not implemented in "<<__FUNCTION__;
            }
            return;
        }

        qCritical()<<"Unhandled manifoldBrepSolid representation in "<<__FUNCTION__;
    }

    qCritical()<<"Unhandled IfcSolidModel representation in "<<__FUNCTION__;

    return;
}


void RSolidModelImportMethod::createComponentFromIfcBooleanResult( const shared_ptr<IfcBooleanResult>& bool_result, shared_ptr<RIFCImportData>& inputItemData )
{
    shared_ptr<IfcBooleanOperator>& ifc_boolean_operator = bool_result->m_Operator;
    shared_ptr<IfcBooleanOperand> ifc_first_operand = bool_result->m_FirstOperand;
    shared_ptr<IfcBooleanOperand> ifc_second_operand = bool_result->m_SecondOperand;
    if( !ifc_boolean_operator || !ifc_first_operand || !ifc_second_operand )
    {
        qDebug()<<"Invalid IfcBooleanOperator or IfcBooleanOperand";
        return;
    }
    carve::csg::CSG::OP csg_operation = carve::csg::CSG::A_MINUS_B;

    if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_UNION )
    {
        csg_operation = carve::csg::CSG::UNION;
    }
    else if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_INTERSECTION )
    {
        csg_operation = carve::csg::CSG::INTERSECTION;
    }
    else if( ifc_boolean_operator->m_enum == IfcBooleanOperator::ENUM_DIFFERENCE )
    {
        csg_operation = carve::csg::CSG::A_MINUS_B;
    }
    else
    {
        qDebug()<<"Invalid IfcBooleanOperator in "<<__FUNCTION__;
        return;
    }

    shared_ptr<IfcPPEntity> entity_first_operand = dynamic_pointer_cast<IfcPPEntity>( ifc_first_operand );
    shared_ptr<IfcPPEntity> entity_second_operand = dynamic_pointer_cast<IfcPPEntity>( ifc_second_operand );

    // Convert the first operand
    shared_ptr<ItemShapeData> first_operand_data( new ItemShapeData() );
    if( !first_operand_data )
    {
        qDebug()<<"Out of memory in "<<__FUNCTION__;
    }
    shared_ptr<ItemShapeData> empty_operand;
    IfcPPSolidModelConverter->convertIfcBooleanOperand( ifc_first_operand, first_operand_data, empty_operand );

    // Convert the second operand
    shared_ptr<ItemShapeData> second_operand_data( new ItemShapeData() );
    if( !second_operand_data )
    {
        qDebug()<<"Out of memory in "<<__FUNCTION__;
    }
    IfcPPSolidModelConverter->convertIfcBooleanOperand( ifc_second_operand, second_operand_data, first_operand_data );

    // For every first operand polyhedrons, apply all second operand polyhedrons
    std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& vec_first_operand_meshsets = first_operand_data->m_meshsets;
    for(size_t i_meshset_first = 0; i_meshset_first < vec_first_operand_meshsets.size(); ++i_meshset_first)
    {
        shared_ptr<carve::mesh::MeshSet<3> >& first_operand_meshset = vec_first_operand_meshsets[i_meshset_first];
        std::vector<shared_ptr<carve::mesh::MeshSet<3> > >& vec_second_operand_meshsets = second_operand_data->m_meshsets;
        for( size_t i_meshset_second = 0; i_meshset_second < vec_second_operand_meshsets.size(); ++i_meshset_second )
        {
            shared_ptr<carve::mesh::MeshSet<3> >& second_operand_meshset = vec_second_operand_meshsets[i_meshset_second];
            shared_ptr<carve::mesh::MeshSet<3> > result;

            CSG_Adapter::computeCSG(first_operand_meshset, second_operand_meshset, csg_operation, result, nullptr, bool_result.get());

            first_operand_meshset = result;
        }
    }

    auto pt1 = first_operand_data->m_meshsets[0]->vertex_storage;

    // To do: finish boolean import

    return;
}


template<typename T>
void RSolidModelImportMethod::createBeamColumnGeometryFromSolid(const vec3& faceCentroid1,
                                                                const vec3& faceCentroid2,
                                                                const vec3& extrusionVector,
                                                                IfcPPEntity* ifcEntity,
                                                                shared_ptr<RIFCImportData>& inputItemData,
                                                                const double& length_factor,
                                                                const std::string& theEntityType,
                                                                const std::vector<std::vector<T> >& profilePaths)
{
    // Get the beam-column length
    const auto length = extrusionVector.length();

    auto theIfcProduct = inputItemData->theIfcProduct.lock();

    // Get the label to name the object
    auto theLabel = theIfcProduct->m_ObjectType.get();

    // Use the IfcLabel as part of the component name
    auto theObjectName = QString::fromStdWString(theLabel->m_value);

    // Append the unique id to the object name so that we know which ifc entity we created the component from
    auto theObjectId = theIfcProduct->m_entity_id;

    theObjectName.append("-ID_");
    theObjectName.append(QString::number(theObjectId));
    theObjectName.remove(' ');

    // Append the type of object to its name
    if(theEntityType == "IfcBeam")
        theObjectName.append("IfcBeam");
    else if(theEntityType == "IfcColumn")
        theObjectName.append("IfcColumn");

    // Set the object name to the input data item
    inputItemData->objectName = theObjectName.toStdWString();

    // Calculate the placement of this column in the world -- IfcProduct has an ObjectPlacement data member that can be a local or global placement
    // The product placement matrix is a 4x4 transformation matrix where the first three row/col are rotations,
    // With the 4th row/col being the homogeneous translation vector
    carve::math::Matrix productPlacementMatrix(carve::math::Matrix::IDENT());
    if(theIfcProduct->m_ObjectPlacement)
    {
        // IfcPlacement2Matrix follows related placements in case of local coordinate systems
        std::unordered_set<IfcObjectPlacement*> placementAlreadyApplied;
        this->convertIfcObjectPlacement(theIfcProduct->m_ObjectPlacement, length_factor, productPlacementMatrix, placementAlreadyApplied);
    }
    else
    {
        qCritical()<<"Could not calculate the placement of the object in "<<__FUNCTION__;
        return;
    }

    // Now that we can extract the product placement; make the points and set to beam
    std::vector<double> centroidVector1{faceCentroid1[0],faceCentroid1[1],faceCentroid1[2]};
    std::vector<double> centroidVector2{faceCentroid2[0],faceCentroid2[1],faceCentroid2[2]};

    // Rotate and translate a vector using the above transformation matrix
    auto theResult1 = preMultiplyVectorMatrix(productPlacementMatrix,centroidVector1);
    auto theResult2 = preMultiplyVectorMatrix(productPlacementMatrix,centroidVector2);

    // Create the first point in the domain
    auto pointOne =  this->createAPoint(theObjectName+"point1",theResult1);
    if(pointOne == nullptr)
    {
        return;
    }

    // Create the second point in the domain
    auto pointTwo = this->createAPoint(theObjectName+"point2",theResult2);
    if(pointTwo == nullptr)
    {
        return;
    }

    // Get the material type from the product
    auto theMatType = inputItemData->theMaterialType;

    QString theComponentID;

    // Test to see if it is a rectangular component (only the import of rectangular beam columns in supported)
    if(profilePaths.front().size() == 4)
    {
        auto crossSectionPoints = profilePaths.front();

        auto width01 = fabs(crossSectionPoints[0][0]- crossSectionPoints[1][0]);
        auto width23 = fabs(crossSectionPoints[2][0]- crossSectionPoints[3][0]);

        auto depth12 = fabs(crossSectionPoints[1][1]- crossSectionPoints[2][1]);
        auto depth30 = fabs(crossSectionPoints[3][1]- crossSectionPoints[0][1]);

        if(width01 - width23 < 0.0001 || depth12 - depth30 < 0.0001)
        {
            // Now that we know that it is a rectangle, get the material
            if(theMatType == RIFCImportData::materialType::concrete)
            {
                theComponentID = "RRectangularRCColumnComponent";
            }
            else if(theMatType == RIFCImportData::materialType::reinforcedConcrete)
            {
                theComponentID = "RRectangularRCColumnComponent";
            }
            else if(theMatType == RIFCImportData::materialType::wood)
            {
                theComponentID = "RRectangularGlulamBeamColumnComponent";
            }
            else if(theMatType == RIFCImportData::materialType::NA)
            {
                qDebug()<<"The material type was not given for "<<theObjectName<<" in "<<__FUNCTION__;
                return;
            }
            else
            {
                qDebug()<<"The type of wall "<<theMatType<<"is not supported for "<<theObjectName;
                return;
            }
        }
        else
        {
            qDebug()<<"Warning, the dimensions for a rectangle are not consistent in "<<__FUNCTION__<<" and the four point quadrilateral version needs to be implemented";
        }
    }
    else
    {
        qDebug()<<"Currently, only the import of rectangular cross-sections is supported in "<<__FUNCTION__;
        return;
    }

    // Component creation functions... the creation of some of these is not supported automatically, need to set manually here
    if(theComponentID == "RCircularRCColumnComponent")
    {
        // Create the component in the domain
        theDomain->createObject(theObjectName,"RCircularRCColumnComponent");

        auto theBeamColumn = qobject_cast<RCircularRCColumnComponent*>(theDomain->getLastAddedObject());
        if(theBeamColumn == nullptr)
        {
            qDebug()<<"Failed to create component in RDomain in "<<__FUNCTION__;
            return;
        }

        // To do: extract the cross-section geometry from BIM
        // Set the diameter parameter
        theDomain->createObject(theObjectName + "theDiameterParameter", "RConstant");
        auto theDiameter =  qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        theDiameter->setCurrentValue(0.10);

        // Set the concrete cover parameter
        theDomain->createObject(theObjectName + "theConcreteCover", "RConstant");
        auto theConcreteCover =  qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        theConcreteCover->setCurrentValue(0.025);

        // Set the reinforcement ratio parameters
        theDomain->createObject(theObjectName + "longReinforcementRatioParameter", "RConstant");
        auto theLongitudinalReinforcementRatio = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
        theLongitudinalReinforcementRatio->setCurrentValue(0.02);

        theDomain->createObject(theObjectName + "transReinforcementRatioParameter", "RConstant");
        auto theTransverseReinforcementRatio = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
        theTransverseReinforcementRatio->setCurrentValue(0.02);

        // Set the concrete strength
        theBeamColumn->setConcreteStrength("C25");

        // Set the parameters to the component
        theBeamColumn->setConcreteCover(theConcreteCover);
        theBeamColumn->setLongitudinalReinforcementRatio(theLongitudinalReinforcementRatio);
        theBeamColumn->setTransverseReinforcementRatio(theTransverseReinforcementRatio);
        theBeamColumn->setDiameter(theDiameter);
        theBeamColumn->setPoint1(pointOne);
        theBeamColumn->setPoint2(pointTwo);

        // Set the RObject to the item data
        inputItemData->theRComponent = theBeamColumn;
    }
    else if(theComponentID == "RWSteelBeamColumnComponent")
    {
        // Create the component in the domain
        theDomain->createObject(theObjectName,"RWSteelBeamColumnComponent");

        auto theBeamColumn = qobject_cast<RWSteelBeamColumnComponent*>(theDomain->getLastAddedObject());
        if(theBeamColumn == nullptr)
        {
            qDebug()<<"Failed to create component in RDomain in "<<__FUNCTION__;
            return;
        }

        // To do: get the shape type from BIM
        QString theWShape = "W12x87";

        // Just in case the shape type comes in lower case or with spaces
        theWShape.remove(" ");
        theWShape = theWShape.toUpper();

        auto stdCrossSection = theWShape.toStdString();

        // Create a text search pattern to match to common shape types in the cross-section database
        std::regex rxW("^(W|M|S|HP)[0-9]+");

        // Create the W section
        WSection theWSection;

        if(std::regex_search(stdCrossSection,rxW))
        {
            // Try to find the cross-section type in the database
            theWSection = theSectionImporter->getWSection(stdCrossSection);

            if(theWSection.Shape == "NULL")
            {
                qCritical()<<"Error, could not get the type "<<theWShape<<" from database";
                return;
            }

            theBeamColumn->setTheWSection(theWSection);
        }

        // Set the orientation
        theBeamColumn->setOrientation(QString::number(0.0));

        // Set the parameters to the component
        theBeamColumn->setPoint1(pointOne);
        theBeamColumn->setPoint2(pointTwo);

        // Set the RObject to the item data
        inputItemData->theRComponent = theBeamColumn;
    }
    else if(theComponentID == "RRectangularRCColumnComponent")
    {
        // Create the component in the domain
        theDomain->createObject(theObjectName,"RRectangularRCColumnComponent");

        auto theBeamColumn = qobject_cast<RRectangularRCColumnComponent*>(theDomain->getLastAddedObject());
        if(theBeamColumn == nullptr)
        {
            qDebug()<<"Failed to create component in RDomain in "<<__FUNCTION__;
            return;
        }

        // Get the points of the cross-section
        auto crossSectionPoints = profilePaths.front();

        // Calculate the width from the cross-section points
        auto width01 = fabs(crossSectionPoints[0][0]- crossSectionPoints[1][0]);

        // Calculate the depth from the cross-section points
        auto depth12 = fabs(crossSectionPoints[1][1]- crossSectionPoints[2][1]);

        // Create the cross-section width and depth parameters
        theDomain->createObject(theObjectName + "theCrossSectionWidth", "RConstant");
        auto theCrossSectionWidth =  qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        theCrossSectionWidth->setCurrentValue(width01);

        theDomain->createObject(theObjectName + "theCrossSectionDepth", "RConstant");
        auto theCrossSectionDepth =  qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        theCrossSectionDepth->setCurrentValue(depth12);

        // Create the concrete cover parameter
        theDomain->createObject(theObjectName + "theConcreteCover", "RConstant");
        auto theConcreteCover =  qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        theConcreteCover->setCurrentValue(0.025);

        // Create the reinforcement ratio parameters
        theDomain->createObject(theObjectName + "longReinforcementRatioParameter", "RConstant");
        auto theLongitudinalReinforcementRatio = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
        theLongitudinalReinforcementRatio->setCurrentValue(0.02);

        theDomain->createObject(theObjectName + "transReinforcementRatioParameter", "RConstant");
        auto theTransverseReinforcementRatio = qobject_cast<RConstant*>(theDomain->getLastAddedObject());
        theTransverseReinforcementRatio->setCurrentValue(0.02);

        // Set the orientation
        theBeamColumn->setOrientation(QString::number(0.00));

        // Set the parameters to the component
        theBeamColumn->setCrossSectionWidth(theCrossSectionWidth);
        theBeamColumn->setCrossSectionDepth(theCrossSectionDepth);
        theBeamColumn->setConcreteCover(theConcreteCover);
        theBeamColumn->setLongitudinalReinforcementRatio(theLongitudinalReinforcementRatio);
        theBeamColumn->setTransverseReinforcementRatio(theTransverseReinforcementRatio);
        theBeamColumn->setPoint1(pointOne);
        theBeamColumn->setPoint2(pointTwo);

        // Set the RObject to the item data
        inputItemData->theRComponent = theBeamColumn;
    }
    else if(theComponentID == "RRectangularGlulamBeamColumnComponent")
    {
        // Create the component in the domain
        theDomain->createObject(theObjectName,"RRectangularGlulamBeamColumnComponent");

        auto theBeamColumn = qobject_cast<RRectangularGlulamBeamColumnComponent*>(theDomain->getLastAddedObject());
        if(theBeamColumn == nullptr)
        {
            qDebug()<<"Failed to create component in RDomain in "<<__FUNCTION__;
            return;
        }

        // Create the cross-section width and depth parameters
        theDomain->createObject(theObjectName + "theCrossSectionWidth", "RConstant");
        auto theCrossSectionWidth =  qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        theCrossSectionWidth->setCurrentValue(0.30);

        theDomain->createObject(theObjectName + "theCrossSectionDepth", "RConstant");
        auto theCrossSectionDepth =  qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        theCrossSectionDepth->setCurrentValue(0.325);

        // Set the orientation
        theBeamColumn->setOrientation(QString::number(0.0));

        // Set the parameters to the component
        theBeamColumn->setCrossSectionWidth(theCrossSectionWidth);
        theBeamColumn->setCrossSectionDepth(theCrossSectionDepth);
        theBeamColumn->setPoint1(pointOne);
        theBeamColumn->setPoint2(pointTwo);

        // Set the RObject to the item data
        inputItemData->theRComponent = theBeamColumn;

    }
    else if(theComponentID == "RHSSRoundBeamColumnComponent")
    {
        // Create the component in the domain
        theDomain->createObject(theObjectName,"RHSSRoundBeamColumnComponent");

        auto theBeamColumn = qobject_cast<RHSSRoundBeamColumnComponent*>(theDomain->getLastAddedObject());
        if(theBeamColumn == nullptr)
        {
            qDebug()<<"Failed to create component in RDomain in "<<__FUNCTION__;
            return;
        }

        // Set the diameter and the wall thickess
        theBeamColumn->setDiameter(QString::number(0.1));
        theBeamColumn->setWallThickness(QString::number(0.05));

        // Set the parameters to the component
        theBeamColumn->setPoint1(pointOne);
        theBeamColumn->setPoint2(pointTwo);

        // Set the RObject to the item data
        inputItemData->theRComponent = theBeamColumn;
    }
    else
    {
        qCritical()<<"Error, the import of the component type "<<theComponentID<<" is not supported in "<<__FUNCTION__;
    }

    return;
}


void RSolidModelImportMethod::createSlabGeometryFromSolid(const std::vector<std::vector<vec2> >& profilePaths, const vec3 extrusionVector, IfcPPEntity* ifcEntity, shared_ptr<RIFCImportData>& inputItemData, const double& lengthFactor)
{
    if(profilePaths.size() == 0)
    {
        qCritical()<<" Failed to get profile paths in "<<__FUNCTION__;
        return;
    }

    if(profilePaths.size() >1)
        qDebug()<<"Slab cannot have more than one face loop in "<<__FUNCTION__;

    //Get the slab thickness
    const auto thickness = extrusionVector[2];

    std::vector<int> faceIndexes;
    std::vector<std::vector<vec2> > faceLoopsForTriangulation;
    geometrySweeper->triangulateLoops(profilePaths, faceLoopsForTriangulation, faceIndexes, ifcEntity);

    auto thePointsVec = profilePaths[0];

    if(thePointsVec.size() != 4)
    {
        qCritical()<<"Only slabs with 4 points are currently supported in "<<__FUNCTION__;
        return;
    }

    RMathUtils theUtils;

    // Arrange the pointsList so that they are useable via a convex hull
    theUtils.convexHullJarvis(thePointsVec);

    // Profile path loops and their indexes
    // 3----------------------------2
    // |                            |
    // |                            |
    // |                            | WIDTH
    // |                            |
    // |                            |
    // |                            |
    // 0-------face_loops[0]--------1
    //           DEPTH

    // Calculate the slab depth -- look at figure above = 1.x - 0.x
    const auto depth = std::fabs(thePointsVec[1].x - thePointsVec[0].x);

    // Calculate the slab width -- look at figure above  = 3.y - 0.y
    const auto width = std::fabs(thePointsVec[3].y - thePointsVec[0].y) ;

    if(thickness <= 0.0 || width <= 0.0 || depth <= 0.0)
    {
        qCritical()<<"Slab depth, width or thickness must be greater than zero  in "<<__FUNCTION__;
        return;
    }

    auto theIfcProduct = inputItemData->theIfcProduct.lock();

    // Get the label to name the object
    auto theLabel = theIfcProduct->m_Name.get();

    auto theObjectName = QString::fromStdWString(theLabel->m_value);

    // Append the unique id so that we know which ifc entity we created the component from
    auto theObjectId = theIfcProduct->m_entity_id;
    theObjectName.append("-ID_");
    theObjectName.append(QString::number(theObjectId));
    theObjectName.remove(' ');
    theObjectName.append("IfcSlab");

    // Set the object name to the input data item
    inputItemData->objectName = theObjectName.toStdWString();

    // Calculate the placement of this slab in the world -- IfcProduct has an ObjectPlacement that can be local or global
    // The product placement matrix is a 4x4 transformation matrix where the first three row/col are rotations,
    // With the 4th row/col being the homogeneous translation vector
    carve::math::Matrix productPlacementMatrix(carve::math::Matrix::IDENT());
    if(theIfcProduct->m_ObjectPlacement)
    {
        //  IfcPlacement2Matrix follows related placements in case of local coordinate systems
        std::unordered_set<IfcObjectPlacement*> placementAlreadyApplied;
        this->convertIfcObjectPlacement( theIfcProduct->m_ObjectPlacement, lengthFactor, productPlacementMatrix,placementAlreadyApplied);
    }
    else
    {
        qCritical()<<"Could not calculate the placement of the object in "<<__FUNCTION__;
        return;
    }

    // Slab corner points
    std::vector<double> point1 = {thePointsVec[0].x,thePointsVec[0].y,-0.5*extrusionVector[2]};
    std::vector<double> point2 = {thePointsVec[1].x,thePointsVec[1].y,-0.5*extrusionVector[2]};
    std::vector<double> point3 = {thePointsVec[2].x,thePointsVec[2].y,-0.5*extrusionVector[2]};
    std::vector<double> point4 = {thePointsVec[3].x,thePointsVec[3].y,-0.5*extrusionVector[2]};

    // Rotate and translate the points into global coordinates
    auto p1 = preMultiplyVectorMatrix(productPlacementMatrix,point1);
    auto p2 = preMultiplyVectorMatrix(productPlacementMatrix,point2);
    auto p3 = preMultiplyVectorMatrix(productPlacementMatrix,point3);
    auto p4 = preMultiplyVectorMatrix(productPlacementMatrix,point4);

    // Create point 1
    auto pointOne =  this->createAPoint(theObjectName+"point1",p1);
    if(pointOne == nullptr)
    {
        return;
    }

    // Create point 2
    auto pointTwo =  this->createAPoint(theObjectName+"point2",p2);
    if(pointTwo == nullptr)
    {
        return;
    }

    // Create point 3
    auto pointThree =  this->createAPoint(theObjectName+"point3",p3);
    if(pointThree == nullptr)
    {
        return;
    }

    // Create point 4
    auto pointFour =  this->createAPoint(theObjectName+"point4",p4);
    if(pointFour == nullptr)
    {
        return;
    }

    // Get the material type from the product
    // To do: Create different slab components based on material type from inputItemData
    auto theMaterial = inputItemData->theIfcMaterial.lock();

    // For now assume that it is an RC slab
    QString theComponentID = "RRCSlabComponent";

    if(theComponentID == "RRCSlabComponent")
    {
        //Get the material type from the product
        theDomain->createObject(theObjectName,"RRCSlabComponent");

        //Get the last added item from the domain
        auto theSlab = qobject_cast<RRCSlabComponent*>(theDomain->getLastAddedObject());
        if(theSlab == nullptr)
        {
            qCritical()<<"Slab was not created in RDomain";
            return;
        }

        theSlab->setPoint1(pointOne);
        theSlab->setPoint2(pointTwo);
        theSlab->setPoint3(pointThree);
        theSlab->setPoint4(pointFour);

        // Now set slab thickness
        RConstant *theConstant;
        theDomain->createObject(theObjectName + "Thickness", "RConstant");
        theConstant = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        theConstant->setCurrentValue(thickness);
        theSlab->setThickness(theConstant);

        // Reinforcement spacing
        theSlab->setLongitudinalReinforcementSpacing(0.2);
        theSlab->setTransverseReinforcementSpacing(0.2);

        // Set the RObject member
        inputItemData->theRComponent = theSlab;
    }
    if(theComponentID == "RCLTPanelComponent")
    {
        // Get the material type from the product
        theDomain->createObject(theObjectName,"RCLTPanelComponent");

        // Get the last added item from the domain
        auto theSlab = qobject_cast<RCLTPanelComponent*>(theDomain->getLastAddedObject());
        if(theSlab == nullptr)
        {
            qCritical()<<"Slab was not created in "<<__FUNCTION__;
            return;
        }

        theSlab->setPoint1(pointOne);
        theSlab->setPoint2(pointTwo);
        theSlab->setPoint3(pointThree);
        theSlab->setPoint4(pointFour);

        // For a 5-layer, E1 panel thickness is 12 inches
        auto cltThickness = 0.30;

        // Now set slab thickness
        RConstant *theConstant;
        theDomain->createObject(theObjectName + "Thickness", "RConstant");
        theConstant = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        theConstant->setCurrentValue(cltThickness);
        theSlab->setThickness(theConstant);

        // Set the RObject member
        inputItemData->theRComponent = theSlab;
    }
    if(theComponentID == "RCompositePanelComponent")
    {
        // Get the material type from the product
        theDomain->createObject(theObjectName,"RCompositePanelComponent");

        // Get the last added item from the domain
        auto theSlab = qobject_cast<RCompositePanelComponent*>(theDomain->getLastAddedObject());
        if(theSlab == nullptr)
        {
            qCritical()<<"Slab was not created in "<<__FUNCTION__;
            return;
        }

        theSlab->setPoint1(pointOne);
        theSlab->setPoint2(pointTwo);
        theSlab->setPoint3(pointThree);
        theSlab->setPoint4(pointFour);

        // The total thickness is the thickness of the concrete topping and the depth of the decking
        auto compPanelThickness = theSlab->getConcreteToppingThickness()+theSlab->getDeckingDepth();

        // Now set slab thickness
        RConstant *theConstant;
        theDomain->createObject(theObjectName + "Thickness", "RConstant");
        theConstant = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        theConstant->setCurrentValue(compPanelThickness);
        theSlab->setThickness(theConstant);

        // Set the RObject member
        inputItemData->theRComponent = theSlab;
    }

    return;
}


void RSolidModelImportMethod::createWallGeometryFromSolid(const std::vector<std::vector<vec2> >& profilePaths, const vec3 extrusionVector, IfcPPEntity* ifcEntity, shared_ptr<RIFCImportData>& inputItemData, const double& lengthFactor)
{
    if(profilePaths.size() == 0)
    {
        qCritical()<<" Failed to get profile paths in "<<__FUNCTION__;
        return;
    }

    if(profilePaths.size() >1)
        qCritical()<<"Wall cannot have more than one face loop in "<<__FUNCTION__;

    // Get the wall thickness
    const auto thickness = extrusionVector[2];

    std::vector<int> faceIndexes;
    std::vector<std::vector<vec2> > faceLoopsForTriangulation;
    geometrySweeper->triangulateLoops(profilePaths, faceLoopsForTriangulation, faceIndexes, ifcEntity);

    auto thePointsVec = profilePaths[0];

    if(thePointsVec.size() != 4)
    {
        qCritical()<<"Only walls with 4 points are supported in "<<__FUNCTION__;
        return;
    }

    RMathUtils theUtils;

    // Arrange the pointsList so that they are useable..
    theUtils.convexHullJarvis(thePointsVec);

    // Profile path loops and their indexes
    // 3----------------------------2
    // |                            |
    // |                            |
    // |                            | WIDTH
    // |                            |
    // |                            |
    // |                            |
    // 0-------face_loops[0]--------1
    //          DEPTH

    // Calculate the wall depth -- look at diagram above = 1.x - 0.x
    const auto depth = std::fabs(faceLoopsForTriangulation[0][1].x - faceLoopsForTriangulation[0][0].x);

    // Calculate the wall width -- look at diagram above = 3.y - 0.y
    const auto width = std::fabs(faceLoopsForTriangulation[0][3].y - faceLoopsForTriangulation[0][0].y);

    if(thickness <= 0.0 || width <= 0.0 || depth <= 0.0)
    {
        qCritical()<<"Depth, width or thickness must be a number greater than zero";
        return;
    }

    auto theIfcProduct = inputItemData->theIfcProduct.lock();

    // Get the label to name the object
    auto theLabel = theIfcProduct->m_Name.get();

    auto theObjectName = QString::fromStdWString(theLabel->m_value);

    // Append the unique id so that we know which ifc entity we created the component from
    auto theObjectId = theIfcProduct->m_entity_id;
    theObjectName.append("-ID_");
    theObjectName.append(QString::number(theObjectId));
    theObjectName.remove(' ');
    theObjectName.append("IfcWall");

    // Get the material type from the product
    auto theMaterial = inputItemData->theIfcMaterial.lock();

    // To do: Create different wall components based on material from inputItemData

    // If the material type is concrete and greater than 6 inches in thickness, has reinforcement, then assume it is a shear wall
    QString typeOfWall = "RRCShearWallComponent";

    theDomain->createObject(theObjectName,typeOfWall);

    // Get the last added item from the domain
    RWallComponent* theWall = nullptr;

    if(typeOfWall == "RRCShearWallComponent")
    {
        theWall = qobject_cast<RRCShearWallComponent*>(theDomain->getLastAddedObject());
    }
    else
    {
        qCritical()<<"Type of wall "<<typeOfWall<<" is not supported in  "<<__FUNCTION__;
        return;
    }

    if(theWall == nullptr)
    {
        qCritical()<<"Failed to create wall of type "<<typeOfWall<<" in "<<__FUNCTION__;
        return;
    }

    // Set the object name to the input data item
    inputItemData->objectName = theObjectName.toStdWString();

    // Calculate the placement of this object in the world -- IfcProduct has an ObjectPlacement that can be local or global
    // The product placement matrix is a 4x4 transformation matrix where the first three row/col are rotations,
    // With the 4th row/col being the homogeneous translation vector
    carve::math::Matrix productPlacementMatrix(carve::math::Matrix::IDENT());
    if(theIfcProduct->m_ObjectPlacement)
    {
        // IfcPlacement2Matrix follows related placements in case of local coordinate systems
        std::unordered_set<IfcObjectPlacement*> placementAlreadyApplied;
        this->convertIfcObjectPlacement(theIfcProduct->m_ObjectPlacement, lengthFactor, productPlacementMatrix, placementAlreadyApplied);
    }
    else
    {
        qCritical()<<"Could not calculate the placement of the object in "<<__FUNCTION__;
        return;
    }

    // Wall corner points
    std::vector<double> point1 = {thePointsVec[0].x,thePointsVec[0].y,-0.5*extrusionVector[2]};
    std::vector<double> point2 = {thePointsVec[1].x,thePointsVec[1].y,-0.5*extrusionVector[2]};
    std::vector<double> point3 = {thePointsVec[2].x,thePointsVec[2].y,-0.5*extrusionVector[2]};
    std::vector<double> point4 = {thePointsVec[3].x,thePointsVec[3].y,-0.5*extrusionVector[2]};

    // Rotate and translate the points into global coordinates
    auto p1 = preMultiplyVectorMatrix(productPlacementMatrix,point1);
    auto p2 = preMultiplyVectorMatrix(productPlacementMatrix,point2);
    auto p3 = preMultiplyVectorMatrix(productPlacementMatrix,point3);
    auto p4 = preMultiplyVectorMatrix(productPlacementMatrix,point4);

    // Create point 1
    auto pointOne = this->createAPoint(theObjectName+"point1",p1);
    if(pointOne == nullptr)
    {
        return;
    }

    // Create point 2
    auto pointTwo = this->createAPoint(theObjectName+"point2",p2);
    if(pointTwo == nullptr)
    {
        return;
    }

    // Create point 3
    auto pointThree = this->createAPoint(theObjectName+"point3",p3);
    if(pointThree == nullptr)
    {
        return;
    }

    // Create point 4
    auto pointFour = this->createAPoint(theObjectName+"point4",p4);
    if(pointFour == nullptr)
    {
        return;
    }

    theWall->setPoint1(pointOne);
    theWall->setPoint2(pointTwo);
    theWall->setPoint3(pointThree);
    theWall->setPoint4(pointFour);

    // Now set slab thickness
    RConstant *theConstant;
    theDomain->createObject(theObjectName + "Thickness", "RConstant");
    theConstant = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
    theConstant->setCurrentValue(thickness);
    theWall->setThickness(theConstant);

    // Set the RObject member
    inputItemData->theRComponent = theWall;

}


void RSolidModelImportMethod::createStdCaseWallGeometryFromSolid(const std::vector<std::vector<vec2> >& profilePaths, const vec3 extrusionVector, IfcPPEntity* ifcEntity, shared_ptr<RIFCImportData>& inputItemData, const double& lengthFactor )
{

    if( profilePaths.size() == 0 )
    {
        qCritical()<<" Failed to get profile paths in "<<__FUNCTION__;
        return;
    }

    if(profilePaths.size() >1)
        qCritical()<<"Wall cannot have more than one face loop in "<<__FUNCTION__;

    // Wall height is calculated  from the depth of extrusion, provided by the geometric representation.
    const auto height = extrusionVector[2];

    std::vector<int> faceIndexes;
    std::vector<std::vector<vec2> > faceLoopsForTriangulation;
    geometrySweeper->triangulateLoops(profilePaths, faceLoopsForTriangulation, faceIndexes, ifcEntity);

    auto thePointsVec = profilePaths[0];

    if(thePointsVec.size() != 4)
    {
        qCritical()<<"Only walls with 4 points are supported";
        return;
    }

    RMathUtils theUtils;

    // Arrange the pointsList so that they are useable..
    theUtils.convexHullJarvis(thePointsVec);

    // Profile path loops and their indexes
    //
    //  ^ y
    //  |
    //  3-------------------------------------------------2
    //  |                                                 |
    //  |                                                 | Thickness
    //  |                                                 |
    //  0----------------face_loops[0]--------------------1 -> x
    //                        Length
    // (height out of page along z axis)

    // Calculate the wall length -- look at diagram above  = 1.x - 0.x
    const auto width = std::fabs(thePointsVec[1].x - thePointsVec[0].x);

    // Calculate the wall thickness -- look at diagram above = 3.y - 0.y
    const auto thickness = std::fabs(thePointsVec[3].y - thePointsVec[0].y) ;

    if(height <= 0.0 || thickness <= 0.0 || width <= 0.0)
    {
        qDebug()<<"Slab geometry has a 0.0 value; depth, width or thickness must be a number greater than zero";
        return;
    }

    auto theIfcProduct = inputItemData->theIfcProduct.lock();

    // Get the label to name the object
    auto theLabel = theIfcProduct->m_Name.get();

    auto theObjectName = QString::fromStdWString(theLabel->m_value);

    // Append the unique id so that we know which ifc entity we created the component from
    auto theObjectId = theIfcProduct->m_entity_id;
    theObjectName.append("-ID_");
    theObjectName.append(QString::number(theObjectId));
    theObjectName.remove(' ');
    theObjectName.append("IfcWallStdCase");

    // Set the object name to the input data item
    inputItemData->objectName = theObjectName.toStdWString();

    // Calculate the placement of this object in the world -- IfcProduct has an ObjectPlacement that can be local or global
    // The product placement matrix is a 4x4 transformation matrix where the first three row/col are rotations,
    // With the 4th row/col being the homogeneous translation vector
    carve::math::Matrix productPlacementMatrix( carve::math::Matrix::IDENT() );
    if(theIfcProduct->m_ObjectPlacement)
    {
        // IfcPlacement2Matrix follows related placements in case of local coordinate systems
        std::unordered_set<IfcObjectPlacement*> placementAlreadyApplied;
        this->convertIfcObjectPlacement(theIfcProduct->m_ObjectPlacement, lengthFactor, productPlacementMatrix, placementAlreadyApplied);
    }
    else
    {
        qDebug()<<"Could not calculate the placement of the object";
        return;
    }

    //Find the midpoints of points 0,3 and 1,2
    vec2 midpoint1 = theUtils.getMidpointCarve2D(thePointsVec[0],thePointsVec[3]);
    vec2 midpoint2 = theUtils.getMidpointCarve2D(thePointsVec[1],thePointsVec[2]);

    // Wall corner points
    auto point1 = std::vector<double>{midpoint1.x,midpoint1.y,0.0};
    auto point2 = std::vector<double>{midpoint2.x,midpoint2.y,0.0};
    auto point3 = std::vector<double>{midpoint2.x,midpoint2.y,height};
    auto point4 = std::vector<double>{midpoint1.x,midpoint1.y,height};

    // Rotate and translate the points into global coordinates
    auto p1 = preMultiplyVectorMatrix(productPlacementMatrix,point1);
    auto p2 = preMultiplyVectorMatrix(productPlacementMatrix,point2);
    auto p3 = preMultiplyVectorMatrix(productPlacementMatrix,point3);
    auto p4 = preMultiplyVectorMatrix(productPlacementMatrix,point4);

    // Before creating anything, extract the material type and define the type of wall to be created
    QString typeOfWall = "null";

    // Get the material type from the product
    auto theMatType = inputItemData->theMaterialType;

    if(theMatType == RIFCImportData::materialType::concrete)
    {
        typeOfWall = "RRCShearWallComponent";
    }
    else if(theMatType == RIFCImportData::materialType::reinforcedConcrete)
    {
        typeOfWall = "RRCShearWallComponent";
    }
    else if(theMatType == RIFCImportData::materialType::wood)
    {
        typeOfWall = "RNonLoadBearingWallComponent";
    }
    else if(theMatType == RIFCImportData::materialType::NA)
    {
        qCritical()<<"The material type was not given for "<<theObjectName<<" in "<<__FUNCTION__;
        return;
    }
    else
    {
        qCritical()<<"The type of wall "<<theMatType<<"is not supported for "<<theObjectName;
        return;
    }

    // Create point 1
    auto pointOne = this->createAPoint(theObjectName+"point1",p1);
    if(pointOne == nullptr)
    {
        return;
    }

    // Create point 2
    auto pointTwo = this->createAPoint(theObjectName+"point2",p2);
    if(pointTwo == nullptr)
    {
        return;
    }

    // Create point 3
    auto pointThree = this->createAPoint(theObjectName+"point3",p3);
    if(pointThree == nullptr)
    {
        return;
    }

    // Create point 4
    auto pointFour = this->createAPoint(theObjectName+"point4",p4);
    if(pointFour == nullptr)
    {
        return;
    }

    RWallComponent* theWall = nullptr;

    if(typeOfWall == "RRCShearWallComponent")
    {
        theDomain->createObject(theObjectName,typeOfWall);
        theWall = qobject_cast<RRCShearWallComponent*>(theDomain->getLastAddedObject());
    }
    else if(typeOfWall == "RNonLoadBearingWallComponent")
    {
        theDomain->createObject(theObjectName,typeOfWall);
        theWall = qobject_cast<RNonLoadBearingWallComponent*>(theDomain->getLastAddedObject());
    }
    else
    {
        qDebug()<<"Type of wall "<<typeOfWall<<" is not supported in  "<<__FUNCTION__;
        return;
    }


    if(theWall == nullptr)
    {
        qCritical()<<"Failed to create wall of type "<<typeOfWall<<" in "<<__FUNCTION__;
        return;
    }

    // Remove the nodes from the points so that it does not automatically create an OpenSees node and mess up the analysis
    // Also, delete this code if want to actually run a finite element analysis on the non load bearing walls
    if(typeOfWall == "RNonLoadBearingWallComponent")
    {
        pointOne->setIsOn(false);
        pointTwo->setIsOn(false);
        pointThree->setIsOn(false);
        pointFour->setIsOn(false);
    }

    // Set the points to the wall
    theWall->setPoint1(pointOne);
    theWall->setPoint2(pointTwo);
    theWall->setPoint3(pointThree);
    theWall->setPoint4(pointFour);

    // Make wall thickness parameter
    RConstant *theConstant;
    theDomain->createObject(theObjectName + "Thickness", "RConstant");
    theConstant = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
    // qDebug()<<"Thickness "<<thickness;
    theConstant->setCurrentValue(thickness);
    theWall->setThickness(theConstant);

    // Set the RObject member to the input data
    inputItemData->theRComponent = theWall;
}


void RSolidModelImportMethod::createWindowGeometryFromSolid(const std::vector<std::vector<vec3> >& profilePaths, IfcPPEntity* ifcEntity, shared_ptr<RIFCImportData>& inputItemData, const double& lengthFactor)
{
    if(profilePaths.size() == 0)
    {
        qCritical()<<" Failed to get profile paths in "<<__FUNCTION__;
        return;
    }

    auto numFaceLoops = profilePaths.size();

    // Sort faceLoopVectors according to their area
    std::vector<std::pair<double, std::vector<vec3>>> pairt(numFaceLoops);

    for(size_t i = 0; i<numFaceLoops; ++i)
    {
        std::vector<vec3> faceLoop = profilePaths[i];

        auto theArea = mathOps.polygonArea(faceLoop);

        pairt[i].first = theArea;
        pairt[i].second = faceLoop;

    }

    // Sorting the pair array. This sorts it in ascending order - the last faceloops are the biggest
    std::sort(pairt.begin(), pairt.end());

    // The faceloops with the largest area are the "elevation" face or profile of the windows

    // Get the last loop, this should have the largest area
    auto profileFaceLoop = pairt[numFaceLoops-1].second;

    if(profileFaceLoop.size() != 4)
    {
        qCritical()<<"Only windows with 4 points are currently supported in "<<__FUNCTION__;
        return;
    }

    // Calculate the centroids of the last two faceloops and calculate the distance between them to get the window thickness
    auto backFaceLoopPrev = pairt[numFaceLoops-2].second;

    auto frontFace = mathOps.computePolygonCentroid(profileFaceLoop);
    auto backFace = mathOps.computePolygonCentroid(backFaceLoopPrev);

    auto thickness = mathOps.distanceBtwnPoints(frontFace,backFace);

    // Arrange the pointsList so that they are useable
    mathOps.convexHullJarvis(profileFaceLoop);

    //  Profile path loops and indexes
    //  3----------------------------2
    //  |                            |
    //  |                            |
    //  |                            |
    //  |                            |
    //  |                            |
    //  |                            |
    //  0-------face_loops[0]--------1

    // Calculate the window width - look at diagram above  = 1 - 0
    const auto width = mathOps.distanceBtwnPoints(profileFaceLoop[1],profileFaceLoop[0]);

    // Calculate the window height - look at diagram above = 3 - 0
    const auto height = mathOps.distanceBtwnPoints(profileFaceLoop[3],profileFaceLoop[0]);

    if(thickness <= 0.0 || width <= 0.0 || height <= 0.0)
    {
        qCritical()<<"Window height, width, or thickness must be a number greater than zero";
        return;
    }

    auto theIfcProduct = inputItemData->theIfcProduct.lock();

    // Get the label to name the object
    auto theLabel = theIfcProduct->m_Name.get();

    auto theObjectName = QString::fromStdWString(theLabel->m_value);

    // Append the unique id so that we know which ifc entity we created the component from
    auto theObjectId = theIfcProduct->m_entity_id;
    theObjectName.append("-ID_");
    theObjectName.append(QString::number(theObjectId));
    theObjectName.remove(' ');
    theObjectName.append("IfcWindow");

    RWindowComponent* theWindow = nullptr;

    // Create a window
    theDomain->createObject(theObjectName,"RWindowComponent");
    theWindow = qobject_cast<RWindowComponent*>(theDomain->getLastAddedObject());

    if(theWindow == nullptr)
    {
        qCritical()<<"Failed to create window component in "<<__FUNCTION__;
        return;
    }


    // Set the object name to the input data item
    inputItemData->objectName = theObjectName.toStdWString();

    // Calculate the placement of this object in the world -- IfcProduct has an ObjectPlacement that can be local or global
    // The product placement matrix is a 4x4 transformation matrix where the first three row/col are rotations,
    // With the 4th row/col being the homogeneous translation vector
    carve::math::Matrix productPlacementMatrix(carve::math::Matrix::IDENT());
    if(theIfcProduct->m_ObjectPlacement)
    {
        // IfcPlacement2Matrix follows related placements in case of local coordinate systems
        std::unordered_set<IfcObjectPlacement*> placementAlreadyApplied;
        this->convertIfcObjectPlacement(theIfcProduct->m_ObjectPlacement, lengthFactor, productPlacementMatrix, placementAlreadyApplied);
    }
    else
    {
        qCritical()<<"Could not calculate the placement of the object in "<<__FUNCTION__;
        return;
    }

    // Corner points of the window
    std::vector<double> point1 = {profileFaceLoop[0].x,profileFaceLoop[0].y,profileFaceLoop[0].z};
    std::vector<double> point2 = {profileFaceLoop[1].x,profileFaceLoop[1].y,profileFaceLoop[1].z};
    std::vector<double> point3 = {profileFaceLoop[2].x,profileFaceLoop[2].y,profileFaceLoop[2].z};
    std::vector<double> point4 = {profileFaceLoop[3].x,profileFaceLoop[3].y,profileFaceLoop[3].z};

    // Rotate and translate the points into global coordinates
    auto p1 = preMultiplyVectorMatrix(productPlacementMatrix,point1);
    auto p2 = preMultiplyVectorMatrix(productPlacementMatrix,point2);
    auto p3 = preMultiplyVectorMatrix(productPlacementMatrix,point3);
    auto p4 = preMultiplyVectorMatrix(productPlacementMatrix,point4);

    // Create point 1
    auto pointOne = this->createAPoint(theObjectName+"point1",p1);
    if(pointOne == nullptr)
    {
        return;
    }

    // Create point 2
    auto pointTwo = this->createAPoint(theObjectName+"point2",p2);
    if(pointTwo == nullptr)
    {
        return;
    }

    // Create point 3
    auto pointThree = this->createAPoint(theObjectName+"point3",p3);
    if(pointThree == nullptr)
    {
        return;
    }

    // Create point 4
    auto pointFour = this->createAPoint(theObjectName+"point4",p4);
    if(pointFour == nullptr)
    {
        return;
    }

    // Delete the nodes since OpenSees analysis will issue an error if extraneous nodes are floating around in space
    // Remove this if want to actually run a finite element analysis on the windows
    pointOne->setIsOn(false);
    pointTwo->setIsOn(false);
    pointThree->setIsOn(false);
    pointFour->setIsOn(false);

    theWindow->setPoint1(pointOne);
    theWindow->setPoint2(pointTwo);
    theWindow->setPoint3(pointThree);
    theWindow->setPoint4(pointFour);

    // Set window thickness
    RConstant *theConstant;
    theDomain->createObject(theObjectName + "Thickness", "RConstant");
    theConstant = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
    theConstant->setCurrentValue(thickness);
    theWindow->setThickness(theConstant);

    // Set the RObject member
    inputItemData->theRComponent = theWindow;

}


std::vector<double> RSolidModelImportMethod::preMultiplyVectorMatrix(const carve::math::Matrix & _mat, const std::vector<double>& v )
{
    double d = 1.0/(_mat.m[0][3]*v[0]+_mat.m[1][3]*v[1]+_mat.m[2][3]*v[2]+_mat.m[3][3]) ;

    return std::vector<double>  {  (_mat.m[0][0]*v[0] + _mat.m[1][0]*v[1] + _mat.m[2][0]*v[2] + _mat.m[3][0])*d,
                (_mat.m[0][1]*v[0] + _mat.m[1][1]*v[1] + _mat.m[2][1]*v[2] + _mat.m[3][1])*d,
                (_mat.m[0][2]*v[0] + _mat.m[1][2]*v[1] + _mat.m[2][2]*v[2] + _mat.m[3][2])*d  };
}


vec3 RSolidModelImportMethod::getCentroidFromFace(shared_ptr<IfcFace>& ifcFace)
{
    vec3 theFaceCentroid;
    theFaceCentroid.setZero();

    // Get the face bounds - there should only be a single face bound, i.e., no shell should be bound within another shell
    const std::vector<shared_ptr<IfcFaceBound> >& vecBounds = ifcFace->m_Bounds;
    if(vecBounds.size()>1)
    {
        qCritical()<<"Bound size too large in "<<__FUNCTION__;
        return theFaceCentroid;
    }

    // Get the loop vector from the IfcFaceBound
    // IfcLoop is a base class of one of IfcEdgeLoop, IfcPolyLoop, IfcVertexLoop
    const shared_ptr<IfcLoop>& loop = vecBounds.front()->m_Bound;
    if( !loop )
    {
        qCritical()<<"Failed to get loops from bound in "<<__FUNCTION__;
        return theFaceCentroid;
    }

    // Convert the loops to meters, it could be in other units
    std::vector<std::vector<vec3> > faceLoops;
    faceLoops.push_back(std::vector<vec3>());

    std::vector<vec3>& loopPoints = faceLoops.back();
    curveConverter->convertIfcLoop(loop,loopPoints);

    // Check orientation and reverse if no orientation specified
    bool orientation = true;
    if(vecBounds.front()->m_Orientation)
        orientation = vecBounds.front()->m_Orientation->m_value;

    if(!orientation)
        std::reverse(loopPoints.begin(), loopPoints.end());

    // Finally, compute the centroid of the face
    theFaceCentroid = mathOps.computePolygonCentroid(loopPoints);

    return theFaceCentroid;
}


vec3 RSolidModelImportMethod::getCentroidFromFaceVector(std::vector<vec3>& Facevector)
{
    vec3 theFaceCentroid;

    theFaceCentroid.setZero();

    theFaceCentroid = mathOps.computePolygonCentroid(Facevector);

    return theFaceCentroid;
}


int RSolidModelImportMethod::mergeCoplanarFaces(std::vector<std::vector<vec3> >& faceVector)
{    
    int numRemoved = 0;

    //double tol = 1e-30;

    std::list<std::vector<vec3>> theList;

    std::copy(faceVector.begin(), faceVector.end(), std::back_inserter( theList ) );

    // Function to check if the vertex already exists in the vector
    auto checkIfExists = [](const std::vector<vec3>& vecIn, const vec3& val )
    {
        auto it = vecIn.begin();
        while (it != vecIn.end())
        {
            if ((*it - val).isZero())
                return true;
            ++it;
        }

        return false;
    };

    // Function to return three points that are not colinear, three points are needed to form a plane
    auto findNotColinearPoint = [&](const vec3& p1, const vec3& p2, const std::vector<vec3>& pointVec )
    {
        auto vecSize = pointVec.size();

        if (vecSize < 3)
            qCritical()<<"Error need at least three points to check if colinear";


        // Check the first two are not coincident
        if ((p1 - p2).isZero())
            qCritical()<<"Error the same two points came into the findNotColinearPoint function in RSolidModelConvereter";

        // Skip the first one since we are using it
        auto it = pointVec.begin()+1;
        while (it != pointVec.end())
        {
            // Need to check if its a zero, a zero is ok, but will throw off the points Colinear calc and return a zero, try to find another point first
            if((*it).isZero())
            {
                ++it;
                continue;
            }

            if (mathOps.pointsColinear(p1,p2,(*it),0.0001) == false)
                return (*it);

            ++it;
        }

        qCritical()<<"All of the given points are colinear, returning empty vector in "<<__FUNCTION__;

        return vec3();
    };


    std::list<std::vector<vec3>>::iterator it = theList.begin();
    while (it != theList.end())
    {
        std::list<std::vector<vec3>>::iterator it2 = theList.begin();
        while (it2 != theList.end())
        {
            if((*it2) == (*it))
            {
                ++it2;
                continue;
            }

            bool coplanar = false;
            int sizeIt2 = it2->size();

            for(int i =0; i < sizeIt2-1; ++i)
            {
                // Get three points from the face that are NOT colinear to make a plane
                vec3 point1 = it->front();
                vec3 point2 = it->back();
                vec3 point3 = findNotColinearPoint(point1,point2,(*it));

                if(point3.isZero())
                    qCritical()<<"Error, zero vector returned";

                auto result = orient3dfast(point1,point2,point3,(*it2)[i]);

                // qDebug()<<std::abs(result);
                // if(std::abs(result) > tol)
                if(result != 0.0)
                {
                    coplanar = false;
                    break;
                }
                else
                    coplanar = true;
            }

            if(coplanar)
            {
                // Push back the points from the coplanarface onto the first face, then delete the second face
                for(auto&& it3 : (*it2))
                {
                    // Check to see if vertices are already in the face, if they are not then add
                    if(!checkIfExists(*it,it3))
                        it->push_back(it3);
                }

                it2 = theList.erase(it2);
                ++numRemoved;
            }
            else
            {
                ++it2;
            }
        }
        ++it;
    }

    faceVector.clear();

    std::copy(theList.begin(), theList.end(), std::back_inserter( faceVector ) );

    return numRemoved;
}


double RSolidModelImportMethod::orient3dfast(const vec3& pa, const vec3& pb, const vec3& pc, const vec3& pd)
{
    double adx, bdx, cdx;
    double ady, bdy, cdy;
    double adz, bdz, cdz;

    adx = pa[0] - pd[0];
    bdx = pb[0] - pd[0];
    cdx = pc[0] - pd[0];
    ady = pa[1] - pd[1];
    bdy = pb[1] - pd[1];
    cdy = pc[1] - pd[1];
    adz = pa[2] - pd[2];
    bdz = pb[2] - pd[2];
    cdz = pc[2] - pd[2];

    return adx * (bdy * cdz - bdz * cdy) + bdx * (cdy * adz - cdz * ady) + cdx * (ady * bdz - adz * bdy);
}


void RSolidModelImportMethod::plotPointForDebug(const vec3& point, shared_ptr<RIFCImportData>& inputItemData, const double& lengthFactor )
{
    auto theIfcProduct = inputItemData->theIfcProduct.lock();

    // Get the label to name the object
    auto theLabel = theIfcProduct->m_ObjectType.get();

    auto theObjectName = QString::fromStdWString(theLabel->m_value);

    // Append the unique id so that we know which ifc entity we created the component from
    auto theObjectId = theIfcProduct->m_entity_id;

    theObjectName.append("-ID_");
    theObjectName.append(QString::number(theObjectId));
    theObjectName.remove(' ');

    // Set the object name to the input data item
    inputItemData->objectName = theObjectName.toStdWString();

    // Calculate the placement of this object in the world -- IfcProduct has an ObjectPlacement that can be local or global
    // The product placement matrix is a 4x4 transformation matrix where the first three row/col are rotations,
    // With the 4th row/col being the homogeneous translation vector
    carve::math::Matrix productPlacementMatrix(carve::math::Matrix::IDENT());
    if(theIfcProduct->m_ObjectPlacement)
    {
        // IfcPlacement2Matrix follows related placements in case of local coordinate systems
        std::unordered_set<IfcObjectPlacement*> placementAlreadyApplied;
        this->convertIfcObjectPlacement(theIfcProduct->m_ObjectPlacement, lengthFactor, productPlacementMatrix,placementAlreadyApplied);
    }

    // Now that we can extract the product placement; make the points and set to beam
    std::vector<double> centroidVector1 = {point[0],point[1],point[2]};

    // Rotate and translate the centroid vector using the above transformation matrix
    auto theResult1 = preMultiplyVectorMatrix(productPlacementMatrix,centroidVector1);

    theDomain->createObject(theObjectName+"debugPoint","RPoint");

    // Get the last added item from the domain
    auto pointOne = qobject_cast<RPoint*>(theDomain->getLastAddedObject());
    if(pointOne == nullptr)
    {
        qCritical()<<"point could not be created in debug";
        return;
    }

    // Set the transformed coordinates
    pointOne->setXCoordinate(theResult1[0]);
    pointOne->setYCoordinate(theResult1[1]);
    pointOne->setZCoordinate(theResult1[2]);

}


void RSolidModelImportMethod::convertIfcObjectPlacement(const shared_ptr<IfcObjectPlacement>& ifcObjectPlacement,
                                                        const double lengthFactor,
                                                        carve::math::Matrix& resultingMatrix,
                                                        std::unordered_set<IfcObjectPlacement*>& placementAlreadyApplied,
                                                        bool onlyRotation)
{
    if(!ifcObjectPlacement )
    {
        return;
    }

    // Prevent cyclic relative placement
    IfcObjectPlacement* placement_ptr = ifcObjectPlacement.get();
    if( placementAlreadyApplied.find( placement_ptr ) != placementAlreadyApplied.end() )
    {
        return;
    }
    placementAlreadyApplied.insert( placement_ptr );

    resultingMatrix = carve::math::Matrix::IDENT();
    shared_ptr<IfcLocalPlacement> local_placement = dynamic_pointer_cast<IfcLocalPlacement>( ifcObjectPlacement );
    if( local_placement )
    {
        shared_ptr<IfcAxis2Placement> relative_axis2placement_select = local_placement->m_RelativePlacement;
        if(relative_axis2placement_select)
        {
            // IfcAxis2Placement = SELECT(IfcAxis2Placement2D,IfcAxis2Placement3D)
            shared_ptr<IfcPlacement> relative_placement = dynamic_pointer_cast<IfcPlacement>( relative_axis2placement_select );
            if( relative_placement )
            {
                shared_ptr<TransformData> primitive_placement_transform;
                placementConverter->convertIfcPlacement( relative_placement, primitive_placement_transform, onlyRotation );
                resultingMatrix = primitive_placement_transform->m_matrix;
            }
            else
            {
                qCritical()<<"unhandled placement in "<<__FUNCTION__;
            }
        }

        if(local_placement->m_PlacementRelTo)
        {
            // placement is relative to other placement
            shared_ptr<IfcObjectPlacement> rel_to_placement = local_placement->m_PlacementRelTo;
            carve::math::Matrix rel_to_placement_matrix( carve::math::Matrix::IDENT() );
            convertIfcObjectPlacement( rel_to_placement, lengthFactor, rel_to_placement_matrix, placementAlreadyApplied, onlyRotation );
            resultingMatrix = rel_to_placement_matrix*resultingMatrix;
        }
        else
        {
            // If the PlacementRelTo is not given, then the IfcProduct is placed absolutely within the world coordinate system
        }
    }
    else if(dynamic_pointer_cast<IfcGridPlacement>(ifcObjectPlacement))
    {
        qCritical()<<"IfcGridPlacement not implemented in "<<__FUNCTION__;
    }

    return;
}


RPoint* RSolidModelImportMethod::createAPoint(const QString& objectName, std::vector<double>& coords)
{
    // First check if any points exist at this location
    auto pointsList = theDomain->findChildren<RPoint*>();

    double tol = 1.0e-4;

    for(auto it: pointsList)
    {
        if (fabs(it->getXCoordinateValue() - coords[0])<tol && fabs(it->getYCoordinateValue() - coords[1])<tol && fabs(it->getZCoordinateValue() - coords[2])<tol)
            return it;
    }

    theDomain->createObject(objectName,"RPoint");

    // Get the last added item from the domain
    auto aPoint = qobject_cast<RPoint*>(theDomain->getLastAddedObject());

    if(aPoint)
    {
        aPoint->setCoordinateVector(coords);
    }
    else
    {
        qCritical()<<"Failed to create a point in "<<__FUNCTION__;
        return nullptr;
    }

    return aPoint;
}

