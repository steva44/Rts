#ifndef RAtkinson1997IntensityModel_H	
#define RAtkinson1997IntensityModel_H	
/*********************************************************************
*                                                                    *
* This file is posted by Dr. Stevan Gavrilovic (steva44@hotmail.com) *
* as a result of work performed in the research group of Professor   *
* Terje Haukaas (terje@civil.ubc.ca) at the University of British    *
* Columbia in Vancouver. The code is part of the computer program    *
* Rts, which is an extension of the computer program Rt developed by *
* Dr. Mojtaba Mahsuli (mahsuli@sharif.edu) in the same group.        *
*                                                                    *
* The content of this file is the product of thesis-related work     *
* performed at the University of British Columbia (UBC). Therefore,  *
* the Rights and Ownership are held by UBC.                          *
*                                                                    *
* Please be cautious when using this code. It is provided “as is”    *
* without warranty of any kind, express or implied.                  *
*                                                                    *
* Contributors to this file:                                         *
*   - Mojtaba Mahsuli                                                *
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/
	
#include "RModel.h"	
#include "REarthquakeBaseClass.h"
	
class RParameter;	
class RResponse;
class RLocation;
	
class RAtkinson1997IntensityModel : public REarthquakeBaseClass
{	
	Q_OBJECT
	Q_PROPERTY(QObject *Magnitude READ getMagnitude WRITE setMagnitude)
    Q_PROPERTY(QObject *HypocentreLocation READ getHypocentreLocation WRITE setHypocentreLocation)
	Q_PROPERTY(RSoilType SoilType READ getSoilType WRITE setSoilType)
	Q_PROPERTY(QObject *Epsilon READ getEpsilon WRITE setEpsilon)

	// Model Error Parameters
	Q_PROPERTY(QObject *C0Uncertainty READ getC0Uncertainty WRITE setC0Uncertainty)
	Q_PROPERTY(QObject *C1Uncertainty READ getC1Uncertainty WRITE setC1Uncertainty)
	Q_PROPERTY(QObject *C2Uncertainty READ getC2Uncertainty WRITE setC2Uncertainty)
	Q_PROPERTY(QObject *C3Uncertainty READ getC3Uncertainty WRITE setC3Uncertainty)
	Q_PROPERTY(QObject *Ca3Uncertainty READ getCa3Uncertainty WRITE setCa3Uncertainty)
	Q_PROPERTY(QObject *Ca4Uncertainty READ getCa4Uncertainty WRITE setCa4Uncertainty)
	Q_PROPERTY(QObject *CsigmaUncertainty READ getCsigmaUncertainty WRITE setCsigmaUncertainty)
	Q_PROPERTY(QObject *CsUncertainty READ getCsUncertainty WRITE setCsUncertainty)

	Q_ENUMS(RSoilType)

public:	
	RAtkinson1997IntensityModel(QObject *parent, QString name);
	~RAtkinson1997IntensityModel();

	enum RSoilType{Rock, FirmSoil};
	
    QObject *getHypocentreLocation() const;
    void setHypocentreLocation(QObject *value);
	QObject *getMagnitude() const;
	void setMagnitude(QObject *value);
	RSoilType getSoilType() const;
	void setSoilType(RSoilType value);
	QObject *getEpsilon() const;
	void setEpsilon(QObject *value);

	QObject *getC0Uncertainty() const;
	void setC0Uncertainty(QObject *value);
	QObject *getC1Uncertainty() const;
	void setC1Uncertainty(QObject *value);
	QObject *getC2Uncertainty() const;
	void setC2Uncertainty(QObject *value);
	QObject *getC3Uncertainty() const;
	void setC3Uncertainty(QObject *value);
	QObject *getCa3Uncertainty() const;
	void setCa3Uncertainty(QObject *value);
	QObject *getCa4Uncertainty() const;
	void setCa4Uncertainty(QObject *value);
	QObject *getCsigmaUncertainty() const;
	void setCsigmaUncertainty(QObject *value);
	QObject *getCsUncertainty() const;
	void setCsUncertainty(QObject *value);

    bool isOn();
    int evaluateModel(RGradientType theGradientType);
	
private:	

    QPointer<RParameter> theHypocentreLocation;
	QPointer<RParameter> theMagnitude;
	RSoilType theSoilType;
	QPointer<RParameter> theEpsilon;

	QPointer<RParameter> theC0Uncertainty;
	QPointer<RParameter> theC1Uncertainty;
	QPointer<RParameter> theC2Uncertainty;
	QPointer<RParameter> theC3Uncertainty;
	QPointer<RParameter> theCa3Uncertainty;
	QPointer<RParameter> theCa4Uncertainty;
	QPointer<RParameter> theCsigmaUncertainty;
	QPointer<RParameter> theCsUncertainty;

    RResponse *theResponse;
};
	
#endif
