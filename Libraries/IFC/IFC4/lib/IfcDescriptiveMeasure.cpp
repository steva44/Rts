/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcMeasureValue.h"
#include "IFC4/include/IfcSizeSelect.h"
#include "IFC4/include/IfcDescriptiveMeasure.h"

// TYPE IfcDescriptiveMeasure = STRING;
IfcDescriptiveMeasure::IfcDescriptiveMeasure() {}
IfcDescriptiveMeasure::IfcDescriptiveMeasure( std::wstring value ) { m_value = value; }
IfcDescriptiveMeasure::~IfcDescriptiveMeasure() {}
shared_ptr<IfcPPObject> IfcDescriptiveMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDescriptiveMeasure> copy_self( new IfcDescriptiveMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcDescriptiveMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDESCRIPTIVEMEASURE("; }
	stream << "'" << encodeStepString( m_value ) << "'";
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcDescriptiveMeasure::toString() const
{
	return m_value;
}
shared_ptr<IfcDescriptiveMeasure> IfcDescriptiveMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDescriptiveMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDescriptiveMeasure>(); }
	shared_ptr<IfcDescriptiveMeasure> type_object( new IfcDescriptiveMeasure() );
	readString( arg, type_object->m_value );
	return type_object;
}