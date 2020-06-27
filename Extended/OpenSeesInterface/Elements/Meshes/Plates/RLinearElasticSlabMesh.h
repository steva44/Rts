#ifndef RLinearElasticSlabMesh_H
#define RLinearElasticSlabMesh_H
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

#include "RSlabMesh.h"
#include "RElement.h"
#include "RTopoMesh.h"

class RLinearElasticSlabMesh : public RSlabMesh
{

public:
    RLinearElasticSlabMesh(RDomain* domain,
                           QString componentName,
                           RMaterialKnowledge* material,
                           RPoint *point1,
                           RPoint *point2,
                           RPoint *point3,
                           RPoint *point4,
                           RParameter* thickness,
                           QPointer<RParameter> numElementsAlongU,
                           QPointer<RParameter> numElementsAlongV,
                           QList<RParameter*> &theParameterList,
                           QList<RElement*> &elementList,
                           int meshOption);
    ~RLinearElasticSlabMesh();

    double getMaxCompressiveStressXX();
    double getMaxTensileStressXX();

    double getMaxCompressiveStressYY();
    double getMaxTensileStressYY();

    double getMaxCompressiveStressZZ();
    double getMaxTensileStressZZ();

    double getMaxShearStressXY();
    double getMaxShearStressYZ();
    double getMaxShearStressXZ();

    // Maximum vertical deflection of the slab in m
    double getMaxVerticalDeflection();

    int commitState();

    // Width of the slab in m
    double getWidth();

    // Length of the slab in m
    double getLength();

    // Thickness of the slab in m
    double getThickness();

    int createMesh(void);

    int createTopologicalMesh(void);

    QVector< QVector<RNode *> >getNodeMatrix(void);

private:

    QPointer<RParameter> theNumElementsAlongU;
    QPointer<RParameter> theNumElementsAlongV;

    QList<RElement*> theComponentElementList;
    double maxTensileStressXX = 0.0;
    double maxCompressiveStressXX = 0.0;

    double maxTensileStressYY = 0.0;
    double maxCompressiveStressYY = 0.0;

    double maxTensileStressZZ = 0.0;
    double maxCompressiveStressZZ = 0.0;

    double maxShearStressXY = 0.0;
    double maxShearStressYZ = 0.0;
    double maxShearStressXZ = 0.0;

    double minShearStressXY = 0.0;
    double minShearStressYZ = 0.0;
    double minShearStressXZ = 0.0;

    RMaterialKnowledge* theMaterial;

};

#endif
