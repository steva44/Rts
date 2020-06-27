/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcSpaceTypeEnum.h"

// TYPE IfcSpaceTypeEnum = ENUMERATION OF	(SPACE	,PARKING	,GFA	,INTERNAL	,EXTERNAL	,USERDEFINED	,NOTDEFINED);
IfcSpaceTypeEnum::IfcSpaceTypeEnum() {}
IfcSpaceTypeEnum::~IfcSpaceTypeEnum() {}
shared_ptr<IfcPPObject> IfcSpaceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSpaceTypeEnum> copy_self( new IfcSpaceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSpaceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSPACETYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_SPACE:	stream << ".SPACE."; break;
		case ENUM_PARKING:	stream << ".PARKING."; break;
		case ENUM_GFA:	stream << ".GFA."; break;
		case ENUM_INTERNAL:	stream << ".INTERNAL."; break;
		case ENUM_EXTERNAL:	stream << ".EXTERNAL."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcSpaceTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_SPACE:	return L"SPACE";
		case ENUM_PARKING:	return L"PARKING";
		case ENUM_GFA:	return L"GFA";
		case ENUM_INTERNAL:	return L"INTERNAL";
		case ENUM_EXTERNAL:	return L"EXTERNAL";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcSpaceTypeEnum> IfcSpaceTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSpaceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSpaceTypeEnum>(); }
	shared_ptr<IfcSpaceTypeEnum> type_object( new IfcSpaceTypeEnum() );
	if( boost::iequals( arg, L".SPACE." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_SPACE;
	}
	else if( boost::iequals( arg, L".PARKING." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_PARKING;
	}
	else if( boost::iequals( arg, L".GFA." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_GFA;
	}
	else if( boost::iequals( arg, L".INTERNAL." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_INTERNAL;
	}
	else if( boost::iequals( arg, L".EXTERNAL." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_EXTERNAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
