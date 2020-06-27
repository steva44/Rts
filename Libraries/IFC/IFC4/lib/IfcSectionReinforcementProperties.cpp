/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcExternalReferenceRelationship.h"
#include "IFC4/include/IfcLengthMeasure.h"
#include "IFC4/include/IfcReinforcementBarProperties.h"
#include "IFC4/include/IfcReinforcingBarRoleEnum.h"
#include "IFC4/include/IfcSectionProperties.h"
#include "IFC4/include/IfcSectionReinforcementProperties.h"

// ENTITY IfcSectionReinforcementProperties 
IfcSectionReinforcementProperties::IfcSectionReinforcementProperties() {}
IfcSectionReinforcementProperties::IfcSectionReinforcementProperties( int id ) { m_entity_id = id; }
IfcSectionReinforcementProperties::~IfcSectionReinforcementProperties() {}
shared_ptr<IfcPPObject> IfcSectionReinforcementProperties::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSectionReinforcementProperties> copy_self( new IfcSectionReinforcementProperties() );
	if( m_LongitudinalStartPosition ) { copy_self->m_LongitudinalStartPosition = dynamic_pointer_cast<IfcLengthMeasure>( m_LongitudinalStartPosition->getDeepCopy(options) ); }
	if( m_LongitudinalEndPosition ) { copy_self->m_LongitudinalEndPosition = dynamic_pointer_cast<IfcLengthMeasure>( m_LongitudinalEndPosition->getDeepCopy(options) ); }
	if( m_TransversePosition ) { copy_self->m_TransversePosition = dynamic_pointer_cast<IfcLengthMeasure>( m_TransversePosition->getDeepCopy(options) ); }
	if( m_ReinforcementRole ) { copy_self->m_ReinforcementRole = dynamic_pointer_cast<IfcReinforcingBarRoleEnum>( m_ReinforcementRole->getDeepCopy(options) ); }
	if( m_SectionDefinition ) { copy_self->m_SectionDefinition = dynamic_pointer_cast<IfcSectionProperties>( m_SectionDefinition->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_CrossSectionReinforcementDefinitions.size(); ++ii )
	{
		auto item_ii = m_CrossSectionReinforcementDefinitions[ii];
		if( item_ii )
		{
			copy_self->m_CrossSectionReinforcementDefinitions.push_back( dynamic_pointer_cast<IfcReinforcementBarProperties>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcSectionReinforcementProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCSECTIONREINFORCEMENTPROPERTIES" << "(";
	if( m_LongitudinalStartPosition ) { m_LongitudinalStartPosition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalEndPosition ) { m_LongitudinalEndPosition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransversePosition ) { m_TransversePosition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ReinforcementRole ) { m_ReinforcementRole->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SectionDefinition ) { stream << "#" << m_SectionDefinition->m_entity_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_CrossSectionReinforcementDefinitions );
	stream << ");";
}
void IfcSectionReinforcementProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcSectionReinforcementProperties::toString() const { return L"IfcSectionReinforcementProperties"; }
void IfcSectionReinforcementProperties::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 6 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSectionReinforcementProperties, expecting 6, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_LongitudinalStartPosition = IfcLengthMeasure::createObjectFromSTEP( args[0], map );
	m_LongitudinalEndPosition = IfcLengthMeasure::createObjectFromSTEP( args[1], map );
	m_TransversePosition = IfcLengthMeasure::createObjectFromSTEP( args[2], map );
	m_ReinforcementRole = IfcReinforcingBarRoleEnum::createObjectFromSTEP( args[3], map );
	readEntityReference( args[4], m_SectionDefinition, map );
	readEntityReferenceList( args[5], m_CrossSectionReinforcementDefinitions, map );
}
void IfcSectionReinforcementProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedProperties::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LongitudinalStartPosition", m_LongitudinalStartPosition ) );
	vec_attributes.push_back( std::make_pair( "LongitudinalEndPosition", m_LongitudinalEndPosition ) );
	vec_attributes.push_back( std::make_pair( "TransversePosition", m_TransversePosition ) );
	vec_attributes.push_back( std::make_pair( "ReinforcementRole", m_ReinforcementRole ) );
	vec_attributes.push_back( std::make_pair( "SectionDefinition", m_SectionDefinition ) );
	if( m_CrossSectionReinforcementDefinitions.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> CrossSectionReinforcementDefinitions_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_CrossSectionReinforcementDefinitions.begin(), m_CrossSectionReinforcementDefinitions.end(), std::back_inserter( CrossSectionReinforcementDefinitions_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "CrossSectionReinforcementDefinitions", CrossSectionReinforcementDefinitions_vec_object ) );
	}
}
void IfcSectionReinforcementProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPreDefinedProperties::getAttributesInverse( vec_attributes_inverse );
}
void IfcSectionReinforcementProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedProperties::setInverseCounterparts( ptr_self_entity );
}
void IfcSectionReinforcementProperties::unlinkFromInverseCounterparts()
{
	IfcPreDefinedProperties::unlinkFromInverseCounterparts();
}
