#ifndef RInelasticStaticStructuralAnalysis_H
#define RInelasticStaticStructuralAnalysis_H
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

class RConstraintHandler;
class RDOFNumberer;
class RLinearSOE;
class RIntegrator;
class REquiSolnAlgo;
class RConvergenceTest;

class EquiSolnAlgo;
class StaticAnalysis;
class Domain;

class RInelasticStaticStructuralAnalysis : public RStructuralAnalysis
{
	Q_OBJECT
    Q_PROPERTY(QObject *ConstraintHandler           READ getTheConstraintHandler    WRITE setTheConstraintHandler)
    Q_PROPERTY(QObject *DOFNumberer                 READ getTheDOFNumberer          WRITE setTheDOFNumberer)
    Q_PROPERTY(QObject *LinearSOESolver             READ getTheLinearSOESolver      WRITE setTheLinearSOESolver)
    Q_PROPERTY(QObject *Integrator                  READ getTheIntegrator           WRITE setTheIntegrator)
    Q_PROPERTY(QObject *ConvergenceTest             READ getTheConvergenceTest      WRITE setTheConvergenceTest)
    Q_PROPERTY(QObject *EquationSolutionAlgorithm   READ getTheEquiSolnAlgo         WRITE setTheEquiSolnAlgo)
    Q_PROPERTY(int MaxSteps                         READ getMaxSteps                WRITE setMaxSteps)

public:
	RInelasticStaticStructuralAnalysis(QObject *parent, QString name);
    ~RInelasticStaticStructuralAnalysis() override;

    void reset() override;

    int conductStructuralAnalysis()  override;

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

    int getMaxSteps() const;
    void setMaxSteps(int value);

private:

    RConstraintHandler *theConstraintHandler;
    RDOFNumberer       *theDOFNumberer;
    RLinearSOE         *theLinearSOESolver;
    RIntegrator        *theIntegrator;
    REquiSolnAlgo      *theSolutionAlgo;
    RConvergenceTest   *theConvergenceTest;

    std::unique_ptr<StaticAnalysis> theLinearStaticAnalysis;

    int createOpenSeesLinearStaticAnalysis(void);

    int maxSteps;
};

#endif
