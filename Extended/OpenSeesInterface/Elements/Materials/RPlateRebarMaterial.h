#ifndef RPlateRebarMaterial_H
#define RPlateRebarMaterial_H
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

#include "RNDMaterial.h"

class NDMaterial;
class PlateRebarMaterial;

class RPlateRebarMaterial : public RNDMaterial
{
    Q_OBJECT

public:
    RPlateRebarMaterial(QObject *parent, QString &name);
    ~RPlateRebarMaterial();

    NDMaterial* getTheNDMaterial(void);
    int update(void);

    double getReinforcementAngle() const;

    //The angle in degrees, positive from the x-y plane. Eg. transverse reinf angle is typically 0 while longitudinal is 90
    void setReinforcementAngle(double value);
    void setTheUniaxialMaterial(RUniaxialMaterial *value);

    int generateOpenSeesInput(std::string& object, int flag);

private:

    RUniaxialMaterial *theUniaxialMaterial;

    //In degrees
    double reinforcementAngle;

    std::unique_ptr<PlateRebarMaterial> thePlateRebarMaterial;
    int createPlateRebarMaterial(void);
};

#endif


