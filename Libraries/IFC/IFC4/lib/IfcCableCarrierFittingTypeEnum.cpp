/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcCableCarrierFittingTypeEnum.h"

// TYPE IfcCableCarrierFittingTypeEnum = ENUMERATION OF	(BEND	,CROSS	,REDUCER	,TEE	,USERDEFINED	,NOTDEFINED);
IfcCableCarrierFittingTypeEnum::IfcCableCarrierFittingTypeEnum() {}
IfcCableCarrierFittingTypeEnum::~IfcCableCarrierFittingTypeEnum() {}
shared_ptr<IfcPPObject> IfcCableCarrierFittingTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCableCarrierFittingTypeEnum> copy_self( new IfcCableCarrierFittingTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCableCarrierFittingTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCABLECARRIERFITTINGTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_BEND:	stream << ".BEND."; break;
		case ENUM_CROSS:	stream << ".CROSS."; break;
		case ENUM_REDUCER:	stream << ".REDUCER."; break;
		case ENUM_TEE:	stream << ".TEE."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcCableCarrierFittingTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_BEND:	return L"BEND";
		case ENUM_CROSS:	return L"CROSS";
		case ENUM_REDUCER:	return L"REDUCER";
		case ENUM_TEE:	return L"TEE";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcCableCarrierFittingTypeEnum> IfcCableCarrierFittingTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCableCarrierFittingTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCableCarrierFittingTypeEnum>(); }
	shared_ptr<IfcCableCarrierFittingTypeEnum> type_object( new IfcCableCarrierFittingTypeEnum() );
	if( boost::iequals( arg, L".BEND." ) )
	{
		type_object->m_enum = IfcCableCarrierFittingTypeEnum::ENUM_BEND;
	}
	else if( boost::iequals( arg, L".CROSS." ) )
	{
		type_object->m_enum = IfcCableCarrierFittingTypeEnum::ENUM_CROSS;
	}
	else if( boost::iequals( arg, L".REDUCER." ) )
	{
		type_object->m_enum = IfcCableCarrierFittingTypeEnum::ENUM_REDUCER;
	}
	else if( boost::iequals( arg, L".TEE." ) )
	{
		type_object->m_enum = IfcCableCarrierFittingTypeEnum::ENUM_TEE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCableCarrierFittingTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCableCarrierFittingTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
