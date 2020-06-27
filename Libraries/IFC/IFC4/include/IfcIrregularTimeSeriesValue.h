/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
class IFCPP_EXPORT IfcDateTime;
class IFCPP_EXPORT IfcValue;
//ENTITY
class IFCPP_EXPORT IfcIrregularTimeSeriesValue : public IfcPPEntity
{ 
public:
	IfcIrregularTimeSeriesValue();
	IfcIrregularTimeSeriesValue( int id );
	~IfcIrregularTimeSeriesValue();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 2; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcIrregularTimeSeriesValue"; }
	virtual const std::wstring toString() const;


	// IfcIrregularTimeSeriesValue -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcDateTime>				m_TimeStamp;
	std::vector<shared_ptr<IfcValue> >	m_ListValues;
};

