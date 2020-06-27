#ifndef RMembranePlateFiberSection_H
#define RMembranePlateFiberSection_H
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

// Wrapper class for OpenSees MembranePlateFiberSection
// Stevan Gavrilovic 12/2018

#include "RSectionForceDeformation.h"

class MembranePlateFiberSection;
class RNDMaterial;
class RParameter;

class RMembranePlateFiberSection : public RSectionForceDeformation
{
    Q_OBJECT

public:
    RMembranePlateFiberSection(QObject *parent, QString name);
    ~RMembranePlateFiberSection();

    SectionForceDeformation* getTheSectionForceDeformation(void);
    int update(void);

    RParameter *getTheThicknessParameter() const;
    void setTheThicknessParameter(RParameter *value);

    RNDMaterial *getTheNDMaterial() const;
    void setTheNDMaterial(RNDMaterial *value);

private:

    int createSectionForceDeformation(void);
    int initialize(void);

    std::unique_ptr<MembranePlateFiberSection> theSectionForceDeformation;

    RNDMaterial* theNDMaterial;

    RParameter* theThicknessParameter;


};

#endif
