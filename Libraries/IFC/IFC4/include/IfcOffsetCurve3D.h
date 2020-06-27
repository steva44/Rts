/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcCurve.h"
class IFCPP_EXPORT IfcCurve;
class IFCPP_EXPORT IfcLengthMeasure;
class IFCPP_EXPORT IfcLogical;
class IFCPP_EXPORT IfcDirection;
//ENTITY
class IFCPP_EXPORT IfcOffsetCurve3D : public IfcCurve
{ 
public:
	IfcOffsetCurve3D();
	IfcOffsetCurve3D( int id );
	~IfcOffsetCurve3D();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 4; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcOffsetCurve3D"; }
	virtual const std::wstring toString() const;


	// IfcRepresentationItem -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcPresentationLayerAssignment> >	m_LayerAssignment_inverse;
	//  std::vector<weak_ptr<IfcStyledItem> >					m_StyledByItem_inverse;

	// IfcGeometricRepresentationItem -----------------------------------------------------------

	// IfcCurve -----------------------------------------------------------

	// IfcOffsetCurve3D -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcCurve>									m_BasisCurve;
	shared_ptr<IfcLengthMeasure>							m_Distance;
	shared_ptr<IfcLogical>									m_SelfIntersect;
	shared_ptr<IfcDirection>								m_RefDirection;
};

