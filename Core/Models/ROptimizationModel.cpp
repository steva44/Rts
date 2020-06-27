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
*   - Terje Haukaas                                                  *
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "ROptimizationModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "ROptimizationAlgorithm.h"
#include "RMatrixOperations.h"

ROptimizationModel::ROptimizationModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    isOrchestratingModel = true;
    theOptimizationAlgorithm = nullptr;
    theObjective = nullptr;
    theResponse = nullptr;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


ROptimizationModel::~ROptimizationModel()
{

}


QStringList ROptimizationModel::getActionList()
{
    QStringList actionList;

    actionList << "&Run"
               << "Separator"
               << "&Help";

    return actionList;
}


QObject * ROptimizationModel::getObjective() const
{	    
    return theObjective;
}	


void ROptimizationModel::setObjective(QObject *value)
{	
    theObjective = qobject_cast<RParameter *>(value);
}	


QObject * ROptimizationModel::getOptimizationAlgorithm() const
{
    return theOptimizationAlgorithm;
}


void ROptimizationModel::setOptimizationAlgorithm(QObject *value)
{
    theOptimizationAlgorithm = qobject_cast<ROptimizationAlgorithm *>(value);
}


int ROptimizationModel::evaluateModel(RGradientType theGradientType)
{
    return theOptimizationAlgorithm->solveOptimizationProblem(theObjective);
}





