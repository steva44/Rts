/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcActionRequestTypeEnum = ENUMERATION OF	(EMAIL	,FAX	,PHONE	,POST	,VERBAL	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcActionRequestTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcActionRequestTypeEnumEnum
	{
		ENUM_EMAIL,
		ENUM_FAX,
		ENUM_PHONE,
		ENUM_POST,
		ENUM_VERBAL,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcActionRequestTypeEnum();
	IfcActionRequestTypeEnum( IfcActionRequestTypeEnumEnum e ) { m_enum = e; }
	~IfcActionRequestTypeEnum();
	virtual const char* className() const { return "IfcActionRequestTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcActionRequestTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcActionRequestTypeEnumEnum m_enum;
};

