/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcSimpleValue.h"
#include "IFC4/include/IfcTimeOrRatioSelect.h"
#include "IFC4/include/IfcDuration.h"

// TYPE IfcDuration = STRING;
IfcDuration::IfcDuration() {}
IfcDuration::IfcDuration( std::wstring value ) { m_value = value; }
IfcDuration::~IfcDuration() {}
shared_ptr<IfcPPObject> IfcDuration::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDuration> copy_self( new IfcDuration() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcDuration::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDURATION("; }
	stream << "'" << encodeStepString( m_value ) << "'";
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcDuration::toString() const
{
	return m_value;
}
shared_ptr<IfcDuration> IfcDuration::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDuration>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDuration>(); }
	shared_ptr<IfcDuration> type_object( new IfcDuration() );
	readString( arg, type_object->m_value );
	return type_object;
}