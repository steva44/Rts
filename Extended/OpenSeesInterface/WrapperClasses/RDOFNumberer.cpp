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

#include "RDOFNumberer.h"
#include "RCM.h"

RDOFNumberer::RDOFNumberer(QObject *parent, QString name)
    : RObject(parent, name)
{
    numberingScheme = "NULL";

    theDOFNumberer = nullptr;
    theGraphNumberer = nullptr;
}


RDOFNumberer::~RDOFNumberer()
{

}


QString RDOFNumberer::getNumberingScheme() const
{
    return numberingScheme;
}


void RDOFNumberer::setNumberingScheme(const QString &value)
{
    if(QString::compare(value,"RCM") == 0)
    {
        theGraphNumberer = new RCM();
    }
}


DOF_Numberer* RDOFNumberer::getTheDOFNumberer()
{
    if(!theDOFNumberer && theGraphNumberer)
        theDOFNumberer = std::make_unique<DOF_Numberer>(*theGraphNumberer);

    return theDOFNumberer.get();
}



