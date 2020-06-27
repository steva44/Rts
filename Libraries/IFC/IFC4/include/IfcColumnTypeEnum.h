/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcColumnTypeEnum = ENUMERATION OF	(COLUMN	,PILASTER	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcColumnTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcColumnTypeEnumEnum
	{
		ENUM_COLUMN,
		ENUM_PILASTER,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcColumnTypeEnum();
	IfcColumnTypeEnum( IfcColumnTypeEnumEnum e ) { m_enum = e; }
	~IfcColumnTypeEnum();
	virtual const char* className() const { return "IfcColumnTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcColumnTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcColumnTypeEnumEnum m_enum;
};
