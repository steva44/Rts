/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcSchedulingTime.h"
class IFCPP_EXPORT IfcDateTime;
//ENTITY
class IFCPP_EXPORT IfcEventTime : public IfcSchedulingTime
{ 
public:
	IfcEventTime();
	IfcEventTime( int id );
	~IfcEventTime();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 7; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcEventTime"; }
	virtual const std::wstring toString() const;


	// IfcSchedulingTime -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>			m_Name;						//optional
	//  shared_ptr<IfcDataOriginEnum>	m_DataOrigin;				//optional
	//  shared_ptr<IfcLabel>			m_UserDefinedDataOrigin;	//optional

	// IfcEventTime -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcDateTime>			m_ActualDate;				//optional
	shared_ptr<IfcDateTime>			m_EarlyDate;				//optional
	shared_ptr<IfcDateTime>			m_LateDate;					//optional
	shared_ptr<IfcDateTime>			m_ScheduleDate;				//optional
};

