/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcPreDefinedPropertySet.h"
class IFCPP_EXPORT IfcWindowPanelOperationEnum;
class IFCPP_EXPORT IfcWindowPanelPositionEnum;
class IFCPP_EXPORT IfcPositiveLengthMeasure;
class IFCPP_EXPORT IfcShapeAspect;
//ENTITY
class IFCPP_EXPORT IfcWindowPanelProperties : public IfcPreDefinedPropertySet
{ 
public:
	IfcWindowPanelProperties();
	IfcWindowPanelProperties( int id );
	~IfcWindowPanelProperties();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 9; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcWindowPanelProperties"; }
	virtual const std::wstring toString() const;


	// IfcRoot -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcGloballyUniqueId>						m_GlobalId;
	//  shared_ptr<IfcOwnerHistory>							m_OwnerHistory;				//optional
	//  shared_ptr<IfcLabel>								m_Name;						//optional
	//  shared_ptr<IfcText>									m_Description;				//optional

	// IfcPropertyDefinition -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelDeclares> >				m_HasContext_inverse;
	//  std::vector<weak_ptr<IfcRelAssociates> >			m_HasAssociations_inverse;

	// IfcPropertySetDefinition -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcTypeObject> >				m_DefinesType_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByTemplate> >		m_IsDefinedBy_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByProperties> >	m_DefinesOccurrence_inverse;

	// IfcPreDefinedPropertySet -----------------------------------------------------------

	// IfcWindowPanelProperties -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcWindowPanelOperationEnum>				m_OperationType;
	shared_ptr<IfcWindowPanelPositionEnum>				m_PanelPosition;
	shared_ptr<IfcPositiveLengthMeasure>				m_FrameDepth;				//optional
	shared_ptr<IfcPositiveLengthMeasure>				m_FrameThickness;			//optional
	shared_ptr<IfcShapeAspect>							m_ShapeAspectStyle;			//optional
};

