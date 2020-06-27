/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcProcess.h"
class IFCPP_EXPORT IfcEventTypeEnum;
class IFCPP_EXPORT IfcEventTriggerTypeEnum;
class IFCPP_EXPORT IfcLabel;
class IFCPP_EXPORT IfcEventTime;
//ENTITY
class IFCPP_EXPORT IfcEvent : public IfcProcess
{ 
public:
	IfcEvent();
	IfcEvent( int id );
	~IfcEvent();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 11; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcEvent"; }
	virtual const std::wstring toString() const;


	// IfcRoot -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcGloballyUniqueId>						m_GlobalId;
	//  shared_ptr<IfcOwnerHistory>							m_OwnerHistory;				//optional
	//  shared_ptr<IfcLabel>								m_Name;						//optional
	//  shared_ptr<IfcText>									m_Description;				//optional

	// IfcObjectDefinition -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelAssigns> >				m_HasAssignments_inverse;
	//  std::vector<weak_ptr<IfcRelNests> >					m_Nests_inverse;
	//  std::vector<weak_ptr<IfcRelNests> >					m_IsNestedBy_inverse;
	//  std::vector<weak_ptr<IfcRelDeclares> >				m_HasContext_inverse;
	//  std::vector<weak_ptr<IfcRelAggregates> >			m_IsDecomposedBy_inverse;
	//  std::vector<weak_ptr<IfcRelAggregates> >			m_Decomposes_inverse;
	//  std::vector<weak_ptr<IfcRelAssociates> >			m_HasAssociations_inverse;

	// IfcObject -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>								m_ObjectType;				//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelDefinesByObject> >		m_IsDeclaredBy_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByObject> >		m_Declares_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByType> >			m_IsTypedBy_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByProperties> >	m_IsDefinedBy_inverse;

	// IfcProcess -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcIdentifier>							m_Identification;			//optional
	//  shared_ptr<IfcText>									m_LongDescription;			//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelSequence> >				m_IsPredecessorTo_inverse;
	//  std::vector<weak_ptr<IfcRelSequence> >				m_IsSuccessorFrom_inverse;
	//  std::vector<weak_ptr<IfcRelAssignsToProcess> >		m_OperatesOn_inverse;

	// IfcEvent -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcEventTypeEnum>						m_PredefinedType;			//optional
	shared_ptr<IfcEventTriggerTypeEnum>					m_EventTriggerType;			//optional
	shared_ptr<IfcLabel>								m_UserDefinedEventTriggerType;	//optional
	shared_ptr<IfcEventTime>							m_EventOccurenceTime;		//optional
};
