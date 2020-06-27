/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcGloballyUniqueId.h"
#include "IFC4/include/IfcIdentifier.h"
#include "IFC4/include/IfcLabel.h"
#include "IFC4/include/IfcObjectPlacement.h"
#include "IFC4/include/IfcOwnerHistory.h"
#include "IFC4/include/IfcPipeFitting.h"
#include "IFC4/include/IfcPipeFittingTypeEnum.h"
#include "IFC4/include/IfcProductRepresentation.h"
#include "IFC4/include/IfcRelAggregates.h"
#include "IFC4/include/IfcRelAssigns.h"
#include "IFC4/include/IfcRelAssignsToProduct.h"
#include "IFC4/include/IfcRelAssociates.h"
#include "IFC4/include/IfcRelConnectsElements.h"
#include "IFC4/include/IfcRelConnectsPortToElement.h"
#include "IFC4/include/IfcRelConnectsWithRealizingElements.h"
#include "IFC4/include/IfcRelContainedInSpatialStructure.h"
#include "IFC4/include/IfcRelCoversBldgElements.h"
#include "IFC4/include/IfcRelDeclares.h"
#include "IFC4/include/IfcRelDefinesByObject.h"
#include "IFC4/include/IfcRelDefinesByProperties.h"
#include "IFC4/include/IfcRelDefinesByType.h"
#include "IFC4/include/IfcRelFillsElement.h"
#include "IFC4/include/IfcRelFlowControlElements.h"
#include "IFC4/include/IfcRelInterferesElements.h"
#include "IFC4/include/IfcRelNests.h"
#include "IFC4/include/IfcRelProjectsElement.h"
#include "IFC4/include/IfcRelReferencedInSpatialStructure.h"
#include "IFC4/include/IfcRelSpaceBoundary.h"
#include "IFC4/include/IfcRelVoidsElement.h"
#include "IFC4/include/IfcText.h"

// ENTITY IfcPipeFitting 
IfcPipeFitting::IfcPipeFitting() {}
IfcPipeFitting::IfcPipeFitting( int id ) { m_entity_id = id; }
IfcPipeFitting::~IfcPipeFitting() {}
shared_ptr<IfcPPObject> IfcPipeFitting::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPipeFitting> copy_self( new IfcPipeFitting() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( createGUID32_wstr().c_str() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy(options) ); }
	if( m_ObjectPlacement ) { copy_self->m_ObjectPlacement = dynamic_pointer_cast<IfcObjectPlacement>( m_ObjectPlacement->getDeepCopy(options) ); }
	if( m_Representation ) { copy_self->m_Representation = dynamic_pointer_cast<IfcProductRepresentation>( m_Representation->getDeepCopy(options) ); }
	if( m_Tag ) { copy_self->m_Tag = dynamic_pointer_cast<IfcIdentifier>( m_Tag->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcPipeFittingTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPipeFitting::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCPIPEFITTING" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_entity_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectPlacement ) { stream << "#" << m_ObjectPlacement->m_entity_id; } else { stream << "*"; }
	stream << ",";
	if( m_Representation ) { stream << "#" << m_Representation->m_entity_id; } else { stream << "*"; }
	stream << ",";
	if( m_Tag ) { m_Tag->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPipeFitting::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcPipeFitting::toString() const { return L"IfcPipeFitting"; }
void IfcPipeFitting::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 9 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPipeFitting, expecting 9, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0], map );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2], map );
	m_Description = IfcText::createObjectFromSTEP( args[3], map );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4], map );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	m_Tag = IfcIdentifier::createObjectFromSTEP( args[7], map );
	m_PredefinedType = IfcPipeFittingTypeEnum::createObjectFromSTEP( args[8], map );
}
void IfcPipeFitting::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcFlowFitting::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcPipeFitting::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcFlowFitting::getAttributesInverse( vec_attributes_inverse );
}
void IfcPipeFitting::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcFlowFitting::setInverseCounterparts( ptr_self_entity );
}
void IfcPipeFitting::unlinkFromInverseCounterparts()
{
	IfcFlowFitting::unlinkFromInverseCounterparts();
}
