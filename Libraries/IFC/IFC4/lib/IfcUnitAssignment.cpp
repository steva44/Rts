/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcUnit.h"
#include "IFC4/include/IfcUnitAssignment.h"

// ENTITY IfcUnitAssignment 
IfcUnitAssignment::IfcUnitAssignment() {}
IfcUnitAssignment::IfcUnitAssignment( int id ) { m_entity_id = id; }
IfcUnitAssignment::~IfcUnitAssignment() {}
shared_ptr<IfcPPObject> IfcUnitAssignment::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcUnitAssignment> copy_self( new IfcUnitAssignment() );
	for( size_t ii=0; ii<m_Units.size(); ++ii )
	{
		auto item_ii = m_Units[ii];
		if( item_ii )
		{
			copy_self->m_Units.push_back( dynamic_pointer_cast<IfcUnit>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcUnitAssignment::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCUNITASSIGNMENT" << "(";
	stream << "(";
	for( size_t ii = 0; ii < m_Units.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcUnit>& type_object = m_Units[ii];
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
	stream << ");";
}
void IfcUnitAssignment::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcUnitAssignment::toString() const { return L"IfcUnitAssignment"; }
void IfcUnitAssignment::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcUnitAssignment, expecting 1, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readSelectList( args[0], m_Units, map );
}
void IfcUnitAssignment::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	if( m_Units.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Units_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_Units.begin(), m_Units.end(), std::back_inserter( Units_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Units", Units_vec_object ) );
	}
}
void IfcUnitAssignment::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcUnitAssignment::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcUnitAssignment::unlinkFromInverseCounterparts()
{
}
