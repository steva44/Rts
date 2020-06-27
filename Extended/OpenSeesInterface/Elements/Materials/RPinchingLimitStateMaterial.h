#ifndef RPinchingLimitStateMaterial_H
#define RPinchingLimitStateMaterial_H
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

// Note: this class is a work in progress and its implementation is still not complete

// $matTag	unique material object integer tag
// $nodeT	integer node tag to define the first node at the extreme end of the associated flexural frame member (L3 or D5 in Figure)
// $nodeB	integer node tag to define the last node at the extreme end of the associated flexural frame member (L2 or D2 in Figure)
// $driftAxis	integer to indicate the drift axis in which lateral-strength degradation will occur. This axis should be orthogonal to the axis of measured rotation (see $rotAxis in Rotation Shear Curve definition)
// driftAxis = 1 – Drift along the x-axis
// driftAxis = 2 – Drift along the y-axis driftAxis = 3 – Drift along the z-axis
// $Kelas	floating point value to define the initial material elastic stiffness (Kelastic); Kelas > 0
// $crvTyp	integer flag to indicate the type of limit curve associated with this material.
// crvTyp = 0 – No limit curve
// crvTyp = 1 – axial limit curve crvTyp = 2 – RotationShearCurve
// $crvTag	integer tag for the unique limit curve object associated with this material
// $YpinchUPN	floating point unloading force pinching factor for loading in the negative direction
// Note: This value must be between zero and unity
// $YpinchRPN	floating point reloading force pinching factor for loading in the negative direction
// Note: This value must be between negative one and unity
// $XpinchRPN	floating point reloading displacement pinching factor for loading in the negative direction
// Note: This value must be between negative one and unity
// $YpinchUNP	floating point unloading force pinching factor for loading in the positive direction
// Note: This value must be between zero and unity
// $YpinchRNP	floating point reloading force pinching factor for loading in the positive direction
// Note: This value must be between negative one and unity
// $XpinchRNP	floating point reloading displacement pinching factor for loading in the positive direction
// Note: This value must be between negative one and unity
// $dmgStrsLimE	floating point force limit for elastic stiffness damage (typically defined as the lowest of shear strength or shear at flexrual yielding).
// This value is used to compute the maximum deformation at flexural yield (δmax Eq. 1) and using the initial elastic stiffness (Kelastic) the monotonic energy (Emono Eq. 1) to yield. Input 1 if this type of damage is not required and set $dmgE1, $dmgE2, $dmgE3, $dmgE4, and $dmgELim to zero
// $dmgDispMax	floating point for ultimate drift at failure (δmax Eq. 1) and is used for strength and stiffness damage.
// This value is used to compute the monotonic energy at axial failure (Emono Eq. 2) by computing the area under the backbone in the positive loading direction up to δmax. Input 1 if this type of damage is not required and set $dmgR1, $dmgR2, $dmgR3, $dmgR4, and $dmgRLim to zero for reloading stiffness damage. Similarly set $dmgS1, $dmgS2, $dmgS3, $dmgS4, and $dmgSLim to zero if reloading strength damage is not required
// $dmgE1 $dmgE2 $dmgE3 $dmgE4	floating point elastic stiffness damage factors α1,α2,α3,α4 shown in Eq. 1
// $dmgELim	floating point elastic stiffness damage limit Dlim shown in Eq. 1; Note: This value must be between zero and unity
// $dmgR1 $dmgR2 $dmgR3 $dmgR4	floating point reloading stiffness damage factors α1,α2,α3,α4 shown in Eq. 1
// $dmgRLim	floating point reloading stiffness damage limit Dlim shown in Eq. 1; Note: This value must be between zero and unity
// $dmgRCyc	floating point cyclic reloading stiffness damage index; Note: This value must be between zero and unity
// $dmgS1 $dmgS2 $dmgS3 $dmgS4	floating point backbone strength damage factors α1,α2,α3,α4 shown in Eq. 1
// $dmgSLim	floating point backbone strength damage limit Dlim shown in Eq. 1; Note: This value must be between zero and unity
// $dmgSCyc	floating point cyclic backbone strength damage index; Note: This value must be between zero and unity

#include "RUniaxialMaterial.h"

class RParameter;
class RMainWindow;
class QCustomPlot;
class Domain;
class Element;
class Node;
class RLimitCurve;

class RPinchingLimitStateMaterial : public RUniaxialMaterial
{
    Q_OBJECT

public:
    RPinchingLimitStateMaterial(QObject *parent, QString &name);
    ~RPinchingLimitStateMaterial();

    UniaxialMaterial* getUniaxialMaterial();
    int update(void);

    // PROPERTIES SET BY THE USER
    RLimitCurve *getTheLimitCurve() const;
    void setTheLimitCurve(RLimitCurve *value);

    RParameter *getKelas() const;
    void setKelas(RParameter *value);

    int getNdT() const;
    void setNdT(int value);

    int getNdB() const;
    void setNdB(int value);

    int getLimitCurveType() const;
    void setLimitCurveType(int value);

    int getDriftAxis() const;
    void setDriftAxis(int value);

protected slots:

    void plotStressStrainBackbone(void);
    void plotStrainHistoryRunOne(void);
    void plotStrainHistoryRunTwo(void);
    void plotStrainHistoryRunThree(void);
    void plotStrainHistoryRunFour(void);
    void plotStrainHistoryRunFive(void);
    void plotStrainHistoryRunSix(void);
    void plotStrainHistoryRunSeven(void);
    void plotStrainHistoryRunEight(void);
    void plotStrainHistoryRunNine(void);
    void plotStrainHistoryRunTen(void);

private:

    int createPinching4MaterialUniaxialMaterial(void);

    UniaxialMaterial* theMaterial;

    // Material parameters
    RParameter* KelasParameter;    // Floating point value to define the initial material elastic stiffness (Kelastic); Kelas > 0

    RLimitCurve* theLimitCurve;

    int ndT;     // Integer node tag to define the first node at the extreme end of the associated flexural frame member (L3 or D5 in Figure)
    int ndB;     // Integer node tag to define the last node at the extreme end of the associated flexural frame member (L2 or D2 in Figure)

    Domain* theOSDomain;
    Node *theNodeT;
    Node *theNodeB;

    int driftAxis;

    // Integer flag to indicate the type of limit curve associated with this material.
    // crvTyp = 0 – No limit curve
    // crvTyp = 1 – axial limit curve crvTyp = 2 – RotationShearCurve
    int crvTyp;

    // Note: This value must be between zero and unity
    double YpinchUPN;	// Floating point unloading force pinching factor for loading in the negative direction

    // Note: This value must be between negative one and unity
    double YpinchRPN;	// Floating point reloading force pinching factor for loading in the negative direction

    // Note: This value must be between negative one and unity
    double XpinchRPN;   // Floating point reloading displacement pinching factor for loading in the negative direction

    // Note: This value must be between zero and unity
    double YpinchUNP;	// Floating point unloading force pinching factor for loading in the positive direction

    // Note: This value must be between negative one and unity
    double YpinchRNP;	// Floating point reloading force pinching factor for loading in the positive direction

    // Note: This value must be between negative one and unity
    double XpinchRNP;	// Floating point reloading displacement pinching factor for loading in the positive direction

    // This value is used to compute the maximum deformation at flexural yield (δmax Eq. 1) and using the initial elastic stiffness (Kelastic) the monotonic energy (Emono Eq. 1) to yield. Input 1 if this type of damage is not required and set $dmgE1, $dmgE2, $dmgE3, $dmgE4, and $dmgELim to zero
    double dmgStrsLimE;	// Floating point force limit for elastic stiffness damage (typically defined as the lowest of shear strength or shear at flexrual yielding).

    // This value is used to compute the monotonic energy at axial failure (Emono Eq. 2) by computing the area under the backbone in the positive loading direction up to δmax. Input 1 if this type of damage is not required and set $dmgR1, $dmgR2, $dmgR3, $dmgR4, and $dmgRLim to zero for reloading stiffness damage. Similarly set $dmgS1, $dmgS2, $dmgS3, $dmgS4, and $dmgSLim to zero if reloading strength damage is not required
    double dmgDispMax;	// Floating point for ultimate drift at failure (δmax Eq. 1) and is used for strength and stiffness damage.

    // Floating point elastic stiffness damage factors α1,α2,α3,α4 shown in Eq. 1
    double dmgE1;
    double dmgE2;
    double dmgE3;
    double dmgE4;

    double dmgELim;	// Floating point elastic stiffness damage limit Dlim shown in Eq. 1; Note: This value must be between zero and unity

    // Floating point reloading stiffness damage factors α1,α2,α3,α4 shown in Eq. 1
    double dmgR1;
    double dmgR2;
    double dmgR3;
    double dmgR4;
    double dmgRLim;	// Floating point reloading stiffness damage limit Dlim shown in Eq. 1; Note: This value must be between zero and unity
    double dmgRCyc;	// Floating point cyclic reloading stiffness damage index; Note: This value must be between zero and unity
    // Floating point backbone strength damage factors α1,α2,α3,α4 shown in Eq. 1
    double dmgS1;
    double dmgS2;
    double dmgS3;
    double dmgS4;
    double dmgSLim;	// Floating point backbone strength damage limit Dlim shown in Eq. 1; Note: This value must be between zero and unity
    double dmgSCyc;	// Floating point cyclic backbone strength damage index; Note: This value must be between zero and unity

    void createPlots(const QVector<double> &stresses,const QVector<double> &strains,const QVector<double> &steps,const QString& description);
    void outputMaterialProperties(void);

};




#endif


