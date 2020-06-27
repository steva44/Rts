/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcGridPlacement.h"
#include "IFC4/include/IfcGridPlacementDirectionSelect.h"
#include "IFC4/include/IfcLocalPlacement.h"
#include "IFC4/include/IfcProduct.h"
#include "IFC4/include/IfcVirtualGridIntersection.h"

// ENTITY IfcGridPlacement 
IfcGridPlacement::IfcGridPlacement() {}
IfcGridPlacement::IfcGridPlacement( int id ) { m_entity_id = id; }
IfcGridPlacement::~IfcGridPlacement() {}
shared_ptr<IfcPPObject> IfcGridPlacement::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcGridPlacement> copy_self( new IfcGridPlacement() );
	if( m_PlacementLocation ) { copy_self->m_PlacementLocation = dynamic_pointer_cast<IfcVirtualGridIntersection>( m_PlacementLocation->getDeepCopy(options) ); }
	if( m_PlacementRefDirection ) { copy_self->m_PlacementRefDirection = dynamic_pointer_cast<IfcGridPlacementDirectionSelect>( m_PlacementRefDirection->getDeepCopy(options) ); }
	return copy_self;
}
void IfcGridPlacement::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCGRIDPLACEMENT" << "(";
	if( m_PlacementLocation ) { stream << "#" << m_PlacementLocation->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_PlacementRefDirection ) { m_PlacementRefDirection->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcGridPlacement::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcGridPlacement::toString() const { return L"IfcGridPlacement"; }
void IfcGridPlacement::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcGridPlacement, expecting 2, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_PlacementLocation, map );
	m_PlacementRefDirection = IfcGridPlacementDirectionSelect::createObjectFromSTEP( args[1], map );
}
void IfcGridPlacement::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObjectPlacement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PlacementLocation", m_PlacementLocation ) );
	vec_attributes.push_back( std::make_pair( "PlacementRefDirection", m_PlacementRefDirection ) );
}
void IfcGridPlacement::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcObjectPlacement::getAttributesInverse( vec_attributes_inverse );
}
void IfcGridPlacement::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObjectPlacement::setInverseCounterparts( ptr_self_entity );
}
void IfcGridPlacement::unlinkFromInverseCounterparts()
{
	IfcObjectPlacement::unlinkFromInverseCounterparts();
}
