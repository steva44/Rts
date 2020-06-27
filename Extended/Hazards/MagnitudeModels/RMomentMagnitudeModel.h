#ifndef RMomentMagnitudeModel_H
#define RMomentMagnitudeModel_H
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

class RParameter;
class RResponse;

class RMomentMagnitudeModel : public RModel
{
	Q_OBJECT
	Q_PROPERTY(QObject *ShearModulus READ getShearModulus WRITE setShearModulus)
	Q_PROPERTY(QObject *RuptureArea READ getRuptureArea WRITE setRuptureArea)
	Q_PROPERTY(QObject *RuptureDisplacement READ getRuptureDisplacement WRITE setRuptureDisplacement)
	Q_PROPERTY(QObject *Theta1 READ getTheta1 WRITE setTheta1)
	Q_PROPERTY(QObject *Theta2 READ getTheta2 WRITE setTheta2)
	Q_PROPERTY(QObject *Epsilon READ getEpsilon WRITE setEpsilon)
    Q_PROPERTY(QObject *Occurrence READ getOccurrence WRITE setOccurrence)

public:
	RMomentMagnitudeModel(QObject *parent, QString name);
	~RMomentMagnitudeModel();

	// Define the mandatory read/write methods for all QProperties
	QObject *getShearModulus() const;
	void setShearModulus(QObject *value);
	QObject *getRuptureArea() const;
	void setRuptureArea(QObject *value);
	QObject *getRuptureDisplacement() const;
	void setRuptureDisplacement(QObject *value);
	QObject *getTheta1() const;
	void setTheta1(QObject *value);
	QObject *getTheta2() const;
	void setTheta2(QObject *value);
	QObject *getEpsilon() const;
	void setEpsilon(QObject *value);
    QObject *getOccurrence() const;
    void setOccurrence(QObject *value);

	QList<RParameter *> getPhysicalParameterList();

	// Define the method where the actual model is coded
	int evaluateModel(RGradientType theGradientType);

private:
	// Declare input data members
	QPointer<RParameter> theShearModulus;
	QPointer<RParameter> theRuptureArea;
	QPointer<RParameter> theRuptureDisplacement;
	QPointer<RParameter> theTheta1;
	QPointer<RParameter> theTheta2;
	QPointer<RParameter> theEpsilon;
    QPointer<RParameter> theOccurrence;

	// Declare responses
	RResponse *theMomentMagnitude;

};

#endif // RMomentMagnitudeModel_H
