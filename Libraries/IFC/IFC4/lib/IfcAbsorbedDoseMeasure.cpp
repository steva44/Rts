/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcAbsorbedDoseMeasure.h"

// TYPE IfcAbsorbedDoseMeasure = REAL;
IfcAbsorbedDoseMeasure::IfcAbsorbedDoseMeasure() {}
IfcAbsorbedDoseMeasure::IfcAbsorbedDoseMeasure( double value ) { m_value = value; }
IfcAbsorbedDoseMeasure::~IfcAbsorbedDoseMeasure() {}
shared_ptr<IfcPPObject> IfcAbsorbedDoseMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAbsorbedDoseMeasure> copy_self( new IfcAbsorbedDoseMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcAbsorbedDoseMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCABSORBEDDOSEMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcAbsorbedDoseMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcAbsorbedDoseMeasure> IfcAbsorbedDoseMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcAbsorbedDoseMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcAbsorbedDoseMeasure>(); }
	shared_ptr<IfcAbsorbedDoseMeasure> type_object( new IfcAbsorbedDoseMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
