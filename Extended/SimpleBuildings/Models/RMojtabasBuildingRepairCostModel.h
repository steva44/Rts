#ifndef RMojtabasBuildingRepairCostModel_H
#define RMojtabasBuildingRepairCostModel_H
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
*   - Mojtaba Mahsuli                                                *
*                                                                    *
*********************************************************************/

#include "RModel.h"
#include "RContinuousRandomVariable.h"
#include "RBIM.h"

class RParameter;
class RResponse;

class RMojtabasBuildingRepairCostModel : public RModel
{
	Q_OBJECT	
		
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
    Q_PROPERTY(RParameter *StructuralDamageRatio READ getStructuralDamageRatio WRITE setStructuralDamageRatio)
    Q_PROPERTY(RParameter *NonstructuralDriftDamageRatio READ getNonstructuralDriftDamageRatio WRITE setNonstructuralDriftDamageRatio)
    Q_PROPERTY(RParameter *NonstructuralAccelerationDamageRatio READ getNonstructuralAccelerationDamageRatio WRITE setNonstructuralAccelerationDamageRatio)
    Q_PROPERTY(RParameter *ContentDamageRatio READ getContentDamageRatio WRITE setContentDamageRatio)

public:
	RMojtabasBuildingRepairCostModel(QObject *parent, QString name);
	~RMojtabasBuildingRepairCostModel();

    QObject *getBIM() const;
    void setBIM(QObject *value);

    RParameter *getStructuralDamageRatio() const;
    void setStructuralDamageRatio(RParameter *value);

    RParameter *getNonstructuralDriftDamageRatio() const;
    void setNonstructuralDriftDamageRatio(RParameter *value);

    RParameter *getNonstructuralAccelerationDamageRatio() const;
    void setNonstructuralAccelerationDamageRatio(RParameter *value);

    RParameter *getContentDamageRatio() const;
    void setContentDamageRatio(RParameter *value);

    int evaluateModel(RGradientType theGradientType);

private:

    QPointer<RBIM> theBIM = nullptr;
    RParameter *theStructuralDamageRatio = nullptr;
    RParameter *theNonstructuralDriftDamageRatio = nullptr;
    RParameter *theNonstructuralAccelerationDamageRatio = nullptr;
    RParameter *theContentDamageRatio = nullptr;
    QPointer<RContinuousRandomVariable> theLossEpsilon;
    RResponse *theResponse;

};

#endif
