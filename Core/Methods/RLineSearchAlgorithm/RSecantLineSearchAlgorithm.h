#ifndef RSecantLineSearchAlgorithm_H
#define RSecantLineSearchAlgorithm_H
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

#include "RLineSearchAlgorithm.h"

#include "qcustomplot.h"

class RSecantLineSearchAlgorithm : public RLineSearchAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double LowerBound READ getLowerBound WRITE setLowerBound)
    Q_PROPERTY(double UpperBound READ getUpperBound WRITE setUpperBound)
    Q_PROPERTY(double Tolerance READ getTolerance WRITE setTolerance)
    Q_PROPERTY(int MaxIterations READ getMaxIterations WRITE setMaxIterations)
    Q_PROPERTY(double Delay READ getDelay WRITE setDelay)

public:
    RSecantLineSearchAlgorithm(QObject *parent, QString name);
    ~RSecantLineSearchAlgorithm();

    double getLowerBound() const;
    void setLowerBound(double value);

    double getUpperBound() const;
    void setUpperBound(double value);

    double getTolerance() const;
    void setTolerance(double value);

    int getMaxIterations() const;
    void setMaxIterations(int value);

    double getDelay() const;
    void setDelay(double value);

    double findPoint(RLineSearchCallBackAssistant *theCallBackAssistant);

private:

    double theLowerBound;
    double theUpperBound;
    double theTolerance;
    int theMaxIterations;
    double theDelay;
    QPointer<QCustomPlot> customPlot;
};

#endif
