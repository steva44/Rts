#ifndef RFibreSectionMeshGenerator_H
#define RFibreSectionMeshGenerator_H
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

#include "RMethod.h"
#include <functional>

class RMaterialKnowledge;
class RPoint;
class RNode;
class RParameter;
class RElement;
class RFiber;
class RFibreSectionKnowledge;
class RNDMaterial;
class RQuadRCSectionDamageRegion;
class RQuadSectionDamageRegion;
class R2DPoint;
class RConcreteMaterialKnowledge;

class RFibreSectionMeshGenerator : public RMethod
{

public:
    RFibreSectionMeshGenerator(RDomain *theDomain);
    ~RFibreSectionMeshGenerator();

    // Meshes out fibres for a reinforced concrete quadrilateral section
    int meshRCQuadrilateralSection(const R2DPoint& point1,
                                   const R2DPoint& point2,
                                   const R2DPoint& point3,
                                   const R2DPoint& point4,
                                   const QString& parent,
                                   const int numCoverFibresAlong12,
                                   const int numCoverFibresAlong23,
                                   const int numCoreFibresAlong12,
                                   const int numCoreFibresAlong23,
                                   RParameter* offseta,
                                   RQuadRCSectionDamageRegion* theSegment,
                                   RConcreteMaterialKnowledge *theCoreMaterialKnowledge,
                                   RConcreteMaterialKnowledge *theCoverMaterialKnowledge);


    // Meshes out fibres for a generic quadrilateral section
    int meshHomogeneousQuadrilateralSection(const R2DPoint& point1,
                                            const R2DPoint& point2,
                                            const R2DPoint& point3,
                                            const R2DPoint& point4,
                                            const int numFibresAlong12,
                                            const int numFibresAlong23,
                                            const QString& materialModel,
                                            const QString& parent,
                                            RQuadSectionDamageRegion* theSegment,
                                            RMaterialKnowledge *theMaterialKnowledge);


    // Meshes out fibres for a quadrilateral section of reinforcement
    int meshQuadrilateralRebarLayer(const R2DPoint& point1,
                                    const R2DPoint& point2,
                                    const R2DPoint& point3,
                                    const R2DPoint& point4,
                                    const int numRebarAlong12,
                                    const int numRebarAlong23,
                                    const int numRebarAlong34,
                                    const int numRebarAlong41,
                                    const double rebarArea,
                                    const QString materialType,
                                    const QString& parent,
                                    RParameter* offseta,
                                    RParameter* offsetb,
                                    RParameter* offsetc,
                                    RParameter* offsetd,
                                    RQuadRCSectionDamageRegion* theSegment,
                                    RMaterialKnowledge *theMaterialKnowledge);


    // Meshes out fibres for a reinforced concrete circular section
    int meshRCCircularSection(const double outerRadius,
                              const double innerRadius,
                              const int numFibresAlongR,
                              const int numFibresAlongC,
                              const QString coreMaterialType,
                              const QString coverMaterialType,
                              RQuadRCSectionDamageRegion* theSegment,
                              RMaterialKnowledge *theMaterialKnowledge);


    // Meshes a quadrilateral fibre patch
    int meshQuadrilateralFibrePatch(const R2DPoint& point1,
                                    const R2DPoint& point2,
                                    const R2DPoint& point3,
                                    const R2DPoint& point4,
                                    const int numFibresAlong12,
                                    const int numFibresAlong23,
                                    const QString& materialType,
                                    const QString& parent,
                                    std::vector<RFiber*> *theFibreVector,
                                    RMaterialKnowledge *theMaterialKnowledge);


    // Meshes a linear rebar layer, including the first and last rebar
    int meshLinearRebarLayer(const R2DPoint& point1,
                             const R2DPoint& point2,
                             const QString& materialType,
                             const QString& parent,
                             const int numRebar,
                             const double rebarArea,
                             std::vector<RFiber*> *theFibreVector,
                             RMaterialKnowledge *theMaterialKnowledge);


    // This function skips the first and last rebar so that there is no overlap where two linear rebar layers meet
    int meshLinearRebarLayerII(const R2DPoint& point1,
                               const R2DPoint& point2,
                               const QString& materialType,
                               const QString& parent,
                               const int numRebar,
                               const double rebarArea,
                               std::vector<RFiber*> *theFibreVector,
                               RMaterialKnowledge *theMaterialKnowledge);


    // Meshes a layer of fibres for a plate
    int meshPlateLayer(const R2DPoint& point1,
                       const R2DPoint& point2,
                       const QString& materialType,
                       const QString& parent,
                       const int numFibres,
                       const RParameter* thickness,
                       std::vector<RFiber*> *theFibreVector,
                       RMaterialKnowledge *theMaterialKnowledge);

private:

    int fiberCounter;
    int matCounter;

    QString theFiberType;
    QList<RParameter *> theNodalParameterList;

    bool nodalDebug;
    double tol;

};

#endif
