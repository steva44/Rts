/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcLabel.h"
#include "IFC4/include/IfcPresentationLayerAssignment.h"
#include "IFC4/include/IfcProductRepresentation.h"
#include "IFC4/include/IfcRepresentationContext.h"
#include "IFC4/include/IfcRepresentationItem.h"
#include "IFC4/include/IfcRepresentationMap.h"
#include "IFC4/include/IfcShapeAspect.h"
#include "IFC4/include/IfcTopologyRepresentation.h"

// ENTITY IfcTopologyRepresentation 
IfcTopologyRepresentation::IfcTopologyRepresentation() {}
IfcTopologyRepresentation::IfcTopologyRepresentation( int id ) { m_entity_id = id; }
IfcTopologyRepresentation::~IfcTopologyRepresentation() {}
shared_ptr<IfcPPObject> IfcTopologyRepresentation::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTopologyRepresentation> copy_self( new IfcTopologyRepresentation() );
	if( m_ContextOfItems )
	{
		if( options.shallow_copy_IfcRepresentationContext ) { copy_self->m_ContextOfItems = m_ContextOfItems; }
		else { copy_self->m_ContextOfItems = dynamic_pointer_cast<IfcRepresentationContext>( m_ContextOfItems->getDeepCopy(options) ); }
	}
	if( m_RepresentationIdentifier ) { copy_self->m_RepresentationIdentifier = dynamic_pointer_cast<IfcLabel>( m_RepresentationIdentifier->getDeepCopy(options) ); }
	if( m_RepresentationType ) { copy_self->m_RepresentationType = dynamic_pointer_cast<IfcLabel>( m_RepresentationType->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Items.size(); ++ii )
	{
		auto item_ii = m_Items[ii];
		if( item_ii )
		{
			copy_self->m_Items.push_back( dynamic_pointer_cast<IfcRepresentationItem>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcTopologyRepresentation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCTOPOLOGYREPRESENTATION" << "(";
	if( m_ContextOfItems ) { stream << "#" << m_ContextOfItems->m_entity_id; } else { stream << "*"; }
	stream << ",";
	if( m_RepresentationIdentifier ) { m_RepresentationIdentifier->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RepresentationType ) { m_RepresentationType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_Items );
	stream << ");";
}
void IfcTopologyRepresentation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcTopologyRepresentation::toString() const { return L"IfcTopologyRepresentation"; }
void IfcTopologyRepresentation::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTopologyRepresentation, expecting 4, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_ContextOfItems, map );
	m_RepresentationIdentifier = IfcLabel::createObjectFromSTEP( args[1], map );
	m_RepresentationType = IfcLabel::createObjectFromSTEP( args[2], map );
	readEntityReferenceList( args[3], m_Items, map );
}
void IfcTopologyRepresentation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcShapeModel::getAttributes( vec_attributes );
}
void IfcTopologyRepresentation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcShapeModel::getAttributesInverse( vec_attributes_inverse );
}
void IfcTopologyRepresentation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcShapeModel::setInverseCounterparts( ptr_self_entity );
}
void IfcTopologyRepresentation::unlinkFromInverseCounterparts()
{
	IfcShapeModel::unlinkFromInverseCounterparts();
}
