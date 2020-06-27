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

#include "RArmijoLineSearchAlgorithm.h"
#include "RLineSearchCallBackAssistant.h"
#include "RDomain.h"
#include <math.h>

RArmijoLineSearchAlgorithm::RArmijoLineSearchAlgorithm(QObject *parent, QString name)
    : RLineSearchAlgorithm(parent, name)
{
    theBase = 0.5;
}


RArmijoLineSearchAlgorithm::~RArmijoLineSearchAlgorithm()
{

}


double RArmijoLineSearchAlgorithm::getBase() const
{
    return theBase;
}


void RArmijoLineSearchAlgorithm::setBase(double value)
{
    theBase = value;
}


double RArmijoLineSearchAlgorithm::findPoint(RLineSearchCallBackAssistant *theCallBackAssistant)
{
    // Initial declarations
    double stepSize = 1.0;
    double stepSizeOneStepBack;
    double meritFunction;
    double meritFunctionOneStepBack;


    // Evaluate the merit function
    meritFunction = theCallBackAssistant->evaluate_F(stepSize);


    // Cut trial solution until merit function start increasing
    for (int i=1; i<=10; i++) {

        stepSizeOneStepBack = pow(theBase, i);

        meritFunctionOneStepBack = theCallBackAssistant->evaluate_F(stepSizeOneStepBack);

        if (meritFunctionOneStepBack < meritFunction) {

            // The merit function went down when we stepped back, so keep going
            stepSize = stepSizeOneStepBack;
            meritFunction = meritFunctionOneStepBack;
        }
        else {

            // The merit function did NOT go down when we stepped back, so keep the original step size
            break;
        }
    }


    if (theOutputLevel >= RObject::Medium) {

        qDebug() << this->objectName() << "is returning value" << stepSize;
    }

    return stepSize;
}

