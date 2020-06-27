/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcFireSuppressionTerminalTypeEnum = ENUMERATION OF	(BREECHINGINLET	,FIREHYDRANT	,HOSEREEL	,SPRINKLER	,SPRINKLERDEFLECTOR	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcFireSuppressionTerminalTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcFireSuppressionTerminalTypeEnumEnum
	{
		ENUM_BREECHINGINLET,
		ENUM_FIREHYDRANT,
		ENUM_HOSEREEL,
		ENUM_SPRINKLER,
		ENUM_SPRINKLERDEFLECTOR,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcFireSuppressionTerminalTypeEnum();
	IfcFireSuppressionTerminalTypeEnum( IfcFireSuppressionTerminalTypeEnumEnum e ) { m_enum = e; }
	~IfcFireSuppressionTerminalTypeEnum();
	virtual const char* className() const { return "IfcFireSuppressionTerminalTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcFireSuppressionTerminalTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcFireSuppressionTerminalTypeEnumEnum m_enum;
};
