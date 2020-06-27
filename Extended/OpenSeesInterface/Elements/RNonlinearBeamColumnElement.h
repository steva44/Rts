#ifndef RNonlinearBeamColumnElement_H
#define RNonlinearBeamColumnElement_H
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

// Abstract base class for a nonlinear beam column element

#include "RElement.h"

class RNode;
class RParameter;
class CrdTransf;
class RSectionForceDeformation;
class BeamIntegration;

class RNonlinearBeamColumnElement : public RElement
{
    Q_OBJECT
    
public:
    RNonlinearBeamColumnElement(QObject *parent, QString &name);
    ~RNonlinearBeamColumnElement();

    inline void setRho(RParameter* theParameter){ theRhoParameter = theParameter;}
    inline void setOrientation(RParameter* theParameter){ theOrientationParameter = theParameter;}

    int getNumNodes();
    RNode* getNode(int i) const;
    int setNode(int i, RNode *theNode);

    virtual int getGlobalResponse(QVector<double> *response, QString type,int sectionNum) = 0;
    virtual int getLocalResponse(QVector<double> *response, QString type, int sectionNum) = 0;

    QString getTheCrdTransformationType() const;
    void setCrdTransformationType(const QString &value);

    std::shared_ptr<std::vector<RSectionForceDeformation*>> getTheSectionForceDeformation() const;
    void setTheSectionForceDeformation(std::shared_ptr<std::vector<RSectionForceDeformation*>> value);

    QString getIntegrationType() const;
    void setIntegrationType(const QString &value);

    std::vector<double> getVecxzPlane() const;

    std::vector<double> getSectionLocations(void) const;

    QPointer<RParameter> getTheOrientation() const;

    double getLength(void) const;

    int getNumSections() const;

protected:
    RNode *theNode1;
    RNode *theNode2;

    QPointer<RParameter> theRhoParameter;
    QPointer<RParameter> theOrientationParameter;

    QString theCrdTransformationType;
    std::unique_ptr<CrdTransf> theCrdTransformation;

    QString theIntegrationType;
    std::unique_ptr<BeamIntegration> theBeamIntegration;

    void createTheCrdTransformation(void);

    void createTheBeamIntegration(void);

    std::shared_ptr<std::vector<RSectionForceDeformation*>> theSectionForceDeformation;

    std::vector<double> VecxzPlane;

    int numSections;
};



#endif
