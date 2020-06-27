/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcAnalysisModelTypeEnum = ENUMERATION OF	(IN_PLANE_LOADING_2D	,OUT_PLANE_LOADING_2D	,LOADING_3D	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcAnalysisModelTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcAnalysisModelTypeEnumEnum
	{
		ENUM_IN_PLANE_LOADING_2D,
		ENUM_OUT_PLANE_LOADING_2D,
		ENUM_LOADING_3D,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcAnalysisModelTypeEnum();
	IfcAnalysisModelTypeEnum( IfcAnalysisModelTypeEnumEnum e ) { m_enum = e; }
	~IfcAnalysisModelTypeEnum();
	virtual const char* className() const { return "IfcAnalysisModelTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcAnalysisModelTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcAnalysisModelTypeEnumEnum m_enum;
};
