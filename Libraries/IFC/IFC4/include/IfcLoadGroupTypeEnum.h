/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcLoadGroupTypeEnum = ENUMERATION OF	(LOAD_GROUP	,LOAD_CASE	,LOAD_COMBINATION	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcLoadGroupTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcLoadGroupTypeEnumEnum
	{
		ENUM_LOAD_GROUP,
		ENUM_LOAD_CASE,
		ENUM_LOAD_COMBINATION,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcLoadGroupTypeEnum();
	IfcLoadGroupTypeEnum( IfcLoadGroupTypeEnumEnum e ) { m_enum = e; }
	~IfcLoadGroupTypeEnum();
	virtual const char* className() const { return "IfcLoadGroupTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcLoadGroupTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcLoadGroupTypeEnumEnum m_enum;
};
