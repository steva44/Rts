#include "RLinearDynamicStructuralAnalysis.h"
#include "RDomain.h"
#include "RMatrixOperations.h"
#include "RMainWindow.h"

#include <QCoreApplication>

#ifdef OSG
#include "ROSGPlotter.h"
#endif


RLinearDynamicStructuralAnalysis::RLinearDynamicStructuralAnalysis(QObject *parent, QString name)
    : RStructuralAnalysis(parent, name)
{
    theTimeStepper = nullptr;
    theTimeStep = 0.01;
    theEndTime = 10.0;
    thePlotInterval = 3;
    recordersHaveBeenCreated = false;
}

RLinearDynamicStructuralAnalysis::~RLinearDynamicStructuralAnalysis()
{

}


QObject * RLinearDynamicStructuralAnalysis::getTimeStepper() const
{
    return theTimeStepper;
}

void RLinearDynamicStructuralAnalysis::setTimeStepper(QObject *value)
{
    theTimeStepper = qobject_cast<RTimeStepper *>(value);
}



double RLinearDynamicStructuralAnalysis::getTimeStep() const
{
    return theTimeStep;
}

void RLinearDynamicStructuralAnalysis::setTimeStep(double value)
{
    if (value <= 0.0) {
        qCritical() << "The time step must be greater than zero.";
        value = 0.01;
    }
    theTimeStep = value;
}



double RLinearDynamicStructuralAnalysis::getEndTime() const
{
    return theEndTime;
}

void RLinearDynamicStructuralAnalysis::setEndTime(double value)
{
    if (value <= 0.0) {
        qCritical() << "The end time must be greater than zero.";
        value = 10.0;
    }
    theEndTime = value;
}



int RLinearDynamicStructuralAnalysis::getPlotInterval() const
{
    return thePlotInterval;
}

void RLinearDynamicStructuralAnalysis::setPlotInterval(int value)
{
    if (value <= 0.0) {
        qCritical() << "The plot interval must be greater than zero.";
        value = 10;
    }
    thePlotInterval = value;
}



int RLinearDynamicStructuralAnalysis::setRecorders(RStructuralAnalysisModel *theStructuralModel, QList<QPointer<RObject> > passedStructuralObjects, QStringList passedResponses)
{
    // Set data members
    theStructuralObjects = passedStructuralObjects;
    theResponses = passedResponses;


    // Create response objects and store them in a list, which is another data member of this class
    if (!recordersHaveBeenCreated) {

        while (theResponseList.count() > 0) {
            delete theResponseList.takeAt(0);
        }

        // Instantiating the response objects
        for (int i = 0; i < theStructuralObjects.count(); i++) {
            theDomain->createObject(theStructuralModel->objectName() + theStructuralObjects[i]->objectName() + theResponses[i] + "TimeSeries", "RTimeSeriesResponse");
            RResponse *response = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
            response->setModel(theStructuralModel);
            theResponseList << response;
        }

        recordersHaveBeenCreated = true;

        // Update the domain tree
        RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
        mainWindow->updateTree("Parameters");

    }

    return 0;
}



int RLinearDynamicStructuralAnalysis::recordResponses(double time)
{
    for (int i=0; i<theResponseList.count(); i++) {

        QByteArray byteArrayName = theResponses[i].toLatin1();
        const char * propertyName = byteArrayName.data();

        bool ok;
        double response = theStructuralObjects[i]->property(propertyName).toDouble(&ok);
        if (!ok) {
            qCritical() << "Error: In the model" << objectName() << ": Could not read the property" << theResponses[i] << "in the structural object" << theStructuralObjects[i]->objectName() << ".";
            return -1;
        }



        RTimeSeries *theOutputTimeSeries = parameterToTimeSeries(theResponseList.at(i));
        if (!theOutputTimeSeries) {
            qCritical() << "Error: The response object" << theResponseList.at(i)->objectName() << "does not contain a times series object.";
            return -1;
        }

        theOutputTimeSeries->setXYZvalues(time, response, 0.0, 0.0);

    }

    return 0;
}



int RLinearDynamicStructuralAnalysis::conductStructuralAnalysis(RGradientType theGradientType, QList<RParameter *> theAllParametersList)
{

#ifdef OSG
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    // Create a plotter
    ROSGPlotter* thePlotter = mainWindow->getOSGPlotter();
#endif

    RMatrixOperations theMatrixOperations(theDomain);


    // Call the assembler to get the number of free DOFs
    int numDOFs = theAssembler->getNumFreeDOFs();


    // Allocate memory for stiffness (K), mass (M) and damping (C) matrices
    QVector< QVector<double> > K(numDOFs, QVector<double>(numDOFs));
    QVector< QVector<double> > M(numDOFs, QVector<double>(numDOFs));
    QVector< QVector<double> > C(numDOFs, QVector<double>(numDOFs));


    // Establish M, K and C
    theAssembler->assembleMatricesAndVectors(&M,&C,&K,0,0,0.0,0,0);


    // Allocate memory for displacement, acceleration, velocity and external force vectors at time step i and i+1
    QVector<double> displacement(numDOFs);
    QVector<double> velocity(numDOFs);
    QVector<double> acceleration(numDOFs);
    QVector<double> PatTimeStepiplus1(numDOFs);


    // Declare a double to determine the maximum displacement
    double maximum_displacement=0;


    // Declare integer for updating the plot
    int i=0;


    // Perform dynamic analysis
    for (double time = 0; time <(theEndTime+theTimeStep); time += theTimeStep)
    {
        // Increment integer at every time step
        i++;

        // Keep the user interface responsive in case this takes time to process...
        QCoreApplication::processEvents();

        // Assemble the vector of external forces at current time step
        theAssembler->assembleMatricesAndVectors(0,0,0,0,&PatTimeStepiplus1,time,0,0);


        // Send values to Newmark time stepper to get response at current time step
        theTimeStepper->conductNewmarkIteration(true, &M, &K, &C, &PatTimeStepiplus1, nullptr, &displacement, &velocity, &acceleration , theTimeStep,nullptr);


        //qDebug()<< 1*displacement[0]*1000;


        double maxDisplacement = theMatrixOperations.maxAbs(&displacement);

        double scaleFactor = sin(time*3.14154*10.0);

        QVector<double> displacement2 = displacement;

        theMatrixOperations.scaleVector(scaleFactor,&displacement2,&displacement);

        // Give the new displacements to the nodes
        theAssembler->setDeformations(&displacement);

        //qDebug()<<theMatrixOperations.maxAbs(&displacement);

        // Determine maximum displacement
        if (maximum_displacement < maxDisplacement)
        {
            maximum_displacement=maxDisplacement;
        }


        // Call commit although this is a linear analysis, to make sure responses are set in the components (max strain, etc)
        theAssembler->commitState();


#ifdef OSG
        // Update the plot
        if ( (remainder(i-1, thePlotInterval) == 0.0) && (theOutputLevel >= RObject::Medium) ) {
            qDebug() << this->objectName() << "has reached time" << time;
            qDebug() << "";
            thePlotter->plotStructure(true,"Dynamic",time);
        }
        else if (time==0.0) {
           thePlotter->plotStructure(true,"Dynamic",time);
        }
#endif

        // Give the responses to the time series recorders (no need to wait for "commit" in a linear analysis)
        recordResponses(time);

    }



    qDebug() <<"In "<< this->objectName() << "the Maximum displacement is " << maximum_displacement;


    #if defined(DETAILEDBUILDINGS) && defined(OSG)
        thePlotter->plotDynamicStructure();
    #endif


    return 0;

}
