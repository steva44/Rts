/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcConstructionEquipmentResourceTypeEnum = ENUMERATION OF	(DEMOLISHING	,EARTHMOVING	,ERECTING	,HEATING	,LIGHTING	,PAVING	,PUMPING	,TRANSPORTING	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcConstructionEquipmentResourceTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcConstructionEquipmentResourceTypeEnumEnum
	{
		ENUM_DEMOLISHING,
		ENUM_EARTHMOVING,
		ENUM_ERECTING,
		ENUM_HEATING,
		ENUM_LIGHTING,
		ENUM_PAVING,
		ENUM_PUMPING,
		ENUM_TRANSPORTING,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcConstructionEquipmentResourceTypeEnum();
	IfcConstructionEquipmentResourceTypeEnum( IfcConstructionEquipmentResourceTypeEnumEnum e ) { m_enum = e; }
	~IfcConstructionEquipmentResourceTypeEnum();
	virtual const char* className() const { return "IfcConstructionEquipmentResourceTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcConstructionEquipmentResourceTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcConstructionEquipmentResourceTypeEnumEnum m_enum;
};

