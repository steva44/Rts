#ifndef RDummyMeritFunction_H
#define RDummyMeritFunction_H
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

#include "RMeritFunction.h"


class RDummyMeritFunction : public RMeritFunction
{
	Q_OBJECT
    Q_PROPERTY(double ObjectiveMultiplier READ getObjectiveMultiplier WRITE setObjectiveMultiplier)
    Q_PROPERTY(double ObjectivePower READ getObjectivePower WRITE setObjectivePower)
    Q_PROPERTY(double ConstraintMultiplier READ getConstraintMultiplier WRITE setConstraintMultiplier)
    Q_PROPERTY(double ConstraintPower READ getConstraintPower WRITE setConstraintPower)

public:
	RDummyMeritFunction(QObject *parent, QString name);
	~RDummyMeritFunction();

    double getObjectiveMultiplier() const;
    void setObjectiveMultiplier(double value);

    double getObjectivePower() const;
    void setObjectivePower(double value);

    double getConstraintMultiplier() const;
    void setConstraintMultiplier(double value);

    double getConstraintPower() const;
    void setConstraintPower(double value);

    double getMeritFunctionValue(double objectiveFunction, QVector<double> *equalityConstraints, QVector<double> *inequalityConstraints);

private:

    double theObjectiveMultiplier;
    double theObjectivePower;
    double theConstraintMultiplier;
    double theConstraintPower;
};

#endif
