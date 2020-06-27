/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcCableCarrierSegmentTypeEnum = ENUMERATION OF	(CABLELADDERSEGMENT	,CABLETRAYSEGMENT	,CABLETRUNKINGSEGMENT	,CONDUITSEGMENT	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcCableCarrierSegmentTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcCableCarrierSegmentTypeEnumEnum
	{
		ENUM_CABLELADDERSEGMENT,
		ENUM_CABLETRAYSEGMENT,
		ENUM_CABLETRUNKINGSEGMENT,
		ENUM_CONDUITSEGMENT,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcCableCarrierSegmentTypeEnum();
	IfcCableCarrierSegmentTypeEnum( IfcCableCarrierSegmentTypeEnumEnum e ) { m_enum = e; }
	~IfcCableCarrierSegmentTypeEnum();
	virtual const char* className() const { return "IfcCableCarrierSegmentTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcCableCarrierSegmentTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcCableCarrierSegmentTypeEnumEnum m_enum;
};
