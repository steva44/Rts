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

#include "RParameter.h"

RParameter::RParameter(QObject *parent, QString name)
    : RObject(parent, name)
{
    theCurrentValue = 0.0;
    discount = true;

    NodalParameterFlag = false;

    //the default current value lambda
    theCurrentValueLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
    {
        if(theDDMParameter != nullptr || theDDMParameter2 != nullptr)
        {
            if(theDDMParameter == this)
                return 1.0;
            else
                return 0.0;
        }

        return theCurrentValue;
    };

}


RParameter::~RParameter()
{

}


double RParameter::getCurrentValue(RParameter* theDDMParameter, RParameter* theDDMParameter2)  const
{   
    return theCurrentValueLambda(theDDMParameter, theDDMParameter2);
}


double  RParameter::getCurrentValue(QString name) const
{
    return 0.0;
}


void RParameter::setCurrentValue(double value)
{
    theCurrentValue = value;

    // Reset the current value lambda incase it was set with a lambda at some point
    theCurrentValueLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
    {
        if(theDDMParameter != nullptr || theDDMParameter2 != nullptr)
        {
            if(theDDMParameter == this)
                return 1.0;
            else
                return 0.0;
        }

        return theCurrentValue;
    };
}


void RParameter::setCurrentValue(std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> LambdaValue)
{
    theCurrentValueLambda = std::move(LambdaValue);  //Transfer ownership to the RParameter of lambda
}


double * RParameter::getCurrentValuePointer()
{
    return &theCurrentValue;
}


void RParameter::setNodalParameterFlag(bool flag)
{
    NodalParameterFlag = flag;
}


bool RParameter::isNodalParameter()
{
    return NodalParameterFlag;
}


bool RParameter::getDiscount() const
{
    return discount;
}


void RParameter::setDiscount(bool value)
{
    discount = value;
}
