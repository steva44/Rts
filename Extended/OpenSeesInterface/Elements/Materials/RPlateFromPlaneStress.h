#ifndef RPlateFromPlaneStress_H
#define RPlateFromPlaneStress_H
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
class PlateFromPlaneStressMaterial;

class RPlateFromPlaneStress : public RNDMaterial
{
    Q_OBJECT

public:
    RPlateFromPlaneStress(QObject *parent, QString &name);
    ~RPlateFromPlaneStress();

    //Note: this is the material used by this class not returned by it!!
    void setTheNDMaterial(RNDMaterial *value);
    int update(void);

    NDMaterial* getTheNDMaterial(void);

    RParameter *getOutOfPlaneShearModulus() const;
    void setOutOfPlaneShearModulus(RParameter *value);

    int generateOpenSeesInput(std::string& object, int flag);

private:

    RParameter* OutOfPlaneShearModulus;

    //This class requires that a plane stress material be set
    RNDMaterial* planeStressMaterial;

    std::unique_ptr<PlateFromPlaneStressMaterial> thePlateFromPlaneStressMaterial;
    int createPlateFromPlaneStressMaterial(void);
};

#endif


