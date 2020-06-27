/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcBeamTypeEnum.h"

// TYPE IfcBeamTypeEnum = ENUMERATION OF	(BEAM	,JOIST	,HOLLOWCORE	,LINTEL	,SPANDREL	,T_BEAM	,USERDEFINED	,NOTDEFINED);
IfcBeamTypeEnum::IfcBeamTypeEnum() {}
IfcBeamTypeEnum::~IfcBeamTypeEnum() {}
shared_ptr<IfcPPObject> IfcBeamTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBeamTypeEnum> copy_self( new IfcBeamTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBeamTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBEAMTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_BEAM:	stream << ".BEAM."; break;
		case ENUM_JOIST:	stream << ".JOIST."; break;
		case ENUM_HOLLOWCORE:	stream << ".HOLLOWCORE."; break;
		case ENUM_LINTEL:	stream << ".LINTEL."; break;
		case ENUM_SPANDREL:	stream << ".SPANDREL."; break;
		case ENUM_T_BEAM:	stream << ".T_BEAM."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcBeamTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_BEAM:	return L"BEAM";
		case ENUM_JOIST:	return L"JOIST";
		case ENUM_HOLLOWCORE:	return L"HOLLOWCORE";
		case ENUM_LINTEL:	return L"LINTEL";
		case ENUM_SPANDREL:	return L"SPANDREL";
		case ENUM_T_BEAM:	return L"T_BEAM";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcBeamTypeEnum> IfcBeamTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBeamTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBeamTypeEnum>(); }
	shared_ptr<IfcBeamTypeEnum> type_object( new IfcBeamTypeEnum() );
	if( boost::iequals( arg, L".BEAM." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_BEAM;
	}
	else if( boost::iequals( arg, L".JOIST." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_JOIST;
	}
	else if( boost::iequals( arg, L".HOLLOWCORE." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_HOLLOWCORE;
	}
	else if( boost::iequals( arg, L".LINTEL." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_LINTEL;
	}
	else if( boost::iequals( arg, L".SPANDREL." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_SPANDREL;
	}
	else if( boost::iequals( arg, L".T_BEAM." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_T_BEAM;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
