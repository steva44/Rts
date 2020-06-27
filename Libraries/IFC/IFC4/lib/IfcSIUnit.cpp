/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcDimensionalExponents.h"
#include "IFC4/include/IfcSIPrefix.h"
#include "IFC4/include/IfcSIUnit.h"
#include "IFC4/include/IfcSIUnitName.h"
#include "IFC4/include/IfcUnitEnum.h"

// ENTITY IfcSIUnit 
IfcSIUnit::IfcSIUnit() {}
IfcSIUnit::IfcSIUnit( int id ) { m_entity_id = id; }
IfcSIUnit::~IfcSIUnit() {}
shared_ptr<IfcPPObject> IfcSIUnit::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSIUnit> copy_self( new IfcSIUnit() );
	if( m_Dimensions ) { copy_self->m_Dimensions = dynamic_pointer_cast<IfcDimensionalExponents>( m_Dimensions->getDeepCopy(options) ); }
	if( m_UnitType ) { copy_self->m_UnitType = dynamic_pointer_cast<IfcUnitEnum>( m_UnitType->getDeepCopy(options) ); }
	if( m_Prefix ) { copy_self->m_Prefix = dynamic_pointer_cast<IfcSIPrefix>( m_Prefix->getDeepCopy(options) ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcSIUnitName>( m_Name->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSIUnit::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCSIUNIT" << "(";
	if( m_Dimensions ) { stream << "#" << m_Dimensions->m_entity_id; } else { stream << "*"; }
	stream << ",";
	if( m_UnitType ) { m_UnitType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Prefix ) { m_Prefix->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSIUnit::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcSIUnit::toString() const { return L"IfcSIUnit"; }
void IfcSIUnit::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSIUnit, expecting 4, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Dimensions, map );
	m_UnitType = IfcUnitEnum::createObjectFromSTEP( args[1], map );
	m_Prefix = IfcSIPrefix::createObjectFromSTEP( args[2], map );
	m_Name = IfcSIUnitName::createObjectFromSTEP( args[3], map );
}
void IfcSIUnit::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcNamedUnit::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Prefix", m_Prefix ) );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
}
void IfcSIUnit::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcNamedUnit::getAttributesInverse( vec_attributes_inverse );
}
void IfcSIUnit::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcNamedUnit::setInverseCounterparts( ptr_self_entity );
}
void IfcSIUnit::unlinkFromInverseCounterparts()
{
	IfcNamedUnit::unlinkFromInverseCounterparts();
}