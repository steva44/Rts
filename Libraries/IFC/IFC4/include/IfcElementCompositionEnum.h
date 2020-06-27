/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcElementCompositionEnum = ENUMERATION OF	(COMPLEX	,ELEMENT	,PARTIAL);
class IFCPP_EXPORT IfcElementCompositionEnum : virtual public IfcPPObject
{
public:
	enum IfcElementCompositionEnumEnum
	{
		ENUM_COMPLEX,
		ENUM_ELEMENT,
		ENUM_PARTIAL
	};

	IfcElementCompositionEnum();
	IfcElementCompositionEnum( IfcElementCompositionEnumEnum e ) { m_enum = e; }
	~IfcElementCompositionEnum();
	virtual const char* className() const { return "IfcElementCompositionEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcElementCompositionEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	IfcElementCompositionEnumEnum m_enum;
};

