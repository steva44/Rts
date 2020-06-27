#ifndef RConcreteComponent_H
#define RConcreteComponent_H
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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

// Base class for reinforced concrete components

#include "RConcreteMaterialKnowledge.h"
#include "RConfinedConcreteMaterialKnowledge.h"
#include "RSteelMaterialKnowledge.h"

#include <QString>
#include <QObject>
#include <QDebug>
#include <QPointer>
#include <memory>

class RParameter;

class RConcreteComponent
{
    Q_PROPERTY(QString ConcreteStrength READ getConcreteStrength WRITE setConcreteStrength)
    Q_PROPERTY(QObject* LongitudinalReinforcementRatio READ getLongitudinalReinforcementRatio WRITE setLongitudinalReinforcementRatio)
    Q_PROPERTY(QObject* TransverseReinforcementRatio READ getTransverseReinforcementRatio WRITE setTransverseReinforcementRatio)
    Q_PROPERTY(QObject* ConcreteCover READ getConcreteCover WRITE setConcreteCover)
    Q_PROPERTY(QString LongitudinalRebarDesignation READ getLongitudinalRebarDesignation WRITE setLongitudinalRebarDesignation)
    Q_PROPERTY(QString TransverseRebarDesignation READ getTransverseRebarDesignation WRITE setTransverseRebarDesignation)

public:
    RConcreteComponent();
    ~RConcreteComponent();

    int  setConcreteStrength(const QString& value);

    void setConcreteCover(QObject *value);

    void setLongitudinalRebarDesignation(const QString &value);

    void setTransverseRebarDesignation(const QString &value);

    void setLongitudinalReinforcementRatio(QObject *value);

    void setTransverseReinforcementRatio(QObject *value);

    void setLongitudinalReinforcementSpacing(double value);

    void setTransverseReinforcementSpacing(double value);

    double getLongitudinalReinforcementDiameter(void);

    double getTransverseReinforcementDiameter(void);

    double getLongitudinalReinforcementSpacing() const;

    double getTransverseReinforcementSpacing() const;

    QString getLongitudinalRebarDesignation() const;

    QString getTransverseRebarDesignation() const;

    double getRebarDiameter(const QString& rebarDesignation) const;

    double getRebarArea(const QString& rebarDesignation) const;

    double getConcreteStrengthValue() const;

    QString getConcreteStrength();

    RParameter* getConcreteCoverThickness() const;

    RParameter* getTheLongitudinalReinforcementRatioParameter() const;

    RParameter* getTheTransverseReinforcementRatioParameter() const;

    RConcreteMaterialKnowledge* getTheConcreteMaterialKnowledge() const;

    RConcreteMaterialKnowledge* getTheConfinedConcreteMaterialKnowledge() const;

    RSteelMaterialKnowledge* getTheReinforcementMaterialKnowledge() const;

    double getLongitudinalReinforcementAngle() const;

    void setLongitudinalReinforcementAngle(double value);

    double getTransverseReinforcementAngle() const;

    void setTransverseReinforcementAngle(double value);

    double getLongitudinalBarArea(void) const;

    double getTransBarArea(void) const;

protected:

    QString theConcreteStrength;

    QPointer<RParameter> concreteCoverThickness;
    QPointer<RParameter> theLongitudinalReinforcementRatioParameter;
    QPointer<RParameter> theTransverseReinforcementRatioParameter;

    QString longitudinalRebarDesignation;
    QString transverseRebarDesignation;

    double transverseReinforcementSpacing;
    double longitudinalReinforcementSpacing;

    double longitudinalReinforcementAngle;
    double transverseReinforcementAngle;

    // Material knowledge containing material properties
    std::unique_ptr<RConcreteMaterialKnowledge> theConcreteMaterialKnowledge;
    std::unique_ptr<RConfinedConcreteMaterialKnowledge> theConfinedConcreteMaterialKnowledge;
    std::unique_ptr<RSteelMaterialKnowledge> theReinforcementMaterialKnowledge;

};

#endif
