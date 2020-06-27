/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcRelConnects.h"
class IFCPP_EXPORT IfcElement;
class IFCPP_EXPORT IfcConnectionGeometry;
class IFCPP_EXPORT IfcIdentifier;
//ENTITY
class IFCPP_EXPORT IfcRelInterferesElements : public IfcRelConnects
{ 
public:
	IfcRelInterferesElements();
	IfcRelInterferesElements( int id );
	~IfcRelInterferesElements();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 9; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcRelInterferesElements"; }
	virtual const std::wstring toString() const;


	// IfcRoot -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcGloballyUniqueId>		m_GlobalId;
	//  shared_ptr<IfcOwnerHistory>			m_OwnerHistory;				//optional
	//  shared_ptr<IfcLabel>				m_Name;						//optional
	//  shared_ptr<IfcText>					m_Description;				//optional

	// IfcRelationship -----------------------------------------------------------

	// IfcRelConnects -----------------------------------------------------------

	// IfcRelInterferesElements -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcElement>				m_RelatingElement;
	shared_ptr<IfcElement>				m_RelatedElement;
	shared_ptr<IfcConnectionGeometry>	m_InterferenceGeometry;		//optional
	shared_ptr<IfcIdentifier>			m_InterferenceType;			//optional
	LogicalEnum							m_ImpliedOrder;
};

