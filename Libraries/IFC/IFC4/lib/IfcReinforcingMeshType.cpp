/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcAreaMeasure.h"
#include "IFC4/include/IfcBendingParameterSelect.h"
#include "IFC4/include/IfcGloballyUniqueId.h"
#include "IFC4/include/IfcIdentifier.h"
#include "IFC4/include/IfcLabel.h"
#include "IFC4/include/IfcOwnerHistory.h"
#include "IFC4/include/IfcPositiveLengthMeasure.h"
#include "IFC4/include/IfcPropertySetDefinition.h"
#include "IFC4/include/IfcReinforcingMeshType.h"
#include "IFC4/include/IfcReinforcingMeshTypeEnum.h"
#include "IFC4/include/IfcRelAggregates.h"
#include "IFC4/include/IfcRelAssigns.h"
#include "IFC4/include/IfcRelAssignsToProduct.h"
#include "IFC4/include/IfcRelAssociates.h"
#include "IFC4/include/IfcRelDeclares.h"
#include "IFC4/include/IfcRelDefinesByType.h"
#include "IFC4/include/IfcRelNests.h"
#include "IFC4/include/IfcRepresentationMap.h"
#include "IFC4/include/IfcText.h"

// ENTITY IfcReinforcingMeshType 
IfcReinforcingMeshType::IfcReinforcingMeshType() {}
IfcReinforcingMeshType::IfcReinforcingMeshType( int id ) { m_entity_id = id; }
IfcReinforcingMeshType::~IfcReinforcingMeshType() {}
shared_ptr<IfcPPObject> IfcReinforcingMeshType::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcReinforcingMeshType> copy_self( new IfcReinforcingMeshType() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( createGUID32_wstr().c_str() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_ApplicableOccurrence ) { copy_self->m_ApplicableOccurrence = dynamic_pointer_cast<IfcIdentifier>( m_ApplicableOccurrence->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_HasPropertySets.size(); ++ii )
	{
		auto item_ii = m_HasPropertySets[ii];
		if( item_ii )
		{
			copy_self->m_HasPropertySets.push_back( dynamic_pointer_cast<IfcPropertySetDefinition>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_RepresentationMaps.size(); ++ii )
	{
		auto item_ii = m_RepresentationMaps[ii];
		if( item_ii )
		{
			copy_self->m_RepresentationMaps.push_back( dynamic_pointer_cast<IfcRepresentationMap>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Tag ) { copy_self->m_Tag = dynamic_pointer_cast<IfcLabel>( m_Tag->getDeepCopy(options) ); }
	if( m_ElementType ) { copy_self->m_ElementType = dynamic_pointer_cast<IfcLabel>( m_ElementType->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcReinforcingMeshTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	if( m_MeshLength ) { copy_self->m_MeshLength = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_MeshLength->getDeepCopy(options) ); }
	if( m_MeshWidth ) { copy_self->m_MeshWidth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_MeshWidth->getDeepCopy(options) ); }
	if( m_LongitudinalBarNominalDiameter ) { copy_self->m_LongitudinalBarNominalDiameter = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_LongitudinalBarNominalDiameter->getDeepCopy(options) ); }
	if( m_TransverseBarNominalDiameter ) { copy_self->m_TransverseBarNominalDiameter = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_TransverseBarNominalDiameter->getDeepCopy(options) ); }
	if( m_LongitudinalBarCrossSectionArea ) { copy_self->m_LongitudinalBarCrossSectionArea = dynamic_pointer_cast<IfcAreaMeasure>( m_LongitudinalBarCrossSectionArea->getDeepCopy(options) ); }
	if( m_TransverseBarCrossSectionArea ) { copy_self->m_TransverseBarCrossSectionArea = dynamic_pointer_cast<IfcAreaMeasure>( m_TransverseBarCrossSectionArea->getDeepCopy(options) ); }
	if( m_LongitudinalBarSpacing ) { copy_self->m_LongitudinalBarSpacing = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_LongitudinalBarSpacing->getDeepCopy(options) ); }
	if( m_TransverseBarSpacing ) { copy_self->m_TransverseBarSpacing = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_TransverseBarSpacing->getDeepCopy(options) ); }
	if( m_BendingShapeCode ) { copy_self->m_BendingShapeCode = dynamic_pointer_cast<IfcLabel>( m_BendingShapeCode->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_BendingParameters.size(); ++ii )
	{
		auto item_ii = m_BendingParameters[ii];
		if( item_ii )
		{
			copy_self->m_BendingParameters.push_back( dynamic_pointer_cast<IfcBendingParameterSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcReinforcingMeshType::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCREINFORCINGMESHTYPE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_entity_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ApplicableOccurrence ) { m_ApplicableOccurrence->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertySets );
	stream << ",";
	writeEntityList( stream, m_RepresentationMaps );
	stream << ",";
	if( m_Tag ) { m_Tag->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ElementType ) { m_ElementType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MeshLength ) { m_MeshLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MeshWidth ) { m_MeshWidth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalBarNominalDiameter ) { m_LongitudinalBarNominalDiameter->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransverseBarNominalDiameter ) { m_TransverseBarNominalDiameter->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalBarCrossSectionArea ) { m_LongitudinalBarCrossSectionArea->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransverseBarCrossSectionArea ) { m_TransverseBarCrossSectionArea->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalBarSpacing ) { m_LongitudinalBarSpacing->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransverseBarSpacing ) { m_TransverseBarSpacing->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BendingShapeCode ) { m_BendingShapeCode->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_BendingParameters.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcBendingParameterSelect>& type_object = m_BendingParameters[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ");";
}
void IfcReinforcingMeshType::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcReinforcingMeshType::toString() const { return L"IfcReinforcingMeshType"; }
void IfcReinforcingMeshType::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 20 ){ std::stringstream err; err << "Wrong parameter count for entity IfcReinforcingMeshType, expecting 20, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0], map );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2], map );
	m_Description = IfcText::createObjectFromSTEP( args[3], map );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromSTEP( args[4], map );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
	readEntityReferenceList( args[6], m_RepresentationMaps, map );
	m_Tag = IfcLabel::createObjectFromSTEP( args[7], map );
	m_ElementType = IfcLabel::createObjectFromSTEP( args[8], map );
	m_PredefinedType = IfcReinforcingMeshTypeEnum::createObjectFromSTEP( args[9], map );
	m_MeshLength = IfcPositiveLengthMeasure::createObjectFromSTEP( args[10], map );
	m_MeshWidth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[11], map );
	m_LongitudinalBarNominalDiameter = IfcPositiveLengthMeasure::createObjectFromSTEP( args[12], map );
	m_TransverseBarNominalDiameter = IfcPositiveLengthMeasure::createObjectFromSTEP( args[13], map );
	m_LongitudinalBarCrossSectionArea = IfcAreaMeasure::createObjectFromSTEP( args[14], map );
	m_TransverseBarCrossSectionArea = IfcAreaMeasure::createObjectFromSTEP( args[15], map );
	m_LongitudinalBarSpacing = IfcPositiveLengthMeasure::createObjectFromSTEP( args[16], map );
	m_TransverseBarSpacing = IfcPositiveLengthMeasure::createObjectFromSTEP( args[17], map );
	m_BendingShapeCode = IfcLabel::createObjectFromSTEP( args[18], map );
	readSelectList( args[19], m_BendingParameters, map );
}
void IfcReinforcingMeshType::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcReinforcingElementType::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "MeshLength", m_MeshLength ) );
	vec_attributes.push_back( std::make_pair( "MeshWidth", m_MeshWidth ) );
	vec_attributes.push_back( std::make_pair( "LongitudinalBarNominalDiameter", m_LongitudinalBarNominalDiameter ) );
	vec_attributes.push_back( std::make_pair( "TransverseBarNominalDiameter", m_TransverseBarNominalDiameter ) );
	vec_attributes.push_back( std::make_pair( "LongitudinalBarCrossSectionArea", m_LongitudinalBarCrossSectionArea ) );
	vec_attributes.push_back( std::make_pair( "TransverseBarCrossSectionArea", m_TransverseBarCrossSectionArea ) );
	vec_attributes.push_back( std::make_pair( "LongitudinalBarSpacing", m_LongitudinalBarSpacing ) );
	vec_attributes.push_back( std::make_pair( "TransverseBarSpacing", m_TransverseBarSpacing ) );
	vec_attributes.push_back( std::make_pair( "BendingShapeCode", m_BendingShapeCode ) );
	if( m_BendingParameters.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> BendingParameters_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_BendingParameters.begin(), m_BendingParameters.end(), std::back_inserter( BendingParameters_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "BendingParameters", BendingParameters_vec_object ) );
	}
}
void IfcReinforcingMeshType::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcReinforcingElementType::getAttributesInverse( vec_attributes_inverse );
}
void IfcReinforcingMeshType::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcReinforcingElementType::setInverseCounterparts( ptr_self_entity );
}
void IfcReinforcingMeshType::unlinkFromInverseCounterparts()
{
	IfcReinforcingElementType::unlinkFromInverseCounterparts();
}
