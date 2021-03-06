/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcLuminousIntensityDistributionMeasure.h"

// TYPE IfcLuminousIntensityDistributionMeasure = REAL;
IfcLuminousIntensityDistributionMeasure::IfcLuminousIntensityDistributionMeasure() {}
IfcLuminousIntensityDistributionMeasure::IfcLuminousIntensityDistributionMeasure( double value ) { m_value = value; }
IfcLuminousIntensityDistributionMeasure::~IfcLuminousIntensityDistributionMeasure() {}
shared_ptr<IfcPPObject> IfcLuminousIntensityDistributionMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLuminousIntensityDistributionMeasure> copy_self( new IfcLuminousIntensityDistributionMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcLuminousIntensityDistributionMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLUMINOUSINTENSITYDISTRIBUTIONMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcLuminousIntensityDistributionMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcLuminousIntensityDistributionMeasure> IfcLuminousIntensityDistributionMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcLuminousIntensityDistributionMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcLuminousIntensityDistributionMeasure>(); }
	shared_ptr<IfcLuminousIntensityDistributionMeasure> type_object( new IfcLuminousIntensityDistributionMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}
