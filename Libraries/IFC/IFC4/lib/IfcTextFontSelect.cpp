/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcTextFontSelect.h"

// TYPE IfcTextFontSelect = SELECT	(IfcExternallyDefinedTextFont	,IfcPreDefinedTextFont);
shared_ptr<IfcTextFontSelect> IfcTextFontSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcTextFontSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcTextFontSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcTextFontSelect>();
	}
	shared_ptr<IfcTextFontSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}