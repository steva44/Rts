#ifndef RLoadControlIntegrator_H
#define RLoadControlIntegrator_H
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

// Wrapper class for OpenSees LoadControl
// Stevan Gavrilovic 12/2018

#include "RIntegrator.h"

class Integrator;
class LoadControl;

class RLoadControlIntegrator : public RIntegrator
{
    Q_OBJECT
    Q_PROPERTY(double DeltaLambda   READ getDeltaLambda  WRITE setDeltaLambda)
    Q_PROPERTY(int NumberIncrements READ getNumIncr      WRITE setNumIncr)
    Q_PROPERTY(double MinLambda     READ getMinLambda    WRITE setMinLambda)
    Q_PROPERTY(double Maxlambda     READ getMaxlambda    WRITE setMaxlambda)

public:
    RLoadControlIntegrator(QObject *parent, QString name);
    ~RLoadControlIntegrator();

    double getDeltaLambda() const;
    void setDeltaLambda(double value);

    int getNumIncr() const;
    void setNumIncr(int value);

    double getMinLambda() const;
    void setMinLambda(double value);

    double getMaxlambda() const;
    void setMaxlambda(double value);

    Integrator* getTheIntegrator();

    int getMaxSteps();

protected:

    std::unique_ptr<LoadControl> theIntegrator;

    double deltaLambda;
    int numIncrements;
    std::unique_ptr<double> minLambda;
    std::unique_ptr<double> maxlambda;

};

#endif
