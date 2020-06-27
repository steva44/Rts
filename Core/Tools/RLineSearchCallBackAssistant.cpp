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

#include "RLineSearchCallBackAssistant.h"
#include <QDateTime>

RLineSearchCallBackAssistant::RLineSearchCallBackAssistant()
{

}


RLineSearchCallBackAssistant::~RLineSearchCallBackAssistant()
{

}


double RLineSearchCallBackAssistant::evaluate_f(double parameterValue)
{
    // Original function value
    double functionValue = evaluate_F(parameterValue);

    // Perturbation
    double perturbation = 0.001;
    if (parameterValue != 0.0) {
        perturbation = perturbation * parameterValue;
    }

    // Perturbed function value
    double perburbedForwardFunctionValue = evaluate_F(parameterValue + perturbation);

    // Sought derivative
    return (perburbedForwardFunctionValue - functionValue) / perturbation;
}


double RLineSearchCallBackAssistant::evaluate_h(double parameterValue)
{
    // Original function value
    double functionValue = evaluate_F(parameterValue);

    // Perturbation
    double perturbation = 0.001;
    if (parameterValue != 0.0) {
        perturbation = perturbation * parameterValue;
    }

    // Function value, perturbed forward
    double perburbedForwardFunctionValue = evaluate_F(parameterValue + perturbation);

    // Function value, perturbed backward
    double perburbedBackwardFunctionValue = evaluate_F(parameterValue - perturbation);

    // Sought derivative
    return (perburbedForwardFunctionValue - 2.0 * functionValue + perburbedBackwardFunctionValue) / (perturbation * perturbation);
}

