/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcStructuralLoadStatic.h"
class IFCPP_EXPORT IfcLinearForceMeasure;
class IFCPP_EXPORT IfcLinearMomentMeasure;
//ENTITY
class IFCPP_EXPORT IfcStructuralLoadLinearForce : public IfcStructuralLoadStatic
{ 
public:
	IfcStructuralLoadLinearForce();
	IfcStructuralLoadLinearForce( int id );
	~IfcStructuralLoadLinearForce();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 7; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcStructuralLoadLinearForce"; }
	virtual const std::wstring toString() const;


	// IfcStructuralLoad -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>				m_Name;						//optional

	// IfcStructuralLoadOrResult -----------------------------------------------------------

	// IfcStructuralLoadStatic -----------------------------------------------------------

	// IfcStructuralLoadLinearForce -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcLinearForceMeasure>	m_LinearForceX;				//optional
	shared_ptr<IfcLinearForceMeasure>	m_LinearForceY;				//optional
	shared_ptr<IfcLinearForceMeasure>	m_LinearForceZ;				//optional
	shared_ptr<IfcLinearMomentMeasure>	m_LinearMomentX;			//optional
	shared_ptr<IfcLinearMomentMeasure>	m_LinearMomentY;			//optional
	shared_ptr<IfcLinearMomentMeasure>	m_LinearMomentZ;			//optional
};
