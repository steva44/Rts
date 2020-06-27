/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcProductRepresentationSelect.h"
class IFCPP_EXPORT IfcAxis2Placement;
class IFCPP_EXPORT IfcRepresentation;
class IFCPP_EXPORT IfcShapeAspect;
class IFCPP_EXPORT IfcMappedItem;
//ENTITY
class IFCPP_EXPORT IfcRepresentationMap : virtual public IfcProductRepresentationSelect, public IfcPPEntity
{ 
public:
	IfcRepresentationMap();
	IfcRepresentationMap( int id );
	~IfcRepresentationMap();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 2; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcRepresentationMap"; }
	virtual const std::wstring toString() const;


	// IfcRepresentationMap -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcAxis2Placement>			m_MappingOrigin;
	shared_ptr<IfcRepresentation>			m_MappedRepresentation;
	// inverse attributes:
	std::vector<weak_ptr<IfcShapeAspect> >	m_HasShapeAspects_inverse;
	std::vector<weak_ptr<IfcMappedItem> >	m_MapUsage_inverse;
};
