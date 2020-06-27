/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "IfcPPBasicTypes.h"
#include "IfcPPObject.h"
#include "IfcPPGlobal.h"
#include "IfcPresentationStyleSelect.h"
#include "IfcPresentationStyle.h"
class IFCPP_EXPORT IfcTextStyleForDefinedFont;
class IFCPP_EXPORT IfcTextStyleTextModel;
class IFCPP_EXPORT IfcTextFontSelect;
class IFCPP_EXPORT IfcBoolean;
//ENTITY
class IFCPP_EXPORT IfcTextStyle : virtual public IfcPresentationStyleSelect, public IfcPresentationStyle
{ 
public:
	IfcTextStyle();
	IfcTextStyle( int id );
	~IfcTextStyle();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual size_t getNumAttributes() { return 5; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcTextStyle"; }
	virtual const std::wstring toString() const;


	// IfcPresentationStyle -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>					m_Name;						//optional

	// IfcTextStyle -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcTextStyleForDefinedFont>	m_TextCharacterAppearance;	//optional
	shared_ptr<IfcTextStyleTextModel>		m_TextStyle;				//optional
	shared_ptr<IfcTextFontSelect>			m_TextFontStyle;
	shared_ptr<IfcBoolean>					m_ModelOrDraughting;		//optional
};
