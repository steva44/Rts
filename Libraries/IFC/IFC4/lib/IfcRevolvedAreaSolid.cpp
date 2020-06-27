/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcAxis1Placement.h"
#include "IFC4/include/IfcAxis2Placement3D.h"
#include "IFC4/include/IfcPlaneAngleMeasure.h"
#include "IFC4/include/IfcPresentationLayerAssignment.h"
#include "IFC4/include/IfcProfileDef.h"
#include "IFC4/include/IfcRevolvedAreaSolid.h"
#include "IFC4/include/IfcStyledItem.h"

// ENTITY IfcRevolvedAreaSolid 
IfcRevolvedAreaSolid::IfcRevolvedAreaSolid() {}
IfcRevolvedAreaSolid::IfcRevolvedAreaSolid( int id ) { m_entity_id = id; }
IfcRevolvedAreaSolid::~IfcRevolvedAreaSolid() {}
shared_ptr<IfcPPObject> IfcRevolvedAreaSolid::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRevolvedAreaSolid> copy_self( new IfcRevolvedAreaSolid() );
	if( m_SweptArea )
	{
		if( options.shallow_copy_IfcProfileDef ) { copy_self->m_SweptArea = m_SweptArea; }
		else { copy_self->m_SweptArea = dynamic_pointer_cast<IfcProfileDef>( m_SweptArea->getDeepCopy(options) ); }
	}
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement3D>( m_Position->getDeepCopy(options) ); }
	if( m_Axis ) { copy_self->m_Axis = dynamic_pointer_cast<IfcAxis1Placement>( m_Axis->getDeepCopy(options) ); }
	if( m_Angle ) { copy_self->m_Angle = dynamic_pointer_cast<IfcPlaneAngleMeasure>( m_Angle->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRevolvedAreaSolid::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCREVOLVEDAREASOLID" << "(";
	if( m_SweptArea ) { stream << "#" << m_SweptArea->m_entity_id; } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_entity_id; } else { stream << "*"; }
	stream << ",";
	if( m_Axis ) { stream << "#" << m_Axis->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_Angle ) { m_Angle->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcRevolvedAreaSolid::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcRevolvedAreaSolid::toString() const { return L"IfcRevolvedAreaSolid"; }
void IfcRevolvedAreaSolid::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRevolvedAreaSolid, expecting 4, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_SweptArea, map );
	readEntityReference( args[1], m_Position, map );
	readEntityReference( args[2], m_Axis, map );
	m_Angle = IfcPlaneAngleMeasure::createObjectFromSTEP( args[3], map );
}
void IfcRevolvedAreaSolid::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSweptAreaSolid::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Axis", m_Axis ) );
	vec_attributes.push_back( std::make_pair( "Angle", m_Angle ) );
}
void IfcRevolvedAreaSolid::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSweptAreaSolid::getAttributesInverse( vec_attributes_inverse );
}
void IfcRevolvedAreaSolid::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSweptAreaSolid::setInverseCounterparts( ptr_self_entity );
}
void IfcRevolvedAreaSolid::unlinkFromInverseCounterparts()
{
	IfcSweptAreaSolid::unlinkFromInverseCounterparts();
}