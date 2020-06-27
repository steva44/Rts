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
#include "RElement.h"
#include "RSectionForceDeformation.h"

RFibreSectionKnowledge::RFibreSectionKnowledge(RDomain *domain, QString componentName)
    : RSectionKnowledge(domain, componentName)
{
    numSections = -1;
}


RFibreSectionKnowledge::~RFibreSectionKnowledge()
{

}


int RFibreSectionKnowledge::getNumSections() const
{
    return numSections;
}


