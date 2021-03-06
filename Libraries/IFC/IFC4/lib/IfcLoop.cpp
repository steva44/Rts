/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcLoop.h"
#include "IFC4/include/IfcPresentationLayerAssignment.h"
#include "IFC4/include/IfcStyledItem.h"

// ENTITY IfcLoop 
IfcLoop::IfcLoop() {}
IfcLoop::IfcLoop( int id ) { m_entity_id = id; }
IfcLoop::~IfcLoop() {}
shared_ptr<IfcPPObject> IfcLoop::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLoop> copy_self( new IfcLoop() );
	return copy_self;
}
void IfcLoop::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCLOOP" << "(";
	stream << ");";
}
void IfcLoop::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcLoop::toString() const { return L"IfcLoop"; }
void IfcLoop::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcLoop::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTopologicalRepresentationItem::getAttributes( vec_attributes );
}
void IfcLoop::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTopologicalRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcLoop::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTopologicalRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcLoop::unlinkFromInverseCounterparts()
{
	IfcTopologicalRepresentationItem::unlinkFromInverseCounterparts();
}
