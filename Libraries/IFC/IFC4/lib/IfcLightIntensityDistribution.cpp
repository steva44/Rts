/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "model/IfcPPException.h"
#include "model/IfcPPAttributeObject.h"
#include "model/IfcPPGuid.h"
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "IFC4/include/IfcLightDistributionCurveEnum.h"
#include "IFC4/include/IfcLightDistributionData.h"
#include "IFC4/include/IfcLightIntensityDistribution.h"

// ENTITY IfcLightIntensityDistribution 
IfcLightIntensityDistribution::IfcLightIntensityDistribution() {}
IfcLightIntensityDistribution::IfcLightIntensityDistribution( int id ) { m_entity_id = id; }
IfcLightIntensityDistribution::~IfcLightIntensityDistribution() {}
shared_ptr<IfcPPObject> IfcLightIntensityDistribution::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLightIntensityDistribution> copy_self( new IfcLightIntensityDistribution() );
	if( m_LightDistributionCurve ) { copy_self->m_LightDistributionCurve = dynamic_pointer_cast<IfcLightDistributionCurveEnum>( m_LightDistributionCurve->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_DistributionData.size(); ++ii )
	{
		auto item_ii = m_DistributionData[ii];
		if( item_ii )
		{
			copy_self->m_DistributionData.push_back( dynamic_pointer_cast<IfcLightDistributionData>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcLightIntensityDistribution::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCLIGHTINTENSITYDISTRIBUTION" << "(";
	if( m_LightDistributionCurve ) { m_LightDistributionCurve->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_DistributionData );
	stream << ");";
}
void IfcLightIntensityDistribution::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_entity_id; }
const std::wstring IfcLightIntensityDistribution::toString() const { return L"IfcLightIntensityDistribution"; }
void IfcLightIntensityDistribution::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcLightIntensityDistribution, expecting 2, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_LightDistributionCurve = IfcLightDistributionCurveEnum::createObjectFromSTEP( args[0], map );
	readEntityReferenceList( args[1], m_DistributionData, map );
}
void IfcLightIntensityDistribution::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "LightDistributionCurve", m_LightDistributionCurve ) );
	if( m_DistributionData.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> DistributionData_vec_object( new IfcPPAttributeObjectVector() );
		std::copy( m_DistributionData.begin(), m_DistributionData.end(), std::back_inserter( DistributionData_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "DistributionData", DistributionData_vec_object ) );
	}
}
void IfcLightIntensityDistribution::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcLightIntensityDistribution::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcLightIntensityDistribution::unlinkFromInverseCounterparts()
{
}
