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
class IFCPP_EXPORT IfcPositiveLengthMeasure;
class IFCPP_EXPORT IfcNonNegativeLengthMeasure;
class IFCPP_EXPORT IfcNormalisedRatioMeasure;
class IFCPP_EXPORT IfcShapeAspect;
class IFCPP_EXPORT IfcLengthMeasure;
//ENTITY
class IFCPP_EXPORT IfcWindowLiningProperties : public IfcPreDefinedPropertySet
{ 
public:
	IfcWindowLiningProperties();
	IfcWindowLiningProperties( int id );
	~IfcWindowLiningProperties();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 16; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcWindowLiningProperties"; }
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

	// IfcWindowLiningProperties -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcPositiveLengthMeasure>				m_LiningDepth;				//optional
	shared_ptr<IfcNonNegativeLengthMeasure>				m_LiningThickness;			//optional
	shared_ptr<IfcNonNegativeLengthMeasure>				m_TransomThickness;			//optional
	shared_ptr<IfcNonNegativeLengthMeasure>				m_MullionThickness;			//optional
	shared_ptr<IfcNormalisedRatioMeasure>				m_FirstTransomOffset;		//optional
	shared_ptr<IfcNormalisedRatioMeasure>				m_SecondTransomOffset;		//optional
	shared_ptr<IfcNormalisedRatioMeasure>				m_FirstMullionOffset;		//optional
	shared_ptr<IfcNormalisedRatioMeasure>				m_SecondMullionOffset;		//optional
	shared_ptr<IfcShapeAspect>							m_ShapeAspectStyle;			//optional
	shared_ptr<IfcLengthMeasure>						m_LiningOffset;				//optional
	shared_ptr<IfcLengthMeasure>						m_LiningToPanelOffsetX;		//optional
	shared_ptr<IfcLengthMeasure>						m_LiningToPanelOffsetY;		//optional
};

