/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcDoorPanelPositionEnum = ENUMERATION OF	(LEFT	,MIDDLE	,RIGHT	,NOTDEFINED);
class IFCPP_EXPORT IfcDoorPanelPositionEnum : virtual public IfcPPObject
{
public:
	enum IfcDoorPanelPositionEnumEnum
	{
		ENUM_LEFT,
		ENUM_MIDDLE,
		ENUM_RIGHT,
		ENUM_NOTDEFINED
	};

	IfcDoorPanelPositionEnum();
	IfcDoorPanelPositionEnum( IfcDoorPanelPositionEnumEnum e ) { m_enum = e; }
	~IfcDoorPanelPositionEnum();
	virtual const char* className() const { return "IfcDoorPanelPositionEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcDoorPanelPositionEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcDoorPanelPositionEnumEnum m_enum;
};
