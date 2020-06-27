/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcDocumentSelect.h"

// TYPE IfcDocumentSelect = SELECT	(IfcDocumentInformation	,IfcDocumentReference);
shared_ptr<IfcDocumentSelect> IfcDocumentSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcDocumentSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcDocumentSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcDocumentSelect>();
	}
	shared_ptr<IfcDocumentSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}