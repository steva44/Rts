#pragma once
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPObject.h"

class IFCPP_EXPORT IfcPPEntityFactory
{
public:
	static IfcPPEntity* createEntityObject( const std::string& class_name_upper );
};
