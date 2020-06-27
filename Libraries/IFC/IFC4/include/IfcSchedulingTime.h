/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
class IFCPP_EXPORT IfcLabel;
class IFCPP_EXPORT IfcDataOriginEnum;
//ENTITY
class IFCPP_EXPORT IfcSchedulingTime : public IfcPPEntity
{ 
public:
	IfcSchedulingTime();
	IfcSchedulingTime( int id );
	~IfcSchedulingTime();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 3; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcSchedulingTime"; }
	virtual const std::wstring toString() const;


	// IfcSchedulingTime -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcLabel>			m_Name;						//optional
	shared_ptr<IfcDataOriginEnum>	m_DataOrigin;				//optional
	shared_ptr<IfcLabel>			m_UserDefinedDataOrigin;	//optional
};
