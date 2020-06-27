/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcInventoryTypeEnum = ENUMERATION OF	(ASSETINVENTORY	,SPACEINVENTORY	,FURNITUREINVENTORY	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcInventoryTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcInventoryTypeEnumEnum
	{
		ENUM_ASSETINVENTORY,
		ENUM_SPACEINVENTORY,
		ENUM_FURNITUREINVENTORY,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcInventoryTypeEnum();
	IfcInventoryTypeEnum( IfcInventoryTypeEnumEnum e ) { m_enum = e; }
	~IfcInventoryTypeEnum();
	virtual const char* className() const { return "IfcInventoryTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcInventoryTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcInventoryTypeEnumEnum m_enum;
};

