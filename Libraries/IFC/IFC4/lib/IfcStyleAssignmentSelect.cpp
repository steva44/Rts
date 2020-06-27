/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcStyleAssignmentSelect.h"

// TYPE IfcStyleAssignmentSelect = SELECT	(IfcPresentationStyle	,IfcPresentationStyleAssignment);
shared_ptr<IfcStyleAssignmentSelect> IfcStyleAssignmentSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcStyleAssignmentSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcStyleAssignmentSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcStyleAssignmentSelect>();
	}
	shared_ptr<IfcStyleAssignmentSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}