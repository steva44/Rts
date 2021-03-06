/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcPresentationItem.h"

// ENTITY IfcPresentationItem 
IfcPresentationItem::IfcPresentationItem() {}
IfcPresentationItem::IfcPresentationItem( int id ) { m_entity_id = id; }
IfcPresentationItem::~IfcPresentationItem() {}
shared_ptr<IfcPPObject> IfcPresentationItem::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPresentationItem> copy_self( new IfcPresentationItem() );
	return copy_self;
}
void IfcPresentationItem::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCPRESENTATIONITEM" << "(";
	stream << ");";
}
void IfcPresentationItem::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcPresentationItem::toString() const { return L"IfcPresentationItem"; }
void IfcPresentationItem::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcPresentationItem::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
}
void IfcPresentationItem::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPresentationItem::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcPresentationItem::unlinkFromInverseCounterparts()
{
}
