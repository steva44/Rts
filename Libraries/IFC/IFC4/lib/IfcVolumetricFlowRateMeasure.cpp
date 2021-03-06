/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcVolumetricFlowRateMeasure.h"

// TYPE IfcVolumetricFlowRateMeasure = REAL;
IfcVolumetricFlowRateMeasure::IfcVolumetricFlowRateMeasure() {}
IfcVolumetricFlowRateMeasure::IfcVolumetricFlowRateMeasure( double value ) { m_value = value; }
IfcVolumetricFlowRateMeasure::~IfcVolumetricFlowRateMeasure() {}
shared_ptr<IfcPPObject> IfcVolumetricFlowRateMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcVolumetricFlowRateMeasure> copy_self( new IfcVolumetricFlowRateMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcVolumetricFlowRateMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCVOLUMETRICFLOWRATEMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcVolumetricFlowRateMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcVolumetricFlowRateMeasure> IfcVolumetricFlowRateMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcVolumetricFlowRateMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcVolumetricFlowRateMeasure>(); }
	shared_ptr<IfcVolumetricFlowRateMeasure> type_object( new IfcVolumetricFlowRateMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
