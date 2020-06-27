/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcHeatExchangerTypeEnum.h"

// TYPE IfcHeatExchangerTypeEnum = ENUMERATION OF	(PLATE	,SHELLANDTUBE	,USERDEFINED	,NOTDEFINED);
IfcHeatExchangerTypeEnum::IfcHeatExchangerTypeEnum() {}
IfcHeatExchangerTypeEnum::~IfcHeatExchangerTypeEnum() {}
shared_ptr<IfcPPObject> IfcHeatExchangerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcHeatExchangerTypeEnum> copy_self( new IfcHeatExchangerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcHeatExchangerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCHEATEXCHANGERTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_PLATE:	stream << ".PLATE."; break;
		case ENUM_SHELLANDTUBE:	stream << ".SHELLANDTUBE."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcHeatExchangerTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_PLATE:	return L"PLATE";
		case ENUM_SHELLANDTUBE:	return L"SHELLANDTUBE";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcHeatExchangerTypeEnum> IfcHeatExchangerTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcHeatExchangerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcHeatExchangerTypeEnum>(); }
	shared_ptr<IfcHeatExchangerTypeEnum> type_object( new IfcHeatExchangerTypeEnum() );
	if( boost::iequals( arg, L".PLATE." ) )
	{
		type_object->m_enum = IfcHeatExchangerTypeEnum::ENUM_PLATE;
	}
	else if( boost::iequals( arg, L".SHELLANDTUBE." ) )
	{
		type_object->m_enum = IfcHeatExchangerTypeEnum::ENUM_SHELLANDTUBE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcHeatExchangerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcHeatExchangerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
