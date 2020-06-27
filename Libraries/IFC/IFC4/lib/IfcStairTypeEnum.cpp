/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcStairTypeEnum.h"

// TYPE IfcStairTypeEnum = ENUMERATION OF	(STRAIGHT_RUN_STAIR	,TWO_STRAIGHT_RUN_STAIR	,QUARTER_WINDING_STAIR	,QUARTER_TURN_STAIR	,HALF_WINDING_STAIR	,HALF_TURN_STAIR	,TWO_QUARTER_WINDING_STAIR	,TWO_QUARTER_TURN_STAIR	,THREE_QUARTER_WINDING_STAIR	,THREE_QUARTER_TURN_STAIR	,SPIRAL_STAIR	,DOUBLE_RETURN_STAIR	,CURVED_RUN_STAIR	,TWO_CURVED_RUN_STAIR	,USERDEFINED	,NOTDEFINED);
IfcStairTypeEnum::IfcStairTypeEnum() {}
IfcStairTypeEnum::~IfcStairTypeEnum() {}
shared_ptr<IfcPPObject> IfcStairTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStairTypeEnum> copy_self( new IfcStairTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcStairTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSTAIRTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_STRAIGHT_RUN_STAIR:	stream << ".STRAIGHT_RUN_STAIR."; break;
		case ENUM_TWO_STRAIGHT_RUN_STAIR:	stream << ".TWO_STRAIGHT_RUN_STAIR."; break;
		case ENUM_QUARTER_WINDING_STAIR:	stream << ".QUARTER_WINDING_STAIR."; break;
		case ENUM_QUARTER_TURN_STAIR:	stream << ".QUARTER_TURN_STAIR."; break;
		case ENUM_HALF_WINDING_STAIR:	stream << ".HALF_WINDING_STAIR."; break;
		case ENUM_HALF_TURN_STAIR:	stream << ".HALF_TURN_STAIR."; break;
		case ENUM_TWO_QUARTER_WINDING_STAIR:	stream << ".TWO_QUARTER_WINDING_STAIR."; break;
		case ENUM_TWO_QUARTER_TURN_STAIR:	stream << ".TWO_QUARTER_TURN_STAIR."; break;
		case ENUM_THREE_QUARTER_WINDING_STAIR:	stream << ".THREE_QUARTER_WINDING_STAIR."; break;
		case ENUM_THREE_QUARTER_TURN_STAIR:	stream << ".THREE_QUARTER_TURN_STAIR."; break;
		case ENUM_SPIRAL_STAIR:	stream << ".SPIRAL_STAIR."; break;
		case ENUM_DOUBLE_RETURN_STAIR:	stream << ".DOUBLE_RETURN_STAIR."; break;
		case ENUM_CURVED_RUN_STAIR:	stream << ".CURVED_RUN_STAIR."; break;
		case ENUM_TWO_CURVED_RUN_STAIR:	stream << ".TWO_CURVED_RUN_STAIR."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcStairTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_STRAIGHT_RUN_STAIR:	return L"STRAIGHT_RUN_STAIR";
		case ENUM_TWO_STRAIGHT_RUN_STAIR:	return L"TWO_STRAIGHT_RUN_STAIR";
		case ENUM_QUARTER_WINDING_STAIR:	return L"QUARTER_WINDING_STAIR";
		case ENUM_QUARTER_TURN_STAIR:	return L"QUARTER_TURN_STAIR";
		case ENUM_HALF_WINDING_STAIR:	return L"HALF_WINDING_STAIR";
		case ENUM_HALF_TURN_STAIR:	return L"HALF_TURN_STAIR";
		case ENUM_TWO_QUARTER_WINDING_STAIR:	return L"TWO_QUARTER_WINDING_STAIR";
		case ENUM_TWO_QUARTER_TURN_STAIR:	return L"TWO_QUARTER_TURN_STAIR";
		case ENUM_THREE_QUARTER_WINDING_STAIR:	return L"THREE_QUARTER_WINDING_STAIR";
		case ENUM_THREE_QUARTER_TURN_STAIR:	return L"THREE_QUARTER_TURN_STAIR";
		case ENUM_SPIRAL_STAIR:	return L"SPIRAL_STAIR";
		case ENUM_DOUBLE_RETURN_STAIR:	return L"DOUBLE_RETURN_STAIR";
		case ENUM_CURVED_RUN_STAIR:	return L"CURVED_RUN_STAIR";
		case ENUM_TWO_CURVED_RUN_STAIR:	return L"TWO_CURVED_RUN_STAIR";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcStairTypeEnum> IfcStairTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcStairTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcStairTypeEnum>(); }
	shared_ptr<IfcStairTypeEnum> type_object( new IfcStairTypeEnum() );
	if( boost::iequals( arg, L".STRAIGHT_RUN_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_STRAIGHT_RUN_STAIR;
	}
	else if( boost::iequals( arg, L".TWO_STRAIGHT_RUN_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_TWO_STRAIGHT_RUN_STAIR;
	}
	else if( boost::iequals( arg, L".QUARTER_WINDING_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_QUARTER_WINDING_STAIR;
	}
	else if( boost::iequals( arg, L".QUARTER_TURN_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_QUARTER_TURN_STAIR;
	}
	else if( boost::iequals( arg, L".HALF_WINDING_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_HALF_WINDING_STAIR;
	}
	else if( boost::iequals( arg, L".HALF_TURN_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_HALF_TURN_STAIR;
	}
	else if( boost::iequals( arg, L".TWO_QUARTER_WINDING_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_TWO_QUARTER_WINDING_STAIR;
	}
	else if( boost::iequals( arg, L".TWO_QUARTER_TURN_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_TWO_QUARTER_TURN_STAIR;
	}
	else if( boost::iequals( arg, L".THREE_QUARTER_WINDING_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_THREE_QUARTER_WINDING_STAIR;
	}
	else if( boost::iequals( arg, L".THREE_QUARTER_TURN_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_THREE_QUARTER_TURN_STAIR;
	}
	else if( boost::iequals( arg, L".SPIRAL_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_SPIRAL_STAIR;
	}
	else if( boost::iequals( arg, L".DOUBLE_RETURN_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_DOUBLE_RETURN_STAIR;
	}
	else if( boost::iequals( arg, L".CURVED_RUN_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_CURVED_RUN_STAIR;
	}
	else if( boost::iequals( arg, L".TWO_CURVED_RUN_STAIR." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_TWO_CURVED_RUN_STAIR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcStairTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
