/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcWorkPlanTypeEnum = ENUMERATION OF	(ACTUAL	,BASELINE	,PLANNED	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcWorkPlanTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcWorkPlanTypeEnumEnum
	{
		ENUM_ACTUAL,
		ENUM_BASELINE,
		ENUM_PLANNED,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcWorkPlanTypeEnum();
	IfcWorkPlanTypeEnum( IfcWorkPlanTypeEnumEnum e ) { m_enum = e; }
	~IfcWorkPlanTypeEnum();
	virtual const char* className() const { return "IfcWorkPlanTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcWorkPlanTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcWorkPlanTypeEnumEnum m_enum;
};
