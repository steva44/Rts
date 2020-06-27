/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcDocumentConfidentialityEnum = ENUMERATION OF	(PUBLIC	,RESTRICTED	,CONFIDENTIAL	,PERSONAL	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcDocumentConfidentialityEnum : virtual public IfcPPObject
{
public:
	enum IfcDocumentConfidentialityEnumEnum
	{
		ENUM_PUBLIC,
		ENUM_RESTRICTED,
		ENUM_CONFIDENTIAL,
		ENUM_PERSONAL,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcDocumentConfidentialityEnum();
	IfcDocumentConfidentialityEnum( IfcDocumentConfidentialityEnumEnum e ) { m_enum = e; }
	~IfcDocumentConfidentialityEnum();
	virtual const char* className() const { return "IfcDocumentConfidentialityEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcDocumentConfidentialityEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcDocumentConfidentialityEnumEnum m_enum;
};
