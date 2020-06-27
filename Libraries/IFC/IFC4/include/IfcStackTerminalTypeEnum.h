/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcStackTerminalTypeEnum = ENUMERATION OF	(BIRDCAGE	,COWL	,RAINWATERHOPPER	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcStackTerminalTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcStackTerminalTypeEnumEnum
	{
		ENUM_BIRDCAGE,
		ENUM_COWL,
		ENUM_RAINWATERHOPPER,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcStackTerminalTypeEnum();
	IfcStackTerminalTypeEnum( IfcStackTerminalTypeEnumEnum e ) { m_enum = e; }
	~IfcStackTerminalTypeEnum();
	virtual const char* className() const { return "IfcStackTerminalTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcStackTerminalTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcStackTerminalTypeEnumEnum m_enum;
};

