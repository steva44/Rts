/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcStructuralSurfaceMemberTypeEnum.h"

// TYPE IfcStructuralSurfaceMemberTypeEnum = ENUMERATION OF	(BENDING_ELEMENT	,MEMBRANE_ELEMENT	,SHELL	,USERDEFINED	,NOTDEFINED);
IfcStructuralSurfaceMemberTypeEnum::IfcStructuralSurfaceMemberTypeEnum() {}
IfcStructuralSurfaceMemberTypeEnum::~IfcStructuralSurfaceMemberTypeEnum() {}
shared_ptr<IfcPPObject> IfcStructuralSurfaceMemberTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStructuralSurfaceMemberTypeEnum> copy_self( new IfcStructuralSurfaceMemberTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcStructuralSurfaceMemberTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSTRUCTURALSURFACEMEMBERTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_BENDING_ELEMENT:	stream << ".BENDING_ELEMENT."; break;
		case ENUM_MEMBRANE_ELEMENT:	stream << ".MEMBRANE_ELEMENT."; break;
		case ENUM_SHELL:	stream << ".SHELL."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcStructuralSurfaceMemberTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_BENDING_ELEMENT:	return L"BENDING_ELEMENT";
		case ENUM_MEMBRANE_ELEMENT:	return L"MEMBRANE_ELEMENT";
		case ENUM_SHELL:	return L"SHELL";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcStructuralSurfaceMemberTypeEnum> IfcStructuralSurfaceMemberTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcStructuralSurfaceMemberTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcStructuralSurfaceMemberTypeEnum>(); }
	shared_ptr<IfcStructuralSurfaceMemberTypeEnum> type_object( new IfcStructuralSurfaceMemberTypeEnum() );
	if( boost::iequals( arg, L".BENDING_ELEMENT." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceMemberTypeEnum::ENUM_BENDING_ELEMENT;
	}
	else if( boost::iequals( arg, L".MEMBRANE_ELEMENT." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceMemberTypeEnum::ENUM_MEMBRANE_ELEMENT;
	}
	else if( boost::iequals( arg, L".SHELL." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceMemberTypeEnum::ENUM_SHELL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceMemberTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcStructuralSurfaceMemberTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}