/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcGloballyUniqueId.h"
#include "IFC4/include/IfcIdentifier.h"
#include "IFC4/include/IfcLabel.h"
#include "IFC4/include/IfcOwnerHistory.h"
#include "IFC4/include/IfcPropertySetDefinition.h"
#include "IFC4/include/IfcRelAggregates.h"
#include "IFC4/include/IfcRelAssigns.h"
#include "IFC4/include/IfcRelAssignsToProduct.h"
#include "IFC4/include/IfcRelAssociates.h"
#include "IFC4/include/IfcRelDeclares.h"
#include "IFC4/include/IfcRelDefinesByType.h"
#include "IFC4/include/IfcRelNests.h"
#include "IFC4/include/IfcRepresentationMap.h"
#include "IFC4/include/IfcTankType.h"
#include "IFC4/include/IfcTankTypeEnum.h"
#include "IFC4/include/IfcText.h"

// ENTITY IfcTankType 
IfcTankType::IfcTankType() {}
IfcTankType::IfcTankType( int id ) { m_entity_id = id; }
IfcTankType::~IfcTankType() {}
shared_ptr<IfcPPObject> IfcTankType::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTankType> copy_self( new IfcTankType() );
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
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcTankTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTankType::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCTANKTYPE" << "(";
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
	stream << ");";
}
void IfcTankType::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcTankType::toString() const { return L"IfcTankType"; }
void IfcTankType::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 10 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTankType, expecting 10, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0], map );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2], map );
	m_Description = IfcText::createObjectFromSTEP( args[3], map );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromSTEP( args[4], map );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
	readEntityReferenceList( args[6], m_RepresentationMaps, map );
	m_Tag = IfcLabel::createObjectFromSTEP( args[7], map );
	m_ElementType = IfcLabel::createObjectFromSTEP( args[8], map );
	m_PredefinedType = IfcTankTypeEnum::createObjectFromSTEP( args[9], map );
}
void IfcTankType::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcFlowStorageDeviceType::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcTankType::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcFlowStorageDeviceType::getAttributesInverse( vec_attributes_inverse );
}
void IfcTankType::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcFlowStorageDeviceType::setInverseCounterparts( ptr_self_entity );
}
void IfcTankType::unlinkFromInverseCounterparts()
{
	IfcFlowStorageDeviceType::unlinkFromInverseCounterparts();
}
