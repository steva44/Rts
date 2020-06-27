#ifndef RPGDIntensityModel_H
#define RPGDIntensityModel_H
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
#include "RContinuousRandomVariable.h"
#include "RLinearSolver.h"
#include "RIntensityModel.h"

class RParameter;
class RResponse;
class RLocation;

class RPGDIntensityModel : public RIntensityModel
{
    Q_OBJECT
    Q_PROPERTY(QString MagnitudeList READ getMagnitudeList WRITE setMagnitudeList)
    Q_PROPERTY(QString HypocentreList READ getHypocentreList WRITE setHypocentreList)
    Q_PROPERTY(QObject *PeakGroundAcceleration READ getPeakGroundAcceleration WRITE setPeakGroundAcceleration)

public:
    RPGDIntensityModel(QObject *parent, QString name);
    ~RPGDIntensityModel();

    int resetTime();

    QString getMagnitudeList();
    void setMagnitudeList(QString value);

    QString getHypocentreList();
    void setHypocentreList(QString value);

    QObject *getPeakGroundAcceleration() const;
    void setPeakGroundAcceleration(QObject *value);

    double getProbabilityOfLiquefaction();

    double getLateralPGD();

    void setSoilSusceptibilityLevel(QString name);

    void getLandslideSuceptibility(QString geologicalType, bool dry, double angle);

    double getLandslidePGD();

    bool isOn();
    int evaluateModel(RGradientType theGradientType);

private:
    // Liquefaction parameters
    double theLiquefactionSusceptibility;
    double theProbabilityOfLiquefaction;
    double theVerticalPGD;
    double theLateralPGD;
    double theExpectedPGD;

    // landslide parameters
    int theLandslideSusceptibility;
    double cprime;
    double phiprime;
    double minimumSlopeAngle;
    double minimumCriticalAcceleration;

    QList<QPointer<RParameter> > theMagnitudeList;
    QList<QPointer<RParameter> > theHypocentreList;
    QPointer<RParameter> thePeakGroundAcceleration;
    QString theSoilSusceptibilityLevel;

    RResponse *theResponse;
};

#endif
