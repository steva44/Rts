#ifndef RBroydenEquationSolutionAlgorithm_H
#define RBroydenEquationSolutionAlgorithm_H
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

// Wrapper class for OpenSees Broyden
// Stevan Gavrilovic 12/2018

// Broyden(int tangent = CURRENT_TANGENT, int n = 10);

#include "REquiSolnAlgo.h"

class EquiSolnAlgo;
class Broyden;
class RConvergenceTest;

class RBroydenEquationSolutionAlgorithm : public REquiSolnAlgo
{
    Q_OBJECT

    Q_PROPERTY(QObject *ConvergenceTest         READ getTheConvergenceTest      WRITE setTheConvergenceTest)
    Q_PROPERTY(int NumberSteps                  READ getNumStepsNewTangent      WRITE setNumStepsNewTangent)

public:
    // Define the constructor and destructor
    RBroydenEquationSolutionAlgorithm(QObject *parent, QString name);
    ~RBroydenEquationSolutionAlgorithm();

    QObject *getTheConvergenceTest() const;
    void setTheConvergenceTest(QObject *value);

    EquiSolnAlgo* getTheEquationSolutionAlgo();

    int getNumStepsNewTangent() const;
    void setNumStepsNewTangent(int value);

protected:

    std::unique_ptr<Broyden> theEquiSolutionAlgo;
    RConvergenceTest   *theConvergenceTest;
    int numStepsNewTangent;

};

#endif
