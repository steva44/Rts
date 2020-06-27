/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcExternalReferenceRelationship.h"
#include "IFC4/include/IfcExternallyDefinedTextFont.h"
#include "IFC4/include/IfcIdentifier.h"
#include "IFC4/include/IfcLabel.h"
#include "IFC4/include/IfcURIReference.h"

// ENTITY IfcExternallyDefinedTextFont 
IfcExternallyDefinedTextFont::IfcExternallyDefinedTextFont() {}
IfcExternallyDefinedTextFont::IfcExternallyDefinedTextFont( int id ) { m_entity_id = id; }
IfcExternallyDefinedTextFont::~IfcExternallyDefinedTextFont() {}
shared_ptr<IfcPPObject> IfcExternallyDefinedTextFont::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcExternallyDefinedTextFont> copy_self( new IfcExternallyDefinedTextFont() );
	if( m_Location ) { copy_self->m_Location = dynamic_pointer_cast<IfcURIReference>( m_Location->getDeepCopy(options) ); }
	if( m_Identification ) { copy_self->m_Identification = dynamic_pointer_cast<IfcIdentifier>( m_Identification->getDeepCopy(options) ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	return copy_self;
}
void IfcExternallyDefinedTextFont::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCEXTERNALLYDEFINEDTEXTFONT" << "(";
	if( m_Location ) { m_Location->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcExternallyDefinedTextFont::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcExternallyDefinedTextFont::toString() const { return L"IfcExternallyDefinedTextFont"; }
void IfcExternallyDefinedTextFont::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcExternallyDefinedTextFont, expecting 3, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Location = IfcURIReference::createObjectFromSTEP( args[0], map );
	m_Identification = IfcIdentifier::createObjectFromSTEP( args[1], map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2], map );
}
void IfcExternallyDefinedTextFont::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcExternalReference::getAttributes( vec_attributes );
}
void IfcExternallyDefinedTextFont::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcExternalReference::getAttributesInverse( vec_attributes_inverse );
}
void IfcExternallyDefinedTextFont::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcExternalReference::setInverseCounterparts( ptr_self_entity );
}
void IfcExternallyDefinedTextFont::unlinkFromInverseCounterparts()
{
	IfcExternalReference::unlinkFromInverseCounterparts();
}