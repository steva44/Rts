/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcActorSelect.h"
#include "IfcObjectReferenceSelect.h"
#include "IfcResourceObjectSelect.h"
class IFCPP_EXPORT IfcPerson;
class IFCPP_EXPORT IfcOrganization;
class IFCPP_EXPORT IfcActorRole;
//ENTITY
class IFCPP_EXPORT IfcPersonAndOrganization : virtual public IfcActorSelect, virtual public IfcObjectReferenceSelect, virtual public IfcResourceObjectSelect, public IfcPPEntity
{ 
public:
	IfcPersonAndOrganization();
	IfcPersonAndOrganization( int id );
	~IfcPersonAndOrganization();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 3; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcPersonAndOrganization"; }
	virtual const std::wstring toString() const;


	// IfcPersonAndOrganization -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcPerson>					m_ThePerson;
	shared_ptr<IfcOrganization>				m_TheOrganization;
	std::vector<shared_ptr<IfcActorRole> >	m_Roles;					//optional
};
