/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcWindowPanelPositionEnum = ENUMERATION OF	(LEFT	,MIDDLE	,RIGHT	,BOTTOM	,TOP	,NOTDEFINED);
class IFCPP_EXPORT IfcWindowPanelPositionEnum : virtual public IfcPPObject
{
public:
	enum IfcWindowPanelPositionEnumEnum
	{
		ENUM_LEFT,
		ENUM_MIDDLE,
		ENUM_RIGHT,
		ENUM_BOTTOM,
		ENUM_TOP,
		ENUM_NOTDEFINED
	};

	IfcWindowPanelPositionEnum();
	IfcWindowPanelPositionEnum( IfcWindowPanelPositionEnumEnum e ) { m_enum = e; }
	~IfcWindowPanelPositionEnum();
	virtual const char* className() const { return "IfcWindowPanelPositionEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcWindowPanelPositionEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcWindowPanelPositionEnumEnum m_enum;
};

