/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcPipeSegmentTypeEnum = ENUMERATION OF	(CULVERT	,FLEXIBLESEGMENT	,RIGIDSEGMENT	,GUTTER	,SPOOL	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcPipeSegmentTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcPipeSegmentTypeEnumEnum
	{
		ENUM_CULVERT,
		ENUM_FLEXIBLESEGMENT,
		ENUM_RIGIDSEGMENT,
		ENUM_GUTTER,
		ENUM_SPOOL,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcPipeSegmentTypeEnum();
	IfcPipeSegmentTypeEnum( IfcPipeSegmentTypeEnumEnum e ) { m_enum = e; }
	~IfcPipeSegmentTypeEnum();
	virtual const char* className() const { return "IfcPipeSegmentTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcPipeSegmentTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcPipeSegmentTypeEnumEnum m_enum;
};

