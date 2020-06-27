/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcDataOriginEnum = ENUMERATION OF	(MEASURED	,PREDICTED	,SIMULATED	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcDataOriginEnum : virtual public IfcPPObject
{
public:
	enum IfcDataOriginEnumEnum
	{
		ENUM_MEASURED,
		ENUM_PREDICTED,
		ENUM_SIMULATED,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcDataOriginEnum();
	IfcDataOriginEnum( IfcDataOriginEnumEnum e ) { m_enum = e; }
	~IfcDataOriginEnum();
	virtual const char* className() const { return "IfcDataOriginEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcDataOriginEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcDataOriginEnumEnum m_enum;
};
