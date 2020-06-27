#ifndef RCrew_H
#define RCrew_H
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

// This class contains a crew of labourers that is used in construction/demolition/repair activities

#include "RSchedulingEnums.h"
#include "RContinuousRandomVariable.h"
#include "RTask.h"

#include <QString>
#include <QVector>
#include <memory>
#include <tuple>

class RLabourer;
class REquipment;

class RCrew
{

public:
    RCrew();
    ~RCrew();

    // Returns the number of labourers in the crew
    int getNumberOfLabourers(void);

    // Add a labourer to the crew
    void addLabourer(const RLabourer& labourer,int numLabourers);

    // Add equipment to the crew
    void addEquipment(const REquipment& equipment,int numEquipment);

    // Add a task to the crew
    void addTask(QString& CSICode, double quantity, QString& description, RComponent* theParentComponent);
    void addTask(RTask task);

    // Unique RSMeans crew code
    RCrewCode getCrewCode(void);
    void setTheCrewCode(const RCrewCode &value);

    // Get the task vector of the crew
    QVector<RTask> getTheTaskVector() const;

    //Overload the comparison operators to compare if two crew are one and the same
    friend bool operator== (const RCrew &crew1, const RCrew &crew2)
    {
        if(crew1.theCrewCode == crew2.theCrewCode)
            return true;
        else
            return false;
    }
    friend bool operator!= (const RCrew &crew1, const RCrew &crew2)
    {
        if(crew1.theCrewCode == crew2.theCrewCode)
            return false;
        else
            return true;
    }

private:

    RCrewCode theCrewCode;

    // Vectors of labourers, equipment, and tasks of the crew
    QVector<RLabourer> labourersOnCrew;
    QVector<REquipment> equipmentOnCrew;
    QVector<RTask> theTaskVector;
};

#endif // RCrew_H
