/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcEnergyMeasure.h"

// TYPE IfcEnergyMeasure = REAL;
IfcEnergyMeasure::IfcEnergyMeasure() {}
IfcEnergyMeasure::IfcEnergyMeasure( double value ) { m_value = value; }
IfcEnergyMeasure::~IfcEnergyMeasure() {}
shared_ptr<IfcPPObject> IfcEnergyMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcEnergyMeasure> copy_self( new IfcEnergyMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcEnergyMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCENERGYMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcEnergyMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcEnergyMeasure> IfcEnergyMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcEnergyMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcEnergyMeasure>(); }
	shared_ptr<IfcEnergyMeasure> type_object( new IfcEnergyMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
