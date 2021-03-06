/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"

// TYPE IfcFontWeight = STRING;
class IFCPP_EXPORT IfcFontWeight : public IfcPPObject
{
public:
	IfcFontWeight();
	IfcFontWeight( std::wstring value );
	~IfcFontWeight();
	virtual const char* className() const { return "IfcFontWeight"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcFontWeight> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	std::wstring m_value;
};

