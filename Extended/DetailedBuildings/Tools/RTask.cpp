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

#include "RTask.h"
#include "RRandomVariable.h"
#include "RComponent.h"

#include "RCrew.h"

RTask::RTask()
{

}


RTask::RTask(RCrew* parent, QString CSInum, double unitsQuantity, QString description, RComponent* component)
    : Description(description), CSICode(CSInum),quantityOfUnits(unitsQuantity), parentCrew(parent), parentComponent(component)
{
    CSICodeAsDouble = CSInum.toDouble();
}


RTask::~RTask()
{

}


QString RTask::getCSICode() const
{
    return CSICode;
}


double RTask::getCSICodeAsDouble() const
{
    return CSICodeAsDouble;
}


double RTask::getQuantityOfUnits() const
{
    return quantityOfUnits;
}


RCrew* RTask::getParentCrew() const
{
    return parentCrew;
}


QString RTask::getDescription() const
{
    return Description;
}


void RTask::setDescription(const QString &value)
{
    Description = value;
}


RComponent *RTask::getParentComponent() const
{
    return parentComponent;
}

