/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcWorkScheduleTypeEnum = ENUMERATION OF	(ACTUAL	,BASELINE	,PLANNED	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcWorkScheduleTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcWorkScheduleTypeEnumEnum
	{
		ENUM_ACTUAL,
		ENUM_BASELINE,
		ENUM_PLANNED,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcWorkScheduleTypeEnum();
	IfcWorkScheduleTypeEnum( IfcWorkScheduleTypeEnumEnum e ) { m_enum = e; }
	~IfcWorkScheduleTypeEnum();
	virtual const char* className() const { return "IfcWorkScheduleTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcWorkScheduleTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcWorkScheduleTypeEnumEnum m_enum;
};
