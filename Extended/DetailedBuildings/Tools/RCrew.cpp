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

#include "RCrew.h"
#include "RLabourer.h"
#include "REquipment.h"
#include "RComponent.h"

RCrew::RCrew()
{	

}


RCrew::~RCrew()
{

}


int RCrew::getNumberOfLabourers(void)
{
    return labourersOnCrew.size();
}


void RCrew::addLabourer(const RLabourer& labourer,int numLabourers)
{
    for(int i = 0; i<numLabourers;++i)
    {
        labourersOnCrew.push_back(labourer);
    }
}


void RCrew::addEquipment(const REquipment& equipment,int numEquipment)
{
    for(int i = 0; i<numEquipment;++i)
    {
        equipmentOnCrew.push_back(equipment);
    }
}


void RCrew::addTask(QString& CSICode, double quantity, QString& description, RComponent* theParentComponent)
{
    RTask newTask(this,CSICode,quantity, description, theParentComponent);

    theTaskVector.push_back(newTask);

}


void RCrew::addTask(RTask task)
{
    theTaskVector.push_back(task);
}


RCrewCode RCrew::getCrewCode(void)
{
    return theCrewCode;
}


QVector<RTask> RCrew::getTheTaskVector() const
{
    return theTaskVector;
}


void RCrew::setTheCrewCode(const RCrewCode &value)
{
    theCrewCode = value;
}
