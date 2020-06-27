/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcMeasureValue.h"
#include "IFC4/include/IfcElectricCurrentMeasure.h"

// TYPE IfcElectricCurrentMeasure = REAL;
IfcElectricCurrentMeasure::IfcElectricCurrentMeasure() {}
IfcElectricCurrentMeasure::IfcElectricCurrentMeasure( double value ) { m_value = value; }
IfcElectricCurrentMeasure::~IfcElectricCurrentMeasure() {}
shared_ptr<IfcPPObject> IfcElectricCurrentMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcElectricCurrentMeasure> copy_self( new IfcElectricCurrentMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcElectricCurrentMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELECTRICCURRENTMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcElectricCurrentMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcElectricCurrentMeasure> IfcElectricCurrentMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcElectricCurrentMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcElectricCurrentMeasure>(); }
	shared_ptr<IfcElectricCurrentMeasure> type_object( new IfcElectricCurrentMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}