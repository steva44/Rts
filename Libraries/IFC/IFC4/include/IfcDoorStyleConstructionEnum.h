/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcDoorStyleConstructionEnum = ENUMERATION OF	(ALUMINIUM	,HIGH_GRADE_STEEL	,STEEL	,WOOD	,ALUMINIUM_WOOD	,ALUMINIUM_PLASTIC	,PLASTIC	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcDoorStyleConstructionEnum : virtual public IfcPPObject
{
public:
	enum IfcDoorStyleConstructionEnumEnum
	{
		ENUM_ALUMINIUM,
		ENUM_HIGH_GRADE_STEEL,
		ENUM_STEEL,
		ENUM_WOOD,
		ENUM_ALUMINIUM_WOOD,
		ENUM_ALUMINIUM_PLASTIC,
		ENUM_PLASTIC,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcDoorStyleConstructionEnum();
	IfcDoorStyleConstructionEnum( IfcDoorStyleConstructionEnumEnum e ) { m_enum = e; }
	~IfcDoorStyleConstructionEnum();
	virtual const char* className() const { return "IfcDoorStyleConstructionEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcDoorStyleConstructionEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcDoorStyleConstructionEnumEnum m_enum;
};
