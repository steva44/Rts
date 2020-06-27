/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcRampFlightTypeEnum.h"

// TYPE IfcRampFlightTypeEnum = ENUMERATION OF	(STRAIGHT	,SPIRAL	,USERDEFINED	,NOTDEFINED);
IfcRampFlightTypeEnum::IfcRampFlightTypeEnum() {}
IfcRampFlightTypeEnum::~IfcRampFlightTypeEnum() {}
shared_ptr<IfcPPObject> IfcRampFlightTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRampFlightTypeEnum> copy_self( new IfcRampFlightTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcRampFlightTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCRAMPFLIGHTTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_STRAIGHT:	stream << ".STRAIGHT."; break;
		case ENUM_SPIRAL:	stream << ".SPIRAL."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcRampFlightTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_STRAIGHT:	return L"STRAIGHT";
		case ENUM_SPIRAL:	return L"SPIRAL";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcRampFlightTypeEnum> IfcRampFlightTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcRampFlightTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcRampFlightTypeEnum>(); }
	shared_ptr<IfcRampFlightTypeEnum> type_object( new IfcRampFlightTypeEnum() );
	if( boost::iequals( arg, L".STRAIGHT." ) )
	{
		type_object->m_enum = IfcRampFlightTypeEnum::ENUM_STRAIGHT;
	}
	else if( boost::iequals( arg, L".SPIRAL." ) )
	{
		type_object->m_enum = IfcRampFlightTypeEnum::ENUM_SPIRAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcRampFlightTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcRampFlightTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}