/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcHatchLineDistanceSelect.h"
#include "IFC4/include/IfcMeasureValue.h"
#include "IFC4/include/IfcSizeSelect.h"
#include "IFC4/include/IfcPositiveLengthMeasure.h"

// TYPE IfcPositiveLengthMeasure = IfcLengthMeasure;
IfcPositiveLengthMeasure::IfcPositiveLengthMeasure() {}
IfcPositiveLengthMeasure::~IfcPositiveLengthMeasure() {}
shared_ptr<IfcPPObject> IfcPositiveLengthMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPositiveLengthMeasure> copy_self( new IfcPositiveLengthMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcPositiveLengthMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPOSITIVELENGTHMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcPositiveLengthMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcPositiveLengthMeasure> IfcPositiveLengthMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPositiveLengthMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPositiveLengthMeasure>(); }
	shared_ptr<IfcPositiveLengthMeasure> type_object( new IfcPositiveLengthMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
