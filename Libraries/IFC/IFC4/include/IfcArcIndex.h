/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcPositiveInteger.h"
#include "IfcSegmentIndexSelect.h"

// TYPE IfcArcIndex = LIST [3:3] OF IfcPositiveInteger;
class IFCPP_EXPORT IfcArcIndex : public IfcPositiveInteger, public IfcSegmentIndexSelect
{
public:
	IfcArcIndex();
	~IfcArcIndex();
	virtual const char* className() const { return "IfcArcIndex"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcArcIndex> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	std::vector<IfcPositiveInteger> m_vec;
};

