#ifndef RBoundedExponentialMagnitudeModel_H
#define RBoundedExponentialMagnitudeModel_H
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
*                                                                    *
*********************************************************************/

#include "RModel.h"

class RParameter;
class RResponse;
class RContinuousRandomVariable;

class RBoundedExponentialMagnitudeModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *MinimumMagnitude READ getMinimumMagnitude WRITE setMinimumMagnitude)
    Q_PROPERTY(QObject *MaximumMagnitude READ getMaximumMagnitude WRITE setMaximumMagnitude)
    Q_PROPERTY(QObject *Beta READ getBeta WRITE setBeta)
    Q_PROPERTY(QObject *Occurrence READ getOccurrence WRITE setOccurrence)

public:
    RBoundedExponentialMagnitudeModel(QObject *parent, QString name);
    ~RBoundedExponentialMagnitudeModel();

    QObject *getMinimumMagnitude() const;
    void setMinimumMagnitude(QObject *value);

    QObject *getMaximumMagnitude() const;
    void setMaximumMagnitude(QObject *value);

    QObject *getBeta() const;
    void setBeta(QObject *value);

    QObject *getOccurrence() const;
    void setOccurrence(QObject *value);

    QList<RParameter *> getPhysicalParameterList();

    int evaluateModel(RGradientType theGradientType);

private:
    QPointer<RParameter> theMinimumMagnitude;
    QPointer<RParameter> theMaximumMagnitude;
    QPointer<RParameter> theBeta;
    QPointer<RContinuousRandomVariable> theTheta;
    QPointer<RParameter> theOccurrence;

    RResponse *theMagnitude;
};

#endif
