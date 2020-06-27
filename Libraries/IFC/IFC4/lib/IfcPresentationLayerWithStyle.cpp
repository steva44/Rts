/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcIdentifier.h"
#include "IFC4/include/IfcLabel.h"
#include "IFC4/include/IfcLayeredItem.h"
#include "IFC4/include/IfcLogical.h"
#include "IFC4/include/IfcPresentationLayerWithStyle.h"
#include "IFC4/include/IfcPresentationStyle.h"
#include "IFC4/include/IfcText.h"

// ENTITY IfcPresentationLayerWithStyle 
IfcPresentationLayerWithStyle::IfcPresentationLayerWithStyle() {}
IfcPresentationLayerWithStyle::IfcPresentationLayerWithStyle( int id ) { m_entity_id = id; }
IfcPresentationLayerWithStyle::~IfcPresentationLayerWithStyle() {}
shared_ptr<IfcPPObject> IfcPresentationLayerWithStyle::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPresentationLayerWithStyle> copy_self( new IfcPresentationLayerWithStyle() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_AssignedItems.size(); ++ii )
	{
		auto item_ii = m_AssignedItems[ii];
		if( item_ii )
		{
			copy_self->m_AssignedItems.push_back( dynamic_pointer_cast<IfcLayeredItem>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Identifier ) { copy_self->m_Identifier = dynamic_pointer_cast<IfcIdentifier>( m_Identifier->getDeepCopy(options) ); }
	if( m_LayerOn ) { copy_self->m_LayerOn = dynamic_pointer_cast<IfcLogical>( m_LayerOn->getDeepCopy(options) ); }
	if( m_LayerFrozen ) { copy_self->m_LayerFrozen = dynamic_pointer_cast<IfcLogical>( m_LayerFrozen->getDeepCopy(options) ); }
	if( m_LayerBlocked ) { copy_self->m_LayerBlocked = dynamic_pointer_cast<IfcLogical>( m_LayerBlocked->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_LayerStyles.size(); ++ii )
	{
		auto item_ii = m_LayerStyles[ii];
		if( item_ii )
		{
			copy_self->m_LayerStyles.push_back( dynamic_pointer_cast<IfcPresentationStyle>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcPresentationLayerWithStyle::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCPRESENTATIONLAYERWITHSTYLE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_AssignedItems.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcLayeredItem>& type_object = m_AssignedItems[ii];
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
	stream << ",";
	if( m_Identifier ) { m_Identifier->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_LayerOn ) { m_LayerOn->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LayerFrozen ) { m_LayerFrozen->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LayerBlocked ) { m_LayerBlocked->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_LayerStyles );
	stream << ");";
}
void IfcPresentationLayerWithStyle::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcPresentationLayerWithStyle::toString() const { return L"IfcPresentationLayerWithStyle"; }
void IfcPresentationLayerWithStyle::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 8 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPresentationLayerWithStyle, expecting 8, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
	m_Description = IfcText::createObjectFromSTEP( args[1], map );
	readSelectList( args[2], m_AssignedItems, map );
	m_Identifier = IfcIdentifier::createObjectFromSTEP( args[3], map );
	m_LayerOn = IfcLogical::createObjectFromSTEP( args[4], map );
	m_LayerFrozen = IfcLogical::createObjectFromSTEP( args[5], map );
	m_LayerBlocked = IfcLogical::createObjectFromSTEP( args[6], map );
	readEntityReferenceList( args[7], m_LayerStyles, map );
}
void IfcPresentationLayerWithStyle::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationLayerAssignment::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LayerOn", m_LayerOn ) );
	vec_attributes.push_back( std::make_pair( "LayerFrozen", m_LayerFrozen ) );
	vec_attributes.push_back( std::make_pair( "LayerBlocked", m_LayerBlocked ) );
	if( m_LayerStyles.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> LayerStyles_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_LayerStyles.begin(), m_LayerStyles.end(), std::back_inserter( LayerStyles_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "LayerStyles", LayerStyles_vec_object ) );
	}
}
void IfcPresentationLayerWithStyle::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationLayerAssignment::getAttributesInverse( vec_attributes_inverse );
}
void IfcPresentationLayerWithStyle::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationLayerAssignment::setInverseCounterparts( ptr_self_entity );
}
void IfcPresentationLayerWithStyle::unlinkFromInverseCounterparts()
{
	IfcPresentationLayerAssignment::unlinkFromInverseCounterparts();
}
