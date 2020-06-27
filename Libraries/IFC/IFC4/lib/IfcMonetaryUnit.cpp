/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcLabel.h"
#include "IFC4/include/IfcMonetaryUnit.h"

// ENTITY IfcMonetaryUnit 
IfcMonetaryUnit::IfcMonetaryUnit() {}
IfcMonetaryUnit::IfcMonetaryUnit( int id ) { m_entity_id = id; }
IfcMonetaryUnit::~IfcMonetaryUnit() {}
shared_ptr<IfcPPObject> IfcMonetaryUnit::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMonetaryUnit> copy_self( new IfcMonetaryUnit() );
	if( m_Currency ) { copy_self->m_Currency = dynamic_pointer_cast<IfcLabel>( m_Currency->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMonetaryUnit::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCMONETARYUNIT" << "(";
	if( m_Currency ) { m_Currency->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMonetaryUnit::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcMonetaryUnit::toString() const { return L"IfcMonetaryUnit"; }
void IfcMonetaryUnit::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMonetaryUnit, expecting 1, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Currency = IfcLabel::createObjectFromSTEP( args[0], map );
}
void IfcMonetaryUnit::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Currency", m_Currency ) );
}
void IfcMonetaryUnit::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMonetaryUnit::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcMonetaryUnit::unlinkFromInverseCounterparts()
{
}
