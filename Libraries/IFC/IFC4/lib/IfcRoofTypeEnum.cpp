/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcRoofTypeEnum.h"

// TYPE IfcRoofTypeEnum = ENUMERATION OF	(FLAT_ROOF	,SHED_ROOF	,GABLE_ROOF	,HIP_ROOF	,HIPPED_GABLE_ROOF	,GAMBREL_ROOF	,MANSARD_ROOF	,BARREL_ROOF	,RAINBOW_ROOF	,BUTTERFLY_ROOF	,PAVILION_ROOF	,DOME_ROOF	,FREEFORM	,USERDEFINED	,NOTDEFINED);
IfcRoofTypeEnum::IfcRoofTypeEnum() {}
IfcRoofTypeEnum::~IfcRoofTypeEnum() {}
shared_ptr<IfcPPObject> IfcRoofTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRoofTypeEnum> copy_self( new IfcRoofTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcRoofTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCROOFTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_FLAT_ROOF:	stream << ".FLAT_ROOF."; break;
		case ENUM_SHED_ROOF:	stream << ".SHED_ROOF."; break;
		case ENUM_GABLE_ROOF:	stream << ".GABLE_ROOF."; break;
		case ENUM_HIP_ROOF:	stream << ".HIP_ROOF."; break;
		case ENUM_HIPPED_GABLE_ROOF:	stream << ".HIPPED_GABLE_ROOF."; break;
		case ENUM_GAMBREL_ROOF:	stream << ".GAMBREL_ROOF."; break;
		case ENUM_MANSARD_ROOF:	stream << ".MANSARD_ROOF."; break;
		case ENUM_BARREL_ROOF:	stream << ".BARREL_ROOF."; break;
		case ENUM_RAINBOW_ROOF:	stream << ".RAINBOW_ROOF."; break;
		case ENUM_BUTTERFLY_ROOF:	stream << ".BUTTERFLY_ROOF."; break;
		case ENUM_PAVILION_ROOF:	stream << ".PAVILION_ROOF."; break;
		case ENUM_DOME_ROOF:	stream << ".DOME_ROOF."; break;
		case ENUM_FREEFORM:	stream << ".FREEFORM."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcRoofTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_FLAT_ROOF:	return L"FLAT_ROOF";
		case ENUM_SHED_ROOF:	return L"SHED_ROOF";
		case ENUM_GABLE_ROOF:	return L"GABLE_ROOF";
		case ENUM_HIP_ROOF:	return L"HIP_ROOF";
		case ENUM_HIPPED_GABLE_ROOF:	return L"HIPPED_GABLE_ROOF";
		case ENUM_GAMBREL_ROOF:	return L"GAMBREL_ROOF";
		case ENUM_MANSARD_ROOF:	return L"MANSARD_ROOF";
		case ENUM_BARREL_ROOF:	return L"BARREL_ROOF";
		case ENUM_RAINBOW_ROOF:	return L"RAINBOW_ROOF";
		case ENUM_BUTTERFLY_ROOF:	return L"BUTTERFLY_ROOF";
		case ENUM_PAVILION_ROOF:	return L"PAVILION_ROOF";
		case ENUM_DOME_ROOF:	return L"DOME_ROOF";
		case ENUM_FREEFORM:	return L"FREEFORM";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcRoofTypeEnum> IfcRoofTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcRoofTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcRoofTypeEnum>(); }
	shared_ptr<IfcRoofTypeEnum> type_object( new IfcRoofTypeEnum() );
	if( boost::iequals( arg, L".FLAT_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_FLAT_ROOF;
	}
	else if( boost::iequals( arg, L".SHED_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_SHED_ROOF;
	}
	else if( boost::iequals( arg, L".GABLE_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_GABLE_ROOF;
	}
	else if( boost::iequals( arg, L".HIP_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_HIP_ROOF;
	}
	else if( boost::iequals( arg, L".HIPPED_GABLE_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_HIPPED_GABLE_ROOF;
	}
	else if( boost::iequals( arg, L".GAMBREL_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_GAMBREL_ROOF;
	}
	else if( boost::iequals( arg, L".MANSARD_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_MANSARD_ROOF;
	}
	else if( boost::iequals( arg, L".BARREL_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_BARREL_ROOF;
	}
	else if( boost::iequals( arg, L".RAINBOW_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_RAINBOW_ROOF;
	}
	else if( boost::iequals( arg, L".BUTTERFLY_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_BUTTERFLY_ROOF;
	}
	else if( boost::iequals( arg, L".PAVILION_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_PAVILION_ROOF;
	}
	else if( boost::iequals( arg, L".DOME_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_DOME_ROOF;
	}
	else if( boost::iequals( arg, L".FREEFORM." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_FREEFORM;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
