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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RParametricPoint.h"
#include "RDomain.h"

#ifndef Q_OS_UNIX
#include "MuParser\muParserDef.h"
#endif

RParametricPoint::RParametricPoint(QObject *parent, QString name)
    : RParameter(parent, name)
{

    auto evalThis = [this](RParameter* theDDMParameter, RParameter* theDDMParameter2){return evaluateExpression(RGradientType::NONE);};

    this->setCurrentValue(evalThis);
}


RParametricPoint::~RParametricPoint()
{
}


QString RParametricPoint::getExpression() const	
{	
    return theExpression;
}	


void RParametricPoint::setExpression(QString value)	
{	
    theExpression = value;
    reset();
}	


double RParametricPoint::evaluateExpression(RGradientType theGradientType)
{	
    double response = 0.0;

    try {
        // if "theParser" is NULL, it mean that the function has not been parsed. If not, no action is needed.
        if (theParser == nullptr) {
            // Parsing the function
            int result = parseExpression();
            if (result < 0) {
                qCritical() << "Error: Could not parse the function" << objectName() << " in evaluateModel().";
                return response;
            }
        }

        response = theParser->Eval();

        //First-order using FDM forward perturbation
        if(theGradientType  == RGradientType::DDM)
        {
            double originalVariableValue, perturbedVariableValue, perturbedResponseValue, perturbation;

            auto perturbationFactor = 0.00000001;

            theFOFDMMap.clear();

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

                theFOFDMMap.insert(theParameter,theGradient);
            }
        }

        //Second-order using FDM forward perturbation
        if(theGradientType  == RGradientType::D2DM)
        {
            auto perturbationFactor = 0.0001;

            double originalVariableValue, perturbedVariableValuefwd, perturbedVariableValueback, perturbedResponseValueFwd,perturbedResponseValueBack, perturbation;

            theSOFDMMap.clear();

            //Get F2DM using the central difference scheme

            for (auto& theParameter : theExplicitParameterList)
            {
                originalVariableValue = theParameter->getCurrentValue();

                if (originalVariableValue == 0.0) {
                    perturbation = perturbationFactor;
                }
                else {
                    perturbation = perturbationFactor * originalVariableValue;
                }

                perturbedVariableValuefwd = originalVariableValue + perturbation;
                perturbedVariableValueback = originalVariableValue - perturbation;

                //First do the forward value
                theParameter->setCurrentValue(perturbedVariableValuefwd);
                perturbedResponseValueFwd = theParser->Eval();

                //now rerun with the back value
                theParameter->setCurrentValue(perturbedVariableValueback);
                perturbedResponseValueBack = theParser->Eval();

                auto theGradient = (perturbedResponseValueFwd - 2*response + perturbedResponseValueBack) /(perturbation*perturbation);

                theParameter->setCurrentValue(originalVariableValue);

                theSOFDMMap.insert(theParameter,theGradient);
            }
        }

        return response;

        // Catching the exception thrown by "theParser", if there has been any (MuParser throws an exception for error handling). also, the error message is taken from "theParser" and printed in the output pane.
    } catch (mu::Parser::exception_type &error) {

        return response;

#ifdef Q_OS_UNIX
        std::string erroStdStr = error.GetMsg();
        qCritical() << "Error in model" << objectName() << " in evaluateModel() :" << qPrintable(QString().fromStdString(erroStdStr));
#else
        std::wstring erroStdStr = static_cast<std::wstring>(error.GetMsg());
        qCritical() << "Error in model" << objectName() << " in evaluateModel() :" << qPrintable(QString().fromStdWString(erroStdStr));
#endif
    }
}


int RParametricPoint::parseExpression()
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


void RParametricPoint::reset()
{
    if (theParser) {
        theParser.reset(new mu::Parser());
    }
    theExplicitParameterList.clear();
    theFOFDMMap.clear();
}



