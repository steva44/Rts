/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcPoint.h"
#include "IFC4/include/IfcPresentationLayerAssignment.h"
#include "IFC4/include/IfcStyledItem.h"
#include "IFC4/include/IfcVertexPoint.h"

// ENTITY IfcVertexPoint 
IfcVertexPoint::IfcVertexPoint() {}
IfcVertexPoint::IfcVertexPoint( int id ) { m_entity_id = id; }
IfcVertexPoint::~IfcVertexPoint() {}
shared_ptr<IfcPPObject> IfcVertexPoint::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcVertexPoint> copy_self( new IfcVertexPoint() );
	if( m_VertexGeometry ) { copy_self->m_VertexGeometry = dynamic_pointer_cast<IfcPoint>( m_VertexGeometry->getDeepCopy(options) ); }
	return copy_self;
}
void IfcVertexPoint::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCVERTEXPOINT" << "(";
	if( m_VertexGeometry ) { stream << "#" << m_VertexGeometry->m_entity_id; } else { stream << "$"; }
	stream << ");";
}
void IfcVertexPoint::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcVertexPoint::toString() const { return L"IfcVertexPoint"; }
void IfcVertexPoint::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcVertexPoint, expecting 1, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_VertexGeometry, map );
}
void IfcVertexPoint::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcVertex::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "VertexGeometry", m_VertexGeometry ) );
}
void IfcVertexPoint::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcVertex::getAttributesInverse( vec_attributes_inverse );
}
void IfcVertexPoint::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcVertex::setInverseCounterparts( ptr_self_entity );
}
void IfcVertexPoint::unlinkFromInverseCounterparts()
{
	IfcVertex::unlinkFromInverseCounterparts();
}
