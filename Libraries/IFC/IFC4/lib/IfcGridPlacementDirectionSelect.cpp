/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcGridPlacementDirectionSelect.h"

// TYPE IfcGridPlacementDirectionSelect = SELECT	(IfcDirection	,IfcVirtualGridIntersection);
shared_ptr<IfcGridPlacementDirectionSelect> IfcGridPlacementDirectionSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcGridPlacementDirectionSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcGridPlacementDirectionSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcGridPlacementDirectionSelect>();
	}
	shared_ptr<IfcGridPlacementDirectionSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}
