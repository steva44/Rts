/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcSurfaceStyleElementSelect.h"
#include "IfcPresentationItem.h"
class IFCPP_EXPORT IfcColourRgb;
//ENTITY
class IFCPP_EXPORT IfcSurfaceStyleLighting : virtual public IfcSurfaceStyleElementSelect, public IfcPresentationItem
{ 
public:
	IfcSurfaceStyleLighting();
	IfcSurfaceStyleLighting( int id );
	~IfcSurfaceStyleLighting();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 4; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcSurfaceStyleLighting"; }
	virtual const std::wstring toString() const;


	// IfcPresentationItem -----------------------------------------------------------

	// IfcSurfaceStyleLighting -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcColourRgb>	m_DiffuseTransmissionColour;
	shared_ptr<IfcColourRgb>	m_DiffuseReflectionColour;
	shared_ptr<IfcColourRgb>	m_TransmissionColour;
	shared_ptr<IfcColourRgb>	m_ReflectanceColour;
};

