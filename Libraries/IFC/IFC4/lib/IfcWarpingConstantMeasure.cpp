/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcWarpingConstantMeasure.h"

// TYPE IfcWarpingConstantMeasure = REAL;
IfcWarpingConstantMeasure::IfcWarpingConstantMeasure() {}
IfcWarpingConstantMeasure::IfcWarpingConstantMeasure( double value ) { m_value = value; }
IfcWarpingConstantMeasure::~IfcWarpingConstantMeasure() {}
shared_ptr<IfcPPObject> IfcWarpingConstantMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWarpingConstantMeasure> copy_self( new IfcWarpingConstantMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcWarpingConstantMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWARPINGCONSTANTMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcWarpingConstantMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcWarpingConstantMeasure> IfcWarpingConstantMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWarpingConstantMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWarpingConstantMeasure>(); }
	shared_ptr<IfcWarpingConstantMeasure> type_object( new IfcWarpingConstantMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
