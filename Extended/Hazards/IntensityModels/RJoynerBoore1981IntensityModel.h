#ifndef RJoynerBoore1981IntensityModel_H	
#define RJoynerBoore1981IntensityModel_H	
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
	
class RJoynerBoore1981IntensityModel : public REarthquakeBaseClass
{	
	Q_OBJECT
	Q_PROPERTY(QObject *Magnitude READ getMagnitude WRITE setMagnitude)
	Q_PROPERTY(QObject *Alpha READ getAlpha WRITE setAlpha)
	Q_PROPERTY(QObject *Beta READ getBeta WRITE setBeta)
	Q_PROPERTY(QObject *B READ getB WRITE setB)
	Q_PROPERTY(QObject *Epsilon READ getEpsilon WRITE setEpsilon)
    Q_PROPERTY(QObject *HypocentreLocation READ getHypocentreLocation WRITE setHypocentreLocation)
	
public:	
	RJoynerBoore1981IntensityModel(QObject *parent, QString name);
	~RJoynerBoore1981IntensityModel();
	
	QObject *getMagnitude() const;
	void setMagnitude(QObject *value);

	QObject *getAlpha() const;
	void setAlpha(QObject *value);

	QObject *getBeta() const;
	void setBeta(QObject *value);

	QObject *getB() const;
	void setB(QObject *value);

	QObject *getEpsilon() const;
	void setEpsilon(QObject *value);

    QObject *getHypocentreLocation() const;
    void setHypocentreLocation(QObject *value);	
	
    bool isOn();
    int evaluateModel(RGradientType theGradientType);

private:

	QPointer<RParameter> theMagnitude;
	QPointer<RParameter> theAlpha;
	QPointer<RParameter> theBeta;
    QPointer<RParameter> theB;
	QPointer<RParameter> theEpsilon;
    QPointer<RParameter> theHypocentreLocation;

    RResponse *theResponse;
};
	
#endif
