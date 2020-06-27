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

// TYPE IfcIntegerCountRateMeasure = INTEGER;
class IFCPP_EXPORT IfcIntegerCountRateMeasure : public IfcDerivedMeasureValue
{
public:
	IfcIntegerCountRateMeasure();
	IfcIntegerCountRateMeasure( int value );
	~IfcIntegerCountRateMeasure();
	virtual const char* className() const { return "IfcIntegerCountRateMeasure"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcIntegerCountRateMeasure> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	int m_value;
};

