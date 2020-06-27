#ifndef RRPlateFiberMaterial_H
#define RRPlateFiberMaterial_H
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

class RParameter;
class RMaterialKnowledge;
class NDMaterial;
class PlateFiberMaterial;

// This plate-fiber material wrapper converts any three-dimensional material into a plate fiber material (by static condensation) appropriate for shell analysis.
// nDMaterial PlateFiber $matTag $threeDTag
// $matTag	integer tag identifying material
// $threeDTag	material tag for a previously-defined three-dimensional material

class RPlateFiberMaterial : public RNDMaterial
{
    Q_OBJECT

public:
    RPlateFiberMaterial(QObject *parent, QString &name);
    ~RPlateFiberMaterial();

    NDMaterial* getTheNDMaterial(void);
    int update(void);

    NDMaterial *getThe3DMaterial() const;
    void setThe3DMaterial(NDMaterial *value);

private:

    int createRPlateFiberMaterial(void);

    std::unique_ptr<PlateFiberMaterial> thePlateFiberMaterial;

    NDMaterial* the3DMaterial;

};

#endif


