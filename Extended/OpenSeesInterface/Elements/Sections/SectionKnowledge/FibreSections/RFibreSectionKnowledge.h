#ifndef RFibreSectionKnowledge_H
#define RFibreSectionKnowledge_H
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

// Base class for all fibre discretized cross-sections

#include "RSectionKnowledge.h"
#include "RSection.h"
#include "RFiber.h"

class RElement;
class RSectionForceDeformation;

class RFibreSectionKnowledge : public RSectionKnowledge
{
    Q_OBJECT

public:  
    RFibreSectionKnowledge(RDomain *domain, QString componentName);
    ~RFibreSectionKnowledge();
    
    virtual  std::shared_ptr<std::vector<RSectionForceDeformation*>> getTheSectionForceDeformation(const QString& sectionID) = 0;

    int getNumSections() const;

protected:

    int numSections;
};

#endif
