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

#include "RRootFindingModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RSearchDirection.h"
#include "RLineSearchAlgorithm.h"

#include <QDateTime>
#include "qcustomplot.h"

RRootFindingModel::RRootFindingModel(QObject *parent, QString name)
    : RModel(parent, name), RLineSearchCallBackAssistant()
{
    isOrchestratingModel = true;

    theFunction = nullptr;
    theModel = nullptr;
    theParameter = nullptr;
    customPlot = nullptr;
    theLineSearchAlgorithm = nullptr;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    thePerturbation = 0.01;
}


RRootFindingModel::~RRootFindingModel()
{
    delete customPlot;
}


QObject * RRootFindingModel::getFunction() const
{
    return theFunction;
}


void RRootFindingModel::setFunction(QObject *value)
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


QObject * RRootFindingModel::getParameter() const
{
    return theParameter;
}


void RRootFindingModel::setParameter(QObject *value)
{
    theParameter = qobject_cast<RParameter *>(value);
    if (!theParameter) {
        qCritical() << "Invalid parameter given to" << this->objectName();
    }
}


QObject * RRootFindingModel::getLineSearchAlgorithm() const
{
    return theLineSearchAlgorithm;
}


void RRootFindingModel::setLineSearchAlgorithm(QObject *value)
{
    theLineSearchAlgorithm = qobject_cast<RLineSearchAlgorithm *>(value);
    if (!theLineSearchAlgorithm) {
        qCritical() << "Invalid search algorithm given to" << this->objectName();
    }
}


double RRootFindingModel::getPerturbation() const
{
    return thePerturbation;
}


void RRootFindingModel::setPerturbation(double value)
{
    thePerturbation = value;
}


double RRootFindingModel::evaluate_F(double parameterValue)
{
    // F(x) = relevant in optimization but NOT in root-finding
    // f(x) = function whose root is sought
    // h(x) = df/dx

    // This method should NOT be called in root-finding
    qCritical() << "The function evaluation method in" << this->objectName() << "should NOT be called in a Root Finding search";
    return -1;
}


double RRootFindingModel::evaluate_f(double parameterValue)
{
    // F(x) = relevant in optimization but NOT in root-finding
    // f(x) = function whose root is sought
    // h(x) = df/dx


    // Set x-value, i.e., parameter value
    theParameter->setCurrentValue(parameterValue);


    // Run the analysis
    theModel->runAnalysis(RGradientType::NONE);
    theDomain->clearAnalyzedFlags();


    // Return function value
    return theFunction->getCurrentValue();
}


double RRootFindingModel::evaluate_h(double parameterValue)
{


    // Set the parameter value
    theParameter->setCurrentValue(parameterValue);


    // Compute the gradient of f
    return firstOrderFiniteDifference();
}


int RRootFindingModel::evaluateModel(RGradientType theGradientType)
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


double RRootFindingModel::firstOrderFiniteDifference()
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

