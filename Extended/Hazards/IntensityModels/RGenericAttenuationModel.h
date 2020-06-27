#ifndef RGenericAttenuationModel_H	
#define RGenericAttenuationModel_H	
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
*                                                                    *
*********************************************************************/
	
#include "RModel.h"	
#include "RIntensityModel.h"
	
class RParameter;	
class RResponse;
class RLocation;
	
class RGenericAttenuationModel : public RIntensityModel
{	
	Q_OBJECT
	Q_PROPERTY(QString MagnitudeList READ getMagnitudeList WRITE setMagnitudeList)
    Q_PROPERTY(QString HypocentreList READ getHypocentreList WRITE setHypocentreList)
	Q_PROPERTY(QObject *Theta1 READ getTheta1 WRITE setTheta1)
    Q_PROPERTY(QObject *Theta2 READ getTheta2 WRITE setTheta2)
    Q_PROPERTY(QObject *Theta3 READ getTheta3 WRITE setTheta3)
	Q_PROPERTY(QObject *Theta4 READ getTheta4 WRITE setTheta4)
    Q_PROPERTY(QObject *Epsilon READ getEpsilon WRITE setEpsilon)
	
public:	
	RGenericAttenuationModel(QObject *parent, QString name);
	~RGenericAttenuationModel();

	QString getMagnitudeList();
	void setMagnitudeList(QString value);
    QString getHypocentreList();
    void setHypocentreList(QString value);
	QObject *getTheta1() const;
	void setTheta1(QObject *value);
    QObject *getTheta2() const;
    void setTheta2(QObject *value);
    QObject *getTheta3() const;
    void setTheta3(QObject *value);
	QObject *getTheta4() const;
	void setTheta4(QObject *value);
    QObject *getEpsilon() const;
	void setEpsilon(QObject *value);

    bool isOn();
    int evaluateModel(RGradientType theGradientType);
	
private:	

	QList<QPointer<RParameter> > theMagnitudeList;
    QList<QPointer<RParameter> > theHypocentreList;
	QPointer<RParameter> theTheta1;
    QPointer<RParameter> theTheta2;
    QPointer<RParameter> theTheta3;
	QPointer<RParameter> theTheta4;
    QPointer<RParameter> theEpsilon;
		
    RResponse *theResponse;
};
	
#endif
