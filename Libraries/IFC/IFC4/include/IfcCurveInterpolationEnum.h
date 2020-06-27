/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcCurveInterpolationEnum = ENUMERATION OF	(LINEAR	,LOG_LINEAR	,LOG_LOG	,NOTDEFINED);
class IFCPP_EXPORT IfcCurveInterpolationEnum : virtual public IfcPPObject
{
public:
	enum IfcCurveInterpolationEnumEnum
	{
		ENUM_LINEAR,
		ENUM_LOG_LINEAR,
		ENUM_LOG_LOG,
		ENUM_NOTDEFINED
	};

	IfcCurveInterpolationEnum();
	IfcCurveInterpolationEnum( IfcCurveInterpolationEnumEnum e ) { m_enum = e; }
	~IfcCurveInterpolationEnum();
	virtual const char* className() const { return "IfcCurveInterpolationEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcCurveInterpolationEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcCurveInterpolationEnumEnum m_enum;
};

