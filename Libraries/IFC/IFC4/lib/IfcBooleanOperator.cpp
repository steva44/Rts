/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcBooleanOperator.h"

// TYPE IfcBooleanOperator = ENUMERATION OF	(UNION	,INTERSECTION	,DIFFERENCE);
IfcBooleanOperator::IfcBooleanOperator() {}
IfcBooleanOperator::~IfcBooleanOperator() {}
shared_ptr<IfcPPObject> IfcBooleanOperator::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBooleanOperator> copy_self( new IfcBooleanOperator() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBooleanOperator::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBOOLEANOPERATOR("; }
	switch( m_enum )
	{
		case ENUM_UNION:	stream << ".UNION."; break;
		case ENUM_INTERSECTION:	stream << ".INTERSECTION."; break;
		case ENUM_DIFFERENCE:	stream << ".DIFFERENCE."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcBooleanOperator::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_UNION:	return L"UNION";
		case ENUM_INTERSECTION:	return L"INTERSECTION";
		case ENUM_DIFFERENCE:	return L"DIFFERENCE";
	}
	return L"";
}
shared_ptr<IfcBooleanOperator> IfcBooleanOperator::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBooleanOperator>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBooleanOperator>(); }
	shared_ptr<IfcBooleanOperator> type_object( new IfcBooleanOperator() );
	if( boost::iequals( arg, L".UNION." ) )
	{
		type_object->m_enum = IfcBooleanOperator::ENUM_UNION;
	}
	else if( boost::iequals( arg, L".INTERSECTION." ) )
	{
		type_object->m_enum = IfcBooleanOperator::ENUM_INTERSECTION;
	}
	else if( boost::iequals( arg, L".DIFFERENCE." ) )
	{
		type_object->m_enum = IfcBooleanOperator::ENUM_DIFFERENCE;
	}
	return type_object;
}
