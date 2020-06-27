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
*                                                                    *
*********************************************************************/

#include "RIndependenceProbabilityTransformation.h"
#include "RDomain.h"
#include "RContinuousRandomVariable.h"
#include "RMatrixOperations.h"
#include "RProbabilityDistributions.h"

#include <math.h>

RIndependenceProbabilityTransformation::RIndependenceProbabilityTransformation(QObject *parent, QString name)
    : RProbabilityTransformation(parent, name)
{
    theProbabilityDistributions = nullptr;
}


RIndependenceProbabilityTransformation::~RIndependenceProbabilityTransformation()
{

}


QObject * RIndependenceProbabilityTransformation::getProbabilityDistributions() const
{
    return theProbabilityDistributions;
}


void RIndependenceProbabilityTransformation::setProbabilityDistributions(QObject *value)
{
    theProbabilityDistributions = qobject_cast<RProbabilityDistributions *>(value);
}


int RIndependenceProbabilityTransformation::initializeAtStartOfAnalysis()
{
    // Does nothing. There is no need for the correlation matrix here.
    return 0;
}


int RIndependenceProbabilityTransformation::transformPointToStandardNormalSpace(const QVector<double> *originalVector, QVector<double> *standardNormalVector)
{
    RContinuousRandomVariable *randomVariable;
    for (int i = 0; i < randomVariablesCount; i++){
        randomVariable = qobject_cast<RContinuousRandomVariable *>(randomVariableList->value(i));
        double xi = (*originalVector)[i];

        double CDF = randomVariable->getCDFValue(xi);
        // Get the inverse CDF value of the variable "CDF" for the standard normal distribution
        double ui = theProbabilityDistributions->getInverseNormalCDF(CDF, 1.0) + 0.0;

        (*standardNormalVector)[i] = ui;
    }


    // Print u
    if (theOutputLevel >= RObject::Medium) {
        qDebug() << "Current point in standard-normal space, u:";
        theDomain->print(standardNormalVector);
        qDebug() << "";
    }

    return 0;
}


int RIndependenceProbabilityTransformation::transformPointToOriginalSpace(const QVector<double> *standardNormalVector, QVector<double> *originalVector)
{
    RContinuousRandomVariable *randomVariable;
    for (int i = 0; i < randomVariablesCount; i++){
        randomVariable = qobject_cast<RContinuousRandomVariable *>(randomVariableList->value(i));
        double ui = (*standardNormalVector)[i];

        // Get the CDF value of zi for the standard normal distribution
        double CDF = theProbabilityDistributions->getNormalCDF(ui - 0.0, 1.0);
        double inverseCDF = randomVariable->getInverseCDFValue(CDF);
        (*originalVector)[i] = inverseCDF;
    }

    // Print X
    if (theOutputLevel == RObject::Maximum) {
        qDebug() << "Current point in original space, x:";
        theDomain->print(originalVector);
        qDebug() << "";
    }
    return 0;
}


int RIndependenceProbabilityTransformation::computeDiagonalJacobian_u_x_AsVector(const QVector<double> *originalVector, const QVector<double> *standardNormalVector, QVector<double> *jacobianV_u_x)
{
    RContinuousRandomVariable *randomVariable;
    for (int j=0; j<jacobianV_u_x->size(); j++) {
        (*jacobianV_u_x)[j] = 0.0;
    }

    for (int i = 0; i < randomVariablesCount; i++){
        randomVariable = qobject_cast<RContinuousRandomVariable *>(randomVariableList->value(i));

        double xi = (*originalVector)[i];
        double ui = (*standardNormalVector)[i];

        // Get the CDF value of ui for the standard normal distribution
        double PDF = theProbabilityDistributions->getNormalPDF(ui - 0.0, 1.0);
        if (PDF == 0.0) {
            qCritical() << "Error: The PDF value of the random variable" << randomVariableList->value(i)->objectName() << "is zero, probably due to a too large step.";
            //return -1;
        }

        // Calculate jacobian_u_x
        (*jacobianV_u_x)[i] = randomVariable->getPDFValue(xi) / PDF;
    }

    return 0;
}


int RIndependenceProbabilityTransformation::computeJacobian(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, QVector< QVector<double> > *jacobian, RProbabilityTransformation::RJacobianType jacobianType)
{

    for (int i=0; i<jacobian->size(); i++) {
        for (int j=0; j<jacobian[0][0].size(); j++) {
            (*jacobian)[i][j] = 0.0;
        }
    }

    int size = pointInOriginalSpace->size();
    QVector<double> *jacobianV_u_x = new QVector<double>(size);

    int result = computeDiagonalJacobian_u_x_AsVector(pointInOriginalSpace, pointInStandardNormalSpace, jacobianV_u_x);
    if (result < 0) {
        qCritical() << "Error: Could not compute the diagonal Jacobian matrix.";
        return -1;
    }


    for (int i = 0; i < randomVariablesCount; i++){

        double value = 0.0;

        if (jacobianType == RProbabilityTransformation::u_x) {

            value = (*jacobianV_u_x)[i];

        } else if (jacobianType == RProbabilityTransformation::x_u) {

            value = 1.0 / (*jacobianV_u_x)[i];

        }

        (*jacobian)[i][i] = value;
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

    if (jacobianV_u_x) {
        delete jacobianV_u_x;
        jacobianV_u_x = nullptr;
    }

    return 0;
}


int RIndependenceProbabilityTransformation::transformGradientToOriginalSpace(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, const QVector<double> *vectorInStandardNormalSpace, QVector<double> *vectorInOriginalSpace)
{
    int size = pointInOriginalSpace->size();
    QVector<double> *jacobianV_u_x = new QVector<double>(size);

    int result = computeDiagonalJacobian_u_x_AsVector(pointInOriginalSpace, pointInStandardNormalSpace, jacobianV_u_x);
    if (result < 0) {
        qCritical() << "Error: Could not compute the diagonal Jacobian matrix.";
        return -1;
    }

    // Compute:  vectorInOriginalSpace = jacobian_u_x^T * vectorInStandardNormalSpace
    // Since the jacobian is calculated as a vector here, a simple vector multiplication is performed:
    for (int i=0; i<vectorInOriginalSpace->size(); i++) {
        (*vectorInOriginalSpace)[i] = vectorInStandardNormalSpace->at(i) * jacobianV_u_x->at(i);
    }


    if (jacobianV_u_x) {
        delete jacobianV_u_x;
        jacobianV_u_x = nullptr;
    }
    return 0;
}


int RIndependenceProbabilityTransformation::transformGradientToStandardNormalSpace(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, const QVector<double> *vectorInOriginalSpace, QVector<double> *vectorInStandardNormalSpace)
{
    int size = pointInOriginalSpace->size();
    QVector<double> *jacobianV_u_x = new QVector<double>(size);

    int result = computeDiagonalJacobian_u_x_AsVector(pointInOriginalSpace, pointInStandardNormalSpace, jacobianV_u_x);
    if (result < 0) {
        qCritical() << "Error: Could not compute the diagonal Jacobian matrix.";
        return -1;
    }

    // Compute:  vectorInStandardNormalSpace = jacobian_x_u^T * vectorInOriginalSpace
    // Since the jacobian is calculated as a vector here, a simple vector division is performed:
    for (int i=0; i<vectorInOriginalSpace->size(); i++) {
        (*vectorInStandardNormalSpace)[i] = vectorInOriginalSpace->at(i) / jacobianV_u_x->at(i);
    }


    if (jacobianV_u_x) {
        delete jacobianV_u_x;
        jacobianV_u_x = nullptr;
    }

    return 0;
}


int RIndependenceProbabilityTransformation::computeSensitivityToDistributionParameters(const QVector<double> *pointInOriginalSpace, QVector< QVector<double> > *dy_dMean, QVector< QVector<double> > *dy_dStdv)
{
    // Create the "z-vector"
    int numRVs = pointInOriginalSpace->size();
    QVector<double> *z = new QVector<double>(numRVs);
    int status = transformPointToStandardNormalSpace(pointInOriginalSpace, z);
    if (status < 0) {
        qCritical() << "Error: Cannot transform the vector of random variables to the standard normal space.";
        return -1;
    }

    QVector<double> *dzdMeanVector = new QVector<double>(numRVs);
    QVector<double> *dzdStdvVector = new QVector<double>(numRVs);


    // Loop over all random variables. The sensitivity of the y-vector to each random variable mean or stdv will be a row of the Jacobian matrix.
    for (int i = 0; i < numRVs; i++) {
        RContinuousRandomVariable *randomVariable = qobject_cast<RContinuousRandomVariable *>(randomVariableList->value(i));
        double xi = (*pointInOriginalSpace)[i];
        double zi = (*z)[i];


        // First, we need to compute (L^-1 * dzdMean) and (L^-1 * dzdStdv)
        // Note: L=I here because there is no correlation

        // Computing dzdMean and dzdStdv
        double dzdMean = 0.0;
        double dzdStdv = 0.0;
        status = computeDzDMeanStdv(theProbabilityDistributions, randomVariable, xi, zi, &dzdMean, &dzdStdv);

        for (int j=0; j<dzdMeanVector->size(); j++) {
            (*dzdMeanVector)[j] = 0.0;
        }

        for (int j=0; j<dzdStdvVector->size(); j++) {
            (*dzdStdvVector)[j] = 0.0;
        }

        (*dzdMeanVector)[i] = dzdMean;
        (*dzdStdvVector)[i] = dzdStdv;

        // Second, we need to compute d(L^-1)/d(Mean) * z, which is equal to zero, because L=I and dL/dMean = 0

        // This will be the sensitivity with respect to the random variable i. This will form a row of the Jacobian matrix.

        // Setting the ith row of the Jacobian matrix
        for (int j=0; j<dzdMeanVector->size(); j++) {

            (*dy_dMean)[i][j] = (*dzdMeanVector)[j];
            (*dy_dStdv)[i][j] = (*dzdStdvVector)[j];

        }

    }

    // Freeing the matrices and vectors
    if (z) {
        delete z;
        z = nullptr;
    }
    if (dzdMeanVector) {
        delete dzdMeanVector;
        dzdMeanVector = nullptr;
    }
    if (dzdStdvVector) {
        delete dzdStdvVector;
        dzdStdvVector = nullptr;
    }

    return 0;
}

