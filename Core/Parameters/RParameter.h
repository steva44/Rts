#ifndef RParameter_H
#define RParameter_H
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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RObject.h"

#include <functional>

class RParameter : public RObject
{
	Q_OBJECT

public:
	RParameter(QObject *parent, QString name);
	virtual ~RParameter();
	
    virtual double getCurrentValue(RParameter* theDDMParameter = nullptr, RParameter* theDDMParameter2 = nullptr) const;
    virtual double getCurrentValue(QString name) const;

    virtual void setCurrentValue(double value);
    virtual void setCurrentValue(std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> LambdaValue);

	double *getCurrentValuePointer();

    // Is this a nodal Parameter?
    void setNodalParameterFlag(bool flag);
    bool isNodalParameter();

    bool getDiscount() const;
    void setDiscount(bool value);

protected:

    double theCurrentValue;

private:

    std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2 )> theCurrentValueLambda;

    bool NodalParameterFlag;

    bool discount;

};

#endif
