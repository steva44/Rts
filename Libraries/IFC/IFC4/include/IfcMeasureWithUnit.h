/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcAppliedValueSelect.h"
#include "IfcMetricValueSelect.h"
class IFCPP_EXPORT IfcValue;
class IFCPP_EXPORT IfcUnit;
//ENTITY
class IFCPP_EXPORT IfcMeasureWithUnit : virtual public IfcAppliedValueSelect, virtual public IfcMetricValueSelect, public IfcPPEntity
{ 
public:
	IfcMeasureWithUnit();
	IfcMeasureWithUnit( int id );
	~IfcMeasureWithUnit();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 2; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcMeasureWithUnit"; }
	virtual const std::wstring toString() const;


	// IfcMeasureWithUnit -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcValue>	m_ValueComponent;
	shared_ptr<IfcUnit>		m_UnitComponent;
};
