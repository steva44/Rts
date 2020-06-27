#ifndef RRectangularFibreSectionKnowledge_H
#define RRectangularFibreSectionKnowledge_H
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

// Rectangular section to mesh out
/*
*                  Depth
*
*       /  4^----------------^3
* cover |   |        ^ y     |
*       /   |        |       |
*           |        |       | Width
*           | z<-----*       |
*           |                |
*           |                |
*          1^----------------^2
*
*          Notice Z axis orientated along the cross-section depth
*/

#include "RFibreSectionKnowledge.h"
#include "RSection.h"
#include "RElement.h"
#include "RSectionForceDeformation.h"

class RGlulamMaterialKnowledge;
class RRectangularGlulamBeamColumnComponent;

class RRectangularFibreSectionKnowledge : public RFibreSectionKnowledge
{

Q_OBJECT

public:
    RRectangularFibreSectionKnowledge(RDomain* domain,
                                        RRectangularGlulamBeamColumnComponent* component,
                                        int& numSec,
                                        int& numFibresAlong12,
                                        int& numFibresAlong23,
                                        QString& materialModel);
    ~RRectangularFibreSectionKnowledge();

    // Cross-section area in m2
    RParameter *getArea();

    // Vector of sections
    std::shared_ptr<std::vector<RSectionForceDeformation*>> getTheSectionForceDeformation(const QString& sectionID);

private:

    RRectangularGlulamBeamColumnComponent* theComponent;
    RParameter *theDepth;
    RParameter *theWidth;
    RParameter *getGJ();
    RParameter *theAreaParameter;
    RParameter *theLongitudinalReinforcementRatio;
    QPointer<RParameter> theGJParameter;

    int numFibresAlong12;
    int numFibresAlong23;

    // Material knowledge containing material properties
    QString theMaterialModel;
    RGlulamMaterialKnowledge* theMatKnowledge;
};

#endif
