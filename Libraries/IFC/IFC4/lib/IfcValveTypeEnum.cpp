/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcValveTypeEnum.h"

// TYPE IfcValveTypeEnum = ENUMERATION OF	(AIRRELEASE	,ANTIVACUUM	,CHANGEOVER	,CHECK	,COMMISSIONING	,DIVERTING	,DRAWOFFCOCK	,DOUBLECHECK	,DOUBLEREGULATING	,FAUCET	,FLUSHING	,GASCOCK	,GASTAP	,ISOLATING	,MIXING	,PRESSUREREDUCING	,PRESSURERELIEF	,REGULATING	,SAFETYCUTOFF	,STEAMTRAP	,STOPCOCK	,USERDEFINED	,NOTDEFINED);
IfcValveTypeEnum::IfcValveTypeEnum() {}
IfcValveTypeEnum::~IfcValveTypeEnum() {}
shared_ptr<IfcPPObject> IfcValveTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcValveTypeEnum> copy_self( new IfcValveTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcValveTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCVALVETYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_AIRRELEASE:	stream << ".AIRRELEASE."; break;
		case ENUM_ANTIVACUUM:	stream << ".ANTIVACUUM."; break;
		case ENUM_CHANGEOVER:	stream << ".CHANGEOVER."; break;
		case ENUM_CHECK:	stream << ".CHECK."; break;
		case ENUM_COMMISSIONING:	stream << ".COMMISSIONING."; break;
		case ENUM_DIVERTING:	stream << ".DIVERTING."; break;
		case ENUM_DRAWOFFCOCK:	stream << ".DRAWOFFCOCK."; break;
		case ENUM_DOUBLECHECK:	stream << ".DOUBLECHECK."; break;
		case ENUM_DOUBLEREGULATING:	stream << ".DOUBLEREGULATING."; break;
		case ENUM_FAUCET:	stream << ".FAUCET."; break;
		case ENUM_FLUSHING:	stream << ".FLUSHING."; break;
		case ENUM_GASCOCK:	stream << ".GASCOCK."; break;
		case ENUM_GASTAP:	stream << ".GASTAP."; break;
		case ENUM_ISOLATING:	stream << ".ISOLATING."; break;
		case ENUM_MIXING:	stream << ".MIXING."; break;
		case ENUM_PRESSUREREDUCING:	stream << ".PRESSUREREDUCING."; break;
		case ENUM_PRESSURERELIEF:	stream << ".PRESSURERELIEF."; break;
		case ENUM_REGULATING:	stream << ".REGULATING."; break;
		case ENUM_SAFETYCUTOFF:	stream << ".SAFETYCUTOFF."; break;
		case ENUM_STEAMTRAP:	stream << ".STEAMTRAP."; break;
		case ENUM_STOPCOCK:	stream << ".STOPCOCK."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcValveTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_AIRRELEASE:	return L"AIRRELEASE";
		case ENUM_ANTIVACUUM:	return L"ANTIVACUUM";
		case ENUM_CHANGEOVER:	return L"CHANGEOVER";
		case ENUM_CHECK:	return L"CHECK";
		case ENUM_COMMISSIONING:	return L"COMMISSIONING";
		case ENUM_DIVERTING:	return L"DIVERTING";
		case ENUM_DRAWOFFCOCK:	return L"DRAWOFFCOCK";
		case ENUM_DOUBLECHECK:	return L"DOUBLECHECK";
		case ENUM_DOUBLEREGULATING:	return L"DOUBLEREGULATING";
		case ENUM_FAUCET:	return L"FAUCET";
		case ENUM_FLUSHING:	return L"FLUSHING";
		case ENUM_GASCOCK:	return L"GASCOCK";
		case ENUM_GASTAP:	return L"GASTAP";
		case ENUM_ISOLATING:	return L"ISOLATING";
		case ENUM_MIXING:	return L"MIXING";
		case ENUM_PRESSUREREDUCING:	return L"PRESSUREREDUCING";
		case ENUM_PRESSURERELIEF:	return L"PRESSURERELIEF";
		case ENUM_REGULATING:	return L"REGULATING";
		case ENUM_SAFETYCUTOFF:	return L"SAFETYCUTOFF";
		case ENUM_STEAMTRAP:	return L"STEAMTRAP";
		case ENUM_STOPCOCK:	return L"STOPCOCK";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcValveTypeEnum> IfcValveTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcValveTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcValveTypeEnum>(); }
	shared_ptr<IfcValveTypeEnum> type_object( new IfcValveTypeEnum() );
	if( boost::iequals( arg, L".AIRRELEASE." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_AIRRELEASE;
	}
	else if( boost::iequals( arg, L".ANTIVACUUM." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_ANTIVACUUM;
	}
	else if( boost::iequals( arg, L".CHANGEOVER." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_CHANGEOVER;
	}
	else if( boost::iequals( arg, L".CHECK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_CHECK;
	}
	else if( boost::iequals( arg, L".COMMISSIONING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_COMMISSIONING;
	}
	else if( boost::iequals( arg, L".DIVERTING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DIVERTING;
	}
	else if( boost::iequals( arg, L".DRAWOFFCOCK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DRAWOFFCOCK;
	}
	else if( boost::iequals( arg, L".DOUBLECHECK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DOUBLECHECK;
	}
	else if( boost::iequals( arg, L".DOUBLEREGULATING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DOUBLEREGULATING;
	}
	else if( boost::iequals( arg, L".FAUCET." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_FAUCET;
	}
	else if( boost::iequals( arg, L".FLUSHING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_FLUSHING;
	}
	else if( boost::iequals( arg, L".GASCOCK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_GASCOCK;
	}
	else if( boost::iequals( arg, L".GASTAP." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_GASTAP;
	}
	else if( boost::iequals( arg, L".ISOLATING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_ISOLATING;
	}
	else if( boost::iequals( arg, L".MIXING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_MIXING;
	}
	else if( boost::iequals( arg, L".PRESSUREREDUCING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_PRESSUREREDUCING;
	}
	else if( boost::iequals( arg, L".PRESSURERELIEF." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_PRESSURERELIEF;
	}
	else if( boost::iequals( arg, L".REGULATING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_REGULATING;
	}
	else if( boost::iequals( arg, L".SAFETYCUTOFF." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_SAFETYCUTOFF;
	}
	else if( boost::iequals( arg, L".STEAMTRAP." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_STEAMTRAP;
	}
	else if( boost::iequals( arg, L".STOPCOCK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_STOPCOCK;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
