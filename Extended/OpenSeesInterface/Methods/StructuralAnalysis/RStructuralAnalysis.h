#ifndef RStructuralAnalysis_H
#define RStructuralAnalysis_H
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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RMethod.h"
#include "RStructuralAnalysisModel.h"

#include <functional>

class RVisualDamageModel;
class RStevesRepairImpactModel;

class RStructuralAnalysis : public RMethod
{
    Q_OBJECT

    Q_PROPERTY(QObject* VisualDamageModel READ getVisualDamageModel WRITE setVisualDamageModel)

public:
    RStructuralAnalysis(QObject *parent, QString name);
    virtual ~RStructuralAnalysis();

    virtual int setRecorders(RStructuralAnalysisModel *theStructuralModel, QList<QPointer<RObject> > passedStructuralObjects, QStringList passedResponses);
    virtual int conductStructuralAnalysis() = 0;
    virtual AnalysisModel *getTheAnalysisModel() const;
    virtual void setTheAnalysisModel(AnalysisModel *value);

    virtual void reset(void);

    double getMaxDisplacement() const;

    QObject *getVisualDamageModel() const;
    void setVisualDamageModel(QObject *value);

    QObject *getRepairImpactModel() const;
    void setRepairImpactModel(QObject *value);

    std::shared_ptr<std::function<void(double&)>> getCallback() const;
    void setCallback(std::shared_ptr<std::function<void(double&)>> &value);

protected:

    AnalysisModel *theAnalysisModel;

    RVisualDamageModel* theVisualDamageModel;

    std::shared_ptr<std::function<void(double&)>> callback;

    double maxDisplacement;

};

#endif
