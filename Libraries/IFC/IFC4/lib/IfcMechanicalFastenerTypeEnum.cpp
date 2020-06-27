/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcMechanicalFastenerTypeEnum.h"

// TYPE IfcMechanicalFastenerTypeEnum = ENUMERATION OF	(ANCHORBOLT	,BOLT	,DOWEL	,NAIL	,NAILPLATE	,RIVET	,SCREW	,SHEARCONNECTOR	,STAPLE	,STUDSHEARCONNECTOR	,USERDEFINED	,NOTDEFINED);
IfcMechanicalFastenerTypeEnum::IfcMechanicalFastenerTypeEnum() {}
IfcMechanicalFastenerTypeEnum::~IfcMechanicalFastenerTypeEnum() {}
shared_ptr<IfcPPObject> IfcMechanicalFastenerTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMechanicalFastenerTypeEnum> copy_self( new IfcMechanicalFastenerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcMechanicalFastenerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCMECHANICALFASTENERTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_ANCHORBOLT:	stream << ".ANCHORBOLT."; break;
		case ENUM_BOLT:	stream << ".BOLT."; break;
		case ENUM_DOWEL:	stream << ".DOWEL."; break;
		case ENUM_NAIL:	stream << ".NAIL."; break;
		case ENUM_NAILPLATE:	stream << ".NAILPLATE."; break;
		case ENUM_RIVET:	stream << ".RIVET."; break;
		case ENUM_SCREW:	stream << ".SCREW."; break;
		case ENUM_SHEARCONNECTOR:	stream << ".SHEARCONNECTOR."; break;
		case ENUM_STAPLE:	stream << ".STAPLE."; break;
		case ENUM_STUDSHEARCONNECTOR:	stream << ".STUDSHEARCONNECTOR."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcMechanicalFastenerTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_ANCHORBOLT:	return L"ANCHORBOLT";
		case ENUM_BOLT:	return L"BOLT";
		case ENUM_DOWEL:	return L"DOWEL";
		case ENUM_NAIL:	return L"NAIL";
		case ENUM_NAILPLATE:	return L"NAILPLATE";
		case ENUM_RIVET:	return L"RIVET";
		case ENUM_SCREW:	return L"SCREW";
		case ENUM_SHEARCONNECTOR:	return L"SHEARCONNECTOR";
		case ENUM_STAPLE:	return L"STAPLE";
		case ENUM_STUDSHEARCONNECTOR:	return L"STUDSHEARCONNECTOR";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcMechanicalFastenerTypeEnum> IfcMechanicalFastenerTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcMechanicalFastenerTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcMechanicalFastenerTypeEnum>(); }
	shared_ptr<IfcMechanicalFastenerTypeEnum> type_object( new IfcMechanicalFastenerTypeEnum() );
	if( boost::iequals( arg, L".ANCHORBOLT." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_ANCHORBOLT;
	}
	else if( boost::iequals( arg, L".BOLT." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_BOLT;
	}
	else if( boost::iequals( arg, L".DOWEL." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_DOWEL;
	}
	else if( boost::iequals( arg, L".NAIL." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_NAIL;
	}
	else if( boost::iequals( arg, L".NAILPLATE." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_NAILPLATE;
	}
	else if( boost::iequals( arg, L".RIVET." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_RIVET;
	}
	else if( boost::iequals( arg, L".SCREW." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_SCREW;
	}
	else if( boost::iequals( arg, L".SHEARCONNECTOR." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_SHEARCONNECTOR;
	}
	else if( boost::iequals( arg, L".STAPLE." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_STAPLE;
	}
	else if( boost::iequals( arg, L".STUDSHEARCONNECTOR." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_STUDSHEARCONNECTOR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcMechanicalFastenerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}