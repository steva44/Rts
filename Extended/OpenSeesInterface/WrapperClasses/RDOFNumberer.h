#ifndef RDOFNumberer_H
#define RDOFNumberer_H
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

// Wrapper class for OpenSees DOF_Numberer
// Stevan Gavrilovic 12/2018

#include "RObject.h"
#include <DOF_Numberer.h>
#include "GraphNumberer.h"
#include <RCM.h>

class RDOFNumberer : public RObject
{
    Q_OBJECT

    Q_PROPERTY(QString NumberingScheme   READ getNumberingScheme  WRITE setNumberingScheme)

public:
    // Define the constructor and destructor
    RDOFNumberer(QObject *parent, QString name);
    ~RDOFNumberer();

    DOF_Numberer* getTheDOFNumberer();

    QString getNumberingScheme() const;
    void setNumberingScheme(const QString &value);

protected:

    std::unique_ptr<DOF_Numberer> theDOFNumberer;
    GraphNumberer* theGraphNumberer;

    QString numberingScheme;

};

#endif
