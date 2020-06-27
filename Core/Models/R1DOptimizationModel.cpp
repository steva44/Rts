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

#include "R1DOptimizationModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RDecisionVariable.h"
#include "RSearchDirection.h"
#include "RMatrixOperations.h"
#include "RModelManager.h"
#include <QDateTime>

R1DOptimizationModel::R1DOptimizationModel(QObject *parent, QString name)
    : RModel(parent, name), RLineSearchCallBackAssistant()
{
    isOrchestratingModel = true;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    theFunction = nullptr;
    theModel = nullptr;
    theParameter = nullptr;
    thePerturbation = 0.01;
    theLineSearchAlgorithm = nullptr;
    customPlot = nullptr;
}


R1DOptimizationModel::~R1DOptimizationModel()
{
    delete customPlot;
}


QObject * R1DOptimizationModel::getFunction() const
{
    return theFunction;
}


void R1DOptimizationModel::setFunction(QObject *value)
{
    theFunction = qobject_cast<RParameter *>(value);
    if (!theFunction) {
        qCritical() << "Invalid function given to" << this->objectName();
        return;
    }

    // Get pointer to the model
    RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theFunction);
    if (!theInputCastAsResponse) {
        qCritical() << this->objectName() << "needs the input function to be a generic response";
        return;
    }
    theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    if (!theModel) {
        qCritical() << this->objectName() << "could not find a model when asking the input";
    }

}


QObject * R1DOptimizationModel::getParameter() const
{
    return theParameter;
}


void R1DOptimizationModel::setParameter(QObject *value)
{
    theParameter = qobject_cast<RParameter *>(value);
    if (!theParameter) {
        qCritical() << "Invalid parameter given to" << this->objectName();
    }
}


QObject * R1DOptimizationModel::getLineSearchAlgorithm() const
{
    return theLineSearchAlgorithm;
}


void R1DOptimizationModel::setLineSearchAlgorithm(QObject *value)
{
    theLineSearchAlgorithm = qobject_cast<RLineSearchAlgorithm *>(value);
    if (!theLineSearchAlgorithm) {
        qCritical() << "Invalid search algorithm given to" << this->objectName();
    }
}


double R1DOptimizationModel::getPerturbation() const
{
    return thePerturbation;
}


void R1DOptimizationModel::setPerturbation(double value)
{
    thePerturbation = value;
}


double R1DOptimizationModel::evaluate_F(double parameterValue)
{
    // Set x-value, i.e., parameter value
    theParameter->setCurrentValue(parameterValue);


    // Evaluate the function
    theModel->runAnalysis(RGradientType::NONE);
    theDomain->clearAnalyzedFlags();


    // Return the value of the function
    return theFunction->getCurrentValue();
}


double R1DOptimizationModel::evaluate_f(double parameterValue)
{
    // Set x-value, i.e., parameter value
    theParameter->setCurrentValue(parameterValue);


    // Compute gradient by finite difference
    return firstOrderFiniteDifference();
}


double R1DOptimizationModel::evaluate_h(double parameterValue)
{
    // Set x-value, i.e., parameter value
    theParameter->setCurrentValue(parameterValue);


    // Compute Hessian by finite difference
    return secondOrderFiniteDifference();
}


int R1DOptimizationModel::evaluateModel(RGradientType theGradientType)
{
    // Call the algorithm
    double optimalParameterValue = theLineSearchAlgorithm->findPoint(this);

    // Output
    if (theOutputLevel == RObject::Maximum) {

        qDebug() << "The optimal parameter value in" << this->objectName() << "is" << optimalParameterValue;
    }

    // Set the response
    theResponse->setCurrentValue(optimalParameterValue);

    return 0;
}


double R1DOptimizationModel::firstOrderFiniteDifference()
{
    // Get pointer to the model
    RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theFunction);
    if (!theInputCastAsResponse) {
        qCritical() << this->objectName() << "needs the input function to be a generic response";
    }
    RModel *theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());


    // Run the analysis at the current value of the parameter
    theModel->runAnalysis(RGradientType::NONE);
    theDomain->clearAnalyzedFlags();
    double functionValue = theFunction->getCurrentValue();


    // Perturb the parameter and run the analysis again
    double parameterValueBackup = theParameter->getCurrentValue();
    double perturbation;
    if (parameterValueBackup == 0.0) {
        perturbation = thePerturbation;
    }
    else {
        perturbation = thePerturbation * parameterValueBackup;
    }
    theParameter->setCurrentValue(parameterValueBackup+perturbation);
    theModel->runAnalysis(RGradientType::NONE);
    theDomain->clearAnalyzedFlags();
    double perburbedFunctionValue = theFunction->getCurrentValue();


    // Reset parameter value
    theParameter->setCurrentValue(parameterValueBackup);


    // Calucate the gradient
    return (perburbedFunctionValue - functionValue) / perturbation;
}


double R1DOptimizationModel::secondOrderFiniteDifference()
{
    // Run the analysis at the current value of the parameter
    theModel->runAnalysis(RGradientType::NONE);
    theDomain->clearAnalyzedFlags();
    double functionValue = theFunction->getCurrentValue();


    // Set perturbation value
    double parameterValueBackup = theParameter->getCurrentValue();
    double perturbation;
    if (parameterValueBackup == 0.0) {
        perturbation = thePerturbation;
    }
    else {
        perturbation = thePerturbation * parameterValueBackup;
    }


    // Perturb the parameter FORWARD and run the analysis again
    theParameter->setCurrentValue(parameterValueBackup+perturbation);
    theModel->runAnalysis(RGradientType::NONE);
    theDomain->clearAnalyzedFlags();
    double perburbedForwardFunctionValue = theFunction->getCurrentValue();


    // Perturb the parameter BACKWARD and run the analysis again
    theParameter->setCurrentValue(parameterValueBackup-perturbation);
    theModel->runAnalysis(RGradientType::NONE);
    theDomain->clearAnalyzedFlags();
    double perburbedBackwardFunctionValue = theFunction->getCurrentValue();


    // Reset parameter value
    theParameter->setCurrentValue(parameterValueBackup);


    // Calucate the second-order derivative
    return (perburbedForwardFunctionValue - 2.0 * functionValue + perburbedBackwardFunctionValue) / (perturbation * perturbation);
}


