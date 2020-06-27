#ifndef RMaterialKnowledge_H
#define RMaterialKnowledge_H
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

// Base class for all material knowledge classes
// Material knowledge classes contain the parameters employed by the material models

#include "RMethod.h"

class RParameter;

class RMaterialKnowledge : public RMethod
{
    Q_OBJECT

public:
    RMaterialKnowledge(RDomain *theDomain, QString componentName);
    ~RMaterialKnowledge();

    // Youngs modulus parameter
    virtual RParameter *getYoungsModulus();

    // Poissons ratio
    virtual RParameter *getPoissonsRatio();

    // Mass density
    virtual RParameter *getDensity();

    // List of all parameters
    inline virtual QList<RParameter*> getParameterList(){return theParameterList;}

    // Component name that the material belongs too
    QString getTheComponentName() const;

protected:

    // Component name that the material belongs too
    QString theComponentName;

    // List of all parameters
    QList<RParameter*> theParameterList;
};

#endif


