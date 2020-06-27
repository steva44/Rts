/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcPerformanceHistoryTypeEnum.h"

// TYPE IfcPerformanceHistoryTypeEnum = ENUMERATION OF	(USERDEFINED	,NOTDEFINED);
IfcPerformanceHistoryTypeEnum::IfcPerformanceHistoryTypeEnum() {}
IfcPerformanceHistoryTypeEnum::~IfcPerformanceHistoryTypeEnum() {}
shared_ptr<IfcPPObject> IfcPerformanceHistoryTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPerformanceHistoryTypeEnum> copy_self( new IfcPerformanceHistoryTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPerformanceHistoryTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPERFORMANCEHISTORYTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcPerformanceHistoryTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcPerformanceHistoryTypeEnum> IfcPerformanceHistoryTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPerformanceHistoryTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPerformanceHistoryTypeEnum>(); }
	shared_ptr<IfcPerformanceHistoryTypeEnum> type_object( new IfcPerformanceHistoryTypeEnum() );
	if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPerformanceHistoryTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPerformanceHistoryTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
