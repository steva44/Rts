#ifndef RSectionForceDeformation_H
#define RSectionForceDeformation_H
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
*                                                                    *
*********************************************************************/

// Wrapper class for OpenSees SectionForceDeformation
// Stevan Gavrilovic 12/2018

// Abstract class; can only derive from it

#include "RSection.h"
#include "RQuadRCSectionDamageRegion.h"

class SectionForceDeformation;

class RSectionForceDeformation : public RSection
{
    Q_OBJECT

public:
    RSectionForceDeformation(QObject *parent, QString name);
    virtual ~RSectionForceDeformation();

    virtual SectionForceDeformation* getTheSectionForceDeformation(void) = 0;
    virtual int update(void) = 0;

    int getSectionNumber() const;
    void setSectionNumber(int value);

protected:

    int sectionNumber;

};

#endif
