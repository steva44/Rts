#ifndef RContinuousDecisionVariable_H
#define RContinuousDecisionVariable_H
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

#include "RDecisionVariable.h"

class RContinuousDecisionVariable : public RDecisionVariable
{
	Q_OBJECT
    Q_PROPERTY(QString InitialValue READ getInitialValue WRITE setInitialValue) // Sets the initial value as a parameter object or a double
    Q_PROPERTY(QString UpperBound READ getUpperBound WRITE setUpperBound) // Sets the upper bound as a parameter object or a double
    Q_PROPERTY(QString LowerBound READ getLowerBound WRITE setLowerBound) 	// Sets the lower bound as a parameter object or a double
    Q_PROPERTY(QString IncrementalCost READ getIncrementalCost WRITE setIncrementalCost) // Sets the cost of increasing the decision variable by one unit as a parameter object or a double
    Q_PROPERTY(double BetaSensitivityWrtCost READ getBetaSensitivityWrtCost) // Read-only meta-property that shows the derivative of the realiability index wrt. the cost associated with the decision variable
    Q_PROPERTY(double PfSensitivityWrtCost READ getPfSensitivityWrtCost) // Read-only meta-property that shows the derivative of the failure probability wrt. the cost associated with the decision variable

public:
	RContinuousDecisionVariable(QObject *parent, QString name);
	~RContinuousDecisionVariable();

	// getter/setter methods for meta-properties
	QString getInitialValue();
	void setInitialValue(QString value);
	double getInitialValueValue();
	QString getUpperBound();
	void setUpperBound(QString value);
	double getUpperBoundValue(); 
	QString getLowerBound();
	void setLowerBound(QString value);
	double getLowerBoundValue();
	QString getIncrementalCost();
	void setIncrementalCost(QString value);
	double getIncrementalCostValue();
	double getBetaSensitivityWrtCost();
	void setBetaSensitivityWrtCost(double value);
	double getPfSensitivityWrtCost();
	void setPfSensitivityWrtCost(double value);

	// Returns a list of parameters that are input to the decision variable as lower bound, upper bound, etc.
	QList<RParameter *> getParameterList();

    QStringList getActionList();

    void setCurrentValue(double value) override;
    void setCurrentValue(std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> LambdaValue) override;

private:
	// Data members for meta-properties
	QPointer<RParameter> theInitialValueParameter;
	double theInitialValue;
	QPointer<RParameter> theUpperBoundParameter;
	double theUpperBound;
	QPointer<RParameter> theLowerBoundParameter;
	double theLowerBound;
	QPointer<RParameter> theIncrementalCostParameter;
	double theIncrementalCost;

	// Data members for sensitivities
	double theBetaSensitivityWrtCost;
	double thePfSensitivityWrtCost;

};

#endif // RContinuousDecisionVariable_H
