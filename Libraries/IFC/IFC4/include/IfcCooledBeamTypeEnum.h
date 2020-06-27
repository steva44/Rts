/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcCooledBeamTypeEnum = ENUMERATION OF	(ACTIVE	,PASSIVE	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcCooledBeamTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcCooledBeamTypeEnumEnum
	{
		ENUM_ACTIVE,
		ENUM_PASSIVE,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcCooledBeamTypeEnum();
	IfcCooledBeamTypeEnum( IfcCooledBeamTypeEnumEnum e ) { m_enum = e; }
	~IfcCooledBeamTypeEnum();
	virtual const char* className() const { return "IfcCooledBeamTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcCooledBeamTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcCooledBeamTypeEnumEnum m_enum;
};
