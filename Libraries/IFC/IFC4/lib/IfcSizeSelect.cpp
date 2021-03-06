/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcDescriptiveMeasure.h"
#include "IFC4/include/IfcLengthMeasure.h"
#include "IFC4/include/IfcNormalisedRatioMeasure.h"
#include "IFC4/include/IfcPositiveLengthMeasure.h"
#include "IFC4/include/IfcPositiveRatioMeasure.h"
#include "IFC4/include/IfcRatioMeasure.h"
#include "IFC4/include/IfcSizeSelect.h"

// TYPE IfcSizeSelect = SELECT	(IfcDescriptiveMeasure	,IfcLengthMeasure	,IfcNormalisedRatioMeasure	,IfcPositiveLengthMeasure	,IfcPositiveRatioMeasure	,IfcRatioMeasure);
shared_ptr<IfcSizeSelect> IfcSizeSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcSizeSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcSizeSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcSizeSelect>();
	}
	shared_ptr<IfcSizeSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}
