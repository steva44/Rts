#ifndef RElasticMembranePlateSection_H
#define RElasticMembranePlateSection_H
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

// Wrapper class for OpenSees ElasticMembranePlateSection
// Stevan Gavrilovic 12/2018

#include "RSectionForceDeformation.h"

class ElasticMembranePlateSection;
class RParameter;

class RElasticMembranePlateSection : public RSectionForceDeformation
{
    Q_OBJECT

public:
    // Define the constructor and destructor
    RElasticMembranePlateSection(QObject *parent, QString name);
    ~RElasticMembranePlateSection();

    SectionForceDeformation* getTheSectionForceDeformation(void);
    int update(void);

    RParameter *getTheEParameter() const;
    void setTheEParameter(RParameter *value);

    RParameter *getTheNuParameter() const;
    void setTheNuParameter(RParameter *value);

    RParameter *getTheThicknessParameter() const;
    void setTheThicknessParameter(RParameter *value);

    RParameter *getTheRhoParameter() const;
    void setTheRhoParameter(RParameter *value);

    int generateOpenSeesInput(std::string& object, int flag);

private:

    int createSectionForceDeformation(void);
    int initialize(void);

    std::unique_ptr<ElasticMembranePlateSection> theSectionForceDeformation;

    RParameter* theEParameter;
    RParameter* theNuParameter;
    RParameter* theThicknessParameter;
    RParameter* theRhoParameter;

};

#endif
