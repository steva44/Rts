/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcDerivedMeasureValue.h"

// TYPE IfcCompoundPlaneAngleMeasure = LIST [3:4] OF INTEGER;
class IFCPP_EXPORT IfcCompoundPlaneAngleMeasure : public IfcDerivedMeasureValue
{
public:
	IfcCompoundPlaneAngleMeasure();
	~IfcCompoundPlaneAngleMeasure();
	virtual const char* className() const { return "IfcCompoundPlaneAngleMeasure"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcCompoundPlaneAngleMeasure> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	std::vector<int> m_vec;
};
