/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcReinforcingBarSurfaceEnum.h"

// TYPE IfcReinforcingBarSurfaceEnum = ENUMERATION OF	(PLAIN	,TEXTURED);
IfcReinforcingBarSurfaceEnum::IfcReinforcingBarSurfaceEnum() {}
IfcReinforcingBarSurfaceEnum::~IfcReinforcingBarSurfaceEnum() {}
shared_ptr<IfcPPObject> IfcReinforcingBarSurfaceEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcReinforcingBarSurfaceEnum> copy_self( new IfcReinforcingBarSurfaceEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcReinforcingBarSurfaceEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCREINFORCINGBARSURFACEENUM("; }
	switch( m_enum )
	{
		case ENUM_PLAIN:	stream << ".PLAIN."; break;
		case ENUM_TEXTURED:	stream << ".TEXTURED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcReinforcingBarSurfaceEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_PLAIN:	return L"PLAIN";
		case ENUM_TEXTURED:	return L"TEXTURED";
	}
	return L"";
}
shared_ptr<IfcReinforcingBarSurfaceEnum> IfcReinforcingBarSurfaceEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcReinforcingBarSurfaceEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcReinforcingBarSurfaceEnum>(); }
	shared_ptr<IfcReinforcingBarSurfaceEnum> type_object( new IfcReinforcingBarSurfaceEnum() );
	if( boost::iequals( arg, L".PLAIN." ) )
	{
		type_object->m_enum = IfcReinforcingBarSurfaceEnum::ENUM_PLAIN;
	}
	else if( boost::iequals( arg, L".TEXTURED." ) )
	{
		type_object->m_enum = IfcReinforcingBarSurfaceEnum::ENUM_TEXTURED;
	}
	return type_object;
}
