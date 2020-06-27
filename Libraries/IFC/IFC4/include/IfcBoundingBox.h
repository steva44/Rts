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
class IFCPP_EXPORT IfcCartesianPoint;
class IFCPP_EXPORT IfcPositiveLengthMeasure;
//ENTITY
class IFCPP_EXPORT IfcBoundingBox : public IfcGeometricRepresentationItem
{ 
public:
	IfcBoundingBox();
	IfcBoundingBox( int id );
	~IfcBoundingBox();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 4; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcBoundingBox"; }
	virtual const std::wstring toString() const;


	// IfcRepresentationItem -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcPresentationLayerAssignment> >	m_LayerAssignment_inverse;
	//  std::vector<weak_ptr<IfcStyledItem> >					m_StyledByItem_inverse;

	// IfcGeometricRepresentationItem -----------------------------------------------------------

	// IfcBoundingBox -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcCartesianPoint>							m_Corner;
	shared_ptr<IfcPositiveLengthMeasure>					m_XDim;
	shared_ptr<IfcPositiveLengthMeasure>					m_YDim;
	shared_ptr<IfcPositiveLengthMeasure>					m_ZDim;
};

