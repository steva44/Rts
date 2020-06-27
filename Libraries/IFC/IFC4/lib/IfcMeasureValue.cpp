/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <map>
#include "model/IfcPPBasicTypes.h"
#include "model/IfcPPException.h"
#include "reader/ReaderUtil.h"
#include "IFC4/include/IfcAmountOfSubstanceMeasure.h"
#include "IFC4/include/IfcAreaMeasure.h"
#include "IFC4/include/IfcComplexNumber.h"
#include "IFC4/include/IfcContextDependentMeasure.h"
#include "IFC4/include/IfcCountMeasure.h"
#include "IFC4/include/IfcDescriptiveMeasure.h"
#include "IFC4/include/IfcElectricCurrentMeasure.h"
#include "IFC4/include/IfcLengthMeasure.h"
#include "IFC4/include/IfcLuminousIntensityMeasure.h"
#include "IFC4/include/IfcMassMeasure.h"
#include "IFC4/include/IfcNonNegativeLengthMeasure.h"
#include "IFC4/include/IfcNormalisedRatioMeasure.h"
#include "IFC4/include/IfcNumericMeasure.h"
#include "IFC4/include/IfcParameterValue.h"
#include "IFC4/include/IfcPlaneAngleMeasure.h"
#include "IFC4/include/IfcPositiveLengthMeasure.h"
#include "IFC4/include/IfcPositivePlaneAngleMeasure.h"
#include "IFC4/include/IfcPositiveRatioMeasure.h"
#include "IFC4/include/IfcRatioMeasure.h"
#include "IFC4/include/IfcSolidAngleMeasure.h"
#include "IFC4/include/IfcThermodynamicTemperatureMeasure.h"
#include "IFC4/include/IfcTimeMeasure.h"
#include "IFC4/include/IfcVolumeMeasure.h"
#include "IFC4/include/IfcValue.h"
#include "IFC4/include/IfcMeasureValue.h"

// TYPE IfcMeasureValue = SELECT	(IfcAmountOfSubstanceMeasure	,IfcAreaMeasure	,IfcComplexNumber	,IfcContextDependentMeasure	,IfcCountMeasure	,IfcDescriptiveMeasure	,IfcElectricCurrentMeasure	,IfcLengthMeasure	,IfcLuminousIntensityMeasure	,IfcMassMeasure	,IfcNonNegativeLengthMeasure	,IfcNormalisedRatioMeasure	,IfcNumericMeasure	,IfcParameterValue	,IfcPlaneAngleMeasure	,IfcPositiveLengthMeasure	,IfcPositivePlaneAngleMeasure	,IfcPositiveRatioMeasure	,IfcRatioMeasure	,IfcSolidAngleMeasure	,IfcThermodynamicTemperatureMeasure	,IfcTimeMeasure	,IfcVolumeMeasure);
shared_ptr<IfcMeasureValue> IfcMeasureValue::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	if( arg.size() == 0 ){ return shared_ptr<IfcMeasureValue>(); }
	if( arg.compare(L"$")==0 )
	{
		return shared_ptr<IfcMeasureValue>();
	}
	if( arg.compare(L"*")==0 )
	{
		return shared_ptr<IfcMeasureValue>();
	}
	shared_ptr<IfcMeasureValue> result_object;
	readSelectType( arg, result_object, map );
	return result_object;
}
