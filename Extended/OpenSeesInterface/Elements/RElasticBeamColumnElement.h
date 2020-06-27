#ifndef RElasticBeamColumnElement_H
#define RElasticBeamColumnElement_H
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

// Base class for a linear elastic beam-column element

#include "RElement.h"

class RNode;
class RParameter;
class CrdTransf;

class RElasticBeamColumnElement : public RElement
{
    Q_OBJECT
    
public:
    RElasticBeamColumnElement(QObject *parent, QString &name);
    ~RElasticBeamColumnElement();

    // The cross-section parameters
    inline void setRho(RParameter* theParameter){ theRhoParameter = theParameter;}
    inline void setOrientation(RParameter* theParameter){ theOrientationParameter = theParameter;}
    inline void setE(RParameter* theParameter){ theEParameter = theParameter;}
    inline void setNu(RParameter* theParameter){ theNuParameter= theParameter;}
    inline void setA(RParameter* theParameter){ theAParameter= theParameter;}
    inline void setIz(RParameter* theParameter){ theIzParameter = theParameter;}
    inline void setIy(RParameter* theParameter){ theIyParameter = theParameter;}
    inline void setJ(RParameter* theParameter){ theJParameter = theParameter;}
    inline void setAvZ(RParameter* theParameter){ theAvZParameter = theParameter;}
    inline void setAvY(RParameter* theParameter){ theAvYParameter = theParameter;}

    // Get and set functions for the end nodes
    int    getNumNodes();
    RNode* getNode(int i) const;
    int    setNode(int i, RNode *theNode);

    // Global finite element response (forces, stresses, strains, etc.)
    virtual int  getGlobalResponse(QVector<double> *response, QString type, double xi) = 0;

    // Local finite element response (forces, stresses, strains, etc.)
    virtual int  getLocalResponse(QVector<double> *response, QString type, double xi) = 0;

    // The type of coordinate trasformation, e.g., linear, P-delta, etc.
    QString getTheCrdTransformationType() const;
    void setCrdTransformationType(const QString &value);

    // Vector to define the x-z plane of the element, i.e., its orientation
    std::vector<double> getVecxzPlane() const;

    // The orientation of the element along its centroidal axis
    QPointer<RParameter> getTheOrientation() const;

    // The length of the element
    double getLength(void);

protected:

    // End nodes
    RNode *theNode1;
    RNode *theNode2;

    // Cross-section parameters
    QPointer<RParameter> theRhoParameter;
    QPointer<RParameter> theOrientationParameter;
    QPointer<RParameter> theEParameter;
    QPointer<RParameter> theNuParameter;
    QPointer<RParameter> theAParameter;
    QPointer<RParameter> theIzParameter;
    QPointer<RParameter> theIyParameter;
    QPointer<RParameter> theIyzParameter;
    QPointer<RParameter> theJParameter;
    QPointer<RParameter> theAvZParameter;
    QPointer<RParameter> theAvYParameter;

    // The type of coordinate trasformation, e.g., linear, P-delta, etc.
    QString theCrdTransformationType;
    std::unique_ptr<CrdTransf> theCrdTransformation;
    void createTheCrdTransformation(void);

    // Vector to define the x-z plane of the element, i.e., its orientation
    std::vector<double> VecxzPlane;
};



#endif
