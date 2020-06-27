#pragma once
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPObject.h"

class IFCPP_EXPORT IfcPPTypeFactory
{
public:
	static shared_ptr<IfcPPObject> createTypeObject( const std::string& class_name_upper, const std::wstring& type_arg, const std::map<int, shared_ptr<IfcPPEntity> >& map_entities );
};
