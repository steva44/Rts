/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcAirTerminalTypeEnum = ENUMERATION OF	(DIFFUSER	,GRILLE	,LOUVRE	,REGISTER	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcAirTerminalTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcAirTerminalTypeEnumEnum
	{
		ENUM_DIFFUSER,
		ENUM_GRILLE,
		ENUM_LOUVRE,
		ENUM_REGISTER,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcAirTerminalTypeEnum();
	IfcAirTerminalTypeEnum( IfcAirTerminalTypeEnumEnum e ) { m_enum = e; }
	~IfcAirTerminalTypeEnum();
	virtual const char* className() const { return "IfcAirTerminalTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcAirTerminalTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcAirTerminalTypeEnumEnum m_enum;
};

