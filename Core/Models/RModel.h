#ifndef RModel_H
#define RModel_H
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
#include <QProcess>

class RParameter;
class RRandomVariable;
class RDecisionVariable;
class RConstant;
class RResponse;

class RModel : public RObject
{
    Q_OBJECT
    Q_PROPERTY(ROutputLevelType OutputLevel READ getOutputLevel WRITE setOutputLevel)

public:
    RModel(QObject *parent, QString name);
    virtual ~RModel();

    // Runs the model analysis by first calling the runAnalysis for upstream responses
    int runAnalysis(RGradientType theGradientType);

    // Terje moved the evaluateModel() method from being protected to being public
    // Evaluates the model; Return values:
    // -1~-9: The model evaluation has failed
    // -10: The DDM evaluation has failed
    // 1: The model HAS the implementation of DDM and both DDM and model have been evaluated successfully
    // 0: The model does NOT have the implementation of DDM and the model has been evaluated successfully
    virtual int evaluateModel(RGradientType theGradientType) = 0;

    // Evaluates the responses that flow upstream, i.e., enter the upstream model
    virtual int setResponsesThatFlowUpstream();

    // Specific implementation of the setObjectName, which sets the object name for the model and its auto-generated responses
    void setObjectName(const QString &name);

    // Indicates if the passed parameter is an input to the model
    bool contains(RParameter *value);

    // Compiles and returns a list of parameters that are input to the model
    virtual QList<RParameter *> getAllParametersList(bool *ok);

    // Returns a list of random variables that are input to the model
    QList<RRandomVariable *> getRandomVariableList();

    // Returns a list of decision variables that are input to the model
    QList<RDecisionVariable *> getDecisionVariableList();

    // Returns a list of constants that are input to the model
    QList<RConstant *> getConstantList();

    // Returns a list of responses from upstream models that are input to the model
    QList<RResponse *> getResponseList();

    // Sets the isAnalyzed flag of all output responses of the model to false
    void setResponseAnalyzed(bool analyzed);

    // Clears the NULL parameters from the passed parameter list
    void clearNullPointers(QList<QPointer<RParameter> > *passdedParameterList);

    // Invokes the operations that must be run before a new analysis, i.e., a new FORM analysis, starts
    virtual void reset();

    // Resets the history variables at the start of a new sample (scenario) in a scenario sampling analysis
    virtual void resetHistoryVariables();

    // Updates the history variables at the end of each function evaluation within a scenario
    virtual void updateHistoryVariables();

    virtual QList<QPair<double, QString> > getTriggerTimes(double startTime, double endTime);

    // This method is only implemented in some hazard models, such as magnitude and Location models; It returns the list of parameters (essentially random variables) that should be sampled every time the hazard model becomes on (in a scenario sampling analysis)
    virtual QList<RParameter *> getPhysicalParameterList();

    // Clears the DDM QMap objects
    void clearDDMMaps();

    // Returns a pointer to the QProcess object that is used to run an external software application
    QProcess *getProcess();

    // Terje is adding:
    virtual QStringList getActionList();

    // Resets time, among other things, and is run from the RScenarioModel
    virtual int resetTime();

    // Have the "is-analyzed" flag here in the model instead of in the responses
    bool isAnalyzed;

public slots:

    virtual void run();


signals:
    // Signal that is emitted when the external software application is finished running
    void processFinished();

protected:

    // Adding a flag to ask if runAnalysis should be automatically called or orchestrated in evaluateModel
    bool isOrchestratingModel;

    // Evaluates gradient of the model response wrt. input parameters by finite difference; used when the function is trying to compute the gradeint by DDMs but the model does not have DDM implementations
    int evaluateModelGradient(RObject::RParameterType parameterType);

    // List of all input parameters
    QList<RParameter *> theAllParametersList;

    // List of all parameters that were created within this model
    QList<RParameter *> theAutoParametersList;

    // Methods and data members that are used to communicate with an external software application
    // Only for file communication with the external software application: deletes the files that are generated by the external software application
    void deleteOutputFiles();

    // Only for file communication with the external software application: Reads and assigns the response values from files after the external application running is complete
    int updateFileResponses();

    // Starts the QProcess that runs the external software application
    void initiatePtocess(QString workingFilePath);

    // Prevents Rt from freezing when the external software application is being run
    void eventLoop(bool continuousProcess);

    // Pointer to the QProcess that runs the external software application
    QPointer<QProcess> theProcess;

    // String that is written to output when the external software application is finished running
    const char *finishString;

    // Indicates whether the external software application has finihsed running
    bool processFinishedAnalyzing;

    // QMap object that relates the computed DDM sensitivities to input parameters for each output response
    QMap<RResponse *, QMap<RParameter *, double> > theDDMMap;

    // QMap object that relates the computed second order DDM sensitivities to the input parameters for each output response
    // For hessian type arrangements will have a matrix in the value part of the map
    QMap<RResponse *, QMap<RParameter *, double> > theD2DMMap;

    // For hessian type arrangements will have a matrix in the value part of the map
    QMap<RResponse *, QMap<RParameter *, double> > theD3DMMap;

    // QMap object that stores the explicit and implicit dependencies of this model to parameters
    QMap<RParameter *, int> theDependenceMap;

protected slots:

    // Shows the output of the external application in the output pane
    void showOutput();

    // Slot that issues a warning if the external application crashes
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

};

#endif // RModel_H
