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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RAlgebraicExpressionModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RDomain.h"
#include "RMainWindow.h"
#include "RModelInferenceDialog.h"

RAlgebraicExpressionModel::RAlgebraicExpressionModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theResponse = nullptr;
    theParser = nullptr;

    theEvaluationCount = 0;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RAlgebraicExpressionModel::~RAlgebraicExpressionModel()
{

}


QString RAlgebraicExpressionModel::getExpression() const	
{	
    return theExpression;
}	


void RAlgebraicExpressionModel::setExpression(QString value)	
{	
    theExpression = value;
    reset();
}	


int RAlgebraicExpressionModel::getEvaluationCount() const
{
    return theEvaluationCount;
}


void RAlgebraicExpressionModel::setEvaluationCount(int value)
{
    theEvaluationCount = value;
}


QList<RParameter *> RAlgebraicExpressionModel::getPhysicalParameterList()
{
    return theAllParametersList;
}


QStringList RAlgebraicExpressionModel::getActionList()
{
    // Remember that the "run" and "help" methods are implemented in base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Model Inference" <<  "Separator" << "&Help";
    return actionList;
}


int RAlgebraicExpressionModel::evaluateModel(RGradientType theGradientType)
{	
    theEvaluationCount++;

    try {
        // if "theParser" is NULL, it mean that the function has not been parsed. If not, no action is needed.
        if (theParser == nullptr) {
            // Parsing the function
            int result = parseExpression();
            if (result < 0) {
                qCritical() << "Error: Could not parse the function" << objectName() << " in evaluateModel().";
                return -1;
            }
        }

        double response = theParser->Eval();

        theResponse->setCurrentValue(response);


        //First-order using FDM
        if(theGradientType  == RGradientType::FDM)
        {
            double originalVariableValue, perturbedVariableValue, perturbedResponseValue, perturbation;

            auto perturbationFactor = 0.00000001;

            //qDebug()<<"In FDM of RAlgebraicExp model";

            theModelFDMMap.clear();

            for (auto& theParameter : theExplicitParameterList)
            {
                originalVariableValue = theParameter->getCurrentValue();

                if (originalVariableValue == 0.0) {
                    perturbation = perturbationFactor;
                }
                else {
                    perturbation = perturbationFactor * originalVariableValue;
                }

                perturbedVariableValue = originalVariableValue + perturbation;

                theParameter->setCurrentValue(perturbedVariableValue);

                perturbedResponseValue = theParser->Eval();

                auto theGradient = (perturbedResponseValue-response) / perturbation;

                theParameter->setCurrentValue(originalVariableValue);

                theModelFDMMap.insert(theParameter,theGradient);
            }

        }


        return 0;

        // Catching the exception thrown by "theParser", if there has been any (MuParser throws an exception for error handling). also, the error message is taken from "theParser" and printed in the output pane.
    } catch (mu::Parser::exception_type &error) {

#ifdef Q_OS_UNIX
        std::string erroStdStr = error.GetMsg();
        qCritical() << "Error in model" << objectName() << " in evaluateModel() :" << qPrintable(QString().fromStdString(erroStdStr));
#else
        std::wstring erroStdStr = error.GetMsg();
        qCritical() << "Error in model" << objectName() << " in evaluateModel() :" << qPrintable(QString().fromStdWString(erroStdStr));
#endif
        return -1;
    }
}


int RAlgebraicExpressionModel::parseExpression()
{
    try {

        if (theParser != nullptr) {
            reset();
        }
        else{
        // Instatiating "theParser" object. We use MuParser package in order to parse and evaluate an expression.
        theParser = std::make_unique<mu::Parser>();
        }


#ifdef Q_OS_UNIX
        std::string stdStrFunction = theExpression.toStdString();
#else
        std::wstring stdStrFunction = theExpression.toStdWString();
#endif

        // Submitting the function expression to "theParser"
        theParser->SetExpr(stdStrFunction);

        QStringList listPos;

#ifdef Q_OS_UNIX
        QString str = QString::fromStdString(stdStrFunction);
#else
        QString str = QString::fromStdWString(stdStrFunction);
#endif

        listPos = str.split("+");

        // Getting a list of the name of parameters that are explicitly used in the function from "theParser". The list is in an array format, which is specific to MuParser package. The list is called "variables".
        mu::varmap_type variables = theParser->GetUsedVar();

        // In the following lines, the names of parameters in the list "variables" is iterated over.
        // These parameters are the parameters (rvs, dvs, constants, and responses) that are explicitly used in this function.
        // The name of each parameter is passed to the domain in order to receive a pointer to the parameter (using findChild() method of the domain).
        // By the end of the loop, theExplicitParameterList, which is the list of explicitly used parameters in the function, is populated.

        mu::varmap_type::const_iterator item = variables.begin();
        QString paramterStr;
        int tempIndex = 0;

        QList<QString> variablesList;

        for (; item != variables.end(); ++item) {


#ifdef Q_OS_UNIX
            // Reading the name of the parameter from "variables" list
            std::string paramterStdStr = item->first;
            // Converting the name from std::string to QString
            paramterStr = QString().fromStdString(paramterStdStr);
#else
            // Reading the name of the parameter from "variables" list
            std::wstring paramterStdStr = item->first;
            // Converting the name from std::string to QString
            paramterStr = QString().fromStdWString(paramterStdStr);
#endif

            // Finding the pointer to the parameter with this name
            RParameter *parameterObject = theDomain->findChild<RParameter *>(paramterStr);

            variablesList<<paramterStr;

            // If the parameter with this name does not exist in the domain, issue an error.
            if (!parameterObject) {
                qCritical() << "Error in model" << objectName() << ": Parameter" << paramterStr << "was not found in the domain.";
                // Delete "theParser" and clear the lists using "reset()" mtehod.
                reset();
                return -1;
            }

            // Submitting the name and a pointer to current value of the parameterObject
            theParser->DefineVar(paramterStdStr, parameterObject->getCurrentValuePointer());

            if (!theExplicitParameterList.contains(parameterObject)) {
                theExplicitParameterList << parameterObject;
            }


            tempIndex++;
        }

        theAllParametersList << theExplicitParameterList;

        return 0;

        // Cathcing the exception thrown by "theParser", if there has been any (MuParser throws an exception for error handling). also, the error message is gotten from "theParser" and printed in the output pane.
    } catch (mu::Parser::exception_type &error) {

#ifdef Q_OS_UNIX
        std::string erroStdStr = error.GetMsg();
        qCritical() << "Error in model " << objectName() << ":" << qPrintable(QString().fromStdString(erroStdStr));
#else
        std::wstring erroStdStr = error.GetMsg();
        qCritical() << "Error in model " << objectName() << ":" << qPrintable(QString().fromStdWString(erroStdStr));
#endif

        return -1;
    }
}


void RAlgebraicExpressionModel::reset()
{
    if (theParser) {
        theParser.reset(new mu::Parser());
    }
    theAllParametersList.clear();
    theExplicitParameterList.clear();
    theModelFDMMap.clear();
}


void RAlgebraicExpressionModel::modelInference()
{
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(this->parent());

    RModelInferenceDialog *dialog = new RModelInferenceDialog(mainWindow, theDomain);
    if (dialog->exec()) {

    }
    delete dialog;

}

