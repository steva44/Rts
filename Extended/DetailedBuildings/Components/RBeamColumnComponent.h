#ifndef RBeamColumnComponent_H
#define RBeamColumnComponent_H
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

// The beam column component parent class, beams and columns inherit information and functionality from this class

#include "RComponent.h"
#include "RPoint.h"
#include "RContinuousRandomVariable.h"

class RBeamColumnMesh;
class RSectionKnowledge;
class RMesh;

class RBeamColumnComponent : public RComponent
{
    Q_OBJECT
    Q_PROPERTY(QObject *Point1 READ getPoint1 WRITE setPoint1)
    Q_PROPERTY(QObject *Point2 READ getPoint2 WRITE setPoint2)
    Q_PROPERTY(QString Orientation READ getOrientation WRITE setOrientation)

public:
    RBeamColumnComponent(QObject *parent, QString &name);
    ~RBeamColumnComponent();

    // The orientation of the beam column along its centroidal axis
    QString getOrientation();
    void setOrientation(QString value);

    // The end points
    inline QObject *getPoint1() const {return thePoint1;}
    void setPoint1(QObject *value);

    inline QObject *getPoint2() const {return thePoint2;}
    void setPoint2(QObject *value);

    // Volume of the beam or column
    double getVolume(void);

    // Length in m of the beam or column
    double getLength(void);
    RParameter* getTheLengthParameter() const;

    // Cross-section area in m2 of the beam or column
    double getCrossSectionArea(void);

    // Integration type for nonlinear analysis
    QString getTheIntegrationType() const;
    void setTheIntegrationType(const QString &value);

    // The end points
    RPoint* thePoint1;
    RPoint* thePoint2;

    QList<RPoint*> getPointList();

    // Get the finite element mesh
    RMesh* getMesh(void);

protected:

    QPointer<RParameter> theOrientationParameter;
    QPointer<RParameter> theLengthParameter;
    QPointer<RParameter> theAreaParameter;

    unique_ptr<RBeamColumnMesh> theMesh;
    unique_ptr<RSectionKnowledge> theCrossSection;

private:

    QMap<RParameter*,RPoint*>theXNodalParameterMap;
    QMap<RParameter*,RPoint*>theYNodalParameterMap;
    QMap<RParameter*,RPoint*>theZNodalParameterMap;

    QString theIntegrationType;

};

#endif
