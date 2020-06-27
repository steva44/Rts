/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcFillStyleSelect.h"
#include "IFC4/include/IfcColour.h"

// TYPE IfcColour = SELECT	(IfcColourSpecification	,IfcPreDefinedColour);
shared_ptr<IfcColour> IfcColour::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcColour>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcColour>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcColour>();
	}
	shared_ptr<IfcColour> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}
