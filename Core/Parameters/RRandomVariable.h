#ifndef RRandomVariable_H
#define RRandomVariable_H
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

class RRandomVariable : public RParameter
{
	Q_OBJECT
    Q_PROPERTY(double CurrentValue READ getCurrentValue WRITE setCurrentValue) // Current realization of the random variable

public:
	RRandomVariable(QObject *parent, QString name);
	virtual ~RRandomVariable();

	// Returns the mean value of the random variable as a double
	virtual double getMeanValue() = 0;

	// Returns the stdv of the random variable as a double
	virtual double getStandardDeviationValue() = 0;

	// Returns the PDF at a given realization of the random variable
	virtual double getPDFValue(double rvValue) = 0;

	// Returns the CDF at a given realization of the random variable
	virtual double getCDFValue(double rvValue) = 0;

	// Returns the inverse CDF at a given probability value
	virtual double getInverseCDFValue(double probValue) = 0;

	// Returns a list of parameters that are input to the random variable as mean, stdv, etc.
	virtual QList<RParameter *> getParameterList() = 0;

    virtual int updateParameters();

	
private:

};

#endif
