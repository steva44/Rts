/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcAxis1Placement.h"
#include "IFC4/include/IfcCartesianPoint.h"
#include "IFC4/include/IfcDirection.h"
#include "IFC4/include/IfcPresentationLayerAssignment.h"
#include "IFC4/include/IfcStyledItem.h"

// ENTITY IfcAxis1Placement 
IfcAxis1Placement::IfcAxis1Placement() {}
IfcAxis1Placement::IfcAxis1Placement( int id ) { m_entity_id = id; }
IfcAxis1Placement::~IfcAxis1Placement() {}
shared_ptr<IfcPPObject> IfcAxis1Placement::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAxis1Placement> copy_self( new IfcAxis1Placement() );
	if( m_Location ) { copy_self->m_Location = dynamic_pointer_cast<IfcCartesianPoint>( m_Location->getDeepCopy(options) ); }
	if( m_Axis ) { copy_self->m_Axis = dynamic_pointer_cast<IfcDirection>( m_Axis->getDeepCopy(options) ); }
	return copy_self;
}
void IfcAxis1Placement::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCAXIS1PLACEMENT" << "(";
	if( m_Location ) { stream << "#" << m_Location->m_entity_id; } else { stream << "*"; }
	stream << ",";
	if( m_Axis ) { stream << "#" << m_Axis->m_entity_id; } else { stream << "$"; }
	stream << ");";
}
void IfcAxis1Placement::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcAxis1Placement::toString() const { return L"IfcAxis1Placement"; }
void IfcAxis1Placement::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcAxis1Placement, expecting 2, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Location, map );
	readEntityReference( args[1], m_Axis, map );
}
void IfcAxis1Placement::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPlacement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Axis", m_Axis ) );
}
void IfcAxis1Placement::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPlacement::getAttributesInverse( vec_attributes_inverse );
}
void IfcAxis1Placement::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPlacement::setInverseCounterparts( ptr_self_entity );
}
void IfcAxis1Placement::unlinkFromInverseCounterparts()
{
	IfcPlacement::unlinkFromInverseCounterparts();
}
