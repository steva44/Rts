/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcCableFittingTypeEnum = ENUMERATION OF	(CONNECTOR	,ENTRY	,EXIT	,JUNCTION	,TRANSITION	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcCableFittingTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcCableFittingTypeEnumEnum
	{
		ENUM_CONNECTOR,
		ENUM_ENTRY,
		ENUM_EXIT,
		ENUM_JUNCTION,
		ENUM_TRANSITION,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcCableFittingTypeEnum();
	IfcCableFittingTypeEnum( IfcCableFittingTypeEnumEnum e ) { m_enum = e; }
	~IfcCableFittingTypeEnum();
	virtual const char* className() const { return "IfcCableFittingTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcCableFittingTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcCableFittingTypeEnumEnum m_enum;
};
