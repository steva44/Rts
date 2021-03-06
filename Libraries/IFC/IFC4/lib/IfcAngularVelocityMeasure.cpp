/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcAngularVelocityMeasure.h"

// TYPE IfcAngularVelocityMeasure = REAL;
IfcAngularVelocityMeasure::IfcAngularVelocityMeasure() {}
IfcAngularVelocityMeasure::IfcAngularVelocityMeasure( double value ) { m_value = value; }
IfcAngularVelocityMeasure::~IfcAngularVelocityMeasure() {}
shared_ptr<IfcPPObject> IfcAngularVelocityMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAngularVelocityMeasure> copy_self( new IfcAngularVelocityMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcAngularVelocityMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCANGULARVELOCITYMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcAngularVelocityMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcAngularVelocityMeasure> IfcAngularVelocityMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcAngularVelocityMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcAngularVelocityMeasure>(); }
	shared_ptr<IfcAngularVelocityMeasure> type_object( new IfcAngularVelocityMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
