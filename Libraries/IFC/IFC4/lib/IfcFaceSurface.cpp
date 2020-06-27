/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcBoolean.h"
#include "IFC4/include/IfcFaceBound.h"
#include "IFC4/include/IfcFaceSurface.h"
#include "IFC4/include/IfcPresentationLayerAssignment.h"
#include "IFC4/include/IfcStyledItem.h"
#include "IFC4/include/IfcSurface.h"
#include "IFC4/include/IfcTextureMap.h"

// ENTITY IfcFaceSurface 
IfcFaceSurface::IfcFaceSurface() {}
IfcFaceSurface::IfcFaceSurface( int id ) { m_entity_id = id; }
IfcFaceSurface::~IfcFaceSurface() {}
shared_ptr<IfcPPObject> IfcFaceSurface::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFaceSurface> copy_self( new IfcFaceSurface() );
	for( size_t ii=0; ii<m_Bounds.size(); ++ii )
	{
		auto item_ii = m_Bounds[ii];
		if( item_ii )
		{
			copy_self->m_Bounds.push_back( dynamic_pointer_cast<IfcFaceBound>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_FaceSurface ) { copy_self->m_FaceSurface = dynamic_pointer_cast<IfcSurface>( m_FaceSurface->getDeepCopy(options) ); }
	if( m_SameSense ) { copy_self->m_SameSense = dynamic_pointer_cast<IfcBoolean>( m_SameSense->getDeepCopy(options) ); }
	return copy_self;
}
void IfcFaceSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCFACESURFACE" << "(";
	writeEntityList( stream, m_Bounds );
	stream << ",";
	if( m_FaceSurface ) { stream << "#" << m_FaceSurface->m_entity_id; } else { stream << "$"; }
	stream << ",";
	if( m_SameSense ) { m_SameSense->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcFaceSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcFaceSurface::toString() const { return L"IfcFaceSurface"; }
void IfcFaceSurface::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcFaceSurface, expecting 3, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_Bounds, map );
	readEntityReference( args[1], m_FaceSurface, map );
	m_SameSense = IfcBoolean::createObjectFromSTEP( args[2], map );
}
void IfcFaceSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcFace::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "FaceSurface", m_FaceSurface ) );
	vec_attributes.push_back( std::make_pair( "SameSense", m_SameSense ) );
}
void IfcFaceSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcFace::getAttributesInverse( vec_attributes_inverse );
}
void IfcFaceSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcFace::setInverseCounterparts( ptr_self_entity );
}
void IfcFaceSurface::unlinkFromInverseCounterparts()
{
	IfcFace::unlinkFromInverseCounterparts();
}
