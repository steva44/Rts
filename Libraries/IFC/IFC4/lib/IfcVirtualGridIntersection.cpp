/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcGridAxis.h"
#include "IFC4/include/IfcLengthMeasure.h"
#include "IFC4/include/IfcVirtualGridIntersection.h"

// ENTITY IfcVirtualGridIntersection 
IfcVirtualGridIntersection::IfcVirtualGridIntersection() {}
IfcVirtualGridIntersection::IfcVirtualGridIntersection( int id ) { m_entity_id = id; }
IfcVirtualGridIntersection::~IfcVirtualGridIntersection() {}
shared_ptr<IfcPPObject> IfcVirtualGridIntersection::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcVirtualGridIntersection> copy_self( new IfcVirtualGridIntersection() );
	for( size_t ii=0; ii<m_IntersectingAxes.size(); ++ii )
	{
		auto item_ii = m_IntersectingAxes[ii];
		if( item_ii )
		{
			copy_self->m_IntersectingAxes.push_back( dynamic_pointer_cast<IfcGridAxis>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_OffsetDistances.size(); ++ii )
	{
		auto item_ii = m_OffsetDistances[ii];
		if( item_ii )
		{
			copy_self->m_OffsetDistances.push_back( dynamic_pointer_cast<IfcLengthMeasure>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcVirtualGridIntersection::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCVIRTUALGRIDINTERSECTION" << "(";
	writeEntityList( stream, m_IntersectingAxes );
	stream << ",";
	writeNumericTypeList( stream, m_OffsetDistances );
	stream << ");";
}
void IfcVirtualGridIntersection::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcVirtualGridIntersection::toString() const { return L"IfcVirtualGridIntersection"; }
void IfcVirtualGridIntersection::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcVirtualGridIntersection, expecting 2, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_IntersectingAxes, map );
	readTypeOfRealList( args[1], m_OffsetDistances );
}
void IfcVirtualGridIntersection::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	if( m_IntersectingAxes.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> IntersectingAxes_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_IntersectingAxes.begin(), m_IntersectingAxes.end(), std::back_inserter( IntersectingAxes_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "IntersectingAxes", IntersectingAxes_vec_object ) );
	}
	if( m_OffsetDistances.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> OffsetDistances_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_OffsetDistances.begin(), m_OffsetDistances.end(), std::back_inserter( OffsetDistances_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "OffsetDistances", OffsetDistances_vec_object ) );
	}
}
void IfcVirtualGridIntersection::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcVirtualGridIntersection::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	shared_ptr<IfcVirtualGridIntersection> ptr_self = dynamic_pointer_cast<IfcVirtualGridIntersection>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcVirtualGridIntersection::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_IntersectingAxes.size(); ++i )
	{
		if( m_IntersectingAxes[i] )
		{
			m_IntersectingAxes[i]->m_HasIntersections_inverse.push_back( ptr_self );
		}
	}
}
void IfcVirtualGridIntersection::unlinkFromInverseCounterparts()
{
	for( size_t i=0; i<m_IntersectingAxes.size(); ++i )
	{
		if( m_IntersectingAxes[i] )
		{
			std::vector<weak_ptr<IfcVirtualGridIntersection> >& HasIntersections_inverse = m_IntersectingAxes[i]->m_HasIntersections_inverse;
			for( auto it_HasIntersections_inverse = HasIntersections_inverse.begin(); it_HasIntersections_inverse != HasIntersections_inverse.end(); )
			{
				shared_ptr<IfcVirtualGridIntersection> self_candidate( *it_HasIntersections_inverse );
				if( self_candidate.get() == this )
				{
					it_HasIntersections_inverse= HasIntersections_inverse.erase( it_HasIntersections_inverse );
				}
				else
				{
					++it_HasIntersections_inverse;
				}
			}
		}
	}
}