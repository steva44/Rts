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

#include "RMaterialKnowledge.h"

RMaterialKnowledge::RMaterialKnowledge(RDomain *theDomain, QString componentName)
    : RMethod(theDomain, componentName)
{
    theComponentName = componentName;
}


RMaterialKnowledge::~RMaterialKnowledge()
{

}


RParameter *RMaterialKnowledge::getYoungsModulus()
{
    qCritical()<<"A material created in" << theComponentName << "was unable to create/provide some material parameter.";

    return 0;
}


RParameter *RMaterialKnowledge::getPoissonsRatio()
{
    qCritical()<<"A material created in" << theComponentName << "was unable to create/provide some material parameter.";

    return 0;
}


RParameter *RMaterialKnowledge::getDensity()
{
    qCritical()<<"A material created in" << theComponentName << "was unable to create/provide some material parameter.";

    return 0;
}


QString RMaterialKnowledge::getTheComponentName() const
{
    return theComponentName;
}




