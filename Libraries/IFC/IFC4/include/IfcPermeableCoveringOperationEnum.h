/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcPermeableCoveringOperationEnum = ENUMERATION OF	(GRILL	,LOUVER	,SCREEN	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcPermeableCoveringOperationEnum : virtual public IfcPPObject
{
public:
	enum IfcPermeableCoveringOperationEnumEnum
	{
		ENUM_GRILL,
		ENUM_LOUVER,
		ENUM_SCREEN,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcPermeableCoveringOperationEnum();
	IfcPermeableCoveringOperationEnum( IfcPermeableCoveringOperationEnumEnum e ) { m_enum = e; }
	~IfcPermeableCoveringOperationEnum();
	virtual const char* className() const { return "IfcPermeableCoveringOperationEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcPermeableCoveringOperationEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcPermeableCoveringOperationEnumEnum m_enum;
};
