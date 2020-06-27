#ifndef RWFibreSectionKnowledge_H
#define RWFibreSectionKnowledge_H
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

#include "RFibreSectionKnowledge.h"

class RSectionForceDeformation;
class RMaterialKnowledge;

class RWFibreSectionKnowledge : public RFibreSectionKnowledge
{

public: 
    RWFibreSectionKnowledge(RDomain *domain,
                            QString componentName,
                            int secNum,
                            RParameter *flangeWidth,
                            RParameter *flangeThickness,
                            RParameter *webHeight,
                            RParameter *webThickness,
                            int numFibresInFlange,
                            int numFibresInWeb,
                            QString materialModel,
                            RMaterialKnowledge *materialKnowledge);
    ~RWFibreSectionKnowledge();

    std::shared_ptr<std::vector<RSectionForceDeformation*>> getTheSectionForceDeformation(const QString& sectionID);

    // Area of the cross-section in m2
    RParameter *getArea();

private:

    // Cross-section parameters
    QPointer<RParameter> theWebHeight;
    QPointer<RParameter> theFlangeWidth;
    QPointer<RParameter> theWebThickness;
    QPointer<RParameter> theFlangeThickness;
    QPointer<RParameter> theAreaParameter;
    QPointer<RParameter> theGJParameter;
    RParameter *getGJ();

    // Cross-section discretization
    int theNumFibresInFlange;
    int theNumFibresInWeb;

    // Material knowledge containing material properties
    QString theMaterialModel;
    RMaterialKnowledge *theMaterialKnowledge;

};

#endif
