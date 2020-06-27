/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcMaterialUsageDefinition.h"
class IFCPP_EXPORT IfcMaterialProfileSet;
class IFCPP_EXPORT IfcCardinalPointReference;
class IFCPP_EXPORT IfcPositiveLengthMeasure;
//ENTITY
class IFCPP_EXPORT IfcMaterialProfileSetUsage : public IfcMaterialUsageDefinition
{ 
public:
	IfcMaterialProfileSetUsage();
	IfcMaterialProfileSetUsage( int id );
	~IfcMaterialProfileSetUsage();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 3; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcMaterialProfileSetUsage"; }
	virtual const std::wstring toString() const;


	// IfcMaterialUsageDefinition -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelAssociatesMaterial> >	m_AssociatedTo_inverse;

	// IfcMaterialProfileSetUsage -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcMaterialProfileSet>					m_ForProfileSet;
	shared_ptr<IfcCardinalPointReference>				m_CardinalPoint;			//optional
	shared_ptr<IfcPositiveLengthMeasure>				m_ReferenceExtent;			//optional
};
