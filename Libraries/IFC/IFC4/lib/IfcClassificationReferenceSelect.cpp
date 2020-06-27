/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcClassificationReferenceSelect.h"

// TYPE IfcClassificationReferenceSelect = SELECT	(IfcClassification	,IfcClassificationReference);
shared_ptr<IfcClassificationReferenceSelect> IfcClassificationReferenceSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcClassificationReferenceSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcClassificationReferenceSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcClassificationReferenceSelect>();
	}
	shared_ptr<IfcClassificationReferenceSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}