#ifndef RLineSearchCallBackAssistant_H
#define RLineSearchCallBackAssistant_H
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

class RLineSearchCallBackAssistant
{

public:
    RLineSearchCallBackAssistant();
    virtual ~RLineSearchCallBackAssistant();

    // F(x) = objective function, or merit function, relevant in optimization, not in root-finding
    virtual double evaluate_F(double parameterValue) = 0;

    // f(x) = dF/dx = function whose root is sought
    virtual double evaluate_f(double parameterValue);

    // h(x) = df/dx = d^2F/dx^2 = Hessian of objective function
    virtual double evaluate_h(double parameterValue);

protected:

};

#endif
