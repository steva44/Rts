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
#include "IFC4/include/IfcRelAssignsToProcess.h"
#include "IFC4/include/IfcRelAssociates.h"
#include "IFC4/include/IfcRelDeclares.h"
#include "IFC4/include/IfcRelDefinesByType.h"
#include "IFC4/include/IfcRelNests.h"
#include "IFC4/include/IfcText.h"
#include "IFC4/include/IfcTypeProcess.h"

// ENTITY IfcTypeProcess 
IfcTypeProcess::IfcTypeProcess() {}
IfcTypeProcess::IfcTypeProcess( int id ) { m_entity_id = id; }
IfcTypeProcess::~IfcTypeProcess() {}
shared_ptr<IfcPPObject> IfcTypeProcess::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTypeProcess> copy_self( new IfcTypeProcess() );
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
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy(options) ); }
	if( m_LongDescription ) { copy_self->m_LongDescription = dynamic_pointer_cast<IfcText>( m_LongDescription->getDeepCopy(options) ); }
	if( m_ProcessType ) { copy_self->m_ProcessType = dynamic_pointer_cast<IfcLabel>( m_ProcessType->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTypeProcess::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCTYPEPROCESS" << "(";
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
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongDescription ) { m_LongDescription->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProcessType ) { m_ProcessType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTypeProcess::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcTypeProcess::toString() const { return L"IfcTypeProcess"; }
void IfcTypeProcess::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 9 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTypeProcess, expecting 9, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0], map );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2], map );
	m_Description = IfcText::createObjectFromSTEP( args[3], map );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromSTEP( args[4], map );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
	m_Identification = IfcIdentifier::createObjectFromSTEP( args[6], map );
	m_LongDescription = IfcText::createObjectFromSTEP( args[7], map );
	m_ProcessType = IfcLabel::createObjectFromSTEP( args[8], map );
}
void IfcTypeProcess::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTypeObject::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Identification", m_Identification ) );
	vec_attributes.push_back( std::make_pair( "LongDescription", m_LongDescription ) );
	vec_attributes.push_back( std::make_pair( "ProcessType", m_ProcessType ) );
}
void IfcTypeProcess::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTypeObject::getAttributesInverse( vec_attributes_inverse );
	if( m_OperatesOn_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> OperatesOn_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_OperatesOn_inverse.size(); ++i )
		{
			if( !m_OperatesOn_inverse[i].expired() )
			{
				OperatesOn_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssignsToProcess>( m_OperatesOn_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "OperatesOn_inverse", OperatesOn_inverse_vec_obj ) );
	}
}
void IfcTypeProcess::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTypeObject::setInverseCounterparts( ptr_self_entity );
}
void IfcTypeProcess::unlinkFromInverseCounterparts()
{
	IfcTypeObject::unlinkFromInverseCounterparts();
}
