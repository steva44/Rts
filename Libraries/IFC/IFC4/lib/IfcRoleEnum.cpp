/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcRoleEnum.h"

// TYPE IfcRoleEnum = ENUMERATION OF	(SUPPLIER	,MANUFACTURER	,CONTRACTOR	,SUBCONTRACTOR	,ARCHITECT	,STRUCTURALENGINEER	,COSTENGINEER	,CLIENT	,BUILDINGOWNER	,BUILDINGOPERATOR	,MECHANICALENGINEER	,ELECTRICALENGINEER	,PROJECTMANAGER	,FACILITIESMANAGER	,CIVILENGINEER	,COMMISSIONINGENGINEER	,ENGINEER	,OWNER	,CONSULTANT	,CONSTRUCTIONMANAGER	,FIELDCONSTRUCTIONMANAGER	,RESELLER	,USERDEFINED);
IfcRoleEnum::IfcRoleEnum() {}
IfcRoleEnum::~IfcRoleEnum() {}
shared_ptr<IfcPPObject> IfcRoleEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRoleEnum> copy_self( new IfcRoleEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcRoleEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCROLEENUM("; }
	switch( m_enum )
	{
		case ENUM_SUPPLIER:	stream << ".SUPPLIER."; break;
		case ENUM_MANUFACTURER:	stream << ".MANUFACTURER."; break;
		case ENUM_CONTRACTOR:	stream << ".CONTRACTOR."; break;
		case ENUM_SUBCONTRACTOR:	stream << ".SUBCONTRACTOR."; break;
		case ENUM_ARCHITECT:	stream << ".ARCHITECT."; break;
		case ENUM_STRUCTURALENGINEER:	stream << ".STRUCTURALENGINEER."; break;
		case ENUM_COSTENGINEER:	stream << ".COSTENGINEER."; break;
		case ENUM_CLIENT:	stream << ".CLIENT."; break;
		case ENUM_BUILDINGOWNER:	stream << ".BUILDINGOWNER."; break;
		case ENUM_BUILDINGOPERATOR:	stream << ".BUILDINGOPERATOR."; break;
		case ENUM_MECHANICALENGINEER:	stream << ".MECHANICALENGINEER."; break;
		case ENUM_ELECTRICALENGINEER:	stream << ".ELECTRICALENGINEER."; break;
		case ENUM_PROJECTMANAGER:	stream << ".PROJECTMANAGER."; break;
		case ENUM_FACILITIESMANAGER:	stream << ".FACILITIESMANAGER."; break;
		case ENUM_CIVILENGINEER:	stream << ".CIVILENGINEER."; break;
		case ENUM_COMMISSIONINGENGINEER:	stream << ".COMMISSIONINGENGINEER."; break;
		case ENUM_ENGINEER:	stream << ".ENGINEER."; break;
		case ENUM_OWNER:	stream << ".OWNER."; break;
		case ENUM_CONSULTANT:	stream << ".CONSULTANT."; break;
		case ENUM_CONSTRUCTIONMANAGER:	stream << ".CONSTRUCTIONMANAGER."; break;
		case ENUM_FIELDCONSTRUCTIONMANAGER:	stream << ".FIELDCONSTRUCTIONMANAGER."; break;
		case ENUM_RESELLER:	stream << ".RESELLER."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcRoleEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_SUPPLIER:	return L"SUPPLIER";
		case ENUM_MANUFACTURER:	return L"MANUFACTURER";
		case ENUM_CONTRACTOR:	return L"CONTRACTOR";
		case ENUM_SUBCONTRACTOR:	return L"SUBCONTRACTOR";
		case ENUM_ARCHITECT:	return L"ARCHITECT";
		case ENUM_STRUCTURALENGINEER:	return L"STRUCTURALENGINEER";
		case ENUM_COSTENGINEER:	return L"COSTENGINEER";
		case ENUM_CLIENT:	return L"CLIENT";
		case ENUM_BUILDINGOWNER:	return L"BUILDINGOWNER";
		case ENUM_BUILDINGOPERATOR:	return L"BUILDINGOPERATOR";
		case ENUM_MECHANICALENGINEER:	return L"MECHANICALENGINEER";
		case ENUM_ELECTRICALENGINEER:	return L"ELECTRICALENGINEER";
		case ENUM_PROJECTMANAGER:	return L"PROJECTMANAGER";
		case ENUM_FACILITIESMANAGER:	return L"FACILITIESMANAGER";
		case ENUM_CIVILENGINEER:	return L"CIVILENGINEER";
		case ENUM_COMMISSIONINGENGINEER:	return L"COMMISSIONINGENGINEER";
		case ENUM_ENGINEER:	return L"ENGINEER";
		case ENUM_OWNER:	return L"OWNER";
		case ENUM_CONSULTANT:	return L"CONSULTANT";
		case ENUM_CONSTRUCTIONMANAGER:	return L"CONSTRUCTIONMANAGER";
		case ENUM_FIELDCONSTRUCTIONMANAGER:	return L"FIELDCONSTRUCTIONMANAGER";
		case ENUM_RESELLER:	return L"RESELLER";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
	}
	return L"";
}
shared_ptr<IfcRoleEnum> IfcRoleEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcRoleEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcRoleEnum>(); }
	shared_ptr<IfcRoleEnum> type_object( new IfcRoleEnum() );
	if( boost::iequals( arg, L".SUPPLIER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_SUPPLIER;
	}
	else if( boost::iequals( arg, L".MANUFACTURER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_MANUFACTURER;
	}
	else if( boost::iequals( arg, L".CONTRACTOR." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_CONTRACTOR;
	}
	else if( boost::iequals( arg, L".SUBCONTRACTOR." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_SUBCONTRACTOR;
	}
	else if( boost::iequals( arg, L".ARCHITECT." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_ARCHITECT;
	}
	else if( boost::iequals( arg, L".STRUCTURALENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_STRUCTURALENGINEER;
	}
	else if( boost::iequals( arg, L".COSTENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_COSTENGINEER;
	}
	else if( boost::iequals( arg, L".CLIENT." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_CLIENT;
	}
	else if( boost::iequals( arg, L".BUILDINGOWNER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_BUILDINGOWNER;
	}
	else if( boost::iequals( arg, L".BUILDINGOPERATOR." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_BUILDINGOPERATOR;
	}
	else if( boost::iequals( arg, L".MECHANICALENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_MECHANICALENGINEER;
	}
	else if( boost::iequals( arg, L".ELECTRICALENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_ELECTRICALENGINEER;
	}
	else if( boost::iequals( arg, L".PROJECTMANAGER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_PROJECTMANAGER;
	}
	else if( boost::iequals( arg, L".FACILITIESMANAGER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_FACILITIESMANAGER;
	}
	else if( boost::iequals( arg, L".CIVILENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_CIVILENGINEER;
	}
	else if( boost::iequals( arg, L".COMMISSIONINGENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_COMMISSIONINGENGINEER;
	}
	else if( boost::iequals( arg, L".ENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_ENGINEER;
	}
	else if( boost::iequals( arg, L".OWNER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_OWNER;
	}
	else if( boost::iequals( arg, L".CONSULTANT." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_CONSULTANT;
	}
	else if( boost::iequals( arg, L".CONSTRUCTIONMANAGER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_CONSTRUCTIONMANAGER;
	}
	else if( boost::iequals( arg, L".FIELDCONSTRUCTIONMANAGER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_FIELDCONSTRUCTIONMANAGER;
	}
	else if( boost::iequals( arg, L".RESELLER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_RESELLER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_USERDEFINED;
	}
	return type_object;
}