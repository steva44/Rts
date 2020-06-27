/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcGeometricRepresentationItem.h"
class IFCPP_EXPORT IfcTransitionCode;
class IFCPP_EXPORT IfcBoolean;
class IFCPP_EXPORT IfcCurve;
class IFCPP_EXPORT IfcCompositeCurve;
//ENTITY
class IFCPP_EXPORT IfcCompositeCurveSegment : public IfcGeometricRepresentationItem
{ 
public:
	IfcCompositeCurveSegment();
	IfcCompositeCurveSegment( int id );
	~IfcCompositeCurveSegment();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 3; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcCompositeCurveSegment"; }
	virtual const std::wstring toString() const;


	// IfcRepresentationItem -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcPresentationLayerAssignment> >	m_LayerAssignment_inverse;
	//  std::vector<weak_ptr<IfcStyledItem> >					m_StyledByItem_inverse;

	// IfcGeometricRepresentationItem -----------------------------------------------------------

	// IfcCompositeCurveSegment -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcTransitionCode>							m_Transition;
	shared_ptr<IfcBoolean>									m_SameSense;
	shared_ptr<IfcCurve>									m_ParentCurve;
	// inverse attributes:
	std::vector<weak_ptr<IfcCompositeCurve> >				m_UsingCurves_inverse;
};

