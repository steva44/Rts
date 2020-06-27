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

#include "RNatafProbabilityTransformation.h"
#include "RDomain.h"
#include "RRandomVariable.h"
#include "RContinuousRandomVariable.h"
#include "RCorrelation.h"
#include "RMatrixOperations.h"
#include "RCholeskyDecomposition.h"
#include "RProbabilityDistributions.h"

#include <QCoreApplication>
#include <math.h>

RNatafProbabilityTransformation::RNatafProbabilityTransformation(QObject *parent, QString &name)
    : RProbabilityTransformation(parent, name)
{
    numRVs = 0;

    theProbabilityDistributions = nullptr;
    theCholeskyDecomposition = nullptr;
}


RNatafProbabilityTransformation::~RNatafProbabilityTransformation()
{

}


QObject * RNatafProbabilityTransformation::getProbabilityDistributions() const
{
    return theProbabilityDistributions;
}


void RNatafProbabilityTransformation::setProbabilityDistributions(QObject *value)
{
    theProbabilityDistributions = qobject_cast<RProbabilityDistributions *>(value);
}


QObject * RNatafProbabilityTransformation::getCholeskyDecomposition() const
{
    return theCholeskyDecomposition;
}


void RNatafProbabilityTransformation::setCholeskyDecomposition(QObject *value)
{
    theCholeskyDecomposition = qobject_cast<RCholeskyDecomposition *>(value);
}


int RNatafProbabilityTransformation::initializeAtStartOfAnalysis()
{
    // Check that the random variable list has been populated
    if (!randomVariableList) {
        return -1;
    }


    // Get number of random variables
    numRVs = randomVariableList->count();


//    // Instantiate the Cholesky matrix
//    if (L) {
//        delete L;
//        L = 0;
    //    }
    L.release();
    L = std::make_unique<QVector<QVector<double>>>(numRVs, QVector<double>(numRVs));


    // Initialize the correlation matrix
    R0 = std::make_unique<QVector<QVector<double>>>(numRVs, QVector<double>(numRVs));



    // Establish the correlation matrix R0

    // First set R0 to the Identity matrix


    for (int i = 0; i < numRVs; ++i) {
        (*R0)[i][i] =1.0;
    }


    int status  = establishCorrelationMatrix(R0.get());
    if (status < 0) {
        qCritical() << "Error: Cannot establish the correlation matrix.";
        return -1;
    }


    // Compute cholesky
   // Linv = new QVector< QVector<double> >(numRVs, QVector<double>(numRVs));
    Linv = std::make_unique<QVector<QVector<double>>>(numRVs, QVector<double>(numRVs));
    status = theCholeskyDecomposition->getCholesky(R0.get(), L.get(), Linv.get());
    if (status < 0) {
        qCritical() << "Error: Cannot compute the Cholesky decomposition of the correlation matrix.";
        return -1;
    }




    // Print the correlation matrix if the user asked for it
    if (theOutputLevel == RObject::Maximum) {
        qDebug() << "The correlation matrix:";
        theDomain->print(R0.get());
        qDebug() << " ";
    }


    return 0;
}


int RNatafProbabilityTransformation::establishCorrelationMatrix(QVector< QVector<double> > *R0, RRandomVariable *perturbedRV)
{
    // Set the correlation coefficients that the user has given
    QList<RCorrelation *> correlationList = theDomain->findChildren<RCorrelation *>();
    RRandomVariable *rv1;
    RRandomVariable *rv2;
    int index1;
    int index2;
    double rho;
    double rho0;
    for (int i = 0; i < correlationList.count(); ++i) {

        // Get the two random variable objects
        rv1 = qobject_cast<RRandomVariable *>(correlationList[i]->getRandomVariable1());
        rv2 = qobject_cast<RRandomVariable *>(correlationList[i]->getRandomVariable2());


        // If this correlation matrix is being established as a result of a perturbation for a sensitivity analysis, then check if one of rv1 or rv2 is the perturbed one; otherwise, conitue.
        if (perturbedRV) {
            if (rv1 != perturbedRV && rv2 != perturbedRV) {
                continue;
            }
        }



        // Do nothing if one or both of the random variables are not subject to this analysis
        if ((!rv1) || (!rv2)) {
            qCritical() << "Warning: Two random variables must be given to the correlation object" << correlationList[i]->objectName() << ".";
            continue;
        }


        // Do nothing if the random variable wasn't found in the random variable list
        index1 = randomVariableList->indexOf(rv1);
        index2 = randomVariableList->indexOf(rv2);
        if ((index1 < 0) || (index2 < 0)) {
            continue;
        }


        // Get the correlation value
        rho = correlationList[i]->getCoefficient();


        // Modify the correlation coefficient
        rho0 = modifyCorrelation(rv1, rv2, rho);


        // Print the correlation matrix if the user asked for it
        if (theOutputLevel >= RObject::Medium) {
            qDebug() << "The correlation coefficient" << correlationList[i]->objectName() << "is changed from" << rho << "to" << rho0;
        }


        // Set the correlation coefficient into the matrix
        (*R0)[index1][index2] = rho0;
        (*R0)[index2][index1] = rho0;
    }

    return 0;
}


double RNatafProbabilityTransformation::modifyCorrelation(RRandomVariable *rv_i, RRandomVariable *rv_j, double rho_original)
{
    double mean_i = rv_i->getMeanValue();
    double mean_j = rv_j->getMeanValue();

    double stdv_i = rv_i->getStandardDeviationValue();
    double stdv_j = rv_j->getStandardDeviationValue();


    double result = 0.0;

    double tol = 1.0e-6;
    double pert = 1.0e-4;

    double rho_old = rho_original;
    double rho_new;
    double f = 0.0;
    double df = 0.0;
    double perturbed_f;

    for (int i = 1; i <= 100; ++i) {


        // Evaluate the function
        residualFunction(rho_original,rho_old, rv_i, mean_i, stdv_i, rv_j, mean_j, stdv_j, &f, &df);


        ///////////////////////////////////////////////////////////////////////////////////////////
        // Evaluate perturbed function
        residualFunction(rho_original, (rho_old+pert),  rv_i, mean_i, stdv_i, rv_j, mean_j, stdv_j, &perturbed_f, &df);


        // Evaluate derivative of function
        df = (perturbed_f - f) / pert;
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////



        if (qAbs(df) < 1.0e-15) {
            qCritical() << "Error: The derivative used in the Newton algorithm within the Nataf transformation is zero.";
        }
        else {

            // Take a Newton step
            rho_new = rho_old - f/df;

            QCoreApplication::processEvents();



            // Check convergence; quit or continue
            if (fabs(1.0-fabs(rho_old/rho_new)) < tol) {
                result = rho_new;
                return result;
            }
            else {
                if (i==100) {
                    qCritical() << "Error: The Newton algorithm within the Nataf transformation did not converge.";
                    result = 0.0;
                    return result;
                }
                else {
                    rho_old = rho_new;
                }

            }
        }
    }


    return result;
}


void RNatafProbabilityTransformation::residualFunction(double rho_original, double rho, RRandomVariable *rv_i, double mean_i, double stdv_i, RRandomVariable *rv_j, double mean_j, double stdv_j, double *f, double *df)
{
    *f = rho_original - doubleIntegral(rv_i, mean_i, stdv_i, rv_j, mean_j, stdv_j, rho, false);

    //*df = -doubleIntegral(rv_i, mean_i, stdv_i, rv_j, mean_j, stdv_j, rho, true);

    return;
}


double RNatafProbabilityTransformation::doubleIntegral(RRandomVariable *rv_i, double mean_i, double stdv_i, RRandomVariable *rv_j, double mean_j, double stdv_j, double rho, bool evaluateDerivative)
{

    // The grid of integration points:
    // 1, 2, ..., i, ..., 2*n in x-direction with intervals h
    // 1, 2, ..., j, ..., 2*m in y-direction with intervals k


    // Selected integration boundaries
    double bound = 5.6;
    double z_i0 = -bound;
    double z_in = bound;
    double z_j0 = -bound;
    double z_jm = bound;

    // Half the number of integration points
    int numPoints = 25;
    int n = numPoints;
    int m = numPoints;

    // Integration point intervals
    double h = (z_in-z_i0)/(2.0*n);
    double k = (z_jm-z_j0)/(2.0*m);

    // Grid of integration points
    QVector<double> z_i;
    QVector<double> z_j;
    for (int i = 0; i<=2*n; ++i) {
        z_i << z_i0 + i*h;
    }
    for (int j=0; j<=2*m; ++j) {
        z_j << z_j0 + j*k;
    }

    // Computing sums (naming terms according to p. 126 in "Numerical Methods" by Faires & Burden)
    double term1 = 0.0;
    double term2 = 0.0;
    double term3 = 0.0;
    double term4 = 0.0;
    double term5 = 0.0;
    double term6 = 0.0;
    double term7 = 0.0;
    double term8 = 0.0;
    double term9 = 0.0;
    double term10 = 0.0;
    double term11 = 0.0;
    double term12 = 0.0;
    double term13 = 0.0;
    double term14 = 0.0;
    double term15 = 0.0;
    double term16 = 0.0;

    term1 = integrand( rv_i, z_i[0] ,    mean_i, stdv_i, rv_j,   z_j[0] ,     mean_j, stdv_j, rho, evaluateDerivative);
    term4 = integrand( rv_i, z_i[2*n] ,  mean_i, stdv_i, rv_j,   z_j[0] ,     mean_j, stdv_j, rho, evaluateDerivative);
    term13 = integrand(rv_i, z_i[0] ,    mean_i, stdv_i, rv_j,   z_j[2*m],    mean_j, stdv_j, rho, evaluateDerivative);
    term16 = integrand(rv_i, z_i[2*n],   mean_i, stdv_i, rv_j,   z_j[2*m],    mean_j, stdv_j, rho, evaluateDerivative);

    for (int i=1; i<=n; ++i) {
        term3 += integrand(rv_i,  z_i[2*i-1],     mean_i, stdv_i, rv_j,  z_j[0],       mean_j, stdv_j, rho, evaluateDerivative);
        term15 += integrand(rv_i, z_i[2*i-1],     mean_i, stdv_i, rv_j,  z_j[2*m],     mean_j, stdv_j, rho, evaluateDerivative);
    }

    for (int i=1; i<=n-1; ++i) {
        term2 += integrand(rv_i,  z_i[2*i],       mean_i, stdv_i, rv_j,  z_j[0],       mean_j, stdv_j, rho, evaluateDerivative);
        term14 += integrand(rv_i, z_i[2*i],       mean_i, stdv_i, rv_j,  z_j[2*m],     mean_j, stdv_j, rho, evaluateDerivative);
    }

    for (int j=1; j<=m-1; ++j) {
        term5 += integrand(rv_i,   z_i[0],    mean_i, stdv_i, rv_j,   z_j[2*j],   mean_j, stdv_j, rho, evaluateDerivative);
        term8 += integrand(rv_i,   z_i[2*n],  mean_i, stdv_i, rv_j,   z_j[2*j],   mean_j, stdv_j, rho, evaluateDerivative);
    }

    for (int j=1; j<=m; ++j) {
        term9 += integrand(rv_i,   z_i[0],   mean_i, stdv_i, rv_j,   z_j[2*j-1],   mean_j, stdv_j, rho, evaluateDerivative);
        term12 += integrand(rv_i,  z_i[2*n], mean_i, stdv_i, rv_j,   z_j[2*j-1],   mean_j, stdv_j, rho, evaluateDerivative);
    }

    for (int j=1; j<=(m-1); ++j) {
        for (int i=1; i<=(n-1); ++i) {
            term6 += integrand(rv_i, z_i[2*i], mean_i, stdv_i, rv_j, z_j[2*j], mean_j, stdv_j, rho, evaluateDerivative);
        }
    }

    for (int j=1; j<=(m-1); ++j) {
        for (int i=1; i<=(n); ++i) {
            term7 += integrand(rv_i, z_i[2*i-1], mean_i, stdv_i, rv_j, z_j[2*j], mean_j, stdv_j, rho, evaluateDerivative);
        }
    }
    for (int j=1; j<=(m); ++j) {
        for (int i=1; i<=(n-1); ++i) {
            term10 += integrand(rv_i, z_i[2*i], mean_i, stdv_i, rv_j, z_j[2*j-1], mean_j, stdv_j, rho, evaluateDerivative);
        }
    }
    for (int j=1; j<=(m); ++j) {
        for (int i=1; i<=(n); ++i) {
            term11 += integrand(rv_i, z_i[2*i-1], mean_i, stdv_i, rv_j, z_j[2*j-1], mean_j, stdv_j, rho, evaluateDerivative);
        }
    }


    double par1 = term1 + 2.0*term2 + 4.0*term3 + term4;
    double par2 = term5 + 2.0*term6 + 4.0*term7 + term8;
    double par3 = term9 + 2.0*term10 + 4.0*term11 + term12;
    double par4 = term13 + 2.0*term14 + 4.0*term15 + term16;
    double result = h*k/9.0 * (par1 + 2.0*par2 + 4.0*par3 + par4);

    return result;
}


double RNatafProbabilityTransformation::integrand(RRandomVariable *rv_i, double z_i, double mean_i, double stdv_i, RRandomVariable *rv_j, double z_j, double mean_j, double stdv_j, double rho_zero, bool evaluateDerivative)
{

    double x_i = rv_i->getInverseCDFValue(theProbabilityDistributions->getNormalCDF(z_i, 1.0));
    double x_j = rv_j->getInverseCDFValue(theProbabilityDistributions->getNormalCDF(z_j, 1.0));

    double thePhi2 = phi2(z_i, z_j, rho_zero);

    double result;
    if (evaluateDerivative) {
        double factor = rho_zero/(2*3.14159265359*(1.0-rho_zero*rho_zero)) + (z_i*z_j*(1.0-rho_zero*rho_zero) - (rho_zero)*( z_i*z_i - 2*rho_zero*z_i*z_j+z_j*z_j ) ) / ((1.0-rho_zero*rho_zero)*(1.0-rho_zero*rho_zero));
        result = ( (x_i-mean_i)/stdv_i * (x_j-mean_j)/stdv_j * thePhi2 * factor);
    }
    else {
        result = ( (x_i-mean_i)/stdv_i * (x_j-mean_j)/stdv_j * thePhi2 );
    }

    return result;
}


double RNatafProbabilityTransformation::phi2(double z_i, double z_j, double rho)
{
    double par = z_i*z_i + z_j*z_j - 2.0*rho*z_i*z_j;
    double theExp = exp(-par/(2.0*(1.0-rho*rho)));
    double result = theExp/(2.0*3.14159265359*sqrt(1.0-rho*rho));

    return result;
}


int RNatafProbabilityTransformation::xToz(const QVector<double> *x, QVector<double> *z)
{
    // Loop through the random variables to create the "z-vector"
    RRandomVariable *randomVariable;
    for (int i = 0; i < numRVs; ++i) {
        randomVariable = qobject_cast<RRandomVariable *>(randomVariableList->value(i));

        (*z)[i] = theProbabilityDistributions->getInverseNormalCDF(randomVariable->getCDFValue((*x)[i]), 1.0);
    }

    return 0;
}


int RNatafProbabilityTransformation::yToz(const QVector<double> *y, QVector<double> *z)
{
    int size=z->size();
    for (int i = 0; i < size; ++i) {
        (*z)[i] = y->at(i);
    }


    // Create matrix operations tool
    RMatrixOperations theMatrixOperations(theDomain);
    theMatrixOperations.multiplyMatrixVector(false, 1.0, L.get(), z, 0.0, z);

    return 0;
}


int RNatafProbabilityTransformation::transformPointToStandardNormalSpace(const QVector<double> *originalVector, QVector<double> *standardNormalVector)
{
    // Make sure that the "initialize" method has been called
    if (numRVs == 0) {
        qCritical() << "ERROR: The probability transformation has been called to carry out a task without having any random variables or without being initialized.";
    }

    // Create the "z-vector"
    //QVector<double> *z = new QVector<double>(numRVs);
    std::unique_ptr<QVector<double>> z = std::make_unique<QVector<double>>(numRVs);
    int status = xToz(originalVector, z.get());
    if (status < 0) {
        qCritical() << "Error: Cannot transform the vector of random variables to the standard normal space.";
        return -1;
    }

    // Compute the product standardNormalVector = L^-1 * z for the trianular matrix L^-1
    for (int i = 0; i < numRVs; ++i) {
        (*standardNormalVector)[i] = z->at(i);
    }


    // Create matrix operations tool
    RMatrixOperations theMatrixOperations(theDomain);


    status = theMatrixOperations.multiplyMatrixVector(false, 1.0, Linv.get(), standardNormalVector, 0.0, standardNormalVector);


    // Print standardNormalVector if the user calls for it
    if (theOutputLevel >= RObject::Medium) {
        qDebug() << "Current point in standard-normal space, u:";
        theDomain->print(standardNormalVector);
        qDebug() << "";
    }

    return 0;
}


int RNatafProbabilityTransformation::transformPointToOriginalSpace(const QVector<double> *standardNormalVector, QVector<double> *originalVector)
{
    // Make sure that the "initialize" method has been called
    if (numRVs == 0) {
        qCritical() << "ERROR: The probability transformation has been called to carry out a task without having any random variables or without being initialized.";
    }


    // z = L*y
    //QVector<double> *z = new QVector<double>(numRVs);
    std::unique_ptr<QVector<double>> z = std::make_unique<QVector<double>>(numRVs);

    int status = yToz(standardNormalVector, z.get());
    if (status < 0) {
        qCritical() << "Error: Cannot transform the vector of stanadrd normal random variables to the original space.";
        return -1;
    }



    // Loop through the random variables to create the vector in the original space
    RRandomVariable *randomVariable;
    for (int i = 0; i < numRVs; ++i) {
        randomVariable = qobject_cast<RRandomVariable *>(randomVariableList->value(i));
        (*originalVector)[i] = randomVariable->getInverseCDFValue(theProbabilityDistributions->getNormalCDF((*z)[i], 1.0));
    }


    // Print the original vector if the user asks for it
    if (theOutputLevel == RObject::Maximum) {
        qDebug() << "Current point in original space, x:";
        theDomain->print(originalVector);
        qDebug() << "";
    }


    return 0;
}


int RNatafProbabilityTransformation::transformGradientToOriginalSpace(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, const QVector<double> *vectorInStandardNormalSpace, QVector<double> *vectorInOriginalSpace)
{
    // Make sure that the "initialize" method has been called
    if (numRVs == 0) {
        qCritical() << "ERROR: The probability transformation has been called to carry out a task without having any random variables or without being initialized.";
    }


    // z = L*y
    std::unique_ptr<QVector<double>> z = std::make_unique<QVector<double>>(numRVs);
    int status = yToz(pointInStandardNormalSpace, z.get());
    if (status < 0) {
        qCritical() << "Error: Cannot transform the vector of stanadrd normal random variables to the original space.";
        return -1;
    }


    // Establish the matrix diag(f/phi)
    RRandomVariable *randomVariable;
    //QVector< QVector<double> > *diag = new QVector< QVector<double> >(numRVs, QVector<double>(numRVs));
    std::unique_ptr<QVector<QVector<double>>> diag = std::make_unique<QVector<QVector<double>>>(numRVs, QVector<double>(numRVs));


    double phi, f;
    for (int i = 0; i < numRVs; ++i) {
        randomVariable = qobject_cast<RRandomVariable *>(randomVariableList->value(i));
        phi=theProbabilityDistributions->getNormalPDF((*z)[i] , 1.0);
        f = randomVariable->getPDFValue((*pointInOriginalSpace)[i]);

        (*diag)[i][i] = f/phi;
    }


    // Compute the vector-matrix product temp = L^-1 * dGdy for the trianular matrix L^-1
    //QVector<double> *temp = new QVector<double>(numRVs);

    // Create matrix operations tool
    RMatrixOperations theMatrixOperations(theDomain);

    std::unique_ptr<QVector<double>> temp = std::make_unique<QVector<double>>(*vectorInStandardNormalSpace);

//    for (int i = 0; i < numRVs; ++i) {
//        (*temp)[i] = vectorInStandardNormalSpace->at(i);
//    }

    status = theMatrixOperations.multiplyMatrixVector(true, 1.0, Linv.get(), temp.get(), 0.0, temp.get());


    // Finalize the product vectorInOriginalSpace = diag(f/phi) * L^-1 * dGdy = diag(f/phi) * temp

    for (int i = 0; i < numRVs; ++i) {
        (*vectorInOriginalSpace)[i] = temp->at(i);
    }

    status = theMatrixOperations.multiplyMatrixVector(true, 1.0, diag.get(), vectorInOriginalSpace, 0.0, vectorInOriginalSpace);

    // Free vectors and matrices used in this method

    return 0;
}


int RNatafProbabilityTransformation::transformGradientToStandardNormalSpace(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, const QVector<double> *vectorInOriginalSpace, QVector<double> *vectorInStandardNormalSpace)
{
    // Make sure that the "initialize" method has been called
    if (numRVs == 0) {
        qCritical() << "ERROR: The probability transformation has been called to carry out a task without having any random variables or without being initialized.";
    }


    // z = L*y
    std::unique_ptr<QVector<double>> z = std::make_unique<QVector<double>>(numRVs);
    int status = yToz(pointInStandardNormalSpace, z.get());
    if (status < 0) {
        qCritical() << "Error: Cannot transform the vector of stanadrd normal random variables to the original space.";
        return -1;
    }


    // Establish the matrix diag(phi/f)
    RRandomVariable *randomVariable;
    std::unique_ptr<QVector<QVector<double>>> diag = std::make_unique<QVector<QVector<double>>>(numRVs, QVector<double>(numRVs));
    double phi, f;
    for (int i = 0; i < numRVs; ++i) {
        randomVariable = qobject_cast<RRandomVariable *>(randomVariableList->value(i));

        phi=theProbabilityDistributions->getNormalPDF((*z)[i], 1.0);


        f = randomVariable->getPDFValue((*pointInOriginalSpace)[i]);
        (*diag)[i][i] = phi/f;
    }


    // Compute the vector-matrix product temp = diag * dgdx = diag * vectorInOriginalSpace
//    QVector<double> *temp = new QVector<double>(numRVs);

//    for (int i = 0; i < numRVs; ++i) {
//        (*temp)[i] = vectorInOriginalSpace->at(i);
//    }

    std::unique_ptr<QVector<double>> temp = std::make_unique<QVector<double>>(*vectorInOriginalSpace);


    // Create matrix operations tool
    RMatrixOperations theMatrixOperations(theDomain);


    status = theMatrixOperations.multiplyMatrixVector(true, 1.0, diag.get(), temp.get(), 0.0, temp.get());


    // Finalize the product vectorInStandardNormalSpace = L * diag * dgdx = L * temp

    for (int i = 0; i < numRVs; ++i) {
        (*vectorInStandardNormalSpace)[i] = temp->at(i);
    }

    status = theMatrixOperations.multiplyMatrixVector(true, 1.0, L.get(), vectorInStandardNormalSpace, 0.0, vectorInStandardNormalSpace);



    return 0;
}


int RNatafProbabilityTransformation::computeJacobian(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, QVector< QVector<double> > *jacobian, RProbabilityTransformation::RJacobianType jacobianType)
{
    // Make sure that the "initialize" method has been called
    if (numRVs == 0) {
        qCritical() << "ERROR: The probability transformation has been called to carry out a task without having any random variables or without being initialized.";
    }


    // z = L*y
    std::unique_ptr<QVector<double>> z = std::make_unique<QVector<double>>(numRVs);
    int status = yToz(pointInStandardNormalSpace, z.get());
    if (status < 0) {
        qCritical() << "Error: Cannot transform the vector of stanadrd normal random variables to the original space.";
        return -1;
    }


    // Establish the matrix diag(phi/f)
    RRandomVariable *randomVariable;
    std::unique_ptr<QVector< QVector<double>>> diag = std::make_unique<QVector<QVector<double>>>(numRVs, QVector<double>(numRVs));
    double phi, f;
    for (int i = 0; i < numRVs; ++i) {
        randomVariable = qobject_cast<RRandomVariable *>(randomVariableList->value(i));
        phi=theProbabilityDistributions->getNormalPDF((*z)[i],1.0);
        f = randomVariable->getPDFValue((*pointInOriginalSpace)[i]);

        double value = 0.0;
        if (jacobianType == RProbabilityTransformation::u_x) {
            value = f / phi;
        } else if (jacobianType == RProbabilityTransformation::x_u) {
            value = phi / f;
        }
        (*diag)[i][i] = value;
    }


    // Create matrix operations tool
    RMatrixOperations theMatrixOperations(theDomain);


    // Compute the matrix product diag*L
    if (jacobianType == RProbabilityTransformation::u_x) {
        int status = theMatrixOperations.multiplyMatrixMatrix(false, false, 1.0, Linv.get(), diag.get(), 0.0, jacobian);
    } else if (jacobianType == RProbabilityTransformation::x_u) {
        int status = theMatrixOperations.multiplyMatrixMatrix(false, false, 1.0, diag.get(), L.get(), 0.0, jacobian);
    }



    // Print jacobian
    if (theOutputLevel >= RObject::Medium) {
        QString jacobianName;
        if (jacobianType == RProbabilityTransformation::u_x) {
            jacobianName = "du/dx";
        } else if (jacobianType == RProbabilityTransformation::x_u) {
            jacobianName = "dx/du";
        }
        qDebug() << qPrintable(QString("Jacobian matrix of %1:").arg(jacobianName));
        theDomain->print(jacobian);
        qDebug() << "";
    }

    return 0;
}


int RNatafProbabilityTransformation::computeSensitivityToDistributionParameters(const QVector<double> *pointInOriginalSpace, QVector< QVector<double> > *dy_dMean, QVector< QVector<double> > *dy_dStdv)
{
    // Create the "z-vector"
    std::unique_ptr<QVector<double>> z = std::make_unique<QVector<double>>(numRVs);
    int status = xToz(pointInOriginalSpace, z.get());
    if (status < 0) {
        qCritical() << "Error: Cannot transform the vector of random variables to the standard normal space.";
        return -1;
    }

    std::unique_ptr<QVector<double>> dzdMeanVector = std::make_unique<QVector<double>>(numRVs);
    std::unique_ptr<QVector<double>> dzdStdvVector = std::make_unique<QVector<double>>(numRVs);
    std::unique_ptr<QVector<QVector<double>>> dLdMean = std::make_unique<QVector<QVector<double>>>(numRVs, QVector<double>(numRVs));
    std::unique_ptr<QVector<QVector<double>>> dLdStdv = std::make_unique<QVector<QVector<double>>>(numRVs, QVector<double>(numRVs));

    //Copy the wihtout iteration
    std::unique_ptr<QVector<QVector<double>>> Loriginal {std::make_unique<QVector<QVector<double>>>(*L)};
    std::unique_ptr<QVector<QVector<double>>> R0original{std::make_unique<QVector<QVector<double>>>(*R0)};

//    // Get a copy of the original L and R0
//    for (int i = 0; i < numRVs; ++i){
//        for (int j = 0; j < numRVs; ++j){
//            (*Loriginal)[i][j] = (*L)[i][j];
//        }
//    }

//    for (int i = 0; i < numRVs; ++i){
//        for (int j = 0; j < numRVs; ++j){
//            (*R0original)[i][j] = (*R0)[i][j];
//        }
//    }


    // Loop over all random variables. The sensitivity of the y-vector to each random variable mean or stdv will be a row of the Jacobian matrix.
    for (int i = 0; i < numRVs; ++i) {

        // Keep the GUI responsive
        QCoreApplication::processEvents();

        RContinuousRandomVariable *randomVariable = qobject_cast<RContinuousRandomVariable *>(randomVariableList->value(i));

        double xi = (*pointInOriginalSpace)[i];
        double zi = (*z)[i];



        // This sensitivity has two terms: (L^-1 * dz/dMean) + (-L^-1 * dL/dMean * L^-1 * z)



        // First, we need to compute (L^-1 * dz/dMean) and (L^-1 * dz/dStdv)

        // Computing dzdMean and dzdStdv
        double dzdMean = 0.0;
        double dzdStdv = 0.0;
        status = computeDzDMeanStdv(theProbabilityDistributions, randomVariable, xi, zi, &dzdMean, &dzdStdv);

        //No need to set to 0.0 since we are doing it above.
//        for (int j=0; j<dzdMeanVector->size(); ++j) {
//            (*dzdMeanVector)[j] = 0.0;
//        }

//        for (int j=0; j<dzdStdvVector->size(); ++j) {
//            (*dzdStdvVector)[j] = 0.0;
//        }


        (*dzdMeanVector)[i]=dzdMean;
        (*dzdStdvVector)[i]=dzdStdv;


        // Create matrix operations tool
        RMatrixOperations theMatrixOperations(theDomain);


        // Compute (L^-1 * dz/dMean) and (L^-1 * dz/dStdv): Use the GSL matrix-vector product for the triangular matrices

        int status = theMatrixOperations.multiplyMatrixVector(false, 1.0, Linv.get(), dzdMeanVector.get(), 0.0, dzdMeanVector.get());


        status = theMatrixOperations.multiplyMatrixVector(false, 1.0, Linv.get(), dzdStdvVector.get(), 0.0, dzdStdvVector.get());



        // Second, we need to compute (-L^-1 * dL/dMean * L^-1 * z) and (-L^-1 * dL/dStdv * L^-1 * z)

        // Computing dL/dMean and dL/dStdv by finite difference

        // Perturb R0 for mean sensitivity analysis and compute dLdMean
        double mean = randomVariable->getMeanValue();
        double stdv = randomVariable->getStandardDeviationValue();
        double h = stdv / 200.0;

        // Perturb the mean
        randomVariable->setMeanValue(mean + h);

        // Establish the perturbed R0
        status = establishCorrelationMatrix(R0.get(), randomVariable);
        if (status < 0) {
            qCritical() << "Error: Cannot establish the perturbed correlation matrix.";
            return -1;
        }


        // Compute the perturbed L
        status = theCholeskyDecomposition->getCholesky(R0.get(), L.get(), nullptr);
        if (status < 0) {
            qCritical() << "Error: Cannot compute the Cholesky decomposition of the perturbed correlation matrix.";
            return -1;
        }



        for (int j = 0; j < numRVs; ++j){
            for (int k = 0; k < numRVs; ++k){
                (*dLdMean)[j][k] = ((*L)[j][k]-(*Loriginal)[j][k])/h;
            }
        }

        // Restore the mean
        randomVariable->setMeanValue(mean);



        // Perturb R0 for stdv sensitivity analysis and compute dLdStdv

        // Perturb the stdv
        randomVariable->setStandardDeviationValue(stdv + h);

        // Establish the perturbed R0
        status = establishCorrelationMatrix(R0.get(), randomVariable);
        if (status < 0) {
            qCritical() << "Error: Cannot establish the perturbed correlation matrix.";
            return -1;
        }

        // Compute the perturbed L
        status = theCholeskyDecomposition->getCholesky(R0.get(), L.get(), nullptr);
        if (status < 0) {
            qCritical() << "Error: Cannot compute the Cholesky decomposition of the perturbed correlation matrix.";
            return -1;
        }
        for (int j = 0; j < numRVs; ++j){
            for (int k = 0; k < numRVs; ++k){
                (*dLdStdv)[j][k] = ((*L)[j][k]-(*Loriginal)[j][k])/h;
            }
        }

        // Restore the stdv
        randomVariable->setStandardDeviationValue(stdv);


        // Restore L and R0

        for (int j = 0; j < numRVs; ++j){
            for (int k = 0; k < numRVs; ++k){
                (*L)[j][k] = (*Loriginal)[j][k];
            }
        }

        for (int j = 0; j < numRVs; ++j){
            for (int k = 0; k < numRVs; ++k){
                (*R0)[j][k] = (*R0original)[j][k];
            }
        }



        // Now, we need to compute (-L^-1 * dL/dMean * L^-1 * z) and (-L^-1 * dL/dStdv * L^-1 * z). We do this step by step using GSL:

        // Compute temp = L^-1 * z


//        for (int ii = 0; ii<z->size(); ++ii) {
//            (*tempVector)[ii] = z->at(ii);
//        }

        std::unique_ptr<QVector<double>> tempVector  = std::make_unique<QVector<double>>(*z);

        status = theMatrixOperations.multiplyMatrixVector(false, 1.0, Linv.get(), tempVector.get(), 0.0, tempVector.get());



        // Compute meanTempVector = dLdMean * L^-1 * z = dLdMean * tempVector

//        for (int ii = 0; ii <tempVector->size(); ++ii) {
//            (*meanTempVector)[ii] = tempVector->at(ii);
//        }

        std::unique_ptr<QVector<double>> meanTempVector = std::make_unique<QVector<double>>(*tempVector);


        status = theMatrixOperations.multiplyMatrixVector(false, 1.0, dLdMean.get(), meanTempVector.get(), 0.0, meanTempVector.get());


        // Compute meanTempVector = L^-1 * dLdMean * L^-1 * z = L^-1 * meanTempVector

        status = theMatrixOperations.multiplyMatrixVector(false, 1.0, Linv.get(), meanTempVector.get(), 0.0, meanTempVector.get());


        // Compute meanTempVector = -L^-1 * dLdMean * L^-1 * z = -meanTempVector

        for (int ii = 0; ii< meanTempVector->size(); ++ii) {
            (*meanTempVector)[ii] = -meanTempVector->at(ii);
        }


        // Compute stdvTempVector = dLdStdv * L^-1 * z = dLdStdv * tempVector
//        for (int ii = 0; ii <tempVector->size(); ++ii) {
//            (*stdvTempVector)[ii] = tempVector->at(ii);
//        }

        std::unique_ptr<QVector<double>> stdvTempVector = std::make_unique<QVector<double>>(*tempVector);


        status = theMatrixOperations.multiplyMatrixVector(false, 1.0, dLdStdv.get(), stdvTempVector.get(), 0.0, stdvTempVector.get());


        // Compute stdvTempVector = L^-1 * dLdStdv * L^-1 * z = L^-1 * stdvTempVector

        status = theMatrixOperations.multiplyMatrixVector(false, 1.0, Linv.get(), stdvTempVector.get(), 0.0, stdvTempVector.get());


        // Compute stdvTempVector = -L^-1 * dLdStdv * L^-1 * z = -stdvTempVector
        for (int j=0; j<randomVariablesCount; ++j) {
            (*meanTempVector)[j] = -meanTempVector->at(j);
        }

        for (int j=0; j< stdvTempVector->size(); ++j) {
            (*stdvTempVector)[j] = -stdvTempVector->at(j);
        }



        // Now, the vectors that are obtained from the first and second part are added. This will be the sensitivity with respect to the random variable i. This will form a row of the Jacobian matrix. Note that the naming does not correspond to what the vector really is at this stage.
        // meanTempVector = meanTempVector + dzdMeanVector

        for (int j = 0; j < numRVs; ++j){

            (*meanTempVector)[j] += (*dzdMeanVector)[j];

        }

        // stdvTempVector = stdvTempVector + dzdStdvVector


        for (int j = 0; j < numRVs; ++j){

            (*stdvTempVector)[j] += (*dzdStdvVector)[j];

        }

        // Setting the ith row of the Jacobian matrix
        for (int j = 0; j < dy_dMean[0][0].size() ; ++j){
            (*dy_dMean)[i][j]=(*meanTempVector)[j];
            (*dy_dStdv)[i][j]=(*stdvTempVector)[j];
        }
    }

    // Freeing the matrices and vectors


    return 0;
}


// **************************************************************************************
// **************************************************************************************
// ************* ORIGINAL:

/*

#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_integration.h>

int RNatafProbabilityTransformation::establishCorrelationMatrix(QVector< QVector<double> > *R0, RRandomVariable *perturbedRV)
{
    // Set the correlation coefficients that the user has given
    QList<RCorrelation *> correlationList = theDomain->findChildren<RCorrelation *>();
    RRandomVariable *rv1;
    RRandomVariable *rv2;
    int index1;
    int index2;
    double rho;
    double rho0;
    for (int i = 0; i < correlationList.count(); ++i) {

        // Get the two random variable objects
        rv1 = qobject_cast<RRandomVariable *>(correlationList[i]->getRandomVariable1());
        rv2 = qobject_cast<RRandomVariable *>(correlationList[i]->getRandomVariable2());


        // If this correlation matrix is being established as a result of a perturbation for a sensitivity analysis, then check if one of rv1 or rv2 is the perturbed one; otherwise, conitue.
        if (perturbedRV) {
            if (rv1 != perturbedRV && rv2 != perturbedRV) {
                continue;
            }
        }



        // Do nothing if one or both of the random variables are not subject to this analysis
        if ((!rv1) || (!rv2)) {
            qCritical() << "Warning: Two random variables must be given to the correlation object" << correlationList[i]->objectName() << ".";
            continue;
        }


        // Do nothing if the random variable wasn't found in the random variable list
        index1 = randomVariableList->indexOf(rv1);
        index2 = randomVariableList->indexOf(rv2);
        if ((index1 < 0) || (index2 < 0)) {
            continue;
        }


        // Get the correlation value
        rho = correlationList[i]->getCoefficient();


        // Modify the correlation coefficient
        rho0 = modifyCorrelation(rv1, rv2, rho);


        // Print the correlation matrix if the user asked for it
        if (theOutputLevel >= RObject::Medium) {
            qDebug() << "The correlation coefficient" << correlationList[i]->objectName() << "is changed from" << rho << "to" << rho0;
        }


        // Set the correlation coefficient into the matrix
        (*R0)[index1][index2] = rho0;
        (*R0)[index2][index1] = rho0;
    }

    return 0;
}

double RNatafProbabilityTransformation::phi2(double z_i, double z_j, double rho)
{
    double par = z_i*z_i + z_j*z_j - 2.0*rho*z_i*z_j;
    double theExp = exp(-par/(2.0*(1.0-rho*rho)));
    double result = theExp/(2.0*M_PI*sqrt(1.0-rho*rho));

    return result;
}

double RNatafProbabilityTransformation::integrand(RRandomVariable *rv_i, double z_i, double mean_i, double stdv_i, RRandomVariable *rv_j, double z_j, double mean_j, double stdv_j, double rho_zero, bool evaluateDerivative)
{
    double x_i = rv_i->getInverseCDFValue(gsl_cdf_gaussian_P(z_i, 1.0));
    double x_j = rv_j->getInverseCDFValue(gsl_cdf_gaussian_P(z_j, 1.0));


    double thePhi2 = phi2(z_i, z_j, rho_zero);

    double result;
    if (evaluateDerivative) {
        double factor = rho_zero/(2*M_PI*(1.0-rho_zero*rho_zero)) + (z_i*z_j*(1.0-rho_zero*rho_zero) - (rho_zero)*( z_i*z_i - 2*rho_zero*z_i*z_j+z_j*z_j ) ) / ((1.0-rho_zero*rho_zero)*(1.0-rho_zero*rho_zero));
        result = ( (x_i-mean_i)/stdv_i * (x_j-mean_j)/stdv_j * thePhi2 * factor);
    }
    else {
        result = ( (x_i-mean_i)/stdv_i * (x_j-mean_j)/stdv_j * thePhi2 );
    }

    return result;
}

double RNatafProbabilityTransformation::doubleIntegral(RRandomVariable *rv_i, double mean_i, double stdv_i, RRandomVariable *rv_j, double mean_j, double stdv_j, double rho, bool evaluateDerivative)
{
    // The grid of integration points:
    // 1, 2, ..., i, ..., 2*n in x-direction with intervals h
    // 1, 2, ..., j, ..., 2*m in y-direction with intervals k


    // Selected integration boundaries
    double bound = 5.6;
    double z_i0 = -bound;
    double z_in = bound;
    double z_j0 = -bound;
    double z_jm = bound;

    // Half the number of integration points
    int numPoints = 25;
    int n = numPoints;
    int m = numPoints;

    // Integration point intervals
    double h = (z_in-z_i0)/(2.0*n);
    double k = (z_jm-z_j0)/(2.0*m);

    // Grid of integration points
    QVector<double> z_i;
    QVector<double> z_j;
    for (int i = 0; i<=2*n; ++i) {
        z_i << z_i0 + i*h;
    }
    for (int j=0; j<=2*m; ++j) {
        z_j << z_j0 + j*k;
    }

    // Computing sums (naming terms according to p. 126 in "Numerical Methods" by Faires & Burden)
    double term1 = 0.0;
    double term2 = 0.0;
    double term3 = 0.0;
    double term4 = 0.0;
    double term5 = 0.0;
    double term6 = 0.0;
    double term7 = 0.0;
    double term8 = 0.0;
    double term9 = 0.0;
    double term10 = 0.0;
    double term11 = 0.0;
    double term12 = 0.0;
    double term13 = 0.0;
    double term14 = 0.0;
    double term15 = 0.0;
    double term16 = 0.0;

    term1 = integrand( rv_i, z_i[0] ,    mean_i, stdv_i, rv_j,   z_j[0] ,     mean_j, stdv_j, rho, evaluateDerivative);
    term4 = integrand( rv_i, z_i[2*n] ,  mean_i, stdv_i, rv_j,   z_j[0] ,     mean_j, stdv_j, rho, evaluateDerivative);
    term13 = integrand(rv_i, z_i[0] ,    mean_i, stdv_i, rv_j,   z_j[2*m],    mean_j, stdv_j, rho, evaluateDerivative);
    term16 = integrand(rv_i, z_i[2*n],   mean_i, stdv_i, rv_j,   z_j[2*m],    mean_j, stdv_j, rho, evaluateDerivative);

    for (int i=1; i<=n; ++i) {
        term3 += integrand(rv_i,  z_i[2*i-1],     mean_i, stdv_i, rv_j,  z_j[0],       mean_j, stdv_j, rho, evaluateDerivative);
        term15 += integrand(rv_i, z_i[2*i-1],     mean_i, stdv_i, rv_j,  z_j[2*m],     mean_j, stdv_j, rho, evaluateDerivative);
    }

    for (int i=1; i<=n-1; ++i) {
        term2 += integrand(rv_i,  z_i[2*i],       mean_i, stdv_i, rv_j,  z_j[0],       mean_j, stdv_j, rho, evaluateDerivative);
        term14 += integrand(rv_i, z_i[2*i],       mean_i, stdv_i, rv_j,  z_j[2*m],     mean_j, stdv_j, rho, evaluateDerivative);
    }

    for (int j=1; j<=m-1; ++j) {
        term5 += integrand(rv_i,   z_i[0],    mean_i, stdv_i, rv_j,   z_j[2*j],   mean_j, stdv_j, rho, evaluateDerivative);
        term8 += integrand(rv_i,   z_i[2*n],  mean_i, stdv_i, rv_j,   z_j[2*j],   mean_j, stdv_j, rho, evaluateDerivative);
    }

    for (int j=1; j<=m; ++j) {
        term9 += integrand(rv_i,   z_i[0],   mean_i, stdv_i, rv_j,   z_j[2*j-1],   mean_j, stdv_j, rho, evaluateDerivative);
        term12 += integrand(rv_i,  z_i[2*n], mean_i, stdv_i, rv_j,   z_j[2*j-1],   mean_j, stdv_j, rho, evaluateDerivative);
    }

    for (int j=1; j<=(m-1); ++j) {
        for (int i=1; i<=(n-1); ++i) {
            term6 += integrand(rv_i, z_i[2*i], mean_i, stdv_i, rv_j, z_j[2*j], mean_j, stdv_j, rho, evaluateDerivative);
        }
    }

    for (int j=1; j<=(m-1); ++j) {
        for (int i=1; i<=(n); ++i) {
            term7 += integrand(rv_i, z_i[2*i-1], mean_i, stdv_i, rv_j, z_j[2*j], mean_j, stdv_j, rho, evaluateDerivative);
        }
    }
    for (int j=1; j<=(m); ++j) {
        for (int i=1; i<=(n-1); ++i) {
            term10 += integrand(rv_i, z_i[2*i], mean_i, stdv_i, rv_j, z_j[2*j-1], mean_j, stdv_j, rho, evaluateDerivative);
        }
    }
    for (int j=1; j<=(m); ++j) {
        for (int i=1; i<=(n); ++i) {
            term11 += integrand(rv_i, z_i[2*i-1], mean_i, stdv_i, rv_j, z_j[2*j-1], mean_j, stdv_j, rho, evaluateDerivative);
        }
    }


    double par1 = term1 + 2.0*term2 + 4.0*term3 + term4;
    double par2 = term5 + 2.0*term6 + 4.0*term7 + term8;
    double par3 = term9 + 2.0*term10 + 4.0*term11 + term12;
    double par4 = term13 + 2.0*term14 + 4.0*term15 + term16;
    double result = h*k/9.0 * (par1 + 2.0*par2 + 4.0*par3 + par4);

    return result;
}

void RNatafProbabilityTransformation::residualFunction(double rho_original, double rho, RRandomVariable *rv_i, double mean_i, double stdv_i, RRandomVariable *rv_j, double mean_j, double stdv_j, double *f, double *df)
{
    *f = rho_original - doubleIntegral(rv_i, mean_i, stdv_i, rv_j, mean_j, stdv_j, rho, false);
    *df = -doubleIntegral(rv_i, mean_i, stdv_i, rv_j, mean_j, stdv_j, rho, true);

    return;
}

double RNatafProbabilityTransformation::modifyCorrelation(RRandomVariable *rv_i, RRandomVariable *rv_j, double rho_original)
{
    double mean_i = rv_i->getMeanValue();
    double mean_j = rv_j->getMeanValue();

    double stdv_i = rv_i->getStandardDeviationValue();
    double stdv_j = rv_j->getStandardDeviationValue();

    double result = 0.0;

    double tol = 1.0e-6;
    double pert = 1.0e-4;

    double rho_old = rho_original;
    double rho_new;
    double f = 0.0;
    double df = 0.0;
    double perturbed_f;

    for (int i = 1; i <= 100; ++i) {

        // Evaluate the function
        residualFunction(rho_original,rho_old, rv_i, mean_i, stdv_i, rv_j, mean_j, stdv_j, &f, &df);


        ///////////////////////////////////////////////////////////////////////////////////////////
        // Evaluate perturbed function
        residualFunction(rho_original, (rho_old+pert),  rv_i, mean_i, stdv_i, rv_j, mean_j, stdv_j, &perturbed_f, &df);

        // Evaluate derivative of function
        df = (perturbed_f - f) / pert;
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////


        if (qAbs(df) < 1.0e-15) {
            qCritical() << "Error: The derivative used in the Newton algorithm within the Nataf transformation is zero.";
        }
        else {

            // Take a Newton step
            rho_new = rho_old - f/df;

            QCoreApplication::processEvents();



            // Check convergence; quit or continue
            if (fabs(1.0-fabs(rho_old/rho_new)) < tol) {
                result = rho_new;
                return result;
            }
            else {
                if (i==100) {
                    qCritical() << "Error: The Newton algorithm within the Nataf transformation did not converge.";
                    result = 0.0;
                    return result;
                }
                else {
                    rho_old = rho_new;
                }

            }
        }
    }


    return result;
}


*/


// **************************************************************************************
// **************************************************************************************

