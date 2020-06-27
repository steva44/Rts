#ifndef RStevesHumanImpactModel_H
#define RStevesHumanImpactModel_H
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
*   - Rodrigo Costa                                                  *
*                                                                    *
*********************************************************************/

// This model calculates the cost of injuries and deaths from earthquakes

#include "RModel.h"
#include "RParameter.h"

class RResponse;
class RBIM;
class RStatistics;

class RStevesHumanImpactModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
    Q_PROPERTY(QObject *InputParameter READ getInputParameter WRITE setInputParameter)

public:
    RStevesHumanImpactModel(QObject *parent, QString name);
    ~RStevesHumanImpactModel();

    int resetTime();

    // The building information model
    QObject *getBIM() const;
    void setBIM(QObject *value);

    // The input parameter is from a structural analysis model
    QObject *getInputParameter() const;
    void setInputParameter(QObject *value);

    int evaluateModel(RGradientType theGradientType);

private:

    // Returns the damage probabilities given drift, medians, and dispersions
    QVector<double> getDamageProbabilities(const QVector<double>& medians,const QVector<double>& dispersions, const double& drift);

    // Fragility function medians, and dispersions
    int getFragilityFunctionMediansAndDispersions(const QString& HazusType, QVector<double>& medians, QVector<double>& dispersions);

    // Calcualte the morbidity - probability of being in a casualty severity level multiplied by an injury and fatality factor
    QVector<double> calculateMorbidity(QString buildingType, double timeOfDay, QString economicWelfare, QVector<double> damageStateProbabilities, QVector<double> theAgeDistributionXVector, QVector<double> theAgeDistributionYVector, double thePercentageOfMales);

    QPointer<RBIM> theBIM;
    QPointer<RParameter> theInputModelResponse;
    RResponse *theResponse;
    RStatistics* theStatsMethod;

    double runningSum;

};

#endif
