/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcMeasureValue.h"
#include "IFC4/include/IfcNonNegativeLengthMeasure.h"

// TYPE IfcNonNegativeLengthMeasure = IfcLengthMeasure;
IfcNonNegativeLengthMeasure::IfcNonNegativeLengthMeasure() {}
IfcNonNegativeLengthMeasure::~IfcNonNegativeLengthMeasure() {}
shared_ptr<IfcPPObject> IfcNonNegativeLengthMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcNonNegativeLengthMeasure> copy_self( new IfcNonNegativeLengthMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcNonNegativeLengthMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCNONNEGATIVELENGTHMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcNonNegativeLengthMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcNonNegativeLengthMeasure> IfcNonNegativeLengthMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcNonNegativeLengthMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcNonNegativeLengthMeasure>(); }
	shared_ptr<IfcNonNegativeLengthMeasure> type_object( new IfcNonNegativeLengthMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
