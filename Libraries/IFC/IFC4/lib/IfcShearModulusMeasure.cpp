/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcShearModulusMeasure.h"

// TYPE IfcShearModulusMeasure = REAL;
IfcShearModulusMeasure::IfcShearModulusMeasure() {}
IfcShearModulusMeasure::IfcShearModulusMeasure( double value ) { m_value = value; }
IfcShearModulusMeasure::~IfcShearModulusMeasure() {}
shared_ptr<IfcPPObject> IfcShearModulusMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcShearModulusMeasure> copy_self( new IfcShearModulusMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcShearModulusMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSHEARMODULUSMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcShearModulusMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcShearModulusMeasure> IfcShearModulusMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcShearModulusMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcShearModulusMeasure>(); }
	shared_ptr<IfcShearModulusMeasure> type_object( new IfcShearModulusMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}