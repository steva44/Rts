/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcResourceObjectSelect.h"
class IFCPP_EXPORT IfcIdentifier;
class IFCPP_EXPORT IfcLabel;
class IFCPP_EXPORT IfcText;
class IFCPP_EXPORT IfcDateTime;
class IFCPP_EXPORT IfcActorSelect;
class IFCPP_EXPORT IfcExternalReferenceRelationship;
class IFCPP_EXPORT IfcRelAssociatesApproval;
class IFCPP_EXPORT IfcResourceApprovalRelationship;
class IFCPP_EXPORT IfcApprovalRelationship;
//ENTITY
class IFCPP_EXPORT IfcApproval : virtual public IfcResourceObjectSelect, public IfcPPEntity
{ 
public:
	IfcApproval();
	IfcApproval( int id );
	~IfcApproval();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 9; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcApproval"; }
	virtual const std::wstring toString() const;


	// IfcApproval -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcIdentifier>									m_Identifier;				//optional
	shared_ptr<IfcLabel>										m_Name;						//optional
	shared_ptr<IfcText>											m_Description;				//optional
	shared_ptr<IfcDateTime>										m_TimeOfApproval;			//optional
	shared_ptr<IfcLabel>										m_Status;					//optional
	shared_ptr<IfcLabel>										m_Level;					//optional
	shared_ptr<IfcText>											m_Qualifier;				//optional
	shared_ptr<IfcActorSelect>									m_RequestingApproval;		//optional
	shared_ptr<IfcActorSelect>									m_GivingApproval;			//optional
	// inverse attributes:
	std::vector<weak_ptr<IfcExternalReferenceRelationship> >	m_HasExternalReferences_inverse;
	std::vector<weak_ptr<IfcRelAssociatesApproval> >			m_ApprovedObjects_inverse;
	std::vector<weak_ptr<IfcResourceApprovalRelationship> >		m_ApprovedResources_inverse;
	std::vector<weak_ptr<IfcApprovalRelationship> >				m_IsRelatedWith_inverse;
	std::vector<weak_ptr<IfcApprovalRelationship> >				m_Relates_inverse;
};

