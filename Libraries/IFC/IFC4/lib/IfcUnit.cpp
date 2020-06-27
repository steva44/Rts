/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcUnit.h"

// TYPE IfcUnit = SELECT	(IfcDerivedUnit	,IfcMonetaryUnit	,IfcNamedUnit);
shared_ptr<IfcUnit> IfcUnit::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcUnit>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcUnit>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcUnit>();
	}
	shared_ptr<IfcUnit> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}