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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RQuasiNewtonSearchDirection.h"
#include "RMatrixOperations.h"
#include "RDomain.h"

#include <math.h>

RQuasiNewtonSearchDirection::RQuasiNewtonSearchDirection(QObject *parent, QString name)
    : RSearchDirection(parent, name)
{
    theType = "BFGS";
    oldPoint = nullptr;
    oldGradient = nullptr;
    HinvOld = nullptr;
}


RQuasiNewtonSearchDirection::~RQuasiNewtonSearchDirection()
{
    if (oldPoint) {
        delete oldPoint;
        oldPoint = nullptr;
    }
    if (oldGradient) {
        delete oldGradient;
        oldGradient = nullptr;
    }
    if (HinvOld) {
        delete HinvOld;
        HinvOld = nullptr;
    }
}


QString RQuasiNewtonSearchDirection::getType()
{
    return theType;
}


void RQuasiNewtonSearchDirection::setType(QString value)
{
    if (value.contains("BFGS")) {
        theType = QString("BFGS");
    }
    else if (value.contains("DFP")) {
        theType = QString("DFP");
    }
    else if (value.contains("Broyden")) {
        theType = QString("Broyden");
    }
    else if (value.contains("SR1")) {
        theType = QString("SR1");
    }
    else {
        qCritical() << "The type of" << this->objectName() << "is given as" << value << "must be BFGS, DFP, Broyden, or SR1";
        theType = QString("BFGS");
    }
}


int RQuasiNewtonSearchDirection::getSearchDirection(QVector<double> const *point, double function, QVector<double> const *gradient, QVector<double> *result)
{
    // Initial declarations
    RMatrixOperations theMatrixOperations(theDomain);
    int numVars = point->size();
    QVector< QVector<double> > Hinv(numVars, QVector<double>(numVars));


    // If it is the first step, initialize class variables
    if (oldPoint==nullptr) {

        if (oldGradient!=nullptr || HinvOld!=nullptr) {
            qCritical() << "Inconsistency in" << this->objectName() << ": Some objects are zero pointers, but others are not.";
        }

        oldPoint = new QVector<double>(numVars);
        oldGradient = new QVector<double>(numVars);
        HinvOld = new QVector< QVector<double> >(numVars, QVector<double>(numVars));

        // Set the old trial point to be used in the next step
        for (int i=0; i<numVars; i++) {
            (*oldPoint)[i] = (*point)[i];
        }

        // Set the old gradient to be used in the next step
        for (int i=0; i<numVars; i++) {
            (*oldGradient)[i] = (*gradient)[i];
        }

        // How to initialize the first Hessian inverse?
        for (int i=0; i<numVars; i++) {
            (*HinvOld)[i][i] = 0.001;
        }

        // Let the first search direction be determined by the "steepest descent" method
        for (int i=0; i<numVars; i++) {
            (*result)[i] = -(*gradient)[i];
        }

        // Give output if requested
        if (theOutputLevel >= RObject::Medium) {

            qDebug() << this->objectName() << "is returning the following search direction:";
            theDomain->print(result);
        }

        return 0;

    }
    else {

        // Determine deltaX = xNew - xPrevious
        QVector<double> deltaX(numVars);
        theMatrixOperations.subtractVectors(point, oldPoint, &deltaX);


        // Determine deltaGrad = gradNew - gradPrevious
        QVector<double> deltaGrad(numVars);
        theMatrixOperations.subtractVectors(gradient, oldGradient, &deltaGrad);


        // Approximate the inverse of the Hessian: Hinv = inverse[H] = inverse[Hessian]
        if (theType == QString("DFP")) {

            // Hinv = HinvOld + deltaXOuterDeltaX / deltaXDotDeltaGrad - (HinvOld * deltaGrad * deltaGrad * HinvOld) / (deltaGrad * HinvOld * deltaGrad)
            // Hinv = HinvOld +                 temp                   - (      temp2         * deltaGrad * HinvOld) / (        temp2       * deltaGrad)
            // Hinv = HinvOld +                 temp                   - (                  temp3         * HinvOld) / (             temp4             )
            // Hinv = HinvOld +                 temp                   - (                      temp5              ) / (             temp4             )
            // Hinv = HinvOld +                 temp                   -                                           temp6
            // Hinv =       temp7                                      -                                           temp6

            QVector< QVector<double> > deltaXOuterDeltaX(numVars, QVector<double>(numVars));
            theMatrixOperations.outerProduct(&deltaX, &deltaX, &deltaXOuterDeltaX);

            double deltaXDotDeltaGrad = theMatrixOperations.dotProduct(&deltaGrad, &deltaX);

            QVector< QVector<double> > temp(numVars, QVector<double>(numVars));
            theMatrixOperations.scaleMatrix(1.0/deltaXDotDeltaGrad, &deltaXOuterDeltaX, &temp);

            QVector<double> temp2(numVars);
            theMatrixOperations.multiplyMatrixVector(false, 1.0, HinvOld, &deltaGrad, 0.0, &temp2);

            QVector< QVector<double> > temp3(numVars, QVector<double>(numVars));
            theMatrixOperations.outerProduct(&temp2, &deltaGrad, &temp3);

            double temp4 = theMatrixOperations.dotProduct(&temp2, &deltaGrad);

            QVector< QVector<double> > temp5(numVars, QVector<double>(numVars));
            theMatrixOperations.multiplyMatrixMatrix(false, false, 1.0, &temp3, HinvOld, 0.0, &temp5);

            QVector< QVector<double> > temp6(numVars, QVector<double>(numVars));
            theMatrixOperations.scaleMatrix(1.0/temp4, &temp5, &temp6);

            QVector< QVector<double> > temp7(numVars, QVector<double>(numVars));
            theMatrixOperations.addMatrices(HinvOld, &temp, &temp7);

            theMatrixOperations.subtractMatrices(&temp7, &temp6, &Hinv);
        }
        else if (theType == QString("Broyden")) {

            // Hinv = HinvOld + ((deltaX - HinvOld * deltaGrad) * deltaX * HinvOld) / (deltaX * HinvOld * deltaGrad)
            // Hinv = HinvOld + ((deltaX -       temp         ) * deltaX * HinvOld) / (     temp2       * deltaGrad)
            // Hinv = HinvOld + (      temp3                    * deltaX * HinvOld) / (                temp4       )
            // Hinv = HinvOld + (                             temp5      * HinvOld) / (                temp4       )
            // Hinv = HinvOld + (                                       temp6     ) / (                temp4       )
            // Hinv = HinvOld +                                                   temp7

            QVector<double> temp(numVars);
            theMatrixOperations.multiplyMatrixVector(false, 1.0, HinvOld, &deltaGrad, 0.0, &temp);

            QVector<double> temp2(numVars);
            theMatrixOperations.multiplyMatrixVector(false, 1.0, HinvOld, &deltaX, 0.0, &temp2);

            QVector<double> temp3(numVars);
            theMatrixOperations.subtractVectors(&deltaX, &temp, &temp3);

            double temp4 = theMatrixOperations.dotProduct(&temp2, &deltaGrad);

            QVector< QVector<double> > temp5(numVars, QVector<double>(numVars));
            theMatrixOperations.outerProduct(&temp3, &deltaX, &temp5);

            QVector< QVector<double> > temp6(numVars, QVector<double>(numVars));
            theMatrixOperations.multiplyMatrixMatrix(false, false, 1.0, &temp5, HinvOld, 0.0, &temp6);

            QVector< QVector<double> > temp7(numVars, QVector<double>(numVars));
            theMatrixOperations.scaleMatrix(1.0/temp4, &temp6, &temp7);

            theMatrixOperations.addMatrices(HinvOld, &temp7, &Hinv);
        }
        else if (theType == QString("SR1")) {

            // Hinv = HinvOld + ((deltaX - HinvOld * deltaGrad) * (deltaX - HinvOld * deltaGrad)) / ((deltaX - HinvOld * deltaGrad) * deltaGrad)
            // Hinv = HinvOld + ((deltaX -       temp         ) * (deltaX -       temp         )) / ((deltaX -       temp         ) * deltaGrad)
            // Hinv = HinvOld + (       temp2                   *        temp2                  ) / (      temp2                    * deltaGrad)
            // Hinv = HinvOld +                               temp3                               /                               temp4
            // Hinv = HinvOld +                                                                 temp5

            QVector<double> temp(numVars);
            theMatrixOperations.multiplyMatrixVector(false, 1.0, HinvOld, &deltaGrad, 0.0, &temp);

            QVector<double> temp2(numVars);
            theMatrixOperations.subtractVectors(&deltaX, &temp, &temp2);

            QVector< QVector<double> > temp3(numVars, QVector<double>(numVars));
            theMatrixOperations.outerProduct(&temp2, &temp2, &temp3);

            double temp4 = theMatrixOperations.dotProduct(&temp2, &deltaGrad);

            QVector< QVector<double> > temp5(numVars, QVector<double>(numVars));
            theMatrixOperations.scaleMatrix(1.0/temp4, &temp3, &temp5);

            theMatrixOperations.addMatrices(HinvOld, &temp5, &Hinv);
        }
        else if (theType == QString("BFGS")) {

            // Hinv = (Eye - deltaXOuterDeltaGrad / deltaXDotDeltaGrad) * HinvOld * (Eye - deltaXOuterDeltaGrad / deltaXDotDeltaGrad) + deltaXOuterDeltaX / deltaXDotDeltaGrad
            // Hinv = (Eye -                   temp                   ) * HinvOld * (Eye -                    temp                  ) +                temp2
            // Hinv = (  temp3                                        ) * HinvOld * (  temp3                                        ) +                temp2
            // Hinv =                                                  temp4      * (  temp3                                        ) +                temp2
            // Hinv =                                                           temp5                                                 +                temp2

            QVector< QVector<double> > Eye(numVars, QVector<double>(numVars));
            for (int i=0; i<numVars; i++) {
                Eye[i][i] = 1.0;
            }

            QVector< QVector<double> > deltaXOuterDeltaGrad(numVars, QVector<double>(numVars));
            theMatrixOperations.outerProduct(&deltaGrad, &deltaX, &deltaXOuterDeltaGrad);

            double deltaXDotDeltaGrad = theMatrixOperations.dotProduct(&deltaGrad, &deltaX);

            QVector< QVector<double> > deltaXOuterDeltaX(numVars, QVector<double>(numVars));
            theMatrixOperations.outerProduct(&deltaX, &deltaX, &deltaXOuterDeltaX);

            QVector< QVector<double> > temp(numVars, QVector<double>(numVars));
            theMatrixOperations.scaleMatrix(1.0/deltaXDotDeltaGrad, &deltaXOuterDeltaGrad, &temp);

            QVector< QVector<double> > temp2(numVars, QVector<double>(numVars));
            theMatrixOperations.scaleMatrix(1.0/deltaXDotDeltaGrad, &deltaXOuterDeltaX, &temp2);

            QVector< QVector<double> > temp3(numVars, QVector<double>(numVars));
            theMatrixOperations.subtractMatrices(&Eye, &temp, &temp3);

            QVector< QVector<double> > temp4(numVars, QVector<double>(numVars));
            theMatrixOperations.multiplyMatrixMatrix(false, false, 1.0, &temp3, HinvOld, 0.0, &temp4);

            QVector< QVector<double> > temp5(numVars, QVector<double>(numVars));
            theMatrixOperations.multiplyMatrixMatrix(false, false, 1.0, &temp4, &temp3, 0.0, &temp5);

            theMatrixOperations.addMatrices(&temp5, &temp2, &Hinv);
        }
        else {

            qCritical() << this->objectName() << "encountered the invalid type" << theType;
        }
    }


    // Calculate the search direction -Hinv*grad
    theMatrixOperations.multiplyMatrixVector(false, -1.0, &Hinv, gradient, 0.0, result);


    // Commit history variables
    for (int i=0; i<numVars; i++) {

        (*oldPoint)[i] = (*point)[i];

        (*oldGradient)[i] = (*gradient)[i];

        for (int j=0; j<numVars; j++) {
            (*HinvOld)[i][j] = Hinv[i][j];
        }
    }


    // Give output if requested
    if (theOutputLevel >= RObject::Medium) {

        qDebug() << this->objectName() << "is returning the following search direction:";
        theDomain->print(result);
    }


    return 0;
}


