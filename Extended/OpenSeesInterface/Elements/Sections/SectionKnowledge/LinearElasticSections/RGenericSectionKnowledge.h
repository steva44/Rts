#ifndef RGenericSectionKnowledge_H
#define RGenericSectionKnowledge_H
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

// Cross-section of a generic section

#include "RLinearElasticSectionKnowledge.h"
#include "RParameter.h"

class RGenericSectionKnowledge : public RLinearElasticSectionKnowledge
{
    Q_OBJECT

public:
    RGenericSectionKnowledge(QObject* parent, QString name);
    ~RGenericSectionKnowledge();

    // Area of cross-section in m^2
    RParameter *getArea();

    // Moment of interia about y axis m^4
    RParameter *getIy();

    // Moment of interia about z axis m^4
    RParameter *getIz();

    // Torsional constant m^4
    RParameter *getJ();

    void setA(RParameter* parameter);
    void setIy(RParameter* parameter);
    void setIz(RParameter* parameter);
    void setJ(RParameter* parameter);

    inline QString getAreaString(void){return theAreaParameter->objectName();}
    inline QString getIyString(void){return theIyParameter->objectName();}
    inline QString getIzString(void){return theIzParameter->objectName();}
    inline QString getJString(void){return theAreaParameter->objectName();}

private:

    QPointer<RParameter> theAreaParameter;
    QPointer<RParameter> theIyParameter;
    QPointer<RParameter> theIzParameter;
    QPointer<RParameter> theJParameter;

};

#endif
