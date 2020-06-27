#ifndef RInelasticDynamicStructuralAnalysis_H
#define RInelasticDynamicStructuralAnalysis_H
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

#include "RStructuralAnalysis.h"
#include "qcustomplot.h"

class RConstraintHandler;
class RDOFNumberer;
class RLinearSOE;
class RIntegrator;
class RConvergenceTest;
class REquiSolnAlgo;

class DirectIntegrationAnalysis;
class Domain;

// The NumberSteps parameter is the number of analysis steps that will be performed with a given time-step dt
// For example, if a time-series has 1000 points with a time-step of 0.01, then to analyze the entire time-series, the NumberSteps should be 1000 and TimeStep should be 0.01
// If these properties are left blank, the default setting will be to evaluate the entire time-series at the time-steps provided by the series
// Alternatively, a user can specify any timeStep and any number of steps. The series will interpolate between values and the analysis will only evaluate up to the number of steps specified.

class RInelasticDynamicStructuralAnalysis : public RStructuralAnalysis
{
    Q_OBJECT

    // OpenSees stuff
    Q_PROPERTY(QObject *ConstraintHandler           READ getTheConstraintHandler    WRITE setTheConstraintHandler)
    Q_PROPERTY(QObject *DOFNumberer                 READ getTheDOFNumberer          WRITE setTheDOFNumberer)
    Q_PROPERTY(QObject *LinearSOESolver             READ getTheLinearSOESolver      WRITE setTheLinearSOESolver)
    Q_PROPERTY(QObject *Integrator                  READ getTheIntegrator           WRITE setTheIntegrator)
    Q_PROPERTY(QObject *ConvergenceTest             READ getTheConvergenceTest      WRITE setTheConvergenceTest)
    Q_PROPERTY(QObject *EquationSolutionAlgorithm   READ getTheEquiSolnAlgo         WRITE setTheEquiSolnAlgo)
    Q_PROPERTY(double TimeStep                      READ getTimeStep                WRITE setTimeStep)
    Q_PROPERTY(double EndTime                       READ getEndTime                 WRITE setEndTime)
    Q_PROPERTY(double StartTime                     READ getStartTime               WRITE setStartTime)
    Q_PROPERTY(int NumberSteps                      READ getNumSteps                WRITE setNumSteps)
    Q_PROPERTY(int PlotInterval                     READ getPlotInterval            WRITE setPlotInterval)
    Q_PROPERTY(double* RayleighDamping              READ getRayleighDamping         WRITE setRayleighDamping)

    //Rts stuff

public:
    RInelasticDynamicStructuralAnalysis(QObject *parent, QString name);
    ~RInelasticDynamicStructuralAnalysis();

    int conductStructuralAnalysis();

    QObject *getTheConstraintHandler() const;
    void setTheConstraintHandler(QObject *value);

    QObject *getTheDOFNumberer() const;
    void setTheDOFNumberer(QObject *value);

    QObject *getTheLinearSOESolver() const;
    void setTheLinearSOESolver(QObject *value);

    QObject *getTheIntegrator() const;
    void setTheIntegrator(QObject *value);

    QObject *getTheConvergenceTest() const;
    void setTheConvergenceTest(QObject *value);

    QObject *getTheEquiSolnAlgo() const;
    void setTheEquiSolnAlgo(QObject *value);

    double getTimeStep() const;
    void setTimeStep(double value);

    double getEndTime() const;
    void setEndTime(double value);

    double getStartTime() const;
    void setStartTime(double value);

    int getPlotInterval() const;
    void setPlotInterval(int value);

    int getMaxSteps() const;
    void setMaxSteps(int value);

    double *getRayleighDamping() const;
    void setRayleighDamping(double *value);

    int getNumSteps() const;
    void setNumSteps(int value);

private:

    int createOpenSeesLinearDirectIntegrationAnalysis(void);

    RConstraintHandler *theConstraintHandler;
    RDOFNumberer       *theDOFNumberer;
    RLinearSOE         *theLinearSOESolver;
    RIntegrator        *theIntegrator;
    REquiSolnAlgo      *theSolutionAlgo;
    RConvergenceTest   *theConvergenceTest;

    std::unique_ptr<DirectIntegrationAnalysis> theTimeHistoryAnalysis;

    double* rayleighDamping;
    double theTimeStep;
    double theStartTime;
    double theEndTime;
    int thePlotInterval;
    int numSteps;
};

#endif
