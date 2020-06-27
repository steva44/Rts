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

#include "RScriptModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RDomain.h"

#include <QScriptEngineDebugger>
#include <QAction>

QScriptValue print(QScriptContext *context, QScriptEngine *engine);
RScriptModel *thisScriptModel;

RScriptModel::RScriptModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theDDM = false;

    theEngine.globalObject().setProperty("print", theEngine.newFunction(print, 1));
    thisScriptModel = this;
}


RScriptModel::~RScriptModel()
{

}


QString RScriptModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}


void RScriptModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


QString RScriptModel::getScriptFile()
{
    return theScriptFile;
}


void RScriptModel::setScriptFile(QString value)
{
    theScriptFile = RDomain::stringToFilePath(value);
}


bool RScriptModel::getDDM()
{
    return theDDM;
}


void RScriptModel::setDDM(bool value)
{
    theDDM = value;
}


int RScriptModel::evaluateModel(RGradientType theGradientType)
{
    // Clearing null pointers
    clearNullPointers(&theInputParameterList);

    if (theOutputLevel >= RObject::Medium) {
        qDebug() << "Starting to run script in model" << objectName() << "...";
    }

    for (int i = 0; i < theInputParameterList.count(); i++) {
        theEngine.globalObject().setProperty(theInputParameterList[i]->objectName(), theInputParameterList[i]->getCurrentValue());
    }

    // For DDM
    theEngine.globalObject().setProperty("evaluateDDM", theDDM);


    if (theScriptText.isEmpty()) {
        QFile scriptFile(theScriptFile);
        if (!scriptFile.open(QIODevice::ReadOnly)) {
            // Error
            qCritical() << "Error 1: Could not open the script file" << theScriptFile << "given to model" << objectName() << ".";
            return -1;
        }

        QTextStream stream(&scriptFile);
        theScriptText = stream.readAll();
        if (theScriptText.isEmpty()) {
            // Error
            qCritical() << "Error 2: The script file" << theScriptFile << "given to model" << objectName() << " is empty.";
            return -1;
        }
        scriptFile.close();
    }

    QScriptValue scriptResult = theEngine.evaluate(theScriptText, theScriptFile);
    if (theEngine.hasUncaughtException()) {
        int line = theEngine.uncaughtExceptionLineNumber();
        qCritical() << "Error 3: Uncaught exception at line" << line << "in the script file given to model" << objectName() << ":" << scriptResult.toString() << ".";
        return -1;
    }

    QList<RResponse *> outputResponseList = this->findChildren<RResponse *>();
    for (int i = 0; i < outputResponseList.count(); i++) {
        QScriptValue scriptValue = theEngine.evaluate(outputResponseList[i]->objectName());
        if (theEngine.hasUncaughtException()) {
            int line = theEngine.uncaughtExceptionLineNumber();
            qCritical() << "Error 4: Uncaught exception at line" << line << "in the script file given to model" << objectName() << ":" << scriptValue.toString() << ".";
            return -1;
        }
        double response = scriptValue.toNumber();
        outputResponseList[i]->setCurrentValue(response);
    }

    // Reading the DDM results from the script
    // Notation: The variable value that is demanded form the script has the following name: d_Response_d_Parameter
    int result = 0;
    if (theGradientType == RGradientType::DDM) {
        if (theDDM) {
            for (int i = 0; i < outputResponseList.count(); i++) {
                QMap<RParameter *, double> theGradientMap;
                for (int j = 0; j < theInputParameterList.count(); j++) {
                    QString DDMVariableName = QString("d_%1_d_%2").arg(outputResponseList[i]->objectName()).arg(theInputParameterList[j]->objectName());
                    QScriptValue scriptValue = theEngine.evaluate(DDMVariableName);
                    if (theEngine.hasUncaughtException()) {
                        int line = theEngine.uncaughtExceptionLineNumber();
                        qCritical() << "Error 5: Uncaught exception at line" << line << "in the script file given to model" << objectName() << ":" << scriptValue.toString() << ".";
                        qCritical() << "Error 6: Could not read the DDM value" << DDMVariableName << "in the model" << objectName() << ".";

                        // return -10 means that the DDM evluation has failed
                        return -10;
                    }
                    double responseDDM = scriptValue.toNumber();
                    theGradientMap.insertMulti(theInputParameterList[j], responseDDM);
                }
                theDDMMap.insertMulti(outputResponseList[i], theGradientMap);
            }
            result = 1;
        } else {
            result = 0;
        }
    }

    if (theOutputLevel >= RObject::Medium) {
        qDebug() << "Running script in model" << objectName() << "done.";
        qDebug() << " ";
    }

    return result;
}


QStringList RScriptModel::getActionList()
{
    // Remember that the "run" and "help" methods are implemented in base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Debug" << "Separator" << "&Help";
    return actionList;
}


void RScriptModel::reset()
{
    theScriptText.clear();
}


void RScriptModel::debug()
{
    QScriptEngineDebugger *debugger = 0;
    debugger = this->findChild<QScriptEngineDebugger *>();
    if (debugger) {
        debugger->detach();
        //debugger->standardWindow()->close();
        delete debugger;
        debugger = 0;
    }

    debugger = new QScriptEngineDebugger(this);

    QWidget *debuggerWidget = debugger->standardWindow();
    //debuggerWidget->setObjectName("QScriptEngineDebugger");
    //RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    //mainWindow->addSubWindow(debuggerWidget, objectName() + " Debugger", false);
    debuggerWidget->setWindowModality(Qt::ApplicationModal);

    debugger->attachTo(&theEngine);
    debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();

    if(theDDM)
    {
    evaluateModel(RGradientType::DDM);
    }
    else{
        evaluateModel(RGradientType::NONE);
    }
}


QScriptValue print(QScriptContext *context, QScriptEngine *engine)
{
    if (thisScriptModel->getOutputLevel() >= RObject::Medium) {
        QString result;
        for (int i = 0; i < context->argumentCount(); i++) {
            //if (i > 0) {
            //	result.append(" ");
            //}
            result.append(context->argument(i).toString());
        }
        qDebug() << qPrintable(result);
    }
    return engine->undefinedValue();
}
