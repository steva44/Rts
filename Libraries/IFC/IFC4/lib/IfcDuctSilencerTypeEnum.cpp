/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDuctSilencerTypeEnum.h"

// TYPE IfcDuctSilencerTypeEnum = ENUMERATION OF	(FLATOVAL	,RECTANGULAR	,ROUND	,USERDEFINED	,NOTDEFINED);
IfcDuctSilencerTypeEnum::IfcDuctSilencerTypeEnum() {}
IfcDuctSilencerTypeEnum::~IfcDuctSilencerTypeEnum() {}
shared_ptr<IfcPPObject> IfcDuctSilencerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDuctSilencerTypeEnum> copy_self( new IfcDuctSilencerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDuctSilencerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDUCTSILENCERTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_FLATOVAL:	stream << ".FLATOVAL."; break;
		case ENUM_RECTANGULAR:	stream << ".RECTANGULAR."; break;
		case ENUM_ROUND:	stream << ".ROUND."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcDuctSilencerTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_FLATOVAL:	return L"FLATOVAL";
		case ENUM_RECTANGULAR:	return L"RECTANGULAR";
		case ENUM_ROUND:	return L"ROUND";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcDuctSilencerTypeEnum> IfcDuctSilencerTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDuctSilencerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDuctSilencerTypeEnum>(); }
	shared_ptr<IfcDuctSilencerTypeEnum> type_object( new IfcDuctSilencerTypeEnum() );
	if( boost::iequals( arg, L".FLATOVAL." ) )
	{
		type_object->m_enum = IfcDuctSilencerTypeEnum::ENUM_FLATOVAL;
	}
	else if( boost::iequals( arg, L".RECTANGULAR." ) )
	{
		type_object->m_enum = IfcDuctSilencerTypeEnum::ENUM_RECTANGULAR;
	}
	else if( boost::iequals( arg, L".ROUND." ) )
	{
		type_object->m_enum = IfcDuctSilencerTypeEnum::ENUM_ROUND;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDuctSilencerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDuctSilencerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}