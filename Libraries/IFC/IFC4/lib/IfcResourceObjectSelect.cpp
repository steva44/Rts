/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcResourceObjectSelect.h"

// TYPE IfcResourceObjectSelect = SELECT	(IfcActorRole	,IfcAppliedValue	,IfcApproval	,IfcConstraint	,IfcContextDependentUnit	,IfcConversionBasedUnit	,IfcExternalInformation	,IfcExternalReference	,IfcMaterialDefinition	,IfcOrganization	,IfcPerson	,IfcPersonAndOrganization	,IfcPhysicalQuantity	,IfcProfileDef	,IfcPropertyAbstraction	,IfcTimeSeries);
shared_ptr<IfcResourceObjectSelect> IfcResourceObjectSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcResourceObjectSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcResourceObjectSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcResourceObjectSelect>();
	}
	shared_ptr<IfcResourceObjectSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}
