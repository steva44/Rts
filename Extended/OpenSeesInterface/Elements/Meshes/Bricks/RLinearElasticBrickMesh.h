#ifndef RLinearElasticBrickMesh_H
#define RLinearElasticBrickMesh_H
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

#include "RBeamColumnMesh.h"

class RMaterialKnowledge;
class RLinearElasticSectionKnowledge;
class RPoint;
class RParameter;
class RSectionKnowledge;

class RLinearElasticBrickMesh : public RBeamColumnMesh
{
    Q_OBJECT

public:
    RLinearElasticBrickMesh(RDomain *theDomain,
                            QString parentName,
                            RMaterialKnowledge *material,
                            RSectionKnowledge *crossSection,
                            RPoint *point1,
                            RPoint *point2,
                            int numElementsU,
                            int numElementsV,
                            int numElementsW,
                            QList<RParameter*> &parameterList,
                            QList<RElement*> &elementList);

    ~RLinearElasticBrickMesh();

    int createMesh(void);

    double getMaxStress();

    double getAxialLoad(int elementNumber);

private:

    int numberElementsU;
    int numberElementsV;
    int numberElementsW;

    RSectionKnowledge *theCrossSection;
    RMaterialKnowledge *theMaterial;
    QString theParentName;

    RParameter* depth;
    RParameter* width;
    
};

#endif