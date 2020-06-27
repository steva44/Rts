#ifndef RRandomVariableWithRandomParametersModel_H
#define RRandomVariableWithRandomParametersModel_H
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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RModel.h"
#include "RProbabilityDistributions.h"

class RParameter;
class RResponse;

class RRandomVariableWithRandomParametersModel : public RModel
{
	Q_OBJECT
	Q_PROPERTY(QObject *StandardNormalRandomVariable READ getStandardNormalRandomVariable WRITE setStandardNormalRandomVariable)
	Q_PROPERTY(QString DistributionType READ getDistributionType WRITE setDistributionType)
	Q_PROPERTY(RRandomParameterType RandomParameterType READ getRandomParameterType WRITE setRandomParameterType)
	Q_PROPERTY(QString ParameterList READ getParameterList WRITE setParameterList)
    Q_PROPERTY(QObject *ProbabilityDistributions READ getProbabilityDistributions WRITE setProbabilityDistributions)
    Q_PROPERTY(QObject *Occurrence READ getOccurrence WRITE setOccurrence)

	Q_ENUMS(RRandomParameterType)

public:
	RRandomVariableWithRandomParametersModel(QObject *parent, QString name);
	~RRandomVariableWithRandomParametersModel();

	enum RRandomParameterType{Parameter1ToParameter4, MeanStdvParameter3Parameter4};

	QObject *getStandardNormalRandomVariable() const;
	void setStandardNormalRandomVariable(QObject *value);

	QString getDistributionType() const;
	void setDistributionType(QString value);

	RRandomParameterType getRandomParameterType() const;
	void setRandomParameterType(RRandomParameterType value);

	QString getParameterList();
	void setParameterList(QString value);

    QObject *getProbabilityDistributions() const;
    void setProbabilityDistributions(QObject *value);

    QObject *getOccurrence() const;
    void setOccurrence(QObject *value);

	// ATTENTION: It is assumed that theStandardNormalRandomVariable is the "physical parameter" that should be sampled over time in a scenario sampling analysis.
	QList<RParameter *> getPhysicalParameterList();

    int evaluateModel(RGradientType theGradientType);

	void setDynamicProperty(QString propertyName, QVariant propertyValue);

private:
	QPointer<RParameter> theStandardNormalRandomVariable;
	QString theDistributionType;
	RRandomParameterType theRandomParameterType;
	QList<QPointer<RParameter> > theParameterList;
    QPointer<RProbabilityDistributions> theProbabilityDistributions;
    QPointer<RParameter> theOccurrence;
	RResponse *theResponse;

};

#endif
