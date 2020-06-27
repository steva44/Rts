/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcSanitaryTerminalTypeEnum = ENUMERATION OF	(BATH	,BIDET	,CISTERN	,SHOWER	,SINK	,SANITARYFOUNTAIN	,TOILETPAN	,URINAL	,WASHHANDBASIN	,WCSEAT	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcSanitaryTerminalTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcSanitaryTerminalTypeEnumEnum
	{
		ENUM_BATH,
		ENUM_BIDET,
		ENUM_CISTERN,
		ENUM_SHOWER,
		ENUM_SINK,
		ENUM_SANITARYFOUNTAIN,
		ENUM_TOILETPAN,
		ENUM_URINAL,
		ENUM_WASHHANDBASIN,
		ENUM_WCSEAT,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcSanitaryTerminalTypeEnum();
	IfcSanitaryTerminalTypeEnum( IfcSanitaryTerminalTypeEnumEnum e ) { m_enum = e; }
	~IfcSanitaryTerminalTypeEnum();
	virtual const char* className() const { return "IfcSanitaryTerminalTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcSanitaryTerminalTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcSanitaryTerminalTypeEnumEnum m_enum;
};

