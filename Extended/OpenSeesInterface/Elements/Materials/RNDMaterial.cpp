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
#include "RMaterialKnowledge.h"
#include "RUniaxialMaterial.h"

#include "UniaxialMaterial.h"
#include "NDMaterial.h"

RNDMaterial::RNDMaterial(QObject *parent, QString &name)
    : RObject(parent, name)
{
}


RNDMaterial::~RNDMaterial()
{
}


QStringList RNDMaterial::getActionList()
{
    QStringList actionList;

    actionList << "&Plot Stress Strain Relation"
               << "&Plot Strain History Run One"
               << "&Plot Strain History Run Two"
               << "&Plot Strain History Run Three"
               << "&Plot Strain History Run Four"
               << "&Plot Strain History Run Five"
               << "&Plot Strain History Run Six"
               << "&Plot Strain History Run Seven"
               << "&Plot Strain History Run Eight"
               << "&Plot Strain History Run Nine"
               << "&Plot Strain History Run Ten";

    return actionList;
}


void RNDMaterial::plotStressStrainRelation(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RNDMaterial::plotStrainHistoryRunOne(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RNDMaterial::plotStrainHistoryRunTwo(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RNDMaterial::plotStrainHistoryRunThree(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RNDMaterial::plotStrainHistoryRunFour(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}

void RNDMaterial::plotStrainHistoryRunFive(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RNDMaterial::plotStrainHistoryRunSix(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RNDMaterial::plotStrainHistoryRunSeven(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RNDMaterial::plotStrainHistoryRunEight(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RNDMaterial::plotStrainHistoryRunNine(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RNDMaterial::plotStrainHistoryRunTen(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


