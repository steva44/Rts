#ifndef RLinearDynamicStructuralAnalysis_H
#define RLinearDynamicStructuralAnalysis_H

#include "RStructuralAnalysis.h"
#include "RAssembler.h"
#include "RLinearSolver.h"
#include "RTimeSeries.h"
#include "RTimeStepper.h"

class RLinearDynamicStructuralAnalysis : public RStructuralAnalysis
{
    Q_OBJECT

    Q_PROPERTY(QObject *Assembler READ getAssembler WRITE setAssembler)
    Q_PROPERTY(QObject *LinearSolver READ getLinearSolver WRITE setLinearSolver)
    Q_PROPERTY(QObject *TimeStepper READ getTimeStepper WRITE setTimeStepper)
    Q_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)
    Q_PROPERTY(double EndTime READ getEndTime WRITE setEndTime)
    Q_PROPERTY(int PlotInterval READ getPlotInterval WRITE setPlotInterval)

public:
    RLinearDynamicStructuralAnalysis(QObject *parent, QString name);
    ~RLinearDynamicStructuralAnalysis();


    QObject *getTimeStepper() const;
    void setTimeStepper(QObject *value);

    double getTimeStep() const;
    void setTimeStep(double value);

    double getEndTime() const;
    void setEndTime(double value);

    int getPlotInterval() const;
    void setPlotInterval(int value);

    int conductStructuralAnalysis(RGradientType theGradientType, QList<RParameter *> theAllParametersList);

    int setRecorders(RStructuralAnalysisModel *theStructuralModel, QList<QPointer<RObject> > passedStructuralObjects, QStringList passedResponses);

private:

    int recordResponses(double time);

    QStringList theResponses;
    QList<RResponse *> theResponseList;
    QList<QPointer<RObject> > theStructuralObjects;

    QPointer<RTimeStepper> theTimeStepper;

    double theTimeStep;
    double theEndTime;
    int thePlotInterval;
    bool recordersHaveBeenCreated;

};

#endif
