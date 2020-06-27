/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcSimpleValue.h"
#include "IFC4/include/IfcBinary.h"

// TYPE IfcBinary = BINARY;
IfcBinary::IfcBinary() {}
IfcBinary::IfcBinary( std::wstring value ) { m_value = value; }
IfcBinary::~IfcBinary() {}
shared_ptr<IfcPPObject> IfcBinary::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBinary> copy_self( new IfcBinary() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcBinary::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBINARY("; }
	stream << "'" << encodeStepString( m_value ) << "'";
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcBinary::toString() const
{
	return m_value;
}
shared_ptr<IfcBinary> IfcBinary::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBinary>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBinary>(); }
	shared_ptr<IfcBinary> type_object( new IfcBinary() );
	readString( arg, type_object->m_value );
	return type_object;
}
