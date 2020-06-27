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

#include "RStructuralAnalysis.h"
#include "RVisualDamageModel.h"
#include "RStevesRepairImpactModel.h"

RStructuralAnalysis::RStructuralAnalysis(QObject *parent, QString name)
    : RMethod(parent, name)
{

    theAnalysisModel = nullptr;
    callback = nullptr;
    theVisualDamageModel = nullptr;

    maxDisplacement = 0.0;
}


RStructuralAnalysis::~RStructuralAnalysis()
{

}


void RStructuralAnalysis::reset(void)
{
    return;
}


int RStructuralAnalysis::setRecorders(RStructuralAnalysisModel *theStructuralModel, QList<QPointer<RObject> > theStructuralObjects, QStringList theResponses)
{
    return 0;
}


AnalysisModel *RStructuralAnalysis::getTheAnalysisModel() const
{
    return theAnalysisModel;
}


void RStructuralAnalysis::setTheAnalysisModel(AnalysisModel *value)
{
    theAnalysisModel = value;
}


double RStructuralAnalysis::getMaxDisplacement() const
{
    return maxDisplacement;
}


QObject *RStructuralAnalysis::getVisualDamageModel() const
{
    return theVisualDamageModel;
}


void RStructuralAnalysis::setVisualDamageModel(QObject *value)
{
    theVisualDamageModel  = qobject_cast<RVisualDamageModel*>(value);

}


std::shared_ptr<std::function<void(double&)>> RStructuralAnalysis::getCallback() const
{
    return callback;
}


void RStructuralAnalysis::setCallback(std::shared_ptr<std::function<void(double&)>> &value)
{
    callback = value;
}



