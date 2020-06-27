/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcExternalReferenceRelationship.h"
#include "IFC4/include/IfcPreDefinedProperties.h"

// ENTITY IfcPreDefinedProperties 
IfcPreDefinedProperties::IfcPreDefinedProperties() {}
IfcPreDefinedProperties::IfcPreDefinedProperties( int id ) { m_entity_id = id; }
IfcPreDefinedProperties::~IfcPreDefinedProperties() {}
shared_ptr<IfcPPObject> IfcPreDefinedProperties::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPreDefinedProperties> copy_self( new IfcPreDefinedProperties() );
	return copy_self;
}
void IfcPreDefinedProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCPREDEFINEDPROPERTIES" << "(";
	stream << ");";
}
void IfcPreDefinedProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcPreDefinedProperties::toString() const { return L"IfcPreDefinedProperties"; }
void IfcPreDefinedProperties::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcPreDefinedProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPropertyAbstraction::getAttributes( vec_attributes );
}
void IfcPreDefinedProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPropertyAbstraction::getAttributesInverse( vec_attributes_inverse );
}
void IfcPreDefinedProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPropertyAbstraction::setInverseCounterparts( ptr_self_entity );
}
void IfcPreDefinedProperties::unlinkFromInverseCounterparts()
{
	IfcPropertyAbstraction::unlinkFromInverseCounterparts();
}