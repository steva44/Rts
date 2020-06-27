/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcOpeningElementTypeEnum = ENUMERATION OF	(OPENING	,RECESS	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcOpeningElementTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcOpeningElementTypeEnumEnum
	{
		ENUM_OPENING,
		ENUM_RECESS,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcOpeningElementTypeEnum();
	IfcOpeningElementTypeEnum( IfcOpeningElementTypeEnumEnum e ) { m_enum = e; }
	~IfcOpeningElementTypeEnum();
	virtual const char* className() const { return "IfcOpeningElementTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcOpeningElementTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcOpeningElementTypeEnumEnum m_enum;
};

