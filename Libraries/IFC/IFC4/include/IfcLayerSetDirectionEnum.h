/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcLayerSetDirectionEnum = ENUMERATION OF	(AXIS1	,AXIS2	,AXIS3);
class IFCPP_EXPORT IfcLayerSetDirectionEnum : virtual public IfcPPObject
{
public:
	enum IfcLayerSetDirectionEnumEnum
	{
		ENUM_AXIS1,
		ENUM_AXIS2,
		ENUM_AXIS3
	};

	IfcLayerSetDirectionEnum();
	IfcLayerSetDirectionEnum( IfcLayerSetDirectionEnumEnum e ) { m_enum = e; }
	~IfcLayerSetDirectionEnum();
	virtual const char* className() const { return "IfcLayerSetDirectionEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcLayerSetDirectionEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcLayerSetDirectionEnumEnum m_enum;
};
