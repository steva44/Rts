/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcResourceLevelRelationship.h"
class IFCPP_EXPORT IfcMonetaryUnit;
class IFCPP_EXPORT IfcPositiveRatioMeasure;
class IFCPP_EXPORT IfcDateTime;
class IFCPP_EXPORT IfcLibraryInformation;
//ENTITY
class IFCPP_EXPORT IfcCurrencyRelationship : public IfcResourceLevelRelationship
{ 
public:
	IfcCurrencyRelationship();
	IfcCurrencyRelationship( int id );
	~IfcCurrencyRelationship();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 7; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcCurrencyRelationship"; }
	virtual const std::wstring toString() const;


	// IfcResourceLevelRelationship -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>				m_Name;						//optional
	//  shared_ptr<IfcText>					m_Description;				//optional

	// IfcCurrencyRelationship -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcMonetaryUnit>			m_RelatingMonetaryUnit;
	shared_ptr<IfcMonetaryUnit>			m_RelatedMonetaryUnit;
	shared_ptr<IfcPositiveRatioMeasure>	m_ExchangeRate;
	shared_ptr<IfcDateTime>				m_RateDateTime;				//optional
	shared_ptr<IfcLibraryInformation>	m_RateSource;				//optional
};

