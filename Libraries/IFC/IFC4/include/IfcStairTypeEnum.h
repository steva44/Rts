/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcStairTypeEnum = ENUMERATION OF	(STRAIGHT_RUN_STAIR	,TWO_STRAIGHT_RUN_STAIR	,QUARTER_WINDING_STAIR	,QUARTER_TURN_STAIR	,HALF_WINDING_STAIR	,HALF_TURN_STAIR	,TWO_QUARTER_WINDING_STAIR	,TWO_QUARTER_TURN_STAIR	,THREE_QUARTER_WINDING_STAIR	,THREE_QUARTER_TURN_STAIR	,SPIRAL_STAIR	,DOUBLE_RETURN_STAIR	,CURVED_RUN_STAIR	,TWO_CURVED_RUN_STAIR	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcStairTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcStairTypeEnumEnum
	{
		ENUM_STRAIGHT_RUN_STAIR,
		ENUM_TWO_STRAIGHT_RUN_STAIR,
		ENUM_QUARTER_WINDING_STAIR,
		ENUM_QUARTER_TURN_STAIR,
		ENUM_HALF_WINDING_STAIR,
		ENUM_HALF_TURN_STAIR,
		ENUM_TWO_QUARTER_WINDING_STAIR,
		ENUM_TWO_QUARTER_TURN_STAIR,
		ENUM_THREE_QUARTER_WINDING_STAIR,
		ENUM_THREE_QUARTER_TURN_STAIR,
		ENUM_SPIRAL_STAIR,
		ENUM_DOUBLE_RETURN_STAIR,
		ENUM_CURVED_RUN_STAIR,
		ENUM_TWO_CURVED_RUN_STAIR,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcStairTypeEnum();
	IfcStairTypeEnum( IfcStairTypeEnumEnum e ) { m_enum = e; }
	~IfcStairTypeEnum();
	virtual const char* className() const { return "IfcStairTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcStairTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcStairTypeEnumEnum m_enum;
};

