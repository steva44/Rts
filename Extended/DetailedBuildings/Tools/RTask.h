#ifndef RTask_H
#define RTask_H
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

// Class that contains a task, with its CSI code, quantities, crew responsible for its completion, etc.

#include "RSchedulingEnums.h"

#include <QString>
#include <vector>
#include <memory>

class RRandomVariable;
class RCrew;
class RComponent;

class RTask
{

public:
    RTask();
    RTask(RCrew* parent, QString CSInum, double unitsQuantity, QString description, RComponent* component);
    ~RTask();

    // CSI code corresponding to this task
    QString getCSICode() const;
    double getCSICodeAsDouble() const;

    // Quantity of units of the task
    double getQuantityOfUnits() const;

    // Crew responsible for completion of the task
    RCrew* getParentCrew() const;

    // Description of the task
    QString getDescription() const;
    void setDescription(const QString &value);

    // Component that the task originates from
    RComponent *getParentComponent() const;

private:

    // Description of the task
    QString Description;

    // CSI code corresponding to this task
    QString CSICode;
    double CSICodeAsDouble;

    // Quantity of units of the task
    double quantityOfUnits;

    // Crew responsible for completion of the task
    RCrew* parentCrew;

    // Component that the task originates from
    RComponent* parentComponent;
};

#endif // RTask_H
