/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcTransportElementTypeEnum = ENUMERATION OF	(ELEVATOR	,ESCALATOR	,MOVINGWALKWAY	,CRANEWAY	,LIFTINGGEAR	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcTransportElementTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcTransportElementTypeEnumEnum
	{
		ENUM_ELEVATOR,
		ENUM_ESCALATOR,
		ENUM_MOVINGWALKWAY,
		ENUM_CRANEWAY,
		ENUM_LIFTINGGEAR,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcTransportElementTypeEnum();
	IfcTransportElementTypeEnum( IfcTransportElementTypeEnumEnum e ) { m_enum = e; }
	~IfcTransportElementTypeEnum();
	virtual const char* className() const { return "IfcTransportElementTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcTransportElementTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcTransportElementTypeEnumEnum m_enum;
};

