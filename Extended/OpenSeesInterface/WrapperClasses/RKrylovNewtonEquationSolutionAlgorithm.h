#ifndef RKrylovNewtonEquationSolutionAlgorithm_H
#define RKrylovNewtonEquationSolutionAlgorithm_H
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

// Wrapper class for OpenSees KrylovNewton
// Stevan Gavrilovic 12/2018

//  KrylovNewton(ConvergenceTest &theTest, int tangent = CURRENT_TANGENT,
//  double iFactor = 0.0, double cFactor = 1.0);

#include "REquiSolnAlgo.h"

class EquiSolnAlgo;
class KrylovNewton;
class RConvergenceTest;

class RKrylovNewtonEquationSolutionAlgorithm : public REquiSolnAlgo
{
    Q_OBJECT

    Q_PROPERTY(QObject *ConvergenceTest         READ getTheConvergenceTest      WRITE setTheConvergenceTest)

public:
    RKrylovNewtonEquationSolutionAlgorithm(QObject *parent, QString name);
    ~RKrylovNewtonEquationSolutionAlgorithm();

    QObject *getTheConvergenceTest() const;
    void setTheConvergenceTest(QObject *value);

    EquiSolnAlgo* getTheEquationSolutionAlgo();

protected:

    std::unique_ptr<KrylovNewton> theEquiSolutionAlgo;
    RConvergenceTest   *theConvergenceTest;

};

#endif
