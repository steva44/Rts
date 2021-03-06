/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcLengthMeasure.h"
#include "IFC4/include/IfcPlaneAngleMeasure.h"
#include "IFC4/include/IfcBendingParameterSelect.h"

// TYPE IfcBendingParameterSelect = SELECT	(IfcLengthMeasure	,IfcPlaneAngleMeasure);
shared_ptr<IfcBendingParameterSelect> IfcBendingParameterSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcBendingParameterSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcBendingParameterSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcBendingParameterSelect>();
	}
	shared_ptr<IfcBendingParameterSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}
