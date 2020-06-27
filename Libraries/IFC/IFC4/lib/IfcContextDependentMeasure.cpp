/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcMeasureValue.h"
#include "IFC4/include/IfcContextDependentMeasure.h"

// TYPE IfcContextDependentMeasure = REAL;
IfcContextDependentMeasure::IfcContextDependentMeasure() {}
IfcContextDependentMeasure::IfcContextDependentMeasure( double value ) { m_value = value; }
IfcContextDependentMeasure::~IfcContextDependentMeasure() {}
shared_ptr<IfcPPObject> IfcContextDependentMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcContextDependentMeasure> copy_self( new IfcContextDependentMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcContextDependentMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCONTEXTDEPENDENTMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcContextDependentMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcContextDependentMeasure> IfcContextDependentMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcContextDependentMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcContextDependentMeasure>(); }
	shared_ptr<IfcContextDependentMeasure> type_object( new IfcContextDependentMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
