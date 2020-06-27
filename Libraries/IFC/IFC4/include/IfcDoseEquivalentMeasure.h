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

// TYPE IfcDoseEquivalentMeasure = REAL;
class IFCPP_EXPORT IfcDoseEquivalentMeasure : public IfcDerivedMeasureValue
{
public:
	IfcDoseEquivalentMeasure();
	IfcDoseEquivalentMeasure( double value );
	~IfcDoseEquivalentMeasure();
	virtual const char* className() const { return "IfcDoseEquivalentMeasure"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcDoseEquivalentMeasure> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	double m_value;
};
