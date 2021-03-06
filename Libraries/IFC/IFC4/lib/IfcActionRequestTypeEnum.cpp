/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcActionRequestTypeEnum.h"

// TYPE IfcActionRequestTypeEnum = ENUMERATION OF	(EMAIL	,FAX	,PHONE	,POST	,VERBAL	,USERDEFINED	,NOTDEFINED);
IfcActionRequestTypeEnum::IfcActionRequestTypeEnum() {}
IfcActionRequestTypeEnum::~IfcActionRequestTypeEnum() {}
shared_ptr<IfcPPObject> IfcActionRequestTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcActionRequestTypeEnum> copy_self( new IfcActionRequestTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcActionRequestTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCACTIONREQUESTTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_EMAIL:	stream << ".EMAIL."; break;
		case ENUM_FAX:	stream << ".FAX."; break;
		case ENUM_PHONE:	stream << ".PHONE."; break;
		case ENUM_POST:	stream << ".POST."; break;
		case ENUM_VERBAL:	stream << ".VERBAL."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcActionRequestTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_EMAIL:	return L"EMAIL";
		case ENUM_FAX:	return L"FAX";
		case ENUM_PHONE:	return L"PHONE";
		case ENUM_POST:	return L"POST";
		case ENUM_VERBAL:	return L"VERBAL";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcActionRequestTypeEnum> IfcActionRequestTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcActionRequestTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcActionRequestTypeEnum>(); }
	shared_ptr<IfcActionRequestTypeEnum> type_object( new IfcActionRequestTypeEnum() );
	if( boost::iequals( arg, L".EMAIL." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_EMAIL;
	}
	else if( boost::iequals( arg, L".FAX." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_FAX;
	}
	else if( boost::iequals( arg, L".PHONE." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_PHONE;
	}
	else if( boost::iequals( arg, L".POST." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_POST;
	}
	else if( boost::iequals( arg, L".VERBAL." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_VERBAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcActionRequestTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
