/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcFootingTypeEnum = ENUMERATION OF	(CAISSON_FOUNDATION	,FOOTING_BEAM	,PAD_FOOTING	,PILE_CAP	,STRIP_FOOTING	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcFootingTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcFootingTypeEnumEnum
	{
		ENUM_CAISSON_FOUNDATION,
		ENUM_FOOTING_BEAM,
		ENUM_PAD_FOOTING,
		ENUM_PILE_CAP,
		ENUM_STRIP_FOOTING,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcFootingTypeEnum();
	IfcFootingTypeEnum( IfcFootingTypeEnumEnum e ) { m_enum = e; }
	~IfcFootingTypeEnum();
	virtual const char* className() const { return "IfcFootingTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcFootingTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcFootingTypeEnumEnum m_enum;
};

