#ifndef RCorrelation_H
#define RCorrelation_H
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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RParameter.h"

class RRandomVariable;

class RCorrelation : public RParameter
{
	Q_OBJECT
    Q_PROPERTY(QObject *RandomVariable1 READ getRandomVariable1 WRITE setRandomVariable1) // Sets one of the two random variable object that are being correlated
    Q_PROPERTY(QObject *RandomVariable2 READ getRandomVariable2 WRITE setRandomVariable2) // Sets the other one of the two random variable object that are being correlated
    Q_PROPERTY(double Coefficient READ getCoefficient WRITE setCoefficient)	// Sets the correlation coefficient between RandomVariable1 and RandomVariable2

public:
	RCorrelation(QObject *parent, QString name);
	~RCorrelation();

    // Getter/setter methods for meta-properties
	QObject *getRandomVariable1() const;
	void setRandomVariable1(QObject *value);
	QObject *getRandomVariable2() const;
	void setRandomVariable2(QObject *value);
	double getCoefficient() const;
	void setCoefficient(double value);

    QStringList getActionList();

private:
	// Data member for meta-properties
	QPointer<RRandomVariable> theRandomVariable1;
	QPointer<RRandomVariable> theRandomVariable2;
	double theCoefficient;
	
};

#endif // RCorrelation_H
