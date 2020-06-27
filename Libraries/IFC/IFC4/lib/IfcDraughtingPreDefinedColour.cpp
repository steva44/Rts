/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcDraughtingPreDefinedColour.h"
#include "IFC4/include/IfcLabel.h"

// ENTITY IfcDraughtingPreDefinedColour 
IfcDraughtingPreDefinedColour::IfcDraughtingPreDefinedColour() {}
IfcDraughtingPreDefinedColour::IfcDraughtingPreDefinedColour( int id ) { m_entity_id = id; }
IfcDraughtingPreDefinedColour::~IfcDraughtingPreDefinedColour() {}
shared_ptr<IfcPPObject> IfcDraughtingPreDefinedColour::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDraughtingPreDefinedColour> copy_self( new IfcDraughtingPreDefinedColour() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	return copy_self;
}
void IfcDraughtingPreDefinedColour::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCDRAUGHTINGPREDEFINEDCOLOUR" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcDraughtingPreDefinedColour::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcDraughtingPreDefinedColour::toString() const { return L"IfcDraughtingPreDefinedColour"; }
void IfcDraughtingPreDefinedColour::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcDraughtingPreDefinedColour, expecting 1, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
}
void IfcDraughtingPreDefinedColour::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedColour::getAttributes( vec_attributes );
}
void IfcDraughtingPreDefinedColour::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPreDefinedColour::getAttributesInverse( vec_attributes_inverse );
}
void IfcDraughtingPreDefinedColour::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedColour::setInverseCounterparts( ptr_self_entity );
}
void IfcDraughtingPreDefinedColour::unlinkFromInverseCounterparts()
{
	IfcPreDefinedColour::unlinkFromInverseCounterparts();
}
