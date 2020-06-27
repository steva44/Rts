/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcMedicalDeviceTypeEnum = ENUMERATION OF	(AIRSTATION	,FEEDAIRUNIT	,OXYGENGENERATOR	,OXYGENPLANT	,VACUUMSTATION	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcMedicalDeviceTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcMedicalDeviceTypeEnumEnum
	{
		ENUM_AIRSTATION,
		ENUM_FEEDAIRUNIT,
		ENUM_OXYGENGENERATOR,
		ENUM_OXYGENPLANT,
		ENUM_VACUUMSTATION,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcMedicalDeviceTypeEnum();
	IfcMedicalDeviceTypeEnum( IfcMedicalDeviceTypeEnumEnum e ) { m_enum = e; }
	~IfcMedicalDeviceTypeEnum();
	virtual const char* className() const { return "IfcMedicalDeviceTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcMedicalDeviceTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcMedicalDeviceTypeEnumEnum m_enum;
};
