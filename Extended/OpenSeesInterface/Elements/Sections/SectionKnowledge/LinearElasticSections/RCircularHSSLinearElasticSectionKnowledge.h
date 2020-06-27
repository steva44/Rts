#ifndef RCircularHSSLinearElasticSectionKnowledge_H
#define RCircularHSSLinearElasticSectionKnowledge_H
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

// Cross-section of a circular hollow steel component

#include "RLinearElasticSectionKnowledge.h"

class RParameter;

class RCircularHSSLinearElasticSectionKnowledge : public RLinearElasticSectionKnowledge
{

public:
    RCircularHSSLinearElasticSectionKnowledge(RDomain *domain, QString componentName, RParameter *outerDiameterParameter, RParameter *wallThicknessParameter);
    ~RCircularHSSLinearElasticSectionKnowledge();

    // Area of cross-section in m^2
    RParameter *getArea();

    // Moment of interia about y axis m^4
    RParameter *getIy();

    // Moment of interia about z axis m^4
    RParameter *getIz();

    // Torsional constant m^4
    RParameter *getJ();

private:

    QPointer<RParameter> theOuterDiameterParameter;
    QPointer<RParameter> theWallThicknessParameter;
    QPointer<RParameter> theAreaParameter;
    QPointer<RParameter> theIyParameter;
    QPointer<RParameter> theIzParameter;
    QPointer<RParameter> theJParameter;

};

#endif
