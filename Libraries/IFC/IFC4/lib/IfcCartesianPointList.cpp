/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcCartesianPointList.h"
#include "IFC4/include/IfcPresentationLayerAssignment.h"
#include "IFC4/include/IfcStyledItem.h"

// ENTITY IfcCartesianPointList 
IfcCartesianPointList::IfcCartesianPointList() {}
IfcCartesianPointList::IfcCartesianPointList( int id ) { m_entity_id = id; }
IfcCartesianPointList::~IfcCartesianPointList() {}
shared_ptr<IfcPPObject> IfcCartesianPointList::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCartesianPointList> copy_self( new IfcCartesianPointList() );
	return copy_self;
}
void IfcCartesianPointList::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCCARTESIANPOINTLIST" << "(";
	stream << ");";
}
void IfcCartesianPointList::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcCartesianPointList::toString() const { return L"IfcCartesianPointList"; }
void IfcCartesianPointList::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcCartesianPointList::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
}
void IfcCartesianPointList::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcCartesianPointList::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcCartesianPointList::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}
