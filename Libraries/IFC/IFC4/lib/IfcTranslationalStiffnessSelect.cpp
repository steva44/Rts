/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcBoolean.h"
#include "IFC4/include/IfcLinearStiffnessMeasure.h"
#include "IFC4/include/IfcTranslationalStiffnessSelect.h"

// TYPE IfcTranslationalStiffnessSelect = SELECT	(IfcBoolean	,IfcLinearStiffnessMeasure);
shared_ptr<IfcTranslationalStiffnessSelect> IfcTranslationalStiffnessSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcTranslationalStiffnessSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcTranslationalStiffnessSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcTranslationalStiffnessSelect>();
	}
	shared_ptr<IfcTranslationalStiffnessSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}