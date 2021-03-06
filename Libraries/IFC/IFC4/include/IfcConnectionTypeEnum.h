/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcConnectionTypeEnum = ENUMERATION OF	(ATPATH	,ATSTART	,ATEND	,NOTDEFINED);
class IFCPP_EXPORT IfcConnectionTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcConnectionTypeEnumEnum
	{
		ENUM_ATPATH,
		ENUM_ATSTART,
		ENUM_ATEND,
		ENUM_NOTDEFINED
	};

	IfcConnectionTypeEnum();
	IfcConnectionTypeEnum( IfcConnectionTypeEnumEnum e ) { m_enum = e; }
	~IfcConnectionTypeEnum();
	virtual const char* className() const { return "IfcConnectionTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcConnectionTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcConnectionTypeEnumEnum m_enum;
};

