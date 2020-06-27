/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcTaskDurationEnum.h"

// TYPE IfcTaskDurationEnum = ENUMERATION OF	(ELAPSEDTIME	,WORKTIME	,NOTDEFINED);
IfcTaskDurationEnum::IfcTaskDurationEnum() {}
IfcTaskDurationEnum::~IfcTaskDurationEnum() {}
shared_ptr<IfcPPObject> IfcTaskDurationEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTaskDurationEnum> copy_self( new IfcTaskDurationEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTaskDurationEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTASKDURATIONENUM("; }
	switch( m_enum )
	{
		case ENUM_ELAPSEDTIME:	stream << ".ELAPSEDTIME."; break;
		case ENUM_WORKTIME:	stream << ".WORKTIME."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcTaskDurationEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_ELAPSEDTIME:	return L"ELAPSEDTIME";
		case ENUM_WORKTIME:	return L"WORKTIME";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcTaskDurationEnum> IfcTaskDurationEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTaskDurationEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTaskDurationEnum>(); }
	shared_ptr<IfcTaskDurationEnum> type_object( new IfcTaskDurationEnum() );
	if( boost::iequals( arg, L".ELAPSEDTIME." ) )
	{
		type_object->m_enum = IfcTaskDurationEnum::ENUM_ELAPSEDTIME;
	}
	else if( boost::iequals( arg, L".WORKTIME." ) )
	{
		type_object->m_enum = IfcTaskDurationEnum::ENUM_WORKTIME;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTaskDurationEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}