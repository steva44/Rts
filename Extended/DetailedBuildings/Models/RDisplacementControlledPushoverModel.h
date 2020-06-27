#ifndef RDisplacementControlledPushoverModel_H
#define RDisplacementControlledPushoverModel_H
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

// This model performs a displacement controlled pushover analysis on a structure
// Produces a plot of the drift vs. input responses
// A displacement control integrator must be given to the model

#include "RResponse.h"
#include "RModel.h"

class RParameter;
class RResponse;
class RDisplacementControlIntegrator;
class RStructuralAnalysis;
class RStructuralAnalysisModel;
class QCustomPlot;

class RDisplacementControlledPushoverModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)
    Q_PROPERTY(double FinalDisplacement READ getFinalDisp WRITE setFinalDisp)
    Q_PROPERTY(QString NumberSteps READ getNumSteps WRITE setNumSteps)
    Q_PROPERTY(QObject* Integrator READ getIntegrator WRITE setIntegrator)
    Q_PROPERTY(QObject *StructuralAnalysisMethod READ getSAMethod WRITE setSAMethod)

public:
    RDisplacementControlledPushoverModel(QObject *parent, QString name);
    ~RDisplacementControlledPushoverModel();

    QString getInputParameterList();
    void setInputParameterList(QString value);

    QObject *getIntegrator() const;
    void setIntegrator(QObject *value);

    double getFinalDisp() const;
    void setFinalDisp(const double value);

    QString getNumSteps();
    void setNumSteps(QString value);
    double getNumStepsValue();

    QObject *getSAMethod() const;
    void setSAMethod(QObject *value);

    int evaluateModel(RGradientType theGradientType);

    // Maximum drift of the structure
    double getMaxDrift();

    // Target displacement
    void setTargetDisplacement(double target);

    // Test to remove
    void fixTopOfWall();

private:

    QList<QPointer<RParameter> > theInputParameterList;

    RStructuralAnalysis* theStructAnalysisMethod;
    RDisplacementControlIntegrator* theIntegrator;
    RResponse *theResponse;

    QPointer<RParameter> theNumStepsParameter;
    double theNumStepsDouble;

    double finalDisplacement;
    QPointer<QCustomPlot> customPlot;
};

#endif
