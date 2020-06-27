#ifndef RMeshGenerator_H
#define RMeshGenerator_H
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

//  This class meshes out the components into finite elements while supplying the nodal gradients

#include "RMethod.h"

class RDecisionVariable;
class RDisplacementBasedElement;
class RAlgebraicExpressionModel;
class RMaterialKnowledge;
class RFibreSection;
class RSection;
class RPoint;
class RNode;
class RParameter;
class RElement;
class RMaterial;
class R2DPoint;
class R3DPoint;

class RMeshGenerator : public RMethod
{

public:
    RMeshGenerator(RDomain *domain);
    ~RMeshGenerator();

    int meshQuadrilateralArea(RPoint *point1,
                              RPoint *point2,
                              RPoint *point3,
                              RPoint *point4,
                              QString elementType,
                              RParameter* numElementsAlong12,
                              RParameter* numElementsAlong23,
                              QString creatorName,
                              QList<RElement *> &elementList);


    int meshQuadrilateralAreaSection(R2DPoint &point1,
                                     R2DPoint &point2,
                                     R2DPoint &point3,
                                     R2DPoint &point4,
                                     int numElementsAlong12,
                                     int numElementsAlong23,
                                     RDisplacementBasedElement* theElement,
                                     QString materialType,
                                     RSection* theSection,
                                     QVector<RMaterial*> *theMaterialList,
                                     QString creatorName,
                                     RMaterialKnowledge *theMaterialKnowledge);


    bool meshLinearElement(RPoint *point1,
                           RPoint *point2,
                           QString elementType,
                           int numberOfElements,
                           QString creatorName,
                           QList<RElement *> &elementList);


    int meshHexhedron(RPoint *point1,
                      RPoint *point2,
                      RPoint *point3,
                      RPoint *point4,
                      RPoint *point5,
                      RPoint *point6,
                      RPoint *point7,
                      RPoint *point8,
                      QString elementType,
                      int numElementsAlong12,
                      int numElementsAlong23,
                      int numElementsAlong18,
                      QString creatorName,
                      QList<RElement *> &elementList);

private:

    QList<RParameter *> theNodalParameterList;

    bool nodalDebug;

    double tol;

};

#endif
