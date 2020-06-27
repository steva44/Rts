/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcBoolean.h"
#include "IFC4/include/IfcModulusOfLinearSubgradeReactionMeasure.h"
#include "IFC4/include/IfcModulusOfTranslationalSubgradeReactionSelect.h"

// TYPE IfcModulusOfTranslationalSubgradeReactionSelect = SELECT	(IfcBoolean	,IfcModulusOfLinearSubgradeReactionMeasure);
shared_ptr<IfcModulusOfTranslationalSubgradeReactionSelect> IfcModulusOfTranslationalSubgradeReactionSelect::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcModulusOfTranslationalSubgradeReactionSelect>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcModulusOfTranslationalSubgradeReactionSelect>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcModulusOfTranslationalSubgradeReactionSelect>();
	}
	shared_ptr<IfcModulusOfTranslationalSubgradeReactionSelect> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}
