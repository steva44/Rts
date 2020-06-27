#ifndef RStandardFORMConvergenceCheck_H
#define RStandardFORMConvergenceCheck_H
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

#include "RConvergenceCheck.h"

class RStandardFORMConvergenceCheck : public RConvergenceCheck
{
	Q_OBJECT
    Q_PROPERTY(double Tolerance1 READ getTolerance1 WRITE setTolerance1)
    Q_PROPERTY(double Tolerance2 READ getTolerance2 WRITE setTolerance2)

public:
	RStandardFORMConvergenceCheck(QObject *parent, QString name);
	~RStandardFORMConvergenceCheck();

    double getTolerance1();
    void setTolerance1(double value);

    double getTolerance2();
    void setTolerance2(double value);

    int check(QVector<double> const *u, double functionValue, QVector<double> const *gradientInStandardNormalSpace);

	double getScaleValue();
	void setScaleValue(double value);

private:

    double theTolerance1;
    double theTolerance2;
	double theScaleValue;
	
};

#endif
