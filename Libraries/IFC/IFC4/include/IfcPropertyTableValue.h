/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcSimpleProperty.h"
class IFCPP_EXPORT IfcValue;
class IFCPP_EXPORT IfcText;
class IFCPP_EXPORT IfcUnit;
class IFCPP_EXPORT IfcCurveInterpolationEnum;
//ENTITY
class IFCPP_EXPORT IfcPropertyTableValue : public IfcSimpleProperty
{ 
public:
	IfcPropertyTableValue();
	IfcPropertyTableValue( int id );
	~IfcPropertyTableValue();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 8; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcPropertyTableValue"; }
	virtual const std::wstring toString() const;


	// IfcPropertyAbstraction -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcExternalReferenceRelationship> >	m_HasExternalReferences_inverse;

	// IfcProperty -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcIdentifier>									m_Name;
	//  shared_ptr<IfcText>											m_Description;				//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcPropertySet> >						m_PartOfPset_inverse;
	//  std::vector<weak_ptr<IfcPropertyDependencyRelationship> >	m_PropertyForDependance_inverse;
	//  std::vector<weak_ptr<IfcPropertyDependencyRelationship> >	m_PropertyDependsOn_inverse;
	//  std::vector<weak_ptr<IfcComplexProperty> >					m_PartOfComplex_inverse;
	//  std::vector<weak_ptr<IfcResourceConstraintRelationship> >	m_HasConstraints_inverse;
	//  std::vector<weak_ptr<IfcResourceApprovalRelationship> >		m_HasApprovals_inverse;

	// IfcSimpleProperty -----------------------------------------------------------

	// IfcPropertyTableValue -----------------------------------------------------------
	// attributes:
	std::vector<shared_ptr<IfcValue> >							m_DefiningValues;			//optional
	std::vector<shared_ptr<IfcValue> >							m_DefinedValues;			//optional
	shared_ptr<IfcText>											m_Expression;				//optional
	shared_ptr<IfcUnit>											m_DefiningUnit;				//optional
	shared_ptr<IfcUnit>											m_DefinedUnit;				//optional
	shared_ptr<IfcCurveInterpolationEnum>						m_CurveInterpolation;		//optional
};
