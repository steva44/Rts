/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "reader/ReaderUtil.h"
#include "writer/WriterUtil.h"
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "IFC4/include/IfcDerivedMeasureValue.h"
#include "IFC4/include/IfcModulusOfTranslationalSubgradeReactionSelect.h"
#include "IFC4/include/IfcModulusOfLinearSubgradeReactionMeasure.h"

// TYPE IfcModulusOfLinearSubgradeReactionMeasure = REAL;
IfcModulusOfLinearSubgradeReactionMeasure::IfcModulusOfLinearSubgradeReactionMeasure() {}
IfcModulusOfLinearSubgradeReactionMeasure::IfcModulusOfLinearSubgradeReactionMeasure( double value ) { m_value = value; }
IfcModulusOfLinearSubgradeReactionMeasure::~IfcModulusOfLinearSubgradeReactionMeasure() {}
shared_ptr<IfcPPObject> IfcModulusOfLinearSubgradeReactionMeasure::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcModulusOfLinearSubgradeReactionMeasure> copy_self( new IfcModulusOfLinearSubgradeReactionMeasure() );
	copy_self->m_value = m_value;
	return copy_self;
}
void IfcModulusOfLinearSubgradeReactionMeasure::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCMODULUSOFLINEARSUBGRADEREACTIONMEASURE("; }
	stream << m_value;
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcModulusOfLinearSubgradeReactionMeasure::toString() const
{
	std::wstringstream strs;
	strs << m_value;
	return strs.str();
}
shared_ptr<IfcModulusOfLinearSubgradeReactionMeasure> IfcModulusOfLinearSubgradeReactionMeasure::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcModulusOfLinearSubgradeReactionMeasure>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcModulusOfLinearSubgradeReactionMeasure>(); }
	shared_ptr<IfcModulusOfLinearSubgradeReactionMeasure> type_object( new IfcModulusOfLinearSubgradeReactionMeasure() );
	readReal( arg, type_object->m_value );
	return type_object;
}