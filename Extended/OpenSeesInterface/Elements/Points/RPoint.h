#ifndef RPoint_H
#define RPoint_H
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

#include <functional>

#include "RObject.h"
#include "RParameter.h"
#include "RNode.h"
#include <QPointer>

class RNode;
class RParameter;

class RPoint : public RObject
{
    Q_OBJECT

    Q_PROPERTY(QString XCoordinate READ getXCoordinate WRITE setXCoordinate)
    Q_PROPERTY(QString YCoordinate READ getYCoordinate WRITE setYCoordinate)
    Q_PROPERTY(QString ZCoordinate READ getZCoordinate WRITE setZCoordinate)

    Q_PROPERTY(QString XDisplacementRestraint READ getXDisplacementRestraint WRITE setXDisplacementRestraint)
    Q_PROPERTY(QString YDisplacementRestraint READ getYDisplacementRestraint WRITE setYDisplacementRestraint)
    Q_PROPERTY(QString ZDisplacementRestraint READ getZDisplacementRestraint WRITE setZDisplacementRestraint)

    Q_PROPERTY(QString XRotationRestraint READ getXRotationRestraint WRITE setXRotationRestraint)
    Q_PROPERTY(QString YRotationRestraint READ getYRotationRestraint WRITE setYRotationRestraint)
    Q_PROPERTY(QString ZRotationRestraint READ getZRotationRestraint WRITE setZRotationRestraint)

    Q_PROPERTY(QString XForce READ getXForce WRITE setXForce)
    Q_PROPERTY(QString YForce READ getYForce WRITE setYForce)
    Q_PROPERTY(QString ZForce READ getZForce WRITE setZForce)

    Q_PROPERTY(QString XMoment READ getXMoment WRITE setXMoment)
    Q_PROPERTY(QString YMoment READ getYMoment WRITE setYMoment)
    Q_PROPERTY(QString ZMoment READ getZMoment WRITE setZMoment)

    Q_PROPERTY(QObject* XMass READ getXMass WRITE setXMass)
    Q_PROPERTY(QObject* YMass READ getYMass WRITE setYMass)
    Q_PROPERTY(QObject* ZMass READ getZMass WRITE setZMass)

    Q_PROPERTY(QObject* Mass READ getMass WRITE setMass)

    Q_PROPERTY(double XDisplacement READ getXDisplacement)
    Q_PROPERTY(double YDisplacement READ getYDisplacement)
    Q_PROPERTY(double ZDisplacement READ getZDisplacement)

    Q_PROPERTY(double XRotation READ getXRotation)
    Q_PROPERTY(double YRotation READ getYRotation)
    Q_PROPERTY(double ZRotation READ getZRotation)

    Q_PROPERTY(QObject* MasterPoint  WRITE setMasterPoint READ getMasterPoint)

public:

    RPoint(QObject *parent, QString &name);
    ~RPoint();

    void setMasterPoint(QObject* value);
    inline RPoint* getMasterPoint(void){return masterPoint;}

    QString getXCoordinate();
    void setXCoordinate(QString value);
    void setXCoordinate(std::function<double(void)>& fxn);

    //Careful using this as parameterization is lost
    void setXCoordinate(double value);
    double getXCoordinateValue();
    QList<RParameter*> getXCoordinateParameters() const;

    QString getYCoordinate() ;
    void setYCoordinate(QString value);
    void setYCoordinate(std::function<double(void)>& fxn);

    //Careful using this as parameterization is lost
    void setYCoordinate(double value);
    double getYCoordinateValue();
    QList<RParameter*> getYCoordinateParameters() const;

    QString getZCoordinate();
    void setZCoordinate(QString value);
    void setZCoordinate(std::function<double(void)>& fxn);

    //Careful using this as parameterization is lost
    void setZCoordinate(double value);
    double getZCoordinateValue();
    QList<RParameter*> getZCoordinateParameters() const;

    //Gets the gradient Correction map from parametric point
    double getXCoordinateFOGradient(RParameter *theParameter);
    double getYCoordinateFOGradient(RParameter *theParameter);
    double getZCoordinateFOGradient(RParameter *theParameter);

    double getXCoordinateSOGradient(RParameter *theParameter);
    double getYCoordinateSOGradient(RParameter *theParameter);
    double getZCoordinateSOGradient(RParameter *theParameter);

    void setCoordinateVector(std::vector<double>& vecIn);

    QString getXDisplacementRestraint() const;
    void setXDisplacementRestraint(QString value);

    QString getYDisplacementRestraint() const;
    void setYDisplacementRestraint(QString value);

    QString getZDisplacementRestraint() const;
    void setZDisplacementRestraint(QString value);

    QString getXRotationRestraint() const;
    void setXRotationRestraint(QString value);

    QString getYRotationRestraint() const;
    void setYRotationRestraint(QString value);

    QString getZRotationRestraint() const;
    void setZRotationRestraint(QString value);

    QString getXForce();
    QObject *getXForceParameter() const;
    double getXForceValue();
    void setXForce(QString value);
    void setXForce(RParameter *value);
    void setXForce(std::function<double(void)> lambda);

    QString getYForce();
    QObject *getYForceParameter() const;
    double getYForceValue();
    void setYForce(QString value);
    void setYForce(RParameter *value);
    void setYForce(std::function<double(void)> lambda);

    QString getZForce();
    QObject *getZForceParameter() const;
    double getZForceValue();
    void setZForce(QString value);
    void setZForce(RParameter *value);
    void setZForce(std::function<double(void)> lambda);

    RParameter* getXMass() const;
    double getXMassValue();
    void setXMass(QObject *value);

    RParameter* getYMass() const;
    double getYMassValue();
    void setYMass(QObject *value);

    RParameter* getZMass() const;
    double getZMassValue();
    void setZMass(QObject *value);

    //These functions set the mass to ALL displacement dofs
    RParameter* getMass() const;
    double getMassValue();
    void setMass(QObject *value);

    QString getXMoment();
    QObject *getXMomentParameter() const;
    double getXMomentValue();
    void setXMoment(QString value);

    QString getYMoment();
    QObject *getYMomentParameter() const;
    double getYMomentValue();
    void setYMoment(QString value);

    QString getZMoment();
    QObject *getZMomentParameter() const;
    double getZMomentValue();
    void setZMoment(QString value);

    double getXDisplacement() const;
    double getYDisplacement() const;
    double getZDisplacement() const;
    double getXRotation() const;
    double getYRotation() const;
    double getZRotation() const;

    inline int getXDisplacementDDM() const {return 0;}
    inline int getYDisplacementDDM() const {return 1;}
    inline int getZDisplacementDDM() const {return 2;}
    inline int getXRotationDDM() const {return 3;}
    inline int getYRotationDDM() const {return 4;}
    inline int getZRotationDDM() const {return 5;}

    //When point is created, create associated node or set one that is already there
    int createOrSetAssociatedNode(RNode* theNode = nullptr);
    RNode *getAssociatedNode();
    void deleteAssociatedNode();

    bool getIsOn() const;
    void setIsOn(bool value);

private:

    bool isOn;
    QPointer<RParameter> theXCoordinateParameter  = nullptr;
    std::unique_ptr<std::function<double(void)>> xCoordinateLambda;
    double theXCoordinateDouble;

    QPointer<RParameter> theYCoordinateParameter = nullptr;
    std::unique_ptr<std::function<double(void)>> yCoordinateLambda;
    double theYCoordinateDouble;

    QPointer<RParameter> theZCoordinateParameter = nullptr;
    std::unique_ptr<std::function<double(void)>> zCoordinateLambda;
    double theZCoordinateDouble;

    bool theXDisplacementRestraint;
    bool theYDisplacementRestraint;
    bool theZDisplacementRestraint;
    bool theXRotationRestraint;
    bool theYRotationRestraint;
    bool theZRotationRestraint;

    QPointer<RParameter> theXForceParameter = nullptr;
    double theXForceDouble;

    QPointer<RParameter> theYForceParameter = nullptr;
    double theYForceDouble;

    QPointer<RParameter> theZForceParameter = nullptr;
    double theZForceDouble;

    QPointer<RParameter> theMassParameter = nullptr;

    QPointer<RParameter> theXMassParameter = nullptr;
    QPointer<RParameter> theYMassParameter = nullptr;
    QPointer<RParameter> theZMassParameter = nullptr;

    QPointer<RParameter> theXMomentParameter = nullptr;
    double theXMomentDouble;

    QPointer<RParameter> theYMomentParameter = nullptr;
    double theYMomentDouble;

    QPointer<RParameter> theZMomentParameter = nullptr;
    double theZMomentDouble;

    QPointer<RNode> theAssociatedNode = nullptr;

    std::function<double(void)> theXForceLambda;
    std::function<double(void)> theYForceLambda;
    std::function<double(void)> theZForceLambda;

    bool xSet;
    bool ySet;
    bool zSet;

    RPoint* masterPoint = nullptr;

};

#endif

