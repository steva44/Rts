/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcPropertySetDefinitionSelect.h"
class IFCPP_EXPORT IfcPropertySetDefinition;

// TYPE IfcPropertySetDefinitionSet = SET [1:?] OF IfcPropertySetDefinition;
class IFCPP_EXPORT IfcPropertySetDefinitionSet : public IfcPropertySetDefinitionSelect
{
public:
	IfcPropertySetDefinitionSet();
	~IfcPropertySetDefinitionSet();
	virtual const char* className() const { return "IfcPropertySetDefinitionSet"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcPropertySetDefinitionSet> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	std::vector<shared_ptr<IfcPropertySetDefinition> > m_vec;
};
