#ifndef RCTestNormDispIncr_H
#define RCTestNormDispIncr_H
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

// Wrapper class for OpenSees CTestNormDispIncr
// Stevan Gavrilovic 12/2018

#include "RConvergenceTest.h"

class ConvergenceTest;
class CTestNormDispIncr;

//$tol	the tolerance criteria used to check for convergence
//$iter	the max number of iterations to check before returning failure condition
//$pFlag	optional print flag, default is 0. valid options:
//0 print nothing
//1 print information on norms each time test() is invoked
//2 print information on norms and number of iterations at end of successful test
//4 at each step it will print the norms and also the ΔU and R(U) vectors.
//5 if it fails to converge at end of $numIter it will print an error message BUT RETURN A SUCCESSFUL test
//$nType	optional type of norm, default is 2. (0 = max-norm, 1 = 1-norm, 2 = 2-norm, ...)

class RCTestNormDispIncr : public RConvergenceTest
{
    Q_OBJECT

    Q_PROPERTY(double Tolerance         READ getTol             WRITE setTol)
    Q_PROPERTY(int MaxNumberIncrements  READ getMaxNumIter      WRITE setMaxNumIter)
    Q_PROPERTY(int PrintFlag            READ getPrintFlag       WRITE setPrintFlag)
    Q_PROPERTY(int NormType             READ getNormType        WRITE setNormType)

public:
    // Define the constructor and destructor
    RCTestNormDispIncr(QObject *parent, QString name);
    ~RCTestNormDispIncr();

    double getTol() const;
    void setTol(double value);

    int getMaxNumIter() const;
    void setMaxNumIter(int value);

    ConvergenceTest* getTheConvergenceTest();

    int getPrintFlag() const;
    void setPrintFlag(int value);

    int getNormType() const;
    void setNormType(int value);

protected:

    std::unique_ptr<CTestNormDispIncr> theConvergenceTest;

    double tol;
    int maxNumIter;
    int print;
    int printFlag;
    int normType;

};

#endif
