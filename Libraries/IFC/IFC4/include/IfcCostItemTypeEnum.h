/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcCostItemTypeEnum = ENUMERATION OF	(USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcCostItemTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcCostItemTypeEnumEnum
	{
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcCostItemTypeEnum();
	IfcCostItemTypeEnum( IfcCostItemTypeEnumEnum e ) { m_enum = e; }
	~IfcCostItemTypeEnum();
	virtual const char* className() const { return "IfcCostItemTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcCostItemTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcCostItemTypeEnumEnum m_enum;
};

