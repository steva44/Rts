#ifndef RRectangularRCFibreSectionKnowledge_H
#define RRectangularRCFibreSectionKnowledge_H
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

class RConcreteMaterialKnowledge;
class RSteelMaterialKnowledge;
class RQuadRCSectionDamageRegion;
class RRectangularRCColumnComponent;
class RSectionForceDeformation;

class RRectangularRCFibreSectionKnowledge : public RFibreSectionKnowledge
{

Q_OBJECT

public:
    RRectangularRCFibreSectionKnowledge(RDomain* domain,
                                        RRectangularRCColumnComponent* component,
                                        int& numSec,
                                        int& numFibresInCoreAlong12,
                                        int& numFibresInCoreAlong23,
                                        int& numFibresInCoverAlong12,
                                        int& numFibresInCoverAlong23,
                                        int& numReinforcementFibresAlong12,
                                        int& numReinforcementFibresAlong23,
                                        int& numReinforcementFibresAlong34,
                                        int& numReinforcementFibresAlong41);
    ~RRectangularRCFibreSectionKnowledge();

    std::shared_ptr<std::vector<RSectionForceDeformation*>> getTheSectionForceDeformation(const QString& sectionID);

private:

    // Cross-section parameters
    RParameter *theDepth;
    RParameter *theWidth;
    RParameter *theCoverThickness;
    RParameter *theLongitudinalReinforcementRatio;
    QPointer<RParameter> theGJParameter;
    RParameter *getGJ();

    // Cross-section discretization
    int numFibresInCoreAlong12;
    int numFibresInCoreAlong23;

    int numFibresInCoverAlong12;
    int numFibresInCoverAlong23;

    int numRebarAlong12;
    int numRebarAlong23;
    int numRebarAlong34;
    int numRebarAlong41;

    // The parent component
    RRectangularRCColumnComponent* theComponent;

    // Material knowledge containing material properties
    RConcreteMaterialKnowledge* theUnconfinedConcreteMatKnowledge;
    RConcreteMaterialKnowledge* theConfinedConcreteMatKnowledge;
    RSteelMaterialKnowledge* theReinforcementMaterialKnowledge;

    // Area of the longitudinal reinforcement
    double theLongRebarArea;
};

#endif
