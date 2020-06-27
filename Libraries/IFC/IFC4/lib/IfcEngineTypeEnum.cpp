/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcEngineTypeEnum.h"

// TYPE IfcEngineTypeEnum = ENUMERATION OF	(EXTERNALCOMBUSTION	,INTERNALCOMBUSTION	,USERDEFINED	,NOTDEFINED);
IfcEngineTypeEnum::IfcEngineTypeEnum() {}
IfcEngineTypeEnum::~IfcEngineTypeEnum() {}
shared_ptr<IfcPPObject> IfcEngineTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcEngineTypeEnum> copy_self( new IfcEngineTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcEngineTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCENGINETYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_EXTERNALCOMBUSTION:	stream << ".EXTERNALCOMBUSTION."; break;
		case ENUM_INTERNALCOMBUSTION:	stream << ".INTERNALCOMBUSTION."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcEngineTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_EXTERNALCOMBUSTION:	return L"EXTERNALCOMBUSTION";
		case ENUM_INTERNALCOMBUSTION:	return L"INTERNALCOMBUSTION";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcEngineTypeEnum> IfcEngineTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcEngineTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcEngineTypeEnum>(); }
	shared_ptr<IfcEngineTypeEnum> type_object( new IfcEngineTypeEnum() );
	if( boost::iequals( arg, L".EXTERNALCOMBUSTION." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_EXTERNALCOMBUSTION;
	}
	else if( boost::iequals( arg, L".INTERNALCOMBUSTION." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_INTERNALCOMBUSTION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
