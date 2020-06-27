#ifndef REquipment_H
#define REquipment_H
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

// Brief class for items of important equipment

#include "RSchedulingEnums.h"

#include <QString>
#include <vector>
#include <memory>

class REquipment
{

public:
    REquipment();
    REquipment(REquipmentType type);
    ~REquipment();

    // Type of equipment
    REquipmentType getTheTypeOfEquipment() const;
    void setTheTypeOfEquipment(const REquipmentType &value);

private:

    REquipmentType theTypeOfEquipment;
};

#endif // REquipment_H
