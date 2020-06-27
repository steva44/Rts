/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcMeasureValue.h"
#include "IFC4/include/IfcCountMeasure.h"

// TYPE IfcCountMeasure = NUMBER;
IfcCountMeasure::IfcCountMeasure() {}
IfcCountMeasure::IfcCountMeasure( int value ) { m_value = value; }
IfcCountMeasure::~IfcCountMeasure() {}
shared_ptr<IfcPPObject> IfcCountMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCountMeasure> copy_self( new IfcCountMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcCountMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOUNTMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcCountMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcCountMeasure> IfcCountMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCountMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCountMeasure>(); }
	shared_ptr<IfcCountMeasure> type_object( new IfcCountMeasure() );
	readInt( arg, type_object->m_value );
	return type_object;
}
