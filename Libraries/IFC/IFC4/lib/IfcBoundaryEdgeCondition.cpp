/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcBoundaryEdgeCondition.h"
#include "IFC4/include/IfcLabel.h"
#include "IFC4/include/IfcModulusOfRotationalSubgradeReactionSelect.h"
#include "IFC4/include/IfcModulusOfTranslationalSubgradeReactionSelect.h"

// ENTITY IfcBoundaryEdgeCondition 
IfcBoundaryEdgeCondition::IfcBoundaryEdgeCondition() {}
IfcBoundaryEdgeCondition::IfcBoundaryEdgeCondition( int id ) { m_entity_id = id; }
IfcBoundaryEdgeCondition::~IfcBoundaryEdgeCondition() {}
shared_ptr<IfcPPObject> IfcBoundaryEdgeCondition::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBoundaryEdgeCondition> copy_self( new IfcBoundaryEdgeCondition() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_TranslationalStiffnessByLengthX ) { copy_self->m_TranslationalStiffnessByLengthX = dynamic_pointer_cast<IfcModulusOfTranslationalSubgradeReactionSelect>( m_TranslationalStiffnessByLengthX->getDeepCopy(options) ); }
	if( m_TranslationalStiffnessByLengthY ) { copy_self->m_TranslationalStiffnessByLengthY = dynamic_pointer_cast<IfcModulusOfTranslationalSubgradeReactionSelect>( m_TranslationalStiffnessByLengthY->getDeepCopy(options) ); }
	if( m_TranslationalStiffnessByLengthZ ) { copy_self->m_TranslationalStiffnessByLengthZ = dynamic_pointer_cast<IfcModulusOfTranslationalSubgradeReactionSelect>( m_TranslationalStiffnessByLengthZ->getDeepCopy(options) ); }
	if( m_RotationalStiffnessByLengthX ) { copy_self->m_RotationalStiffnessByLengthX = dynamic_pointer_cast<IfcModulusOfRotationalSubgradeReactionSelect>( m_RotationalStiffnessByLengthX->getDeepCopy(options) ); }
	if( m_RotationalStiffnessByLengthY ) { copy_self->m_RotationalStiffnessByLengthY = dynamic_pointer_cast<IfcModulusOfRotationalSubgradeReactionSelect>( m_RotationalStiffnessByLengthY->getDeepCopy(options) ); }
	if( m_RotationalStiffnessByLengthZ ) { copy_self->m_RotationalStiffnessByLengthZ = dynamic_pointer_cast<IfcModulusOfRotationalSubgradeReactionSelect>( m_RotationalStiffnessByLengthZ->getDeepCopy(options) ); }
	return copy_self;
}
void IfcBoundaryEdgeCondition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCBOUNDARYEDGECONDITION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TranslationalStiffnessByLengthX ) { m_TranslationalStiffnessByLengthX->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_TranslationalStiffnessByLengthY ) { m_TranslationalStiffnessByLengthY->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_TranslationalStiffnessByLengthZ ) { m_TranslationalStiffnessByLengthZ->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_RotationalStiffnessByLengthX ) { m_RotationalStiffnessByLengthX->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_RotationalStiffnessByLengthY ) { m_RotationalStiffnessByLengthY->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_RotationalStiffnessByLengthZ ) { m_RotationalStiffnessByLengthZ->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcBoundaryEdgeCondition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcBoundaryEdgeCondition::toString() const { return L"IfcBoundaryEdgeCondition"; }
void IfcBoundaryEdgeCondition::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcBoundaryEdgeCondition, expecting 7, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
	m_TranslationalStiffnessByLengthX = IfcModulusOfTranslationalSubgradeReactionSelect::createObjectFromSTEP( args[1], map );
	m_TranslationalStiffnessByLengthY = IfcModulusOfTranslationalSubgradeReactionSelect::createObjectFromSTEP( args[2], map );
	m_TranslationalStiffnessByLengthZ = IfcModulusOfTranslationalSubgradeReactionSelect::createObjectFromSTEP( args[3], map );
	m_RotationalStiffnessByLengthX = IfcModulusOfRotationalSubgradeReactionSelect::createObjectFromSTEP( args[4], map );
	m_RotationalStiffnessByLengthY = IfcModulusOfRotationalSubgradeReactionSelect::createObjectFromSTEP( args[5], map );
	m_RotationalStiffnessByLengthZ = IfcModulusOfRotationalSubgradeReactionSelect::createObjectFromSTEP( args[6], map );
}
void IfcBoundaryEdgeCondition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundaryCondition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessByLengthX", m_TranslationalStiffnessByLengthX ) );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessByLengthY", m_TranslationalStiffnessByLengthY ) );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessByLengthZ", m_TranslationalStiffnessByLengthZ ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessByLengthX", m_RotationalStiffnessByLengthX ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessByLengthY", m_RotationalStiffnessByLengthY ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessByLengthZ", m_RotationalStiffnessByLengthZ ) );
}
void IfcBoundaryEdgeCondition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundaryCondition::getAttributesInverse( vec_attributes_inverse );
}
void IfcBoundaryEdgeCondition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundaryCondition::setInverseCounterparts( ptr_self_entity );
}
void IfcBoundaryEdgeCondition::unlinkFromInverseCounterparts()
{
	IfcBoundaryCondition::unlinkFromInverseCounterparts();
}