/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcProjectionElementTypeEnum = ENUMERATION OF	(USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcProjectionElementTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcProjectionElementTypeEnumEnum
	{
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcProjectionElementTypeEnum();
	IfcProjectionElementTypeEnum( IfcProjectionElementTypeEnumEnum e ) { m_enum = e; }
	~IfcProjectionElementTypeEnum();
	virtual const char* className() const { return "IfcProjectionElementTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcProjectionElementTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcProjectionElementTypeEnumEnum m_enum;
};
