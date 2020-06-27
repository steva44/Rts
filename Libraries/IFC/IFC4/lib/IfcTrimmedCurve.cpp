/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcBoolean.h"
#include "IFC4/include/IfcCurve.h"
#include "IFC4/include/IfcPresentationLayerAssignment.h"
#include "IFC4/include/IfcStyledItem.h"
#include "IFC4/include/IfcTrimmedCurve.h"
#include "IFC4/include/IfcTrimmingPreference.h"
#include "IFC4/include/IfcTrimmingSelect.h"

// ENTITY IfcTrimmedCurve 
IfcTrimmedCurve::IfcTrimmedCurve() {}
IfcTrimmedCurve::IfcTrimmedCurve( int id ) { m_entity_id = id; }
IfcTrimmedCurve::~IfcTrimmedCurve() {}
shared_ptr<IfcPPObject> IfcTrimmedCurve::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTrimmedCurve> copy_self( new IfcTrimmedCurve() );
	if( m_BasisCurve ) { copy_self->m_BasisCurve = dynamic_pointer_cast<IfcCurve>( m_BasisCurve->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Trim1.size(); ++ii )
	{
		auto item_ii = m_Trim1[ii];
		if( item_ii )
		{
			copy_self->m_Trim1.push_back( dynamic_pointer_cast<IfcTrimmingSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_Trim2.size(); ++ii )
	{
		auto item_ii = m_Trim2[ii];
		if( item_ii )
		{
			copy_self->m_Trim2.push_back( dynamic_pointer_cast<IfcTrimmingSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_SenseAgreement ) { copy_self->m_SenseAgreement = dynamic_pointer_cast<IfcBoolean>( m_SenseAgreement->getDeepCopy(options) ); }
	if( m_MasterRepresentation ) { copy_self->m_MasterRepresentation = dynamic_pointer_cast<IfcTrimmingPreference>( m_MasterRepresentation->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTrimmedCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCTRIMMEDCURVE" << "(";
	if( m_BasisCurve ) { stream << "#" << m_BasisCurve->m_entity_id; } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_Trim1.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcTrimmingSelect>& type_object = m_Trim1[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_Trim2.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcTrimmingSelect>& type_object = m_Trim2[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_SenseAgreement ) { m_SenseAgreement->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MasterRepresentation ) { m_MasterRepresentation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTrimmedCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcTrimmedCurve::toString() const { return L"IfcTrimmedCurve"; }
void IfcTrimmedCurve::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTrimmedCurve, expecting 5, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_BasisCurve, map );
	readSelectList( args[1], m_Trim1, map );
	readSelectList( args[2], m_Trim2, map );
	m_SenseAgreement = IfcBoolean::createObjectFromSTEP( args[3], map );
	m_MasterRepresentation = IfcTrimmingPreference::createObjectFromSTEP( args[4], map );
}
void IfcTrimmedCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedCurve::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisCurve", m_BasisCurve ) );
	if( m_Trim1.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Trim1_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_Trim1.begin(), m_Trim1.end(), std::back_inserter( Trim1_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Trim1", Trim1_vec_object ) );
	}
	if( m_Trim2.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Trim2_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_Trim2.begin(), m_Trim2.end(), std::back_inserter( Trim2_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Trim2", Trim2_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "SenseAgreement", m_SenseAgreement ) );
	vec_attributes.push_back( std::make_pair( "MasterRepresentation", m_MasterRepresentation ) );
}
void IfcTrimmedCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcTrimmedCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcTrimmedCurve::unlinkFromInverseCounterparts()
{
	IfcBoundedCurve::unlinkFromInverseCounterparts();
}
