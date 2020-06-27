#ifndef RRectangularLinearElasticSectionKnowledge_H
#define RRectangularLinearElasticSectionKnowledge_H
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

// Cross-section of a rectangular component

#include "RLinearElasticSectionKnowledge.h"

class RParameter;

class RRectangularLinearElasticSectionKnowledge : public RLinearElasticSectionKnowledge
{
    Q_OBJECT

public:
    RRectangularLinearElasticSectionKnowledge(RDomain *domain,
                                              QString componentName,
                                              RParameter *depth,
                                              RParameter *width);
    ~RRectangularLinearElasticSectionKnowledge();

    // Area of cross-section in m^2
    RParameter *getArea();

    // Moment of interia about y axis m^4
    RParameter *getIy();

    // Moment of interia about z axis m^4
    RParameter *getIz();

    // Torsional constant m^4
    RParameter *getJ();

    // Shear area along the y axis in m^2
    RParameter *getAvy();

    // Shear area along the y axis in m^2
    RParameter *getAvz();

    // Depth of the cross-section in m
    QPointer<RParameter> getTheDepth() const;
    void setTheDepth(const QPointer<RParameter> &value);

    // Width of the cross-section in m
    QPointer<RParameter> getTheWidth() const;
    void setTheWidth(const QPointer<RParameter> &value);

private:

    QPointer<RParameter> theDepth;
    QPointer<RParameter> theWidth;
    QPointer<RParameter> theAreaParameter;
    QPointer<RParameter> theIyParameter;
    QPointer<RParameter> theIzParameter;
    QPointer<RParameter> theJParameter;
    QPointer<RParameter> theAvyParameter;
    QPointer<RParameter> theAvzParameter;
};

#endif
