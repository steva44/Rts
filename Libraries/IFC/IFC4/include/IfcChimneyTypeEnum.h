/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcChimneyTypeEnum = ENUMERATION OF	(USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcChimneyTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcChimneyTypeEnumEnum
	{
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcChimneyTypeEnum();
	IfcChimneyTypeEnum( IfcChimneyTypeEnumEnum e ) { m_enum = e; }
	~IfcChimneyTypeEnum();
	virtual const char* className() const { return "IfcChimneyTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcChimneyTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcChimneyTypeEnumEnum m_enum;
};

