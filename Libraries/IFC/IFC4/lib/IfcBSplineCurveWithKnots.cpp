/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcBSplineCurveForm.h"
#include "IFC4/include/IfcBSplineCurveWithKnots.h"
#include "IFC4/include/IfcCartesianPoint.h"
#include "IFC4/include/IfcInteger.h"
#include "IFC4/include/IfcKnotType.h"
#include "IFC4/include/IfcLogical.h"
#include "IFC4/include/IfcParameterValue.h"
#include "IFC4/include/IfcPresentationLayerAssignment.h"
#include "IFC4/include/IfcStyledItem.h"

// ENTITY IfcBSplineCurveWithKnots 
IfcBSplineCurveWithKnots::IfcBSplineCurveWithKnots() {}
IfcBSplineCurveWithKnots::IfcBSplineCurveWithKnots( int id ) { m_entity_id = id; }
IfcBSplineCurveWithKnots::~IfcBSplineCurveWithKnots() {}
shared_ptr<IfcPPObject> IfcBSplineCurveWithKnots::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBSplineCurveWithKnots> copy_self( new IfcBSplineCurveWithKnots() );
	if( m_Degree ) { copy_self->m_Degree = dynamic_pointer_cast<IfcInteger>( m_Degree->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_ControlPointsList.size(); ++ii )
	{
		auto item_ii = m_ControlPointsList[ii];
		if( item_ii )
		{
			copy_self->m_ControlPointsList.push_back( dynamic_pointer_cast<IfcCartesianPoint>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_CurveForm ) { copy_self->m_CurveForm = dynamic_pointer_cast<IfcBSplineCurveForm>( m_CurveForm->getDeepCopy(options) ); }
	if( m_ClosedCurve ) { copy_self->m_ClosedCurve = dynamic_pointer_cast<IfcLogical>( m_ClosedCurve->getDeepCopy(options) ); }
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = dynamic_pointer_cast<IfcLogical>( m_SelfIntersect->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_KnotMultiplicities.size(); ++ii )
	{
		auto item_ii = m_KnotMultiplicities[ii];
		if( item_ii )
		{
			copy_self->m_KnotMultiplicities.push_back( dynamic_pointer_cast<IfcInteger>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_Knots.size(); ++ii )
	{
		auto item_ii = m_Knots[ii];
		if( item_ii )
		{
			copy_self->m_Knots.push_back( dynamic_pointer_cast<IfcParameterValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_KnotSpec ) { copy_self->m_KnotSpec = dynamic_pointer_cast<IfcKnotType>( m_KnotSpec->getDeepCopy(options) ); }
	return copy_self;
}
void IfcBSplineCurveWithKnots::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCBSPLINECURVEWITHKNOTS" << "(";
	if( m_Degree ) { m_Degree->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_ControlPointsList );
	stream << ",";
	if( m_CurveForm ) { m_CurveForm->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ClosedCurve ) { m_ClosedCurve->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeNumericTypeList( stream, m_KnotMultiplicities );
	stream << ",";
	writeNumericTypeList( stream, m_Knots );
	stream << ",";
	if( m_KnotSpec ) { m_KnotSpec->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcBSplineCurveWithKnots::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcBSplineCurveWithKnots::toString() const { return L"IfcBSplineCurveWithKnots"; }
void IfcBSplineCurveWithKnots::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 8 ){ std::stringstream err; err << "Wrong parameter count for entity IfcBSplineCurveWithKnots, expecting 8, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Degree = IfcInteger::createObjectFromSTEP( args[0], map );
	readEntityReferenceList( args[1], m_ControlPointsList, map );
	m_CurveForm = IfcBSplineCurveForm::createObjectFromSTEP( args[2], map );
	m_ClosedCurve = IfcLogical::createObjectFromSTEP( args[3], map );
	m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[4], map );
	readTypeOfIntList( args[5], m_KnotMultiplicities );
	readTypeOfRealList( args[6], m_Knots );
	m_KnotSpec = IfcKnotType::createObjectFromSTEP( args[7], map );
}
void IfcBSplineCurveWithKnots::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBSplineCurve::getAttributes( vec_attributes );
	if( m_KnotMultiplicities.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> KnotMultiplicities_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_KnotMultiplicities.begin(), m_KnotMultiplicities.end(), std::back_inserter( KnotMultiplicities_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "KnotMultiplicities", KnotMultiplicities_vec_object ) );
	}
	if( m_Knots.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Knots_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_Knots.begin(), m_Knots.end(), std::back_inserter( Knots_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Knots", Knots_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "KnotSpec", m_KnotSpec ) );
}
void IfcBSplineCurveWithKnots::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBSplineCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcBSplineCurveWithKnots::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBSplineCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcBSplineCurveWithKnots::unlinkFromInverseCounterparts()
{
	IfcBSplineCurve::unlinkFromInverseCounterparts();
}
