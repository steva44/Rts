/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDuctFittingTypeEnum.h"

// TYPE IfcDuctFittingTypeEnum = ENUMERATION OF	(BEND	,CONNECTOR	,ENTRY	,EXIT	,JUNCTION	,OBSTRUCTION	,TRANSITION	,USERDEFINED	,NOTDEFINED);
IfcDuctFittingTypeEnum::IfcDuctFittingTypeEnum() {}
IfcDuctFittingTypeEnum::~IfcDuctFittingTypeEnum() {}
shared_ptr<IfcPPObject> IfcDuctFittingTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDuctFittingTypeEnum> copy_self( new IfcDuctFittingTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDuctFittingTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDUCTFITTINGTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_BEND:	stream << ".BEND."; break;
		case ENUM_CONNECTOR:	stream << ".CONNECTOR."; break;
		case ENUM_ENTRY:	stream << ".ENTRY."; break;
		case ENUM_EXIT:	stream << ".EXIT."; break;
		case ENUM_JUNCTION:	stream << ".JUNCTION."; break;
		case ENUM_OBSTRUCTION:	stream << ".OBSTRUCTION."; break;
		case ENUM_TRANSITION:	stream << ".TRANSITION."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcDuctFittingTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_BEND:	return L"BEND";
		case ENUM_CONNECTOR:	return L"CONNECTOR";
		case ENUM_ENTRY:	return L"ENTRY";
		case ENUM_EXIT:	return L"EXIT";
		case ENUM_JUNCTION:	return L"JUNCTION";
		case ENUM_OBSTRUCTION:	return L"OBSTRUCTION";
		case ENUM_TRANSITION:	return L"TRANSITION";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcDuctFittingTypeEnum> IfcDuctFittingTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDuctFittingTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDuctFittingTypeEnum>(); }
	shared_ptr<IfcDuctFittingTypeEnum> type_object( new IfcDuctFittingTypeEnum() );
	if( boost::iequals( arg, L".BEND." ) )
	{
		type_object->m_enum = IfcDuctFittingTypeEnum::ENUM_BEND;
	}
	else if( boost::iequals( arg, L".CONNECTOR." ) )
	{
		type_object->m_enum = IfcDuctFittingTypeEnum::ENUM_CONNECTOR;
	}
	else if( boost::iequals( arg, L".ENTRY." ) )
	{
		type_object->m_enum = IfcDuctFittingTypeEnum::ENUM_ENTRY;
	}
	else if( boost::iequals( arg, L".EXIT." ) )
	{
		type_object->m_enum = IfcDuctFittingTypeEnum::ENUM_EXIT;
	}
	else if( boost::iequals( arg, L".JUNCTION." ) )
	{
		type_object->m_enum = IfcDuctFittingTypeEnum::ENUM_JUNCTION;
	}
	else if( boost::iequals( arg, L".OBSTRUCTION." ) )
	{
		type_object->m_enum = IfcDuctFittingTypeEnum::ENUM_OBSTRUCTION;
	}
	else if( boost::iequals( arg, L".TRANSITION." ) )
	{
		type_object->m_enum = IfcDuctFittingTypeEnum::ENUM_TRANSITION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDuctFittingTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDuctFittingTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
