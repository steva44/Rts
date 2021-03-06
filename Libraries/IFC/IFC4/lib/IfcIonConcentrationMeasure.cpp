/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcIonConcentrationMeasure.h"

// TYPE IfcIonConcentrationMeasure = REAL;
IfcIonConcentrationMeasure::IfcIonConcentrationMeasure() {}
IfcIonConcentrationMeasure::IfcIonConcentrationMeasure( double value ) { m_value = value; }
IfcIonConcentrationMeasure::~IfcIonConcentrationMeasure() {}
shared_ptr<IfcPPObject> IfcIonConcentrationMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcIonConcentrationMeasure> copy_self( new IfcIonConcentrationMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcIonConcentrationMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCIONCONCENTRATIONMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcIonConcentrationMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcIonConcentrationMeasure> IfcIonConcentrationMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcIonConcentrationMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcIonConcentrationMeasure>(); }
	shared_ptr<IfcIonConcentrationMeasure> type_object( new IfcIonConcentrationMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
