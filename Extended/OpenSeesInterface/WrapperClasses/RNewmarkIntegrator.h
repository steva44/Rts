#ifndef RNewmarkIntegrator_H
#define RNewmarkIntegrator_H
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

// Wrapper class for OpenSees Newmark
// Stevan Gavrilovic 12/2018

#include "RIntegrator.h"

class Integrator;
class Newmark;

class RNewmarkIntegrator : public RIntegrator
{
    Q_OBJECT

    Q_PROPERTY(double Gamma    READ getGamma   WRITE setGamma)
    Q_PROPERTY(double Beta     READ getBeta    WRITE setBeta)

public:
    RNewmarkIntegrator(QObject *parent, QString name);
    ~RNewmarkIntegrator();

    double getGamma() const;
    void setGamma(double value);

    double getBeta() const;
    void setBeta(double value);

    Integrator* getTheIntegrator();

    int getMaxSteps();

protected:

    std::unique_ptr<Newmark> theIntegrator;

    double gamma;
    double beta;

};

#endif
