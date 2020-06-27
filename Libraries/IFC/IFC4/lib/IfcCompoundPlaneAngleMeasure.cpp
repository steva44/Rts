/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcCompoundPlaneAngleMeasure.h"

// TYPE IfcCompoundPlaneAngleMeasure = LIST [3:4] OF INTEGER;
IfcCompoundPlaneAngleMeasure::IfcCompoundPlaneAngleMeasure() {}
IfcCompoundPlaneAngleMeasure::~IfcCompoundPlaneAngleMeasure() {}
shared_ptr<IfcPPObject> IfcCompoundPlaneAngleMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCompoundPlaneAngleMeasure> copy_self( new IfcCompoundPlaneAngleMeasure() );
	for( size_t ii=0; ii<m_vec.size(); ++ii )
	{
		int item_ii = m_vec[ii];
		copy_self->m_vec.push_back( item_ii );
	}
	return copy_self;
}
void IfcCompoundPlaneAngleMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOMPOUNDPLANEANGLEMEASURE("; }
	writeNumericList( stream, m_vec );
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcCompoundPlaneAngleMeasure::toString() const
{
	std::wstringstream strs;
	for( size_t ii = 0; ii < m_vec.size(); ++ii )				{					if( ii > 0 )					{						strs << L", ";					}					strs << m_vec[ii];				}			return strs.str(); 
}
shared_ptr<IfcCompoundPlaneAngleMeasure> IfcCompoundPlaneAngleMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCompoundPlaneAngleMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCompoundPlaneAngleMeasure>(); }
	shared_ptr<IfcCompoundPlaneAngleMeasure> type_object( new IfcCompoundPlaneAngleMeasure() );
	readIntList( arg, type_object->m_vec );
	return type_object;
}