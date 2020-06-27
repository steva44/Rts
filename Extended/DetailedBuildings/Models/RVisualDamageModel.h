#ifndef RVisualDamageModel_H
#define RVisualDamageModel_H
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

// This class calculates the visual damage on building components

#include "RModel.h"

class RBIM;
class RResponse;
class RParameter;
class RContinuousRandomVariable;
class RShellSectionDamageRegion;

class RVisualDamageModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *InputParameter READ getInputParameter WRITE setInputParameter)
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)

public:
    RVisualDamageModel(QObject *parent, QString name);
    ~RVisualDamageModel() override;

    // The input parameter is a structural analysis model
    QObject *getInputParameter() const;
    void setInputParameter(QObject *value);

    // Get and set the building information model
    QObject *getBIM() const;
    void setBIM(QObject *value);

    int evaluateModel(RGradientType theGradientType) override;

    int resetTime() override;

private:

    QPointer<RParameter> theInputParameter;

    void createDatabaseOfRandomVariables();

    // The building information model
    RBIM* theBIM;

    // Dummy response so that the model evaluates
    RResponse* theResponse;

    // The crack width that requires repair
    RContinuousRandomVariable* theCrackWidthRequiresRepair;

    // The crack length that requires repair
    RContinuousRandomVariable* theCrackLengthRequiresRepair;

    // Beta factor accounting for strain gradient:
    double getColumnCrackWidth(const double& beta, const double& de, const double& epsSteel, const double& Aeff);


    // Calculates the damage of a reinforced concrete shell element (slab, wall, etc.) according to the modified compression field theory
    int getRCShellDamage(RShellSectionDamageRegion* shellDamageRegion, double& spallingLimit,
                         double& coverLimit, double& crushingLimit, double& bucklingLimit,double& fractureLimit,
                         double& shellThickness, double& coverThickness, double& longReinfDiameter, double& transReinfDiameter,
                         double& longReinfSpacing,double& transReinfSpacing,double& areaSx,double& areaSy,
                         size_t faceIndex1, size_t faceIndex2);

    // Reinforced concrete shell cracking parameters in a vector of size 3 where:
    // Crack Width [0]
    // Crack Spacing [1]
    // Crack Orientation (theta) [2]
    std::vector<double> getShellCrackParameters(const double& epsilonxx, const double& epsilonyy, const double& gammaxy, const double& reinfDiamaterxx, const double& reinfDiamateryy,
                                                const double& reinfSpacingxx, const double& reinfSpacingyy, const double& areaSx, const double& areaSy, const double& coverThickness);
};

#endif
