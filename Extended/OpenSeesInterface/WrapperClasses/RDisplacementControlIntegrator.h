#ifndef RDisplacementControlIntegrator_H
#define RDisplacementControlIntegrator_H
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

// Wrapper class for OpenSees DisplacementControl
// Stevan Gavrilovic 12/2018

// node	node whose response controls solution
// dof	degree of freedom at the node, valid options: 1 through ndf at node.
// incr	first displacement increment <math>\Delta U_{\text{dof}}</math>
// numIter	the number of iterations the user would like to occur in the solution algorithm. Optional, default = 1.0.
// <math>\Delta U \text{min}</math>	the min stepsize the user will allow. optional, defualt = <math>\Delta U_{min} = \Delta U_0</math>
// <math>\Delta U \text{max}</math>	the max stepsize the user will allow. optional, default = <math>\Delta U_{max} = \Delta U_0</math>

#include "RIntegrator.h"

class Integrator;
class DisplacementControl;
class RPoint;

class RDisplacementControlIntegrator : public RIntegrator
{
    Q_OBJECT
    Q_PROPERTY(QObject *Point READ getPoint WRITE setPoint)
    Q_PROPERTY(int dof READ getDof WRITE setDof)
    Q_PROPERTY(double DisplacementIncrement READ getDispIncr WRITE setDispIncr)
    Q_PROPERTY(int NumberIncrements READ getNumIncr WRITE setNumIncr)
    Q_PROPERTY(double MinDelta READ getMinDelta WRITE setMinDelta)
    Q_PROPERTY(double MaxDelta READ getMaxDelta WRITE setMaxDelta)

public:
    RDisplacementControlIntegrator(QObject *parent, QString name);
    ~RDisplacementControlIntegrator();

    void reset();

    QObject *getPoint() const;
    void setPoint(QObject *value);

    double getDispIncr() const;
    void setDispIncr(double value);

    int getNumIncr() const;
    void setNumIncr(int value);

    int getDof() const;
    void setDof(int value);

    double getMinDelta() const;
    void setMinDelta(double value);

    double getMaxDelta() const;
    void setMaxDelta(double value);

    Integrator* getTheIntegrator();

    int getMaxSteps();

protected:

    std::unique_ptr<DisplacementControl> theIntegrator;

    RPoint* thePoint;
    double dispIncrement;
    int numIncrements;
    int dof;
    double minDelta;
    double maxDelta;

};

#endif
